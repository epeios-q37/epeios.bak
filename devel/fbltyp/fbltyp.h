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

//D Frontend/Backend Layout TYPes

#ifndef FBLTYP_INC_
# define FBLTYP_INC_

# define FBLTYP_NAME		"FBLTYP"

# if defined( E_DEBUG ) && !defined( FBLTYP_NODBG )
#  define FBLTYP_DBG
# endif

#include "err.h"
#include "flw.h"
#include "str.h"
#include "bitbch.h"

# ifdef M
#  define FBLTYP_M_	M
# endif

/***************/
/***** New *****/
/***************/

// For simplification, some 'New' are at end of file.

/***************/
/***** Old *****/
/***************/

#ifdef CPE_C_MSC
#	undef GetObject
#endif

#define FBLTYP_UNDEFINED_ID8	((fbltyp::id8__)BSO_U8_MAX)
#define FBLTYP_UNDEFINED_ID16	((fbltyp::id16__)BSO_U16_MAX)
#define FBLTYP_UNDEFINED_ID32	((fbltyp::id32__)BSO_U32_MAX)
#define FBLTYP_UNDEFINED_ID64	((fbltyp::id64__)BSO_U64_MAX)
#define FBLTYP_UNDEFINED_ID		((fbltyp::id__)BSO_UINT_MAX)

