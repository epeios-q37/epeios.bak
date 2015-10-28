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

//	$Id: dbsctt.h,v 1.20 2009/11/12 18:25:54 csimon Exp $

#ifndef DBSCTT__INC
#define DBSCTT__INC

#define DBSCTT_NAME		"DBSCTT"

#define	DBSCTT_VERSION	"$Revision: 1.20 $"

#define DBSCTT_OWNER		"Claude SIMON (http://zeusw.org/intl/contact.html)"

#if defined( E_DEBUG ) && !defined( DBSCTT_NODBG )
#define DBSCTT_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.20 $
//C Claude SIMON (http://zeusw.org/intl/contact.html)
//R $Date: 2009/11/12 18:25:54 $

/* End of automatic documentation generation part. */

/* Addendum to the automatic documentation generation part. */
//D DataBaSe DaTA 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"
#include "str.h"
#include "dtfptb.h"
#include "stk.h"
#include "lstbch.h"
#include "lstctn.h"
#include "flm.h"
#include "que.h"
#include "dbsdct.h"
#include "dbssct.h"

namespace dbsctt {

	using dbsbsc::rrow__;
	using dbsbsc::datum_;
	using dbsbsc::_cache_;

	class content__
	{
	private:
		dbsdct::dynamic_content_ *_Dynamic;
		dbssct::static_content_ *_Static;
	public:
		void reset( bso::bool__ = true )
		{
			_Dynamic = NULL;
			_Static = NULL;
		}
		content__( void )
		{
			reset( false );
		}
		void Init( dbsdct::dynamic_content_ &Dynamic )
		{
			if ( ( _Dynamic != NULL ) || ( _Static != NULL ) )
				ERRu();

			_Dynamic = &Dynamic;
		}
		void Init( dbssct::static_content_ &Static )
		{
			if ( ( _Dynamic != NULL ) || ( _Static != NULL ) )
				ERRu();

			_Static = &Static;
		}
		rrow__ Store( const datum_ &Data )
		{
			if ( _Static != NULL )
				return _Static->Store( Data );
			else if ( _Dynamic != NULL )
				return _Dynamic->Store( Data );
			else
				ERRu();

			return NONE;	// Pour viter un 'warning'.
		}
		void Erase( rrow__ Row )
		{
			if ( _Static != NULL )
				_Static->Erase( Row );
			else if ( _Dynamic != NULL )
				_Dynamic->Erase( Row );
			else
				ERRu();
		}
		void Store(
			const datum_ &Data,
			rrow__ Row )
		{
			if ( _Static != NULL )
				_Static->Store( Data, Row );
			else if ( _Dynamic != NULL )
				_Dynamic->Store( Data, Row );
			else
				ERRu();
		}
		// Retourne 'true' si l'enregistrement existe, faux sinon.
		bso::bool__ Retrieve(
			rrow__ Row,
			datum_ &Datum ) const
		{
			if ( _Static != NULL ) {
				 _Static->Retrieve( Row, Datum );
				 return true;
			} else if ( _Dynamic != NULL )
				return _Dynamic->Retrieve( Row, Datum );
			else
				ERRu();

			return false;	// Pour viter un 'warning'.
		}
		// Retourne 'true' si l'enregistrement existe, faux sinon.
		bso::bool__ Retrieve(
			rrow__ Row,
			datum_ &Datum,
			_cache_ &Cache ) const
		{
			if ( _Static != NULL ) {
				_Static->Retrieve( Row, Datum, Cache );
				return true;
			} else if ( _Dynamic != NULL )
				return _Dynamic->Retrieve( Row, Datum, Cache );
			else
				ERRu();

			return false;	// Pour viter un 'warning'.
		}
		time_t ModificationTimeStamp( void ) const
		{
			if ( _Static != NULL )
				return _Static->ModificationTimeStamp();
			else if ( _Dynamic != NULL )
				return _Dynamic->ModificationTimeStamp();
			else
				ERRu();

			return 0;	// Pour viter un 'warning'.
		}
		rrow__ First( void ) const
		{
			if ( _Static != NULL )
				return _Static->First();
			else if ( _Dynamic != NULL )
				return _Dynamic->First();
			else
				ERRu();

			return NONE;
		}
		rrow__ Last( void ) const
		{
			if ( _Static != NULL )
				return _Static->Last();
			else if ( _Dynamic != NULL )
				return _Dynamic->Last();
			else
				ERRu();

			return NONE;
		}
		rrow__ Next( rrow__ Row ) const
		{
			if ( _Static != NULL )
				return _Static->Next( Row );
			else if ( _Dynamic != NULL )
				return _Dynamic->Next( Row );
			else
				ERRu();

			return NONE;
		}
		rrow__ Previous( rrow__ Row ) const
		{
			if ( _Static != NULL )
				return _Static->Previous( Row );
			else if ( _Dynamic != NULL )
				return _Dynamic->Previous( Row );
			else
				ERRu();

			return NONE;
		}
		epeios::size__ Extent( void ) const
		{
			if ( _Static != NULL )
				return _Static->Extent();
			else if ( _Dynamic != NULL )
				return _Dynamic->Extent();
			else
				ERRu();

			return 0;	// Pour viter un 'warning'.
		}
		epeios::size__ Amount( void ) const
		{
			if ( _Static != NULL )
				return _Static->Amount();
			else if ( _Dynamic != NULL )
				return _Dynamic->Amount();
			else
				ERRu();

			return 0;	// Pour viter un 'warning'.
		}
		bso::bool__ Exists( rrow__ Row ) const
		{
			if ( _Static != NULL )
				return _Static->Exists( Row );
			else if ( _Dynamic != NULL )
				return _Dynamic->Exists( Row );
			else
				ERRu();

			return false;
		}
		bso::bool__ IsEmpty( void ) const
		{
			if ( _Static != NULL )
				return _Static->IsEmpty();
			else if ( _Dynamic != NULL )
				return _Dynamic->IsEmpty();
			else
				ERRu();

			return false;
		}
		void CompleteInitialization( void )
		{
			if ( _Static != NULL )
				_Static->CompleteInitialization();
			else if ( _Dynamic != NULL )
				_Dynamic->CompleteInitialization();
			else
				ERRu();
		}
		epeios::size__ InitializationCompleted( void ) const
		{
			if ( _Static != NULL )
				return _Static->InitializationCompleted();
			else if ( _Dynamic != NULL )
				return _Dynamic->InitializationCompleted();
			else
				ERRu();

			return false;	// Pour viter un 'warning'.
		}
		void ErasePhysically( void )
		{
			if ( _Static != NULL )
				 _Static->ErasePhysically();
			else if ( _Dynamic != NULL )
				_Dynamic->ErasePhysically();
			else
				ERRu();
		}
	};

