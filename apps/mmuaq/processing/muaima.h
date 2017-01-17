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

	inline void SkipCRLF_( flw::sIFlow &Flow )
	{
		if ( Flow.Get() != '\r' )
			qRGnr();

		if ( Flow.Get() != '\n' )
			qRGnr();
	}

	class rDriverBase_
	{
	private:
		qRMV( flw::sIFlow, F_, Flow_ );
		eDelimiter Delimiter_;
		_::eContext Context_;
		rPendingIFlow_ Pending_;
		bso::sSize Level_;	// Parenthensis level.
		bso::sSize Force_;	// Remaining amount of byte to put without taking care of value.
		void HandleFreeContext_(
			fdr::sByte Byte,
			flw::sIFlow &Flow,
			fdr::sByte *Buffer,
			fdr::sSize &Amount )
		{
			bso::sBool Put = true;	// If 'true', 'Byte' is put in 'Buffer'.

			switch ( Byte ) {
			case '"':
				if ( Delimiter_ == dNone ) {
					Delimiter_ = dQuote;
					Put = false;
				} else if ( Delimiter_ == dQuote ) {
					Context_ = _::cEOF;
					Put = false;
				} else
					Context_ = _::cQuoted;
				break;
			case '(':
				if ( Delimiter_ == dNone ) {
					Delimiter_ = dParenthesis;
					Put = false;
				} else if ( Level_ == bso::U8Max )
					qRLmt();
				else 
					Level_++;
				// We stay in the same context.
				break;
			case ')':
				if ( Level_ == 0 ) {
					if ( Delimiter_ != dParenthesis )
						qRGnr();

					Context_ = _::cEOF;
					Put = false;
				} else
					Level_--;

				// We stay in the same context.
				break;
			case '[':	// Can happen with 'FETCH' 'BODY[HEADER]' reponse.
				if ( Delimiter_ == dBracket )
					qRGnr();
				break;
			case ']':
				if ( Delimiter_ == dBracket ) {
					Context_ = _::cEOF;
					Put = false;
				}
				break;		
			case '{':
				if ( Delimiter_ == dNone ) {
					Delimiter_ = dBracket;
					Put = false;
				}

				Context_ = _::cLiteral;
				break;
			case '}':
				qRGnr();
				break;
			case '\r':
				if ( Flow.View() != '\n' )
					qRGnr();

				if ( ( Delimiter_ == dCRLF ) || ( Delimiter_ == dNone ) ) {	// Can sometimes occur when directly called from 'HandleFreeContext_(...)'.
					Context_ = _::cEOF;
					Put = false;
				} else 
					Force_ = 1;
				break;
			case '\n':	// Should have be already skipped.
				qRGnr();
				break;
			case 0:		// Not allowed
				qRGnr();
				break;
			default:
				break;
			}

			if ( Put )
				Buffer[Amount++] = Byte;
		}
		bso::sBool HandleQuotedContext_(
			fdr::sByte Byte,
			flw::sIFlow &Flow,
			fdr::sByte *Buffer,
			fdr::sSize &Amount )
		{
			switch ( Byte ) {
			case '\\':
				Byte = Flow.View();
				if ( ( Byte != '\\' ) && ( Byte != '"' ) )
					qRGnr();

				if ( Delimiter_ == dQuote ) {
					Buffer[Amount++] = Flow.Get();
				} else {
					Buffer[Amount++] = '\\';
					Force_ = 1;
				}
				break;
			case '"':
				if ( Delimiter_ == dQuote )
					Context_ = _::cEOF;
				else {
					Buffer[Amount++] = Byte;
					Context_ = _::cFree;
				}
				break;
			default:
				Buffer[Amount++]=Byte;
				break;
			}

			return true;
		}
		void HandleLiteralContext_(
			fdr::sByte Byte,
			flw::sIFlow &Flow,
			fdr::sByte *Buffer,
			fdr::sSize &Amount )
		{
			if ( Byte == '}' ) {
				if ( Delimiter_ != dLiteral )
					Force_ += 2;	// To include the 'CRLF'.
				Context_ = _::cFree;
			} else if ( !isdigit(Byte) ) {
				qRGnr();
			} else if ( Force_ < ( ( bso::SizeMax / 10 ) -1 ) ) {
				Force_ = Force_ * 10 + ( Byte - '0' );
			} else
				qRLmt();

			if ( Delimiter_ != dLiteral )
				Buffer[Amount++] = Byte;
		}
		void HandleContext_(
			flw::sIFlow &Flow,
			fdr::sByte *Buffer,
			fdr::sSize &Amount )
		{
			bso::sBool HandleSPCRLF = true;

			switch ( Context_ ) {
			case _::cFree:
				HandleFreeContext_( Flow.Get(), Flow, Buffer, Amount );
				break;
			case _::cQuoted:
				HandleQuotedContext_( Flow.Get(), Flow, Buffer, Amount );
				break;
			case _::cLiteral:
				HandleLiteralContext_( Flow.Get(), Flow, Buffer, Amount );
				break;
			case _::cEOF:
				HandleSPCRLF = false;
				break;
			default:
				qRGnr();
				break;
			}

			if ( Context_ == _::cEOF ) {
				if ( HandleSPCRLF && !Flow.EndOfFlow() ) {
					switch ( Flow.Get() ) {
					case ' ':
						break;
					case '\r':
						if ( Flow.Get() != '\n' )
							qRGnr();
						break;
					case '\n':	// '\r' was already eaten.
						break;
					default:	// All above space chars are already be eaten.
						break;
					}
				}
			}
		}
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
			flw::sIFlow &Flow,
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
			fdr::sByte *Buffer )
		{
			fdr::sSize Amount = 0;

			flw::sIFlow &Flow = F_();

			if ( Maximum < 2 )	// Due to the 'Pending' handling, that must be place in the 'Buffer' to put at least chars.
				qRVct();

			while ( Amount < Maximum ) {
				if ( !Pending_.IsEmpty() ) {
					if ( Maximum > ( Amount + 1 ) ) { // To ensure that there is place in 'Buffer' to put a ' '.
						Amount += Pending_.ReadUpTo( Maximum - Amount - 1, Buffer + Amount );

						if ( Pending_.IsEmpty() )
							if ( Flow.View() != '\r' )
								Buffer[Amount++] = ' ';
					} else
						Maximum = Amount;	// To exit the loop.
				} else if ( ( Force_ != 0 ) && ( Context_ != _::cLiteral ) ) {
					bso::sSize PonctualAmount = Flow.ReadUpTo( Force_ > Maximum - Amount ? Maximum - Amount : Force_, Buffer + Amount );

					Amount += PonctualAmount;

					Force_ -= PonctualAmount;
				} else {
					HandleContext_( Flow, Buffer, Amount );
				}

				if ( ( Context_ == _::cEOF ) || Flow.EndOfFlow() ) {
					Context_ = _::cEOF;
					Maximum = Amount;	// To exit the loop.
				}
			}

			return Amount;
		}
		void Dismiss( bso::sBool Unlock )
		{
			F_().Dismiss( Unlock );
		}
		fdr::sTID ITake( fdr::sTID Owner )
		{
			return F_().IDriver().ITake( Owner );
		}

	};

	class rResponseDriver_
	: public rDriver_
	{
	private:
		rDriverBase_ Base_;
		flw::sDressedIFlow<> Flow_;
	protected:
		virtual fdr::sSize FDRRead(
			fdr::sSize Maximum,
			fdr::sByte* Buffer ) override
		{
			return Base_.Read( Maximum, Buffer );
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
			rDriver_::reset( P );
			tol::reset( P, Base_, Flow_ );
		}
		qCVDTOR( rResponseDriver_ );
		void Init(
			flw::sIFlow &Flow,
			const str::dString &PendingData,
			eDelimiter Delimiter )
		{
			Base_.Init( Flow, PendingData, Delimiter );
			rDriver_::Init( fdr::ts_Default );
		}
		void Init(
			fdr::rIDriver &Driver,
			const str::dString &PendingData,
			eDelimiter Delimiter )
		{
			Flow_.Init( Driver );
			Base_.Init( Flow_, PendingData, Delimiter );
			rDriver_::Init( fdr::ts_Default );
		}
	};

	// Response code.
	qENUM( ResponseCode ) {
		// Not really response code, but 
		rcOK,
		rcNo,
		rcBad,
		rcPreAuth,
		rcBye,
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
		s_amount,
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
		void Init( fdr::rIODriver &Driver )
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
		txf::sOFlow &OFlow( void )
		{
			return OFlow_;
		}
		eResponseCode GetPendingResponseCode( void );
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
		// Also resets the 'PendingCodeIsStatus_'
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
			default:
				qRGnr();
				break;
			}

			PendingCode_ = rc_Undefined;

			return Status;
		}
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
		const str::dString &Sequence,
		const str::dString &Items,
		rConsole &Console );

	class rValueDriver_
	: public rDriver_
	{
	public:
		rDriverBase_ Base_;
		flw::sDressedIFlow<> Flow_;
	protected:
		virtual fdr::sSize FDRRead(
			fdr::sSize Maximum,
			fdr::sByte* Buffer ) override
		{
			return Base_.Read( Maximum, Buffer );
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
		void reset( bso::sBool P = true ) {
			rDriver_::reset( P  );
			tol::reset( P, Base_, Flow_ );
		}
		qCVDTOR( rValueDriver_ );
		void Init( flw::sIFlow &Flow )
		{
			rDriver_::Init( fdr::ts_Default );
			Base_.Init( Flow, str::wString(), dNone );
		}
		void Init( fdr::rIDriver &Driver )
		{
			rDriver_::Init( fdr::ts_Default );
			Flow_.Init( Driver );
			Base_.Init( Flow_, str::wString(), dNone );
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

		class rConsole
		{
		private:
			flw::sDressedIFlow<> Flow_;
			rValueDriver_ ValueDriver_;
		public:
			void reset( bso::sBool P = true )
			{
				tol::reset( P, Flow_, ValueDriver_ );
			}
			qCDTOR( rConsole );
			void Init( fdr::rIDriver &Driver )
			{
				Flow_.Init( Driver );
				// 'ValueDriver_' will be initialized as needed.
			}
			eName Get( void );
			fdr::rIDriver &GetValueDriver( void )
			{
				ValueDriver_.Init( Flow_ );

				return ValueDriver_;
			}
		};
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

	class cList
	{
	protected:
		virtual void MUAIMAOnMailbox( const str::dString &Name ) = 0;
	public:
		qCALLBACK( List );
		void OnMailbox( const str::dString &Name )
		{
			return MUAIMAOnMailbox( Name );
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
		eStatus HandleStatus_( qRPN );
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
		eStatus GetFolders(
			const str::dString &Folder,
			cList &Callback,
			qRPD );
	};
}


#endif
