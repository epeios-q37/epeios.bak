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

// SoCLe Locale

#ifndef SCLL_INC_
# define SCLL_INC_

# define SCLL_NAME		"SCLL"

# if defined( E_DEBUG ) && !defined( SCLL_NODBG )
#  define SCLL_DBG
# endif

# include "err.h"
# include "lcl.h"

namespace scll {
	class rRack {
	public:
		lcl::locale Locale;
		lcl::layer__ SoftwareLayer;
		lcl::layer__ ConfigurationLayer;
		lcl::layer__ ProjectLayer;
		void reset( bso::sBool P = true )
		{
			Locale.reset( P );
			SoftwareLayer = LCL_UNDEFINED_LAYER;
			ConfigurationLayer = LCL_UNDEFINED_LAYER;
			ProjectLayer = LCL_UNDEFINED_LAYER;
		}
		qCDTOR( rRack );
		void Init( void )
		{
			Locale.Init();
			SoftwareLayer = Locale.CreateEmbedded( rgstry::name( "Software" ) );
			ConfigurationLayer = Locale.CreateEmbedded( rgstry::name( "Configuration" ) );
			ProjectLayer = Locale.CreateEmbedded( rgstry::name( "Project" ) );
		}
	};

	void SetLocale( rRack &Rack );

	rRack &GetRack( void );

	E_CDEF(char, DefaultMarker, '%' );

	const lcl::locale_ &GetLocale( void );

	enum target__ {
		tMain,
		tConfiguration,
		tProject,
		t_amount,
		t_Undefined
	};

	const char *GetLabel( target__ Target );

	void Dump(
		target__ Target,
		bso::bool__ RootToo,
		xml::rWriter &Writer );

	void Erase( target__ Target );

/*	void Set(
		target__ Target,
		const rgstry::entry__ &Entry );
*/

	void Load(
		target__ Target,
		xtf::extended_text_iflow__ &Flow,
		const fnm::name___ &Directory );

	void Fill(
		target__ Target,
		const fnm::name___ &Directory,
		const str::string_ &XML );

	void Insert(
		target__ Target,
		const fnm::name___ &Directory,
		const str::string_ &XML,
		rgstry::eRootTagHandling RootTagHandling );

	bso::bool__ Normalize_(
		const str::string_ &In,
		str::string_ &Out );

	template <typename source> inline const str::string_ &GetTranslation(
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

	template <typename source, typename ... tags> inline const str::string_ &GetTranslation(
		const source &Source,
		const char *Language,
		str::string_ &Translation,
		const tags &... Tags )
	{
	qRH
		str::string Intermediate;
	qRB
		Intermediate.Init();
		GetLocale().GetTranslation( Source, Language, Intermediate, Tags... );

		if ( !Normalize_( Intermediate, Translation ) )
			qRFwk();
	qRR
	qRT
	qRE
		return Translation;
	}

	/*
	const str::string_ &GetTranslation(
		const str::dString &Text,
		const char *Language,
		str::string_ &Translation );

	const str::string_ &GetTranslation(
		const char *Text,
		const char *Language,
		str::string_ &Translation );

	const str::string_ &GetTranslation(
		const lcl::meaning_ &Meaning,
		const char *Language,
		str::string_ &Translation );
	*/

	void TranslateTags(
		str::string_ &String,
		const char *Language,
		char Marker = DefaultMarker );

	void TranslateTags(
		const str::string_ &In,
		const char *Language,
		str::string_ &Out,
		char Marker = DefaultMarker );

}

#endif
