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

//	$Id: ndbctt.h,v 1.15 2013/04/15 10:50:52 csimon Exp $

#ifndef NDBCTT_INC_
#define NDBCTT_INC_

#define NDBCTT_NAME		"NDBCTT"

#define	NDBCTT_VERSION	"$Revision: 1.15 $"

#define NDBCTT_OWNER		"Claude SIMON"

#if defined( E_DEBUG ) && !defined( NDBCTT_NODBG )
#define NDBCTT_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.15 $
//C Claude SIMON (csimon at zeusw dot org)
//R $Date: 2013/04/15 10:50:52 $

/* End of automatic documentation generation part. */

/* Addendum to the automatic documentation generation part. */
//D New DataBase ConTenT (will be moved on its own project in the future) 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "ndbbsc.h"
#include "ndbdct.h"
#include "ndbsct.h"

namespace ndbctt {

	using ndbbsc::rrow__;
	using ndbbsc::datum_;
	using ndbbsc::cache_;

	enum type__ {
		tStatic,
		tDynamic,
		t_amount,
		t_Undefined
	};

	struct post_initialization_function__
	{
	protected:
		virtual void NDBCTTCompleteInitialization( void ) = 0;
	public:
		void CompleteInitialization( void )
		{
			NDBCTTCompleteInitialization();
		}
	};

