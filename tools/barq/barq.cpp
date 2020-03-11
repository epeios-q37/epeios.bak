/*
	Copyright (C) 2016 by Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of 'barq'.

    'barq' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'barq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'barq'.  If not, see <http://www.gnu.org/licenses/>.
*/

// Backend A(PI) Retriever.

#include "barq.h"

#include "registry.h"

#include "scltool.h"
#include "sclerror.h"

#include "err.h"
#include "cio.h"
#include "epsmsc.h"
#include "xpp.h"
#include "fnm.h"
#include "flf.h"
#include "fblcst.h"
#include "fblfaq.h"
#include "fblovl.h"

using cio::CErr;
using cio::COut;
using cio::CIn;

using namespace fblfaq;

using xml::rWriter;

SCLI_DEF( barq, NAME_LC, NAME_MC );

const scli::sInfo &scltool::SCLTOOLInfo( void )
{
	return barq::Info;
}

namespace {
	void PrintHeader_( void )
	{
		COut << NAME_MC " V" VERSION << " (" WEBSITE_URL ")" << txf::nl;
		COut << "Copyright (C) " COPYRIGHT << txf::nl;
		COut << txf::pad << "Build : " __DATE__ " " __TIME__ << " (" << cpe::GetDescription() << ')' << txf::nl;
	}
}

enum cast_type {
	ctUnknow,
	ctStatic,
	ctBunch,
	ctContainer,
	ctOther,
	ct_amount
};

static inline const char *GetTypeName(
	fblcst::cast__ Cast,
	cast_type &CastType)
{
	static const char *TypeName = NULL;
	CastType = ctStatic;

	switch ( Cast ) {
	case fblcst::cEnd:
		qRGnr();
		break;
	case fblcst::cObject:
		TypeName = "object";
		break;
	case fblcst::cBoolean:
		TypeName = "boolean";
		break;
	case fblcst::cBooleans:
		TypeName = "booleans";
		CastType = ctBunch;
		break;
	case fblcst::cSInt:
		TypeName = "sint";
		break;
	case fblcst::cSInts:
		TypeName = "sints";
		CastType = ctBunch;
		break;
	case fblcst::cUInt:
		TypeName = "uint";
		break;
	case fblcst::cUInts:
		TypeName = "uints";
		CastType = ctBunch;
		break;
	case fblcst::cSBig:
		TypeName = "sbig";
		break;
	case fblcst::cSBigs:
		TypeName = "sbigs";
		CastType = ctBunch;
		break;
	case fblcst::cUBig:
		TypeName = "ubig";
		break;
	case fblcst::cUBigs:
		TypeName = "ubigs";
		CastType = ctBunch;
		break;
	case fblcst::cId8:
		TypeName = "id8";
		break;
	case fblcst::cId8s:
		TypeName = "id8s";
		CastType = ctBunch;
		break;
	case fblcst::cXId8s:
		TypeName = "xid8s";
		CastType = ctContainer;
		break;
	case fblcst::cId16:
		TypeName = "id16";
		break;
	case fblcst::cId16s:
		TypeName = "id16s";
		CastType = ctBunch;
		break;
	case fblcst::cXId16s:
		TypeName = "xid16s";
		CastType = ctContainer;
		break;
	case fblcst::cId32:
		TypeName = "id32";
		break;
	case fblcst::cId32s:
		TypeName = "id32s";
		CastType = ctBunch;
		break;
	case fblcst::cXId32s:
		TypeName = "xid32s";
		CastType = ctContainer;
		break;
	case fblcst::cId:
		TypeName = "id";
		break;
	case fblcst::cIds:
		TypeName = "ids";
		CastType = ctBunch;
		break;
	case fblcst::cXIds:
		TypeName = "xids";
		CastType = ctContainer;
		break;
	case fblcst::cChar:
		TypeName = "char";
		break;
	case fblcst::cString:
		TypeName = "string";
		CastType = ctBunch;
		break;
	case fblcst::cStrings:
		TypeName = "strings";
		CastType = ctContainer;
		break;
	case fblcst::cXStrings:
		TypeName = "xstrings";
		CastType = ctBunch;
		break;
	case fblcst::cByte:
		TypeName = "byte";
		break;
	case fblcst::cBinary:
		TypeName = "binary";
		CastType = ctBunch;
		break;
	case fblcst::cBinaries:
		TypeName = "binaries";
		break;
	case fblcst::cItem8s:
		TypeName = "item8s";
		CastType = ctContainer;
		break;
	case fblcst::cItem16s:
		TypeName = "item16s";
		CastType = ctContainer;
		break;
	case fblcst::cItem32s:
		TypeName = "item32s";
		CastType = ctContainer;
		break;
	case fblcst::cItems:
		TypeName = "items";
		CastType = ctContainer;
		break;
	case fblcst::cXItem8s:
		TypeName = "xitem8s";
		CastType = ctContainer;
		break;
	case fblcst::cXItem16s:
		TypeName = "xitem16s";
		CastType = ctContainer;
		break;
	case fblcst::cXItem32s:
		TypeName = "xitem32s";
		CastType = ctContainer;
		break;
	case fblcst::cXItems:
		TypeName = "xitems";
		CastType = ctContainer;
		break;
	case fblcst::cCommandsDetails:
		TypeName = "commands_details";
		CastType = ctBunch;
		break;
	case fblcst::cObjectsReferences:
		TypeName = "objects_references";
		CastType = ctContainer;
		break;
	case fblcst::cFlow:
		TypeName = "flow";
		CastType = ctOther;
		break;
	default:
		qRGnr();
		break;
	}

	return TypeName;
}

