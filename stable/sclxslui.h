/*
	Header for the 'sclxslui' library by Claude SIMON (csimon at zeusw dot org)
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

//	$Id: xxx.h,v 1.9 2012/11/14 16:06:23 csimon Exp $

#ifndef SCLXSLUI__INC
#define SCLXSLUI__INC

#define SCLXSLUI_NAME		"SCLXSLUI"

#define	SCLXSLUI_VERSION	"$Revision: 1.9 $"

#define SCLXSLUI_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &SCLXSLUITutor;

#if defined( E_DEBUG ) && !defined( SCLXSLUI_NODBG )
#define SCLXSLUI_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.9 $
//C Claude SIMON (csimon at zeusw dot org)
//R $Date: 2012/11/14 16:06:23 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

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
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
