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

//	$Id: mmi.h,v 1.63 2013/03/15 14:11:53 csimon Exp $

#ifndef MMI__INC
#define MMI__INC

#define MMI_NAME		"MMI"

#define	MMI_VERSION	"$Revision: 1.63 $"

#define MMI_OWNER		"Claude SIMON (http://zeusw.org/intl/contact.html)"


#if defined( E_DEBUG ) && !defined( MMI_NODBG )
#define MMI_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.63 $
//C Claude SIMON (http://zeusw.org/intl/contact.html)
//R $Date: 2013/03/15 14:11:53 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D MultiMemory Index 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

# error "Obsolete ! Use 'IAS' instead !"

#include "err.h"
#include "flw.h"
#include "mmm.h"
#include "tym.h"

namespace mmi {

	//t The type of an index in the indexed multimemory.
	E_ROW( index__ );

	// Prdclaration.
	class indexed_multimemory_;

	class _base_indexed_multimemory_driver__
	: public mdr::E_MEMORY_DRIVER__
	{
	private:
		index__ _Index;
		const indexed_multimemory_ *&Multimemoire_;
	protected:
		virtual void MDRAllocate( mdr::size__ Capacity )
		{
			ERRc();
		}
		virtual mdr::size__ MDRUnderlyingSize( void )
		{
			if ( Multimemoire_ != NULL )
				return Size();
			else
				return 0;
		}
		// Dporte.
		virtual void MDRRecall(
			mdr::row_t__ Position,
			mdr::size__ Amount,
			mdr::datum__ *Buffer );
		virtual void MDRStore(
			const mdr::datum__ *Buffer,
			mdr::size__ Amount,
			mdr::row_t__ Position )
		{
			ERRc();
		}
	public:
		_base_indexed_multimemory_driver__( const indexed_multimemory_ *&Multimemoire )
		: Multimemoire_( Multimemoire )
		{}
		_base_indexed_multimemory_driver__( indexed_multimemory_ *&Multimemoire )
		: Multimemoire_( *(const indexed_multimemory_ **)& Multimemoire )
		{}
		void reset( bool P = true )
		{
			E_MEMORY_DRIVER__::reset( P );

			_Index = NONE;
			Multimemoire_ = NULL;
		}
		//f Initialize with 'Multimemory' multimemory.
		void Init( void )
		{
			E_MEMORY_DRIVER__::Init();

			_Index = NONE;
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
		mdr::size__ Size( void ) const;
		friend class indexed_multimemory_driver__;
	};

	//c This class is the standard memory driver for the indexed multimemory.
	class indexed_multimemory_driver__
	: public _base_indexed_multimemory_driver__
	{
	private:
		indexed_multimemory_ *Multimemoire_;
		// memoire  laquelle il a t affect
	protected:
		// Dporte.
		virtual void MDRStore(
			const mdr::datum__ *Buffer,
			mdr::size__ Amount,
			mdr::row_t__ Position );
		// Dporte.
		virtual void MDRAllocate( mdr::size__ Capacity );
	public:
		indexed_multimemory_driver__( void )
		: _base_indexed_multimemory_driver__( Multimemoire_ ) {}
		void reset( bool P = true )
		{
			_base_indexed_multimemory_driver__::reset( P );
		}
		//f Initialize with the 'Multimemory' multimemory.
		void Init( indexed_multimemory_ &Multimemory )
		{
			_base_indexed_multimemory_driver__::Init();
			Multimemoire_ = &Multimemory;
		}
		//f The 'Index' memory becomes the memory handled by this driver.
	};

	//c Same as 'mmmi_indexed_multimemory_driver_', but for read-only memory.
	class const_indexed_multimemory_driver__
	: public _base_indexed_multimemory_driver__
	{
	private:
		const indexed_multimemory_ *Multimemoire_;
		// memoire  laquelle il a t affect
	public:
		const_indexed_multimemory_driver__( void	)
		: _base_indexed_multimemory_driver__( Multimemoire_ ) {}
		void reset( bool P = true )
		{
			_base_indexed_multimemory_driver__::reset( P );
		}
		//f Initialize with the 'Multimemory' multimemory.
		void Init( const indexed_multimemory_ &Multimemory )
		{
			_base_indexed_multimemory_driver__::Init();
			Multimemoire_ = &Multimemory;
		}
		//f The 'Index' memory becomes the memory handled by this driver.
	};

	struct descripteur__
	{
		mmm::descriptor__ Descripteur;
		bso::ubyte__ Addendum;
	};
}

#include "bch.h"

namespace mmi {

	typedef bch::E_BUNCHt_( descripteur__, index__ ) descriptors_;

#define MMI_BUFFER_SIZE	1024

