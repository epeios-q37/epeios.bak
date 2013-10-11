/*
	'fbltyp' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'fbltyp' header file ('fbltyp.h').
	Copyright (C) 2004 Claude SIMON.

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



//	$Id: fbltyp.cpp,v 1.17 2013/04/15 10:50:50 csimon Exp $

#define FBLTYP__COMPILATION

#include "fbltyp.h"

class fbltyptutor
: public ttr_tutor
{
public:
	fbltyptutor( void )
	: ttr_tutor( FBLTYP_NAME )
	{
#ifdef FBLTYP_DBG
		Version = FBLTYP_VERSION "\b\bD $";
#else
		Version = FBLTYP_VERSION;
#endif
		Owner = FBLTYP_OWNER;
		Date = "$Date: 2013/04/15 10:50:50 $";
	}
	virtual ~fbltyptutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace fbltyp;

static inline void PutSize_(
	bso::size__ Size,
	flw::oflow__ &Flow )
{
	dtfptb::VPut( Size, Flow );
}

static inline bso::size__ GetSize_( flw::iflow__ &Flow )
{
	bso::size__ Size;

	return dtfptb::VGet( Flow, Size );
}

/*


static inline void _Put(
	const str::string_ &Value,
	flw::oflow__ &Flow )
{
	Put( Value, Flow );
}

static inline void _Get(
	flw::iflow__ &Flow,
	str::string_ &Value )
{
	Get( Flow, Value );
}
*/


static inline void _Put(
	object_reference__ Value,
	flw::oflow__ &Flow )
{
	Put( Value, Flow );
}

static inline void _Get(
	flw::iflow__ &Flow,
	object_reference__ &Value )
{
	Get( Flow, Value );
}

static inline void _Put(
	const bso::raw__ Value,
	flw::oflow__ &Flow )
{
	Put( Value, Flow );
}

static inline void _Get(
	flw::iflow__ &Flow,
	bso::raw__ &Value )
{
	Get( Flow, Value );
}


