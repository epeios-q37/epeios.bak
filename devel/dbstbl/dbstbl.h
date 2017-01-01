/*
	Copyright (C) 1999-2017 Claude SIMON (http://q37.info/contact/).

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


#ifndef DBSTBL__INC
#define DBSTBL__INC

#define DBSTBL_NAME		"DBSTBL"

#if defined( E_DEBUG ) && !defined( DBSTBL_NODBG )
#define DBSTBL_DBG
#endif
//D DataBaSe TaBLe 

#include "err.h"
#include "flw.h"
#include "dbsctt.h"
#include "dbsidx.h"
#include "cpe.h"

#ifdef DBSTBL_THREAD_SAFE
#	define DBSTBL__THREAD_SAFE
#elif !defined( DBSTBL_NO_THREAD_SAFE )
#	ifdef CPE_F_MT
#		define DBSTBL__THREAD_SAFE
#	endif
#endif

#ifdef DBSTBL__THREAD_SAFE
#	include "lck.h"
#endif

namespace dbstbl {

	using namespace dbsctt;
	using namespace dbsidx;

	enum mode__ {
		// Pas d'indexation, ou diffre.
		mBulk,
		// Lecture seule.
		mReadOnly,
		// Lecture/criture.
		mReadWrite,
		// Administrateur : changement de la structure  (cration/suppression d'index, p. ex.).
		mAdmin,
		m_amount,
		m_Undefined
	};

	inline mdr::mode__ Convert( mode__ Mode )
	{
		switch ( Mode ) {
		case mBulk:
		case mReadWrite:
		case mAdmin:
			return mdr::mReadWrite;
			break;
		case mReadOnly:
			return mdr::mReadOnly;
			break;
		case m_Undefined:
			ERRu();
			break;
		default:
			ERRu();
			break;
		}

		return mdr::m_Undefined;	// Pour viter un 'warning'.
	}

	typedef bch::E_BUNCH_( index_ * ) _indexes_;

	typedef bch::E_BUNCH_( rrow__ ) rrows_;
	E_AUTO( rrows );

	typedef ctn::E_XMCONTAINER_( datum_ ) data_;
	E_AUTO( data );

	typedef bch::E_BUNCH_( epeios::row__ ) rows_;
	E_AUTO( rows );

	class table_
	{
	private:
#if 0
		void _ConnectIndexToFileIfNot( irow__ Row ) const
		{
#ifdef DBSTBL_DBG
			if ( !Indexes.Exists( Row ) )
				ERRc();
#endif
			if ( !Indexes( Row )->InitializationCompleted() ) {
				if ( !S_.Content->InitializationCompleted() )
					S_.Content->CompleteInitialization();

				Indexes( Row )->CompleteInitialization();

			}
		}
#endif
		void _CompleteInitializationIfNeeded( bso::bool__ CompleteInitializationIfNeeded ) const
		{
			if ( !S_.Content->InitializationCompleted() && CompleteInitializationIfNeeded )
				S_.Content->CompleteInitialization();
		}
		dbsctt::content__ &_C( bso::bool__ CompleteInitializationIfNeeded = true ) const	// L'absence de 'const' est normale.
		{
			_Test( mReadOnly );

			_CompleteInitializationIfNeeded( CompleteInitializationIfNeeded );

			return *S_.Content;
		}
		dbsctt::content__ &_C( bso::bool__ CompleteInitializationIfNeeded = true )
		{
			_Test( mReadWrite );

			_CompleteInitializationIfNeeded( CompleteInitializationIfNeeded );

			return *S_.Content;
		}
#if 0
		const index_ &_I(
			irow__ Row,
			bso::bool__ DontConnectToFiles = false ) const
		{
			if ( !DontConnectToFiles )
				_ConnectIndexToFileIfNot( Row );

			return *Indexes( Row );
		}
		index_ &_I(
			irow__ Row,
			bso::bool__ DontConnectToFiles = false )
		{
			if ( !DontConnectToFiles )
				_ConnectIndexToFileIfNot( Row );

			return *Indexes( Row );
		}
#endif
		void _TestMode( mode__ Mode ) const
		{
			switch ( Mode ) {
			case mBulk:
				ERRc();
				break;
			case mAdmin:
				switch ( S_.Mode ) {
				case mBulk:
					ERRu();
					break;
				case mAdmin:
					break;
				case mReadWrite:
				case mReadOnly:
					ERRu();
					break;
				default:
					ERRc();
					break;
				}
				break;
			case mReadWrite:
				switch ( S_.Mode ) {
				case mBulk:
				case mAdmin:
				case mReadWrite:
					break;
				case mReadOnly:
					ERRu();
					break;
				default:
					ERRc();
					break;
				}
				break;
			case mReadOnly:
				break;
				switch ( S_.Mode ) {
				case mBulk:
					ERRu();
					break;
				case mAdmin:
				case mReadWrite:
				case mReadOnly:
					break;
				default:
					ERRc();
					break;
				}
				break;
			default:
				ERRc();
				break;
			}
		}
		void _Test( mode__ Mode )
		{
			_TestMode( Mode );

			switch ( S_.Mode ) {
			case mBulk:
			case mReadWrite:
			case mAdmin:
				break;
			case mReadOnly:
				ERRu();
				break;
			default:
				ERRu();
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
			case mAdmin:
				break;
			default:
				ERRu();
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
			case mAdmin:
				return false;
				break;
			default:
				ERRu();
				return false;	// Pour viter un warning.
				break;
			}
		}
	protected:
		virtual void DBSTBLErasePhysically( void )
		{}
	public:
		struct s
		{
			str::string_ ::s Label;
			_indexes_::s Indexes;
			dbsctt::content__ *Content;	// On n'utilise pas 'content_', pour pouvoir appliquer
										// une opration non 'const' dans une mthode 'const'.
			mode__ Mode;
		} &S_;
		str::string_ Label;	// Sert d'identifiant discriminat de la table. Utile pour servir de base aux noms de fichiers associes  la table.
		_indexes_ Indexes;
		table_( s &S )
		: S_( S ),
		  Label( S.Label ),
		  Indexes( S.Indexes )
		{}
		void reset( bso::bool__ P = true )
		{
			Label.reset();
			Indexes.reset( P );
			S_.Content = NULL;
			S_.Mode = m_Undefined;
		}
		E_VDTOR( table_ )	// Pour qu'un 'delete' sur cette classe appelle le destructeur de la classe hritante.
		void plug( mmm::E_MULTIMEMORY_ &MM )
		{
			Label.plug( MM );
			Indexes.plug( MM );
		}
		table_ &operator =( const table_ &T )
		{
			Label = T.Label;
			Indexes = T.Indexes;
			S_.Content = T.S_.Content;
			S_.Mode = T.S_.Mode;

			return *this;
		}
		void Init(
			const str::string_ &Label,
			dbsctt::content__ &Content,
			mode__ Mode )
		{
			reset();

			Indexes.Init();

			this->Label.Init( Label );
			S_.Content = &Content;
			S_.Mode = Mode;
		}
		E_NAVt( _C()., rrow__ );
		dbsctt::content__ &Content( bso::bool__ CompleteInitializationIfNeeded )
		{
			return _C( CompleteInitializationIfNeeded );
		}
		dbsctt::content__ &Content( bso::bool__ CompleteInitializationIfNeeded ) const	// L'absence de 'const' est normal.
		{
			return _C( CompleteInitializationIfNeeded );
		}
		void AddIndex( index_ &Index )
		{
			_Test( mAdmin );

			Indexes.Append( &Index );
		}
		rrow__ Insert( const datum_ &Datum )
		{
			_Test( mReadWrite );

			rrow__ Row = _C().Store( Datum );

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

			_C().Store( Datum, RecordRow );

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

			_C().Retrieve( Row, Datum );
		}
		void Retrieve(
			const rrows_ &Rows,
			data_ &Data ) const;
		void Delete( rrow__ RecordRow )
		{
			_Test( mReadWrite );

			_C().Erase( RecordRow );

			if ( !_IsBulk() )
				_DeleteFromIndexes( RecordRow );
		}
		void Delete( const rrows_ &RecordRows );
		mode__ SwitchMode( mode__ Mode )
		{
			mode__ OldMode = S_.Mode;

			switch ( Mode ) {
			case mBulk:
//				_ResetAllIndexes();
				break;
			case mAdmin:
			case mReadWrite:
			case mReadOnly:
				switch( OldMode ) {
				case mBulk:
					if ( !AreAllIndexesSynchronized() )
						ERRu();
					break;
				case mAdmin:
				case mReadOnly:
				case mReadWrite:
					break;
				default:
					ERRc();
					break;
				}
				break;
			default:
				ERRu();
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

			return _C().Exists( RecordRow );
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
		void ErasePhysically( void )
		{
			DBSTBLErasePhysically();
		}
	};

	E_AUTO( table )

#ifdef DBSTBL__THREAD_SAFE

#	ifdef DBSTBL_DEFAULT_DELAY
#		define DBSTBL__DEFAULT_DELAY	DBSTBL_DEFAULT_DELAY
#	else
#		define DBSTBL__DEFAULT_DELAY	100	// en ms.
#	endif

	class thread_safe_table_
	{
	private:
		lck::control___<table_> _Control;
		lck::control___<table_> &_C( void )
		{
			return _Control;
		}
		table_ &_Lock( void )
		{
			return _C().GetExclusiveAccess();
		}
		void _Release( void )
		{
			_C().ReleaseExclusiveAccess();
		}
	protected:
		virtual void DBSTBLErasePhysically( void );
	public:
		table_ Table;
		struct s
		{
			table_::s Table;
		} &S_;
		thread_safe_table_( s &S )
		: S_( S ),
		  Table( S.Table )
		{}
		void reset( bso::bool__ P = true )
		{
			Table.reset( P );
			_Control.reset( P );
		}
		void plug( mmm::E_MULTIMEMORY_ &MM )
		{
			Table.plug( MM );
		}
		thread_safe_table_ &operator =( const thread_safe_table_ &TST )
		{
			ERRl();

			return *this;	// Pour viter un warning.
		}
		void Init(
			const str::string_ &Label,
			dbsctt::content__ &Content,
			mode__ Mode )
		{
			reset();

			Table.Init( Label, Content, Mode );

			_Control.Init( Table );
		}
		void AddIndex( index_ &Index );
		rrow__ Insert( const datum_ &Datum );
		void Insert(
			const data_ &Data,
			rrows_ &RecordRows );
		void Update(
			const datum_ &Datum,
			rrow__ RecordRow );
		void Update(
			const data_ &Data,
			const rrows_ &RecordRows );
		void Retrieve(
			rrow__ Row,
			datum_ &Datum );
		void Retrieve(
			const rrows_ &Rows,
			data_ &Data );
		void Delete( rrow__ RecordRow );
		void Delete(
			const rrows_ &RecordRows );
#if 0
		rrow__ LooseSeek(
			const datum_ &Datum,
			irow__ IRow,
			behavior__ EqualBehavior,
			skip_level__ SkipLevel,
			bso::sign__ &Sign );
		rrow__ StrictSeek(
			const datum_ &Datum,
			irow__ IRow,
			behavior__ EqualBehavior,
			skip_level__ SkipLevel );
		bso::sign__ Compare(
			rrow__ RecordRow,
			const datum_&Pattern,
			irow__ IndexRow,
			skip_level__ SkipLevel );
		rrow__ First( irow__ IRow );
		rrow__ Last( irow__ IRow );
		rrow__ Next( 
			irow__ IRow,
			rrow__ Row );
		rrow__ GetStrictGreater( 
			irow__ IRow,
			rrow__ Row,
			skip_level__ SkipLevel );
		rrow__ Previous( 
			irow__ IRow,
			rrow__ Row );
#endif
		mdr::size__ Amount( void );
		mdr::size__ Extent( void );
		mode__ SwitchMode( mode__ Mode );
		mode__ Mode( void );
		bso::bool__ RecordExists( rrow__ RecordRow );
		// 'Rows' contient les position dans 'RecordRows' des enregistrement inexistants.
		void TestRecordsExistence(
			const rrows_ &RecordRows,
			rows_ &Rows );
#if 0
		void Reindex(
			irow__ IndexRow,
			observer_functions__ &Observer = *(observer_functions__ *)NULL );
#endif
		void ReindexAll( observer_functions__ &Observer = *(observer_functions__ *)NULL );
#if 0
		bso::bool__ IsIndexSynchronized( irow__ IndexRow );
#endif
		bso::bool__ AreAllIndexesSynchronized( void );
		void ErasePhysically( void )
		{
			DBSTBLErasePhysically();
		}
	};

	E_AUTO( thread_safe_table )

#	define	E_DBTABLE_	thread_safe_table_
#	define	E_DBTABLE	thread_safe_table
#else
#	define	E_DBTABLE_	table_
#	define	E_DBTABLE	table
#endif

	class exploded_table
	: public table
	{
	private:
		// Seulement l'un des deux est utilis.
		dbsdct::exploded_dynamic_content _Dynamic;
		dbssct::exploded_static_content _Static;
	protected:
		virtual void DBSTBLErasePhysically( void )
		{
			table::ErasePhysically();
			Content.ErasePhysically();
		}
	public:
		content__ Content;
		void InitStatic(
			epeios::size__ Size,
			const str::string_ &Path,
			const str::string_ &RootFileName,
			mode__ Mode,
			flm::id__ ID );
		void InitDynamic(
			const str::string_ &Path,
			const str::string_ &RootFileName,
			mode__ Mode,
			flm::id__ ID );
	};

	typedef exploded_table exploded_table_;


}

/*$END$*/
#endif
