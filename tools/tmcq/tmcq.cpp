/*
	Copyright (C) 2007-2011, 2015 Claude SIMON (http://q37.info/contact/).

	This file is part of tmcq.

    tmcq is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

    tmcq is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with tmcq.  If not, see <http://www.gnu.org/licenses/>
*/

#include "registry.h"

#include "mthtmc.h"

#include "scltool.h"
#include "sclerror.h"

#include "err.h"
#include "cio.h"
#include "epsmsc.h"
#include "xpp.h"
#include "fnm.h"
#include "flf.h"

using cio::CErr;
using cio::COut;
using cio::CIn;

# define NAME_MC			"TMCq"
# define NAME_LC			"tmcq"
# define NAME_UC			"TMCQ"
# define WEBSITE_URL		"http://q37.info/"
# define AUTHOR_NAME		"Claude SIMON"
# define AUTHOR_CONTACT		"http://q37.info/contact/"
# define OWNER_NAME			"Claude SIMON"
# define OWNER_CONTACT		"http://q37.info/contact/"
# define COPYRIGHT			COPYRIGHT_YEARS " " OWNER_NAME " (" OWNER_CONTACT ")"	

# if 0	// Retrieved from registry.
#define FORMAT_ENV			"ETMCCALC_FORMAT"
#define PUNCTUATIONS_ENV	"ETMCCALC_PUNCTUATIONS"

#define FCP_MARKER	'P'
#define FCP_FORMAT	"%2H:%2m:%2s%p%2f"

#define AE_MARKER	'E'
#define AE_FORMAT	"%2H%p%2m%p%2s%p%2f"

#define DEFAULT_FORMAT			"%P"
#define DEFAULT_PUNCTUATIONS	";:"
# endif


static void PrintHeader_( void )
{
	COut << NAME_MC " V" VERSION << " (" WEBSITE_URL ")" << txf::nl;
	COut << "Copyright (C) " COPYRIGHT << txf::nl;
	COut << txf::pad << "Build : " __DATE__ " " __TIME__ << " (" << cpe::GetDescription() << ')' << txf::nl;
}

namespace {

	void Process_(
		const str::string_ &FPS,
		const str::strings_ &Items,
		const str::string_ &Format,
		const mthtmc::tokens__ &Tokens )
	{
	qRH
		ctn::qCMITEMsl( str::string_ ) Item;
		sdr::row__ Row = qNIL;
		mthtmc::wXFPS OutputXFPS, XFPS;
		mthtmc::wTimecode Timecode, TotalTimecode;
		sdr::row__ Position = qNIL;
		mthtmc::wPeriod Period;
	qRB
		Row = Items.First();

		Item.Init( Items );

		if ( Row == qNIL )
			qRGnr();

		Position = FPS.First();

		if ( !OutputXFPS.Init(FPS, Position) || ( Position != qNIL ) )
			sclmisc::ReportAndAbort( "BadFPS", FPS );

		XFPS.Init( OutputXFPS );

		Position = Item( Row ).First();

		Period.Init();
		mthtmc::Convert( XFPS, Period );

		TotalTimecode.Init();

		mthtmc::Convert( mthitg::integer( 0 ), Period, TotalTimecode );

		while ( Row != qNIL ) {
			Position = Item( Row ).First();

			Timecode.Init();

			if ( ( !mthtmc::Convert( Item( Row ), Position, XFPS, Timecode ) ) || ( Position != qNIL ) )
				sclmisc::ReportAndAbort("BadTimecode", Item( Row ) );

			mthtmc::Add( TotalTimecode, Timecode, TotalTimecode );

	//		cio::cout << Timecode << txf::tab << TotalTimecode << txf::nl;
	//		XFPS = Timecode.XFPS;

			Row = Items.Next( Row );
		}

		if ( !mthtmc::Display( TotalTimecode, OutputXFPS, Format, Tokens, cio::COut ) )
		{
			cio::CErr << '\'' << Format << '\'' << " is not a valid format specification !" << txf::nl;
			qRReturn;
		}

		cio::COut << txf::nl << txf::commit;
	qRR
	qRT
	qRE
	}

	void Convert_(
		const str::string_ &RawTokens,
		mthtmc::tokens__ &Tokens )
	{
		bso::char__ DS = tol::GetLocaleDecimalSeparator();

		switch ( RawTokens.Amount() ) {
		case 1 :
			Tokens.Init( DS, RawTokens( RawTokens.First() ) );
			break;
		case 2:
			Tokens.Init( DS, RawTokens( RawTokens.First() ), RawTokens( RawTokens.First( 1 ) ) );
			break;
		case 3:
			Tokens.Init( DS, RawTokens( RawTokens.First() ), RawTokens( RawTokens.First( 1 ) ), RawTokens( RawTokens.First( 2 ) ) );
			break;
		default:
			sclmisc::ReportAndAbort( "BadTokens", RawTokens );
			break;
		}

	}

	void Process_( void )
	{
	qRH
		str::string FPS, FormatAlias, Format, RawTokens;
		str::strings Timecodes;
		bso::bool__ Missing = false;
		mthtmc::tokens__ Tokens;
	qRB
		FPS.Init();
		sclmisc::MGetValue( registry::FPS, FPS );

		FormatAlias.Init();
		Missing = sclmisc::OGetValue( registry::FormatAlias, FormatAlias ) == NULL;

		Format.Init();
		if ( Missing )
			sclmisc::MGetValue( registry::Format, Format );
		else
			sclmisc::MGetValue(rgstry::tentry___( registry::PredefinedFormat, FormatAlias ), Format );

		RawTokens.Init();
		sclmisc::MGetValue( registry::Tokens, RawTokens );

		Tokens.Init();
		Convert_( RawTokens, Tokens );

		Timecodes.Init();
		if ( !sclmisc::GetValues( registry::Timecode, Timecodes ) )
			sclmisc::ReportAndAbort("TooFewArguments");

		Process_( FPS, Timecodes, Format, Tokens );
	qRR
	qRT
	qRE
	}

	void DisplayHelpMessage_( const char *Message )
	{
	qRH
		str::string Translation;
	qRB
		Translation.Init();
		sclmisc::GetBaseTranslation( Message, Translation );
		cio::COut << Translation;
	qRR
	qRT
	qRE
	}

	void HelpTokens_( void )
	{
		DisplayHelpMessage_( "TokensHelpMessage" );
	}

	void HelpFormat_( void )
	{
		DisplayHelpMessage_( "FormatHelpMessage" );
	}

}

#define C( name )\
	else if ( Command == #name )\
		name##_()
		

int scltool::SCLTOOLMain(
	const str::string_ &Command,
	const scltool::oddities__ &Oddities )
{
	int ExitValue = EXIT_FAILURE;
qRH
qRB
	if ( Command == "Version" )
		PrintHeader_();
	else if ( Command == "License" )
		epsmsc::PrintLicense( NAME_MC );
	C( Process );
	C( HelpFormat );
	C( HelpTokens );
	else
		qRGnr();

	ExitValue = EXIT_SUCCESS;
qRR
qRT
qRE
	return ExitValue;
}

const char *sclmisc::SCLMISCTargetName = NAME_LC;
const char *sclmisc::SCLMISCProductName = NAME_MC;

