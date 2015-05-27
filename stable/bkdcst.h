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

//	$Id: bkdcst.h,v 1.2 2011/06/11 18:16:00 csimon Exp $

#ifndef BKDCST__INC
#define BKDCST__INC

#define BKDCST_NAME		"BKDCST"

#define	BKDCST_VERSION	"$Revision: 1.2 $"

#define BKDCST_OWNER		"Claude SIMON"

#if defined( E_DEBUG ) && !defined( BKDCST_NODBG )
#define BKDCST_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.2 $
//C Claude SIMON (csimon at zeusw dot org)
//R $Date: 2011/06/11 18:16:00 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D BacKenD CaSTs 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

# error "Obsolete ! Use 'FBL...' libraries instead !"

#include "err.h"
#include "flw.h"

//d Version casts version.
#define BKDCST_CASTS_VERSION	"1"

namespace bkdcst {
	//e casts.
	enum cast {
		//i End of request or end of parameters list.
		cEnd = 0,
		//i Object ('bso::ushort__')'.
		cObject,
		//i Boolean ('bso::bool__')
		cBoolean,
		//i Booleans ('bitbch::bit_bunch_')
		cBooleans,
		//i Portable signed long (bso::slong__ ).
		cSLong,
		//i Portable signed longs (SET_( bso::slong__ ) ).
		cSLongs,
		//i Portable unsigned long (bso::ulong__ ).
		cULong,
		//i Portable unsigned longs (SET_( bso::ulong__ ) ).
		cULongs,
		//i 8 bits identifier ( 'bso::ubyte__' ).
		cId8,
		//i Array of 8 bits identifiers ( 'SET_( bso::ubyte__ )' )
		cIds8,
		//i Array of array of 8 bits identifiers ( 'XMCONTAINER_( SET_( bso::ubyte__ ) )' )
		cXIds8,
		//i 16 bits identifier ( 'bso::ushort__' ).
		cId16,
		//i Array of 16 bits identifiers ( 'SET_( bso::ushort__ )' )
		cIds16,
		//i Array of array of 16 bits identifiers ( 'XMCONTAINER_( SET_( bso::ushort_ ) )' )
		cXIds16,
		//i 32 bits identifier ('bso::ulong__').
		cId32,
		//i Array of 32 bits identifiers ( 'SET_( bso::ulong__ )' )
		cIds32,
		//i Array of array of 32 bits identifiers ( 'XMCONTAINER_( SET_( bso::ulong__ ) )' )
		cXIds32,
		//i Character ('bso::char__' ).
		cChar,
		//i String ('str::string_').
		cString,
		//i Array of strings ( 'XMCONTAINER_( str::string_ )' )
		cStrings,
		//i Array of array of strings ( 'XCONTAINER_( strings_ )' )
		cXStrings,
		/*i A byte, as data common data cluster for all plateform.
		Has no intrensec signification ('bso::raw__'). */
		cByte,
		//i Binary 'SET_( bso::raw__ )'.
		cBinary,
		//i Array of binaries ( 'XMCONTAINER_( SET_( bso::raw__ ) )' ).
		cBinaries,
		//i An array of 8 bits items ( 'XMCONTAINER_( { bso::ubyte__, str::string_ } )' ).
		cItems8,
		//i An array of 16 bits items ( 'XMCONTAINER_( { bso::ushort__, str::string_ } )' ).
		cItems16,
		//i An array of 32 bits items ( 'XMCONTAINER_( { bso::ulong__, str::string_ } )' ).
		cItems32,
		//i An array of 8 bits extended items ( 'XMCONTAINER_( { bso::ubyte__, XMCONTAINER_( str::string_ ) } )' ).
		cXItems8,
		//i An array of 16 bits extended items ( 'XMCONTAINER_( { bso::ubyte__, XMCONTAINER_( str::string_ ) } )' ).
		cXItems16,
		//i An array of 32 bits extended items ( 'XMCONTAINER_( { bso::ubyte__, XMCONTAINER_( str::string_ ) } )' ).
		cXItems32,
		// The following casts are used for some special commands.
		//i Array of command descriptions ( 'XCONTAINER_( { str::string_, SET_( bso::ubyte__ ) } )' ).
		cCommandsDetails,
		//i Array of object types and identifiers. ( 'SET_( { bso::ushort__, bso::ushort__ } )' ).
		cObjectsReferences,
		//i Amount of casts.
		c_amount,
		//i Not a cast, but signalize an invalid cast.
		c_Unknow,
	};	// A modification must be reflected by 'CastsNames'.

	//o The casts names.
	extern const char *CastsNames[c_amount];

	//f Get id from cast named 'CastName'.
	cast GetID( const char *CastName );

}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
