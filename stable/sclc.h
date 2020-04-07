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

// SoCLe C(LI)

#ifndef SCLC_INC_
# define SCLC_INC_

# define SCLC_NAME		"SCLC"

# if defined( E_DEBUG ) && !defined( SCLC_NODBG )
#  define SCLC_DBG
# endif

# include "sclm.h"

# include "err.h"

namespace sclc {
	template <typename source, typename ... tags> inline void Display(
		const source &Source,
		txf::sWFlow &Flow,
		const tags &... Tags )
	{
	qRH
		str::string Translation;
	qRB
		Translation.Init();

		Flow << scll::GetTranslation( Source, sclm::GetBaseLanguage(), Translation, Tags... ) << txf::nl;
	qRR
	qRT
	qRE
	}

	template <typename source, typename ... tags> inline void ReportAndAbort(
		const source &Source,
		const tags &... Tags )
	{
		sclm::ReportAndAbort( Source, Tags... );
	}
}

#endif