	//c An indexed multimemory.
	class indexed_multimemory_
	{
	private:
		void Initialize_(
			mdr::size__ CapaciteCourante,
			mdr::size__ NouvelleCapacite )
		{
#if 1
			descripteur__ Buffer[MMI_BUFFER_SIZE];

			mdr::size__ Amount = NouvelleCapacite - CapaciteCourante;
			mdr::size__ HandledAmount = 0;

			memset( Buffer, MMM_UNDEFINED_DESCRIPTOR, sizeof( descripteur__ ) * ( ( Amount  > MMI_BUFFER_SIZE ? MMI_BUFFER_SIZE : Amount ) ) );

			while ( Amount != HandledAmount ) {
				Descripteurs.Store( Buffer, ( ( Amount - HandledAmount ) > MMI_BUFFER_SIZE ? MMI_BUFFER_SIZE : ( Amount - HandledAmount ) ), CapaciteCourante + HandledAmount );
				HandledAmount += ( ( Amount - HandledAmount ) > MMI_BUFFER_SIZE ? MMI_BUFFER_SIZE : ( Amount - HandledAmount ) );
			}

#else
			descripteur__ D;
			D.Descripteur = MMM_UNDEFINED_DESCRIPTOR;
			D.Capacite = 0;
			Descripteurs.Store( D, CapaciteCourante, NouvelleCapacite - CapaciteCourante );
#endif
		}
	// fonctions
		void AllouerPlus_(
			mdr::size__ CapaciteCourante,
			mdr::size__ NouvelleCapacite,
			aem::mode__ Mode )
		{
			Descripteurs.Allocate( NouvelleCapacite, Mode );

			Initialize_( CapaciteCourante, NouvelleCapacite );
		}
		// alloue plus de la place pour pouvoir contenir 'NouvelleCapacite' objets,
		// sachant que 'Capacite courante' est la capacite actuelle
		void AllouerMoins_(
			mdr::size__ CapaciteCourante,
			mdr::size__ NouvelleCapacite,
			aem::mode__ Mode );
		// alloue plus de la place pour pouvoir contenir 'NouvelleCapacite' objets,
		// sachant que 'Capacite courante' est la capacite actuelle
		void Lire_(
			index__ Index,
			mdr::row_t__ Position,
			mdr::size__ Taille,
			mdr::datum__ *Tampon ) const
		{
			Multimemoire.Read( Descripteurs(*Index).Descripteur, Position, Taille, Tampon, Descripteurs(*Index).Addendum );
		}
		// place dans 'Tampon' 'Taille' octets,  partir de 'Position', de l'objet 'Index'
		void Ecrire_(
			const mdr::datum__ *Tampon,
			mdr::size__ Taille,
			index__ Index,
			mdr::row_t__ Position )
		{
			Multimemoire.Write( Tampon, Taille, Descripteurs(*Index).Descripteur, Position, Descripteurs(*Index).Addendum );
		}
		// crit  'Position' de l'objet 'Index' 'Taille' octets de 'Tampon'
		void Allouer_(
			index__ Index,
			mdr::size__ Nombre )
		{
			descripteur__ D = Descripteurs.Get( *Index );

			D.Descripteur = Multimemoire.Reallocate( D.Descripteur, Nombre, D.Addendum );

//			D.Capacite = Nombre;

			Descripteurs.Store( D, *Index );
		}
		// allocation pour 'Capacite' objets
		void Liberer_( index__ Index )
		{
			descripteur__ D = Descripteurs.Get( *Index );

			Multimemoire.Free( D.Descripteur );

			D.Descripteur = MMM_UNDEFINED_DESCRIPTOR;
//			D.Capacite = 0;

			Descripteurs.Store( D, *Index );
		}
		// libre la mmoire d'index 'Index'
	public:
		descriptors_ Descripteurs;
		// les diffrents descripteurs
		mmm::multimemory_ Multimemoire;
		// la memoire gnrale
		struct s
		{
			descriptors_::s Descripteurs;
			mmm::multimemory_::s Multimemoire;
		};
		indexed_multimemory_( s &S )
		: Descripteurs( S.Descripteurs ),
		  Multimemoire( S.Multimemoire )
		{}
		// prparation
		void reset( bool P = true )
		{
			Descripteurs.reset( P );
			Multimemoire.reset( P );
		}
		void plug( mmm::multimemory_ &M )
		{
			Descripteurs.plug( M );
			this->Multimemoire.plug( M );
		}
		indexed_multimemory_ &operator =( const indexed_multimemory_ &O )
		{
			ERRc();

			return *this;
		}
		void Copy(
			const indexed_multimemory_ &O,
			mdr::size__ Size )
		{
			Descripteurs.Allocate( Size, aem::mFit );
			Descripteurs.Store( O.Descripteurs, Size );
			Multimemoire = O.Multimemoire;
		}
	/*	void ecrire( flo_sortie_ &F ) const
		{
			Descripteurs.ecrire( F );
			Multimemoire.ecrire( F );
		}
		void lire( flo_entree_ &F )
		{
			Descripteurs.lire( F );
			Multimemoire.lire( F );
		}*/
		//f Initialization.
		void Init( void )
		{
			Descripteurs.Init();
			Multimemoire.Init();
		}
		//f Allocate 'Capacity' memories in the indexed multimemory. 'ActualCapacity' is the actual capacity.
		void Allocate(
			mdr::size__ Amount,
			aem::mode__ Mode = aem::m_Default )
		{
			mdr::size__ CurrentAmount = Descripteurs.Amount();

			if ( CurrentAmount > Amount )
				AllouerMoins_( CurrentAmount, Amount, Mode );
			else
				AllouerPlus_( CurrentAmount, Amount, Mode );
		}
		index__ New( index__ Index = NONE )
		{
			descripteur__ D = { MMM_UNDEFINED_DESCRIPTOR, 0 };

			if ( Index == NONE )
				Index = Descripteurs.New();
			else if ( !Descripteurs.Exists( Index ) )
				ERRc();

			Descripteurs.Store( D, Index );

			return Index;
		}
		bso::bool__ Exists( index__ Index ) const
		{
			return Descripteurs.Exists( Index );
		}
		//f Put 'Amount' bytes in 'Buffer' from the 'Index' memory at 'Position' .
		void Read(
			index__ Index,
			mdr::row_t__ Position,
			mdr::size__ Amount,
			mdr::datum__ *Buffer ) const
		{
			Lire_( Index, Position, Amount, Buffer );
		}
		//f Put 'Amount' bytes at 'Position' to the 'Index' memory from 'Buffer'.
		void Write(
			const mdr::datum__ *Buffer,
			mdr::size__ Amount,
			index__ Index,
			mdr::row_t__ Position )
		{
			Ecrire_( Buffer, Amount, Index, Position );
		}
		//f Allocate 'Size' bytes in the 'Index' memory.
		void Reallocate(
			index__ Index,
			mdr::size__ Size )
		{
			Allouer_( Index, Size );
		}
		//f Discard the 'Index' memory.
		void Free( index__ Index )
		{
			Liberer_( Index );
		}
		//f Return the size of the 'Index' memory.
		mdr::size__ Size( index__ Index ) const
		{
			mmm::descriptor__ D = Descripteurs( *Index ).Descripteur;

			if ( D != NONE )
				return Multimemoire.Size( D );
			else
				return 0;
		}
		/*f Delete 'Amount' entries from 'Position',
		'ActualCapacity' is the actual capacity.
		No reallocationg to gain place is made. */
		void RemoveWithoutReallocating(
			mdr::row__ Position,
			mdr::size__ ActualCapacity,
			mdr::size__ Amount );
		void Shift( index__ Index )	// Shift all elements from 'Index' from one position to the end. Last element is lost.
		{
			descripteur__ Descripteur = {NONE, 0};
			Descripteurs.Store( Descripteurs, Descripteurs.Amount() - 1 - *Index, *Index + 1, Index );
			Descripteurs.Store( Descripteur, Index );
		}
#if 0
		void write(
			flw::oflow__ &OFlow,
			mdr::size__ Size) const
		{
			Descripteurs.write( 0, Size, OFlow );
			Multimemoire.write( OFlow );
		}
		void read(
			flw::iflow__ &IFlow,
			mdr::size__ Size )
		{
			Descripteurs.read( IFlow, 0, Size );
			Multimemoire.read( IFlow );
		}
#endif
	};

