/*
	Copyright (C) 2016-2017 Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of 'MMUAq' software.

    'MMUAq' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'MMUAq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'MMUAq'.  If not, see <http://www.gnu.org/licenses/>.
*/

// MUA Internet Message Access protocol (IMAP 4rev1 - RFC 3501 & 5530).

#ifndef MUAIMA__INC
# define MUAIMA__INC

# ifdef XXX_DBG
#	define MUAIMA__DBG
# endif

# include "muaimabs.h"

namespace muaima {
	using namespace muaimabs;

	class cValue_
	{
	protected:
		virtual void MUAIMAOnEOF( void ) = 0;
	public:
		qCALLBACK( Value_ );
		void OnEOF( void )
		{
			return MUAIMAOnEOF();
		}
	};

	class rValueDriver_
	: public rDriver_
	{
	public:
		rDriverBase_ Base_;
		flw::sDressedIFlow<> Flow_;
		cValue_ *Callback_;
		bso::sBool EOFHandled_;
	protected:
		virtual fdr::sSize FDRRead(
			fdr::sSize Maximum,
			fdr::sByte* Buffer ) override
		{
			fdr::sSize Amount = Base_.Read( Maximum, Buffer );

			if ( Base_.EndOfFlow() ) {
				if ( !EOFHandled_  ) {
					EOFHandled_ = true;
					if ( Callback_ != NULL )
						Callback_->OnEOF();
				}
			} else
				EOFHandled_ = false;

			return Amount;
		}
		virtual void FDRDismiss( bso::sBool Unlock ) override
		{
			return Base_.Dismiss( Unlock );
		}
		virtual fdr::sTID FDRITake( fdr::sTID Owner ) override
		{
			return Base_.ITake( Owner );
		}
	public:
		void reset( bso::sBool P = true )
		{
			rDriver_::reset( P  );
			tol::reset( P, Base_, Flow_, Callback_, EOFHandled_ );
		}
		qCVDTOR( rValueDriver_ );
		void Init(
			flw::sIFlow &Flow,
			eDelimiter Delimiter,
			cValue_ *Callback )
		{
			rDriver_::Init( fdr::ts_Default );
			Base_.Init( Flow, str::wString(), Delimiter );
			Callback_ = Callback;
			EOFHandled_ = false;
		}
		void Init(
			fdr::rIDriver &Driver,
			eDelimiter Delimiter,
			cValue_ *Callback )
		{
			rDriver_::Init( fdr::ts_Default );
			Flow_.Init( Driver );
			Base_.Init( Flow_, str::wString(), Delimiter );
			Callback_ = Callback;
		}
	};

	// Handles items pairs (
	namespace  item {
		qENUM( Name ) {
			nBody,
			nBodyWithSection,
			nBodyStructure,
			nEnvelope,
			nFlags,
			nInternalDate,
			nRFC822,
			nRFC822_Header,
			nRFC822_Size,
			nRFC822_Text,
			nUID,
			n_amount,
			n_None,
			n_Undefined,
		};

		const char *GetLabel( eName Name );
	}

	class cResponse_
	{
	protected:
		virtual void MUAIMAOnResponse(
			eResponseCode Code,
			fdr::rIDriver &Driver) = 0;
	public:
		qCALLBACK( Response_ );
		void OnResponse(
			eResponseCode Code,
			fdr::rIDriver &Driver )
		{
			return MUAIMAOnResponse( Code, Driver );
		}
	};

	class cFolders
	{
	protected:
		virtual void MUAIMAOnFolder( const str::dString &Name ) = 0;
	public:
		qCALLBACK( Folders );
		void OnFolder( const str::dString &Name )
		{
			return MUAIMAOnFolder( Name );
		}
	};

	namespace get_mail_ {
		class sRFC822ValueCallback_
		: public cValue_
		{
		private:
			qRMV( fdr::rIDriver, D_, Driver_ );
		protected:
			virtual void MUAIMAOnEOF( void ) override
			{
				fdr::Purge( D_() );
			}
		public:
			void reset( bso::sBool P = true )
			{
				tol::reset( P, Driver_ );
			}
			qCVDTOR( sRFC822ValueCallback_ );
			void Init( fdr::rIDriver &Driver )
			{
				Driver_ = &Driver;
			}
		};


		class sFetchResponseValueCallback_
		: public cValue_
		{
		private:
			qRMV( rConsole, C_, Console_ );
		protected:
			virtual void MUAIMAOnEOF( void ) override
			{
				if ( C_().SkipRemainingReponses() != sOK )
					qRGnr();

				C_().SkipResponse();
			}
		public:
			void reset( bso::sBool P = true )
			{
				tol::reset( P, Console_ );
			}
			qCVDTOR( sFetchResponseValueCallback_ );
			void Init( rConsole &Console )
			{
				Console_ = &Console;
			}
		};

