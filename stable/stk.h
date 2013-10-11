/*
	Header for the 'stk' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Copyright (C) 2000-2004 Claude SIMON (http://zeusw.org/intl/contact.html).

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

//	$Id: stk.h,v 1.33 2013/04/15 10:50:45 csimon Exp $

#ifndef STK__INC
#define STK__INC

#define STK_NAME		"STK"

#define	STK_VERSION	"$Revision: 1.33 $"

#define STK_OWNER		"Claude SIMON (http://zeusw.org/intl/contact.html)"

#include "ttr.h"

extern class ttr_tutor &STKTutor;

#if defined( E_DEBUG ) && !defined( STK_NODBG )
#define STK_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.33 $
//C Claude SIMON (http://zeusw.org/intl/contact.html)
//R $Date: 2013/04/15 10:50:45 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D STacK 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "bch.h"
//#include "ctn.h"

namespace stk {

	//c Stack of static objects of type 't'. Use 'STACK_( t )' rather then directly this class.
	template <typename structure, typename item, typename row> class stack_
	: public structure
	{
	public:
		struct s
		: public structure::s
		{};
		stack_( s &S )
		: structure( S )
		{}
		void reset( bool P = true )
		{
			structure::reset( P );
		}
		void plug( sdr::E_SDRIVER__ &SD )
		{
			structure::plug( SD );
		}
		void plug( ags::E_ASTORAGE_ &AS )
		{
			structure::plug( AS );
		}
		stack_ &operator =( const stack_ &S )
		{
			structure::operator =( S );

			return *this;
		}
		//f Initialization.
		void Init( void )
		{
			structure::Init();
			structure::SetNoDecreasingState( true );
		}
		//f Place 'Object' at the top of the stack. Return the position where this object is put.
		row Push( const item &Object )
		{
			return structure::Append( Object );
		}
		//f Remove the object at row 'R'. If 'Adjust' at 'true', than adjust the size of the stack.
		void Remove(
			row Row,
			aem::mode__ Mode = aem::m_Default )
		{
			structure::Remove( Row );
		}
		row Pop( void )
		{
			row Row = structure::Last();

			Remove( Row );

			return Row;
		}
		row Pop( item &Item )
		{
			structure::Recall( structure::Last(), Item );

			return Pop();
		}
		//f Return 'true' if 'Object' exists in the stack, false otherwise.
		bso::bool__ Exists( const item &Object ) const
		{
			return structure::Search( Object, 0, structure::Amount() ) != E_NIL;
		}
		//f Return true if an entry exists for row 'Row'.
		bso::bool__ Exists( row Row ) const
		{
			return structure::Exists( Row );
		}
		//f Return the value stord on top of the stack.
		void Top( item &Item ) const
		{
#ifdef STK_DBG
			if ( structure::Amount() == 0 )
				ERRFwk();
#endif
			structure::Recall( structure::Last(), Item );
		}
	};

	E_AUTO3( stack )

	template <typename item, typename row> class bstack_
	: public stack_< bch::bunch_< item, row >, item, row >
	{
	public:
		struct s
		: public stack_< bch::bunch_< item, row >, item, row >::s
		{};
		bstack_( s &S )
		: stack_< bch::bunch_< item, row >, item, row >( S )
		{}
		row Pop( item &Item )
		{
			return stack_< bch::bunch_< item, row >, item, row >::Pop( Item );
		}
		item Pop( void )
		{
			item Item;

			Pop( Item );

			return Item;
		}
		void Top( item &Item ) const
		{
			stack_< bch::bunch_< item, row >, item, row >::Top( Item );
		}
		item Top( void ) const
		{
			item Item;

			Top( Item );

			return Item;
		}
	};

	E_AUTO2( bstack )

	//m A stack of static object of type 't'.
	#define E_BSTACKt_( item, row )	bstack_< item, row >
	#define E_BSTACKt( item, row )	bstack< item, row >

	E_ROW( row__ );

	#define E_BSTACK_( item )	E_BSTACKt_( item, stk::row__ )
	#define E_BSTACK( item )	E_BSTACKt( item, stk::row__ )

# ifdef XTN__EXTEND
	#define E_XCSTACKt_( item, row )	stack_< ctn::E_XCONTAINERt_( item, row ), item, row >
	#define E_XCSTACKt( item, row )	stack< ctn::E_XCONTAINERt_( item, row ), item, row >

	#define E_XCSTACK_( item )		E_XCSTACKt_( item, stk::row__ )
	#define E_XCSTACK( item )		E_XCSTACKt( item, stk::row__ )

	#define E_XMCSTACKt_( item, row )	stack_< ctn::E_XMCONTAINERt_( item, row ), item, row >
	#define E_XMCSTACKt( item, row )	stack< ctn::E_XMCONTAINERt_( item, row ), item, row >

	#define E_XMCSTACK_( item )		E_XMCSTACKt_( item, stk::row__ )
	#define E_XMCSTACK( item )		E_XMCSTACKt( item, stk::row__ )
# else
	#define E_CSTACKt_( item, row )	stack_< ctn::E_CONTAINERt_( item, row ), item, row >
	#define E_CSTACKt( item, row )	stack< ctn::E_CONTAINERt_( item, row ), item, row >

	#define E_CSTACK_( item )		E_CSTACKt_( item, stk::row__ )
	#define E_CSTACK( item )		E_CSTACKt( item, stk::row__ )

	#define E_MCSTACKt_( item, row )	stack_< ctn::E_MCONTAINERt_( item, row ), item, row >
	#define E_MCSTACKt( item, row )	stack< ctn::E_MCONTAINERt_( item, row ), item, row >

	#define E_MCSTACK_( item )		E_MCSTACKt_( item, stk::row__ )
	#define E_MCSTACK( item )		E_MCSTACKt( item, stk::row__ )
# endif
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