	E_AUTO( indexed_multimemory )

#ifndef FLM__COMPILATION

	class indexed_multimemory_file_manager___
	{
	private:
		bch::bunch_file_manager___ _Descriptors;
		mmm::multimemory_file_manager___ _Multimemory;
	public:
		void reset( bso::bool__ P = true )
		{
			_Descriptors.reset( P );
			_Multimemory.reset( P );
		}
		indexed_multimemory_file_manager___( void )
		{
			reset( false );
		}
		~indexed_multimemory_file_manager___( void )
		{
			reset();
		}
		void Init( 
			const char *DescriptorsFileName,
			const char *MultimemoryFileName,
			const char *MultimemoryFreeFragmentPositionsFileName,
			fil::mode__ Mode,
			bso::bool__ Persistent,
			flm::id__ ID )
		{
			_Descriptors.Init( DescriptorsFileName, Mode, Persistent, ID );
			_Multimemory.Init( MultimemoryFileName, MultimemoryFreeFragmentPositionsFileName, Mode, Persistent, ID );
		}
		fil::mode__ Mode( fil::mode__ Mode )
		{
			fil::mode__ ModeBuffer = _Descriptors.Mode( Mode );

			if ( ModeBuffer != _Multimemory.Mode( Mode ) )
				ERRc();

			return ModeBuffer;
		}
		fil::mode__ Mode( void ) const
		{
			fil::mode__ Mode = _Descriptors.Mode();

			if ( Mode != _Multimemory.Mode() )
				ERRc();

			return Mode;
		}
		uym::state__ State( void ) const
		{
			uym::state__ State = _Descriptors.State();

			if ( !uym::IsError( State ) )
				if ( State != _Multimemory.State() )
					State = uym::sInconsistent;

			return State;
		}
		uym::state__ Bind( void )	// A appeler seulement aprs 'Plug(...)'.
		{
			uym::state__ State = _Descriptors.Bind();

			if ( !uym::IsError( State ) )
				if ( State != _Multimemory.Bind() )
					State = uym::sInconsistent;

			return State;
		}
		uym::state__ Settle( void )
		{
			uym::state__ State = _Descriptors.Settle();

			if ( !uym::IsError( State ) )
				if ( State != _Multimemory.Settle() )
					State = uym::sInconsistent;

			return State;
		}
		void ReleaseFile( void )
		{
			_Descriptors.ReleaseFile();
			_Multimemory.ReleaseFile();
		}
		bso::bool__ IsPersistent( void ) const
		{
#ifdef MMI_DBG
			if ( _Descriptors.IsPersistent() != _Multimemory.IsPersistent() )
				ERRc();
#endif
			return _Descriptors.IsPersistent();
		}
		void Drop( void )
		{
			_Descriptors.Drop();
			_Multimemory.Drop();
		}
		tym::memory_file_manager___ &DescriptorsFileManager( void )
		{
			return _Descriptors;
		}
		mmm::multimemory_file_manager___ &MultimemoryFileManager( void )
		{
			return _Multimemory;
		}
		bso::bool__ Exists( void ) const
		{
			bso::bool__ Exists = _Descriptors.Exists();

			if ( Exists != _Multimemory.Exists() )
				ERRc();

			return Exists;
		}
		time_t TimeStamp( void ) const
		{
			time_t DescriptorsTimeStamp, MultimemoryTimeStamp;

			DescriptorsTimeStamp = _Descriptors.TimeStamp();
			MultimemoryTimeStamp = _Multimemory.TimeStamp();

			return ( DescriptorsTimeStamp > MultimemoryTimeStamp ? DescriptorsTimeStamp : MultimemoryTimeStamp );
		}
#ifdef CPE__VC
#	undef CreateFile
#endif
		bso::bool__ CreateFiles( err::handling__ ErrorHandling )
		{
			bso::bool__ Success = _Descriptors.CreateFile( ErrorHandling );

			if ( !Success )
				return false;

			Success = _Multimemory.CreateFile( ErrorHandling );

			return Success;
		}
	};

