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

#include "muaimabs.h"

#include "stsfsm.h"

using namespace muaimabs;

namespace {
	inline const str::dString &GetAlphanumMinus_(
		flw::rRFlow &Flow,
		str::dString &AlphaNum )
	{
		flw::sByte Byte = 0;
		while ( !Flow.EndOfFlow() && ( isalnum( Byte = Flow.View() ) || ( Byte == '-' ) ) )
			AlphaNum.Append( Flow.Get() );

		return AlphaNum;
	}
}

bso::sBool muaimabs::rDriverBase_::HandleFreeContext_(
	fdr::sByte Byte,
	flw::rRFlow &Flow,
	fdr::sByte *Buffer,
	fdr::sSize &Amount )
{
	bso::sBool Put = true;	// If 'true', 'Byte' is put in 'Buffer'.
	bso::sBool HandleCRLF = true;

	switch ( Byte ) {
	case ' ':
		if ( Delimiter_ == dNone ) {
			Context_ = _::cEOF;
			Put = false;
			HandleCRLF = false;
		}
		break;
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
			Delimiter_ = dLiteral;
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

	return HandleCRLF;
}

void muaimabs::rDriverBase_::HandleQuotedContext_(
	fdr::sByte Byte,
	flw::rRFlow &Flow,
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
		Buffer[Amount++] = Byte;
		break;
	}
}

void muaimabs::rDriverBase_::HandleLiteralContext_(
	fdr::sByte Byte,
	flw::rRFlow &Flow,
	fdr::sByte *Buffer,
	fdr::sSize &Amount )
{
	if ( Byte == '}' ) {
		if ( Delimiter_ != dLiteral )
			Force_ += 2;	// To include the 'CRLF'.
		else
			Flow.Skip( 2 );
		Context_ = _::cFree;
	} else if ( !isdigit( Byte ) ) {
		qRGnr();
	} else if ( Force_ < ((bso::SizeMax / 10) - 1) ) {
		Force_ = Force_ * 10 + (Byte - '0');
	} else
		qRLmt();

	if ( Delimiter_ != dLiteral )
		Buffer[Amount++] = Byte;
}

