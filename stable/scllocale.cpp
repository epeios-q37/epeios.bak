/*
	Copyright (C) 2000-2015 Claude SIMON (http://q37.info/contact/).

	This file is part of the Epeios framework.

	The Epeios framework is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

	The Epeios framework is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with the Epeios framework.  If not, see <http://www.gnu.org/licenses/>
*/

#define SCLLOCALE__COMPILATION

#include "scllocale.h"

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

#include "lcl.h"
#include "fnm.h"
#include "dir.h"
#include "cio.h"

#include "sclmisc.h"

using namespace scllocale;

namespace {
	using lcl::level__;
}

static lcl::locale Locale_;

static level__ SoftwareLevel_ = LCL_UNDEFINED_LEVEL;
static level__ ConfigurationLevel_ = LCL_UNDEFINED_LEVEL;
static level__ ProjectLevel_ = LCL_UNDEFINED_LEVEL;

const lcl::locale_ &scllocale::GetLocale( void )
{
	return Locale_;
}

static void GetLocaleFileParsingErrorMeaning_(
	const rgstry::context___ &Context,
	lcl::meaning_ &Meaning )
{
qRH
	lcl::meaning MeaningBuffer;
qRB
	Meaning.SetValue( SCLLOCALE_NAME "_LocaleFileParsingError" );

	MeaningBuffer.Init();
	rgstry::GetMeaning( Context, MeaningBuffer );

	Meaning.AddTag( MeaningBuffer );
qRR
qRT
qRE
}

static level__ GetLevel_( target__ Target )
{
	switch ( Target ) {
	case tMain:
		return SoftwareLevel_;
		break;
	case tConfiguration:
		return ConfigurationLevel_;
		break;
	case tProject:
		return ProjectLevel_;
		break;
	default:
		qRFwk();
		break;
	}

	return *(level__ *)NULL;	// Pour viter une 'warning'.
}

void scllocale::EraseLocale( target__ Target )
{
	Locale_.Erase( GetLevel_( Target ) );
}

static void Load_(
	level__ Level,
	xtf::extended_text_iflow__ &Flow,
	const char *Directory,
	const char *RootPath,
	const char *ErrorLabel )
{
qRH
	rgstry::context___ Context;
qRB
	Locale_.Erase( Level );

	Context.Init();
	if ( Locale_.Fill( Level, Flow, xpp::criterions___( str::string( Directory ) ), RootPath, Context ) != rgstry::sOK )
		sclmisc::ReportParsingErrorAndAbort( ErrorLabel, Context );
qRR
qRT
qRE
}

void scllocale::SetLocale(
	target__ Target,
	const rgstry::entry__ &Entry )
{
	rgstry::level__ Level = GetLevel_( Target );

	Locale_.Erase( Level );

	Locale_.Set( Level, Entry );
}

void scllocale::LoadLocale(
	target__ Target,
	xtf::extended_text_iflow__ &Flow,
	const char *Directory,
	const char *RootPath )
{
	const char *ErrorLabel = NULL;

	switch ( Target ) {
	case tMain:
		ErrorLabel = SCLLOCALE_NAME "_LocaleParsingError";
		break;
	case tConfiguration:
		ErrorLabel = SCLLOCALE_NAME "_ConfigurationLocaleParsingError";
		break;
	case tProject:
		ErrorLabel = SCLLOCALE_NAME "_ProjectLocaleParsingError";
		break;
	default:
		qRFwk();
		break;
	}

	return Load_( GetLevel_( Target ), Flow, Directory, RootPath, ErrorLabel );
}

// Bien que dfinit dans un '.cpp', et propre  ce '.cpp', VC++ se mlange les pinceaux avec le 'callback__' dfinit dans 'sclxhtml.cpp', d'o le 'namespace'.
namespace {
	typedef tagsbs::long_tags_callback__ _callback__;