static void Generate_(
	const fblcst::cast__ &Cast,
	bso::u8__ ID,	// Id of the parameter, NOT of the type.
	rWriter &Writer,
	bso::bool__ IsLast )
{
qRH
	cast_type CastType = ctUnknow;
	str::string TypeName;
qRB
	Writer.PushTag( "Parameter" );
	Writer.PutAttribute( "Discriminator", ID );

	if ( IsLast )
		Writer.PutAttribute( "Position", "Last" );

	TypeName.Init( GetTypeName( Cast, CastType ) );

	switch ( CastType ) {
	case ctStatic:
		Writer.PutAttribute( "Type", "Static" );
		break;
	case ctBunch:
		Writer.PutAttribute( "Type", "Bunch" );
		break;
	case ctContainer:
		Writer.PutAttribute( "Type", "Container" );
		break;
	case ctOther:
		Writer.PutAttribute( "Type", "Other" );
		break;
	default:
		qRGnr();
		break;
	}

	Writer.PutValue( TypeName, "Name" );

	Writer.PutValue( Cast, "ID" );	// We want the id of the type, NOT the parameter's one (not 'ID').
	Writer.PopTag();
qRR
qRT
qRE
}

static inline bso::bool__ IsLast_(
	const parameters_ &Parameters,
	sdr::row__ Current )
{
	if ( ( Current = Parameters.Next( Current ) ) == qNIL )
		return true;

	if ( Parameters( Current ) != fblcst::cEnd )
		return false;

	return Parameters.Next( Current ) == qNIL;
}

static void Generate_(
	const parameters_ &Parameters,
	rWriter &Writer )
{
	sdr::row__ P = Parameters.First();
	fblcst::cast__ Cast;
	bso::u8__ ID = 1;

	if ( ( Cast = (fblcst::cast__)*Parameters( P ) ) == fblcst::cEnd )
		Writer.PushTag( "Out" );
	else {
		Writer.PushTag( "In" );
		Generate_( Cast, ID, Writer, IsLast_( Parameters, P ) );
		ID++;
	}

	P = Parameters.Next( P );

	while( P != qNIL ) {
		if ( ( Cast = (fblcst::cast__)*Parameters( P ) ) == fblcst::cEnd ) {

			if ( Parameters.Next( P ) != qNIL ) {
				ID = 1;
				Writer.PopTag();
				Writer.PushTag( "Out" );
			}
		} else {
			Generate_( Cast, ID, Writer, IsLast_( Parameters, P ) );
			ID++;
		}

		P = Parameters.Next( P );
	}

	Writer.PopTag();
}

static void Generate_(
	const parameters_ &Parameters,
	rWriter &Writer,
	sdr::size__ &Position )
{
	Writer.PushTag( "Parameters" );

	Writer.PutAttribute( "Amount", Parameters.Amount() );		// Don't forget ; there is the 'end of input parameters' cast.

	Writer.PutAttribute( "Position", Position );

	Position += Parameters.Amount();

	if ( Parameters.Amount() > 1 )
		Generate_( Parameters, Writer );

	Writer.PopTag();
}

