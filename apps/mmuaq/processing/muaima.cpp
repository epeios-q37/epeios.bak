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
	inline const str::dString &GetAlphanum_(
		flw::sIFlow &Flow,
		str::dString &AlphaNum )
	{
		while ( !Flow.EndOfFlow() && isalnum( Flow.View() ) )
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
	C( Bye );
	C( Capability );
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

	eCode Get( flw::sIFlow &Flow )
	{
		eCode Code = c_Undefined;
	qRH
		str::wString Pattern;
		flw::sByte Byte = 0;
	qRB
		EOFT;

		Pattern.Init();

		GetAlphanum_( Flow, Pattern );

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
	flw::sIFlow &Flow = IFlow_;

	EOFT;

	if ( SkipTag_ ) {
		Code = code_::Get( Flow );
		SkipTag_ = false;
	} else if ( Flow.View() != '*' ) {
		SkipTag_ = true;
		Code = c_None;
	} else {
		Flow.Skip();

		if ( Flow.Get() != ' ' )
			qRGnr();

		Code = code_::Get( Flow );
	}

	return Code;
}

#define C( name )	case s##name : return #name; break
const char *base::GetLabel( eStatus Status )
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
	using namespace base;

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
		stsfsm::Fill<base::eStatus>( _::Automat, base::s_amount, GetLabel );
	}

	eStatus Get( flw::sIFlow &Flow )
	{
		eStatus Status = s_Undefined;
	qRH
		str::wString Pattern;
	qRB
		EOFT;

		Pattern.Init();

		GetAlphanum_( Flow, Pattern );

		Status = _::GetStatus( Pattern );
	qRR
	qRT
	qRE
		return Status;
	}
}

namespace _ {
	qENUM( Command_ ) {
		cLogout,	// NOT the counter-part of 'Login' ; closes the connection.
		cCapability,
		cLogin,
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
		C( Logout );
		C( Capability );
		C( Login);
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

	namespace _ {
		namespace _ {
			void SkipTag( flw::sIFlow &Flow )
			{
			qRH
				str::wString Dummy;
			qRB
				Dummy.Init();

				GetAlphanum_( Flow, Dummy );
			qRR
			qRT
			qRE
			}
		}

		base::eStatus HandleAnswer(
			flw::sIFlow &Flow,
			bso::sBool NoPending )
		{
			EOFT;

			if ( ( Flow.View() == '*' ) && !NoPending )
			{ 
				Flow.Skip();

				EOFT;

				if ( Flow.Get() != ' ' )
				qRGnr();

				return base::s_Pending;
			} else {
				if ( NoPending ) {
					if ( Flow.Get() != '*' )
						qRGnr();
				} else
					_::SkipTag( Flow );

				EOFT;

				if ( Flow.Get() != ' ' )
					qRGnr();

				return status_::Get( Flow );
			}
		}
	}

	base::eStatus HandleAnswer(
		rSession &Session,
		bso::sBool NoPending = false )
	{
		base::eStatus Status = _::HandleAnswer( Session.IFlow(), NoPending );

		if ( ( Status < base::s_amount ) ) {
			if ( Session.IFlow().View() != '\r' ) {
				if ( Session.IFlow().Get() != ' ' )
					qRGnr();

				if ( Session.IFlow().View() == '[' ) {
					Session.IFlow().Skip();
					Session.SetPendingStatus( Status );
					Status = base::s_Pending;
				}
			}
		}

		return Status;
	}
}

#include "cio.h"

base::eStatus muaima::base::GetCompletionStatus( rSession &Session )
{
	base::eStatus Status = Session.GetPendingStatus();

	if ( Status == s_Undefined )
		Status = _::HandleAnswer( Session );

	Session.SetPendingStatus( base::s_Undefined );

	return Status;
}


base::eStatus muaima::base::Connect( rSession &Session )
{
	return _::HandleAnswer( Session, true );
}

base::eStatus muaima::base::Logout( rSession &Session )
{
	_::SendCommand( Session.GetNextTag(), _::cLogout, Session.OFlow() );
	_::SendCFLR( Session.OFlow() );

	return _::HandleAnswer( Session );
}

base::eStatus muaima::base::Capability( rSession &Session )
{
	_::SendCommand( Session.GetNextTag(), _::cCapability, Session.OFlow() );
	_::SendCFLR( Session.OFlow() );

	return _::HandleAnswer( Session );
}

base::eStatus muaima::base::Login(
	const str::dString &Username,
	const str::dString &Password,
	rSession &Session )
{
	_::SendCommand(Session.GetNextTag(), _::cLogin, Session.OFlow() );

	Session.OFlow() << ' ' << Username << ' ' << Password;

	_::SendCFLR( Session.OFlow() );

	return _::HandleAnswer( Session );
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

