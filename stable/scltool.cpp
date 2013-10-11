/*
	'scltool' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'scltool' header file ('scltool.h').
	Copyright (C) 20112004 Claude SIMON.

	This file is part of the Epeios (http://zeusw.org/epeios/) project.

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
	of the License, or (at your option) any later version.
 
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, go to http://www.fsf.org/
	or write to the:
  
         	         Free Software Foundation, Inc.,
           59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/



//	$Id: scltool.cpp,v 1.10 2013/07/18 19:46:22 csimon Exp $

#define SCLTOOL__COMPILATION

#include "scltool.h"

class scltooltutor
: public ttr_tutor
{
public:
	scltooltutor( void )
	: ttr_tutor( SCLTOOL_NAME )
	{
#ifdef SCLTOOL_DBG
		Version = SCLTOOL_VERSION "\b\bD $";
#else
		Version = SCLTOOL_VERSION;
#endif
		Owner = SCLTOOL_OWNER;
		Date = "$Date: 2013/07/18 19:46:22 $";
	}
	virtual ~scltooltutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

#include "cio.h"

#include "sclrgstry.h"
#include "scllocale.h"
#include "sclmisc.h"
#include "sclerror.h"

using namespace scltool;

using cio::COut;
using scllocale::GetLocale;

static STR_BUFFER___ Language_;

#define DEFAULT_LANGUAGE	"en"

static rgstry::multi_level_registry Registry_;

static rgstry::level__ RegistryProjectLevel_ = RGSTRY_UNDEFINED_LEVEL;

const char *scltool::GetLanguage( void )
{
	if ( Language_ == NULL )
		return DEFAULT_LANGUAGE;

	return Language_;
}

void scltool::AddDefaultCommands( clnarg::description_ &Description )
{
	Description.AddCommand( CLNARG_NO_SHORT, "version", cVersion );
	Description.AddCommand( CLNARG_NO_SHORT, "help", cHelp );
	Description.AddCommand( CLNARG_NO_SHORT, "license", cLicense );
}


void scltool::PrintDefaultCommandDescriptions(
	const char *ProgramName,
	const clnarg::description_ &Description )
{
ERRProlog
	CLNARG_BUFFER__ Buffer;
	lcl::meaning Meaning;
	str::string Translation;
ERRBegin
	Translation.Init();
	COut << scllocale::GetLocale().GetTranslation( "ProgramDescription", GetLanguage(), Translation ) << txf::nl;
	COut << txf::nl;

	COut << ProgramName << ' ' << Description.GetCommandLabels( cVersion, Buffer ) << txf::nl;
	Meaning.Init();
	clnarg::GetVersionCommandDescription( Meaning );
	Translation.Init();
	GetLocale().GetTranslation( Meaning, GetLanguage(), Translation );
	COut << txf::pad << Translation << '.' << txf::nl;

	COut << ProgramName << ' ' << Description.GetCommandLabels( cLicense, Buffer ) << txf::nl;
	Meaning.Init();
	clnarg::GetLicenseCommandDescription( Meaning );
	Translation.Init();
	GetLocale().GetTranslation( Meaning, GetLanguage(), Translation );
	COut << txf::pad << Translation << '.' << txf::nl;

	COut << ProgramName << ' ' << Description.GetCommandLabels( cHelp, Buffer ) << txf::nl;
	Meaning.Init();
	clnarg::GetHelpCommandDescription( Meaning );
	Translation.Init();
	GetLocale().GetTranslation( Meaning, GetLanguage(), Translation );
	COut << txf::pad << Translation << '.' << txf::nl;

ERRErr
ERREnd
ERREpilog
}

void scltool::ReportAndExit(
	const char *Text,
	int ExitValue )
{
ERRProlog
	lcl::meaning Meaning;
ERRBegin
	Meaning.Init();

	Meaning.SetValue( Text );

	ReportAndExit( Meaning, ExitValue );
ERRErr
ERREnd
ERREpilog
}

static void ReportSCLPendingError_( void )
{
	if ( sclerror::ReportPendingError( GetLanguage(), err::hUserDefined ) )
		ERRRst();
}

int main(
	int argc,
	const char *argv[] )
{
ERRFProlog
	str::string Language;
ERRFBegin
	sclmisc::Initialize( TargetName, NULL );

	Language.Init();

	if ( sclrgstry::GetRegistry().GetValue( sclrgstry::Language, sclrgstry::GetRoot(), Language ) ) 
		Language.Convert( Language_ );

	Registry_.Init();

	Registry_.PushImportedLevel( sclrgstry::GetRegistry(), sclrgstry::GetRoot() );
	RegistryProjectLevel_ = Registry_.PushEmbeddedLevel( str::string( "Project" ) );

	Main( argc, argv );
ERRFErr
	if ( ERRType >= err::t_amount ) {
		if ( ERRType == err::t_Abort )
			ERRExitValue = EXIT_FAILURE;
		ReportSCLPendingError_();
	}
ERRFEnd
	cio::COut << txf::commit;
	cio::CErr << txf::commit;
ERRFEpilog
	return ERRExitValue;
}

#define PROJECT_ROOT_PATH	"Projects/Project[@target=\"%1\"]"

void scltool::LoadProject(
	const char *FileName,
	const char *Target )
{
ERRProlog
	str::string Path;
	STR_BUFFER___ Buffer;
	rgstry::context___ Context;
	lcl::meaning Meaning;
ERRBegin
	Path.Init( PROJECT_ROOT_PATH );
	str::ReplaceTag( Path, 1, str::string( Target ), '%' );

	if ( Registry_.Fill( RegistryProjectLevel_, FileName, xpp::criterions___(), Path.Convert( Buffer ), Context ) != rgstry::sOK ) {
		Meaning.Init();
		rgstry::GetMeaning( Context, Meaning );
		ReportAndExit( Meaning );
	};

ERRErr
ERREnd
ERREpilog
}

void scltool::LoadProject(
	const str::string_ &FileName,
	const char *Target )
{
ERRProlog
	STR_BUFFER___ Buffer;
ERRBegin
	LoadProject( FileName.Convert( Buffer ), Target );
ERRErr
ERREnd
ERREpilog
}

bso::bool__ scltool::GetValue(
	const rgstry::tentry__ &Entry,
	str::string_ &Value )
{
	return Registry_.GetValue( Entry, Value );
}

bso::bool__ scltool::GetValues(
	const rgstry::tentry__ &Entry,
	str::strings_ &Values )
{
	return Registry_.GetValues( Entry, Values );
}

const str::string_ &scltool::GetOptionalValue(
	const rgstry::tentry__ &Entry,
	str::string_ &Value,
	bso::bool__ *Missing )
{
	if ( !GetValue( Entry, Value ) )
		if ( Missing != NULL )
			*Missing = true;
	
	return Value;
}

const char *scltool::GetOptionalValue(
	const rgstry::tentry__ &Entry,
	STR_BUFFER___ &Buffer,
	bso::bool__ *Missing )
{
ERRProlog
	str::string Value;
	bso::bool__ LocalMissing = false;
ERRBegin
	Value.Init();

	GetOptionalValue( Entry, Value, &LocalMissing );

	if ( LocalMissing ) {
		if ( Missing != NULL )
			*Missing = true;
	} else
		Value.Convert( Buffer );
ERRErr
ERREnd
ERREpilog
	return Buffer;
}

const str::string_ &scltool::GetMandatoryValue(
	const rgstry::tentry__ &Entry,
	str::string_ &Value )
{
	if ( !GetValue( Entry, Value ) ) {
		sclrgstry::ReportBadOrNoValueForEntryError( Entry );
		ERRExit( EXIT_FAILURE );
	}

	return Value;
}

const char *scltool::GetMandatoryValue(
	const rgstry::tentry__ &Entry,
	STR_BUFFER___ &Buffer )
{
ERRProlog
	str::string Value;
ERRBegin
	Value.Init();

	GetMandatoryValue( Entry, Value );

	Value.Convert( Buffer );
ERRErr
ERREnd
ERREpilog
	return Buffer;
}

template <typename t> static bso::bool__ GetUnsignedNumber_(
	const rgstry::tentry__ &Entry,
	t Limit,
	t &Value )
{
	bso::bool__ Present = false;
ERRProlog
	str::string RawValue;
	sdr::row__ Error = E_NIL;
ERRBegin
	RawValue.Init();

	if ( !( Present = GetValue( Entry, RawValue ) ) )
		ERRReturn;

	RawValue.ToNumber( Limit, Value, &Error );

	if ( Error != E_NIL ) {
		sclrgstry::ReportBadOrNoValueForEntryError( Entry );
		ERRExit( EXIT_FAILURE );
	}
ERRErr
ERREnd
ERREpilog
	return Present;
}

template <typename t> static bso::bool__ GetSignedNumber_(
	const rgstry::tentry__ &Entry,
	t LowerLimit,
	t UpperLimit,
	t &Value )
{
	bso::bool__ Present = false;
ERRProlog
	str::string RawValue;
	sdr::row__ Error = E_NIL;
ERRBegin
	RawValue.Init();

	if ( !( Present = GetValue( Entry, RawValue ) ) )
		ERRReturn;

	RawValue.ToNumber( UpperLimit, LowerLimit, Value, &Error );

	if ( Error != E_NIL ) {
		sclrgstry::ReportBadOrNoValueForEntryError( Entry );
		ERRExit( EXIT_FAILURE );
	}
ERRErr
ERREnd
ERREpilog
	return Present;
}

#define UN( name, type )\
	type scltool::GetMandatory##name(\
		const rgstry::tentry__ &Entry,\
		type Limit  )\
	{\
		type Value;\
\
		if ( !GetUnsignedNumber_( Entry, Limit, Value ) ) {\
			sclrgstry::ReportBadOrNoValueForEntryError( Entry );\
			ERRExit( EXIT_FAILURE );\
		}\
\
		return Value;\
	}\
	type scltool::Get##name(\
		const rgstry::tentry__ &Entry,\
		type DefaultValue,\
		type Limit )\
	{\
		type Value;\
\
		if ( !GetUnsignedNumber_( Entry, Limit, Value ) )\
			Value = DefaultValue;\
\
		return Value;\
	}


UN( UInt, bso::uint__ )
#ifdef BSO__64BITS_ENABLED
UN( U64, bso::u64__ )
#endif
UN( U32, bso::u32__ )
UN( U16, bso::u16__ )
UN( U8, bso::u8__ )

#define SN( name, type )\
	type scltool::GetMandatory##name(\
		const rgstry::tentry__ &Entry,\
		type Min,\
		type Max)\
	{\
		type Value;\
\
		if ( !GetSignedNumber_( Entry, Min, Max, Value ) ) {\
			sclrgstry::ReportBadOrNoValueForEntryError( Entry );\
			ERRExit( EXIT_FAILURE );\
		}\
		return Value;\
	}\
	type scltool::Get##name(\
		const rgstry::tentry__ &Entry,\
		type DefaultValue,\
		type Min,\
		type Max )\
	{\
		type Value;\
\
		if ( !GetSignedNumber_( Entry, Min, Max, Value ) )\
			Value = DefaultValue;\
\
		return Value;\
	}

	SN( SInt, bso::sint__ )
#ifdef BSO__64BITS_ENABLED
	SN( S64, bso::s64__ )
#endif
	SN( S32, bso::s32__ )
	SN( S16, bso::s16__ )
	SN( S8, bso::s8__ )






/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class scltoolpersonnalization
: public scltooltutor
{
public:
	scltoolpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~scltoolpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the ending of the application  */
	}
};


/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

// 'static' by GNU C++.

static scltoolpersonnalization Tutor;

ttr_tutor &SCLTOOLTutor = Tutor;
