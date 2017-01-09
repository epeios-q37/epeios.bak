/*
	Copyright (C) 2016-2017 by Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of 'MMUAq'.

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

#include "imap.h"
#include "registry.h"
#include "pop3.h"

#include "muaima.h"
#include "muaimf.h"
#include "muapo3.h"

#include "scltool.h"
#include "sclerror.h"

#include "err.h"
#include "cio.h"
#include "csdbnc.h"
#include "epsmsc.h"
#include "xpp.h"
#include "fnm.h"
#include "flf.h"

using cio::CErr;
using cio::COut;
using cio::CIn;

# define NAME_MC			"MMUAq"
# define NAME_LC			"mmuaq"
# define NAME_UC			"MMUAQ"
# define WEBSITE_URL		"http://q37.info"
# define AUTHOR_NAME		"Claude SIMON"
# define AUTHOR_CONTACT		"http://q37.info/contact/"
# define OWNER_NAME			"Claude SIMON"
# define OWNER_CONTACT		"http://q37.info/contact/"
# define COPYRIGHT			COPYRIGHT_YEARS " " OWNER_NAME " (" OWNER_CONTACT ")"	

namespace {
	void PrintHeader_( void )
	{
		COut << NAME_MC " V" VERSION << " (" WEBSITE_URL ")" << txf::nl;
		COut << "Copyright (C) " COPYRIGHT << txf::nl;
		COut << txf::pad << "Build : " __DATE__ " " __TIME__ << " (" << cpe::GetDescription() << ')' << txf::nl;
	}

	void Test_( void )
	{
		cio::COut << "Test" << txf::nl;
	}

	void B64Encode_( void )
	{
	qRH
		str::wString Input, Output;
		sclmisc::rIDriverRack IRack;
		sclmisc::rODriverRack ORack;
		cdgb64::rEncodingODriver Encoder;
	qRB
		Input.Init();
		sclmisc::OGetValue( registry::parameter::Input, Input );

		Output.Init();
		sclmisc::OGetValue( registry::parameter::Output, Output );

		Encoder.Init( ORack.Init( Output ), cdgb64::fOriginal );
		fdr::Copy( IRack.Init( Input ), Encoder );
	qRR
		IRack.HandleError();
		ORack.HandleError();
	qRT
	qRE
	}

	void B64Decode_( void )
	{
	qRH
		str::wString Input, Output;
		sclmisc::rIDriverRack IRack;
		sclmisc::rODriverRack ORack;
		cdgb64::rDecodingIDriver Decoder;
	qRB
		Input.Init();
		sclmisc::OGetValue( registry::parameter::Input, Input );

		Output.Init();
		sclmisc::OGetValue( registry::parameter::Output, Output );

		Decoder.Init( IRack.Init( Input ) );
		fdr::Copy( Decoder, ORack.Init( Output ) );
	qRR
		IRack.HandleError();
		ORack.HandleError();
	qRT
	qRE
	}

	void POP3List_( void )
	{
		return pop3::List();
	}

	void POP3Retrieve_( void )
	{
		return pop3::Retrieve();
	}

	void POP3Top_( void )
	{
		return pop3::Top();
	}

	void POP3UIDL_( void )
	{
		return pop3::UIDL();
	}

	void IMAPCapability_( void )
	{
		return imap::Capability();
	}

	void NextTag_( void )
	{
	qRH
		str::wString Tag;
	qRB
		Tag.Init();
		
		sclmisc::OGetValue( registry::parameter::Tag, Tag  );

		cio::COut << muabsc::GetNextIMAPTag( Tag ) << txf::nl;
	qRR
	qRT
	qRE
	}
	/*
	void GetIndexes_( void )
	{
	qRH
		csdbnc::rIODriver Server;	
		muabsc::wIndexes Indexes;
	qRB
		if ( !pop3_::InitAndAuthenticate( Server ) )
			qRGnr();

		Indexes.Init();

		muapo3::GetIndexes( Server, Indexes );

		muabsc::Dump( Indexes, cio::COut );
	qRR
	qRT
	qRE
	}
	*/
	namespace {
		void GetHeader_( muaimf::dHeader &Header )
		{
		qRH
			sclmisc::rIDriverRack IRack;
			str::wString Input;
		qRB
			Input.Init();
			sclmisc::OGetValue( registry::parameter::Input, Input );

			muaimf::Fill( IRack.Init( Input ), Header );
		qRR
			IRack.HandleError();
		qRT
		qRE
		}
	}

	void ShowHeader_( void )
	{
	qRH
		sclmisc::rODriverRack ORack;
		str::wString Output;
		muaimf::wHeader Header;
	qRB
		Header.Init();
		GetHeader_( Header );

		Output.Init();
		sclmisc::OGetValue( registry::parameter::Output, Output );

		muaimf::Dump( Header, ORack.Init( Output ) );
	qRR
		ORack.HandleError();
	qRT
	qRE
	}

	namespace {
		void Dump_(
			muaimf::dFRows &Rows,
			const muaimf::dHeader &Header,
			txf::sOFlow &Flow )
		{
		qRH
			str::wString Value;
			sdr::sRow Row = qNIL;
		qRB	
			Row = Rows.First();

			while ( Row != qNIL ) {
				Value.Init();
				Flow << Header.GetBody( Rows( Row ), Value ) << txf::nl;

				Row = Rows.Next( Row );
			}
		qRR
		qRT
		qRE
		}

		void Dump_(
			const str::dString &FieldName,
			muaimf::dFRows &Rows,
			const muaimf::dHeader &Header,
			txf::sOFlow &Flow )
		{
		qRH
			str::wString Translation;
			str::wString Value;
		qRB
			switch ( Header.Amount() ) {
			case 0:
				Translation.Init();
				sclmisc::GetBaseTranslation( "NoSuchField", Translation, FieldName );

				Flow << Translation << txf::nl;
				break;
			case 1:
				Value.Init();
				Flow << FieldName << ": " << Header.GetBody( Header.First(), Value ) << txf::nl;
				break;
			default:
				Flow << FieldName << ": ";
				Dump_( Rows, Header, Flow );
				break;
			}
		qRR
		qRT
		qRE
		}
	}

	void GetField_( void )
	{
	qRH
		muaimf::wHeader Header;
		str::wString FieldName;
		muaimf::eField Field;
		muaimf::sFRow FieldRow = qNIL;
		muaimf::wFRows FieldRows;
		str::wString Output;
		sclmisc::rTextOFlowRack Rack;
	qRB
		Header.Init();
		GetHeader_( Header );

		FieldName.Init();
		sclmisc::MGetValue( registry::parameter::FieldName, FieldName );

		Field = muaimf::GetField( FieldName );

		if ( Field == muaimf::f_Undefined )
			sclmisc::ReportAndAbort("UnknownField", FieldName );

		FieldRows.Init();

		Header.Search( Field, FieldRows );

		Output.Init();
		sclmisc::OGetValue( registry::parameter::Output, Output );

		Dump_( FieldName, FieldRows, Header, Rack.Init( Output ) );
	qRR
		Rack.HandleError();
	qRT
	qRE
	}
}

#define C( name )\
	else if ( Command == #name )\
		name##_()

int scltool::SCLTOOLMain(
	const str::dString &Command,
	const scltool::fOddities &Oddities )
{
	int ExitValue = EXIT_FAILURE;
qRH
qRB
	if ( Command == "Version" )
		PrintHeader_();
	else if ( Command == "License" )
		epsmsc::PrintLicense( NAME_MC );
	C( Test );
	C( B64Encode );
	C( B64Decode );
	C( NextTag );
	C( POP3List );
	C( POP3Retrieve );
	C( POP3Top );
	C( POP3UIDL );
	C( IMAPCapability );
//	C( GetIndexes );
	C( ShowHeader );
	C( GetField );
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