#define FBLTYP_ITEM( type, name )\
	typedef fbltyp::_item_<type##__>	name##_;\
	E_AUTO( name )\
	typedef fbltyp::_item_<type##_t__>	name##_t_;\
	E_AUTO( name##_t )

#define FBLTYP_ITEMS( name )\
	typedef ctn::E_MCONTAINER_( name##_ )	name##s_;\
	E_AUTO( name##s )\
	typedef ctn::E_MCONTAINER_( name##_t_ )	name##s_t_;\
	E_AUTO( name##s_t )

#define FBLTYP_XITEM( type, name )\
	typedef fbltyp::_extended_item_<type##__>	name##_;\
	E_AUTO( name )\
	typedef fbltyp::_extended_item_<type##_t__>	name##_t_;\
	E_AUTO( name##_t )

#define FBLTYP_XITEMS( name )\
	typedef ctn::E_CONTAINER_( name##_ )	name##s_;\
	E_AUTO( name##s )\
	typedef ctn::E_CONTAINER_( name##_t_ )	name##s_t_;\
	E_AUTO( name##s_t )

# define FBLTYP_MIMIC( stype, sname )	E_TMIMIC__( stype##__, sname##__ )

# define FBLTYP_MIMICS( stype, btype, sname, bname )\
	FBLTYP_MIMIC( stype, sname );\
	class bname##_\
	: public bch::E_BUNCH_( sname##__ )\
	{\
	public:\
		struct s\
		: public bch::E_BUNCH_( sname##__ )::s\
		{};\
		bname##_( s &S )\
		: bch::E_BUNCH_( sname##__ )( S )\
		{}\
		btype##_ &operator *( void )\
		{\
			return *(btype##_ *)this;\
		}\
		const btype##_ &operator *( void ) const\
		{\
			return *(const btype##_ *)this;\
		}\
	};\
	E_AUTO( bname )

# define FBLTYP_MIMICX( stype, btype, xtype, sname, bname, xname )\
	FBLTYP_MIMICS( stype, btype, sname, bname );\
	E_TMIMIC( xtype, xname );

# define FBLTYP_MIMIC_ID8( sname ) FBLTYP_MIMIC( fbltyp::id8, sname )
# define FBLTYP_MIMIC_ID16( sname ) FBLTYP_MIMIC( fbltyp::id16, sname )
# define FBLTYP_MIMIC_ID32( sname ) FBLTYP_MIMIC( fbltyp::id32, sname )
# define FBLTYP_MIMIC_ID64( sname ) FBLTYP_MIMIC( fbltyp::id64, sname )
# define FBLTYP_MIMIC_ID( sname ) FBLTYP_MIMIC( fbltyp::id, sname )

# define FBLTYP_MIMIC_ID8S( sname, bname ) FBLTYP_MIMICS( fbltyp::id8, fbltyp::id8s, sname, bname )
# define FBLTYP_MIMIC_ID16S( sname, bname ) FBLTYP_MIMICS( fbltyp::id16, fbltyp::id16s, sname, bname )
# define FBLTYP_MIMIC_ID32S( sname, bname ) FBLTYP_MIMICS( fbltyp::id32, fbltyp::id32s, sname, bname )
# define FBLTYP_MIMIC_ID64S( sname, bname ) FBLTYP_MIMICS( fbltyp::id64, fbltyp::id64s, sname, bname )
# define FBLTYP_MIMIC_IDS( sname, bname ) FBLTYP_MIMICS( fbltyp::id, fbltyp::ids, sname, bname )

# define FBLTYP_MIMIC_XID8S( sname, bname, xname ) FBLTYP_MIMICX( fbltyp::id8, fbltyp::id8S, fbltyp::xid8s, sname, bname, xname )
# define FBLTYP_MIMIC_XID16S( sname, bname, xname ) FBLTYP_MIMICX( fbltyp::id16, fbltyp::id16s, fbltyp::xid16s, sname, bname, xname )
# define FBLTYP_MIMIC_XID32S( sname, bname, xname ) FBLTYP_MIMICX( fbltyp::id32, fbltyp::id32s, fbltyp::xid32s, sname, bname, xname )
# define FBLTYP_MIMIC_XID64S( sname, bname, xname ) FBLTYP_MIMICX( fbltyp::id64, fbltyp::id64s, fbltyp::xid64s, sname, bname, xname )
# define FBLTYP_MIMIC_XIDS( sname, bname, xname ) FBLTYP_MIMICX( fbltyp::id, fbltyp::ids, fbltyp::xids, sname, bname, xname )

# define FBLTYP_MIMIC_INT( sname ) FBLTYP_MIMIC( fbltyp::int, sname )
# define FBLTYP_MIMIC_INTS( sname, bname ) FBLTYP_MIMICS( fbltyp::int, fbltyp::ints, sname, bname )

# define FBLTYP_MIMIC_BIG( sname ) FBLTYP_MIMIC( fbltyp::big, sname )
# define FBLTYP_MIMIC_BIGS( sname, bname ) FBLTYP_MIMICS( fbltyp::big, fbltyp::bigs, sname, bname )

namespace fbltyp {
	//t Value.
	typedef str::dString value_;
	typedef str::wString value;

	template <class id> class _item_
	{
	public:
		struct s
		{
			value_::s Value;
			id ID;
		} &S_;
		value_ Value;
		_item_( s &S )
		: S_( S ),
		  Value( S.Value )
		{}
		void reset( bool P = true )
		{
			Value.reset( P );
		}
		void plug( str::sHook &Hook )
		{
			Value.plug( Hook );
		}
		void plug( qASd *AS )
		{
			Value.plug( AS );
		}
		_item_ &operator =( const _item_ &I )
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
	typedef str::dStrings values_;
	typedef str::wStrings values;

	template <class id> class _extended_item_
	{
	public:
		struct s
		{
			values_::s Values;
			id ID;
		} &S_;
		values_ Values;
		_extended_item_( s &S )
		: S_( S ),
		  Values( S.Values )
		{}
		void reset( bool P = true )
		{
			Values.reset( P );
		}
		void plug( qASd *AS )
		{
			Values.plug( AS );
		}
		_extended_item_ &operator =( const _extended_item_ &I )
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
		void plug( qASd *AS )
		{
			Name.plug( AS );
			Casts.plug( AS );
		}
		detail_ &operator =( const detail_ &D )
		{
			Name = D.Name;
			Casts = D.Casts;

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
		bso::u16__ Type;
		bso::u16__ Identifier;
	};

	typedef bso::u16__	object_t__;
	E_TMIMIC__( object_t__, object__ );
	qCDEF( object__, UndefinedObject, BSO_U16_MAX );

	typedef bso::bool__ boolean_t__;
	typedef boolean_t__ boolean__;
	typedef bitbch::E_BIT_BUNCH_ booleans_;
	E_AUTO( booleans );
	typedef bitbch::E_BIT_BUNCH_ booleans_t_;
	E_AUTO( booleans_t );

# define FBLTYP_S( type )\
	typedef bch::E_BUNCH_( type##__ ) type##s_;\
	E_AUTO( type##s )\
	typedef bch::E_BUNCH_( type##_t__ ) type##s_t_;\
	E_AUTO( type##s_t )

# define FBLTYP_X( type )\
	FBLTYP_S( type )\
	typedef ctn::E_MCONTAINER_( type##s_ ) x##type##s_;\
	E_AUTO( x##type##s )\
	typedef ctn::E_MCONTAINER_( type##s_t_ ) x##type##s_t_;\
	E_AUTO( x##type##s_t )



	typedef bso::sint__ sint_t__;
	typedef sint_t__ sint__;
	FBLTYP_S( sint )

	typedef bso::uint__ uint_t__;
	typedef uint_t__ uint__;
	FBLTYP_S( uint )

	typedef bso::sHuge sbig_t__;
	typedef sbig_t__ sbig__;
	FBLTYP_S( sbig )

	typedef bso::sUHuge ubig_t__;
	typedef ubig_t__ ubig__;
	FBLTYP_S( ubig )

	typedef bso::u8__ id8_t__;
	E_TMIMIC__(id8_t__, id8__ );
	FBLTYP_X( id8 );

	typedef bso::u16__ id16_t__;
	E_TMIMIC__( id16_t__, id16__ );
	FBLTYP_X( id16 );

	typedef bso::u32__ id32_t__;
	E_TMIMIC__( id32_t__, id32__ );
	FBLTYP_X( id32 );

	typedef bso::u64__ id64_t__;
	E_TMIMIC__( id64_t__, id64__ );
	FBLTYP_X( id64 );

	typedef bso::uint__ id_t__;
	E_TMIMIC__( id_t__, id__ );
	FBLTYP_X( id );

	typedef bso::char__ char_t__;
	typedef char_t__	char__;
	typedef str::string_	string_;
	typedef str::string		string;
	typedef str::dStrings	strings_;
	typedef str::wStrings	strings;
	typedef ctn::E_CONTAINER_( strings_ )	xstrings_;
	typedef ctn::E_CONTAINER( strings_ )	xstrings;
	typedef bso::byte__	byte_t__;
	typedef byte_t__	byte__;
	typedef bch::E_BUNCH_( bso::byte__ ) binary_;
	typedef bch::E_BUNCH( bso::byte__ )	binary;
	typedef ctn::E_MCONTAINER_( binary_ ) binaries_;
	typedef ctn::E_MCONTAINER( binary_ ) binaries;
	FBLTYP_ITEM( id8, item8 )
	FBLTYP_ITEMS( item8 )
	FBLTYP_ITEM( id16, item16 )
	FBLTYP_ITEMS( item16 )
	FBLTYP_ITEM( id32, item32 )
	FBLTYP_ITEMS( item32 )
	FBLTYP_ITEM( id64, item64 )
	FBLTYP_ITEMS( item64 )
	FBLTYP_ITEM( id, item )
	FBLTYP_ITEMS( item )
	FBLTYP_XITEM( id8, xitem8 )
	FBLTYP_XITEMS( xitem8 )
	FBLTYP_XITEM( id16, xitem16 )
	FBLTYP_XITEMS( xitem16 )
	FBLTYP_XITEM( id32, xitem32 )
	FBLTYP_XITEMS( xitem32 )
	FBLTYP_XITEM( id64, xitem64 )
	FBLTYP_XITEMS( xitem64 )
	FBLTYP_XITEM( id, xitem )
	FBLTYP_XITEMS( xitem )
	/* Both below declaration are not part of the protocol. */
	typedef fbltyp::detail_<id8__, sdr::row__>	command_detail_	;
	typedef fbltyp::detail<id8__, sdr::row__>	command_detail;
	/**/
	typedef ctn::E_CONTAINER_( command_detail_ ) commands_details_;
	typedef ctn::E_CONTAINER( command_detail_ ) commands_details;
	typedef bch::E_BUNCH_( object_reference__ ) objects_references_;
	typedef bch::E_BUNCH( object_reference__ ) objects_references;
	typedef flw::iflow__ flow__;

	E_CDEF( id8__, UndefinedId8, FBLTYP_UNDEFINED_ID8 );
	E_CDEF( id__, UndefinedId, FBLTYP_UNDEFINED_ID );


	template <typename type> inline void _StraightPut(
		type Value,
		flw::oflow__ &Flow )
	{
		dtfptb::VPut( Value, Flow );
	}

	template <typename type> inline void _StraightGet(
		flw::iflow__ &Flow,
		type &Value )
	{
		dtfptb::VGet( Flow, Value );
	}

	template <typename type> inline void _StraightDPut(
		type Value,
		flw::oflow__ &Flow )
	{
		_StraightPut( *Value, Flow );
	}

	template <typename type> inline void _StraightDGet(
		flw::iflow__ &Flow,
		type &Value )
	{
		_StraightGet( Flow, *Value );
	}

	template <typename type> inline void _AdjustingPut(
		type Value,
		flw::oflow__ &Flow )
	{
		_StraightPut( (type)( Value + 1 ), Flow );	// 'qNIL ('-1') devient 0, donc ne prend qu'un seul octet.
	}

	template <typename type> inline void _AdjustingGet(
		flw::iflow__ &Flow,
		type &Value )
	{
		_StraightGet( Flow, Value );

		Value--;	// '0' devient 'qNIL' (voir '_AdjustingPut()').
	}

	template <typename type> inline void _AdjustingDGet(
		flw::iflow__ &Flow,
		type &Value )
	{
		_AdjustingGet( Flow, *Value );
	}

	template <typename type> void _AdjustingDPut(
		type Value,
		flw::oflow__ &Flow )
	{
		_AdjustingPut( *Value, Flow );
	}

	template <typename t> inline t *_New( void )
	{
		t *P = new t;

		if ( P == NULL )
			qRAlc();

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

# undef FBLTYP_S

#define FBLTYP_S( name, type )\
	inline type *New##name( void )\
	{\
		return _SNew<type>();\
	}\
	inline void Delete##name( type *P )\
	{\
		_SDelete( P );\
	}\

#define FBLTYP_D( name, type )\
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
		_StraightDPut( Object, OFlow );
	}

	//f Return 'Object' from 'IFlow'.
	inline void GetObject(
		flw::iflow__ &IFlow,
		object__ &Object )
	{
		_StraightDGet( IFlow, Object );
	}

	FBLTYP_S( Object, object__ )

	//f Put 'Boolean' into 'OFlow'.
	inline void PutBoolean(
		boolean__ Boolean,
		flw::oflow__ &OFlow )
	{
		dtfptb::FPut( Boolean, OFlow );
	}

	//f Return 'Boolean' from 'IFlow'.
	inline void GetBoolean(
		flw::iflow__ &IFlow,
		boolean__ &Boolean )
	{
		dtfptb::FGet( IFlow, *(bso::u8__ *)&Boolean );
	}

	FBLTYP_S( Boolean, boolean__ )

	//f Put 'Booleans' into 'OFlow'.
	void PutBooleans(
		const booleans_ &Booleans,
		flw::oflow__ &OFlow );

	//f Return 'Booleans' from 'IFlow'.
	void GetBooleans(
		flw::iflow__ &IFlow,
		booleans_ &Booleans );

	FBLTYP_D( Booleans, booleans )

/**/
	//f Put 'sInt' into 'Flow'.
	inline void PutSInt(
		sint__ Int,
		flw::oflow__ &OFlow )
	{
		_StraightPut( Int, OFlow );
	}

	//f Get 'sInt' from 'IFlow'.
	inline void GetSInt(
		flw::iflow__ &IFlow,
		sint__ &Int )
	{
		_StraightGet( IFlow, Int );
	}

	FBLTYP_S( SInt, sint__ )

	//f Put 'SInts' into 'Flow'.
	void PutSInts(
		const sints_ &Ints,
		flw::oflow__ &OFlow );

	//f Get 'SInts' from 'IFlow'.
	void GetSInts(
		flw::iflow__ &IFlow,
		sints_ &SInts );

	FBLTYP_D( SInts, sints )

	//f Put 'UInt' into 'Flow'.
	inline void PutUInt(
		uint__ Int,
		flw::oflow__ &OFlow )
	{
		_StraightPut( Int, OFlow );
	}

	//f Get 'UInt' from 'IFlow'.
	inline void GetUInt(
		flw::iflow__ &IFlow,
		uint__ &Int )
	{
		_StraightGet( IFlow, Int );
	}

	FBLTYP_S( UInt, uint__ )

	//f Put 'UInts' into 'Flow'.
	void PutUInts(
		const uints_ &UInts,
		flw::oflow__ &OFlow );

	//f Get 'UInts' from 'IFlow'.
	void GetUInts(
		flw::iflow__ &IFlow,
		uints_ &UInts );

	FBLTYP_D( UInts, uints )

/**/
/**/
	//f Put 'sBig' into 'Flow'.
	inline void PutSBig(
		sbig__ Big,
		flw::oflow__ &OFlow )
	{
		_StraightPut( Big, OFlow );
	}

	//f Get 'sBig' from 'IFlow'.
	inline void GetSBig(
		flw::iflow__ &IFlow,
		sbig__ &Big )
	{
		_StraightGet( IFlow, Big );
	}

	FBLTYP_S( SBig, sbig__ )

	//f Put 'SBigs' into 'Flow'.
	void PutSBigs(
		const sbigs_ &Bigs,
		flw::oflow__ &OFlow );

	//f Get 'SBigs' from 'IFlow'.
	void GetSBigs(
		flw::iflow__ &IFlow,
		sbigs_ &SBigs );

	FBLTYP_D( SBigs, sbigs )

	//f Put 'UBig' into 'Flow'.
	inline void PutUBig(
		ubig__ Big,
		flw::oflow__ &OFlow )
	{
		_StraightPut( Big, OFlow );
	}

	//f Get 'UBig' from 'IFlow'.
	inline void GetUBig(
		flw::iflow__ &IFlow,
		ubig__ &Big )
	{
		_StraightGet( IFlow, Big );
	}

	FBLTYP_S( UBig, ubig__ )

	//f Put 'UBigs' into 'Flow'.
	void PutUBigs(
		const ubigs_ &UBigs,
		flw::oflow__ &OFlow );

	//f Get 'UBigs' from 'IFlow'.
	void GetUBigs(
		flw::iflow__ &IFlow,
		ubigs_ &UBigs );

	FBLTYP_D( UBigs, ubigs )

/**/

	//f Put 'Id8' into 'OFlow'.
	inline void PutId8(
		id8__ Id8,
		flw::oflow__ &OFlow )
	{
		dtfptb::FPut( *Id8, OFlow );
	}

	//f Return 'Id8' from 'IFlow'.
	inline void GetId8(
		flw::iflow__ &IFlow,
		id8__ &Id8 )
	{
		dtfptb::FGet( IFlow, *Id8 );
	}

	FBLTYP_S( Id8, id8__ )

	//f Put 'Id8s' into 'OFlow'.
	void PutId8s(
		const id8s_ &Id8s,
		flw::oflow__ &OFlow );

	//f Get 'Id8s' from 'OFlow'.
	void GetId8s(
		flw::iflow__ &IFlow,
		id8s_ &Id8s );

	FBLTYP_D( Id8s, id8s )

	//f Put 'XId8s' into 'OFlow'.
	void PutXId8s(
		const xid8s_ &XId8s,
		flw::oflow__ &OFlow );

	//f Get 'XId8s' from 'OFlow'.
	void GetXId8s(
		flw::iflow__ &IFlow,
		xid8s_ &XId8s );

	FBLTYP_D( XId8s, xid8s )

	//f Put 'Id16' into 'OFlow'.
	inline void PutId16(
		id16__ Id16,
		flw::oflow__ &OFlow )
	{
		_AdjustingDPut( Id16, OFlow );
	}

	//f Return 'Id16' from 'IFlow'.
	inline void GetId16(
		flw::iflow__ &IFlow,
		id16__ &Id16 )
	{
		_AdjustingDGet( IFlow, Id16 );
	}

	FBLTYP_S( Id16, id16__ )

	//f Put 'Id16s' into 'OFlow'.
	void PutId16s(
		const id16s_ &Id16s,
		flw::oflow__ &OFlow );

	//f Get 'Id16s' from 'OFlow'.
	void GetId16s(
		flw::iflow__ &IFlow,
		id16s_ &Id16s );

	FBLTYP_D( Id16s, id16s )

	//f Put 'XId16s' into 'OFlow'.
	void PutXId16s(
		const xid16s_ &XId16s,
		flw::oflow__ &OFlow );

	//f Get 'XId16s' from 'OFlow'.
	void GetXId16s(
		flw::iflow__ &IFlow,
		xid16s_ &XId16s );

	FBLTYP_D( XId16s, xid16s )


	//f Put 'Id32' into 'OFlow'.
	inline void PutId32(
		id32__ Id32,
		flw::oflow__ &OFlow )
	{
		_AdjustingDPut( Id32, OFlow );
	}

	//f Return 'Id32' from 'IFlow'.
	inline void GetId32(
		flw::iflow__ &IFlow,
		id32__ &Id32 )
	{
		_AdjustingDGet( IFlow, Id32 );
	}

	FBLTYP_S( Id32, id32__ )

	//f Put 'Id32s' into 'OFlow'.
	void PutId32s(
		const id32s_ &Id32s,
		flw::oflow__ &OFlow );

	//f Get 'Id32s' from 'OFlow'.
	void GetId32s(
		flw::iflow__ &IFlow,
		id32s_ &Id32s );

	FBLTYP_D( Id32s, id32s )

	//f Put 'XId32s' into 'OFlow'.
	void PutXId32s(
		const xid32s_ &XId32s,
		flw::oflow__ &OFlow );

	//f Get 'XId32s' from 'OFlow'.
	void GetXId32s(
		flw::iflow__ &IFlow,
		xid32s_ &XId32s );

	FBLTYP_D( XId32s, xid32s )
	//f Put 'Id64' into 'OFlow'.
	inline void PutId64(
		id64__ Id64,
		flw::oflow__ &OFlow )
	{
		_AdjustingDPut( Id64, OFlow );
	}

	//f Return 'Id64' from 'IFlow'.
	inline void GetId64(
		flw::iflow__ &IFlow,
		id64__ &Id64 )
	{
		_AdjustingDGet( IFlow, Id64 );
	}

	FBLTYP_S( Id64, id64__ )

	//f Put 'Id64s' into 'OFlow'.
	void PutId64s(
		const id64s_ &Id64s,
		flw::oflow__ &OFlow );

	//f Get 'Id64s' from 'OFlow'.
	void GetId64s(
		flw::iflow__ &IFlow,
		id64s_ &Id64s );

	FBLTYP_D( Id64s, id64s )

	//f Put 'XId64s' into 'OFlow'.
	void PutXId64s(
		const xid64s_ &XId64s,
		flw::oflow__ &OFlow );

	//f Get 'XId64s' from 'OFlow'.
	void GetXId64s(
		flw::iflow__ &IFlow,
		xid64s_ &XId64s );

	FBLTYP_D( XId64s, xid64s )


	//f Put 'Id' into 'OFlow'.
	inline void PutId(
		id__ Id,
		flw::oflow__ &OFlow )
	{
		_AdjustingDPut( Id, OFlow );
	}

	//f Return 'Id' from 'IFlow'.
	inline void GetId(
		flw::iflow__ &IFlow,
		id__ &Id )
	{
		_AdjustingDGet( IFlow, Id );
	}

	FBLTYP_S( Id, id__ )

	//f Put 'Ids' into 'OFlow'.
	void PutIds(
		const ids_ &Ids,
		flw::oflow__ &OFlow );

	//f Get 'Ids' from 'OFlow'.
	void GetIds(
		flw::iflow__ &IFlow,
		ids_ &Ids );

	FBLTYP_D( Ids, ids )

	//f Put 'XIds' into 'OFlow'.
	void PutXIds(
		const xids_ &XIds,
		flw::oflow__ &OFlow );

	//f Get 'XIds' from 'OFlow'.
	void GetXIds(
		flw::iflow__ &IFlow,
		xids_ &XIds );

	FBLTYP_D( XIds, xids )

	//f Put 'Char' into 'OFlow'.
	inline void PutChar(
		char__ Char,
		flw::oflow__ &OFlow )
	{
		dtfptb::FPut( Char, OFlow );
	}

	//f Return 'Char' from 'IFlow'.
	inline void GetChar(
		flw::iflow__ &IFlow,
		char__ &Char )
	{
		dtfptb::FGet( IFlow, Char );
	}

	FBLTYP_S( Char, char__ )

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

	FBLTYP_D( String, string )

	//f Put 'Strings' into 'OFlow'.
	void PutStrings(
		const strings_ &Strings,
		flw::oflow__ &OFlow );

	//f Get 'Strings' from 'IFlow'.
	void GetStrings(
		flw::iflow__ &IFlow,
		strings_ &Strings );

	FBLTYP_D( Strings, strings )

	//f Put 'XStrings' into 'OFlow'.
	void PutXStrings(
		const xstrings_ &XStrings,
		flw::oflow__ &OFlow );

	//f Get 'XStrings' from 'IFlow'.
	void GetXStrings(
		flw::iflow__ &IFlow,
		xstrings_ &Strings );

	FBLTYP_D( XStrings, xstrings )

	//f Put 'Byte' into 'OFlow'.
	inline void PutByte(
		byte__ Byte,
		flw::oflow__ &OFlow )
	{
		dtfptb::FPut( Byte, OFlow );
	}

	//f Return 'Byte' from 'IFlow'.
	inline void GetByte(
		flw::iflow__ &IFlow,
		byte__ &Byte )
	{
		dtfptb::FGet( IFlow, Byte );
	}

	FBLTYP_S( Byte, byte__ )

	//f Put 'Binary' into 'OFlow'.
	void PutBinary(
		const binary_ &Binary,
		flw::oflow__ &OFlow );

	//f Get 'Binary' from 'IFlow'.
	void GetBinary(
		flw::iflow__ &IFlow,
		binary_ &Binary );

	FBLTYP_D( Binary, binary )

	//f Put 'Binaries' into 'OFlow'.
	void PutBinaries(
		const binaries_ &Binaries,
		flw::oflow__ &OFlow );

	//f Get 'Binaries' from 'IFlow'.
	void GetBinaries(
		flw::iflow__ &IFlow,
		binaries_ &Binaries );

	FBLTYP_D( Binaries, binaries )

	//f Put 'Item8' into 'OFlow'.
	void PutItem8(
		const item8_ Item8,
		flw::oflow__ &OFlow );

	//f Get 'Item8s' from 'IFlow'.
	void GetItem8(
	   flw::iflow__ &IFlow,
	   item8_ &Item8 );

	FBLTYP_D( Item8s, item8s )

	//f Put 'Item8s' into 'OFlow'.
	void PutItem8s(
		const item8s_ &Item8s,
		flw::oflow__ &OFlow );

	//f Get 'Item8s' from 'IFlow'.
	void GetItem8s(
		flw::iflow__ &IFlow,
		item8s_ &Item8s );

	FBLTYP_D( Item16s, item16s )

	//f Put 'Item16' into 'OFlow'.
	void PutItem16(
		const item16_ Item16,
		flw::oflow__ &OFlow );

	//f Get 'Item16' from 'IFlow'.
	void GetItem16(
	   flw::iflow__ &IFlow,
	   item16_ &Item16 );

	FBLTYP_D( Item16, item16 )

	//f Put 'Item16s' into 'OFlow'.
	void PutItem16s(
		const item16s_ &Item16s,
		flw::oflow__ &OFlow );

	//f Get 'Item16s' from 'IFlow'.
	void GetItem16s(
		flw::iflow__ &IFlow,
		item16s_ &Item16s );

	FBLTYP_D( item16s, item16s )


	//f Put 'Item32' into 'OFlow'.
	void PutItem32(
		const item32_ Item32,
		flw::oflow__ &OFlow );

	//f Get 'Item32' from 'IFlow'.
	void GetItem32(
	   flw::iflow__ &IFlow,
	   item32_ &Item32 );

	FBLTYP_D( Item32, item32s )

	//f Put 'Item32s' into 'OFlow'.
	void PutItem32s(
		const item32s_ &Item32s,
		flw::oflow__ &OFlow );

	//f Get 'Item32s' from 'IFlow'.
	void GetItem32s(
		flw::iflow__ &IFlow,
		item32s_ &Item32s );

	FBLTYP_D( Item32s, item32s )

	//f Put 'Item64' into 'OFlow'.
	void PutItem64(
		const item64_ Item64,
		flw::oflow__ &OFlow );

	//f Get 'Item64' from 'IFlow'.
	void GetItem64(
	   flw::iflow__ &IFlow,
	   item64_ &Item64 );

	FBLTYP_D( Item64, item64s )

	//f Put 'Item64s' into 'OFlow'.
	void PutItem64s(
		const item64s_ &Item64s,
		flw::oflow__ &OFlow );

	//f Get 'Item64s' from 'IFlow'.
	void GetItem64s(
		flw::iflow__ &IFlow,
		item64s_ &Item64s );

	FBLTYP_D( Item64s, item64s )

	//f Put 'Item' into 'OFlow'.
	void PutItem(
		const item_ Item,
		flw::oflow__ &OFlow );

	//f Get 'Item' from 'IFlow'.
	void GetItem(
	   flw::iflow__ &IFlow,
	   item_ &Item );

	FBLTYP_D( Item, items )

	//f Put 'Items' into 'OFlow'.
	void PutItems(
		const items_ &Items,
		flw::oflow__ &OFlow );

	//f Get 'Items' from 'IFlow'.
	void GetItems(
		flw::iflow__ &IFlow,
		items_ &Items );

	FBLTYP_D( Items, items )


	//f Put 'XItem8s' into 'OFlow'.
	void PutXItem8s(
		const xitem8s_ &XItem8s,
		flw::oflow__ &OFlow );

	//f Get 'XItems8' from 'IFlow'.
	void GetXItem8s(
		flw::iflow__ &IFlow,
		xitem8s_ &XItem8s );

	FBLTYP_D( XItem8s, xitem8s )

	//f Put 'XItems16' into 'OFlow'.
	void PutXItem16s(
		const xitem16s_ &XItem16s,
		flw::oflow__ &OFlow );

	//f Get 'XItem16s' from 'IFlow'.
	void GetXItem16s(
		flw::iflow__ &IFlow,
		xitem16s_ &XItem16s );

	FBLTYP_D( XItem16s, xitem16s )


		//f Put 'XItem32s' into 'OFlow'.
	void PutXItem32s(
		const xitem32s_ &XItem32s,
		flw::oflow__ &OFlow );

	//f Get 'XItem32s' from 'IFlow'.
	void GetXItem32s(
		flw::iflow__ &IFlow,
		xitem32s_ &XItem32s );

	FBLTYP_D( XItem32s, xitem32s )

	//f Put 'XItem64s' into 'OFlow'.
	void PutXItem64s(
		const xitem64s_ &XItem64s,
		flw::oflow__ &OFlow );

	//f Get 'XItem64s' from 'IFlow'.
	void GetXItem64s(
		flw::iflow__ &IFlow,
		xitem64s_ &XItem64s );

	FBLTYP_D( XItem64s, xitem64s )


		//f Put 'XItems' into 'OFlow'.
	void PutXItems(
		const xitems_ &XItems,
		flw::oflow__ &OFlow );

	//f Get 'XItems' from 'IFlow'.
	void GetXItems(
		flw::iflow__ &IFlow,
		xitems_ &XItems );

	FBLTYP_D( XItems, xitems )


	//f Put 'CommandsItems' into 'OFlow'.
	void PutCommandsDetails(
		const commands_details_ &CommandsDetails,
		flw::oflow__ &OFlow );

	//f Get 'CommandsDetails' from 'IFlow'.
	void GetCommandsDetails(
		flw::iflow__ &IFlow,
		commands_details_ &CommandsDetails );

	FBLTYP_D( CommandsDetails, commands_details )

	//f Put 'ObjectsReferences' into 'OFlow'.
	void PutObjectsReferences(
		const objects_references_ &ObjectsReferences,
		flw::oflow__ &OFlow );

	//f Get 'ObjectsReferences' from 'IFlow'.
	void GetObjectsReferences(
		flw::iflow__ &IFlow,
		objects_references_ &ObjectsReferences );

	FBLTYP_D( ObjectsReferences, objects_references )


}

/***************/
/***** New *****/
/***************/


namespace fbltyp {
	typedef string_	dString;
	typedef string wString;

	typedef strings_ dStrings;
	typedef strings wStrings;

	typedef xstrings_ dStringsSet;
	typedef xstrings wStringsSet;

# define M( id, Id )\
	typedef id##__	s##Id;\
	typedef id##s_	d##Id##s;\
	qW( Id##s )

	M( id, Id );
	M( id8, Id8 );
	M( id16, Id16 );
	M( id32, Id32 );
	M( id64, Id64 );
	M( uint, UInt );
	M( sint, SInt );
	M( ubig, UBig );
	M( sbig, SBig );
	M( boolean, Boolean );

# undef M

	template <typename bunch> inline void Convert(
		const bunch &Bunch,
		dIds &Ids )
	{
		sdr::sRow Row = Bunch.First();

		while( Row != qNIL ) {
			Ids.Add( *Bunch( Row ) );

			Row = Bunch.Next( Row );
		}
	}


	template <typename bunch> inline void Convert(
		const dIds &Ids,
		bunch &Bunch )
	{
		sdr::sRow Row = Ids.First();

		while( Row != qNIL ) {
			Bunch.Add( *Ids( Row ) );

			Row = Ids.Next( Row );
		}
	}

	typedef item_ dIString;
	qW( IString );

	typedef items_ dIStrings;
	qW( IStrings );

	typedef xitems_ dIXStrings;
	qW( IXStrings );

	typedef object__ sObject;
}


# ifdef FBLTYP_M_
#   define M FBLTYP_M_
# endif

#endif
