/*
	Header for the 'ias' library by Claude SIMON (csimon at zeusw dot org)
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

//	$Id: ias.h,v 1.6 2013/04/15 10:50:51 csimon Exp $

#ifndef IAS__INC
#define IAS__INC

#define IAS_NAME		"IAS"

#define	IAS_VERSION	"$Revision: 1.6 $"

#define IAS_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &IASTutor;

#if defined( E_DEBUG ) && !defined( IAS_NODBG )
#define IAS_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.6 $
//C Claude SIMON (csimon at zeusw dot org)
//R $Date: 2013/04/15 10:50:51 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D Indexed Aggregated Storage 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"
#include "ags.h"
#include "tys.h"

namespace ias {

	//t The type of an index in the indexed multimemory.
	E_ROW( index__ );

	// Prédéclaration.
	class indexed_aggregated_storage_;

	class _indexed_aggregated_storage_driver__
	: public sdr::E_SDRIVER__
	{
	private:
		index__ _Index;
		const indexed_aggregated_storage_ *&_AStorage;
	protected:
		virtual void SDRAllocate( sdr::size__ Capacity )
		{
			ERRFwk();
		}
		virtual sdr::size__ SDRUnderlyingSize( void )
		{
			if ( _AStorage != NULL )
				return Size();
			else
				return 0;
		}
		// Déportée.
		virtual void SDRRecall(
			sdr::row_t__ Position,
			sdr::size__ Amount,
			sdr::datum__ *Buffer );
		virtual void SDRStore(
			const sdr::datum__ *Buffer,
			sdr::size__ Amount,
			sdr::row_t__ Position )
		{
			ERRFwk();
		}
	public:
		_indexed_aggregated_storage_driver__( const indexed_aggregated_storage_ *&AStorage )
		: _AStorage( AStorage )
		{}
		_indexed_aggregated_storage_driver__( indexed_aggregated_storage_ *&AStorage )
		: _AStorage( *(const indexed_aggregated_storage_ **)& AStorage )
		{}
		void reset( bool P = true )
		{
			E_SDRIVER__::reset( P );

			_Index = E_NIL;
			_AStorage = NULL;
		}
		//f Initialize with 'Multimemory' multimemory.
		void Init( void )
		{
			E_SDRIVER__::Init();

			_Index = E_NIL;
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
		// Déportée.
		sdr::size__ Size( void ) const;
		friend class indexed_aggregated_storage_driver__;
	};

	//c This class is the standard memory driver for the indexed multimemory.
	class indexed_aggregated_storage_driver__
	: public _indexed_aggregated_storage_driver__
	{
	private:
		indexed_aggregated_storage_ *_AStorage;
		// memoire à laquelle il a été affecté
	protected:
		// Déportée.
		virtual void SDRStore(
			const sdr::datum__ *Buffer,
			sdr::size__ Amount,
			sdr::row_t__ Position );
		// Déportée.
		virtual void SDRAllocate( sdr::size__ Capacity );
	public:
		indexed_aggregated_storage_driver__( void )
		: _indexed_aggregated_storage_driver__( _AStorage ) {}
		void reset( bool P = true )
		{
			_indexed_aggregated_storage_driver__::reset( P );
		}
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
		// memoire à laquelle il a été affecté
	public:
		const_indexed_aggregated_storage_driver__( void )
		: _indexed_aggregated_storage_driver__( AStorage_ ) {}
		void reset( bool P = true )
		{
			_indexed_aggregated_storage_driver__::reset( P );
		}
		//f Initialize with the 'Multimemory' multimemory.
		void Init( const indexed_aggregated_storage_ &AStorage )
		{
			_indexed_aggregated_storage_driver__::Init();
			AStorage_ = &AStorage;
		}
		//f The 'Index' memory becomes the memory handled by this driver.
	};
}

#include "bch.h"

namespace ias {

	using ags::descriptor__;

	typedef bch::E_BUNCHt_( descriptor__, index__ ) descriptors_;

#define IAS_BUFFER_SIZE	1024

	//c An indexed multimemory.
	class indexed_aggregated_storage_
	{
	private:
		void Initialize_(
			sdr::size__ CapaciteCourante,
			sdr::size__ NouvelleCapacite )
		{
#if 1
			descriptor__ Buffer[IAS_BUFFER_SIZE];

			sdr::size__ Amount = NouvelleCapacite - CapaciteCourante;
			sdr::size__ HandledAmount = 0;

			memset( Buffer, -1, sizeof( descriptor__ ) * ( ( Amount  > IAS_BUFFER_SIZE ? IAS_BUFFER_SIZE : Amount ) ) );

			while ( Amount != HandledAmount ) {
				Descriptors.Store( Buffer, ( ( Amount - HandledAmount ) > IAS_BUFFER_SIZE ? IAS_BUFFER_SIZE : ( Amount - HandledAmount ) ), CapaciteCourante + HandledAmount );
				HandledAmount += ( ( Amount - HandledAmount ) > IAS_BUFFER_SIZE ? IAS_BUFFER_SIZE : ( Amount - HandledAmount ) );
			}

#else
			descriptor__ D;
			D.Descriptor = MMM_UNDEFINED_DESCRIPTOR;
			D.Capacite = 0;
			Descriptors.Store( D, CapaciteCourante, NouvelleCapacite - CapaciteCourante );
#endif
		}
	// fonctions
		void AllouerPlus_(
			sdr::size__ CapaciteCourante,
			sdr::size__ NouvelleCapacite,
			aem::mode__ Mode )
		{
			Descriptors.Allocate( NouvelleCapacite, Mode );

			Initialize_( CapaciteCourante, NouvelleCapacite );
		}
		// alloue plus de la place pour pouvoir contenir 'NouvelleCapacite' objets,
		// sachant que 'Capacite courante' est la capacite actuelle
		void AllouerMoins_(
			sdr::size__ CapaciteCourante,
			sdr::size__ NouvelleCapacite,
			aem::mode__ Mode );
		// alloue plus de la place pour pouvoir contenir 'NouvelleCapacite' objets,
		// sachant que 'Capacite courante' est la capacite actuelle
		void Lire_(
			index__ Index,
			sdr::row_t__ Position,
			sdr::size__ Taille,
			sdr::datum__ *Tampon ) const
		{
			AStorage.Read( Descriptors(*Index), Position, Taille, Tampon );
		}
		// place dans 'Tampon' 'Taille' octets, à partir de 'Position', de l'objet 'Index'
		void Ecrire_(
			const sdr::datum__ *Tampon,
			sdr::size__ Taille,
			index__ Index,
			sdr::row_t__ Position )
		{
			AStorage.Write( Tampon, Taille, Descriptors(*Index), Position );
		}
		// écrit à 'Position' de l'objet 'Index' 'Taille' octets de 'Tampon'
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
		// libère la mémoire d'index 'Index'
	public:
		descriptors_ Descriptors;
		// les différents Descriptors
		ags::aggregated_storage_ AStorage;
		// la memoire générale
		struct s
		{
			descriptors_::s Descriptors;
			ags::aggregated_storage_::s AStorage;
		};
		indexed_aggregated_storage_( s &S )
		: Descriptors( S.Descriptors ),
		  AStorage( S.AStorage )
		{}
		// préparation
		void reset( bool P = true )
		{
			Descriptors.reset( P );
			AStorage.reset( P );
		}
		void plug( ags::E_ASTORAGE_ &AS )
		{
			Descriptors.plug( AS );
			this->AStorage.plug( AS );
		}
		indexed_aggregated_storage_ &operator =( const indexed_aggregated_storage_ &IAS )
		{
			ERRFwk();

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

			if ( CurrentAmount > Amount )
				AllouerMoins_( CurrentAmount, Amount, Mode );
			else
				AllouerPlus_( CurrentAmount, Amount, Mode );
		}
		index__ New( index__ Index = E_NIL )
		{
			descriptor__ D = AGS_UNDEFINED_DESCRIPTOR;

			if ( Index == E_NIL )
				Index = Descriptors.New();
			else if ( !Descriptors.Exists( Index ) )
				ERRPrm();

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
			sdr::datum__ *Buffer ) const
		{
			Lire_( Index, Position, Amount, Buffer );
		}
		//f Put 'Amount' bytes at 'Position' to the 'Index' memory from 'Buffer'.
		void Write(
			const sdr::datum__ *Buffer,
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

			if ( D != E_NIL )
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
#if 0
		void write(
			flw::oflow__ &OFlow,
			sdr::size__ Size) const
		{
			Descriptors.write( 0, Size, OFlow );
			AStorage.write( OFlow );
		}
		void read(
			flw::iflow__ &IFlow,
			sdr::size__ Size )
		{
			Descriptors.read( IFlow, 0, Size );
			AStorage.read( IFlow );
		}
#endif
	};

	E_AUTO( indexed_aggregated_storage )

#ifndef FLS__COMPILATION

	class indexed_aggregated_storage_file_manager___
	{
	private:
		bch::bunch_file_manager___ _Descriptors;
		ags::aggregated_storage_file_manager___ _AStorage;
	public:
		void reset( bso::bool__ P = true )
		{
			_Descriptors.reset( P );
			_AStorage.reset( P );
		}
		indexed_aggregated_storage_file_manager___( void )
		{
			reset( false );
		}
		~indexed_aggregated_storage_file_manager___( void )
		{
			reset();
		}
		void Init( 
			const char *DescriptorsFileName,
			const char *AStorageFileName,
			fil::mode__ Mode,
			bso::bool__ Persistent,
			fls::id__ ID )
		{
			_Descriptors.Init( DescriptorsFileName, Mode, Persistent, ID );
			_AStorage.Init( AStorageFileName, Mode, Persistent, ID );
		}
		fil::mode__ Mode( fil::mode__ Mode )
		{
			fil::mode__ ModeBuffer = _Descriptors.Mode( Mode );

			if ( ModeBuffer != _AStorage.Mode( Mode ) )
				ERRPrm();

			return ModeBuffer;
		}
		fil::mode__ Mode( void ) const
		{
			fil::mode__ Mode = _Descriptors.Mode();

			if ( Mode != _AStorage.Mode() )
				ERRPrm();

			return Mode;
		}
		uys::state__ State( void ) const
		{
			uys::state__ State = _Descriptors.State();

			if ( !uys::IsError( State ) )
				if ( State != _AStorage.State() )
					State = uys::sInconsistent;

			return State;
		}
		uys::state__ Bind( void )	// A appeler seulement aprés 'Plug(...)'.
		{
			uys::state__ State = _Descriptors.Bind();

			if ( !uys::IsError( State ) )
				if ( State != _AStorage.Bind() )
					State = uys::sInconsistent;

			return State;
		}
		uys::state__ Settle( void )
		{
			uys::state__ State = _Descriptors.Settle();

			if ( !uys::IsError( State ) )
				if ( State != _AStorage.Settle() )
					State = uys::sInconsistent;

			return State;
		}
		void ReleaseFile( void )
		{
			_Descriptors.ReleaseFile();
			_AStorage.ReleaseFile();
		}
		bso::bool__ IsPersistent( void ) const
		{
#ifdef IAS_DBG
			if ( _Descriptors.IsPersistent() != _AStorage.IsPersistent() )
				ERRFwk();
#endif
			return _Descriptors.IsPersistent();
		}
		void Drop( void )
		{
			_Descriptors.Drop();
			_AStorage.Drop();
		}
		tys::storage_file_manager___ &DescriptorsFileManager( void )
		{
			return _Descriptors;
		}
		ags::aggregated_storage_file_manager___ &AStorageFileManager( void )
		{
			return _AStorage;
		}
		bso::bool__ Exists( void ) const
		{
			bso::bool__ Exists = _Descriptors.Exists();

			if ( Exists != _AStorage.Exists() )
				ERRFwk();

			return Exists;
		}
		time_t TimeStamp( void ) const
		{
			time_t DescriptorsTimeStamp, AStorageTimeStamp;

			DescriptorsTimeStamp = _Descriptors.TimeStamp();
			AStorageTimeStamp = _AStorage.TimeStamp();

			return ( DescriptorsTimeStamp > AStorageTimeStamp ? DescriptorsTimeStamp : AStorageTimeStamp );
		}
#ifdef CPE__VC
#	undef CreateFile
#endif
		bso::bool__ CreateFiles( err::handling__ ErrorHandling )
		{
			bso::bool__ Success = _Descriptors.CreateFile( ErrorHandling );

			if ( !Success )
				return false;

			Success = _AStorage.CreateFile( ErrorHandling );

			return Success;
		}
	};

	inline uys::state__ Plug(
		indexed_aggregated_storage_ &AStorage,
		indexed_aggregated_storage_file_manager___ &FileManager )
	{
		uys::state__ State = bch::Plug( AStorage.Descriptors, FileManager.DescriptorsFileManager() );

		if ( uys::IsError( State ) )
			FileManager.reset();
		else
			if ( ags::Plug( AStorage.AStorage, FileManager.AStorageFileManager() ) != State ) {
				State = uys::sInconsistent;
				FileManager.reset();
			}

		return State;
	}

#endif

	inline void _indexed_aggregated_storage_driver__::SDRRecall(
		sdr::row_t__ Position,
		sdr::size__ Amount,
		sdr::datum__ *Buffer )
	{
		_AStorage->Read( _Index, Position, Amount, Buffer );
	}

	inline sdr::size__ _indexed_aggregated_storage_driver__::Size( void ) const
	{
		if ( _Index == E_NIL )
			return 0;
		else
			return _AStorage->Size( _Index );
	}

	inline void indexed_aggregated_storage_driver__::SDRStore(
		const sdr::datum__ *Buffer,
		sdr::size__ Amount,
		sdr::row_t__ Position )
	{
		_AStorage->Write( Buffer, Amount, _Index, Position );
	}

	inline void indexed_aggregated_storage_driver__::SDRAllocate( sdr::size__ Capacity )
	{
		_AStorage->Reallocate( _Index, Capacity );
	}
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
