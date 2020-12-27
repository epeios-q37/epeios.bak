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

// MUA IMA(P) BaSe

#ifndef MUAIMABS_INC_
# define MUAIMABS_INC_

# ifdef XXX_DBG
#	define MUAXMP__DBG
# endif

# include "muabsc.h"

namespace muaimabs {
	typedef fdr::rRDressedDriver rDriver_;
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

	// Main delimiter, which is dicarded ; only the content is returned.
	qENUM( Delimiter ) {
		dNone,			// No delimiter detected yet.
		dCRLF,			// To read the entire message until 'CRLF' of an status response.
		dBracket,		// '[' : for optional response code.
		dParenthesis,	// '(' : parenthisized list.
		dQuote,			// Quoted ('"') string.
		dLiteral,		// Literal ('{') string.
//		dSpace,			// Separation of basic strings. Can be a space, CRLF, EOF.
		d_amount,
		d_Undefined
	};

	namespace _ {
		// Delimiters are re-issued.
		qENUM( Context ) {
			cFree,
			cQuoted,	// Quoted string.
			cLiteral,	// Reading literal string _size_. The content itself is handled through 'Force_'.
			cEOF,		// All data red;
			// No parenthensis, as 'Level_' is enough to inform about.
			// No bracket corresponding item, because it's always a root delimiter; or should be handled as regular char.
			c_amount,
			c_Undefined
		};
	}

	inline void SkipCRLF_( flw::rRFlow &Flow )
	{
		if ( Flow.Get() != '\r' )
			qRGnr();

		if ( Flow.Get() != '\n' )
			qRGnr();
	}

	class rDriverBase_
	{
	private:
		qRMV( flw::rRFlow, F_, Flow_ );
		eDelimiter Delimiter_;
		_::eContext Context_;
		rPendingIFlow_ Pending_;
		bso::sSize Level_;	// Parenthensis level.
		bso::sSize Force_;	// Remaining amount of byte to put without taking care of value.
		bso::sBool HandleFreeContext_(
			fdr::sByte Byte,
			flw::rRFlow &Flow,
			fdr::sByte *Buffer,
			fdr::sSize &Amount );
		void HandleQuotedContext_(
			fdr::sByte Byte,
			flw::rRFlow &Flow,
			fdr::sByte *Buffer,
			fdr::sSize &Amount );
		void HandleLiteralContext_(
			fdr::sByte Byte,
			flw::rRFlow &Flow,
			fdr::sByte *Buffer,
			fdr::sSize &Amount );
		void HandleContext_(
			flw::rRFlow &Flow,
			fdr::sByte *Buffer,
			fdr::sSize &Amount );
	public:
		void reset( bso::sBool P = true )
		{
			tol::reset( P, Flow_, Pending_ );
			Delimiter_ = d_Undefined;
			Context_ = _::c_Undefined,
			Force_ = 0;
			Level_ = 0;
		}
		qCVDTOR( rDriverBase_ );
		void Init(
			flw::rRFlow &Flow,
			const str::dString &PendingData,
			eDelimiter Delimiter )
		{
			Delimiter_ = Delimiter;
			Context_ = _::cFree;
			Flow_ = &Flow;
			Pending_.Init( PendingData );
			Force_ = 0;
			Level_ = 0;
		}
		fdr::sSize Read(
			fdr::sSize Maximum,
			fdr::sByte *Buffer );
		bso::sBool Dismiss(
			bso::sBool Unlock,
			qRPN )
		{
//			F_().Dismiss( Unlock, qRP );

				return true;
		}
		fdr::sTID RTake( fdr::sTID Owner )
		{
			return F_().RDriver().RTake( Owner );
		}
		bso::sBool EndOfFlow( void )
		{
			return Context_ == _::cEOF;
		}
	};

	class rResponseDriver_
	: public rDriver_
	{
	private:
		rDriverBase_ Base_;
		flw::rDressedRFlow<> Flow_;
	protected:
		virtual fdr::sSize FDRRead(
			fdr::sSize Maximum,
			fdr::sByte* Buffer ) override
		{
			return Base_.Read( Maximum, Buffer );
		}
		virtual bso::sBool FDRDismiss(
			bso::sBool Unlock,
			qRPN) override
		{
			return Base_.Dismiss(Unlock, qRP);
		}
		virtual fdr::sTID FDRRTake(fdr::sTID Owner) override
		{
			return Base_.RTake( Owner );
		}
	public:
		void reset( bso::sBool P = true )
		{
			rDriver_::reset( P );
			tol::reset( P, Base_, Flow_ );
		}
		qCVDTOR( rResponseDriver_ );
		void Init(
			flw::rRFlow &Flow,
			eDelimiter Delimiter,
			const str::dString &PendingData = str::wString() )
		{
			Base_.Init( Flow, PendingData, Delimiter );
			rDriver_::Init( fdr::ts_Default );
			// The object 'Init(...)' several times during its lifespan, so the 'Dismissed', which drives the 'verbose' driver, is sometines called to late.
			// By calling below method, 'Dimissed()' is called each time EOF is reached.
			rDriver_::SetAutoDismissOnEOF();
		}
		void Init(
			fdr::rRDriver &Driver,
			eDelimiter Delimiter,
			const str::dString &PendingData = str::wString() )
		{
			Flow_.Init( Driver );
			Base_.Init( Flow_, PendingData, Delimiter );
			rDriver_::Init( fdr::ts_Default );
		}
		void Purge( void )
		{
			fdr::Purge( *this );
		}
	};

