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

// Frontend/Backend Layout CaSTs

#ifndef FBLCST_INC_
# define FBLCST_INC_

# define FBLCST_NAME		"FBLCST"

# if defined( E_DEBUG ) && !defined( FBLCST_NODBG )
#  define FBLCST_DBG
# endif

# include "err.h"
# include "flw.h"

# include "fbltyp.h"

namespace fblcst {
	//e casts.
	enum cast__ {
		//i End of request or end of parameters list.
		cEnd = 0,
		//i Object ('bso::u16__')'.
		cObject,
		//i Boolean ('bso::bool__')
		cBoolean,
		//i Booleans ('bitbch::bit_bunch_')
		cBooleans,
		//i Portable int (bso::sint__ ).
		cSInt,
		//i Portable ints (SET_( bso::sint__ ) ).
		cSInts,
		//i Portable unsigned long (bso::uint__ ).
		cUInt,
		//i Portable unsigned longs (SET_( bso::uint__ ) ).
		cUInts,
		//i Portable big (bso::sbig__ ).
		cSBig,
		//i Portable bigs (SET_( bso::sbig__ ) ).
		cSBigs,
		//i Portable unsigned long (bso::ubig__ ).
		cUBig,
		//i Portable unsigned longs (SET_( bso::ubig__ ) ).
		cUBigs,
		//i 8 bits identifier ( 'bso::u8__' ).
		cId8,
		//i Array of 8 bits identifiers ( 'SET_( bso::u8__ )' )
		cId8s,
		//i Array of array of 8 bits identifiers ( 'XMCONTAINER_( SET_( bso::u8__ ) )' )
		cXId8s,
		//i 16 bits identifier ( 'bso::u16__' ).
		cId16,
		//i Array of 16 bits identifiers ( 'SET_( bso::u16__ )' )
		cId16s,
		//i Array of array of 16 bits identifiers ( 'XMCONTAINER_( SET_( bso::u16_ ) )' )
		cXId16s,
		//i 32 bits identifier ('bso::u32__').
		cId32,
		//i Array of 32 bits identifiers ( 'SET_( bso::u32__ )' )
		cId32s,
		//i Array of array of 32 bits identifiers ( 'XMCONTAINER_( SET_( bso::u32__ ) )' )
		cXId32s,
		//i Generic identifier ( 'bso::uint__' ).
		cId,
		//i Array of generic identifier ( 'SET_( bso::uint__ )' ).
		cIds,
		//i Array of array of bits identifiers ( 'XMCONTAINER_( SET_( bso::uint__ ) )' )
		cXIds,
		//i Character ('bso::char__' ).
		cChar,
		//i String ('str::string_').
		cString,
		//i Array of strings ( 'XMCONTAINER_( str::string_ )' )
		cStrings,
		//i Array of array of strings ( 'XCONTAINER_( strings_ )' )
		cXStrings,
		cStringsSet = cXStrings,
		/*i A byte, as data common data cluster for all plateform.
		Has no intrensec signification ('bso::raw__'). */
		cByte,
		//i Binary 'SET_( bso::raw__ )'.
		cBinary,
		//i Array of binaries ( 'XMCONTAINER_( SET_( bso::raw__ ) )' ).
		cBinaries,
		//i An array of 8 bits items ( 'XMCONTAINER_( { bso::u8__, str::string_ } )' ).
		cItem8s,
		//i An array of 16 bits items ( 'XMCONTAINER_( { bso::u16__, str::string_ } )' ).
		cItem16s,
		//i An array of 32 bits items ( 'XMCONTAINER_( { bso::u32__, str::string_ } )' ).
		cItem32s,
		//i An array of items ( 'XMCONTAINER_( { bso::u32__, str::string_ } )' ).
		cItems,
		//i An array of 8 bits extended items ( 'XMCONTAINER_( { bso::uint__, XMCONTAINER_( str::string_ ) } )' ).
		cIStrings = cItems,
		cXItem8s,
		//i An array of 16 bits extended items ( 'XMCONTAINER_( { bso::u8__, XMCONTAINER_( str::string_ ) } )' ).
		cXItem16s,
		//i An array of 32 bits extended items ( 'XMCONTAINER_( { bso::u8__, XMCONTAINER_( str::string_ ) } )' ).
		cXItem32s,
		//i An array of extended items ( 'XMCONTAINER_( { bso::uint__, XMCONTAINER_( str::string_ ) } )' ).
		cXItems,
		// The following casts are used for some special commands.
		//i Array of command descriptions ( 'XCONTAINER_( { str::string_, SET_( bso::u8__ ) } )' ).
		cCommandsDetails,
		//i Array of object types and identifiers. ( 'SET_( { bso::u16__, bso::u16__ } )' ).
		cObjectsReferences,
		//i Flux gnric. Doit tre plac en dernier de laiste des paramtres entrants et/ou sortants.
		cFlow,
		//i Amount of casts.
		c_amount,
		//i Not a cast, but signalize an invalid cast.
		c_Undefined,
	};	// A modification must be reflected by 'CastsNames'.

//	typedef fbltyp::id8_t__ cast__;
	typedef fbltyp::id8s_ casts_;
	typedef fbltyp::id8s casts;


	//o The casts names.
	extern const char *CastsNames[c_amount];

	//f Get id from cast named 'CastName'.
	cast__ GetID( const char *CastName );
}
#endif