void muaimabs::rDriverBase_::HandleContext_(
	flw::rRFlow &Flow,
	fdr::sByte *Buffer,
	fdr::sSize &Amount )
{
	bso::sBool HandleSPCRLF = true;

	if ( ( Context_ != _::cEOF ) && Flow.EndOfFlow() )
		Context_ = _::cEOF;

	switch ( Context_ ) {
	case _::cFree:
		HandleSPCRLF = HandleFreeContext_( Flow.Get(), Flow, Buffer, Amount );
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

fdr::sSize muaimabs::rDriverBase_::Read(
	fdr::sSize Maximum,
	fdr::sByte *Buffer )
{
	fdr::sSize Amount = 0;

	flw::rRFlow &Flow = F_();

	if ( Maximum < 2 )	// Due to the 'Pending' handling, that must be place in the 'Buffer' to put at least chars.
		qRVct();

	while ( Amount < Maximum ) {
		if ( !Pending_.IsEmpty() ) {
			if ( Maximum > ( Amount + 1 ) ) { // To ensure that there is place in 'Buffer' to put a ' '.
				Amount += Pending_.ReadUpTo( Maximum - Amount - 1, Buffer + Amount );

				if ( Pending_.IsEmpty() )
					if ( Flow.View() != '\r' )
						Buffer[Amount++] = ' ';
			}
			else
				Maximum = Amount;	// To exit the loop.
		}
		else if ( (Force_ != 0) && (Context_ != _::cLiteral) ) {
			bso::sSize PonctualAmount = Flow.ReadUpTo( Force_ > Maximum - Amount ? Maximum - Amount : Force_, Buffer + Amount );

			Amount += PonctualAmount;

			Force_ -= PonctualAmount;

			if ( (  Force_ == 0 ) && ( Delimiter_ == dLiteral ) )
				Context_ = _::cEOF;
		} else {
			HandleContext_( Flow, Buffer, Amount );
		}

		if ( ( Context_ == _::cEOF ) || Flow.EndOfFlow() ) {
			Context_ = _::cEOF;
			if ( Amount != 0 )
				F_().Dismiss(false);
			Maximum = Amount;	// To exit the loop.
		}
	}

	return Amount;
}

#define C( name )	case rc##name : return #name; break

const char *muaimabs::GetLabel( eResponseCode Code )
{
	switch ( Code ) {
	C( OK );
	C( No );
	C( Bad );
	C( PreAuth);
	C( Bye );
	C( Disconnected );
	C( Alert );
	C( BadCharSet );
	C( Capability );
	C( Parse );
	C( PermanentFlags );
	case rcReadOnly:
		return "Read-Only";
		break;
	case rcReadWrite:
		return "Read-Write";
		break;
	C( TryCreate );
	C( UIDNext );
	C( UIDValidity );
	C( Unseen );
	C( Unavailable );
	C( AuthenticationFailed );
	C( AuthorizationFailed );
	C( Expired );
	C( PrivacyRequired );
	C( ContactAdmin );
	C( NoPerm );
	C( InUse );
	C( ExpungeIssued );
	C( Corruption );
	C( ServerBug );
	C( ClientBug );
	C( CanNot );
	C( Limit );
	C( OverQuota );
	C( AlreadyExists );
	C( NonExistent );
	C( List );
	C( LSub );
	C( Status );
	C( Search );
	C( Flags );
	C( Exists );
	C( Recent );
	C( Expunge );
	C( Fetch );
	default:
		qRFwk();
		break;
	}

	return NULL;	// To avoid a warning.
}

#undef C

namespace code_ {
	namespace _ {
		stsfsm::wAutomat Automat;

		const str::dString &GetUCLabel(
			eResponseCode Code,
			str::dString &Label )
		{
			Label.Append( GetLabel( Code ) );

			str::ToUpper( Label );

			return Label;
		}

		eResponseCode GetCode( const str::dString &Pattern )
		{
			return stsfsm::GetId( Pattern, Automat, rc_Undefined, rc_amount );
		}
	}

	void FillAutomat( void )
	{
		_::Automat.Init();
		stsfsm::Fill<eResponseCode>( _::Automat, rc_amount, _::GetUCLabel );
	}

	eResponseCode Get(
		flw::rRFlow &Flow,
		str::dString &Before )
	{
		eResponseCode Code = rc_Undefined;
	qRH
		str::wString Pattern;
	qRB
		if ( isdigit( Flow.View() ) ) {
			GetAlphanumMinus_( Flow, Before );

			if ( Flow.Get() != ' ' )
				qRGnr();
		}

		Pattern.Init();

		GetAlphanumMinus_( Flow, Pattern );

		Code = _::GetCode( Pattern );

		if ( Flow.View() == ' ' )
			Flow.Skip();
	qRR
	qRT
	qRE
		return Code;
	}
}

eResponseCode muaimabs::rConsole::GetPendingResponseCode( void )
{
	eResponseCode Code = rc_Undefined;
qRH
	flw::rRFlow &Flow = IFlow_;
	str::wString PendingData, Tag;
qRB
	PendingData.Init();

	if ( PendingCode_ != rc_Undefined ) {
		if ( PendingCodeIsStatus_ ) {
			Code = rc_None;
			PendingCodeIsStatus_ = false;
		} else {
			Code = PendingCode_;
			PendingCode_ = rc_Undefined;
		}
		ResponseDriver_.Init( Flow, dCRLF, PendingData );
	} else if ( Flow.EndOfFlow() ) {
		Code = rc_None;
		PendingCode_ = rcDisconnected;
		PendingCodeIsStatus_ = true;
	} else {
		if ( Flow.View() == '*' ) {
			Flow.Skip();

			if ( NoTaggedStatusResponse_ ) {
				PendingCodeIsStatus_ = true;
				NoTaggedStatusResponse_ = false;
			}
		} else {
			Tag.Init();
			GetAlphanumMinus_( Flow, Tag );
			PendingCodeIsStatus_ = true;
		}

		if ( Flow.Get() != ' ' )
			qRGnr();

		Code = code_::Get( Flow, PendingData );

		if ( Flow.View() == '[' ) {
			PendingCode_ = Code;
			if ( PendingData.Amount() != 0 )
				qRGnr();
			Flow.Skip();
			Code = code_::Get( Flow, PendingData );
			ResponseDriver_.Init( Flow, dBracket, PendingData );
		} else {
			if ( PendingCodeIsStatus_ ) {
				PendingCode_ = Code;
				Code = rc_None;
				PendingCodeIsStatus_ = false;
			}

			ResponseDriver_.Init( Flow, dCRLF, PendingData );
		}
	}
qRR
qRT
qRE
	return Code;
}

#define C( name )	case s##name : return #name; break
const char *muaimabs::GetLabel( eStatus Status )
{
	switch ( Status ) {
	C( OK );
	C( NO );
	C( BAD );
	C( Disconnected );
	default:
		qRFwk();
		break;
	}

	return NULL;	// To avoid a warning.
}
#undef C


namespace status_ {
	namespace _ {
		stsfsm::wAutomat Automat;

		eStatus GetStatus( const str::dString &Pattern )
		{
			return stsfsm::GetId( Pattern, Automat, s_Undefined, s_amount );
		}
	}

	void FillAutomat( void )
	{
		_::Automat.Init();
		stsfsm::Fill<eStatus>( _::Automat, s_amount, GetLabel );
	}

	eStatus Get( flw::rRFlow &Flow )
	{
		eStatus Status = s_Undefined;
	qRH
		str::wString Pattern;
	qRB
		Pattern.Init();

		GetAlphanumMinus_( Flow, Pattern );

		Status = _::GetStatus( Pattern );
	qRR
	qRT
	qRE
		return Status;
	}
}

# define CRLF "\r\n"

void muaimabs::Connect( rConsole &Console )
{
	Console.ReportUntaggedStatusResponse();
}

void muaimabs::Login(
	const str::dString &Username,
	const str::dString &Password,
	rConsole &Console )
{
	Console.WFlow() << Console.GetNextTag() << " LOGIN " << Username << ' ' << Password  << CRLF << txf::commit;
}

void muaimabs::Logout( rConsole &Console )
{
	Console.WFlow() << Console.GetNextTag() << " LOGOUT" << CRLF << txf::commit;
}

void muaimabs::Capability( rConsole &Console )
{
	Console.WFlow() << Console.GetNextTag() << " CAPABILITY " << CRLF << txf::commit;
}

void muaimabs::Select(
	const str::dString &Mailbox,
	rConsole &Console )
{
	Console.WFlow() << Console.GetNextTag() << " SELECT " << Mailbox << CRLF << txf::commit;
}

void muaimabs::List(
	const str::dString &Reference,
	const str::dString &Mailbox,
	rConsole &Console )
{
	Console.WFlow() << Console.GetNextTag() << " LIST \"" << Reference << "\" \"" << Mailbox << '"' << CRLF << txf::commit;
}

void muaimabs::LSub(
	const str::dString &Reference,
	const str::dString &Mailbox,
	rConsole &Console )
{
	Console.WFlow() << Console.GetNextTag() << " LSUB \"" << Reference << "\" \"" << Mailbox << '"' << CRLF << txf::commit;
}

void muaimabs::Fetch(
	eFlavor Flavor,
	const str::dString &SequenceSet,
	const str::dString &Items,
	rConsole &Console )
{
	Console.WFlow() << Console.GetNextTag() << ( Flavor == fUID ? " UID" : "" ) << " FETCH " << SequenceSet << ' ' << Items << CRLF << txf::commit;
}

namespace {
	void FillAutomats_( void )
	{
		code_::FillAutomat();
		status_::FillAutomat();
	}
}

qGCTOR( muaimabs )
{
	FillAutomats_();
}


