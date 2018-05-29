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

//	$Id: ndbtbl.h,v 1.25 2013/04/15 10:50:53 csimon Exp $

#ifndef NDBTBL__INC
#define NDBTBL__INC

#define NDBTBL_NAME		"NDBTBL"

#define	NDBTBL_VERSION	"$Revision: 1.25 $"

#define NDBTBL_OWNER		"Claude SIMON"

#if defined( E_DEBUG ) && !defined( NDBTBL_NODBG )
#define NDBTBL_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.25 $
//C Claude SIMON (csimon at zeusw dot org)
//R $Date: 2013/04/15 10:50:53 $

/* End of automatic documentation generation part. */

/* Addendum to the automatic documentation generation part. */
//D New DataBase TaBLe (will be moved on its own project in the future) 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"
#include "ndbbsc.h"
#include "ndbctt.h"
#include "ndbidx.h"

namespace ndbtbl {
	using namespace ndbctt;
	typedef ndbctt::post_initialization_function__ content_post_initialization_function__;

	using namespace ndbidx;
	typedef ndbidx::post_initialization_function__ index_post_initialization_function__;

	using ndbbsc::rrows_;
	using ndbbsc::rrows;

	enum mode__ {
		// Pas d'indexation, ou diffre.
		mBulk,
		// Lecture seule.
		mReadOnly,
		// Lecture/criture.
		mReadWrite,
		m_amount,
		m_Undefined
	};

	inline fil::mode__ Convert( mode__ Mode )
	{
		switch ( Mode ) {
		case mBulk:
		case mReadWrite:
			return fil::mReadWrite;
			break;
		case mReadOnly:
			return fil::mReadOnly;
			break;
		case m_Undefined:
			qRFwk();
			break;
		default:
			qRFwk();
			break;
		}

		return fil::m_Undefined;	// Pour viter un 'warning'.
	}

	typedef bch::E_BUNCH( index_ * ) _indexes;

	typedef ctn::E_MCONTAINER_( datum_ ) data_;
	E_AUTO( data );

	typedef bch::E_BUNCH_( sdr::row__ ) rows_;
	E_AUTO( rows );


	typedef ndbctt::content_ _content_;

	class table_
	{
	private:
		_indexes _Indexes;
		void _TestMode( mode__ Mode ) const
		{
			switch ( Mode ) {
			case mBulk:
				qRFwk();
				break;
			case mReadWrite:
				switch ( S_.Mode ) {
				case mBulk:
				case mReadWrite:
					break;
				case mReadOnly:
					qRFwk();
					break;
				default:
					qRFwk();
					break;
				}
				break;
			case mReadOnly:
				break;
				switch ( S_.Mode ) {
				case mBulk:
					qRFwk();
					break;
				case mReadWrite:
				case mReadOnly:
					break;
				default:
					qRFwk();
					break;
				}
				break;
			default:
				qRFwk();
				break;
			}
		}
		void _Test( mode__ Mode )
		{
			_TestMode( Mode );

			switch ( S_.Mode ) {
			case mBulk:
			case mReadWrite:
				break;
			case mReadOnly:
				qRFwk();
				break;
			default:
				qRFwk();
				break;
			}
		}
		void _Test( mode__ Mode ) const
		{
			_TestMode( Mode );

			switch ( S_.Mode ) {
			case mBulk:
			case mReadWrite:
			case mReadOnly:
				break;
			default:
				qRFwk();
				break;
			}
		}
#if 0
		void _Reindex(
			irow__ Row,
			observer_functions__ &Observer );
#endif
		void _InsertInIndexes( rrow__ Row );
		void _DeleteFromIndexes( rrow__ Row );
		void _ReindexAll( observer_functions__ &Observer );
		void _ResetAllIndexes( void );
		bso::bool__ _IsBulk( void ) const
		{
			switch ( S_.Mode ) {
			case mBulk:
				return true;
			case mReadWrite:
			case mReadOnly:
				return false;
				break;
			default:
				qRFwk();
				return false;	// Pour viter un warning.
				break;
			}
		}
	public:
		struct s
		{
			_content_::s Content;
			mode__ Mode;
		} &S_;
		_content_ Content;
		table_( s &S )
		: S_( S ),
		  Content( S.Content )
		{}
		void reset( bso::bool__ P = true )
		{
			Content.reset( P );
			S_.Mode = m_Undefined;
		}
		E_VDTOR( table_ )	// Pour qu'un 'delete' sur cette classe appelle le destructeur de la classe hritante.
		void plug( qAS_ &AS )
		{
			Content.plug( AS );
		}
		table_ &operator =( const table_ &T )
		{
			Content = T.Content;
			
			// _Indexes = T._Indexes;	// Ne contient que des pointeurs; les indexes correspondant doivent tre copis extrieurement.
			// S_.Mode = T.S_.Mode;	// N'est pas du contenu.

			return *this;
		}
		void Init( mode__ Mode )
		{
			reset();

			Content.Init();
			_Indexes.Init();

			S_.Mode = Mode;
		}
		void InitStatic(
			sdr::size__ Size,
			mode__ Mode,
			content_post_initialization_function__ &PostInitializationFunction )
		{
			Init( Mode );

			Content.InitStatic( Size, PostInitializationFunction );
		}
		void InitDynamic(
			mode__ Mode,
			content_post_initialization_function__ &PostInitializationFunction )
		{
			Init( Mode );

			Content.InitDynamic( PostInitializationFunction );
		}
		void Bufferize( void )
		{
			Content.Bufferize();
		}
		E_NAVt( Content()., rrow__ );
		void AddIndex( index_ &Index )
		{
			_Indexes.Append( &Index );
		}
		rrow__ Insert( const datum_ &Datum )
		{
			_Test( mReadWrite );

			rrow__ Row = Content().Store( Datum );

			if ( !_IsBulk() )
				_InsertInIndexes( Row );

			return Row;
		}
		void Insert(
			const data_ &Data,
			rrows_ &RecordRows );
		void Update(
			const datum_ &Datum,
			rrow__ RecordRow )
		{
			_Test( mReadWrite );

			if ( !_IsBulk() )
				_DeleteFromIndexes( RecordRow );

			Content().Store( Datum, RecordRow );

			if ( !_IsBulk() )
				_InsertInIndexes( RecordRow );
		}
		void Update(
			const data_ &Data,
			const rrows_ &RecordRows );
		void Retrieve(
			rrow__ Row,
			datum_ &Datum ) const
		{
			_Test( mReadOnly );

			Content().Retrieve( Row, Datum );
		}
		void Retrieve(
			const rrows_ &Rows,
			data_ &Data ) const;
		void Delete( rrow__ RecordRow )
		{
			_Test( mReadWrite );

			/* Si l'on est dans le cas d'un primo-accs  l'index, celui-ci s'appuie sur le premier lment de 'Content()' pour trouver sa racine.
			Si 'Content()' n'en cotient plus qu'un seul, pour viter de l'effacer avant initialisation de l'index,
			on efface l'lment de l'index avant de l'effacer de 'Content()'. */

			if ( !_IsBulk() )
				_DeleteFromIndexes( RecordRow );

			Content().Erase( RecordRow );
		}
		void Delete( const rrows_ &RecordRows );
		mode__ SwitchMode( mode__ Mode )
		{
			mode__ OldMode = S_.Mode;

			switch ( Mode ) {
			case mBulk:
//				_ResetAllIndexes();
				break;
			case mReadWrite:
			case mReadOnly:
				switch( OldMode ) {
				case mBulk:
					if ( !AreAllIndexesSynchronized() )
						qRFwk();
					break;
				case mReadOnly:
				case mReadWrite:
					break;
				default:
					qRFwk();
					break;
				}
				break;
			default:
				qRFwk();
				break;
			}

			S_.Mode = Mode;

			return OldMode;
		}
		mode__ Mode( void ) const
		{
			return S_.Mode;
		}
		bso::bool__ RecordExists( rrow__ RecordRow ) const
		{
			_Test( mReadOnly );

			return Content().Exists( RecordRow );
		}
		// 'Rows' contient les position dans 'RecordRows' des enregistrement inexistants.
		void TestRecordsExistence(
			const rrows_ &RecordRows,
			rows_ &Rows ) const;
		void ReindexAll( observer_functions__ &Observer = *(observer_functions__ *)NULL )
 		{
			_ReindexAll( Observer );
		}
		bso::bool__ AreAllIndexesSynchronized( void ) const;
		type__ Type( void ) const
		{
			return Content().Type();
		}
	};

