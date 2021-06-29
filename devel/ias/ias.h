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

//	$Id: ias.h,v 1.6 2013/04/15 10:50:51 csimon Exp $

#ifndef IAS_INC_
# define IAS_INC_

# define IAS_NAME		"IAS"

# define	IAS_VERSION	"$Revision: 1.6 $"

# define IAS_OWNER		"Claude SIMON"

# if defined( E_DEBUG ) && !defined( IAS_NODBG )
#   define IAS_DBG
# endif

// Indexed Aggregated Storage

# include "err.h"
# include "flw.h"
# include "ags.h"
# include "tys.h"
# include "bch.h"


namespace ias {

	//t The type of an index in the indexed multimemory.
	E_ROW( index__ );

	// Prdclaration.
	class indexed_aggregated_storage_;

	typedef sdr::sStorageDriver sStorageDriver_;

	class _indexed_aggregated_storage_driver__
	: public sStorageDriver_
	{
	private:
		index__ _Index;
		const indexed_aggregated_storage_ *&_AStorage;
	protected:
		virtual void SDRAllocate( sdr::size__ Capacity )
		{
			qRFwk();
		}
		virtual sdr::size__ SDRSize( void ) const;
		// Dporte.
		virtual void SDRRecall(
			sdr::row_t__ Position,
			sdr::size__ Amount,
			sdr::byte__ *Buffer );
		virtual void SDRStore(
			const sdr::byte__ *Buffer,
			sdr::size__ Amount,
			sdr::row_t__ Position )
		{
			qRFwk();
		}
	public:
		_indexed_aggregated_storage_driver__( const indexed_aggregated_storage_ *&AStorage )
		: _AStorage( AStorage )
		{
			reset( false );
		}
		_indexed_aggregated_storage_driver__( indexed_aggregated_storage_ *&AStorage )
		: _AStorage( *(const indexed_aggregated_storage_ **)& AStorage )
		{
			reset( false );
		}
		void reset( bool P = true )
		{
			sStorageDriver_::reset( P );

			_Index = qNIL;
			_AStorage = NULL;
		}
		qVDTOR( _indexed_aggregated_storage_driver__ );
		//f Initialize with 'Multimemory' multimemory.
		void Init( void )
		{
			sStorageDriver_::Init();

			_Index = qNIL;
		}
		//f The 'Index' memory becomes the memory handled by this memory driver.
		void Index( index__ Index )
		{
			_Index = Index;
		}
		//f Return the index of the current memory.
		index__ Index( void ) const
		{
			return _Index;
		}
		// Dporte.
		friend class indexed_aggregated_storage_driver__;
	};

	//c This class is the standard memory driver for the indexed multimemory.
	class indexed_aggregated_storage_driver__
	: public _indexed_aggregated_storage_driver__
	{
	private:
		indexed_aggregated_storage_ *_AStorage;
		// memoire  laquelle il a t affect
	protected:
		// Dporte.
		virtual void SDRStore(
			const sdr::byte__ *Buffer,
			sdr::size__ Amount,
			sdr::row_t__ Position );
		// Dporte.
		virtual void SDRAllocate( sdr::size__ Capacity );
	public:
		indexed_aggregated_storage_driver__( void )
		: _indexed_aggregated_storage_driver__( _AStorage )
		{
			reset( false );
		}
		void reset( bool P = true )
		{
			_indexed_aggregated_storage_driver__::reset( P );
		}
		qVDTOR( indexed_aggregated_storage_driver__ );
		//f Initialize with the 'Multimemory' multimemory.
		void Init( indexed_aggregated_storage_ &AStorage )
		{
			_indexed_aggregated_storage_driver__::Init();
			_AStorage = &AStorage;
		}
		//f The 'Index' memory becomes the memory handled by this driver.
	};