	inline uym::state__ Plug(
		indexed_multimemory_ &Memory,
		indexed_multimemory_file_manager___ &FileManager )
	{
		uym::state__ State = bch::Plug( Memory.Descripteurs, FileManager.DescriptorsFileManager() );

		if ( uym::IsError( State ) )
			FileManager.reset();
		else
			if ( mmm::Plug( Memory.Multimemoire, FileManager.MultimemoryFileManager() ) != State ) {
				State = uym::sInconsistent;
				FileManager.reset();
			}

		return State;
	}

#endif

	inline void _base_indexed_multimemory_driver__::MDRRecall(
		mdr::row_t__ Position,
		mdr::size__ Amount,
		mdr::datum__ *Buffer )
	{
		Multimemoire_->Read( _Index, Position, Amount, Buffer );
	}

	inline mdr::size__ _base_indexed_multimemory_driver__::Size( void ) const
	{
		if ( _Index == NONE )
			return 0;
		else
			return Multimemoire_->Size( _Index );
	}

	inline void indexed_multimemory_driver__::MDRStore(
		const mdr::datum__ *Buffer,
		mdr::size__ Amount,
		mdr::row_t__ Position )
	{
		Multimemoire_->Write( Buffer, Amount, _Index, Position );
	}

	inline void indexed_multimemory_driver__::MDRAllocate( mdr::size__ Capacity )
	{
		Multimemoire_->Reallocate( _Index, Capacity );
	}
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
