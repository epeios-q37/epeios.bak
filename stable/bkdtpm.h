/*
	Header for the 'bkdtpm' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Copyright (C) 2001-2004 Claude SIMON (http://zeusw.org/intl/contact.html).

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

//	$Id: bkdtpm.h,v 1.4 2011/06/11 18:16:00 csimon Exp $

#ifndef BKDTPM__INC
#define BKDTPM__INC

#define BKDTPM_NAME		"BKDTPM"

#define	BKDTPM_VERSION	"$Revision: 1.4 $"

#define BKDTPM_OWNER		"Claude SIMON (http://zeusw.org/intl/contact.html)"

#include "ttr.h"

extern class ttr_tutor &BKDTPMTutor;

#if defined( E_DEBUG ) && !defined( BKDTPM_NODBG )
#define BKDTPM_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.4 $
//C Claude SIMON (http://zeusw.org/intl/contact.html)
//R $Date: 2011/06/11 18:16:00 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D BacKenD TyPes Manager 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

# error "Obsolete ! Use 'FBL...' libraries instead !"

#include "err.h"
#include "flw.h"
#include "str.h"
#include "ctn.h"
#include "bitbch.h"
#include "dtfptb.h"
#include "cpe.h"

#ifdef CPE__VC
#	undef GetObject
#endif

#define BKDTPM_UNDEFINED_ID32	((bkdtpm::id32__)BSO_ULONG_MAX)
#define BKDTPM_UNDEFINED_ID16	((bkdtpm::id16__)BSO_USHORT_MAX)
#define BKDTPM_UNDEFINED_ID8	((bkdtpm::id8__)BSO_UBYTE_MAX)

#define BKDTPM_ITEM( id, name )\
	typedef bkdtpm::item_<id>	name##_;\
	E_AUTO( name )

#define BKDTPM_ITEMS( item, name )\
	typedef ctn::E_XMCONTAINER_( item##_ )	name##_;\
	E_AUTO( name )

#define BKDTPM_XITEM( id, name )\
	typedef bkdtpm::extended_item_<id>	name##_;\
	E_AUTO( name )

#define BKDTPM_XITEMS( item, name )\
	typedef ctn::E_XCONTAINER_( item##_ )	name##_;\
	E_AUTO( name )

namespace bkdtpm {

	//t Value.
	typedef str::string_ value_;
	typedef str::string value;

	template <class id> class item_
	{
	public:
		value_ Value;
		struct s
		{
			value_::s Value;
			id ID;
		} &S_;
		item_( s &S )
		: S_( S ),
		  Value( S.Value )
		{}
		void reset( bool P = true )
		{
			Value.reset( P );
		}
		void plug( mdr::E_MEMORY_DRIVER__ &MD )
		{
			Value.plug( MD );
		}
		void plug( mmm::multimemory_ &M )
		{
			Value.plug( M );
		}
		item_ &operator =( const item_ &I )
		{
			Value = I.Value;
			S_.ID = I.S_.ID;

			return *this;
		}
		//f Initialization.
		void Init( void )
		{
			Value.Init();
		}
		//f Initialization with id 'ID' and value 'Value'.
		void Init(
			id ID,
			const value_ &Value )
		{
			this->Value.Init( Value );
			S_.ID = ID;
		}
		//f Return the value of the ID.
		id ID( void ) const
		{
			return S_.ID;
		}
		//f 'ID' becomes the ID.
		void ID( id ID ) 
		{
			S_.ID = ID;
		}
	};

	//f Values.
	typedef ctn::E_XMCONTAINER_( value_ ) values_;
	E_AUTO( values )

	template <class id> class extended_item_
	{
	public:
		values_ Values;
		struct s
		{
			values_::s Values;
			id ID;
		} &S_;
		extended_item_( s &S )
		: S_( S ),
		  Values( S.Values )
		{}
		void reset( bool P = true )
		{
			Values.reset( P );
		}
		void plug( mmm::multimemory_ &M )
		{
			Values.plug( M );
		}
		extended_item_ &operator =( const extended_item_ &I )
		{
			Values = I.Values;
			S_.ID = I.S_.ID;

			return *this;
		}
		//f Initialization.
		void Init( void )
		{
			Values.Init();
		}
		//f Return the value of the ID.
		id ID( void ) const
		{
			return S_.ID;
		}
		//f 'ID' becomes the ID.
		void ID( id ID ) 
		{
			S_.ID = ID;
		}
	};

	//t Name.
	typedef str::string_ name_;
	E_AUTO( name )

	template <class t, typename r> class detail_
	{
	public:
		name_ Name;
		bch::E_BUNCHt_( t, r ) Casts;
		struct s {
			name_::s Name;
			typename bch::E_BUNCHt_( t, r )::s Casts;
		};
		detail_( s &S )
		: Name( S.Name ),
		  Casts( S.Casts )
		{}
		void reset( bool P = true )
		{
			Name.reset( P );
			Casts.reset( P );
		}
		void plug( mmm::multimemory_ &M )
		{
			Name.plug( M );
			Casts.plug( M );
		}
		detail_ &operator =( const detail_ &CI )
		{
			Name = CI.Name;
			Casts = CI.Casts;

			return *this;
		}
		// Initialization.
		void Init( void )
		{
			Name.Init();
			Casts.Init();
		}
	};

	E_AUTO2( detail )

	struct object_reference__
	{
		bso::ushort__ Type;
		bso::ushort__ Identifier;
	};

	typedef bso::ushort__	object_t__;
	E_TYPEDEF( object_t__, object__ );
	typedef bso::bool__ boolean_t__;
	typedef boolean_t__	boolean__;
	typedef bitbch::bit_bunch_<epeios::row__>	booleans_;
	E_AUTO( booleans )
	typedef bso::slong__	slong__;
	typedef bch::E_BUNCH_( slong__ ) slongs_;
	typedef bch::E_BUNCH( slong__ ) slongs;
	typedef bso::ulong__	ulong__;
	typedef bch::E_BUNCH_( ulong__ ) ulongs_;
	typedef bch::E_BUNCH( ulong__ ) ulongs;
	typedef bso::ubyte__ id8_t__;
	E_TYPEDEF( id8_t__,	id8__ );
	typedef bch::E_BUNCH_( id8__ ) ids8_;
	typedef bch::E_BUNCH( id8__ ) ids8;
	typedef ctn::E_XMCONTAINER_( ids8_ ) xids8_;
	typedef ctn::E_XMCONTAINER( ids8_ ) xids8;
	typedef bso::ushort__ id16_t__;
	E_TYPEDEF( id16_t__,	id16__ );
	typedef bch::E_BUNCH_( id16__ ) ids16_;
	typedef bch::E_BUNCH( id16__ ) ids16;
	typedef ctn::E_XMCONTAINER_( ids16_ ) xids16_;
	typedef ctn::E_XMCONTAINER( ids16_ ) xids16;
	typedef bso::ulong__ id32_t__;
	E_TYPEDEF( id32_t__,	id32__ );
	typedef bch::E_BUNCH_( id32__ ) ids32_;
	typedef bch::E_BUNCH( id32__ ) ids32;
	typedef ctn::E_XMCONTAINER_( ids32_ ) xids32_;
	typedef ctn::E_XMCONTAINER( ids32_ ) xids32;
	typedef bso::char__ char_t__;
	typedef char_t__	char__;
	typedef str::string_	string_;
	typedef str::string		string;
	typedef ctn::E_XMCONTAINER_( string_ )	strings_;
	typedef ctn::E_XMCONTAINER( string_ )	strings;
	typedef ctn::E_XCONTAINER_( strings_ )	xstrings_;
	typedef ctn::E_XCONTAINER( strings_ )	xstrings;
	typedef bso::raw__	byte_t__;
	typedef byte_t__	byte__;
	typedef bch::E_BUNCH_( bso::raw__ ) binary_;
	typedef bch::E_BUNCH( bso::raw__ )	binary;
	typedef ctn::E_XMCONTAINER_( binary_ ) binaries_;
	typedef ctn::E_XMCONTAINER( binary_ ) binaries;
	BKDTPM_ITEM( id8__, item8 )
	BKDTPM_ITEMS( item8, items8 )
	BKDTPM_ITEM( id16__, item16 )
	BKDTPM_ITEMS( item16, items16 )
	BKDTPM_ITEM( id32__, item32 )
	BKDTPM_ITEMS( item32, items32 )
	BKDTPM_XITEM( id8__, xitem8 )
	BKDTPM_XITEMS( xitem8, xitems8 )
	BKDTPM_XITEM( id16__, xitem16 )
	BKDTPM_XITEMS( xitem16, xitems16 )
	BKDTPM_XITEM( id32__, xitem32 )
	BKDTPM_XITEMS( xitem32, xitems32 )
	/* Both below declaration are not part of the protocol. */
	typedef bkdtpm::detail_<id8__, epeios::row__>	command_detail_	;
	typedef bkdtpm::detail<id8__, epeios::row__>	command_detail;
	/**/
	typedef ctn::E_XCONTAINER_( command_detail_ ) commands_details_;
	typedef ctn::E_XCONTAINER( command_detail_ ) commands_details;
	typedef bch::E_BUNCH_( object_reference__ ) objects_references_;
	typedef bch::E_BUNCH( object_reference__ ) objects_references;

	template <typename t> inline t *_New( void )
	{
		t *P = new t;

		if ( P == NULL )
			ERRa();

		return P;
	}

	template <typename t> inline void _Delete( t *P )
	{
		delete P;
	}

	template <typename t> inline t *_SNew( void )
	{
		return _New<t>();
	}

	template <typename t> inline void _SDelete( t *P )
	{
		_Delete( P );
	}

	template <typename t> inline t *_DNew( void )
	{
		t *P = _New<t>();

		P->Init();

		return P;
	}

	template <typename t> inline void _DDelete( t *P )
	{
		_Delete( P );
	}

