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

//	$Id: dtfntv.h,v 1.4 2012/11/14 16:06:26 csimon Exp $

#ifndef DTFNTV__INC
#define DTFNTV__INC

#define DTFNTV_NAME		"DTFNTV"

#define	DTFNTV_VERSION	"$Revision: 1.4 $"

#define DTFNTV_OWNER		"Claude SIMON (http://zeusw.org/intl/contact.html)"

#if defined( E_DEBUG ) && !defined( DTFNTV_NODBG )
#define DTFNTV_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.4 $
//C Claude SIMON (http://zeusw.org/intl/contact.html)
//R $Date: 2012/11/14 16:06:26 $

/* End of automatic documentation generation part. */

/* Addendum to the automatic documentation generation part. */
//D Data TransFert NaTiVe 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"
#include "dtfbsc.h"

namespace dtfntv {
	using namespace dtfbsc;

	//f Put 'Object', which is a pointer of type 'object', in 'Flow'.	
	template <typename object> inline void PutPointer(
		const object *Object,
		flw::oflow___ &Flow )
	{
		flw::Put( Object, Flow );
	}
		
	//f Get from 'Flow' a pointer to an object of type 'object'.
	template <typename object> inline object *GetPointer( flw::iflow___ &Flow )
	{
		object *P;
		
		flw::Get( Flow, P );
		
		return P;
	}

	template <typename object__> inline void _Put(
		const object__ &Object,
		flw::oflow___ &Flow )
	{
		flw::Put( Object, Flow );
	}

	template <typename object__> inline object__ _Get( flw::iflow___ &Flow )
	{
		flw::datum__ Object[sizeof( object__ )];

		Flow.Get( sizeof( object__), Object );

		return *(object__ *)Object;
	}

	//f Put 'UShort' into 'Flow'.
	inline void PutUShort(
		ushort__ UShort,
		flw::oflow___ &Flow )
	{
		_Put( UShort, Flow );
	}

	//f Return the unsigned short stored in 'Flow'.
	inline ushort__ GetUShort( flw::iflow___ &Flow )
	{
		return _Get<ushort__>( Flow );
	}

	//f Put 'SShort' into 'Flow'.
	inline void PutSShort(
		sshort__ SShort,
		flw::oflow___ &Flow )
	{
		_Put( SShort, Flow );
	}

	//f Return the signed short stored in 'Flow'.
	inline sshort__ GetSShort( flw::iflow___ &Flow )
	{
		return _Get<sshort__>( Flow );
	}

	//f Put 'ULong' into 'Flow'.
	inline void PutULong(
		ushort__ ULong,
		flw::oflow___ &Flow )
	{
		_Put( ULong, Flow );
	}

	//f Return the unsigned long stored in 'Flow'.
	inline ulong__ GetULong( flw::iflow___ &Flow )
	{
		return _Get<ulong__>( Flow );
	}

	//f Put 'SLong' into 'Flow'.
	inline void PutSLong(
		sshort__ SLong,
		flw::oflow___ &Flow )
	{
		_Put( SLong, Flow );
	}

	//f Return the signed long stored in 'Flow'.
	inline slong__ GetSLong( flw::iflow___ &Flow )
	{
		return _Get<slong__>( Flow );
	}

}

/*$END$*/
#endif
