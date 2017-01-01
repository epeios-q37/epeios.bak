/*
	Copyright (C) 1999-2017 Claude SIMON (http://q37.info/contact/).

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

//	$Id: xxx.h,v 1.9 2012/11/14 16:06:23 csimon Exp $

#ifndef SCLXSLUI__INC
#define SCLXSLUI__INC

#define SCLXSLUI_NAME		"SCLXSLUI"

#define	SCLXSLUI_VERSION	"$Revision: 1.9 $"

#define SCLXSLUI_OWNER		"Claude SIMON"

#if defined( E_DEBUG ) && !defined( SCLXSLUI_NODBG )
#define SCLXSLUI_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.9 $
//C Claude SIMON (csimon at zeusw dot org)
//R $Date: 2012/11/14 16:06:23 $

/* End of automatic documentation generation part. */

/* Addendum to the automatic documentation generation part. */
//D SoCLe XSL UI 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

# include "err.h"
# include "flw.h"
# include "xml.h"
# include "fbltyp.h"

namespace sclxslui {
	template <typename type> inline void _PutId(
		type Id,
		const char *Name,
		type UndefinedValue,
		bso::bool__ Always,
		xml::writer_ &Writer )
	{
		if ( Id != UndefinedValue )
			xml::PutAttribute( Name, *Id, Writer );
		else if ( Always )
			Writer.PutAttribute( Name, "" );
	}

	inline void PutId(
		fbltyp::id__ Id,
		const char *Name,
		bso::bool__ Always,
		xml::writer_ &Writer )
	{
		_PutId( Id, Name, FBLTYP_UNDEFINED_ID, Always, Writer );
	}

	inline void PutId(
		fbltyp::id32__ Id,
		const char *Name,
		bso::bool__ Always,
		xml::writer_ &Writer )
	{
		_PutId( Id, Name, FBLTYP_UNDEFINED_ID32, Always, Writer );
	}

	inline void PutId(
		fbltyp::id16__ Id,
		const char *Name,
		bso::bool__ Always,
		xml::writer_ &Writer )
	{
		_PutId( Id, Name, FBLTYP_UNDEFINED_ID16, Always, Writer );
	}

	inline void PutId(
		fbltyp::id8__ Id,
		const char *Name,
		bso::bool__ Always,
		xml::writer_ &Writer )
	{
		_PutId( Id, Name, FBLTYP_UNDEFINED_ID8, Always, Writer );
	}
}

/*$END$*/
#endif