	class const_indexed_aggregated_storage_driver__
	: public _indexed_aggregated_storage_driver__
	{
	private:
		const indexed_aggregated_storage_ *AStorage_;
		// memoire  laquelle il a t affect
	public:
		const_indexed_aggregated_storage_driver__( void )
		: _indexed_aggregated_storage_driver__( AStorage_ )
		{
			reset( false );
		}
		void reset( bool P = true )
		{
			_indexed_aggregated_storage_driver__::reset( P );
		}
		qVDTOR( const_indexed_aggregated_storage_driver__ );
		//f Initialize with the 'Multimemory' multimemory.
		void Init( const indexed_aggregated_storage_ &AStorage )
		{
			_indexed_aggregated_storage_driver__::Init();
			AStorage_ = &AStorage;
		}
		//f The 'Index' memory becomes the memory handled by this driver.
	};
}

namespace ias {

	using ags::descriptor__;

	typedef bch::E_BUNCHt_( descriptor__, index__ ) descriptors_;

# define IAS_BUFFER_SIZE	1024

	qHOOKS2( bch::sHook, Descriptors, ags::sHook, Storage );

	//c An indexed multimemory.
	class indexed_aggregated_storage_
	{
	private:
		void _Initialize(
			sdr::size__ CapaciteCourante,
			sdr::size__ NouvelleCapacite )
		{
# ifdef IAS_DBG
			if ( CapaciteCourante >= NouvelleCapacite )
				qRFwk();
# endif
			descriptor__ Buffer[IAS_BUFFER_SIZE];

			sdr::size__ Amount = NouvelleCapacite - CapaciteCourante;
			sdr::size__ HandledAmount = 0;

			memset( Buffer, -1, sizeof( descriptor__ ) * ( ( Amount  > IAS_BUFFER_SIZE ? IAS_BUFFER_SIZE : Amount ) ) );

			while ( Amount != HandledAmount ) {
				Descriptors.Store( Buffer, ( ( Amount - HandledAmount ) > IAS_BUFFER_SIZE ? IAS_BUFFER_SIZE : ( Amount - HandledAmount ) ), CapaciteCourante + HandledAmount );
				HandledAmount += ( ( Amount - HandledAmount ) > IAS_BUFFER_SIZE ? IAS_BUFFER_SIZE : ( Amount - HandledAmount ) );
			}
		}
		void _Free(
			sdr::size__ CapaciteCourante,
			sdr::size__ NouvelleCapacite )
		{
# ifdef IAS_DBG
			if ( CapaciteCourante <= NouvelleCapacite )
				qRFwk();
# endif
			while ( CapaciteCourante-- > NouvelleCapacite )
				AStorage.Free( Descriptors.Get( CapaciteCourante ) );
		}
	// fonctions
		void Lire_(
			index__ Index,
			sdr::row_t__ Position,
			sdr::size__ Taille,
			sdr::byte__ *Tampon ) const
		{
			AStorage.Read( Descriptors(*Index), Position, Taille, Tampon );
		}
		// place dans 'Tampon' 'Taille' octets,  partir de 'Position', de l'objet 'Index'
		void Ecrire_(
			const sdr::byte__ *Tampon,
			sdr::size__ Taille,
			index__ Index,
			sdr::row_t__ Position )
		{
			AStorage.Write( Tampon, Taille, Descriptors(*Index), Position );
		}
		// crit  'Position' de l'objet 'Index' 'Taille' octets de 'Tampon'
		void Allouer_(
			index__ Index,
			sdr::size__ Nombre )
		{
			descriptor__ D = Descriptors.Get( *Index );

			D = AStorage.Reallocate( D, Nombre );

//			D.Capacite = Nombre;

			Descriptors.Store( D, *Index );
		}
		// allocation pour 'Capacite' objets
		void Liberer_( index__ Index )
		{
			descriptor__ D = Descriptors.Get( *Index );

			AStorage.Free( D );

			D = AGS_UNDEFINED_DESCRIPTOR;
//			D.Capacite = 0;

			Descriptors.Store( D, *Index );
		}
		// libre la mmoire d'index 'Index'
	public:
		descriptors_ Descriptors;
		// les diffrents Descriptors
		ags::aggregated_storage_ AStorage;
		// la memoire gnrale
		struct s
		{
			descriptors_::s Descriptors;
			ags::aggregated_storage_::s AStorage;
		};
		indexed_aggregated_storage_( s &S )
		: Descriptors( S.Descriptors ),
		  AStorage( S.AStorage )
		{}
		// prparation
		void reset( bool P = true )
		{
			Descriptors.reset( P );
			AStorage.reset( P );
		}
		void plug( sHooks &Hooks )
		{
			Descriptors.plug( Hooks.Descriptors_ );
			AStorage.plug( Hooks.Storage_ );
		}
		void plug( qASd *AS )
		{
			Descriptors.plug( AS );
			AStorage.plug( AS );
		}
		indexed_aggregated_storage_ &operator =( const indexed_aggregated_storage_ &IAS )
		{
			qRFwk();

			return *this;
		}
		void Copy(
			const indexed_aggregated_storage_ &O,
			sdr::size__ Size )
		{
			Descriptors.Allocate( Size, aem::mFitted );
			Descriptors.Store( O.Descriptors, Size );
			AStorage = O.AStorage;
		}
	/*	void ecrire( flo_sortie_ &F ) const
		{
			Descriptors.ecrire( F );
			AStorage.ecrire( F );
		}
		void lire( flo_entree_ &F )
		{
			Descriptors.lire( F );
			AStorage.lire( F );
		}*/
		//f Initialization.
		void Init( void )
		{
			Descriptors.Init();
			AStorage.Init();
		}
		//f Allocate 'Capacity' memories in the indexed multimemory. 'ActualCapacity' is the actual capacity.
		void Allocate(
			sdr::size__ Amount,
			aem::mode__ Mode = aem::m_Default )
		{
			sdr::size__ CurrentAmount = Descriptors.Amount();

			if ( Amount > CurrentAmount ) {
				Descriptors.Allocate( Amount, Mode );
				_Initialize( CurrentAmount, Amount );
			} else if ( Amount < CurrentAmount ) {
				_Free( CurrentAmount, Amount );
				Descriptors.Allocate( Amount, Mode );
			} else
				Descriptors.Allocate( Amount, Mode );
		}
		void PreAllocate( sdr::size__ Amount )
		{
			sdr::size__ CurrentAmount = Descriptors.Amount();

			if ( Amount > CurrentAmount ) {
				Descriptors.PreAllocate( Amount );
				_Initialize( CurrentAmount, Amount );
			} else if ( Amount < CurrentAmount ) {
				_Free( CurrentAmount, Amount );
				Descriptors.PreAllocate( Amount );
			} else
				Descriptors.PreAllocate( Amount );
		}
		void SetStepped( sdr::size__ Step )
		{
			Descriptors.SetStepped( Step );

			// Pas de '_Initialize(...)' ou de '_Free(...)', car la taille ne change pas (juste ventuellement l''extent').
		}
		index__ New( index__ Index = qNIL )
		{
			descriptor__ D = AGS_UNDEFINED_DESCRIPTOR;

			if ( Index == qNIL )
				Index = Descriptors.New();
			else if ( !Descriptors.Exists( Index ) )
				qRFwk();

			Descriptors.Store( D, Index );

			return Index;
		}
		bso::bool__ Exists( index__ Index ) const
		{
			return Descriptors.Exists( Index );
		}
		//f Put 'Amount' bytes in 'Buffer' from the 'Index' memory at 'Position' .
		void Read(
			index__ Index,
			sdr::row_t__ Position,
			sdr::size__ Amount,
			sdr::byte__ *Buffer ) const
		{
			Lire_( Index, Position, Amount, Buffer );
		}
		//f Put 'Amount' bytes at 'Position' to the 'Index' memory from 'Buffer'.
		void Write(
			const sdr::byte__ *Buffer,
			sdr::size__ Amount,
			index__ Index,
			sdr::row_t__ Position )
		{
			Ecrire_( Buffer, Amount, Index, Position );
		}
		//f Allocate 'Size' bytes in the 'Index' memory.
		void Reallocate(
			index__ Index,
			sdr::size__ Size )
		{
			Allouer_( Index, Size );
		}
		//f Discard the 'Index' memory.
		void Free( index__ Index )
		{
			Liberer_( Index );
		}
		//f Return the size of the 'Index' memory.
		sdr::size__ Size( index__ Index ) const
		{
			descriptor__ D = Descriptors( *Index );

			if ( D != qNIL )
				return AStorage.Size( D );
			else
				return 0;
		}
		/*f Delete 'Amount' entries from 'Position',
		'ActualCapacity' is the actual capacity.
		No reallocationg to gain place is made. */
		void RemoveWithoutReallocating(
			sdr::row__ Position,
			sdr::size__ ActualCapacity,
			sdr::size__ Amount );
		void Shift( index__ Index )	// Shift all elements from 'Index' from one position to the end. Last element is lost.
		{
			descriptor__ Descriptor = AGS_UNDEFINED_DESCRIPTOR;
			Descriptors.Store( Descriptors, Descriptors.Amount() - 1 - *Index, *Index + 1, Index );
			Descriptors.Store( Descriptor, Index );
		}
		sdr::size__ Amount( void ) const
		{
			return Descriptors.Amount();
		}
	};