# define M( name, type )\
static inline void _Put(\
	type##__ Value,\
	flw::oflow__ &Flow )\
{\
	Put##name( Value, Flow );\
}\
static inline void _Get(\
	flw::iflow__ &Flow,\
	type##__ &Value )\
{\
	Get##name( Flow, Value );\
}

M( SInt, sint );
M( UInt, uint );
M( Id8, id8 );
M( Id16, id16 );
M( Id32, id32 );
M( Id, id );
M( Boolean, boolean );
M( Object, object );

namespace {
	template <typename s> inline void PutSet_(
		const s &S,
		flw::oflow__ &OFlow )
	{
		sdr::row__ P = S.First();

		PutSize_( S.Amount(), OFlow );

		while( P != E_NIL ) {
			_Put( S( P ), OFlow );
			P = S.Next( P );
		}
	}

	template <typename s, typename i> inline void GetSet_(
		flw::iflow__ &IFlow,
		s &S )
	{
		i I;
		bso::size__ Size = GetSize_( IFlow );

		while( Size-- ) {
			_Get( IFlow, I );
			S.Append( I );
		}
	}
}

void fbltyp::PutSInts(
	const sints_ &SInts,
	flw::oflow__ &OFlow )
{
	PutSet_( SInts, OFlow );
}

void fbltyp::GetSInts(
	flw::iflow__ &IFlow,
	sints_ &SInts )
{
	GetSet_<sints_, sint__>( IFlow, SInts );
}
void fbltyp::PutUInts(
	const uints_ &UInts,
	flw::oflow__ &OFlow )
{
	PutSet_( UInts, OFlow );
}

void fbltyp::GetUInts(
	flw::iflow__ &IFlow,
	uints_ &UInts )
{
	GetSet_<uints_, uint__>( IFlow, UInts );
}

void fbltyp::PutId8s(
	const id8s_ &Id8s,
	flw::oflow__ &OFlow )
{
	PutSet_( Id8s, OFlow );
}

void fbltyp::GetId8s(
	flw::iflow__ &IFlow,
	id8s_ &Id8s )
{
	GetSet_<id8s_, id8__>( IFlow, Id8s );
}

void fbltyp::PutId16s(
	const id16s_ &Id16s,
	flw::oflow__ &OFlow )
{
	PutSet_( Id16s, OFlow );
}

void fbltyp::GetId16s(
	flw::iflow__ &IFlow,
	id16s_ &Id16s )
{
	GetSet_<id16s_, id16__>( IFlow, Id16s );
}

namespace {
	template <typename t> inline void Put_(
		const _item_<t> &I,
		flw::oflow__ &OFlow )
	{
		_Put( I.ID(), OFlow );
		OFlow << I.Value;
	}

	template <typename t> inline void Get_(
		flw::iflow__ &IFlow,
		_item_<t> &I )
	{
		_Get( IFlow, I.S_.ID );
		IFlow >> I.Value;
	}

	inline void Put_(
		const str::string_ &String,
		flw::oflow__ &OFlow )
	{
		PutString( String, OFlow );
	}

	inline void Get_(
		flw::iflow__ &IFlow,
		string_ &String )
	{
		GetString( IFlow, String );
	}

	inline void Put_(
		const binary_ &Binary,
		flw::oflow__ &OFlow )
	{
		PutBinary( Binary, OFlow );
	}

	inline void Get_(
		flw::iflow__ &IFlow,
		binary_ &Binary )
	{
		GetBinary( IFlow, Binary );
	}

	inline void Put_(
		const id8s_ &Id8s,
		flw::oflow__ &OFlow )
	{
		PutSet_( Id8s, OFlow );
	}

	inline void Get_(
		flw::iflow__ &IFlow,
		id8s_ &Id8s )
	{
		GetSet_<id8s_, id8__>( IFlow, Id8s );
	}

	inline void Put_(
		const id16s_ &Id16s,
		flw::oflow__ &OFlow )
	{
		PutSet_( Id16s, OFlow );
	}

	inline void Get_(
		flw::iflow__ &IFlow,
		id16s_ &Id16s )
	{
		GetSet_<id16s_, id16__>( IFlow, Id16s );
	}

	inline void Put_(
		const id32s_ &Id32s,
		flw::oflow__ &OFlow )
	{
		PutSet_( Id32s, OFlow );
	}

	inline void Get_(
		flw::iflow__ &IFlow,
		id32s_ &Id32s )
	{
		GetSet_<id32s_, id32__>( IFlow, Id32s );
	}

	inline void Put_(
		const ids_ &Ids,
		flw::oflow__ &OFlow )
	{
		PutSet_( Ids, OFlow );
	}

	inline void Get_(
		flw::iflow__ &IFlow,
		ids_ &Ids )
	{
		GetSet_<ids_, id__>( IFlow, Ids );
	}

	inline void Put_(
		const command_detail_ &CD,
		flw::oflow__ &OFlow )
	{
		OFlow << CD.Name;
		PutSet_( CD.Casts, OFlow );
	}

	inline void Get_(
		flw::iflow__ &IFlow,
		command_detail_ &CD )
	{
		IFlow >> CD.Name;
		GetSet_<bch::E_BUNCH_( id8_t__ ), id8_t__>( IFlow, CD.Casts );
	}

	template <typename t> inline void Put_(
		const _extended_item_<t> &I,
		flw::oflow__ &OFlow );

	void Put_(
		const strings_ &Strings,
		flw::oflow__ &OFlow );	// Predeclaration.

	template <typename c, typename i> inline void GenericPutContainer_(
		const c &C,
		flw::oflow__ &OFlow )
	{
		i Item;
		sdr::row__ P = C.First();

		Item.Init( C );

		PutSize_( C.Amount(), OFlow );

		while ( P != E_NIL ) {
			Put_( Item( P ), OFlow );
			P = C.Next( P );
		}
	}

	template <typename c, typename i> inline void PutContainer_(
		const c &C,
		flw::oflow__ &OFlow )
	{
		GenericPutContainer_<c, ctn::E_CITEM( i )>( C, OFlow );
	}

	template <typename c, typename i> inline void PutMContainer_(
		const c &C,
		flw::oflow__ &OFlow )
	{
		GenericPutContainer_<c, ctn::E_CMITEM( i )>( C, OFlow );
	}

	template <typename t> inline void Put_(
		const _extended_item_<t> &I,
		flw::oflow__ &OFlow )
	{
		_Put( I.ID(), OFlow );
		PutMContainer_<strings_, string_>( I.Values, OFlow );
	}

	void Get_(
		flw::iflow__ &IFlow,
		strings_ &Strings );	// Predeclaration.

	template <typename t> inline void Get_(
		flw::iflow__ &IFlow,
		_extended_item_<t> &I );	// Predeclaration.

	template <typename c, typename i> inline void GetContainer_(
		flw::iflow__ &IFlow,
		c &C )
	{
		i Item;
		bso::size__ Size = GetSize_( IFlow );

		while( Size-- ) {
			Item.Init();

			Get_( IFlow, Item );
			C.Append( Item );
		}
	}

	template <typename t> inline void Get_(
		flw::iflow__ &IFlow,
		_extended_item_<t> &I )
	{
		_Get( IFlow, I.S_.ID );
		GetContainer_<strings_, string>( IFlow, I.Values );
	}

	inline void Put_(
		const strings_ &Strings,
		flw::oflow__ &OFlow )
	{
		PutMContainer_<strings_, string_>( Strings, OFlow );
	}

	inline void Get_(
		flw::iflow__ &IFlow,
		strings_ &Strings )
	{
		GetContainer_<strings_, string>( IFlow, Strings );
	}

}

void fbltyp::PutXId8s(
	const xid8s_ &XId8s,
	flw::oflow__ &OFlow )
{
	PutMContainer_<xid8s_, id8s_>( XId8s, OFlow );
}

void fbltyp::GetXId8s(
	flw::iflow__ &IFlow,
	xid8s_ &XId8s )
{
	GetContainer_<xid8s_, id8s>( IFlow, XId8s );
}

void fbltyp::PutXId16s(
	const xid16s_ &XId16s,
	flw::oflow__ &OFlow )
{
	PutMContainer_<xid16s_, id16s_>( XId16s, OFlow );
}

void fbltyp::GetXId16s(
	flw::iflow__ &IFlow,
	xid16s_ &XId16s )
{
	GetContainer_<xid16s_, id16s>( IFlow, XId16s );
}

void fbltyp::PutBooleans(
	const booleans_ &Booleans,
	flw::oflow__ &OFlow )	// To optimize.
{
	PutSet_( Booleans, OFlow );
}

void fbltyp::GetBooleans(
	flw::iflow__ &IFlow,
	booleans_ &Booleans ) // To optimize.
{
	GetSet_<booleans_, bso::bool__>( IFlow, Booleans );
}

void fbltyp::PutId32s(
	const id32s_ &Id32s,
	flw::oflow__ &OFlow )
{
	Put_( Id32s, OFlow );
}

void fbltyp::GetId32s(
	flw::iflow__ &IFlow,
	id32s_ &Id32s )
{
	Get_( IFlow, Id32s );
}
void fbltyp::PutXId32s(
	const xid32s_ &XId32s,
	flw::oflow__ &OFlow )
{
	PutMContainer_<xid32s_, id32s_>( XId32s, OFlow );
}

void fbltyp::GetXId32s(
	flw::iflow__ &IFlow,
	xid32s_ &XId32s )
{
	GetContainer_<xid32s_, id32s>( IFlow, XId32s );
}

void fbltyp::PutIds(
	const ids_ &Ids,
	flw::oflow__ &OFlow )
{
	Put_( Ids, OFlow );
}

void fbltyp::GetIds(
	flw::iflow__ &IFlow,
	ids_ &Ids )
{
	Get_( IFlow, Ids );
}
void fbltyp::PutXIds(
	const xids_ &XIds,
	flw::oflow__ &OFlow )
{
	PutMContainer_<xids_, ids_>( XIds, OFlow );
}

void fbltyp::GetXIds(
	flw::iflow__ &IFlow,
	xids_ &XIds )
{
	GetContainer_<xids_, ids>( IFlow, XIds );
}

void fbltyp::PutStrings(
	const strings_ &Strings,
	flw::oflow__ &OFlow )
{
	Put_( Strings, OFlow );
}

void fbltyp::GetStrings(
   flw::iflow__ &IFlow,
   strings_ &Strings )
{
	Get_( IFlow, Strings );
}

void fbltyp::PutXStrings(
	const xstrings_ &XStrings,
	flw::oflow__ &OFlow )
{
	PutContainer_<xstrings_, strings_>( XStrings, OFlow );
}

void fbltyp::GetXStrings(
   flw::iflow__ &IFlow,
   xstrings_ &XStrings )
{
	GetContainer_<xstrings_, strings>( IFlow, XStrings );
}

void fbltyp::PutBinary(
	const binary_ &Binary,
	flw::oflow__ &OFlow )
{
	PutSet_( Binary, OFlow );
}

void fbltyp::GetBinary(
   flw::iflow__ &IFlow,
   binary_ &Binary )
{
	GetSet_<binary_, bso::raw__>( IFlow, Binary );
}

void fbltyp::PutBinaries(
	const binaries_ &Binaries,
	flw::oflow__ &OFlow )
{
	PutMContainer_<binaries_, binary_>( Binaries, OFlow );
}

void fbltyp::GetBinaries(
	flw::iflow__ &IFlow,
	binaries_ &Binaries )
{
	GetContainer_<binaries_, binary>( IFlow, Binaries );
}

void fbltyp::PutItem8(
	const item8_ Item8,
	flw::oflow__ &OFlow )
{
	Put_( Item8, OFlow );
}

void fbltyp::GetItem8(
   flw::iflow__ &IFlow,
   item8_ &Item8 )
{
	Get_( IFlow, Item8 );
}

void fbltyp::PutItem8s(
	const item8s_ &Item8s,
	flw::oflow__ &OFlow )
{
	PutMContainer_<item8s_, item8_>( Item8s, OFlow );
}

void fbltyp::GetItem8s(
   flw::iflow__ &IFlow,
   item8s_ &Item8s )
{
	GetContainer_<item8s_, item8>( IFlow, Item8s );
}

void fbltyp::PutItem16(
	const item16_ Item16,
	flw::oflow__ &OFlow )
{
	Put_( Item16, OFlow );
}

void fbltyp::GetItem16(
   flw::iflow__ &IFlow,
   item16_ &Item16 )
{
	Get_( IFlow, Item16 );
}

void fbltyp::PutItem16s(
	const item16s_ &Item16s,
	flw::oflow__ &OFlow )
{
	PutMContainer_<item16s_, item16_>( Item16s, OFlow );
}

void fbltyp::GetItem16s(
   flw::iflow__ &IFlow,
   item16s_ &Item16s )
{
	GetContainer_<item16s_, item16>( IFlow, Item16s );
}

void fbltyp::PutItem32(
	const item32_ Item32,
	flw::oflow__ &OFlow )
{
	Put_( Item32, OFlow );
}

void fbltyp::GetItem32(
   flw::iflow__ &IFlow,
   item32_ &Item32 )
{
	Get_( IFlow, Item32 );
}

void fbltyp::PutItem32s(
	const item32s_ &Item32s,
	flw::oflow__ &OFlow )
{
	PutMContainer_<item32s_, item32_>( Item32s, OFlow );
}

void fbltyp::GetItem32s(
   flw::iflow__ &IFlow,
   item32s_ &Item32s )
{
	GetContainer_<item32s_, item32>( IFlow, Item32s );
}

void fbltyp::PutItem(
	const item_ Item,
	flw::oflow__ &OFlow )
{
	Put_( Item, OFlow );
}

void fbltyp::GetItem(
   flw::iflow__ &IFlow,
   item_ &Item )
{
	Get_( IFlow, Item );
}

void fbltyp::PutItems(
	const items_ &Items,
	flw::oflow__ &OFlow )
{
	PutMContainer_<items_, item_>( Items, OFlow );
}

void fbltyp::GetItems(
   flw::iflow__ &IFlow,
   items_ &Items )
{
	GetContainer_<items_, item>( IFlow, Items );
}


void fbltyp::PutXItem8s(
	const xitem8s_ &XItem8s,
	flw::oflow__ &OFlow )
{
	PutContainer_<xitem8s_, xitem8_>( XItem8s, OFlow );
}

void fbltyp::GetXItem8s(
   flw::iflow__ &IFlow,
   xitem8s_ &XItem8s )
{
	GetContainer_<xitem8s_, xitem8>( IFlow, XItem8s );
}

void fbltyp::PutXItem16s(
	const xitem16s_ &XItem16s,
	flw::oflow__ &OFlow )
{
	PutContainer_<xitem16s_, xitem16_>( XItem16s, OFlow );
}

void fbltyp::GetXItem16s(
   flw::iflow__ &IFlow,
   xitem16s_ &XItem16s )
{
	GetContainer_<xitem16s_, xitem16>( IFlow, XItem16s );
}

void fbltyp::PutXItem32s(
	const xitem32s_ &XItem32s,
	flw::oflow__ &OFlow )
{
	PutContainer_<xitem32s_, xitem32_>( XItem32s, OFlow );
}

void fbltyp::GetXItem32s(
   flw::iflow__ &IFlow,
   xitem32s_ &XItem32s )
{
	GetContainer_<xitem32s_, xitem32>( IFlow, XItem32s );
}

void fbltyp::PutXItems(
	const xitems_ &XItems,
	flw::oflow__ &OFlow )
{
	PutContainer_<xitems_, xitem_>( XItems, OFlow );
}

void fbltyp::GetXItems(
   flw::iflow__ &IFlow,
   xitems_ &XItems )
{
	GetContainer_<xitems_, xitem>( IFlow, XItems );
}

void fbltyp::PutObjectsReferences(
	const objects_references_ &ObjectsReferences,
	flw::oflow__ &OFlow )
{
	PutSet_( ObjectsReferences, OFlow );
}

void fbltyp::GetObjectsReferences(
	flw::iflow__ &IFlow,
	objects_references_ &ObjectsReferences )
{
	GetSet_<objects_references_, object_reference__>( IFlow, ObjectsReferences );
}

void fbltyp::PutCommandsDetails(
	const fbltyp::commands_details_ &CommandsDetails,
	flw::oflow__ &OFlow )
{
	PutContainer_<commands_details_, command_detail_ >( CommandsDetails, OFlow );
}

void fbltyp::GetCommandsDetails(
	flw::iflow__ &IFlow,
	fbltyp::commands_details_ &CommandsDetails )
{
	GetContainer_<commands_details_, command_detail>( IFlow, CommandsDetails );
}


/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class fbltyppersonnalization
: public fbltyptutor
{
public:
	fbltyppersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~fbltyppersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the ending of the application  */
	}
};


/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

// 'static' by GNU C++.

static fbltyppersonnalization Tutor;

ttr_tutor &FBLTYPTutor = Tutor;
