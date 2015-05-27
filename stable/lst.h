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

#ifndef LST__INC
# define LST__INC

# define LST_NAME		"LST"

# if defined( E_DEBUG ) && !defined( LST_NODBG )
#  define LST_DBG
# endif

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

// LiST

#include "err.h"
#include "ids.h"
#include "bitbch.h"

namespace lst {

	typedef ids::E_IDS_STORE_( sdr::row__ ) store_;

	// Retourne l'id de la premire entre disponible (hors pile des 'released').
	uys::state__ WriteToFile_(
		const store_ &Store,
		const fnm::name___ &Name );

	uys::state__ ReadFromFile_(
		const fnm::name___ &Name,
		store_ &Store );

	inline uys::state__ Test_(
		const fnm::name___ &FileName,
		time_t ReferenceTimeStamp )
	{
		if ( !FileName.Amount()  )
			return uys::sInconsistent;

		if ( !fil::Exists( FileName ) )
			return uys::sAbsent;

		if ( fil::GetLastModificationTime( FileName ) <= ReferenceTimeStamp )
			return uys::sInconsistent;

		return uys::sExists;
	}

	sdr::row_t__ Successeur_(
		sdr::row_t__ Element,
		sdr::size__ Amount,
		const store_ &Libres );

	sdr::row_t__ Predecesseur_(
		sdr::row_t__ Element,
		const store_ &Libres );

	void Insert_(
		sdr::row_t__ First,
		sdr::row_t__ Last,
		store_ &Store );

	void MarkAsReleased_(
		sdr::row_t__ First,
		sdr::row_t__ Last,
		store_ &Store );


	//c Handle a list of objects. Use 'LIST_' rather than directly this class.
	template <typename r, typename r_t> class list_
	{
	protected:
		/*v Cette fonction est appele lors d'allocations dans la liste;
		permet de synchroniser la taille de la liste avec d'autres ensembles;
		'Size' est la capacit alloue. */
		virtual void LSTAllocate(
			sdr::size__ Size,
			aem::mode__ Mode ) = 0;
	private:
		// Return the extent, based on 'Locations'.
		sdr::row_t__ Extent_( void ) const
		{
			return *Locations.GetFirstAvailable();
		}
		sdr::row_t__ Nouveau_( aem::mode__ Mode )
		{
			bso::bool__ Released = false;

			sdr::row_t__ New = *Locations.New( Released );

			if ( !Released )
				LSTAllocate( Extent_(), Mode );

			return New;
		}
		// Retourne l'lment succdant  'Element', ou LST_INEXISTANT si inexistant.
		sdr::row_t__ Successeur_( sdr::row_t__ Element ) const
		{
			return lst::Successeur_( Element, Extent_(), Locations );
		}
		// Retourne l'lment prcdent 'Element', ou LST_INEXISTANT si inexistant.
		sdr::row_t__ Predecesseur_( sdr::row_t__ Element ) const
		{
			return lst::Predecesseur_( Element, Locations );
		}
	public:
		//o Store of locations.
		store_ Locations;
		struct s
		{
			store_::s Locations;
		};
	// fonctions
		list_( s &S )
		: Locations( S.Locations )
		{}
		void reset( bool P = true )
		{
			Locations.reset( P );
		}
		void plug( sdr::E_SDRIVER__ &SD )
		{
			Locations.plug( SD );
		}
		void plug( ags::E_ASTORAGE_ &AS )
		{
			Locations.plug( AS );
		}
		list_ &operator =( const list_ &L )
		{
			Locations = L.Locations;

			return *this;
		}
	/*	void ecrire( flo_sortie_ &Flot )
		{
			FLOEcrire( S_.Nombre, Flot );

			Libres.ecrire( Flot );
		}
		void lire( flo_entree_ &Flot )
		{
			FLOLire( Flot, S_.Nombre );

			Libres.lire( Flot );
		}
	*/	//f Initialiration.
		void Init( void )
		{
			Locations.Init();
		}
		//f Delete 'Entry'.
		void Delete( r Entry )
		{
			Locations.Release( *Entry );
		}
		//f Return the position of a new entry.
		r New( aem::mode__ Mode = aem::m_Default )
		{
			return (r_t)Nouveau_( Mode );
		}
		//f Return the row of a new entry. Use 'Row' if != 'E_NIL' (restoration purpose).
		r New(
			r Row,
			aem::mode__ Mode = aem::m_Default )
		{
			if ( Row != E_NIL ) {
				r FirstAvailable = *Locations.GetFirstAvailable();

				if ( Locations.New( *Row ) ) {

					LSTAllocate( *Row + 1, Mode );

					if ( Row != FirstAvailable )
						MarkAsReleased_( *FirstAvailable, *Row - 1, Locations );
				}

				return Row;
			} else
				return New();
		}
		//f Return the first entry if exists, 'E_NIL' if list empty.
		r First( void ) const
		{
			if ( Extent_() )
				if ( !Locations.IsAvailable( 0 ) )
					return 0;
				else
					return Successeur_( 0 );
			else
				return E_NIL;
		}
		r First( sdr::size__ Offset ) const
		{
			r Row = First();

			if ( Row != E_NIL )
				Row = Next( Row, Offset  );

			return Row;
		}
		//f Return the last entry, 'E_NIL' if list empty.
		r Last( void ) const
		{
			if ( Extent_() )
			{
				sdr::row_t__ P = Extent_() - 1;

				if ( !Locations.IsAvailable( P ) )
					return P;
				else
					return Predecesseur_( P );
			}
			else
				return E_NIL;
		}
		r Last( sdr::size__ Offset ) const
		{
			r Row = Last();

			if ( Row != E_NIL )
				Row = Previous( Row, Offset  );

			return Row;
		}
		//f Return true if empty, false otherway.
		bso::bool__ IsEmpty( void ) const
		{
			return Amount() == 0;
		}
		//f Return the entry next to 'Entry', 'E_NIL' if 'Entry' is the last one.
		r Next(
			r Entry,
			sdr::size__ Offset = 1 ) const
		{
			if ( ( *Entry += Offset ) < Extent_() )
				if ( !Locations.IsAvailable( *Entry ) )
					return Entry;
				else
					return Successeur_( *Entry );
			else
				return E_NIL;
		}
		//f Return the previous entry of 'Entry', 'E_NIL' if 'Entry' the first one.
		r Previous(
			r Entry,
			sdr::size__ Offset = 1 ) const
		{
			if ( ( *Entry -= Offset ) > 0 )
				if ( !Locations.IsAvailable( *Entry ) )
					return Entry;
				else
					return Predecesseur_( *Entry );
			else
				return E_NIL;
		}
		//f Amount of entries, NOT the extent of the list.
		sdr::size__ Amount( void ) const
		{
			return Extent_() - Locations.Amount();
		}
		//f Extent of list.
		sdr::size__ Extent( void ) const
		{
			return Extent_();
		}
		//f Return true if 'Entry' exists, false otherwise.
		bso::bool__ Exists( r Entry ) const
		{
			if ( *Entry >= Extent_() )
				return false;
			else
				return !Locations.IsAvailable( *Entry );
		}
		// Ne peut tre appel que lorsqu'il y a aucune entre libre.
		void Allocate(
			sdr::size__ Size,
			aem::mode__ Mode = aem::m_Default )
		{
			if ( Locations.Amount() != 0 )
				ERRFwk();

//			Locations.Init( Size );

			LSTAllocate( Size, Mode );
		}
	};

