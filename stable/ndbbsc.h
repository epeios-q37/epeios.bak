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

//	$Id: ndbbsc.h,v 1.10 2013/04/15 10:50:52 csimon Exp $

#ifndef NDBBSC__INC
#define NDBBSC__INC

#define NDBBSC_NAME		"NDBBSC"

#define	NDBBSC_VERSION	"$Revision: 1.10 $"

#define NDBBSC_OWNER		"Claude SIMON"

#if defined( E_DEBUG ) && !defined( NDBBSC_NODBG )
#define NDBBSC_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.10 $
//C Claude SIMON (csimon at zeusw dot org)
//R $Date: 2013/04/15 10:50:52 $

/* End of automatic documentation generation part. */

/* Addendum to the automatic documentation generation part. */
//D New DataBase BaSiCs (will be moved on its own project in the future) 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "bso.h"
#include "str.h"
#include "lstctn.h"
#include "que.h"
#include "bch.h"

namespace ndbbsc {
	qROW( rrow__ );	// 'Record row'.

	typedef bch::E_BUNCH_( rrow__ ) rrows_;
	E_AUTO( rrows );


	qROW( crow__ );	// 'Cache row'.

	typedef bso::char__ atom__;
	typedef str::string_ datum_;
	typedef str::string datum;

	typedef lstctn::E_LXMCONTAINERt_( datum_, crow__ ) _container_;
	typedef que::E_MQUEUEt_( crow__ ) _queue_;
	typedef bch::E_BUNCHt_( crow__, rrow__ ) _list_;
	typedef bch::E_BUNCHt_( rrow__, crow__ ) _links_;

	#define NDBBSC_CACHE_DEFAULT_AMOUNT_MAX	( 2 << 12 )

	class cache_
	{
	private:
		void _PutToHead( crow__ Row )
		{
			if ( Queue.Exists( Row ) && Queue.IsMember( Row ) )
				Queue.Delete( Row );

			if ( Queue.IsEmpty() )
				Queue.Create( Row );
			else
				Queue.BecomePrevious( Row, Queue.Head() );
		}
		bso::bool__ _IsMember( rrow__ Row ) const
		{
			if ( List.Exists( Row ) )
				return List( Row ) != qNIL;
			else
				return false;
		}
	public:
		_container_ Container;
		_queue_ Queue;
		_list_ List;
		_links_ Links;
		struct s {
			_container_::s Container;
			_queue_::s Queue;
			_list_::s List;
			_links_::s Links;
			bso::size__ AmountMax;
		} &S_;
		cache_( s &S )
		: S_( S ),
		  Container( S.Container ),
		  Queue( S.Queue ),
		  List( S.List ),
		  Links( S.Links )
		{}
		void reset( bso::bool__ P = true )
		{
			Container.reset( P );
			Queue.reset( P );
			List.reset( P );
			Links.reset( P );

			S_.AmountMax = 0;
		}
		void plug( qAS_ &AS )
		{
			Container.plug( AS );
			Queue.plug( AS );
			List.plug( AS );
			Links.plug( AS );
		}
		cache_ &operator =( const cache_ &C )
		{
			Container = C.Container;
			Queue = C.Queue;
			List = C.List;
			Links = C.Links;

			S_.AmountMax = C.S_.AmountMax;

			return *this;
		}
		void Init(
			sdr::size__ Size,
			bso::size__ AmountMax = NDBBSC_CACHE_DEFAULT_AMOUNT_MAX )
		{
			reset();

			Container.Init();
			Queue.Init();
			List.Init();
			Links.Init();

			List.Allocate( Size );
			List.FillWith( qNIL );

			S_.AmountMax = AmountMax;
		}
		void Resize( sdr::size__ Size )
		{
			sdr::size__ CurrentSize = List.Amount();

			if ( CurrentSize > Size )
				qRFwk();
			else if ( CurrentSize < Size ) {
				List.Allocate( Size );

				List.FillWith( qNIL, CurrentSize );
			}
		}
		bso::bool__ Retrieve(
			rrow__ Row,
			datum_ &Datum )
		{
			bso::bool__ IsMember = false;

			if ( IsMember = _IsMember( Row ) ) {
				crow__ CacheRow = List( Row );
				Container.Recall( CacheRow, Datum );
				_PutToHead( CacheRow );
			}

			return IsMember;
		}
		void Store(
			const datum_ &Datum,
			rrow__ Row )
		{
#ifdef NDBBSC_DBG
			if ( _IsMember( Row ) )
				qRFwk();
#endif
			crow__ CacheRow = qNIL;

			if ( Queue.Amount() >= S_.AmountMax ) {
				CacheRow = Queue.Tail();
				List.Store( qNIL, Links( CacheRow ) );
			} else {
				CacheRow = Container.New();
				Links.Allocate( Container.Extent() );
				Queue.Allocate( Container.Extent() );
			}

			Container.Store( Datum, CacheRow );
			Links.Store( Row, CacheRow );
			_PutToHead( CacheRow );
			List.Store( CacheRow, Row );
		}
		void Remove( rrow__ Row )
		{
			if ( _IsMember( Row ) ) {
				crow__ CacheRow = List( Row );
				Container.Remove( CacheRow );
				List.Store( qNIL, Links( CacheRow ) );
				Links.Store( qNIL, CacheRow );
				Queue.Delete( CacheRow );
				List.Store( qNIL, Row );
			}
		}
		const sdr::size__ Size( void ) const
		{
			return List.Amount();
		}
	};

	E_AUTO( cache )

	void DropFile(
		const str::string_ &RootFileName,
		const char *Extension );


}

/*$END$*/
#endif