static const char *Convert_( unsigned long V )
{
	static char Buffer[20];

	sprintf( Buffer, "%lu",  V );

	return Buffer;
}

static void Generate_(
	const command_ &Command,
	rWriter &Writer,
	sdr::size__ &Position )
{
	Writer.PushTag( "Command" );

	Writer.PutValue( Command.Identification.Value, "Name" );
	Writer.PutValue( Convert_( *Command.Identification.ID() ), "ID" );

	Generate_( Command.Parameters, Writer, Position );

	Writer.PopTag();
}

static void Generate_(
	const commands_ &Commands,
	rWriter &Writer )
{
qRH
	sdr::row__ P;
	sdr::size__ Position = 0;
qRB
	Writer.PushTag( "Commands" );

	Writer.PutAttribute( "Amount", Commands.Amount() );
	P = Commands.First();

	while( P != qNIL ) {
		Generate_( Commands( P ), Writer, Position );
		P = Commands.Next( P );
	}

//	Commands.Flush();

	Writer.PopTag();
qRR
qRT
qRE
}

// 'abc_def_ghi' -> 'AbcDefGhi'.
static void Convert_( str::string_ &Buffer )
{
qRH
	str::string Temp;
	sdr::row__ P = qNIL;
qRB
	Temp.Init();

	P = Buffer.First();

	if ( P != qNIL )
		if ( Buffer( P ) != '_' ) {
			Temp.Append( toupper( Buffer( P ) ) );

			P = Buffer.Next( P );

			while ( P != qNIL )	{
				if ( Buffer( P ) == '_' ) {
					P = Buffer.Next( P );

				if ( P != qNIL )
					Temp.Append( toupper( Buffer( P ) ) );
				} else
					Temp.Append( Buffer( P ) );

				if ( P != qNIL )
					P = Buffer.Next(  P );
			}

			Buffer = Temp;
		}
qRR
qRT
qRE
}

static void Generate_(
	const type_ &Type,
	rWriter &Writer )
{
qRH
	str::string Buffer;
qRB
	Buffer.Init();

	Buffer = Type.Prefix;

	Writer.PushTag( "Prefix" );

	Writer.PutAttribute( "Default", Buffer );

	str::ToLower( Buffer );
	Writer.PutAttribute( "Lower", Buffer );

	str::ToUpper( Buffer );
	Writer.PutAttribute( "Upper", Buffer );

	Writer.PopTag();

	Buffer.Init();

	Buffer.Init();

	Buffer = Type.Name;

	Writer.PushTag( "Name" );

	Writer.PutAttribute( "Raw", Buffer );

	Writer.PutAttribute( "Underscored", Buffer );

	Convert_( Buffer );

	Writer.PutAttribute( "Mixedcased", Buffer );

	Writer.PopTag();
qRR
qRT
qRE
}

static void Generate_(
	const type_ &Type,
	bso::bool__ IsMaster,
	rWriter &Writer )
{
	Writer.PushTag( "Type" );
	if ( IsMaster )
		Writer.PutAttribute( "Object", "Master" );

	Writer.PutValue( Convert_( *Type.ID() ), "ID" );
	Generate_( Type, Writer );

	if ( Type.Commands.Amount() != 0 )
		Generate_( Type.Commands, Writer );

	Writer.PopTag();
}

class dummy_reporting_callbacks__
: public fblfrd::reporting_callback__
{
protected:
	void FBLFRDReport(
		fblovl::reply__ Reply,
		const char *Message )
	{
	}
};

namespace {
	typedef fblfrd::cFrontend cFrontend_;

	class sFrontend_
	: public cFrontend_
	{
	protected:
		virtual void FBLFRDOnConnection( void ) override {}
		virtual void FBLFRDOnDisconnection( void ) override {}
	};
}

namespace {
	qCDEF(char *, EmbeddedCommandLabel, "Embedded" );
	qCDEF(char *, StraightCommandLabel, "Straight" );
	qCDEF(char *, ProxyCommandLabel, "Proxy" );

	const str::dString &GetPluginFilename_(
		const str::dString &Id,
		str::dString &Filename )
	{
		return sclmisc::MGetValue( rgstry::rTEntry( registry::definition::TaggedPluginFilename, Id ), Filename );
	}

