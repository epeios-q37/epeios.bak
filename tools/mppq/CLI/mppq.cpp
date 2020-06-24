/*
	Copyright (C) 2017 by Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of 'MPPq'.

    'MPPq' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'MPPq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'MPPq'.  If not, see <http://www.gnu.org/licenses/>.
*/

# include "mppq.h"

#include "registry.h"

#include "scle.h"
#include "sclm.h"
#include "sclt.h"

#include "err.h"
#include "cio.h"
#include "epsmsc.h"
#include "xpp.h"
#include "fnm.h"
#include "flf.h"
#include "crt.h"

using cio::CErr;
using cio::COut;
using cio::CIn;

SCLI_DEF( mppq, NAME_LC, NAME_MC );

namespace {
	typedef str::dStrings dSlide;
	qW( Slide );

	void PrintHeader_( void )
	{
		COut << NAME_MC " V" VERSION << " (" WEBSITE_URL ")" << txf::nl;
		COut << "Copyright (C) " COPYRIGHT << txf::nl;
		COut << txf::pad << "Build : " __DATE__ " " __TIME__ << " (" << cpe::GetDescription() << ')' << txf::nl;
	}

	void PrintSlideSeparator_( txf::sWFlow &Flow )
	{
		// 'Marp' slide separator.
		Flow << "---" << txf::nl;
	}

	void Display_(
		sdr::sRow Last,
		const dSlide &Slide,
		txf::sWFlow &Flow,
		bso::sBool DisplaySeparator )
	{
		sdr::sRow Row = Slide.First();

		if( ( Row != qNIL ) && ( Last == Row ) )
			Flow << Slide( Row );
		else {
			while ( Row != Last ) {
				Flow << Slide( Row ) << txf::nl;

				Row = Slide.Next( Row );
			}
		}

		if ( DisplaySeparator )
			PrintSlideSeparator_( Flow );
	}

	bso::sBool IsListItem_( const str::dString &Line )
	{
		bso::sBool Is = false;
	qRH;
		flx::sStringRFlow Flow;
		bso::sBool DigitFound = false;
		bso::sBool SpaceAwaited = false;
		bso::sBool Continue = true;
	qRB;
		Flow.Init( Line );

		while ( Continue && !Flow.EndOfFlow() ) {
			if ( SpaceAwaited ) {
				Continue = false;
				Is = Flow.Get() == ' ';
			} else if ( DigitFound ) {
				char C = Flow.Get();

				if ( !isdigit( C ) ) {
					if ( C == '.' )
						SpaceAwaited = true;
					else
						Continue = false;
				}
			} else {
				char C = Flow.Get();

				if ( isdigit( C ) )
					DigitFound = true;
				else if ( C == '*' )
					SpaceAwaited = true;
				else if ( C == '-' )
					SpaceAwaited = true;
				else if ( C != ' ' )
					Continue = false;
			}
		}
	qRR;
	qRT;
	qRE;
		return Is;
	}

	bso::sBool IsComment_( const str::dString &Line )
	{
		sdr::sRow Row = Line.First();

		while ( (Row != qNIL) && (Line( Row ) == ' ') )
			Row = Line.Next( Row );

		if ( Row == qNIL )
			return false;

		return Line.Search( str::wString( "<!--" ) ) == Row;
	}


	sdr::sRow NextSkippingComments_(
		const str::dStrings &Slide,
		sdr::sRow Row )
	{
		do {
			Row = Slide.Next( Row );
		} while ( (Row != qNIL) && IsComment_( Slide( Row ) ) );

		return Row;
	}

	void Handle_(
		const str::dStrings &Slide,
		txf::sWFlow &Flow )
	{
		sdr::sRow Row = Slide.First();
		bso::sBool ListItemDetected = false;

		while ( Row != qNIL ) {
			if (IsListItem_(Slide(Row))) {
				Display_(Row, Slide, Flow, true );

				ListItemDetected = true;
			} else if ( ListItemDetected ) {
				ListItemDetected = false;
				Display_( Row, Slide, Flow, true );
			}

			Row = NextSkippingComments_( Slide, Row );
		}

		Display_( qNIL, Slide, Flow, false );
		Flow.Commit();
	}

