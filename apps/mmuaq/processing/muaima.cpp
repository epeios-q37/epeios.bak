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

namespace response_ {
	namespace _ {
		stsfsm::wAutomat Automat;

		namespace _ {
#define C( name )	case r##name : return #name; break
 
			const char *GetLabel( eResponse Response )
			{
				switch ( Response ) {
				C( Capability );
				default:
					qRFwk();
					break;
				}
 
				return NULL;	// To avoid a warning.
			}

#undef C
		}

		const str::dString &GetUCLabel(
			eResponse Response,
			str::dString &Label )
		{
			Label.Append( _::GetLabel( Response ) );

			str::ToUpper( Label );

			return Label;
		}

		eResponse GetResponse( const str::dString &Pattern )
		{
			return stsfsm::GetId( Pattern, Automat, r_Undefined, r_amount );
		}
	}

	void FillAutomat( void )
	{
		_::Automat.Init();
		stsfsm::Fill<eResponse>( _::Automat, r_amount, _::GetUCLabel );
	}

	eResponse Get( flw::sIFlow &Flow )
	{
		eResponse Response = r_Undefined;
	qRH
		str::wString Pattern;
		flw::sByte Byte = 0;
	qRB
		EOFT;

		Pattern.Init();

		while ( ( Byte = Flow.Get() ) != ' ' ) {
			Pattern.Append( Byte );

			EOFT;
		}

		Response = _::GetResponse( Pattern );
	qRR
	qRT
	qRE
		return Response;
	}
}

eResponse muaima::rSession::GetResponse( void )
{
	eResponse Response = r_Undefined;
	flw::sIFlow &Flow = IFlow_;

	EOFT;

	if ( Flow.View() != '*' )
		Response = r_None;
	else {
		Flow.Skip();

		if ( Flow.Get() != ' ' )
			qRGnr();

		Response = response_::Get( Flow );
	}

	return Response;
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

	base::eIndicator HandleAnswer( flw::sIFlow &Flow )
	{
		if ( Flow.EndOfFlow() )
			return base::iErroneous;

		return base::iOK;
	}
}

#include "cio.h"

base::eIndicator muaima::base::Connect( rSession &Session )
{
	return _::HandleAnswer( Session.IFlow() );
}

base::eIndicator muaima::base::Logout( rSession &Session )
{
	_::SendCommand( Session.GetNextTag(), _::cLogout, Session.OFlow() );
	_::SendCFLR( Session.OFlow() );

	return _::HandleAnswer( Session.IFlow() );
}

base::eIndicator muaima::base::Capability( rSession &Session )
{
	_::SendCommand( Session.GetNextTag(), _::cCapability, Session.OFlow() );
	_::SendCFLR( Session.OFlow() );

	return _::HandleAnswer( Session.IFlow() );
}

base::eIndicator muaima::base::Login(
	const str::dString &Username,
	const str::dString &Password,
	rSession &Session )
{
	_::SendCommand(Session.GetNextTag(), _::cLogin, Session.OFlow() );

	Session.OFlow() << ' ' << Username << ' ' << Password;

	_::SendCFLR( Session.OFlow() );

	return _::HandleAnswer( Session.IFlow() );
}

qGCTOR( muaima )
{
	response_::FillAutomat();
}

