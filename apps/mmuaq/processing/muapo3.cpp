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
#include "htp.h"
#include "txmtbl.h"

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

	void SkipAnswer_( flw::sIFlow &Flow )
	{
		bso::sBool Continue = !Flow.EndOfFlow();

		while ( Continue ) {
			switch ( Flow.Get() ) {
			case CR_:
				if ( !Flow.EndOfFlow() && Flow.View() == LF_ )
					Flow.Skip();
			case LF_:
				Continue = false;
				break;
			default:
				Continue = !Flow.EndOfFlow();
				break;
			}
		}
	}

	eIndicator CleanBegin_(
		flw::sIFlow &Flow,
		bso::sBool SkipAnswer )	// Only used for an 'OK' answer.
	{
		switch ( Flow.Get() ) {
		case '+':
			Flow.Skip( 2 );	// To skip 'OK'.

			if ( SkipAnswer )
				SkipAnswer_( Flow );
			else
				Flow.Skip( 1 );	// The space after the 'OK'.
			return iOK;
			break;
		case '-':
			Flow.Skip( 3 );	// To skip 'ERR'.

			if ( Flow.View() == ' ' )
				Flow.Skip();

			return iError;
			break;
		default:
			// Server returned a anwser which is not POP3 compliant.
			return iErroneous;
			break;
		}

		return i_Undefined;	// To avoid a warning.
	}

	eIndicator Clean_( flw::sIFlow &Flow )
	{
		eIndicator Indicator = i_Undefined;

		Indicator = CleanBegin_( Flow, true );

		return Indicator;
	}

	eIndicator Authenticate_(
		const str::dString &User,
		const str::dString &Pass,
		flw::sIFlow &In,
		txf::sOFlow &Out )
	{
		eIndicator Indicator = i_Undefined;

		if ( !( Indicator = Clean_( In ) ).IsTrue() )
			return Indicator;

		SendCommand_( cUser, Out );
		Out << User << NL_ << txf::commit;
		if ( !( Indicator = Clean_( In ) ).IsTrue() )
			return Indicator;

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

eIndicator muapo3::base::Authenticate(
	const str::dString &Username,
	const str::dString &Password,
	fdr::rIODriver &Server,
	hBody & Body )
{
	eIndicator Indicator = i_Undefined;
qRH
	flw::sDressedIFlow<> IFlow;
	txf::rOFlow OFlow;
qRB
	IFlow.Init( Server );
	OFlow.Init( Server );

	Indicator = Authenticate_( Username, Password, IFlow, OFlow );

	Body.Init( Server, false );
qRR
qRT
qRE
	return Indicator;
}

eIndicator muapo3::base::List(
	bso::sUInt Index,
	fdr::rIODriver &Server,
	bso::sBool SkipAnswer,
	hBody &Body )
{
	eIndicator Indicator = i_Undefined;
qRH
	flw::sDressedIFlow<> IFlow;
	txf::rOFlow OFlow;
qRB
	IFlow.Init( Server );
	OFlow.Init( Server );

	SendCommand_( cList, OFlow );

	if ( Index != 0 )
		OFlow << Index;
	
	OFlow << NL_<< txf::commit;

	if ( !( Indicator = CleanBegin_( IFlow, SkipAnswer ) ).IsTrue() ) {
		Body.Init( Server, false );
		qRReturn;
	} else
		Body.Init( Server, Index == 0 );
qRR
qRT
qRE
	return Indicator;
}

eIndicator muapo3::base::Retrieve(
	bso::sUInt Index,
	fdr::rIODriver &Server,
	bso::sBool SkipAnswer,
	hBody &Body )
{
	eIndicator Indicator = i_Undefined;
qRH
	flw::sDressedIFlow<> IFlow;
	txf::rOFlow OFlow;
qRB
	IFlow.Init( Server );
	OFlow.Init( Server );

	SendCommand_( cRetr, OFlow );
	OFlow << Index << NL_ << txf::commit;

	if ( !( Indicator = CleanBegin_( IFlow, SkipAnswer ) ).IsTrue() )
		qRReturn;

	Body.Init( Server, true );
qRR
qRT
qRE
	return Indicator;
}

eIndicator muapo3::base::Top(
	bso::sUInt Index,
	bso::sUInt AmountOfLine,
	fdr::rIODriver &Server,
	bso::sBool SkipAnswer,
	hBody &Body )
{
	eIndicator Indicator = i_Undefined;
qRH
	flw::sDressedIFlow<> IFlow;
	txf::rOFlow OFlow;
qRB
	IFlow.Init( Server );
	OFlow.Init( Server );

	SendCommand_( cTop, OFlow );
	OFlow << Index << ' ' << AmountOfLine << NL_ << txf::commit;

	if ( !( Indicator = CleanBegin_( IFlow, SkipAnswer ) ).IsTrue() )
		qRReturn;

	Body.Init( Server, true );
qRR
qRT
qRE
	return Indicator;
}

eIndicator muapo3::base::UIDL(
	bso::sUInt Index,
	fdr::rIODriver &Server,
	hBody &Body )
{
	eIndicator Indicator = i_Undefined;
qRH
	flw::sDressedIFlow<> IFlow;
	txf::rOFlow OFlow;
qRB
	IFlow.Init( Server );
	OFlow.Init( Server );

	SendCommand_( cUidl, OFlow );

	if ( Index != 0 )
		OFlow << Index;
	
	OFlow << NL_<< txf::commit;

	if ( !( Indicator = CleanBegin_( IFlow, Index == 0 ) ).IsTrue() ) {
		Body.Init( Server, false );
		qRReturn;
	} else
		Body.Init( Server, Index == 0 );
qRR
qRT
qRE
	return Indicator;
}

eIndicator muapo3::base::Quit(
	fdr::rIODriver &Server,
	hBody &Body )
{
	eIndicator Indicator = i_Undefined;
qRH
	flw::sDressedIFlow<> IFlow;
	txf::rOFlow OFlow;
qRB
	IFlow.Init( Server );
	OFlow.Init( Server );

	SendCommand_( cQuit, OFlow );
	OFlow << NL_ << txf::commit;

	if ( !( Indicator = CleanBegin_( IFlow, false ) ).IsTrue() )
		qRReturn;

	Body.Init( Server, true );
qRR
qRT
qRE
	return Indicator;
}

bso::sBool muapo3::Authenticate(
	const str::dString &Username,
	const str::dString &Password,
	fdr::rIODriver &Server,
	qRPN )
{
	bso::sBool Success = false;
qRH
	hBody Body;
qRB
	Success = base::Authenticate(Username, Password, Server, Body ).Boolean();

	if ( !Success )
		if ( qRPT )
			qRGnr();
qRR
qRT
qRE
	return Success;
}

bso::sBool muapo3::Quit(
	fdr::rIODriver &Server,
	qRPN )
{
	bso::sBool Success = false;
qRH
	hBody Body;
qRB
	Success = base::Quit( Server, Body ).Boolean();

	if ( !Success )
		if ( qRPT )
			qRGnr();
qRR
qRT
qRE
	return Success;
}


namespace indexes_ {
	namespace {
		bso::sBool Extract_(
			const txmtbl::dLine &Line,
			muabsc::cIndex &Callback )
		{
			muabsc::sIndex Index = 0;
			sdr::sRow Error = qNIL;

			if ( Line.Amount() == 0 )
				return false;

			Line( Line.First() ).ToNumber( Index, &Error );

			if ( Error != qNIL )
				return false;

			Callback.OnIndex( Index );

			return true;
		}

		bso::sBool Extract_(
			fdr::rIDriver &Driver,
			muabsc::cIndex &Callback )
		{
			bso::sBool Success = false;
		qRH
			flw::sDressedIFlow<> Flow;
			xtf::sIFlow XFlow;
			txmtbl::wLine Line;
		qRB
			Flow.Init( Driver );
			XFlow.Init( Flow, utf::f_Default );

			Line.Init();
			while ( txmtbl::GetLine( XFlow, Line, ' ' ) ) {
				if ( !Extract_( Line, Callback ) )
					qRReturn;

				Line.Init();
			}

			Success = true;
		qRR
		qRT
		qRE
			return Success;
		}
	}

#define H( indicator )	if ( ( indicator ) != iOK ) qRReturn
	bso::sBool Get(
		fdr::rIODriver &Server,
		muabsc::cIndex &Callback )
	{
		bso::sBool Success = false;
	qRH
		hBody Body;
	qRB
		H( base::List( 0, Server, true, Body ) );

		if ( !Extract_(Body.GetDriver(), Callback ) )
			qRReturn;

		Success = true;
	qRR
	qRT
	qRE
		return Success;
	}
#undef H
}

bso::sBool muapo3::GetIndexes(
	fdr::rIODriver &Server,
	muabsc::cIndex &Callback,
	qRPN )
{
	if ( !indexes_::Get( Server, Callback ) )
		if ( qRPT )
			qRGnr();
		else
			return false;
	else
		return true;

	return false;	// To avoid a warning.
}

bso::sBool muapo3::GetHeader(
	bso::sUInt Index,
	fdr::rIODriver &Server,
	hBody &Body,
	qRPN )
{
	bso::sBool Success = base::Top(Index, 0, Server, true, Body ).Boolean();

	if ( !Success )
		if ( qRPT )
			qRGnr();

	return Success;
}