		class rRack
		{
		private:
			rValueDriver_ FetchValueDriver_, ItemsValueDriver_, RFC822ValueDriver_;
			sFetchResponseValueCallback_ FetchResponseValueCallback_;
			sRFC822ValueCallback_ ItemsValueCallback_, RFC822ValueCallback_;
			void GetValue_( void );
		public:
			void reset( bso::sBool P = true )
			{
				tol::reset( P, FetchValueDriver_, ItemsValueDriver_, RFC822ValueDriver_, FetchResponseValueCallback_, ItemsValueCallback_, RFC822ValueCallback_ );
			}
			qCDTOR( rRack )
			void Init( rConsole &Console )
			{
				FetchResponseValueCallback_.Init( Console );
				FetchValueDriver_.Init(Console.GetResponseDriver(), dCRLF, &FetchResponseValueCallback_ );
				GetValue_();

				RFC822ValueCallback_.Init( ItemsValueDriver_ );
				RFC822ValueDriver_.Init( ItemsValueDriver_, dNone, &RFC822ValueCallback_ );
			}
			fdr::rIDriver *operator()( void )
			{
				return &RFC822ValueDriver_;
			}
			
		};
	}

	class rSession
	{
	private:
		rConsole Console_;
		qRMV( fdr::rIDriver, VD_, ValueDriver_ );
		bso::sBool Connected_;
		bso::sByte Delimiter_;	// The hierarchy delimiter. '0' means no demimiter (hope that '0' is not a valid delmimiter).
		str::wString Message_;
		get_mail_::rRack GetMailRack_;
		void RetrieveMessage_( void );
		eStatus HandleStatus_(
			eStatus Status,
			qRPN );
		eStatus HandleStatus_( qRPN )
		{
			return HandleStatus_(Console_.GetStatus(), qRP );
		}
		eStatus HandleResponses_(
			cResponse_ &ReponseCallback,
			qRPN );
		eStatus Connect_(
			const str::dString &Username,
			const str::dString &Password,
			qRPN );
		eStatus FetchHierarchyDelimiter_( qRPN );
		eStatus Disconnect_( qRPN );
	public:
		void reset( bso::sBool P = true )
		{
			if ( P ) {
				if ( Connected_ )
					Disconnect_( qRPU ); // We don't care if it fails.
			}

			tol::reset( P, Console_, ValueDriver_, Connected_, Message_, GetMailRack_ );
			Delimiter_ = 0;
		}
		qCDTOR( rSession );
		eStatus Init(
			fdr::rIODriver &Driver,
			const str::dString &Username,
			const str::dString &Password,
			qRPD )
		{
			eStatus Status = s_Undefined;

			Message_.Init();

			if ( Connected_ )
				Status = Disconnect_( qRPU );	// We don't care if it fails.

			Connected_ = false;

			Console_.Init( Driver );

			Status = Connect_( Username, Password, qRP );

			if ( Status == sOK ) {
				Connected_ = true;
				Status = FetchHierarchyDelimiter_( qRP );
			}

			if ( Status != sOK ) {
				if ( qRPT )
					qRGnr();
			}

			// The racks and 'ValueDriver_' will be initalized/set as needed.

			return Status;
		}
		rConsole &Console( void )
		{
			return Console_;
		}
		bso::sByte Delimiter( void ) const
		{
			if ( Delimiter_ == 0 )
				qRGnr();

			return Delimiter_;
		}
		eStatus GetFolders(
			const str::dString &Folder,
			class rFolders &Folders,
			qRPD );
		/* Commands after which, on success, you have to handle 'GetValueDriver(...)'*/
		eStatus GetMail(
			const str::dString &Folder,
			bso::sUInt Number,
			qRPD );
		/* End of commands after which, on success, you have to handle 'GetValueDriver(...)'*/
		fdr::rIDriver &GetValueDriver( void )
		{
			fdr::rIDriver &ValueDriver = VD_();

			ValueDriver_ = NULL;

			return ValueDriver;
		}
	};

	class rFolders
	{
	private:
		qRMV( rSession, S_, Session_ );
		rConsole &C_( void )
		{
			return S_().Console();
		}
		void Init_( rSession &Session )
		{
			Session_ = &Session;
		}
		void GetFolderName_( str::dString &Folder );
	public:
		void reset( bso::sBool P = true )
		{
			tol::reset( P, Session_ );
		}
		qCDTOR( rFolders );
		void Init( void )
		{
			reset();

			// Memebers will bi itialzed as needed.
		}
		// Call until returns 'false'.
		bso::sBool GetFolder( str::dString &Folder )
		{
			eResponseCode Code = rc_Undefined;

			while ( ( ( Code = C_().GetPendingResponseCode() ) != rc_None ) && ( Code != rcList ) )
				C_().SkipResponse();

			if ( Code == rcList ) {
				GetFolderName_( Folder );
				return true;
			} else
				return false;
		}
		friend rSession;
	};

}


#endif