	void Process_(
		flw::rRFlow &RFlow,
		txf::sWFlow &WFlow )
	{
	qRH;
		xtf::sRFlow Flow;
		str::wString Line;
		wSlide Slide;
		bso::sBool Continue = true;
		bso::sBool SeparatorPending = false;
		bso::sBool IsCode = false;
	qRB;
		Flow.Init( RFlow, utf::f_Guess );

		Slide.Init();

		while ( Continue ) {
			if ( Flow.EndOfFlow() )
				Continue = false;
			else {
				Line.Init();
				Flow.GetLine( Line );

				if ( true)  {
					if (Line.Search(str::wString("---")) == Line.AmountOfLeadChars()) {
						Handle_( Slide, WFlow );

						SeparatorPending = true;

						Slide.Init();
					} else {
						if (Line.Search(str::wString("```")) == Line.AmountOfLeadChars()) {
							IsCode = !IsCode;

							if (IsCode)
								Slide.Append(str::wString('\n'));
						}

						if ( SeparatorPending ) {
							PrintSlideSeparator_( WFlow );
							SeparatorPending = false;
						}

						Slide.Append( Line );
					}
				}
			}
		}

		if ( Slide.Amount() != 0 ) {
			if ( SeparatorPending ) {
				PrintSlideSeparator_( WFlow );
				SeparatorPending = false;
			}

			Handle_( Slide, WFlow );
		}
	qRR;
	qRT;
	qRE;
	}

	void Process_(
		const char *Source,
		const char *Target )
	{
	qRH;
		flf::rRFlow SFlow;
		flf::rWDriver TFDriver;	// Target file driver.
		txf::rWFlow TTFlow;	// Target text flow.
		bso::sBool BackedUp = false;
	qRB;
		if ( Source != NULL ) {
			if ( SFlow.Init( Source, err::hUserDefined ) != tol::rSuccess )
				sclm::ReportFileOpeningErrorAndAbort( Source );
		}

		if ( Target != NULL ) {
			sclm::CreateBackupFile( Target );

			BackedUp = true;

			if ( TFDriver.Init( Target, err::hUserDefined ) != tol::rSuccess )
				sclm::ReportFileOpeningErrorAndAbort( Target );

			TTFlow.Init( TFDriver );
		}


		Process_(
			Source == NULL ? CIn.Flow() : SFlow,
			Target == NULL ? COut : TTFlow );
	qRR;
		if ( BackedUp ) {
			tol::reset( TFDriver, TTFlow );
			sclm::RecoverBackupFile( Target );
		}
	qRT;
	qRE;
	}

	void Process_( void )
	{
	qRH;
		str::wString Input, Output;
		qCBUFFERh InputBuffer, OutputBuffer;
	qRB;
		tol::Init( Input );
		sclm::OGetValue( registry::parameter::Input, Input );

		tol::Init( Output );
		sclm::OGetValue( registry::parameter::Output, Output );

		Process_(
			Input.Amount() != 0 ? Input.Convert( InputBuffer ) : NULL,
			Output.Amount() != 0 ? Output.Convert( OutputBuffer ) : NULL );
	qRR;
	qRT;
	qRE;
	}
}

const scli::sInfo &sclt::SCLTInfo( void )
{
	return mppq::Info;
}

#define C( name )\
	else if ( Command == #name )\
		name##_()

int sclt::SCLTMain(
	const str::dString &Command,
	const sclt::fOddities &Oddities )
{
	int ExitValue = EXIT_FAILURE;
qRH
qRB
	if ( Command == "Version" )
		PrintHeader_();
	else if ( Command == "License" )
		epsmsc::PrintLicense( NAME_MC );
	C( Process );
	else
		qRGnr();

	ExitValue = EXIT_SUCCESS;
qRR
qRT
qRE
	return ExitValue;
}
