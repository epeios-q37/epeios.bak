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

#include "muaima.h"

#include "stsfsm.h"

using namespace muaima;

#define EOFT	if ( Flow.EndOfFlow() ) qRGnr();

namespace {
	inline const str::dString &GetAlphanumMinus_(
		flw::sIFlow &Flow,
		str::dString &AlphaNum )
	{
		flw::sByte Byte = 0;
		while ( !Flow.EndOfFlow() && ( isalnum( Byte = Flow.View() ) || ( Byte == '-' ) ) )
			AlphaNum.Append( Flow.Get() );

		return AlphaNum;
	}
}

#define C( name )	case c##name : return #name; break
 
const char *muaima::GetLabel( eCode Code )
{
	switch ( Code ) {
	C( OK );
	C( No );
	C( Bad );
	C( PreAuth);
	C( Bye );
	C( Alert );
	C( BadCharSet );
	C( Capability );
	C( Parse );
	C( PermanentFlags );
	case cReadOnly:
		return "Read-Only";
		break;
	case cReadWrite:
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
			eCode Code,
			str::dString &Label )
		{
			Label.Append( GetLabel( Code ) );

			str::ToUpper( Label );

			return Label;
		}

		eCode GetCode( const str::dString &Pattern )
		{
			return stsfsm::GetId( Pattern, Automat, c_Undefined, c_amount );
		}
	}

	void FillAutomat( void )
	{
		_::Automat.Init();
		stsfsm::Fill<eCode>( _::Automat, c_amount, _::GetUCLabel );
	}

	eCode Get(
		flw::sIFlow &Flow,
		str::dString &Before )
	{
		eCode Code = c_Undefined;
	qRH
		str::wString Pattern;
		flw::sByte Byte = 0;
	qRB
		EOFT;

		if ( isdigit( Flow.View() ) ) {
			GetAlphanumMinus_( Flow, Before );

			if ( Flow.Get() != ' ' )
				qRGnr();
		}

		Pattern.Init();

		GetAlphanumMinus_( Flow, Pattern );

		Code = _::GetCode( Pattern );

		EOFT;

		if ( Flow.View() == ' ' )
			Flow.Skip();
	qRR
	qRT
	qRE
		return Code;
	}
}

eCode muaima::rSession::GetCode( void )
{
	eCode Code = c_Undefined;
qRH
	flw::sIFlow &Flow = IFlow_;
	str::wString PendingData, Tag;
qRB
	PendingData.Init();

	if ( PendingCode_ != c_Undefined ) {
		if ( PendingCodeIsStatus_ ) {
			Code = c_None;
			PendingCodeIsStatus_ = false;
		} else {
			Code = PendingCode_;
			PendingCode_ = c_Undefined;
		}
		ResponseDriver_.Init( Flow, PendingData, false );
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

		EOFT;

		if ( Flow.Get() != ' ' )
			qRGnr();

		Code = code_::Get( Flow, PendingData );

		EOFT;

		if ( Flow.View() == '[' ) {
			PendingCode_ = Code;
			if ( PendingData.Amount() != 0 )
				qRGnr();
			Flow.Skip();
			Code = code_::Get( Flow, PendingData );
			ResponseDriver_.Init( Flow, PendingData, true );
		} else {
			if ( PendingCodeIsStatus_ ) {
				PendingCode_ = Code;
				Code = c_None;
				PendingCodeIsStatus_ = false;
			}

			ResponseDriver_.Init( Flow, PendingData, false );
		}
	}
qRR
qRT
qRE
	return Code;
}

#define C( name )	case s##name : return #name; break
const char *muaima::GetLabel( eStatus Status )
{
	switch ( Status ) {
	C( OK );
	C( NO );
	C( BAD );
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

	eStatus Get( flw::sIFlow &Flow )
	{
		eStatus Status = s_Undefined;
	qRH
		str::wString Pattern;
	qRB
		EOFT;

		Pattern.Init();

		GetAlphanumMinus_( Flow, Pattern );

		Status = _::GetStatus( Pattern );
	qRR
	qRT
	qRE
		return Status;
	}
}

namespace _ {
	qENUM( Command_ ) {
		cLogin,
		cLogout,	// NOT the counter-part of 'Login' ; closes the connection.
		cCapability,
		cSelect,
		cList,
		c_amount,
		c_Undefined
	};

#define C( name )\
	case c##name:\
		return #name;\
		break

	const char *GetLabel_( eCommand_ Command )
	{
		switch ( Command ) {
		C( Login);
		C( Logout );
		C( Capability );
		C( Select );
		C( List );
		default:
			qRGnr();
			break;
		}

		return NULL;	// To avoid a warning.
	}

	void SendCommand(
		const str::dString &Tag,
		eCommand_ Command,
		txf::sOFlow &Flow )
	{
	qRH
		str::wString CommandString;
	qRB
		CommandString.Init( GetLabel_( Command ) );

		str::ToUpper( CommandString );

		Flow << Tag << ' ' << CommandString;
	qRR
	qRT
	qRE
	}

	void SendCFLR( txf::sOFlow &Flow )
	{
		 Flow << "\r\n";

		 Flow.Commit();
	}
}

eStatus muaima::GetCompletionStatus( rSession &Session )
{
	return Session.GetPendingStatus();
}


eStatus muaima::Connect( rSession &Session )
{
	Session.ReportUntaggedStatusResponse();

	return s_Undefined;
}

eStatus muaima::Login(
	const str::dString &Username,
	const str::dString &Password,
	rSession &Session )
{
	_::SendCommand(Session.GetNextTag(), _::cLogin, Session.OFlow() );

	Session.OFlow() << ' ' << Username << ' ' << Password;

	_::SendCFLR( Session.OFlow() );

	return s_Undefined;
}

eStatus muaima::Logout( rSession &Session )
{
	_::SendCommand( Session.GetNextTag(), _::cLogout, Session.OFlow() );
	_::SendCFLR( Session.OFlow() );

	return s_Undefined;
}

eStatus muaima::Capability( rSession &Session )
{
	_::SendCommand( Session.GetNextTag(), _::cCapability, Session.OFlow() );
	_::SendCFLR( Session.OFlow() );

	return s_Undefined;
}

eStatus muaima::Select(
	const str::dString &Mailbox,
	rSession &Session )
{
	_::SendCommand( Session.GetNextTag(), _::cSelect, Session.OFlow() );
	Session.OFlow() << ' ' << Mailbox;
	_::SendCFLR( Session.OFlow() );

	return s_Undefined;
}

eStatus muaima::List(
	const str::dString &Reference,
	const str::dString &Mailbox,
	rSession &Session )
{
	_::SendCommand( Session.GetNextTag(), _::cList, Session.OFlow() );
	Session.OFlow() << " \"" << Reference << "\" \"" << Mailbox << '"';
	_::SendCFLR( Session.OFlow() );

	return s_Undefined;
}

namespace {
	void FillAutomats_( void )
	{
		code_::FillAutomat();
		status_::FillAutomat();
	}
}

qGCTOR( muaima )
{
	FillAutomats_();
}

