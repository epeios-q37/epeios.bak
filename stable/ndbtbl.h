/*
	Header for the 'ndbtbl' library by Claude SIMON (csimon at zeusw dot org)
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

//	$Id: ndbtbl.h,v 1.25 2013/04/15 10:50:53 csimon Exp $

#ifndef NDBTBL__INC
#define NDBTBL__INC

#define NDBTBL_NAME		"NDBTBL"

#define	NDBTBL_VERSION	"$Revision: 1.25 $"

#define NDBTBL_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &NDBTBLTutor;

#if defined( E_DEBUG ) && !defined( NDBTBL_NODBG )
#define NDBTBL_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.25 $
//C Claude SIMON (csimon at zeusw dot org)
//R $Date: 2013/04/15 10:50:53 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

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
		// Pas d'indexation, ou diffèrée.
		mBulk,
		// Lecture seule.
		mReadOnly,
		// Lecture/écriture.
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
			ERRPrm();
			break;
		default:
			ERRPrm();
			break;
		}

		return fil::m_Undefined;	// Pour éviter un 'warning'.
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
				ERRPrm();
				break;
			case mReadWrite:
				switch ( S_.Mode ) {
				case mBulk:
				case mReadWrite:
					break;
				case mReadOnly:
					ERRFwk();
					break;
				default:
					ERRPrm();
					break;
				}
				break;
			case mReadOnly:
				break;
				switch ( S_.Mode ) {
				case mBulk:
					ERRFwk();
					break;
				case mReadWrite:
				case mReadOnly:
					break;
				default:
					ERRFwk();
					break;
				}
				break;
			default:
				ERRPrm();
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
				ERRPrm();
				break;
			default:
				ERRPrm();
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
				ERRPrm();
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
				ERRFwk();
				return false;	// Pour éviter un warning.
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
		E_VDTOR( table_ )	// Pour qu'un 'delete' sur cette classe appelle le destructeur de la classe héritante.
		void plug( ags::E_ASTORAGE_ &AS )
		{
			Content.plug( AS );
		}
		table_ &operator =( const table_ &T )
		{
			Content = T.Content;
			
			// _Indexes = T._Indexes;	// Ne contient que des pointeurs; les indexes correspondant doivent être copiés extèrieurement.
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

			/* Si l'on est dans le cas d'un primo-accés à l'index, celui-ci s'appuie sur le premier élément de 'Content()' pour trouver sa racine.
			Si 'Content()' n'en cotient plus qu'un seul, pour éviter de l'effacer avant initialisation de l'index,
			on efface l'élément de l'index avant de l'effacer de 'Content()'. */

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
						ERRFwk();
					break;
				case mReadOnly:
				case mReadWrite:
					break;
				default:
					ERRPrm();
					break;
				}
				break;
			default:
				ERRPrm();
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

	class table_atomized_file_manager___
	{
	private:
		type__ _Type;
		// Seulement l'un des deux est utilisé.
		ndbdct::dynamic_content_atomized_file_manager___ _Dynamic;
		ndbsct::static_content_atomized_file_manager___ _Static;
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
		table_atomized_file_manager___( void )
		{
			reset( false );
		}
		~table_atomized_file_manager___( void )
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
				ERRPrm();
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
				ERRFwk();
				break;
			}

			return uys::s_Undefined;	// Pour éviter un 'warning'.
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
				ERRFwk();
				break;
			}

			return uys::s_Undefined;	// Pour éviter un 'warning'.
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
				ERRPrm();
				break;
			}

			ERRFwk();

			return _Static.BaseFileName();	// Pour éviter un 'warning'.
		}
		friend uys::state__ Plug(
			table_ &Table,
			table_atomized_file_manager___ &FileManager );
	};

	inline uys::state__ Plug(
		table_ &Table,
		table_atomized_file_manager___ &FileManager )
	{
		uys::state__ State = uys::s_Undefined;

		switch ( Table.Type() ) {
		case tStatic:
			State = ndbsct::Plug( Table.Content.Static(), FileManager._Static );
			break;
		case tDynamic:
			State = ndbdct::Plug( Table.Content.Dynamic(), FileManager._Dynamic );
			break;
		default:
			ERRFwk();
			break;
		}

		return State;

	}



}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