	E_AUTO( table )

	class files_hook___
	{
	private:
		type__ _Type;
		// Seulement l'un des deux est utilis.
		ndbdct::files_hook___ _Dynamic;
		ndbsct::files_hook___ _Static;
		void _InitStatic(
			const str::string_ &BaseFileName,
			mode__ Mode,
			fls::id__ ID );
		void _InitDynamic(
			const str::string_ &BaseFileName,
			mode__ Mode,
			fls::id__ ID );
	public:
		void reset( bso::bool__ P = true )
		{
			_Type = t_Undefined;
			_Dynamic.reset( P );
			_Static.reset( P );
		}
		files_hook___( void )
		{
			reset( false );
		}
		~files_hook___( void )
		{
			reset();
		}
		void Init(
			type__ Type,
			const str::string_ &BaseFileName,
			mode__ Mode,
			fls::id__ ID )
		{
			_Type = Type;

			switch ( _Type ) {
			case tStatic:
				_InitStatic( BaseFileName, Mode, ID );
				break;
			case tDynamic:
				_InitDynamic( BaseFileName, Mode, ID );
				break;
			default:
				qRFwk();
				break;
			}
		}
		uys::state__ Bind( void )
		{
			switch ( _Type ) {
			case tStatic:
				return _Static.Bind();
				break;
			case tDynamic:
				return _Dynamic.Bind();
				break;
			default:
				qRFwk();
				break;
			}

			return uys::s_Undefined;	// Pour viter un 'warning'.
		}
		uys::state__ Settle( void )
		{
			switch ( _Type ) {
			case tStatic:
				return _Static.Settle();
				break;
			case tDynamic:
				return _Dynamic.Settle();
				break;
			default:
				qRFwk();
				break;
			}

			return uys::s_Undefined;	// Pour viter un 'warning'.
		}
		const str::string_ &BaseFileName( type__ Type ) const
		{
			switch ( Type ) {
			case tStatic:
				return _Static.BaseFileName();
				break;
			case tDynamic:
				return _Dynamic.BaseFileName();
				break;
			default:
				qRFwk();
				break;
			}

			qRFwk();

			return _Static.BaseFileName();	// Pour viter un 'warning'.
		}
		ndbdct::files_hook___ &DynamicFilesHook( void )
		{
			return _Dynamic;
		}
		ndbsct::files_hook___ &StaticFilesHook( void )
		{
			return _Static;
		}
	};

	inline uys::state__ Plug(
		table_ &Table,
		files_hook___ &Hook )
	{
		uys::state__ State = uys::s_Undefined;

		switch ( Table.Type() ) {
		case tStatic:
			State = ndbsct::Plug( Table.Content.Static(), Hook.StaticFilesHook() );
			break;
		case tDynamic:
			State = ndbdct::Plug( Table.Content.Dynamic(), Hook.DynamicFilesHook() );
			break;
		default:
			qRFwk();
			break;
		}

		return State;

	}



}

/*$END$*/
#endif
