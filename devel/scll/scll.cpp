/*
	Copyright (C) 1999 Claude SIMON (http://q37.info/contact/).

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

#define SCLL_COMPILATION_

#include "scll.h"

#include "sclm.h"

using namespace scll;

namespace {
	rRack *RackPointer_ = NULL;

	using lcl::layer__;

	rRack &Rack_( void )
	{
		if ( RackPointer_ == NULL )
			qRFwk();

		return *RackPointer_;
	}

	lcl::locale_ &Locale_( void )
	{
		return Rack_().Locale;
	}
}

void scll::SetLocale( rRack &Rack )
{
	if ( RackPointer_ != NULL )
#ifdef CPE_S_POSIX	// Under 'Posix' (at least under 'GNU/Linux') 'RackPointer_' is shared between libraries,
		// so it may be already set by the caller.
		if ( RackPointer_ != &Rack )
#endif
			qRFwk();

	RackPointer_ = &Rack;
}

rRack &scll::GetRack( void )
{
	return Rack_();
}

#define C( name ) case t##name: return #name; break

const char *scll::GetLabel( target__ Target )
{
	switch ( Target ) {
		C( Main );
		C( Configuration );
		C( Project );
	default:
		qRFwk();
		break;
	}

	return NULL;	// To avoid a warning.
}


const lcl::locale_ &scll::GetLocale( void )
{
	return Locale_();
}

static layer__ GetLayer_( target__ Target )
{
	switch ( Target ) {
	case tMain:
		return Rack_().SoftwareLayer;
		break;
	case tConfiguration:
		return Rack_().ConfigurationLayer;
		break;
	case tProject:
		return Rack_().ProjectLayer;
		break;
	default:
		qRFwk();
		break;
	}

	return rgstry::UndefinedLayer;	// Pour &viter une 'warning'.
}

void scll::Dump(
	target__ Target,
	bso::bool__ RootToo,
	xml::rWriter &Writer )
{
	Locale_().Dump( GetLayer_( Target ), qNIL, RootToo, Writer );
}


void scll::Erase( target__ Target )
{
	Locale_().Erase( GetLayer_( Target ) );
}

namespace {
	void Load_(
		layer__ Layer,
		xtf::extended_text_iflow__ &Flow,
		const fnm::name___ &Directory,
		const char *ErrorLabel )
	{
	qRH
		rgstry::context___ Context;
	qRB
		Context.Init();
		if ( !Locale_().Insert( Layer, Flow, xpp::criterions___( Directory ), rgstry::rthIgnore, Context ) )
			sclm::ReportParsingErrorAndAbort( ErrorLabel, Context );
	qRR
	qRT
	qRE
	}
}

void scll::Load(
	target__ Target,
	xtf::extended_text_iflow__ &Flow,
	const fnm::name___ &Directory )
{
	const char *ErrorLabel = NULL;

	switch ( Target ) {
	case tMain:
		ErrorLabel = SCLL_NAME "_LocaleParsingError";
		break;
	case tConfiguration:
		ErrorLabel = SCLL_NAME "_ConfigurationLocaleParsingError";
		break;
	case tProject:
		ErrorLabel = SCLL_NAME "_ProjectLocaleParsingError";
		break;
	default:
		qRFwk();
		break;
	}

	return Load_( GetLayer_( Target ), Flow, Directory, ErrorLabel );
}

void scll::Fill(
	target__ Target,
	const fnm::name___ &Directory,
	const str::string_ &XML )
{
qRH
	flx::E_STRING_IFLOW__ Flow;
	xtf::extended_text_iflow__ XFlow;
qRB
	Flow.Init( XML );
	XFlow.Init( Flow, utf::f_Default );
	if ( !Locale_().Fill( GetLayer_( Target ), XFlow, xpp::criterions___( Directory ), NULL ) )
		qRFwk();
qRR
qRT
qRE
}

void scll::Insert(
	target__ Target,
	const fnm::name___ &Directory,
	const str::string_ &XML,
	rgstry::eRootTagHandling RootTagHandling )
{
qRH
	flx::E_STRING_IFLOW__ Flow;
	xtf::extended_text_iflow__ XFlow;
qRB
	Flow.Init( XML );
	XFlow.Init( Flow, utf::f_Default );
	if ( !Locale_().Insert( GetLayer_( Target ), XFlow, xpp::criterions___( Directory ), RootTagHandling  ) )
		qRFwk();
qRR
qRT
qRE
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
			Locale_().GetTranslation( Tag.Convert(Buffer), _Language, Value );
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
	/*
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
	*/
}

bso::bool__ scll::Normalize_(
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
			if ( !Escape ) {
				if ( TFlow.Flow().AmountWritten() )
					TFlow << ' ';

				Row = SkipLFCRTab_( In, Row );
			}
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
			if ( !Escape )
				TFlow << C;
			break;
		}

		Row = In.Next( Row );

		if ( Escape )
			break;
	}

	if ( !Escape )
		Success = true;
qRR
qRT
qRE
	return Success;
}

void scll::TranslateTags(
	str::string_ &String,
	const char *Language,
	char Marker )
{
	callback__ Callback;

	Callback.Init( Marker, Language );

	tagsbs::SubstituteLongTags( String, Callback, Marker );
}

void scll::TranslateTags(
	const str::string_ &In,
	const char *Language,
	str::string_ &Out,
	char Marker )
{
	callback__ Callback;

	Callback.Init( Marker, Language );

	tagsbs::SubstituteLongTags( In, Callback, Out, Marker );
}