	class content__
	{
	private:
		ndbdct::dynamic_content_ *_Dynamic;
		ndbsct::static_content_ *_Static;
		mutable ndbbsc::cache _Cache;
		mutable post_initialization_function__ *_PostInitializationFunction;
		void _CompleteInitialization( void ) const
		{
			if ( _PostInitializationFunction != NULL ) {
				// On passe par un 'Buffer' pour viter un appel rcursif.
				post_initialization_function__ *Buffer = _PostInitializationFunction;
				_PostInitializationFunction = NULL;
				Buffer->CompleteInitialization();
			}
		}
		type__ _Test( bso::bool__ CompleteInitializationIfNeeded = true ) const
		{
			if ( ( _Static == NULL ) == ( _Dynamic == NULL ) )
				qRFwk();

			if ( CompleteInitializationIfNeeded	)
				_CompleteInitialization();

			if ( _Static != NULL )
				return tStatic;
			else if ( _Dynamic != NULL )
				return tDynamic;
			else
				qRFwk();

			return t_Undefined;	// Pour viter un 'warning'.
		}
		bso::bool__ _UseCache( void ) const
		{
			sdr::size__ Size = 0;

			switch ( _Test() ) {
			case tStatic:
				Size = _Static->Extent();
				break;
			case tDynamic:
				Size = _Dynamic->Extent();
				break;
			default:
				qRFwk();
				break;
			}

			if ( Size > _Cache.Size() )
				_Cache.Resize( Size );	/* Normalement fait  l'initialisation ou lors de l'ajout d'un enregistrement,
										mais donne pas encore initialise lors de l'initialisation en mode 'mono-fichier'. */

			return true;
		}
	public:
		void reset( bso::bool__ = true )
		{
			_Dynamic = NULL;
			_Static = NULL;
			_Cache.reset();
			_PostInitializationFunction = NULL;
		}
		content__( void )
		{
			reset( false );
		}
		content__ &operator =( const content__ &C )
		{
			// Pas de copie des pointeurs !

			C._CompleteInitialization();
			_CompleteInitialization();

			_Cache = C._Cache;

			return *this;
		}
		void Init( void )
		{
			reset();
		}
		void Init(
			ndbdct::dynamic_content_ &Dynamic,
			post_initialization_function__ &PostInitializationFunction )
		{
			if ( ( _Dynamic != NULL ) || ( _Static != NULL ) )
				qRFwk();

			_Dynamic = &Dynamic;
			_Cache.Init( Dynamic.Extent() );

			_PostInitializationFunction = &PostInitializationFunction;
		}
		void Init(
			ndbsct::static_content_ &Static,
			post_initialization_function__ &PostInitializationFunction )
		{
			if ( ( _Dynamic != NULL ) || ( _Static != NULL ) )
				qRFwk();

			_Static = &Static;
			_Cache.Init( Static.Extent() );

			_PostInitializationFunction = &PostInitializationFunction;
		}
		rrow__ Store( const datum_ &Datum )
		{
			rrow__ Row = qNIL;
			sdr::size__ Size = 0;

			switch ( _Test() ) {
			case tStatic:
				Row = _Static->Store( Datum );
				Size = _Static->Extent();
				break;
			case tDynamic:
				Row = _Dynamic->Store( Datum );
				Size = _Dynamic->Extent();
				break;
			default:
				qRFwk();
				break;
			}

			if ( _UseCache() ) {
				_Cache.Resize( Size );
				_Cache.Store( Datum, Row );
			}

			return Row;
		}
		void Erase(	rrow__ Row )
		{
			switch ( _Test() ) {
			case tStatic:
				_Static->Erase( Row );
				break;
			case tDynamic:
				_Dynamic->Erase( Row );
				break;
			default:
				qRFwk();
				break;
			}

			if ( _UseCache() )
				_Cache.Remove( Row );
		}
		void Store(
			const datum_ &Datum,
			rrow__ Row )
		{
			switch ( _Test() ) {
			case tStatic:
				_Static->Store( Datum, Row );
				break;
			case tDynamic:
				_Dynamic->Store( Datum, Row );
				break;
			default:
				qRFwk();
				break;
			}

			if ( _UseCache ()) {
				_Cache.Remove( Row );
				_Cache.Store( Datum, Row );
			}

		}
		// Retourne 'true' si l'enregistrement existe, faux sinon.
		bso::bool__ Retrieve(
			rrow__ Row,
			datum_ &Datum ) const
		{
			bso::bool__ Exists = false;

			if ( !_UseCache() || ( !_Cache.Retrieve( Row, Datum ) ) ) {
				switch ( _Test() ) {
				case tStatic:
					 _Static->Retrieve( Row, Datum );
					 Exists = true;
					break;
				case tDynamic:
					Exists = _Dynamic->Retrieve( Row, Datum );
					break;
				default:
					qRFwk();
					break;
				}

				if ( _UseCache() && Exists )
					_Cache.Store( Datum, Row );
			} else
				Exists = true;

			return Exists;
		}
		time_t ModificationEpochTimeStamp( void ) const
		{
			switch ( _Test( false ) ) {
			case tStatic:
				return _Static->ModificationEpochTimeStamp();
				break;
			case tDynamic:
				return _Dynamic->ModificationEpochTimeStamp();
				break;
			default:
				qRFwk();
				break;
			}

			return 0;	// Pour viter un 'warning'.
		}
		rrow__ First( void ) const
		{
			switch ( _Test() ) {
			case tStatic:
				return _Static->First();
				break;
			case tDynamic:
				return _Dynamic->First();
				break;
			default:
				qRFwk();
				break;
			}

			return qNIL;
		}
		rrow__ Last( void ) const
		{
			switch ( _Test() ) {
			case tStatic:
				return _Static->Last();
				break;
			case tDynamic:
				return _Dynamic->Last();
				break;
			default:
				qRFwk();
				break;
			}

			return qNIL;
		}
		rrow__ Next( rrow__ Row ) const
		{
			switch ( _Test() ) {
			case tStatic:
				return _Static->Next( Row );
				break;
			case tDynamic:
				return _Dynamic->Next( Row );
				break;
			default:
				qRFwk();
				break;
			}

			return qNIL;
		}
		rrow__ Previous( rrow__ Row ) const
		{
			switch ( _Test() ) {
			case tStatic:
				return _Static->Previous( Row );
				break;
			case tDynamic:
				return _Dynamic->Previous( Row );
				break;
			default:
				qRFwk();
				break;
			}

			return qNIL;
		}
		sdr::size__ Extent( void ) const
		{
			switch ( _Test() ) {
			case tStatic:
				return _Static->Extent();
				break;
			case tDynamic:
				return _Dynamic->Extent();
				break;
			default:
				qRFwk();
				break;
			}

			return 0;	// Pour viter un 'warning'.
		}
		sdr::size__ Amount( void ) const
		{
			switch ( _Test() ) {
			case tStatic:
				return _Static->Amount();
				break;
			case tDynamic:
				return _Dynamic->Amount();
				break;
			default:
				qRFwk();
				break;
			}

			return 0;	// Pour viter un 'warning'.
		}
		bso::bool__ Exists( rrow__ Row ) const
		{
			switch ( _Test() ) {
			case tStatic:
				return _Static->Exists( Row );
				break;
			case tDynamic:
				return _Dynamic->Exists( Row );
				break;
			default:
				qRFwk();
				break;
			}

			return false;
		}
		bso::bool__ IsEmpty( void ) const
		{
			switch ( _Test() ) {
			case tStatic:
				return _Static->IsEmpty();
				break;
			case tDynamic:
				return _Dynamic->IsEmpty();
				break;
			default:
				qRFwk();
				break;
			}

			return false;
		}
		type__ Type( void ) const
		{
			return _Test( false );
		}
	};