	class content_
	{
	private:
		content__ _Content;
	public:
		struct s {
			dbsdct::dynamic_content_::s Dynamic;
			dbssct::static_content_::s Static;
		};
		dbsdct::dynamic_content_ Dynamic;
		dbssct::static_content_ Static;
		content_( s &S )
		: Dynamic( S.Dynamic ),
		  Static( S.Static )
		{
		}
		void reset( bso::bool__ P = true )
		{
			_Content.reset( P );
			Dynamic.reset( P );
			Static.reset( P );
		}
		void plug( mmm::E_MULTIMEMORY_ &MM )
		{
			Dynamic.plug( MM );
			Static.plug( MM );
		}
		content_ &operator =( const content_ &C )
		{
			_Content = C._Content;

			Dynamic = C.Dynamic;
			Static = C.Static;

			return *this;
		}
		void InitDynamic( void )
		{
			reset();

			Dynamic.Init();
			_Content.Init( Dynamic );
		}
		void InitStatic( epeios::size__ Size )
		{
			reset();

			Static.Init( Size );
			_Content.Init( Static );
		}
		const content__ *operator ->( void ) const
		{
			return &_Content;
		}
		content__ *operator ->( void )
		{
			return &_Content;
		}
		const content__ *operator *( void ) const
		{
			return &_Content;
		}
		content__ *operator *( void )
		{
			return &_Content;
		}
		E_RWDISCLOSE__( content__, Content )
	};

	E_AUTO( content )

}

/*$END$*/
#endif