	E_AUTO( indexed_aggregated_storage )

# ifndef FLS_COMPILATION_

	template <typename descriptors, typename storage> class hH_
	: public sHooks
	{
	protected:
		descriptors Descriptors_;
		storage Storage_;
	public:
		void reset( bso::bool__ P = true )
		{
			Descriptors_.reset( P );
			Storage_.reset( P );
		}
		hH_( void )
		: sHooks( Descriptors_, Storage_ )
		{
			reset( false );
		}
	};


	class rRH
	: public hH_<bch::rRH, ags::rRH>
	{
	public:
		void Init( void )
		{
			Descriptors_.Init();
			Storage_.Init();
		}
	};

	struct rHF
	{
	public:
		bch::rHF Descriptors;
		ags::rHF Storage;
		void reset( bso::bool__ P = true )
		{
			Descriptors.reset( P );
			Storage.reset( P );
		}
		E_CDTOR( rHF );
		void Init(
			const fnm::name___ &Path,
			const fnm::name___ &Basename );	// Peut tre vide ('NULL') si 'Path' contient dj le nom de fichier.
	};

	class rFH
	: public hH_<bch::rFH, ags::rFH>
	{
	public:
		uys::eState Init(
			const rHF &Filenames,
			uys::mode__ Mode,
			uys::eBehavior Behavior,
			flsq::rId Id)
		{
			uys::eState State = Descriptors_.Init(Filenames.Descriptors, Mode, Behavior, Id);

			if ( !State.IsError() ) {
				if ( State != Storage_.Init(Filenames.Storage, Mode, Behavior, Id) )
					State = uys::sInconsistent;
			}

			return State;
		}
	};

# endif

	inline void _indexed_aggregated_storage_driver__::SDRRecall(
		sdr::row_t__ Position,
		sdr::size__ Amount,
		sdr::byte__ *Buffer )
	{
		_AStorage->Read( _Index, Position, Amount, Buffer );
	}

	inline sdr::size__ _indexed_aggregated_storage_driver__::SDRSize( void ) const
	{
		if ( _Index == qNIL )
			return 0;
		else
			return _AStorage->Size( _Index );
	}

	inline void indexed_aggregated_storage_driver__::SDRStore(
		const sdr::byte__ *Buffer,
		sdr::size__ Amount,
		sdr::row_t__ Position )
	{
		_AStorage->Write( Buffer, Amount, _Index, Position );
	}

	inline void indexed_aggregated_storage_driver__::SDRAllocate( sdr::size__ Capacity )
	{
		if ( _AStorage != NULL )
			_AStorage->Reallocate( _Index, Capacity );
	}
}

#endif