	fblovl::eMode GetMode_( const str::dString &Command )
	{
		if ( Command == EmbeddedCommandLabel )
			return fblovl::mReferenced;
		else if ( Command == StraightCommandLabel )
			return fblovl::mSerialized;
		else if ( Command == ProxyCommandLabel )
			return fblovl::mSerialized;
		else
			qRFwk();

		return fblovl::m_Undefined;
	}
}

static void GetBackendData_(
	const str::dString &Command,
	const str::dString &Arguments,
	types_ &Types,
	str::string_ &ProtocolVersion,
	str::string_ &TargetLabel,
	str::string_ &APIVersion,
	str::string_ &ExtendedBackendInformations,
	str::string_ &BackendCopyright,
	str::string_ &SoftwareInformations )
{
qRH
	plgn::rRetriever<fblovl::cDriver> Retriever;
	csdrcc::rDriver *Driver = NULL;
	str::wString PluginFilename;
	flw::rDressedRWFlow<> Flow;
	fblfrd::universal_frontend___ Frontend;
	sFrontend_ Dummy;
	fblfrd::incompatibility_informations DummyIncompatibilityInformations;
	fblfrd::compatibility_informations__ DummyCompatibilityInformations;
	dummy_reporting_callbacks__ DummyReportingFunctions;
	lcl::meaning Meaning;
qRB
	PluginFilename.Init();
	GetPluginFilename_( Command, PluginFilename );

	Retriever.Init();
	Retriever.Initialize( PluginFilename, NULL, Arguments, plgn::EmptyAbstracts );

	Driver = Retriever.Plugin().New();

	Flow.Init( *Driver );

	DummyCompatibilityInformations.Init( "", "" );
	DummyIncompatibilityInformations.Init();

	DummyReportingFunctions.Init();
	Frontend.Init( str::wString(), Dummy, DummyReportingFunctions );

	if ( !Frontend.Connect( "",  Flow, GetMode_( Command ), DummyCompatibilityInformations, DummyIncompatibilityInformations ) ) {
		Meaning.Init();

		Meaning.SetValue( "IncompatibleBackend" );

		sclerror::SetMeaning( Meaning );

		qRAbort();
	}

	GetDescription( Frontend, Types );

	Frontend.About( ProtocolVersion, TargetLabel, APIVersion, ExtendedBackendInformations, BackendCopyright, SoftwareInformations );
qRR
	if ( ERRFailure() ) {
		Meaning.Init();

		Meaning.SetValue( "UnableToCommunicateWithBackendError" );

		sclerror::SetMeaning( Meaning );

		qRAbort();
	}
qRT
	Frontend.Disconnect();

	Flow.reset();

	if ( Driver != NULL )
		Retriever.Plugin().Delete( Driver );
qRE
}

static void Generate_(
	const types_ &Types,
	sdr::row__ MasterRow,
	rWriter &Writer )
{
	sdr::row__ P;

	Writer.PushTag( "Types" );

	P = Types.First();

	while( P != qNIL ) {
		Generate_( Types( P ), ( P == MasterRow ), Writer );
		P = Types.Next( P );
	}

	Writer.PopTag();
}

static void GenerateMisc_(
	const str::string_ &BackendInformations,
	const str::string_ &BackendCopyright,
	const str::string_ &SoftwareInformations,
	rWriter &Writer )
{
qRH
	str::string Build;
	tol::buffer__ Buffer;
qRB
	Writer.PushTag( "Misc" );
	Writer.PushTag( "Generator" );
	Writer.PutValue( NAME_MC, "Name" );
	Writer.PutValue( VERSION, "Version" );

	Build.Init();
	Build.Append( __DATE__ " " );
	Build.Append( __TIME__ " " );
	Build.Append( cpe::GetDescription() );
	Writer.PutValue( Build, "Build" );

	Writer.PutValue( WEBSITE_URL, "URL" );
	Writer.PushTag( "Author" );
	Writer.PutValue( AUTHOR_NAME, "Name" );
	Writer.PutValue( AUTHOR_CONTACT, "Contact" );
	Writer.PopTag();
	Writer.PutValue( COPYRIGHT, "Copyright" );
	Writer.PopTag();
	Writer.PushTag( "Backend" );
	Writer.PutValue( BackendInformations, "Details" );
	Writer.PutValue( BackendCopyright, "Copyright" );
	Writer.PopTag();
	Writer.PutValue( SoftwareInformations, "Software" );
	Writer.PutValue( tol::Date( Buffer ), "Date" );
	Writer.PutValue( tol::Time( Buffer ), "Time" );
	Writer.PopTag();
qRR
qRT
qRE

}


