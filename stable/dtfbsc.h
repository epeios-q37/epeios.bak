/*
	Header for the 'dtfbsc' library by Claude SIMON (csimon at zeusw dot org)
	Copyright (C) 2000-2001, 2004 Claude SIMON.

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

//	$Id: dtfbsc.h,v 1.12 2013/03/29 11:02:50 csimon Exp $

#ifndef DTFBSC__INC
#define DTFBSC__INC

#define DTFBSC_NAME		"DTFBSC"

#define	DTFBSC_VERSION	"$Revision: 1.12 $"

#define DTFBSC_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &DTFBSCTutor;

#if defined( E_DEBUG ) && !defined( DTFBSC_NODBG )
#define DTFBSC_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.12 $
//C Claude SIMON (csimon at zeusw dot org)
//R $Date: 2013/03/29 11:02:50 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D Data TransFert BaSiC 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

/* Addendum to the automatic documentation generation part. */
//D Data transFer BaSiC.
/* End addendum to automatic documentation generation part. */


#include "err.h"
#include "flw.h"
#include "bso.h"

namespace dtfbsc {
	using bso::u8__;
	using bso::s8__;
	using bso::u16__;
	using bso::s16__;
	using bso::u32__;
	using bso::s32__;

# ifdef BSO_64BITS_ENABLED
	using bso::u64__;
	using bso::s64__;
#endif

	//f Put 'UByte' in 'Flow'.
	inline void PutU8(
		u8__ U8,
		flw::oflow__ &Flow )
	{
		Flow.Put( *(flw::datum__ *)&U8 );
	}

	//f Return unsigned byte in 'Flow'.
	inline u8__ GetU8( flw::iflow__ &Flow )
	{
		return (u8__)Flow.Get();
	}

	//f Put 'SByte' in 'Flow'.
	inline void PutS8(
		s8__ S8,
		flw::oflow__ &Flow )
	{
		Flow.Put( *(flw::datum__ *)&S8 );
	}

	//f Return unsigned byte in 'Flow'.
	inline s8__ GetS8( flw::iflow__ &Flow )
	{
		return (s8__)Flow.Get();
	}

}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
