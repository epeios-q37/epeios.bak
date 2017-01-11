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
			tol::reset( P, Flow_, EOF_, BracketIsEOF_, Pending_ );
		}
		qCVDTOR( rResponseDriver_ );
		void Init(
			flw::sIFlow &Flow,
			const str::dString &PendingData )
		{
			EOF_ = BracketIsEOF_ = false;
			Flow_ = &Flow;
			rDriver_::Init( fdr::ts_Default );
			Pending_.Init( PendingData );
		}
		void BracketIsEOF( void )
		{
			if ( BracketIsEOF_ )
				qRGnr();

			BracketIsEOF_ = true;
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
		cUUIDValidity,
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

	namespace base {
		qENUM( Status ) {
			sOK,
			sNO,
			sBAD,
			s_amount,
			s_Pending,	// Status pending ; other response have to be handled before obtining the status.
			sErroneous,	// Server returned a not 'IMAP' compliant answer.
			s_Undefined
		};
	}

	class rSession
	{
	private:
		str::wString Tag_;
		flw::sDressedIFlow<> IFlow_;
		txf::rOFlow OFlow_;
		rResponseDriver_ ResponseDriver_;
		bso::sBool SkipTag_;	// On first use, the tag is already eaten.
		base::eStatus PendingStatus_;
		str::wString PendingData_;
	public:
		void reset( bso::sBool P = true )
		{
			tol::reset( P, Tag_, IFlow_, OFlow_, SkipTag_, PendingData_  );
			PendingStatus_ = base::s_Undefined;
		}
		qCDTOR( rSession );
		void Init( fdr::rIODriver &Driver )
		{
			tol::Init( Tag_, PendingData_ );
			PendingStatus_ = base::s_Undefined;
			IFlow_.Init( Driver );
			OFlow_.Init( Driver );
			SkipTag_ = true;
		}
		void SetPendingStatus( base::eStatus Status )
		{
			PendingStatus_ = Status;
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
			ResponseDriver_.Init( IFlow_, PendingData_ );

			if ( PendingStatus_ != base::s_Undefined )
				ResponseDriver_.BracketIsEOF();

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
		base::eStatus GetPendingStatus( void ) const
		{
			return PendingStatus_;
		}
	};

	namespace base {
		const char *GetLabel( eStatus Status );
		/*
			If returned value == 'iPending, you must read pendind response using
			'Session.GetCode(...)',  followed by 'GetDataDriver(...)'/'SkipData(...)'
			until 'GetCode(...)' returns 'c_Completed'.
		*/

		eStatus GetCompletionStatus( rSession &Session );

		// This is the first command to call after opening a connection to the server.
		eStatus Connect( rSession &Session );

		// To log in. Most commands are not available when this is not done successfully.
		eStatus Login(
			const str::dString &Username,
			const str::dString &Password,
			rSession &Session );

		// To call just before closing the connexion. You do _not_ need to be logged in to log out.
		eStatus Logout( rSession &Session );
		eStatus Capability( rSession &Session );
		eStatus Select( rSession &Session );
	}
}


#endif
