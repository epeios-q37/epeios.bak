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

# include "muabsc.h"

namespace muaima {
	typedef fdr::rIDressedDriver rDriver_;
	typedef flx::sStringIFlow sSIFlow_;

	class rPendingIFlow_
	: public sSIFlow_
	{
	private:
		str::wString Data_;
	public:
		void reset( bso::sBool P = true )
		{
			sSIFlow_::reset( P );
			tol::reset( P, Data_ );
		}
		qCDTOR( rPendingIFlow_ );
		void Init( const str::dString &Data )
		{
			Data_.Init( Data );
			sSIFlow_::Init( Data_ );
		}
		bso::sBool IsEmpty( void )
		{
			return ( Data_.Amount() == 0 ) || (sSIFlow_::EndOfFlow() );
		}
	};

	class rResponseDriver_
	: public rDriver_
	{
	private:
		qRMV( flw::sIFlow, F_, Flow_ );
		bso::sBool
			EOF_,
			BracketIsEOF_;	// At true if a square backet ']' means a EOF.
		rPendingIFlow_ Pending_;
	protected:
		virtual fdr::sSize FDRRead(
			fdr::sSize Maximum,
			fdr::sByte* Buffer ) override
		{
			bso::sBool Continue = true;
			fdr::sByte Byte = 0;
			fdr::sSize Amount = 0;

			flw::sIFlow &Flow = F_();

			if ( !EOF_ ) {
				while ( Continue ) {
					if ( Pending_.IsEmpty() ) {
						if ( F_().EndOfFlow() )
							qRGnr();

						Byte = Flow.Get();

						if ( ( Byte == '\r' )
							 || ( BracketIsEOF_ && ( Byte ==']' ) ) )
						{
							Continue = false;
							EOF_ = true;
							BracketIsEOF_ = false;

							if ( Flow.EndOfFlow() )
								qRGnr();

							if ( Byte == ']' ) {
								if ( Flow.View() == ' ' )
									Flow.Skip();
								else if ( Flow.View() == '\r' ) {
									if ( Flow.EndOfFlow() || ( Flow.Get() != '\n' ) )
										qRGnr();
								}
							} else if ( Flow.EndOfFlow() || ( Flow.Get() != '\n' ) )
								qRGnr();
						} else {
							Buffer[Amount++] = Byte;

							if ( Amount == Maximum )
								Continue = false;
						}
					} else
						Amount = Pending_.ReadUpTo( Maximum, Buffer );
				}
			}
			return Amount;
		}
		virtual void FDRDismiss( bso::sBool Unlock ) override
		{
			F_().Dismiss( Unlock );
		}
		virtual fdr::sTID FDRITake( fdr::sTID Owner ) override
		{
			return F_().IDriver().ITake( Owner );
		}
	public:
		void reset( bso::sBool P = true )
		{
			rDriver_::reset( P );
			tol::reset( P, Flow_, EOF_, BracketIsEOF_, Pending_ );
		}
		qCVDTOR( rResponseDriver_ );
		void Init(
			flw::sIFlow &Flow,
			const str::dString &PendingData,
			bso::sBool BracketIsEOF )
		{
			EOF_ = false;
			BracketIsEOF_ = BracketIsEOF;
			Flow_ = &Flow;
			rDriver_::Init( fdr::ts_Default );
			Pending_.Init( PendingData );
		}
	};

	// Response code.
	qENUM( Code ) {
		// Not really response code, but 
		cOK,
		cNo,
		cBad,
		cPreAuth,
		cBye,
		// Response codes which may optionnaly be contained in status response.
		cAlert,
		cBadCharSet,
		cCapability,
		cParse,
		cPermanentFlags,
		cReadOnly,
		cReadWrite,
		cTryCreate,
		cUIDNext,
		cUIDValidity,
		cUnseen,
		// Below response codes are from RFC 55530.
		cUnavailable,
		cAuthenticationFailed,
		cAuthorizationFailed,
		cExpired,
		cPrivacyRequired,
		cContactAdmin,
		cNoPerm,
		cInUse,
		cExpungeIssued,
		cCorruption,
		cServerBug,
		cClientBug,
		cCanNot,
		cLimit,
		cOverQuota,
		cAlreadyExists,
		cNonExistent,
		// Reponses to commands.
		// cCapability, // Already present above.
		cList,
		cLSub,
		cStatus,
		cSearch,
		cFlags,
		cExists,
		cRecent,
		cExpunge,
		cFetch,
		c_amount,
		c_None,
		c_Undefined
	};

	const char *GetLabel( eCode Code );

	qENUM( Status ) {
		sOK,
		sNO,
		sBAD,
		s_amount,
		s_Pending,	// Status pending ; other response have to be handled before obtining the status.
		s_Erroneous,	// Server returned a not 'IMAP' compliant answer.
		s_Undefined
	};

	const char *GetLabel( eStatus Status );