	class _content_
	{
	private:
		content__ _Content;
		ndbdct::dynamic_content_ _Dynamic;
		ndbsct::static_content_ _Static;
	public:
		struct s {
			ndbdct::dynamic_content_::s Dynamic;
			ndbsct::static_content_::s Static;
		};
		_content_( s &S )
		: _Dynamic( S.Dynamic ),
		  _Static( S.Static )
		{
		}
		void reset( bso::bool__ P = true )
		{
			_Content.reset( P );
			_Dynamic.reset( P );
			_Static.reset( P );
		}
		void plug( qAS_ &AS )
		{
			_Dynamic.plug( AS );
			_Static.plug( AS );
		}
		_content_ &operator =( const _content_ &C )
		{
			_Content = C._Content;

			_Dynamic = C._Dynamic;
			_Static = C._Static;

			return *this;
		}
		void Init( void )
		{
			reset();

			_Content.Init();
		}
		void InitDynamic( post_initialization_function__ &PostInitializationFunction  )
		{
			reset();

			_Static.Init( 0 );
			_Dynamic.Init();
			_Content.Init( _Dynamic, PostInitializationFunction );
		}
		void InitStatic(
			sdr::size__ Size,
			post_initialization_function__&PostInitializationFunction  )
		{
			reset();

			_Dynamic.Init();
			_Static.Init( Size );
			_Content.Init( _Static, PostInitializationFunction );
		}
		const content__ &operator ()( void ) const
		{
			return _Content;
		}
		content__ &operator ()( void )
		{
			return _Content;
		}
		type__ Type( void ) const
		{
			return _Content.Type();
		}
		ndbsct::static_content_ &Static( void )
		{
			if ( Type() != tStatic )
				qRFwk();

			return _Static;
		}
		const ndbsct::static_content_ &Static( void ) const
		{
			if ( Type() != tStatic )
				qRFwk();

			return _Static;
		}
		const ndbdct::dynamic_content_ &Dynamic( void ) const
		{
			if ( Type() != tDynamic )
				qRFwk();

			return _Dynamic;
		}
		ndbdct::dynamic_content_ &Dynamic( void )
		{
			if ( Type() != tDynamic )
				qRFwk();

			return _Dynamic;
		}
	};

	E_AUTO( _content )

	class content_
	{
	private:
		bso::bool__ _Bufferized;
		const _content_ &_Content( void ) const
		{
			if ( _Bufferized )
				return BContent;
			else
				return DContent;
		}
		_content_ &_Content( void )
		{
			if ( _Bufferized )
				return BContent;
			else
				return DContent;
		}
	public:
		_content BContent;	// 'Bufferized content'.
		struct s
		{
			_content_::s DContent;
		}& S_;
		_content_ DContent;	// 'Direct content'.
		content_( s &S )
		: S_( S ),
		  DContent( S.DContent )
		{}
		void reset( bso::bool__ P = true )
		{
			_Bufferized = false;
			DContent.reset( P );
			BContent.reset( P );
		}
		void plug( qAS_ &AS )
		{
			DContent.plug( AS );
		}
		content_ &operator =( const content_ &C )
		{
			DContent = C.DContent;
			BContent = C.BContent;

			return *this;
		}
		void Init( void )
		{
			_Bufferized = false;

			DContent.Init();
			BContent.Init();
		}
		void InitStatic(
			sdr::size__ Size,
			post_initialization_function__ &PostInitializationFunction )
		{
			_Bufferized = false;

			DContent.InitStatic( Size, PostInitializationFunction );

			BContent.InitStatic( Size, *(post_initialization_function__ *)NULL );
//			BContent = DContent;
		}
		void InitDynamic( post_initialization_function__ &PostInitializationFunction )
		{
			_Bufferized = false;

			DContent.InitDynamic( PostInitializationFunction );

			BContent.InitDynamic( *(post_initialization_function__ *)NULL );
//			BContent = DContent;
		}
		const content__ &operator ()( void ) const
		{
			return _Content()();
		}
		content__ &operator ()( void )
		{
			return _Content()();
		}
		type__ Type( void ) const
		{
			return _Content().Type();
		}
		ndbsct::static_content_ &Static( void )
		{
			return _Content().Static();
		}
		const ndbsct::static_content_ &Static( void ) const
		{
			return _Content().Static();
		}
		const ndbdct::dynamic_content_ &Dynamic( void ) const
		{
			return _Content().Dynamic();
		}
		ndbdct::dynamic_content_ &Dynamic( void )
		{
			return _Content().Dynamic();
		}
		void Bufferize( void )
		{
			if ( !_Bufferized ) {
				BContent = DContent;
				
				_Bufferized = true;
			}
		}
	};



}

/*$END$*/
#endif
