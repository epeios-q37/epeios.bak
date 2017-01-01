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

//	$Id: dbsidx.h,v 1.63 2009/11/15 11:30:58 csimon Exp $

#ifndef DBSIDX__INC
#define DBSIDX__INC

#define DBSIDX_NAME		"DBSIDX"

#define	DBSIDX_VERSION	"$Revision: 1.63 $"

#define DBSIDX_OWNER		"Claude SIMON (http://zeusw.org/intl/contact.html)"

#if defined( E_DEBUG ) && !defined( DBSIDX_NODBG )
#define DBSIDX_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.63 $
//C Claude SIMON (http://zeusw.org/intl/contact.html)
//R $Date: 2009/11/15 11:30:58 $

/* End of automatic documentation generation part. */

/* Addendum to the automatic documentation generation part. */
//D DataBaSe Index 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"
#include "idxbtq.h"
#include "dbsctt.h"
#include "dbsbsc.h"

namespace dbsidx {
	using dbsbsc::datum_;
	using dbsbsc::datum;

	using dbsbsc::rrow__;
	using dbsbsc::rrows_;

	using dbsbsc::skip_level__;

#define DBSIDX_NO_SKIP	DBSBSC_NO_SKIP

	class sort_function__
	{
	protected:
		virtual bso::sign__ DBSIDXCompare(
			const datum_ &Datum1,
			const datum_ &Datum2,
			skip_level__ SkipLevel ) = 0;	// Si == 0, la comparaison se fait sur tous les champs
	public:
		bso::sign__ Compare(
			const datum_ &Datum1,
			const datum_ &Datum2,
			skip_level__ SkipLevel )
		{
			return DBSIDXCompare( Datum1, Datum2, SkipLevel );
		}
	};

	typedef idxbtq::E_INDEXt_( rrow__ ) _index_;
	E_AUTO( _index )

	struct extremities__
	{
		rrow__ Smallest, Greatest;
		bso::ulong__ Used;
		extremities__( void )
		{
			Smallest = Greatest = NONE;
			Used = 0;
		}
	};

	// Pour idiquer le comportement d'un "seek' lorsqu'il tombe sur un lment qu est gal  celui recherch.
	enum behavior__ {
		// On stoppe la recherche.
		bStop,
		// On cherche le plus grand
		bGreater,
		// On cherche le plus petit
		bLesser,
		// On stoppe s'il manque l'un des fils. (sert lors d'une indexation).
		bStopIfOneChildMissing,
		b_amount,
		b_Undefined
	};

		class observer_functions__
	{
	private:
		// Dure entre deuw appels en ms.
		time_t _Delay;
		bso::ulong__ _HandledIndexAmount, _TotalIndexAmount;
	protected:
		virtual void DBSTBLNotify(
			bso::ulong__ HandledIndexAmount,
			bso::ulong__ TotalIndexAmount,
			bso::ulong__ HandledRecordAmount,
			bso::ulong__ TotalRecordAmount,
			bso::ulong__ BalancingCount ) = 0;
	public:
		void reset( bso::bool__ = true )
		{
			_Delay = 1000;	// Dlai par dfaut : 1 s.
			_TotalIndexAmount = _HandledIndexAmount = 0;
		}
		observer_functions__( void )
		{
			reset( false );
		}
		void Init( time_t Delay = 1000 )	// Dlai par dfaut : 1 s.
		{
			reset();

			_Delay = Delay;
		}
		void Set( bso::ulong__ TotalIndexAmount )
		{
			_TotalIndexAmount = TotalIndexAmount;
			_HandledIndexAmount = 0;
		}
		void IncrementHandledIndexAmount( bso::ulong__ Amount = 1 )
		{
			_HandledIndexAmount += Amount;
		}
		void Notify(
			bso::ulong__ HandledRecordAmount,
			bso::ulong__ TotalRecordAmount,
			bso::ulong__ BalancingCount )
		{
			DBSTBLNotify( _HandledIndexAmount, _TotalIndexAmount, HandledRecordAmount, TotalRecordAmount, BalancingCount );
		}
		friend class index_;
	};

	typedef dbsbsc::file_features_	_file_features_;

