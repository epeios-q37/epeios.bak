/*
	Header for the 'ids' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Copyright (C) 2002, 2004 Claude SIMON (http://zeusw.org/intl/contact.html).

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

//	$Id: ids.h,v 1.25 2013/04/11 19:36:49 csimon Exp $

#ifndef IDS__INC
#define IDS__INC

#define IDS_NAME		"IDS"

#define	IDS_VERSION	"$Revision: 1.25 $"

#define IDS_OWNER		"Claude SIMON (http://zeusw.org/intl/contact.html)"


#if defined( E_DEBUG ) && !defined( IDS_NODBG )
#define IDS_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.25 $
//C Claude SIMON (http://zeusw.org/intl/contact.html)
//R $Date: 2013/04/11 19:36:49 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D ID Store 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"
#include "stk.h"
#include "ags.h"

namespace ids {

	//c Store of ids of type 'id__'. Use 'ID_STORE' rather than directly this class.
	template <typename id__> class ids_store_ 
	{
		// Return true if 'Id' available, false otherwise.
		bso::bool__ IsAvailable_( id__ ID ) const
		{
			return Released.Exists( ID ) || ( *ID >= *S_.FirstUnused );
		}
	public:
		struct s
		{
			typename stk::E_BSTACK_( id__ )::s Released;
			//r First unused ID.
			id__ FirstUnused;
		} &S_;
		//o Released IDs
		stk::E_BSTACK_( id__ ) Released;
		ids_store_( s &S )
		: S_( S ),
		  Released( S.Released )
		{}
		void reset( bso::bool__ P = true )
		{
			Released.reset( P );
			S_.FirstUnused = 0;
		}
		void plug( sdr::E_SDRIVER__ &SD )
		{
			Released.plug( SD );
		}
		void plug( ags::E_ASTORAGE_ &AS )
		{
			Released.plug( AS );
		}
		ids_store_ &operator =( const ids_store_ &IS )
		{
			Released = IS.Released;
			S_.FirstUnused = IS.S_.FirstUnused;

			return *this;
		}
		//f Initialization with 'First' as first unused.
		void Init( id__ FirstUnused = 0 )
		{
			S_.FirstUnused = FirstUnused;

			Released.Init();
		}
		void SetFirstUnused( id__ FirstUnused )
		{
			S_.FirstUnused = FirstUnused;
		}
		//f Return a new unused id. 'Released' is set at 'true' if the id is a released one.
		id__ New( bso::bool__ &Released )
		{
			id__ ID;

			if ( this->Released.Amount() ) {
				ID = this->Released.Pop();
				Released = true;
			} else {
				ID = (*S_.FirstUnused)++;

				if ( S_.FirstUnused == 0 )
					ERRLmt();

				Released = false;
			}

			return ID;
		}
		//f Return a new unused id.
		id__ New( void )
		{
			bso::bool__ Released;

			return New( Released );
		}
		//f Mark 'ID' as used. Return true if 'ID' out of range. For restoration purpose only.
		bso::bool__ New( id__ ID )
		{
			if ( *ID >= *S_.FirstUnused ) {
				S_.FirstUnused = *ID + 1;
				return true;
			} else {
#ifdef IDS_DBG
				if ( !IsAvailable_( ID ) )
					ERRPrm();
#endif
				Released.Remove( Released.Search( ID ) );

				return false;
			}
		}
		//f Released id 'ID'.
		void Release( id__ ID )
		{
#ifdef IDS_DBG
			if ( IsAvailable_( ID ) )
				ERRPrm();
#endif
			Released.Push( ID );
		}
		//f Modify the whole object so that 'ID' appears as available. For restoration purpose only.
		void RestorationRelease( id__ ID )
		{
			if ( *ID >= *S_.FirstUnused )
				S_.FirstUnused = *ID + 1;

			Released.Push( ID );
		}
		//f Return the first available id.
		id__ GetFirstAvailable( void ) const
		{
			return S_.FirstUnused;
		}
		//f Return the amount of released IDs.
		sdr::size__ Amount( void ) const
		{
			return Released.Amount();
		}
		//f Return true' if 'ID' is available, false otherwize.
		bso::bool__ IsAvailable( id__ ID ) const
		{
			return IsAvailable_( ID );
		}
	};

	E_AUTO1( ids_store )

	#define E_IDS_STORE_( t )	ids_store_<t>
	#define E_IDS_STORE( t )	ids_store<t>
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
