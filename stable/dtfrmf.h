/*
	Header for the 'dtfrmf' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Copyright (C) 2000-2001, 2003 Claude SIMON (http://zeusw.org/intl/contact.html).

	This file is part of the Epeios (http://zeusw.org/epeios/) project.

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 3
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

//	$Id: dtfrmf.h,v 1.13 2012/11/14 16:06:27 csimon Exp $

#ifndef DTFRMF__INC
#define DTFRMF__INC

#define DTFRMF_NAME		"DTFRMF"

#define	DTFRMF_VERSION	"$Revision: 1.13 $"

#define DTFRMF_OWNER		"Claude SIMON (http://zeusw.org/intl/contact.html)"

#if defined( E_DEBUG ) && !defined( DTFRMF_NODBG )
#define DTFRMF_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.13 $
//C Claude SIMON (http://zeusw.org/intl/contact.html)
//R $Date: 2012/11/14 16:06:27 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D Data TransFert ReMote Features 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#error "Obsolete. Use 'dtfptb' library instead."

#include "err.h"
#include "flw.h"
#include "dtfbsc.h"
#include "flx.h"

namespace dtfrmf {
	using namespace dtfbsc;
	
	//f Put 'Short' in 'Flow'.
	inline void PutShort(
		short__ Short,
		flw::oflow___ &Flow )
	{
		PutByte( (byte__)( Short & 0xff ), Flow );
		PutByte( (byte__)( Short >> 8 ), Flow );
	}

	//f Return short in 'Flow'
	inline short__ GetShort( flw::iflow___ &Flow )
	{
		flw::datum__ Data[2];

		Flow.Get(2, Data );

		return (short__)( (short__)(bso::ubyte__)Data[0] | (short__)( (short__)(bso::ubyte__)Data[1] << (short__)8 ) );

		// return (short__)( (short__)Data[0] + (short__)( (short__)Data[1] << 8 ) );
		// Don't work with CW : in 'a + b', seems to evaluate 'b' before 'a'.
	}

	//f Put 'Long' in 'Flow'.
	inline void PutLong(
		long__ Long,
		flw::oflow___ &Flow )
	{
		PutShort( (short__)( Long & 0xffff ), Flow );
		PutShort( (short__)( Long >> 16 ), Flow );
	}

	//f Return long in 'Flow'
	inline long__ GetLong( flw::iflow___ &Flow )
	{
		flw::datum__ Data[4];

		Flow.Get( 4, Data );

		return (long__)(bso::ubyte__)Data[0] | ( (long__)(bso::ubyte__)Data[1] << 8 ) | ( (long__)(bso::ubyte__)Data[2] << 16 ) | ( (long__)(bso::ubyte__)Data[3] << 24 );

		// return (long__)( (long__)GetShort( Flow ) | (long__)( (long__)GetShort( Flow ) << 16 ) );
		// Don't work with CW : in 'a + b', seems to evaluate 'b' before 'a'.
	}

	//f Write 'Size' into 'Flow'.
	void PutSize(
		bso::ulong__ Size,
		flw::oflow___ &Flow );
	
	//f Return size stored in 'Flow'.
	bso::ulong__ GetSize( flw::iflow___ &Flow );

	//f Encapsulates 'Data' in flow (put size and then the contents of data as is).
	void Encapsulate(
		const flx::bunch_ &Data,
		flw::oflow___ &Flow );
		
	//f Decapsulate what in 'Flow' into 'Data'.
	void Decapsulate(
		flw::iflow___ &Flow,
		flx::bunch_ &Data );
		
		
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
