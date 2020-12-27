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

#ifndef MUAIMA_INC_
# define MUAIMA_INC_

# ifdef XXX_DBG
#	define MUAIMA__DBG
# endif

# include "muaimabs.h"

namespace muaima {
	using namespace muaimabs;

	// Handles items pairs (
	namespace item {
		qENUM( Name ) {
			nBody,
			nBodyWithSection,
			nBodyStructure,
			nEnvelope,
			nFlags,
			nInternalDate,
			n_RFC822First,
			nRFC822 = n_RFC822First,
			nRFC822Header,
			nRFC822Size,
			nRFC822Text,
			n_RFC822Last = nRFC822Text,
			nUID,
			n_amount,
			n_None,
			n_Undefined,
		};

		const char *GetLabel( eName Name );

		inline bso::sBool IsRFC822( eName Name )
		{
			return (Name >= n_RFC822First) && (Name <= n_RFC822Last);
		}
	}

	qENUM( RFC822Part ) {
		rpAll,
		rpSize,
		rpHeader,
		rpText,
		rp_amount,
		rp_Undefined
	};

	extern str::wString CurrentFolder;	// Can be given as the 'Folder' argument, if a successful 'rSeesion::Select(...)' was made bofore.

	class rSession
	{
	private:
		rConsole Console_;
		bso::sBool Connected_;
		bso::sByte Delimiter_;	// The hierarchy delimiter. '0' means no demimiter (hope that '0' is not a valid delmimiter).
		eStatus PendingStatus_;
		str::wString PendingMessage_;
		// If 'Message' == NULL, skips the message.
		void RetrieveMessage_( str::dString *Message );
		eStatus PurgeResponses_(
			str::dString *Message,
			qRPN );
		eStatus Connect_(
			const str::dString &Username,
			const str::dString &Password,
			str::dString *Message,
			qRPN );
		eStatus FetchHierarchyDelimiter_(
			str::dString *Message,
			qRPN );
		eStatus Disconnect_(
			str::dString *Message,
			qRPN );
		bso::sBool GetFetchItem_(
			item::eName ItemName,
			eFlavor Flavor,
			const str::dString &Folder,
			bso::sUInt Number,
			class rFetch_ &Fetch,
			qRPN );
	public:
		void reset( bso::sBool P = true )
		{
			if ( P ) {
				if ( Connected_ ) {
					Connected_ = false;	// To avoid that a 'Disconnect_(...)' will be recalled when an error occured during 'Disconnect_(...)'.
					Disconnect_( NULL, qRPU ); // We don't care if it fails.
				}
			}

			tol::reset( P, Console_, Connected_, PendingMessage_ );
			Delimiter_ = 0;
			PendingStatus_ = s_Undefined;
		}
		qCDTOR( rSession );
		eStatus Init(
			fdr::rRWDriver &Driver,
			const str::dString &Username,
			const str::dString &Password,
			str::dString *Message = NULL,
			qRPD )
		{
			eStatus Status = s_Undefined;

			if ( Connected_ )
				Status = Disconnect_( NULL, qRPU );	// We don't care if it fails.

			Connected_ = false;

			Console_.Init( Driver );

			Status = Connect_( Username, Password, Message, qRP );

			if ( Status == sOK ) {
				Connected_ = true;
				Status = FetchHierarchyDelimiter_( Message, qRP );
			}

			if ( Status != sOK ) {
				if ( qRPT )
					qRGnr();
			}

			PendingStatus_ = s_Undefined;
			PendingMessage_.Init();

			// The racks and 'ValueDriver_' will be initalized/set as needed.

			return Status;
		}
		eStatus Init(
			fdr::rRWDriver &Driver,
			const str::dString &Username,
			const str::dString &Password,
			str::dString &Message,
			qRPD )
		{
			return Init( Driver, Username, Password, &Message, qRP );
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
		bso::sBool Select(
			const str::dString &Folder,
			qRPD );
		/* Calling a method which implies a folder will 'select' this folder, so 'CurrentFolder' can be use as 'Folder'
		for the other methods. */
		bso::sBool GetMailAmount(
			const str::dString &Folder,	// Can NOT be 'CurrentFolder', as to retrieve the amount a 'Select' must be made.
			bso::sUInt &Amount,
			qRPD );
		bso::sBool GetFolders(
			const str::dString &Folder,	// Can be 'CurrentFolder' if a successful 'Select(...)' was made before.
			class rFolders &Folders,
				qRPD );
		// If returns false and 'EndStatus(...)' returns 'sOK', it means that here is no corresponding mail.
		bso::sBool GetRFC822(
			eRFC822Part Part,
			eFlavor Flavor,
			const str::dString &Folder,	// Can be 'CurrentFolder' if a successful 'Select(...)' was made before.
			bso::sUInt Number,
			class rRFC822 &RFC822,
			qRPD );
		bso::sBool GetUID(
			const str::dString &Folder,	// Can be 'CurrentFolder' if a successful 'Select(...)' was made before.
			bso::sUInt Number,
			class rUID &UID,
			qRPD );
		eStatus EndStatus( str::dString *Message = NULL )
		{
			eStatus Status = PendingStatus_;

			if ( Status != s_Undefined ) {
				if ( Message != NULL )
					Message->Append( PendingMessage_ );

				PendingStatus_ = s_Undefined;
				PendingMessage_.Init();
			} else {
				Status = Console_.GetStatus();
				RetrieveMessage_( Message );
			}

			return Status;
		}
		eStatus EndStatus( str::dString &Message  )
		{
			return EndStatus( &Message );
		}
	};