	E_AUTO2( list )


#define E_LISTtx( r, r_t )	list<r, r_t>
#define E_LISTtx_( r, r_t )	list_<r, r_t>

		//d Handle a list of objects.
#define E_LISTt( r )	E_LISTtx( r, sdr::row_t__ )
#define E_LISTt_( r )	E_LISTtx_( r, sdr::row_t__ )

#define E_LIST	E_LISTt( sdr::row__ )
#define E_LIST_	E_LISTt_( sdr::row__ )

#ifndef FLM__COMPILATION

	struct hook_filenames___
	{
	public:
		fnm::name___ Filename;
		void reset( bso::bool__ P = true )
		{
			Filename.reset( P );
		}
		E_CDTOR( hook_filenames___ );
		void Init(
			const fnm::name___ &Path,
			const fnm::name___ &Basename )	// Peut tre vide ('NULL') si 'Path' contient dj le nom de fichier.
		{
			Filename.Init();
			fnm::BuildPath( Path, Basename, "l.q37", Filename );
		}
	};

	class files_hook___
	{
	private:
		lst::store_ *_Store;
		fnm::name___ _Filename;
		uys::mode__ _Mode;
		uys::behavior__ _Behavior;
	public:
		void reset( bso::bool__ P = true )
		{
			if ( P ) {
			//	Settle();	// Fait en amoont, car il manque le 'TimeStamp()'.
			}

			_Filename.reset( P );
			_Store = NULL;
			_Mode = uys::m_Undefined;
			_Behavior = uys::b_Undefined;
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
			const hook_filenames___ &Filenames,
			uys::mode__ Mode,
			uys::behavior__ Behavior )
		{
			reset();

			_Filename.Init( Filenames.Filename );

			_Mode = Mode;
			_Behavior = Behavior;
		}
		void Init(
			const str::string_ &LocalizedFileAffix,
			uys::mode__ Mode,
			uys::behavior__ Behavior );
		uys::mode__ Mode( uys::mode__ Mode )
		{
			tol::Swap( Mode, _Mode );

			return Mode;
		}
		uys::mode__ Mode( void ) const
		{
			return _Mode;
		}
		bso::bool__ IsPersistent( void ) const
		{
			return _Behavior == uys::bPersistent;
		}
		uys::state__ State( void ) const
		{
			return Test_( _Filename, 0 );
		}
		uys::state__ Bind( time_t ReferenceTimeStamp )
		{
			uys::state__ State = Test_( Filename(), ReferenceTimeStamp );

			if ( uys::Exists( State ) )
				State = ReadFromFile_( Filename(), *_Store );

			return State;
		}
		uys::state__ Settle( time_t ReferenceTimeStamp )
		{
			uys::state__ State = uys::s_Undefined;

			if ( _Store == NULL )
				return uys::sInconsistent;

			if ( ( ReferenceTimeStamp == 0 )
				|| ( !fil::Exists( _Filename ) )
				|| ( fil::GetLastModificationTime( _Filename ) <= ReferenceTimeStamp ) )
					State = WriteToFile_( *_Store, _Filename );
			else
				State = uys::sExists;

			if ( uys::IsError( State ) )
				return State;
			
			while ( fil::GetLastModificationTime( _Filename ) <= ReferenceTimeStamp ) {
				tol::EpochTime( true );	// Permet d'attendre une unit de temps.
				fil::Touch( _Filename );
			}

			return State;
		}
		void Drop( void )
		{
			if ( ( _Store == NULL ) || !_Filename.Amount() )
				ERRFwk();

			if ( fil::Exists( _Filename ) )
				if ( !fil::Remove( _Filename ) )
					ERRLbr();
		}
		const fnm::name___ &Filename( void ) const
		{
			return _Filename;
		}
		void Set( lst::store_ &Store )
		{
			if ( _Store != NULL )
				ERRFwk();

			_Store = &Store;
		}
		bso::bool__ Exists( void ) const
		{
			return uys::Exists( State() );
		}
#ifdef CPE__VC
#	undef CreateFile
#endif
		bso::bool__ CreateFiles( err::handling__ ErrorHandling = err::h_Default )
		{
			if ( fil::Exists( _Filename ) ) {
				if ( ErrorHandling == err::hThrowException )
					ERRFwk();
				else
					return false;
			}

			return fil::Create( _Filename, ErrorHandling );
		}
		void ReleaseFiles( void )
		{
			// A des fins de standardisation.
		}
		time_t TimeStamp( void ) const
		{
			if ( Exists() )
				return fil::GetLastModificationTime( _Filename );
			else
				return 0;
		}
	};


