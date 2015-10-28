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

//	$Id: bfl.h,v 1.4 2012/11/14 16:06:24 csimon Exp $

#ifndef BFL__INC
#define BFL__INC

#define BFL_NAME		"BFL"

#if defined( E_DEBUG ) && !defined( BFL_NODBG )
#define BFL_DBG
#endif

//D Bit FLow 

#include "err.h"
#include "flw.h"

namespace bfl {
	using flw::bsize__;
	using flw::datum__;

	//c Input text flow.
	class bit_iflow__
	{
	private:
		// 'Flow' duquel sont lues les donnes.
		flw::iflow__ &_Flow;
		datum__ *_Datum;
		bso::ubyte__ _Counter;
		bso::bool__ _Get( void )
		{
			if ( _Counter == 0 ) {
				_Datum = _Flow.GetCurrentCacheDatum( true );
				_Counter = 8 * sizeof( *_Datum );
			} else
				*_Datum >>= 1;

			_Counter--;

			return *_Datum & 1;
		}
	public:
		bit_iflow__( flw::iflow__ &Flow )
		: _Flow( Flow )
		{
			_Datum = NULL;
			_Counter = 0;
		}
		bso::bool__ Get( void )
		{
			return _Get();
		}
	};


	//c Output text flow.
	class bit_oflow__
	{
	private:
		// 'Flow' dans lequel sont crites les donnes.
		flw::oflow__ &_Flow;
		datum__ *_Datum;
		bso::ubyte__ _Counter;
		void _Put( bso::bool__ Value )
		{
			if ( _Counter == 0 ) {
				_Datum = _Flow.GetCurrentCacheDatum( true );
				_Counter = 8 * sizeof( *_Datum );
			}

			*_Datum  = ( *_Datum >> 1 ) | ( Value ? 0x80 : 0 );

			_Counter--;
		}
	public:
		bit_oflow__( flw::oflow__ &Flow )
		: _Flow( Flow )
		{
			_Datum = NULL;
			_Counter = 0;
		}
		void Put( bso::bool__ Value )
		{
			_Put( Value );
		}
	};
}
#endif
