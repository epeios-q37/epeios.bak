/*
	Header for the 'ndbidx' library by Claude SIMON (csimon at zeusw dot org)
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

//	$Id: ndbidx.h,v 1.29 2013/04/15 10:50:53 csimon Exp $

#ifndef NDBIDX__INC
#define NDBIDX__INC

#define NDBIDX_NAME		"NDBIDX"

#define	NDBIDX_VERSION	"$Revision: 1.29 $"

#define NDBIDX_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &NDBIDXTutor;

#if defined( E_DEBUG ) && !defined( NDBIDX_NODBG )
#define NDBIDX_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.29 $
//C Claude SIMON (csimon at zeusw dot org)
//R $Date: 2013/04/15 10:50:53 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D New DataBase InDeXes (will be moved on its own project in the future) 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "idxbtq.h"
#include "ndbbsc.h"
#include "ndbctt.h"

namespace ndbidx {
	using ndbbsc::datum_;
	using ndbbsc::datum;

	using ndbbsc::rrow__;

	using ndbbsc::rrows_;
	using ndbbsc::rrows;

	enum context__ {
		cIndexation,
		cSearch,
		c_amout,
		c_Undefined
	};

	class sort_function__
	{
	protected:
		virtual bso::sign__ NDBIDXCompare(
			const datum_ &Datum1,
			const datum_ &Datum2,
			context__ Context ) = 0;
	public:
		bso::sign__ Compare(
			const datum_ &Datum1,
			const datum_ &Datum2,
			context__ Context )
		{
			return NDBIDXCompare( Datum1, Datum2, Context );
		}
	};

	typedef idxbtq::E_INDEXt_( rrow__ ) _index_;
	E_AUTO( _index )

	struct extremities__
	{
		rrow__ Smallest, Greatest;
		bso::uint__ Used;
		extremities__( void )
		{
			Smallest = Greatest = E_NIL;
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
		// Dure entre deux appels en ms.
		bso::uint__ _Delay;
		bso::uint__ _HandledIndexAmount, _TotalIndexAmount;
	protected:
		virtual void NDBTBLNotify(
			bso::uint__ HandledIndexAmount,
			bso::uint__ TotalIndexAmount,
			bso::uint__ HandledRecordAmount,
			bso::uint__ TotalRecordAmount,
			bso::uint__ BalancingCount ) = 0;
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
		void Init( bso::uint__ Delay = 1000 )	// Dlai par dfaut : 1 s.
		{
			reset();

			_Delay = Delay;
		}
		void Set( bso::uint__ TotalIndexAmount )
		{
			_TotalIndexAmount = TotalIndexAmount;
			_HandledIndexAmount = 0;
		}
		void IncrementHandledIndexAmount( bso::uint__ Amount = 1 )
		{
			_HandledIndexAmount += Amount;
		}
		void Notify(
			bso::uint__ HandledRecordAmount,
			bso::uint__ TotalRecordAmount,
			bso::uint__ BalancingCount )
		{
			NDBTBLNotify( _HandledIndexAmount, _TotalIndexAmount, HandledRecordAmount, TotalRecordAmount, BalancingCount );
		}
		friend class index_;
	};

	struct post_initialization_function__
	{
	protected:
		virtual void NDBIDXCompleteInitialization( void ) = 0;
	public:
		void CompleteInitialization( void )
		{
			NDBIDXCompleteInitialization();
		}
	};

	class index_
	{
	protected:
	private:
		bso::bool__ _Bufferized;
		sort_function__ *_SortPointer;
		const ndbctt::content_ *_ContentPointer;
		time_t _ModificationEpochTimeStamp;
		mutable post_initialization_function__ *_PostInitializationFunction;
		_index_ &_Index( void )
		{
			if ( _Bufferized )
				return BIndex;
			else
				return DIndex;
		}
		const _index_ &_Index( void ) const
		{
			if ( _Bufferized )
				return BIndex;
			else
				return DIndex;
		}
		void _CompleteInitialization( void ) const
		{
			if ( _PostInitializationFunction != NULL ) {
				// On passe par un 'Buffer' pour viter un appel rcursif.
				post_initialization_function__ *Buffer = _PostInitializationFunction;
				_PostInitializationFunction = NULL;
				Buffer->CompleteInitialization();
			}
		}
		bso::sign__ _Seek(
			const datum_ &Data,
			context__ Context,
			behavior__ StopIfEqual,
			rrow__ &Row,
			bso::u8__ &Round,
			ndbctt::cache_ &Cache ) const;
		const ndbctt::content__ &_Content( void ) const
		{
			return _ContentPointer->operator()();
		}

		bso::bool__ _Retrieve(
			rrow__ Row,
			datum_ &Datum,
			ndbctt::cache_ &Cache ) const
		{
#if 1
			return _Content().Retrieve( Row, Datum );
#else
			_Content( true ).Retrieve( Row, Datum, Cache );
#endif
		}
		void _Touch( bso::bool__ CompareWithContent )
		{
			_ModificationEpochTimeStamp = tol::EpochTime( false );

			if ( CompareWithContent && ( _ModificationEpochTimeStamp == Content().ModificationEpochTimeStamp() ) )
				_ModificationEpochTimeStamp = tol::EpochTime( true );
		}
		rrow__ _SearchStrictLesser(
			rrow__ Row,
			context__ Context ) const;
		rrow__ _SearchStrictGreater(
			rrow__ Row,
			context__ Context ) const;
	public:
		_index BIndex;	// 'bufferized index'.
		_index_ DIndex;	// 'direct index'.
		struct s
		{
			_index_::s DIndex;
			rrow__ Root;
		} &S_;
		index_( s &S )
		: S_( S ),
		  DIndex( S.DIndex )
		{}
		void reset( bso::bool__ P = true )
		{
			BIndex.reset( P );
			DIndex.reset( P );
			S_.Root = E_NIL;
			_Bufferized = false;

			_SortPointer = NULL;
			_ContentPointer = NULL;

			_ModificationEpochTimeStamp = 0;
			_PostInitializationFunction = NULL;
		}
		E_VDTOR( index_ )	// Pour qu'un 'delete' sur cette classe appelle le destructeur de la classe hritante.
		void plug( ags::E_ASTORAGE_ &AS )
		{
			DIndex.plug( AS );
		}
		index_ &operator =( const index_ &I )
		{
			I._CompleteInitialization();
			_CompleteInitialization();

			S_.Root = I.S_.Root;
			BIndex = I.BIndex;
			DIndex = I.DIndex;
			_Bufferized = I._Bufferized;

/*			S_.Sort = I.S_.Sort;
			S_.Content = I.S_.Content;

*/			_ModificationEpochTimeStamp = I._ModificationEpochTimeStamp;

			return *this;
		}
		void Init(
			const ndbctt::content_ &Content = *(const ndbctt::content_ *)NULL,
			sort_function__ &Sort = *(sort_function__ *)NULL,
			post_initialization_function__ &PostInitializationFunction = *(post_initialization_function__ *)NULL )
		{
			BIndex.Init();
			DIndex.Init();
			S_.Root = E_NIL;

			_ContentPointer = &Content;
			_SortPointer = &Sort;

			_ModificationEpochTimeStamp = 0;
			_PostInitializationFunction = &PostInitializationFunction;
		}
		// Vide l'index.
		void Reset( void )
		{
			S_.Root = E_NIL;

			BIndex.Init();
			DIndex.Init();

			_Bufferized = false;

			_ModificationEpochTimeStamp = 0;
		}
		void Allocate(
			sdr::size__ Size,
			aem::mode__ Mode )
		{
			if ( _Bufferized )
				ERRFwk();

			_CompleteInitialization();
			DIndex.Allocate( Size, Mode );
		}
		bso::u8__ Index(
			rrow__ Row,
			extremities__ *Extremities,	// Si  'true', compare d'abord avec les extrmits. Utilis pour la rindexation.
			ndbctt::cache_ &Cache  = *(ndbctt:: cache_ *)NULL );	// Retourne le nombre de noeuds parcourus pour cette indexation.
		void Delete( rrow__ Row )
		{
			_CompleteInitialization();

#ifdef NDBIDX_DBG
			if ( S_.Root == E_NIL )
				ERRFwk();
#endif
			if ( _Bufferized )
				ERRFwk();

			S_.Root = DIndex.Delete( Row, S_.Root );

			_Touch( false );
		}
		rrow__ LooseSeek(
			const datum_ &Datum,
			behavior__ EqualBehavior,
			context__ Context,
			bso::sign__ &Sign ) const;	// Retourne l'lment le plus proche, mme si 
		rrow__ StrictSeek(
			const datum_ &Datum,
			behavior__ EqualBehavior,
			context__ Context ) const
		{
			bso::sign__ Sign;
			rrow__ Row = LooseSeek( Datum, EqualBehavior, Context, Sign );	// Procde au '_CompleteIntialization()'.

			switch ( Sign ) {
			case -1:
				if ( EqualBehavior == bStop )
					Row = E_NIL;
				else
					Row = Next( Row );
				break;
			case 0:
				break;
			case 1:
				if ( EqualBehavior == bStop )
					Row = E_NIL;
				else
					Row = Previous( Row );
				break;
			default:
				ERRFwk();
				break;
			}

			return Row;
		}
		bso::sign__ Compare(
			rrow__ RecordId,
			const datum_ &Pattern,
			context__ Context ) const;
		bso::sign__ Compare(
			const datum_ &Pattern,
			rrow__ RecordId,
			context__ Context ) const
		{
			return -Compare( RecordId, Pattern, Context );	// Procde au '_CompleteInitialization()'.
		}
		bso::sign__ Compare(
			rrow__ RecordRow1,
			rrow__ RecordRow2,
			context__ Context ) const;
		rrow__ SearchRoot( void ) const
		{	
			_CompleteInitialization();

			rrow__ Candidate = S_.Root = _Content().First();

			if ( Candidate != E_NIL ) 
				while ( ( Candidate = _Index().GetTreeParent( Candidate ) ) != E_NIL )
					S_.Root = Candidate;

			return S_.Root;
		}
		rrow__ First( void ) const
		{
			_CompleteInitialization();

			if ( S_.Root != E_NIL )
				return _Index().First( S_.Root );
			else
				return E_NIL;
		}
		rrow__ Last( void ) const
		{
			_CompleteInitialization();

			if ( S_.Root != E_NIL )
				return _Index().Last( S_.Root );
			else
				return E_NIL;
		}
		rrow__ Next( rrow__ Row ) const
		{
			_CompleteInitialization();

			return _Index().Next( Row );
		}
		rrow__ StrictLesser(
			rrow__ Row,
			context__ Context ) const
		{
			_CompleteInitialization();

			rrow__ Candidate = Previous( Row );

			if ( Candidate == E_NIL )
				return E_NIL;
			else if ( Compare( Row, Candidate, Context ) != 0 )
				return Candidate;
			else
				return _SearchStrictLesser( Row, Context );
		}
		rrow__ StrictGreater(
			rrow__ Row,
			context__ Context ) const
		{
			_CompleteInitialization();

			rrow__ Candidate = Next( Row );

			if ( Candidate == E_NIL )
				return E_NIL;
			else if ( Compare( Row, Candidate, Context ) != 0 )
				return Candidate;
			else
				return _SearchStrictGreater( Row, Context );
		}
		rrow__ Previous( rrow__ Row ) const
		{
			_CompleteInitialization();

			return _Index().Previous( Row );
		}
		sdr::size__ Amount( void ) const
		{
			_CompleteInitialization();

			return _Index().Amount();
		}
		bso::bool__ IsSynchronized( void ) const
		{
			return _ModificationEpochTimeStamp > _Content().ModificationEpochTimeStamp();
		}
		sort_function__ &SortFunction( void ) const
		{
			return *_SortPointer;
		}
		const ndbctt::content__ &Content( void ) const
		{
			return _Content();
		}
		void Balance( void )
		{
			if ( _Bufferized )
				ERRFwk();

			_CompleteInitialization();

			if ( S_.Root != E_NIL )
				S_.Root = DIndex.Balance( S_.Root );

			_Touch( false );
		}
		rrow__ CompareTreeAndQueue( void ) const
		{
			_CompleteInitialization();

			if ( S_.Root != E_NIL )
				return _Index().Compare( S_.Root );
			else
				return E_NIL;
		}
		void Bufferize( void )
		{
			if ( !_Bufferized ) {
				BIndex = DIndex;

				_Bufferized = true;
			}
		}
		rrow__ Test( void ) const;
		void Reindex( observer_functions__ &Observer );
		E_RODISCLOSE__( time_t, ModificationEpochTimeStamp );
	};

	E_AUTO( index )

	class files_hook___
	{
	private:
		index_ *_Index;
		str::string _BaseFileName;
		idxbtq::files_hook___ _Hook;
		fil::mode__ _Mode;
		void _ErasePhysically( void )
		{
			_Hook.Drop();
		}
	public:
		void reset( bso::bool__ P = true )
		{
			if ( P ) {
				Settle();
			}

			_Hook.reset( P );
			_Mode = fil::m_Undefined;
			_BaseFileName.reset( P );
			_Index = NULL;
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
			const str::string_ &BaseFileName,
			bso::bool__ Erase,
			fil::mode__ Mode,
			fls::id__ ID );
		void Set( index_ &Index )
		{
			if ( _Index != NULL )
				_Index = &Index;

			_Index = &Index;
		}
		uys::state__ Bind( void )
		{
			uys::state__ State = _Hook.Bind();

			if ( uys::IsError( State ) )
				return State;

			if ( State == uys::sExists )
				_Index->SearchRoot();

			return State;
		}
		uys::state__ Settle( void )
		{
			return _Hook.Settle();
		}
		void CloseFiles( void )
		{
			_Hook.ReleaseFiles();
		}
		const str::string_ &BaseFileName( void ) const
		{
			return _BaseFileName;
		}
		idxbtq::files_hook___ &FilesHook( void )
		{
			return _Hook;
		}
	};

	inline uys::state__ Plug(
		index_ &Index,
		files_hook___ &Hook )
	{
		uys::state__ State = idxbtq::Plug( Index.DIndex, Hook.FilesHook() );

		if ( uys::IsError( State ) ) {
			Hook.reset();
			return State;
		}

		Hook.Set( Index );

		return State;
	}
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