	// Response code.
	qENUM( ResponseCode ) {
		// Not really response code, but simplifies the detection of a status.
		rcOK,
		rcNo,
		rcBad,
		rcPreAuth,
		rcBye,
		// Not an 'IMAP' response code, but it's the counterpart of the one defined in 'eStatus'.
		rcDisconnected,
		// Response codes which may optionnaly be contained in status response.
		rcAlert,
		rcBadCharSet,
		rcCapability,
		rcParse,
		rcPermanentFlags,
		rcReadOnly,
		rcReadWrite,
		rcTryCreate,
		rcUIDNext,
		rcUIDValidity,
		rcUnseen,
		// Below response codes are from RFC 55530.
		rcUnavailable,
		rcAuthenticationFailed,
		rcAuthorizationFailed,
		rcExpired,
		rcPrivacyRequired,
		rcContactAdmin,
		rcNoPerm,
		rcInUse,
		rcExpungeIssued,
		rcCorruption,
		rcServerBug,
		rcClientBug,
		rcCanNot,
		rcLimit,
		rcOverQuota,
		rcAlreadyExists,
		rcNonExistent,
		// Reponses to commands.
		// rcCapability, // Already present above.
		rcList,
		rcLSub,
		rcStatus,
		rcSearch,
		rcFlags,
		rcExists,
		rcRecent,
		rcExpunge,
		rcFetch,
		rc_amount,
		rc_None,
		rc_Undefined
	};

	const char *GetLabel( eResponseCode Code );

	qENUM( Status ) {
		sOK,
		sNO,
		sBAD,
		sDisconnected,	// Not an 'IMAP' status, but hndling this here is easier.
		s_amount,
		s_Erroneous,	// Server returned a not 'IMAP' compliant answer.
		s_Undefined
	};

	const char *GetLabel( eStatus Status );

	class rConsole
	{
	private:
		str::wString Tag_;
		flw::rDressedRFlow<> IFlow_;
		txf::rWFlow OFlow_;
		rResponseDriver_ ResponseDriver_;
		eResponseCode PendingCode_;
		bso::sBool PendingCodeIsStatus_;
		bso::sBool NoTaggedStatusResponse_;	// To handle the connection, where there is no tagged status response.
	public:
		void reset( bso::sBool P = true )
		{
			tol::reset( P, Tag_, IFlow_, OFlow_, PendingCodeIsStatus_, ResponseDriver_, NoTaggedStatusResponse_ );
			PendingCode_ = rc_Undefined;
		}
		qCDTOR( rConsole );
		void Init( fdr::rRWDriver &Driver )
		{
			tol::Init( Tag_ );
			PendingCode_ = rc_Undefined;
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
		txf::sWFlow &WFlow( void )
		{
			return OFlow_;
		}
		eResponseCode GetPendingResponseCode( void );
		bso::sBool Search( eResponseCode WantedCode )
		{
			eResponseCode Code = rc_Undefined;

			while ( ( ( Code = GetPendingResponseCode() ) != rc_None ) && ( Code != WantedCode ) )
				SkipResponse();

			return Code == WantedCode;
		}
		fdr::rRDriver &GetResponseDriver( void )
		{
			return ResponseDriver_;
		}
		void SkipResponse( void )
		{
			ResponseDriver_.Purge();
		}
		void SkipRemainingReponses( void )
		{
			while ( GetPendingResponseCode() != rc_None )
				SkipResponse();
		}
		// Also resets the 'PendingCodeIsStatus_'
		// You have still have to handle 'GetResponseCode(...)' or launch 'SkipReponse(...');
		eStatus GetStatus( void )
		{
			// Althought 'PendingCode' is a status, the 'PendingCodeIsStatus_' is already set to false by 'GetPendingCode(...').
			eStatus Status = s_Undefined;

			switch ( PendingCode_ ) {
			case rcOK:
				Status = sOK;
				break;
			case rcNo:
				Status = sNO;
				break;
			case rcBad:
				Status = sBAD;
				break;
			case rcDisconnected:
				Status = sDisconnected;
				break;
			default:
				qRGnr();
				break;
			}

			PendingCode_ = rc_Undefined;

			return Status;
		}
	};

	// The 'COPY', 'FETCH', 'SEARCH', 'STORE' can be sub-commands of the 'UID' command.
	qENUM( Flavor ) {
		fRegular,
		fUID,
		f_amount,
		f_Undefined,
		f_Default = fRegular,
	};

	/*
		First, call 'Connect(...)', then 'Login(...)' (technically, it's not mandatory
		but if you dont, you would not be able to do very much), and end with
		'Logout(...)'. Between 'Login(....)' and 'Logout(...)', call all other
		functions as needed.

		After calling each functions, call 'rConsole.GetPendingCode(...)',
		then handle 'rConsole::GetResponseDriver(...)' or call 'rConsole::SkipReponse(...)'
		until 'rConsole.GetPendingCode(...)' returns 'c_None'.	Then Call the
		'rConsole::GetStatus(...)',	then handle 'rConsole::GetResponseDriver(...)',
		or call 'rConsole::SkipReponse(...)'.
	*/

	// This is the first command to call after opening a connection to the 'IMAP' server.
	void Connect( rConsole &Console );

	// To log in. Most commands are not available when this is skipped or not done successfully.
	void Login(
		const str::dString &Username,
		const str::dString &Password,
		rConsole &Console );

	// To call just before closing the connexion. You do _not_ need to be logged in to log out.
	void Logout( rConsole &Console );

	// Launches the corresponding 'IMAP' command.
	void Capability( rConsole &Console );
	void Select(
		const str::dString &Mailbox,
		rConsole &Console );
	void List(
		const str::dString &Reference,
		const str::dString &Mailbox,
		rConsole &Console );
	void LSub(
		const str::dString &Reference,
		const str::dString &Mailbox,
		rConsole &Console );
	void Fetch(
		eFlavor Flavor,
		const str::dString &SequenceSet,
		const str::dString &Items,
		rConsole &Console );
}


#endif