	class rConsole
	{
	private:
		str::wString Tag_;
		flw::sDressedIFlow<> IFlow_;
		txf::rOFlow OFlow_;
		rResponseDriver_ ResponseDriver_;
		eCode PendingCode_;
		bso::sBool PendingCodeIsStatus_;
		bso::sBool NoTaggedStatusResponse_;	// To handle the connection, where there is no tagged status response.
	public:
		void reset( bso::sBool P = true )
		{
			tol::reset( P, Tag_, IFlow_, OFlow_, PendingCodeIsStatus_, ResponseDriver_, NoTaggedStatusResponse_ );
			PendingCode_ = c_Undefined;
		}
		qCDTOR( rConsole );
		void Init( fdr::rIODriver &Driver )
		{
			tol::Init( Tag_ );
			PendingCode_ = c_Undefined;
			IFlow_.Init( Driver );
			OFlow_.Init( Driver );
			PendingCodeIsStatus_ = false;
			NoTaggedStatusResponse_ = false;
			// 'ReponseDriver_' will be initialized as needed.
		}
		void ReportUntaggedStatusResponse( void )	// To handle the connection, where there is no tagged status response.
		{
			NoTaggedStatusResponse_ = true;
		}
		const str::dString &GetNextTag( void )
		{
			return muabsc::GetNextIMAPTag( Tag_ );
		}
		const str::dString &GetCurrentTag( void ) const
		{
			if ( Tag_.Amount() == 0)
				qRGnr();

			return Tag_;
		}
		flw::sIFlow &IFlow( void )
		{
			return IFlow_;
		}
		txf::sOFlow &OFlow( void )
		{
			return OFlow_;
		}
		eCode GetCode( void );
		fdr::rIDriver &GetResponseDriver( void )
		{
			return ResponseDriver_;
		}
		void SkipResponse( void )
		{
		qRH
			flw::sDressedIFlow<> Flow;
		qRB
			Flow.Init( GetResponseDriver() );

			while ( !Flow.EndOfFlow() )
				Flow.Skip();
		qRR
		qRT
		qRE
		}
		// Also resets the PendingStatus.
		eStatus GetPendingStatus( void )
		{
			// Althought 'PendingCode' is a status, the 'PendingCodeIsStatus_' is already set to false by 'GetCode(...').
			eStatus Status = s_Undefined;

			switch ( PendingCode_ ) {
			case cOK:
				Status = sOK;
				break;
			case cNo:
				Status = sNO;
				break;
			case cBad:
				Status = sBAD;
				break;
			default:
				qRGnr();
				break;
			}

			PendingCode_ = c_Undefined;

			return Status;
		}
	};

	/*
		First, call 'Connect(...)', then 'Login(...)' (technically, it's not mandatory
		but if you dont, you would not be able to do very much), and end with 
		'Logout(...)'. Between 'Login(....)' and 'Logout(...)', call all other
		functions as needed.

		For all this functions, when they return 's_Pending', call 'rConsole.GetCode(...)',
		then handle 'rConsole::GetResponseDriver(...)' or call 'rConsole::SkipReponse(...)'
		until 'rConsole.GetCode(...)' returns 'c_None'.	Then Call the
		'GetCompletionStatus(...)',	then handle 'rConsole::GetResponseDriver(...)'
		or call 'rConsole::SkipReponse(...)'.

		If one of this functions doesn't return 's_Pending', then handle
		'rConsole::GetResponseDriver(...)' or call 'rConsole::SkipReponse(...)'.
	*/


	// This is the first command to call after opening a connection to the 'IMAP' server.
	eStatus Connect( rConsole &Console );

	// To log in. Most commands are not available when this is skipped or not done successfully.
	eStatus Login(
		const str::dString &Username,
		const str::dString &Password,
		rConsole &Console );

	// To call just before closing the connexion. You do _not_ need to be logged in to log out.
	eStatus Logout( rConsole &Console );

	// Launches the corresponding 'IMAP' command.
	eStatus Capability( rConsole &Console );
	eStatus Select(
		const str::dString &Mailbox,
		rConsole &Console );
	eStatus List(
		const str::dString &Reference,
		const str::dString &Mailbox,
		rConsole &Console );
	eStatus LSub(
		const str::dString &Reference,
		const str::dString &Mailbox,
		rConsole &Console );

	// To call after 'rConsole::GetCode(...)' return 'c_None'.
	eStatus GetCompletionStatus( rConsole &Console );

	class cResponse_
	{
	protected:
		virtual void MUAIMAOnResponse(
			eCode Code,
			fdr::rIDriver &Driver) = 0;
	public:
		qCALLBACK( Response_ );
		void OnReponse(
			eCode Code,
			fdr::rIDriver &Driver )
		{
			return MUAIMAOnResponse( Code, Driver );
		}
	};

	class rSession
	{
	private:
		rConsole Console_;
		bso::sBool Connected_;
		bso::sByte Delimiter_;	// The hierarchy delimiter. '0' means no demimiter (hope that '0' is not a valid delmimiter).
		str::wString Message_;
		void RetrieveMessage_( void );
		eStatus HandleCompletion_(
			eStatus Status,
			qRPN );
		eStatus HandlePending_(
			cResponse_ &ReponseCallback,
			qRPN );
		eStatus Handle_(
			eStatus Status,
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

			tol::reset( P, Console_, Connected_, Message_ );
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

			Console_.Init( Driver );

			Status = Connect_( Username, Password, qRP );

			if ( Status == sOK )
				Status = FetchHierarchyDelimiter_( qRP );

			if ( Status != sOK ) {
				if ( qRPT )
					qRGnr();
			}

			return Status;
		}
	};
}


#endif
