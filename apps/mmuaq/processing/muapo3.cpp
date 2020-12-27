/*
	Copyright (C) 2016-2017 Claude SIMON (http://zeusw.org/epeios/contact.html).

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
	using namespace base;

	qCDEF( char *, NL_, "\r\n" );

	qENUM( Command_ ) {
		cUser,
		cPass,
		cDele,
		cList,
		cRetr,
		cStat,
		cTop,
		cApop,
		cNoop,
		cQuit,
		cRset,
		cUidl,
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

#undef C

	void SendCommand_(
		eCommand_ Command,
		txf::rWFlow &Flow )
	{
	qRH
		str::wString CommandString;
	qRB
		CommandString.Init( GetLabel_( Command ) );

		str::ToUpper( CommandString );

		Flow << CommandString << ' ';
	qRR
	qRT
	qRE
	}

	void SkipAnswer_( flw::rRFlow &Flow )
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
		flw::rRFlow &Flow,
		bso::sBool SkipAnswer )	// Only used for an 'OK' answer.
	{
		switch ( Flow.Get() ) {
		case '+':
			Flow.Skip( 2 );	// To skip 'OK'.

			if ( SkipAnswer )
				SkipAnswer_( Flow );
			else if ( Flow.View() != CR_ )
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

	eIndicator Clean_( flw::rRFlow &Flow )
	{
		eIndicator Indicator = i_Undefined;

		Indicator = CleanBegin_( Flow, true );

		return Indicator;
	}

	eIndicator Authenticate_(
		const str::dString &User,
		const str::dString &Pass,
		flw::rRFlow &In,
		txf::rWFlow &Out )
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
}

eIndicator muapo3::base::Authenticate(
	const str::dString &Username,
	const str::dString &Password,
	fdr::rRWDriver &Server,
	hBody & Body )
{
	eIndicator Indicator = i_Undefined;
qRH
	flw::rDressedRFlow<> IFlow;
	txf::rWFlow OFlow;
qRB
	IFlow.Init( Server );
	OFlow.Init( Server );

	Indicator = Authenticate_( Username, Password, IFlow, OFlow );

	Body.Init( true, Server, false );
qRR
qRT
qRE
	return Indicator;
}

eIndicator muapo3::base::List(
	sNumber Number,
	fdr::rRWDriver &Server,
	bso::sBool SkipAnswer,
	hBody &Body )
{
	eIndicator Indicator = i_Undefined;
qRH
	flw::rDressedRFlow<> IFlow;
	txf::rWFlow OFlow;
qRB
	IFlow.Init( Server );
	OFlow.Init( Server );

	SendCommand_( cList, OFlow );

	if ( Number != 0 )
		OFlow << Number;

	OFlow << NL_<< txf::commit;

	if ( !( Indicator = CleanBegin_( IFlow, SkipAnswer ) ).IsTrue() ) {
		Body.Init( false, Server, false );
	} else
		Body.Init( SkipAnswer, Server, Number == 0 );
qRR
qRT
qRE
	return Indicator;
}

eIndicator muapo3::base::Retrieve(
	sNumber Number,
	fdr::rRWDriver &Server,
	bso::sBool SkipAnswer,
	hBody &Body )
{
	eIndicator Indicator = i_Undefined;
qRH
	flw::rDressedRFlow<> IFlow;
	txf::rWFlow OFlow;
qRB
	IFlow.Init( Server );
	OFlow.Init( Server );

	SendCommand_( cRetr, OFlow );
	OFlow << Number << NL_ << txf::commit;

	if ( ( Indicator = CleanBegin_( IFlow, SkipAnswer ) ).IsTrue() )
		Body.Init( SkipAnswer, Server, true );
qRR
qRT
qRE
	return Indicator;
}

eIndicator muapo3::base::Top(
	sNumber Number,
	bso::sUInt AmountOfLine,
	fdr::rRWDriver &Server,
	bso::sBool SkipAnswer,
	hBody &Body )
{
	eIndicator Indicator = i_Undefined;
qRH
	flw::rDressedRFlow<> IFlow;
	txf::rWFlow OFlow;
qRB
	IFlow.Init( Server );
	OFlow.Init( Server );

	SendCommand_( cTop, OFlow );
	OFlow << Number << ' ' << AmountOfLine << NL_ << txf::commit;

	if ( ( Indicator = CleanBegin_( IFlow, SkipAnswer ) ).IsTrue() )
		Body.Init( SkipAnswer, Server, true );
qRR
qRT
qRE
	return Indicator;
}

eIndicator muapo3::base::UIDL(
	sNumber Number,
	fdr::rRWDriver &Server,
	hBody &Body )
{
	eIndicator Indicator = i_Undefined;
qRH
	flw::rDressedRFlow<> IFlow;
	txf::rWFlow OFlow;
qRB
	IFlow.Init( Server );
	OFlow.Init( Server );

	SendCommand_( cUidl, OFlow );

	if ( Number != 0 )
		OFlow << Number;

	OFlow << NL_<< txf::commit;

	if ( !( Indicator = CleanBegin_( IFlow, Number == 0 ) ).IsTrue() ) {
		Body.Init( false, Server, false );
	} else
		Body.Init( Number == 0, Server, Number == 0 );
qRR
qRT
qRE
	return Indicator;
}

eIndicator muapo3::base::Quit(
	fdr::rRWDriver &Server,
	hBody &Body )
{
	eIndicator Indicator = i_Undefined;
qRH
	flw::rDressedRFlow<> IFlow;
	txf::rWFlow OFlow;
qRB
	IFlow.Init( Server );
	OFlow.Init( Server );

	SendCommand_( cQuit, OFlow );
	OFlow << NL_ << txf::commit;

	if ( ( Indicator = CleanBegin_( IFlow, false ) ).IsTrue() )
		Body.Init( false, Server, true );
qRR
qRT
qRE
	return Indicator;
}

bso::sBool muapo3::Authenticate(
	const str::dString &Username,
	const str::dString &Password,
	fdr::rRWDriver &Server,
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
	fdr::rRWDriver &Server,
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
			fdr::rRDriver &Driver,
			muabsc::cIndex &Callback )
		{
			bso::sBool Success = true;
		qRH
			flw::rDressedRFlow<> Flow;
			xtf::sRFlow XFlow;
			txmtbl::wLine Line;
		qRB
			Flow.Init( Driver );
			XFlow.Init( Flow, utf::f_Default );

			Line.Init();
			while ( txmtbl::GetLine( XFlow, Line, ' ' ) ) {
				if ( !Extract_( Line, Callback ) ) {
					Success = false;
					break;
				}

				Line.Init();
			}

			Success = true;
		qRR
		qRT
		qRE
			return Success;
		}
	}

	bso::sBool Get(
		fdr::rRWDriver &Server,
		muabsc::cIndex &Callback )
	{
		bso::sBool Success = false;
	qRH
		hBody Body;
	qRB
		if ( base::List( 0, Server, true, Body ) == iOK )
			Success = Extract_( Body.GetDriver(), Callback );
	qRR
	qRT
	qRE
		return Success;
	}
}

bso::sBool muapo3::GetIndexes(
	fdr::rRWDriver &Server,
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
	sNumber Number,
	fdr::rRWDriver &Server,
	hBody &Body,
	qRPN )
{
	bso::sBool Success = base::Top( Number, 0, Server, true, Body ).Boolean();

	if ( !Success )
		if ( qRPT )
			qRGnr();

	return Success;
}


bso::sBool muapo3::GetMessage(
	sNumber Number,
	fdr::rRWDriver &Server,
	hBody &Body,
	qRPN )
{
	bso::sBool Success = base::Retrieve( Number, Server, true, Body ).Boolean();

	if ( !Success )
		if ( qRPT )
			qRGnr();

	return Success;
}

bso::sBool muapo3::GetMessage(
	sNumber Number,
	fdr::rRWDriver &Server,
	str::dString &Message,
	qRPN )
{
	bso::sBool Success = false;
qRH
	hBody Body;
	flx::rStringODriver Driver;
qRB
	if ( GetMessage( Number, Server, Body, qRP ) ) {
		Driver.Init( Message, fdr::ts_Default );

		fdr::Copy(Body.GetDriver(),Driver );

		Success = true;
	}
qRR
qRT
qRE
	return Success;
}


namespace uidl_ {
	namespace {
		bso::sBool Extract_(
			const txmtbl::dLine &Line,
			cUIDL &Callback )
		{
			sNumber Number = 0;
			sdr::sRow Error = qNIL;

			if ( Line.Amount() != 2 )
				return false;

			Line( Line.First() ).ToNumber( Number, &Error );

			if ( Error != qNIL )
				return false;

			Callback.OnUIDL( Number, Line( Line.First( 1 ) ) );

			return true;
		}

		bso::sBool Extract_(
			fdr::rRDriver &Driver,
			cUIDL &Callback )
		{
			bso::sBool Success = true;
		qRH
			flw::rDressedRFlow<> Flow;
			xtf::sRFlow XFlow;
			txmtbl::wLine Line;
		qRB
			Flow.Init( Driver );
			XFlow.Init( Flow, utf::f_Default );

			Line.Init();
			while ( txmtbl::GetLine( XFlow, Line, ' ' ) ) {
				if ( !Extract_( Line, Callback ) ) {
					Success = false;
					break;
				}

				Line.Init();
			}
		qRR
		qRT
		qRE
			return Success;
		}
	}

	bso::sBool Get(
		fdr::rRWDriver &Server,
		cUIDL &Callback )
	{
		bso::sBool Success = false;
	qRH
		hBody Body;
	qRB
		if ( base::UIDL( 0, Server, Body ) == iOK )
			Success = Extract_( Body.GetDriver(), Callback );
	qRR
	qRT
	qRE
		return Success;
	}
}

bso::sBool muapo3::GetUIDLs(
	fdr::rRWDriver &Server,
	cUIDL &Callback,
	qRPN )
{
	if ( !uidl_::Get( Server, Callback ) )
		if ( qRPT )
			qRGnr();
		else
			return false;
	else
		return true;

	return false;	// To avoid a warning.
}

namespace get_number_for_uidl_{
	class rCallback
	: public cUIDL
	{
	private:
		str::wString UIDL_;
	protected:
		virtual void MUAPO3OnUIDL(
			sNumber Number,
			const str::dString &UIDL ) override
		{
			if ( UIDL == UIDL_ ) {
				if ( this->Number == 0 )
					this->Number = Number;
				else
					qRGnr();
			}
		}
	public:
		sNumber Number;
		void reset( bso::sBool P = true )
		{
			tol::reset( P, UIDL_);
			Number = 0;
		}
		qCVDTOR( rCallback );
		void Init( const str::dString &UIDL )
		{
			Number = 0;
			UIDL_.Init( UIDL );
		}
	};
}

sNumber muapo3::GetNumberForUIDL(
	const dUIDL &UIDL,
	fdr::rRWDriver &Server )
{
	sNumber Number = 0;
qRH
	get_number_for_uidl_::rCallback Callback;
qRB
	Callback.Init( UIDL );

	GetUIDLs( Server, Callback );

	Number = Callback.Number;
qRR
qRT
qRE
	return Number;
}



