/*
	Copyright (C) 1999-2016 Claude SIMON (http://q37.info/contact/).

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

// LiST

#include "err.h"
#include "ids.h"
#include "bitbch.h"

namespace lst {

	typedef ids::E_IDS_STORE_( sdr::row__ ) store_;

	inline uys::eState Test_(
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

	class fCore
	{
	protected:
		virtual ids::fCore &LSTGetIds( void ) = 0;
	public:
		qCALLBACK_DEF( Core );
		ids::fCore &GetIds( void )
		{
			return LSTGetIds();
		}
	};

	//c Handle a list of objects. Use 'LIST_' rather than directly this class.
	template <typename r, typename r_t> class list_
	: public fCore
	{
	protected:
		virtual void LSTAllocate(
			sdr::size__ Size,
			aem::mode__ Mode ) = 0;
		virtual ids::fCore &LSTGetIds( void ) override
		{
			return Locations;
		}
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
		// Retourne l'�l�ment succ�dant � 'Element', ou LST_INEXISTANT si inexistant.
		sdr::row_t__ Successeur_( sdr::row_t__ Element ) const
		{
			return lst::Successeur_( Element, Extent_(), Locations );
		}
		// Retourne l'�l�ment pr�c�dent 'Element', ou LST_INEXISTANT si inexistant.
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
		: fCore(),
		  Locations( S.Locations )
		{}
		void reset( bool P = true )
		{
			fCore::reset( P );
			Locations.reset( P );
		}
		void plug( qSD__ &SD )
		{
			Locations.plug( SD );
		}
		void plug( qAS_ &AS )
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
			fCore::Init();
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
		//f Return the row of a new entry. Use 'Row' if != 'qNIL' (restoration purpose).
		r New(
			r Row,
			aem::mode__ Mode = aem::m_Default )
		{
			if ( Row != qNIL ) {
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
		//f Return the first entry if exists, 'qNIL' if list empty.
		r First( void ) const
		{
			if ( Extent_() )
				if ( !Locations.IsAvailable( 0 ) )
					return 0;
				else
					return Successeur_( 0 );
			else
				return qNIL;
		}
		r First( sdr::size__ Offset ) const
		{
			r Row = First();

			if ( Row != qNIL )
				Row = Next( Row, Offset  );

			return Row;
		}
		//f Return the last entry, 'qNIL' if list empty.
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
				return qNIL;
		}
		r Last( sdr::size__ Offset ) const
		{
			r Row = Last();

			if ( Row != qNIL )
				Row = Previous( Row, Offset  );

			return Row;
		}
		//f Return true if empty, false otherway.
		bso::bool__ IsEmpty( void ) const
		{
			return Amount() == 0;
		}
		//f Return the entry next to 'Entry', 'qNIL' if 'Entry' is the last one.
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
				return qNIL;
		}
		//f Return the previous entry of 'Entry', 'qNIL' if 'Entry' the first one.
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
				return qNIL;
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
		// Ne peut �tre appel� que lorsqu'il y a aucune entr�e libre.
		void Allocate(
			sdr::size__ Size,
			aem::mode__ Mode = aem::m_Default )
		{
			if ( Locations.Amount() != 0 )
				qRFwk();

//			Locations.Init( Size );

			LSTAllocate( Size, Mode );
		}
		// Some help for the 'Hook' related functionnalities.
		store_ &GetIds( void )
		{
			return Locations;
		}
		const store_ &GetIds( void ) const
		{
			return Locations;
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

	using ids::fHook;

	inline bso::fBool Plug(
		fCore &Core,
		fHook &Hook )
	{
		return ids::Plug( Core.GetIds(), Hook );
	}

	using ids::rHF;
	typedef ids::rFH rFH_;

	class rFH
	: public rFH_
	{
	public:
		uys::eState Init(
			const rHF &Filenames,
			fCore &Core,
			uys::mode__ Mode,
			uys::behavior__ Behavior,
			flsq::id__ ID )
		{
			return rFH_::Init(Filenames, Core.GetIds(), Mode, Behavior, ID );
		}
	};

#endif

	//c Handle a list with a maximum of 't' entries. Use 'LIST__' rather than directly this class.
	template <int t, typename r> class list__
	{
	private:
		// Table de l'occupation de la liste.
		bitbch::bit_bunch__<t, r> Occupation_;
		// Nombre d'�l�ments dans la liste.
		sdr::size__ Nombre_;
	public:
		list__( void )
		{
			Occupation_.Reset( false );
			Nombre_ = 0;
		}
		//f First entry, 'qNIL' if no entries.
		r First( void ) const
		{
			return Occupation_.First( true );
		}
		//f LAsttry, 'qNIL' if no entries.
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
			sdr::row_t__ Position = qNIL;

			if ( Nombre_ == t ) 
			{
				if ( ErrorHandling == err::hThrowException )
					qRFwk();
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


#endif