#define BKDTPM_S( name, type )\
	inline type *New##name( void )\
	{\
		return _SNew<type>();\
	}\
	inline void Delete##name( type *P )\
	{\
		_SDelete( P );\
	}\

#define BKDTPM_D( name, type )\
	inline type *New##name( void )\
	{\
		return _DNew<type>();\
	}\
	inline void Delete##name( type *P )\
	{\
		_DDelete( P );\
	}\


	//f Put 'Object' into 'OFlow'.
	inline void PutObject(
		object__ Object,
		flw::oflow__ &OFlow )
	{
		flw::Put( Object, OFlow );
	}

	//f Return 'Object' from 'IFlow'.
	inline void GetObject(
		flw::iflow__ &IFlow,
		object__ &Object )
	{
		flw::Get( IFlow, Object );
	}

	BKDTPM_S( Object, object__ )
	
	//f Put 'Boolean' into 'OFlow'.
	inline void PutBoolean(
		boolean__ Boolean,
		flw::oflow__ &OFlow )
	{
		flw::Put( Boolean, OFlow );
	}

	//f Return 'Boolean' from 'IFlow'.
	inline void GetBoolean(
		flw::iflow__ &IFlow,
		boolean__ &Boolean )
	{
		flw::Get( IFlow, Boolean );
	}

	BKDTPM_S( Boolean, boolean__ )
	
	//f Put 'Booleans' into 'OFlow'.
	void PutBooleans(
		const booleans_ &Booleans,
		flw::oflow__ &OFlow );

	//f Return 'Booleans' from 'IFlow'.
	void GetBooleans(
		flw::iflow__ &IFlow,
		booleans_ &Booleans );

	BKDTPM_D( Booleans, booleans )

	//f Put 'Slong' into 'Flow'.
	inline void PutSLong(
		slong__ SLong,
		flw::oflow__ &OFlow )
	{
		dtfptb::FittedPutSLong( SLong, OFlow );
	}

	//f Get 'SLong' from 'IFlow'.
	inline void GetSLong(
		flw::iflow__ &IFlow,
		slong__ &SLong )
	{
		SLong = dtfptb::FittedGetSLong( IFlow );
	}
	
	BKDTPM_S( SLong, slong__ )

	//f Put 'Slongs' into 'Flow'.
	void PutSLongs(
		const slongs_ &SLongs,
		flw::oflow__ &OFlow );

	//f Get 'SLongs' from 'IFlow'.
	void GetSLongs(
		flw::iflow__ &IFlow,
		slongs_ &SLongs );

	BKDTPM_D( SLongs, slongs )

	//f Put 'Ulong' into 'Flow'.
	inline void PutULong(
		ulong__ ULong,
		flw::oflow__ &OFlow )
	{
		dtfptb::FittedPutULong( ULong, OFlow );
	}

	//f Get 'ULong' from 'IFlow'.
	inline void GetULong(
		flw::iflow__ &IFlow,
		ulong__ &ULong )
	{
		ULong = dtfptb::FittedGetULong( IFlow );
	}
	
	BKDTPM_S( ULong, ulong__ )

	//f Put 'Ulongs' into 'Flow'.
	void PutULongs(
		const ulongs_ &ULongs,
		flw::oflow__ &OFlow );

	//f Get 'ULongs' from 'IFlow'.
	void GetULongs(
		flw::iflow__ &IFlow,
		ulongs_ &ULongs );

	BKDTPM_D( ULongs, ulongs )

	//f Put 'Id8' into 'OFlow'.
	inline void PutId8(
		id8__ Id8,
		flw::oflow__ &OFlow )
	{
		flw::Put( Id8, OFlow );
	}

	//f Return 'Id8' from 'IFlow'.
	inline void GetId8(
		flw::iflow__ &IFlow,
		id8__ &Id8 )
	{
		flw::Get( IFlow, Id8 );
	}
	
	BKDTPM_S( Id8, id8__ )

	//f Put 'Ids8' into 'OFlow'.
	void PutIds8(
		const ids8_ &Ids8,
		flw::oflow__ &OFlow );

	//f Get 'Ids8' from 'OFlow'.
	void GetIds8(
		flw::iflow__ &IFlow,
		ids8_ &Ids8 );

	BKDTPM_D( Ids8, ids8 )

	//f Put 'XIds8' into 'OFlow'.
	void PutXIds8(
		const xids8_ &XIds8,
		flw::oflow__ &OFlow );

	//f Get 'XIds8' from 'OFlow'.
	void GetXIds8(
		flw::iflow__ &IFlow,
		xids8_ &XIds8 );

	BKDTPM_D( XIds8, xids8 )

	//f Put 'Id16' into 'OFlow'.
	inline void PutId16(
		id16__ Id16,
		flw::oflow__ &OFlow )
	{
		flw::Put( Id16, OFlow );
	}

	//f Return 'Id16' from 'IFlow'.
	inline void GetId16(
		flw::iflow__ &IFlow,
		id16__ &Id16 )
	{
		flw::Get( IFlow, Id16 );
	}
	
	BKDTPM_S( Id16, id16__ )

	//f Put 'Ids16' into 'OFlow'.
	void PutIds16(
		const ids16_ &Ids16,
		flw::oflow__ &OFlow );

	//f Get 'Ids16' from 'OFlow'.
	void GetIds16(
		flw::iflow__ &IFlow,
		ids16_ &Ids16 );

	BKDTPM_D( Ids16, ids16 )

	//f Put 'XIds16' into 'OFlow'.
	void PutXIds16(
		const xids16_ &XIds16,
		flw::oflow__ &OFlow );

	//f Get 'XIds16' from 'OFlow'.
	void GetXIds16(
		flw::iflow__ &IFlow,
		xids16_ &XIds16 );

	BKDTPM_D( XIds16, xids16 )

	//f Put 'Id32' into 'OFlow'.
	inline void PutId32(
		id32__ Id32,
		flw::oflow__ &OFlow )
	{
		flw::Put( Id32, OFlow );
	}

	//f Return 'Id32' from 'IFlow'.
	inline void GetId32(
		flw::iflow__ &IFlow,
		id32__ &Id32 )
	{
		flw::Get( IFlow, Id32 );
	}
	
	BKDTPM_S( Id32, id32__ )

	//f Put 'Ids32' into 'OFlow'.
	void PutIds32(
		const ids32_ &Ids32,
		flw::oflow__ &OFlow );

	//f Get 'Ids32' from 'OFlow'.
	void GetIds32(
		flw::iflow__ &IFlow,
		ids32_ &Ids32 );

	BKDTPM_D( Ids32, ids32 )

	//f Put 'XIds32' into 'OFlow'.
	void PutXIds32(
		const xids32_ &XIds32,
		flw::oflow__ &OFlow );

	//f Get 'XIds32' from 'OFlow'.
	void GetXIds32(
		flw::iflow__ &IFlow,
		xids32_ &XIds32 );

	BKDTPM_D( XIds32, xids32 )

	//f Put 'Char' into 'OFlow'.
	inline void PutChar(
		char__ Char,
		flw::oflow__ &OFlow )
	{
		flw::Put( Char, OFlow );
	}

	//f Return 'Char' from 'IFlow'.
	inline void GetChar(
		flw::iflow__ &IFlow,
		char__ &Char )
	{
		flw::Get( IFlow, Char );
	}

	BKDTPM_S( Char, char__ )

	//f Put 'String' into 'OFlow'.
	inline void PutString(
		const string_ &String,
		flw::oflow__ &OFlow )
	{
		OFlow << String;
	}

	//f Get 'String' from 'IFlow'.
	inline void GetString(
		flw::iflow__ &IFlow,
		string_ &String )
	{
		IFlow >> String;
	}

	BKDTPM_D( String, string )

	//f Put 'Strings' into 'OFlow'.
	void PutStrings(
		const strings_ &Strings,
		flw::oflow__ &OFlow );

	//f Get 'Strings' from 'IFlow'.
	void GetStrings(
		flw::iflow__ &IFlow,
		strings_ &Strings );

	BKDTPM_D( Strings, strings )

	//f Put 'XStrings' into 'OFlow'.
	void PutXStrings(
		const xstrings_ &XStrings,
		flw::oflow__ &OFlow );

	//f Get 'XStrings' from 'IFlow'.
	void GetXStrings(
		flw::iflow__ &IFlow,
		xstrings_ &Strings );

	BKDTPM_D( XStrings, xstrings )

	//f Put 'Byte' into 'OFlow'.
	inline void PutByte(
		byte__ Byte,
		flw::oflow__ &OFlow )
	{
		flw::Put( Byte, OFlow );
	}

	//f Return 'Byte' from 'IFlow'.
	inline void GetByte(
		flw::iflow__ &IFlow,
		byte__ &Byte )
	{
		flw::Get( IFlow, Byte );
	}
	
	BKDTPM_S( Byte, byte__ )

	//f Put 'Binary' into 'OFlow'.
	void PutBinary(
		const binary_ &Binary,
		flw::oflow__ &OFlow );

	//f Get 'Binary' from 'IFlow'.
	void GetBinary(
		flw::iflow__ &IFlow,
		binary_ &Binary );

	BKDTPM_D( Binary, binary )

	//f Put 'Binaries' into 'OFlow'.
	void PutBinaries(
		const binaries_ &Binaries,
		flw::oflow__ &OFlow );

	//f Get 'Binaries' from 'IFlow'.
	void GetBinaries(
		flw::iflow__ &IFlow,
		binaries_ &Binaries );

	BKDTPM_D( Binaries, binaries )

	//f Put 'Item8' into 'OFlow'.
	void PutItem8(
		const item8_ Item8,
		flw::oflow__ &OFlow );

	//f Get 'Items8' from 'IFlow'.
	void GetItem8(
	   flw::iflow__ &IFlow,
	   item8_ &Item8 );

	BKDTPM_D( Items8, items8 )

	//f Put 'Items8' into 'OFlow'.
	void PutItems8(
		const items8_ &Items8,
		flw::oflow__ &OFlow );

	//f Get 'Items8' from 'IFlow'.
	void GetItems8(
		flw::iflow__ &IFlow,
		items8_ &Items8 );

	BKDTPM_D( Items16, items16 )

	//f Put 'Item16' into 'OFlow'.
	void PutItem16(
		const item16_ Item16,
		flw::oflow__ &OFlow );

	//f Get 'Item16' from 'IFlow'.
	void GetItem16(
	   flw::iflow__ &IFlow,
	   item16_ &Item16 );

	BKDTPM_D( Item16, item16 )

	//f Put 'Items16' into 'OFlow'.
	void PutItems16(
		const items16_ &Items16,
		flw::oflow__ &OFlow );

	//f Get 'Items16' from 'IFlow'.
	void GetItems16(
		flw::iflow__ &IFlow,
		items16_ &Items16 );

	BKDTPM_D( items16, items16 )

	//f Put 'Item32' into 'OFlow'.
	void PutItem32(
		const item32_ Item32,
		flw::oflow__ &OFlow );

	//f Get 'Item32' from 'IFlow'.
	void GetItem32(
	   flw::iflow__ &IFlow,
	   item32_ &Item32 );

	BKDTPM_D( Item32, items32 )

	//f Put 'Items32' into 'OFlow'.
	void PutItems32(
		const items32_ &Items32,
		flw::oflow__ &OFlow );

	//f Get 'Items32' from 'IFlow'.
	void GetItems32(
		flw::iflow__ &IFlow,
		items32_ &Items32 );

	BKDTPM_D( Items32, items32 )

	//f Put 'XItems8' into 'OFlow'.
	void PutXItems8(
		const xitems8_ &XItems8,
		flw::oflow__ &OFlow );

	//f Get 'XItems8' from 'IFlow'.
	void GetXItems8(
		flw::iflow__ &IFlow,
		xitems8_ &XItems8 );

	BKDTPM_D( XItems8, xitems8 )

	//f Put 'XItems16' into 'OFlow'.
	void PutXItems16(
		const xitems16_ &XItems16,
		flw::oflow__ &OFlow );

	//f Get 'XItems16' from 'IFlow'.
	void GetXItems16(
		flw::iflow__ &IFlow,
		xitems16_ &XItems16 );

	BKDTPM_D( XItems16, xitems16 )

	//f Put 'XItems32' into 'OFlow'.
	void PutXItems32(
		const xitems32_ &XItems32,
		flw::oflow__ &OFlow );

	//f Get 'XItems32' from 'IFlow'.
	void GetXItems32(
		flw::iflow__ &IFlow,
		xitems32_ &XItems32 );

	BKDTPM_D( XItems32, xitems32 )

	//f Put 'CommandsItems' into 'OFlow'.
	void PutCommandsDetails(
		const commands_details_ &CommandsDetails,
		flw::oflow__ &OFlow );

	//f Get 'CommandsDetails' from 'IFlow'.
	void GetCommandsDetails(
		flw::iflow__ &IFlow,
		commands_details_ &CommandsDetails );

	BKDTPM_D( CommandsDetails, commands_details )

	//f Put 'ObjectsReferences' into 'OFlow'.
	void PutObjectsReferences(
		const objects_references_ &ObjectsReferences,
		flw::oflow__ &OFlow );

	//f Get 'ObjectsReferences' from 'IFlow'.
	void GetObjectsReferences(
		flw::iflow__ &IFlow,
		objects_references_ &ObjectsReferences );

	BKDTPM_D( ObjectsReferences, objects_references )
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