	class index_
	: public _file_features_
	{
	protected:
		virtual void DBSBSCCompleteInitialization( bso::bool__ )
		{
			// Rien  faire.
		}
		virtual void DBSBSCErasePhysically( void )
		{
			// Rien  faire.
		}
	private:
		bso::sign__ _Seek(
			const datum_ &Data,
			skip_level__ SkipLevel,
			behavior__ StopIfEqual,
			rrow__ &Row,
			bso::ubyte__ &Round,
			dbsctt::_cache_ &Cache ) const;
		const dbsctt::content__ &_Content( bso::bool__ CompleteInitializationIfNeeded ) const
		{
			if ( !S_.Content->InitializationCompleted() && CompleteInitializationIfNeeded )
				S_.Content->CompleteInitialization();

			return *S_.Content;
		}
		void _Retrieve(
			rrow__ Row,
			datum_ &Datum,
			dbsctt::_cache_ &Cache ) const
		{
			_Content( true ).Retrieve( Row, Datum, Cache );
		}
		void _Touch( bso::bool__ CompareWithContent )
		{
			S_.ModificationTimeStamp = tol::Clock( false );

			if ( CompareWithContent && ( S_.ModificationTimeStamp == Content( true ).ModificationTimeStamp() ) )
				S_.ModificationTimeStamp = tol::Clock( true );
		}
		rrow__ _SearchStrictGreater(
			rrow__ Row,
			skip_level__ SkipLevel ) const;
	public:
		_index_ BaseIndex;
		struct s
		: public _file_features_::s
		{
			_index_::s BaseIndex;
			rrow__ Root;
			sort_function__ *Sort;
			dbsctt::content__ *Content;
			time_t ModificationTimeStamp;
		} &S_;
		index_( s &S )
		: S_( S ),
		  _file_features_( S ),
		  BaseIndex( S.BaseIndex )
		{}
		void reset( bso::bool__ P = true )
		{
			BaseIndex.reset( P );
			S_.Root = NONE;

			S_.Sort = NULL;
			S_.Content = NULL;

			S_.ModificationTimeStamp = 0;

			_file_features_::reset( P );
		}
		E_VDTOR( index_ )	// Pour qu'un 'delete' sur cette classe appelle le destructeur de la classe hritante.
		void plug( mmm::E_MULTIMEMORY_ &MM )
		{
			BaseIndex.plug( MM );
		}
		index_ &operator =( const index_ &I )
		{
			BaseIndex = I.BaseIndex;
			S_.Root = I.S_.Root;

			S_.Sort = I.S_.Sort;
			S_.Content = I.S_.Content;

			S_.ModificationTimeStamp = I.S_.ModificationTimeStamp;

			return *this;
		}
		void Init(
			dbsctt::content__ &Content = *(dbsctt::content__ *)NULL,
			sort_function__ &Sort = *(sort_function__ *)NULL,
			bso::bool__ Partial = false )
		{
			BaseIndex.Init();
			S_.Root = NONE;

			S_.Content = &Content;
			S_.Sort = &Sort;

			S_.ModificationTimeStamp = 0;

			_file_features_::Init( Partial );
		}
		// Vide l'index.
		void Reset( void )
		{
			S_.Root = NONE;
			BaseIndex.Init();
			ErasePhysically();

			S_.ModificationTimeStamp = 0;
		}
		void Allocate(
			mdr::size__ Size,
			aem::mode__ Mode )
		{
			BaseIndex.Allocate( Size, Mode );
		}
		bso::ubyte__ Index(
			rrow__ Row,
			extremities__ *Extremities,	// Si  'true', compare d'abord avec les extrmits. Utilis pour la rindexation.
			dbsctt::_cache_ &Cache  = *(dbsctt:: _cache_ *)NULL );	// Retourne le nombre de noeuds parcourus pour cette indexation.
		void Delete( rrow__ Row )
		{
#ifdef DBSIDX_DBG
			if ( S_.Root == NONE )
				ERRu();
#endif
			S_.Root = BaseIndex.Delete( Row, S_.Root );

			_Touch( false );
		}
		rrow__ LooseSeek(
			const datum_ &Datum,
			behavior__ EqualBehavior,
			skip_level__ SkipLevel,
			bso::sign__ &Sign ) const;	// Retourne l'lment le plus proche, mme si 
		rrow__ StrictSeek(
			const datum_ &Datum,
			behavior__ EqualBehavior,
			skip_level__ SkipLevel ) const
		{
			bso::sign__ Sign;
			rrow__ Row = LooseSeek( Datum, EqualBehavior, SkipLevel, Sign );

			switch ( Sign ) {
			case -1:
				if ( EqualBehavior == bStop )
					Row = NONE;
				else
					Row = Next( Row );
				break;
			case 0:
				break;
			case 1:
				if ( EqualBehavior == bStop )
					Row = NONE;
				else
					Row = Previous( Row );
				break;
			default:
				ERRc();
				break;
			}

			return Row;
		}
		bso::sign__ Compare(
			rrow__ RecordId,
			const datum_ &Pattern,
			skip_level__ SkipLevel ) const;
		bso::sign__ Compare(
			const datum_ &Pattern,
			rrow__ RecordId,
			skip_level__ SkipLevel ) const
		{
			return -Compare( RecordId, Pattern, SkipLevel );
		}
		bso::sign__ Compare(
			rrow__ RecordRow1,
			rrow__ RecordRow2,
			skip_level__ SkipLevel ) const;
		rrow__ SearchRoot( rrow__ Member )
		{	
			S_.Root = Member;

			if ( Member != NONE ) 
				while ( ( Member = BaseIndex.GetTreeParent( Member ) ) != NONE )
					S_.Root = Member;

			return S_.Root;
		}
		rrow__ First( void ) const
		{
			if ( S_.Root != NONE )
				return BaseIndex.First( S_.Root );
			else
				return NONE;
		}
		rrow__ Last( void ) const
		{
			if ( S_.Root != NONE )
				return BaseIndex.Last( S_.Root );
			else
				return NONE;
		}
		rrow__ Next( rrow__ Row ) const
		{
			return BaseIndex.Next( Row );
		}
		rrow__ StrictGreater(
			rrow__ Row,
			skip_level__ SkipLevel ) const
		{
			rrow__ Candidate = Next( Row );

			if ( Candidate == NONE )
				return NONE;
			else if ( Compare( Row, Candidate, SkipLevel ) != 0 )
				return Candidate;
			else
				return _SearchStrictGreater( Row, SkipLevel );
		}
		rrow__ Previous( rrow__ Row ) const
		{
			return BaseIndex.Previous( Row );
		}
		mdr::size__ Amount( void ) const
		{
			return BaseIndex.Amount();
		}
		bso::bool__ IsSynchronized( void ) const
		{
			return S_.ModificationTimeStamp > _Content( false ).ModificationTimeStamp();
		}
		sort_function__ &SortFunction( void ) const
		{
			return *S_.Sort;
		}
		const dbsctt::content__ &Content( bso::bool__ CompleteInitializationIfNeeded ) const
		{
			return _Content( CompleteInitializationIfNeeded );
		}
		void Balance( void )
		{
			if ( S_.Root != NONE )
				S_.Root = BaseIndex.Balance( S_.Root );

			_Touch( false );
		}
		rrow__ CompareTreeAndQueue( void ) const
		{
			if ( S_.Root != NONE )
				return BaseIndex.Compare( S_.Root );
			else
				return NONE;
		}
		rrow__ Test( void ) const;
		void Reindex( observer_functions__ &Observer );
		E_RODISCLOSE_( time_t, ModificationTimeStamp );
	};