	class callback__
	: public _callback__
	{
	private:
		char _Marker;
		const char *_Language;
	protected:
		virtual bso::bool__ TAGSBSGetTagValue(
			const str::string_ &Tag,
			str::string_ &Value ) override
		{
		qRH
			TOL_CBUFFER___ Buffer;
		qRB
			Locale_.GetTranslation( Tag.Convert(Buffer), _Language, Value );
		qRR
		qRT
		qRE
			return true;
		}
	public:
		void reset( bso::bool__ P = true )
		{
			_Marker = 0;
			_Language = NULL;
			_callback__::reset( P );
		}
		E_CVDTOR( callback__ );
		void Init(
			char Marker,
			const char *Language )
		{
			_Marker = Marker;
			_Language = Language;
			_callback__::Init();
		}
	};

	sdr::row__ SkipLFCRTab_(
		const str::string_ &In,
		sdr::row__ Row )
	{
		while ( ( Row != qNIL) && ( strchr( "\n\r\t", In( Row ) ) != NULL ) )
			Row = In.Next( Row );

		if ( Row == qNIL )
			return In.Last();
		else
			return In.Previous( Row );
	}

	bso::bool__ Normalize_(
		const str::string_ &In,
		str::string_ &Out )
	{
		bso::bool__ Success = false;
	qRH
		sdr::row__ Row = qNIL;
		bso::char__ C = 0;
		bso::bool__ Escape = false;
		flx::E_STRING_TOFLOW___ TFlow;
	qRB
		Row = In.First();
		TFlow.Init( Out );

		while ( Row != qNIL ) {
			switch ( C = In( Row ) ) {
			case '\n':
			case '\r':
			case '\t':
				if ( Escape )
					qRReturn;

				if ( TFlow.Flow().AmountWritten() )
					TFlow << ' ';

				Row = SkipLFCRTab_( In, Row );
				break;
			case 'n':
			case 'r':
				if ( Escape )
					TFlow << txf::nl;
				else
					TFlow << C;
				Escape = false;
				break;
			case 't':
				if ( Escape )
					TFlow << txf::tab;
				else
					TFlow << C;
				Escape = false;
				break;
			case 'p':
				if ( Escape )
					TFlow << txf::pad;
				else
					TFlow << C;
				Escape = false;
				break;
			case '\\':
				if ( Escape ) {
					TFlow << C;
					Escape = false;
				} else
					Escape = true;
				break;
			default:
				if ( Escape )
					qRReturn;
				else
					TFlow << C;
				break;
			}

			Row = In.Next( Row );
		}

		if ( !Escape )
			Success = true;
	qRR
	qRT
	qRE
		return Success;
	}

	template <typename source> const str::string_ &GetTranslation_(
		const source &Source,
		const char *Language,
		str::string_ &Translation )
	{
	qRH
		str::string Intermediate;
	qRB
		Intermediate.Init();
		GetLocale().GetTranslation( Source, Language, Intermediate );

		if ( !Normalize_( Intermediate, Translation ) )
			qRFwk();
	qRR
	qRT
	qRE
		return Translation;
	}
}

const str::string_ &scllocale::GetTranslation(
	const char *Text,
	const char *Language,
	str::string_ &Translation )
{
	return GetTranslation_( Text, Language, Translation );
}

const str::string_ &scllocale::GetTranslation(
	const lcl::meaning_ &Meaning,
	const char *Language,
	str::string_ &Translation )
{
	return GetTranslation_( Meaning, Language, Translation );
}


void scllocale::TranslateTags(
	str::string_ &String,
	const char *Language,
	char Marker )
{
	callback__ Callback;

	Callback.Init( Marker, Language );

	tagsbs::SubstituteLongTags( String, Callback, Marker );
}

void scllocale::TranslateTags(
	const str::string_ &In,
	const char *Language,
	str::string_ &Out,
	char Marker )
{
	callback__ Callback;

	Callback.Init( Marker, Language );

	tagsbs::SubstituteLongTags( In, Callback, Out, Marker );
}

Q37_GCTOR( scllocale )
{
	Locale_.Init();

	SoftwareLevel_ = Locale_.CreateEmbedded( rgstry::name( "Software" ) );
	ConfigurationLevel_ = Locale_.CreateEmbedded( rgstry::name( "Configuration" ) );
	ProjectLevel_ = Locale_.CreateEmbedded( rgstry::name( "Project" ) );
}
