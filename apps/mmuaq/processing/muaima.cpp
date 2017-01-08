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
qRH
	flw::sDressedIFlow<> Flow;
qRB
	Flow.Init( D_() );

	EOFT;

	if ( Flow.View() != '*' )
		Response = r_None;
	else {
		Flow.Skip();

		if ( Flow.Get() != ' ' )
			qRGnr();

		Response = response_::Get( Flow );
	}
qRR
qRT
qRE
	return Response;
}

namespace _ {
	qENUM( Command_ ) {
		cLogout,
		cCapability,
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
		default:
			qRGnr();
			break;
		}

		return NULL;	// To avoid a warning.
	}

	namespace send_command_ {
		void Send(
			const str::dString &Tag,
			eCommand_ Command,
			txf::sOFlow &Flow )
		{
		qRH
			str::wString CommandString;
		qRB
			CommandString.Init( GetLabel_( Command ) );

			str::ToUpper( CommandString );

			Flow << Tag << ' ' << CommandString << ' ' << "\r\n";
		qRR
		qRT
		qRE
		}
	}

	void SendCommand(
		const str::dString &Tag,
		eCommand_ Command,
		fdr::rODriver &Driver )
	{
	qRH
		txf::rOFlow Flow;
	qRB
		Flow.Init( Driver );

		send_command_::Send( Tag, Command, Flow );
	qRR
	qRT
	qRE
	}

	namespace handle_answer_ {
		base::eIndicator Handle( flw::sIFlow &Flow )
		{
			if ( Flow.EndOfFlow() )
				return base::iErroneous;

			return base::iOK;
		}
	}

	base::eIndicator HandleAnswer( rSession &Session )
	{
		base::eIndicator Indicator = base::i_Undefined;
	qRH
		flw::sDressedIFlow<> Flow;
	qRB
		Flow.Init(Session.Driver() );

		Indicator = handle_answer_::Handle( Flow );
	qRR
	qRT
	qRE
		return Indicator;
	}
}

#include "cio.h"

base::eIndicator muaima::base::Connect( rSession &Session )
{
	return _::HandleAnswer( Session );
}

base::eIndicator muaima::base::Logout( rSession &Session )
{
	_::SendCommand( Session.GetNextTag(), _::cLogout, Session.Driver() );

	return _::HandleAnswer( Session );
}

base::eIndicator muaima::base::Capability( rSession &Session )
{
	_::SendCommand( Session.GetNextTag(), _::cCapability, Session.Driver() );

	return _::HandleAnswer( Session );
}

qGCTOR( muaima )
{
	response_::FillAutomat();
}