	class rBase_
	{
	protected:
		qRMV( rSession, S_, Session_ );
		rConsole &C_( void )
		{
			return S_().Console();
		}
		void Init_( rSession &Session )
		{
			Session_ = &Session;
		}
		virtual void MUAIMAOnEnd( void )
		{}
	public:
		void reset( bso::sBool P = true )
		{
			tol::reset( P, Session_ );
		}
		qCDTOR( rBase_ );
		void Init( void )
		{
			reset();

			// Memebers will be initialized as needed.
		}
		eStatus EndStatus( str::dString *Message  = NULL)
		{
			MUAIMAOnEnd();
			return S_().EndStatus( Message );
		}
		eStatus EndStatus( str::dString &Message )
		{
			return EndStatus( &Message );
		}
		friend rSession;
	};

	class rFolders
	: public rBase_
	{
	private:
		void GetFolderName_( str::dString &Folder );
	public:
		void reset( bso::sBool P = true )
		{
			rBase_::reset( P );
		}
		qCVDTOR( rFolders );
		void Init( void )
		{
			rBase_::Init();
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

	class rFetch_
	: public rBase_
	{
	private:
		rResponseDriver_
			Global_,	// Handle the global response,
			Items_,	// Handles all the items (delimited by '()').
			Content_;	// Handles item value.
		void GetValue_( item::eName ItemName );
	protected:
		virtual void MUAIMAOnEnd( void ) override
		{
			Content_.Purge();
			Items_.Purge();
			Global_.Purge();
			C_().SkipRemainingReponses();
		}
		void Init_(
			item::eName ItemName,
			rSession &Session )
		{
			rBase_::Init_( Session );

			Global_.Init( C_().GetResponseDriver(), dCRLF );
			Items_.Init( Global_, dNone );
			Content_.Init( Items_, dNone );

			GetValue_( ItemName );
		}
	public:
		void reset( bso::sBool P = true )
		{
			rBase_::reset( P );
		}
		qCVDTOR( rFetch_ );
		void Init( void )
		{
			rBase_::Init();

			// Other members will be initilaized as needed.
		}
		fdr::rRDriver &GetDriver( void )
		{
			return Content_;
		}
		friend rSession;
	};

	qTCLONE( rFetch_, rRFC822 );
	qTCLONE( rFetch_, rUID );
}


#endif