static void Generate_(
//	const bkdacc::strings_ &RawMessages,
	const types_ &Types,
	sdr::row__ MasterRow,
	const str::string_ &ProtocolVersion,
	const str::string_ &TargetLabel,
	const str::string_ &APIVersion,
	const str::string_ &BackendInformations,
	const str::string_ &BackendCopyright,
	const str::string_ &SoftwareInformations,
	txf::text_oflow__ &Flow )
{
qRH
	xml::rWriter Writer;
	tol::buffer__ Buffer;
	str::string Temp;
qRB
	Writer.Init( Flow, xml::oIndent, xml::e_Default );

	Flow << "<!--                    DON'T MODIFY !!! !" << txf::nl;
	Flow << txf::pad << "This document was generated the " << tol::DateAndTime( Buffer ) << txf::nl;
	Flow << txf::pad << "using " NAME_MC " V" VERSION << " (" WEBSITE_URL ")" << txf::nl;
	Flow << txf::pad << "Build : " __DATE__ " " __TIME__ << " (" << cpe::GetDescription() << ')' << txf::nl;
	Flow << "-->" << txf::nl;

	Writer.PushTag( "API" );
	Writer.PutAttribute( "target", TargetLabel );

	Temp.Init( TargetLabel );
	Writer.PutAttribute( "TargetUppercased", str::ToUpper( Temp ) );

	Writer.PutAttribute( "ProtocolVersion", ProtocolVersion );
	Writer.PutAttribute( "APIVersion", APIVersion );

	GenerateMisc_( BackendInformations, BackendCopyright, SoftwareInformations, Writer );
	Generate_( Types, MasterRow, Writer );

	Writer.PopTag();
qRR
qRT
qRE
}

static sdr::row__ FindMasterType_( const types_ &Types )
{
	sdr::row__ P = qNIL;

	P = Types.First();

	while( ( P != qNIL ) && ( Types( P ).ID() != FBLFRD_MASTER_TYPE ) )
		P = Types.Next( P );

	if ( P == qNIL )
		qRGnr();

	return P;
}

static void Go_(const str::string_ &Command )
{
qRH
	types Types;
	str::string TargetLabel, ProtocolVersion, APIVersion;
	str::string BackendInformations, BackendCopyright, SoftwareInformations;
	sdr::row__ MasterRow = qNIL;
	str::wString Arguments, OutputFilename;
	sclmisc::rTextWFlowRack TWFlowRack;
qRB
	Types.Init();

	TargetLabel.Init();
	ProtocolVersion.Init();
	APIVersion.Init();
	BackendInformations.Init();
	BackendCopyright.Init();
	SoftwareInformations.Init();

	Arguments.Init();

	if ( Command == EmbeddedCommandLabel ) {
		sclmisc::MGetValue( registry::parameter::BackendFilename, Arguments );
	} else if ( Command == StraightCommandLabel ) {
		sclmisc::MGetValue( registry::parameter::HostService, Arguments );
	} else if ( Command == ProxyCommandLabel ) {
		sclmisc::MGetValue( registry::parameter::HostService, Arguments );
		Arguments.Append( ' ' );
		sclmisc::MGetValue( registry::parameter::Identifier, Arguments );
	} else
		qRGnr();

	OutputFilename.Init();
	sclmisc::OGetValue( registry::parameter::OutputFilename, OutputFilename );

	GetBackendData_( Command, Arguments, Types, ProtocolVersion, TargetLabel, APIVersion, BackendInformations, BackendCopyright, SoftwareInformations );

	MasterRow = FindMasterType_( Types );

	txf::sWFlow &Flow = TWFlowRack.Init( OutputFilename );

	if ( TWFlowRack.IsFile() )
		COut << "Backend : " << BackendInformations << " (" << SoftwareInformations << ')' << txf::nl << txf::commit;

	Generate_( Types, MasterRow, ProtocolVersion, TargetLabel, APIVersion, BackendInformations, BackendCopyright, SoftwareInformations, Flow );
qRR
	TWFlowRack.HandleError();
qRT
qRE
}

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
	else
		Go_( Command );

	ExitValue = EXIT_SUCCESS;
qRR
qRT
qRE
	return ExitValue;
}
