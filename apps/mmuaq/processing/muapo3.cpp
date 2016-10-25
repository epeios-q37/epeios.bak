/*
	Copyright (C) 2016 Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of 'MMUAq' software.

    'MUAq' is free software: you can redistribute it and/or modify it
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

#include "muapo3.h"

#include "flx.h"

using namespace muapo3;

namespace {
	qCDEF( char *, NL_, "\r\n" );
}

#define C( name )\
	case c##name:\
		return #name;\
		break


const char *muapo3::GetLabel( eCommand Command )
{
	switch ( Command ) {
	C( User );
	C( Pass );
	C( Dele );
	C( List );
	C( Retr );
	C( Stat );
	C( Top );
	C( Apop );
	C( Noop );
	C( Quit );
	C( Rset );
	C( Uidl );
	C( Capa );
	default:
		qRGnr();
		break;
	}

	return NULL;	// To avoid a warning.
}

namespace {
	void SendCommand_(
		eCommand Command,
		txf::sOFlow &Flow )
	{
	qRH
		str::wString CommandString;
	qRB
		CommandString.Init( GetLabel( Command ) );

		str::ToUpper( CommandString );

		Flow << CommandString << ' ';
	qRR
	qRT
	qRE
	}

	bso::sBool CleanBegin_( flw::sIFlow &Flow )
	{
		switch ( Flow.Get() ) {
		case '+':
			Flow.Skip( 3 );
			return true;
			break;
		case '-':
			Flow.Skip( 4 );
			return false;
			break;
		default:
			qRGnr();
			break;
		}

		return false;	// To avoid a warning.
	}

	bso::sBool Clean_( flw::sIFlow &Flow )
	{
		bso::sBool Success = CleanBegin_( Flow );

		while ( Flow.Get() != '\n' );

		return Success;
	}

	bso::sBool Authenticate_(
		const str::dString &User,
		const str::dString &Pass,
		flw::sIFlow &In,
		txf::sOFlow &Out )
	{
		if ( !Clean_( In ) )
			return false;

		SendCommand_( cUser, Out );
		Out << User << NL_ << txf::commit;
		if ( !Clean_( In ) )
			return false;

		SendCommand_( cPass, Out );
		Out << Pass << NL_ << txf::commit;
		return Clean_( In );
	}

	bso::sSize GetSize_( flw::sIFlow &Flow )
	{
		fdr::sSize Size = 0;
		fdr::sByte C = 0;

		while ( isdigit( C = Flow.Get() ) )
			Size = Size * 10 + C - '0';

		while ( Flow.Get() != '\n' );

		return Size;
	}
}

bso::sBool muapo3::Authenticate(
	const str::dString &Username,
	const str::dString &Password,
	fdr::rIODriver &Server )
{
	bso::sBool Success = false;
qRH
	flw::sIFlow IFlow;
	txf::rOFlow OFlow;
qRB
	IFlow.Init( Server );
	OFlow.Init( Server );

	Success = Authenticate_( Username, Password, IFlow, OFlow );
qRR
qRT
qRE
	return Success;
}


bso::sBool muapo3::List( fdr::rIODriver &Server )
{
	bso::sBool Success = false;
qRH
	flw::sIFlow IFlow;
	txf::rOFlow OFlow;
qRB
	IFlow.Init( Server );
	OFlow.Init( Server );

	SendCommand_( cList, OFlow );
	OFlow << NL_ << txf::commit;

	if ( !CleanBegin_( IFlow ) )
		qRReturn;

	Success = true;
qRR
qRT
qRE
	return Success;
}

bso::sBool muapo3::Retrieve(
	bso::sUInt Index,
	fdr::rIODriver &Server,
	hMessage &Message )
{
	bso::sBool Success = false;
qRH
	flw::sIFlow IFlow;
	txf::rOFlow OFlow;
qRB
	IFlow.Init( Server );
	OFlow.Init( Server );

	SendCommand_( cRetr, OFlow );
	OFlow << Index << NL_ << txf::commit;

	if ( !CleanBegin_( IFlow ) )
		qRReturn;

//	IFlow.Skip();	// Space after the 'OK'.
	/*
	if ( ( Size = GetSize_( IFlow ) ) == 0 )
		qRReturn;
		*/
	Message.Init( Server );

	Success = true;
qRR
qRT
qRE
	return Success;
}

bso::sBool muapo3::Top(
	bso::sUInt Index,
	bso::sUInt AmountOfLine,
	fdr::rIODriver &Server,
	hMessage &Message )
{
	bso::sBool Success = false;
qRH
	flw::sIFlow IFlow;
	txf::rOFlow OFlow;
qRB
	IFlow.Init( Server );
	OFlow.Init( Server );

	SendCommand_( cTop, OFlow );
	OFlow << Index << ' ' << AmountOfLine << NL_ << txf::commit;

	if ( !CleanBegin_( IFlow ) )
		qRReturn;

//	IFlow.Skip();	// Space after the 'OK'.
	/*
	if ( ( Size = GetSize_( IFlow ) ) == 0 )
		qRReturn;
		*/
	Message.Init( Server );

	Success = true;
qRR
qRT
qRE
	return Success;
}

bso::sBool muapo3::Quit( fdr::rIODriver &Server )
{
	bso::sBool Success = false;
qRH
	flw::sIFlow IFlow;
	txf::rOFlow OFlow;
qRB
	IFlow.Init( Server );
	OFlow.Init( Server );

	SendCommand_( cQuit, OFlow );
	OFlow << NL_ << txf::commit;

	if ( !CleanBegin_( IFlow ) )
		qRReturn;

	Success = true;
qRR
qRT
qRE
	return Success;
}