	E_AUTO( index )

	// 'index' stocks dans des fichiers.

	class exploded_index_
	: public index_
	{
	protected:
		virtual void DBSBSCCompleteInitialization( bso::bool__ IgnoreAdditionalFiles )
		{
			_ConnectToFiles();
		}
		virtual void DBSBSCErasePhysically( void )
		{
			index_::DBSBSCErasePhysically();
			_ErasePhysically();
		}
		time_t _GetUnderlyingFilesLastModificationTime( void ) const
		{
			return S_.FileManager.TimeStamp();
		}
	private:
//		void _SaveRoot( void ) const;
		bso::bool__ _ConnectToFiles()
		{
			if ( idxbtq::Connect( BaseIndex, S_.FileManager ) ) {
				index_::SearchRoot( Content( true ).First() );
				return true;
			} else {
				return false;
			}
		}
		void _ErasePhysically( void )
		{
			S_.FileManager.Drop();

//			dbsbsc::DropFile( RootFileName, ROOT_FILE_NAME_EXTENSION );
		}
	public:
		str::string_ RootFileName;
		struct s
		: public index_::s
		{
			idxbtq::index_file_manager___ FileManager;
			str::string_::s RootFileName;
			bso::bool__ Erase;	// Seulement utile lors d'uen initialisation retarde.
			mdr::mode__ Mode;
		} &S_;
		exploded_index_( s &S )
		: S_( S ), 
		  index_( S ),
		  RootFileName( S.RootFileName )
		{}
		void reset( bso::bool__ P = true )
		{

			S_.FileManager.ReleaseFiles();
/*
			if ( P ) {
				if ( ( RootFileName.Amount() != 0 ) && ( ModificationTimeStamp() != 0 ) )
					_SaveRoot();
			}
*/
			S_.FileManager.reset( P );
			index_::reset( P );
			S_.Erase = false;
			S_.Mode = mdr::m_Undefined;
			RootFileName.reset( P );
		}
		void plug( mmm::E_MULTIMEMORY_ & )
		{
			ERRu();	// Cette mthode n'a pas de sens dans ce contexte.
		}
		exploded_index_ &operator =( const exploded_index_ &XI )
		{
			index_::operator =( XI );

			return *this;
		}
		void Init(
			const str::string_ &RootFileName,
			dbsctt::content__ &Content,
			sort_function__ &Sort,
			mdr::mode__ Mode,
			bso::bool__ Erase,
			bso::bool__ Partial,
			flm::id__ ID );
		void CloseFiles( void )
		{
			S_.FileManager.ReleaseFiles();
		}
	};

	E_AUTO( exploded_index )




}

/*$END$*/
#endif