	template <typename list> uys::state__ Plug(
		list &List,
		files_hook___ &Hook,
		sdr::row__ FirstUnused,
		time_t ReferenceTimeStamp )
	{
		uys::state__ State = Test_( Hook.Filename(), ReferenceTimeStamp );

		if ( uys::IsError( State ) )
			Hook.reset();
		else {
			Hook.Set( List.Locations );
			List.Locations.Init( FirstUnused );
		}

		return State;
	}
#endif

	//c Handle a list with a maximum of 't' entries. Use 'LIST__' rather than directly this class.
	template <int t, typename r> class list__
	{
	private:
		// Table de l'occupation de la liste.
		bitbch::bit_bunch__<t, r> Occupation_;
		// Nombre d'lments dans la liste.
		sdr::size__ Nombre_;
	public:
		list__( void )
		{
			Occupation_.Reset( false );
			Nombre_ = 0;
		}
		//f First entry, 'E_NIL' if no entries.
		r First( void ) const
		{
			return Occupation_.First( true );
		}
		//f LAsttry, 'E_NIL' if no entries.
		r Last( void ) const
		{
			return Occupation_.Last( true );
		}
		//f Entry next to 'Entry', none if 'Entry' the last one.
		r Next( r Entry ) const
		{
			return Occupation_.Next( Entry, true );
		}
		//f Previous entry from 'Entry', none if 'Entry' the first one.
		r Previous( r Entry ) const
		{
			return Occupation_.Previous( Entry, true );
		}
		//f Return the position of a new entry.
		r CreateEntry( err::handling__ ErrorHandling = err::h_Default  )
		{
			sdr::row_t__ Position = E_NIL;

			if ( Nombre_ == t ) 
			{
				if ( ErrorHandling == err::hThrowException )
					ERRFwk();
			}
			else
			{
				Position = Occupation_.First( false );

				Occupation_.Write( true, Position );

				Nombre_++;
			}

			return Position;
		}
		//f Delete 'Entry', which MUST exists.
		void Delete( r Entry )
		{
			Occupation_.Write( false, Entry );

			Nombre_--;
		}
		//f Return true if 'Entry' exists, fals otherwise.
		bso::bool__ Exists( r Entry ) const
		{
			return Occupation_.Read( Entry );
		}
		//f Return true if no entry, false otherwise.
		bso::bool__ IsEmpty( void ) const
		{
			return !Nombre_;
		}
		//f Mark the list as full.
		void Fill( void )
		{
			Occupation_.Reset( true );
			Nombre_ = t;
		}
		//f Mark the list as empty.
		void Empty( void )
		{
			Occupation_.Reset( false );
			Nombre_ = 0;
		}
		//f Return true if totally full, false otherwise.
		bso::bool__ IsFull( void )
		{
			return Nombre_ >= t;
		}

		//m Handle a list with a maximum of 'n' entries.
		#define E_LIST__( n )		list__< n >
	};

}


				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
