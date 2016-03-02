/*
	Copyright (C) 1999-2016 Claude SIMON (http://q37.info/contact/).

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

#ifndef SCLLOCALE__INC
# define SCLLOCALE__INC

# define SCLLOCALE_NAME		"SCLLOCALE"

# if defined( E_DEBUG ) && !defined( SCLLOCALE_NODBG )
#  define SCLLOCALE_DBG
# endif

// SoCLe LOCALE

# include "err.h"
# include "flw.h"
# include "str.h"
# include "lcl.h"

namespace scllocale {

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
		xml::writer &Writer );

	void Erase( target__ Target );

	void Set(
		target__ Target,
		const rgstry::entry__ &Entry );

	void Load(
		target__ Target,
		xtf::extended_text_iflow__ &Flow,
		const fnm::name___ &Directory,
		const char *RootPath );

	void Fill(
		target__ Target,
		const char *RootPath,
		const fnm::name___ &Directory,
		const str::string_ &XML );

	void Insert(
		target__ Target,
		const fnm::name___ &Directory,
		const str::string_ &XML,
		rgstry::eRootTagHandling RootTagHandling );

	const str::string_ &GetTranslation(
		const char *Text,
		const char *Language,
		str::string_ &Translation );

	const str::string_ &GetTranslation(
		const lcl::meaning_ &Meaning,
		const char *Language,
		str::string_ &Translation );

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
