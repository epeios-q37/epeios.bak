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

//	$Id: mmg.h,v 1.30 2013/04/11 19:36:50 csimon Exp $

#ifndef MMG__INC
#define MMG__INC

#define MMG_NAME		"MMG"

#define	MMG_VERSION	"$Revision: 1.30 $"

#define MMG_OWNER		"Claude SIMON"


#if defined( E_DEBUG ) && !defined( MMG_NODBG )
#define MMG_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.30 $
//C Claude SIMON (csimon at zeusw dot org)
//R $Date: 2013/04/11 19:36:50 $

/* End of automatic documentation generation part. */

/* Addendum to the automatic documentation generation part. */
//D Memory MerGer 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

//D Memory MerGer.

#include "err.h"
#include "ags.h"
#include "fls.h"

namespace mmg
{
	//e Rule about how the object is handle.
	enum rule
	{
		//i L'object est created.
		rCreation,
		//i L'objet is recovered.
		rRecovery
	};
	//e state of the object
	enum state
	{
		//i Immortal.
		sImmortal,
		//i Mortal.
		sMortal,
	};

	struct merger_memory_driver_s__
	{
		uys::untyped_storage::s Memory;
	};


	// Pilote mmoire  usage interne.
	template <typename st> class merger_memory_driver_
	: public qSD__
	{
	private:
		// Pointeur sur la partie statique de l'objet  sauver.
		st *Static_;
	protected:
		// Alloue 'Capacite' octets.
		virtual void SDRAllocate( sdr::size__ Capacity )
		{
			Memory.Allocate( Capacity + sizeof( st ) );
		}
		virtual sdr::size__ SDRSize( void ) const
		{
			sdr::size__ Size = Memory.Size();

			if ( Size >= sizeof( st ) )
				Size -= sizeof( st );
			else if ( Size != 0 )
				qRFwk();

			return Size;
		}
		virtual void SDRRecall(
			sdr::row_t__ Position,
			sdr::size__ Amount,
			sdr::datum__ *Buffer )
		{
			Memory.Recall( Position + sizeof( st ), Amount, Buffer );
		}
		virtual void SDRStore(
			const sdr::datum__ *Buffer,
			sdr::size__ Amount,
			sdr::row_t__ Position )
		{
			StockerStatique();
			Memory.Store( Buffer, Amount, Position + sizeof( st ) );
		}
	public:
		typedef merger_memory_driver_s__ s;
		s &S_;
		uys::untyped_storage_ Memory;
		void reset( bool P = true )
		{
			qSD__::reset( P );
			Memory.reset( P );
			Static_ = NULL;
		}
		merger_memory_driver_( s &S )
		: S_( S ),
		  Memory( S.Memory )
		{
			reset( false );
		}
		~merger_memory_driver_( void )
		{
			reset();
		}
		void plug( qSD__ &SD )
		{
			Memory.plug( SD );
		}
		void plug( qAS_ &AS )
		{
			Memory.plug( AS );
		}
		void Init(
			st *Static,
			rule Regle )
		{
			Static_ = Static;
			qSD__::Init();
			Memory.Init();

			if ( Regle == mmg::rCreation )
			{
				Memory.Allocate( sizeof( st ) );
				StockerStatique();
			}
			else if ( Regle == mmg::rRecovery ) {
				RecupererStatique();
			} else
				qRFwk();
		}
		void StockerStatique( void )
		{
			if ( Static_ /* && ( Mode_== mdr::mReadWrite )*/ )
				Memory.Store( (sdr::datum__ *)Static_, sizeof( st ), 0 );
		}
		void EcrireDansFlot( flw::oflow__ &Flot ) const
		{
			Flot.Put( Static_, sizeof( st ) );
		}
		void LireDeFlot( flw::iflow__ &Flot )
		{
			Flot.Get( sizeof( st ), Static_ );
		}
		void RecupererStatique( void )
		{
			Memory.Recall( 0, sizeof( st ), (sdr::datum__ *)Static_ );
		}
		void Immortalize( void )
		{
			reset( false );
			plug( *(qSD__ *)NULL );
		}

	};

	// Pour VC++, qui ne comprend rien sinon.
	template <class st> struct mmg_s
	{
		ags::aggregated_storage_::s Memoire;
		// A vrai si le contenu survit  l'objet.
		st Object;
		merger_memory_driver_s__ Driver_;
		mmg::state State;
	};

	/*c Merge all the memory of an object of type 't', including the statical
	part, in one memory. Use 'MMG_MEMORY_MERGER_( t )' rather then directly this class. */
	template <class t, class st> class memory_merger_
	{
	private:
		t Object;
	public:
		// La mmoire qui contient la partie dynamique de l'objet.
		ags::aggregated_storage_ Memoire;
		// La partie statique de l'objet  persister.
	/*	struct s
		: public st
		{
			mmm_multimemoire_::s Memoire;
			// A vrai si le contenu survit  l'objet.
			fdm__bool Persistant;
			friend mmg_centralisateur_memoire_;
		} *S_;
	*/
		struct s
		: public mmg::mmg_s < st > {} &S_;
	private:
		// Le pilote grant les particularits de cet objet
		mmg::merger_memory_driver_ < st > Driver_;
	public:
		memory_merger_( s &S )
		: S_( S ),
		  Object( S.Object ),
		  Memoire( S.Memoire ),
		  Driver_( S.Driver_ )
		{}
		void reset( bool P = true )
		{
			if ( P ) {
				if ( S_.State == mmg::sImmortal )
					Immortalize();
			}

			Object.reset( P );
			Memoire.reset( P );
			Driver_.reset( P );

			S_.State = mmg::sMortal;

		}
		//f Utilisation de 'Pilote' comme pilote mmoire.
		void plug( qSD__ &SD )
		{
			Driver_.plug( SD );
			Memoire.plug( Driver_ );	// Notamment pour initialiser correctement le 'size'.
		}
		void plug( qAS_ &AS )
		{
			Driver_.plug( AS );
			Memoire.plug( Driver_ );
		}
		//f Initialization with rule 'Rule' and mode 'Mode'.
		void Init( mmg::rule Rule )
		{
			if ( S_.State == mmg::sImmortal )
				Immortalize();

			S_.State = mmg::sMortal;

			Object.plug( Memoire );

			Driver_.Init( &S_.Object, Rule );

			Memoire.plug( Driver_ );

			if ( Rule == mmg::rCreation )
				Memoire.Init();
		}
		//f The memory is unpluged and can be used by an other object.
		void Immortalize( void )
		{
			Driver_.Immortalize();
			Object.reset( false );
			Memoire.reset( false );
			Driver_.reset( false );
		}
		memory_merger_ &operator =( const memory_merger_ & )
		{
			qRFwk();

			return *this;	// Only to avoid a warning.
		}
		//f The object would be immortalized or destructed depend of the value of 'State'.
		void State( mmg::state State )
		{
			S_.State = State;
		}
		//f Return the object which is handled by.
		t &operator ()( void )
		{
			return Object;
		}
		//f Return the object which is handled by.
		t *operator ->( void )
		{
			return &Object;
		}
	};

	E_AUTO2( memory_merger )

	//m Merge all the memory, including the statical part, of a 't' type object in one memory.
	#define E_MEMORY_MERGER_( t )	memory_merger_<t, t::s>

	#define E_MEMORY_MERGER( t )	memory_merger<t, t::s>

	/*c File memory merger. Same as mmg_memory_merger_<t, t::s>, but use a file as memory.
	Use 'MMG_FILE_MEMORY_MERGER( t )' rather then directly this class. */
	template <class t, class st> class file_merger___
	: public memory_merger<t, st>
	{
	private:
		fls::file_storage_driver___ PiloteFichier_;
		fls::id__ _ID;
	public:
		void reset( bool P = true )
		{
			memory_merger<t, st>::reset( P );

			PiloteFichier_.reset( P );

			if ( P )
				if ( _ID != FLS_UNDEFINED_ID )
					fls::ReleaseId( _ID );

			_ID = FLS_UNDEFINED_ID;
		}
		file_merger___( void )
		: PiloteFichier_()
		{
			reset( false );
		}
		~file_merger___( void )
		{
			reset();
		}
		/*f Initialization with file named 'FileName'. The object is placed
		in 'ObjectMode', and the file in 'FileMode'. Return a value which depends
		on whether exists or not. If the file doesn't already exist,
		both 'ObjectMode' and 'FileMode' are forced for to 'mdr::mReadWrite'. */
		mmg::rule Init(
			const char *FileName,
			fil::mode__ FileMode )
		{	
			reset();

			bso::bool__ Test;

			Test = fil::Exists( FileName );

			_ID = fls::GetId();

			PiloteFichier_.Init( _ID, FileName, FileMode );
			PiloteFichier_.Persistent();
			PiloteFichier_.Manual();

			memory_merger<t, st>::plug( PiloteFichier_ );

			if ( Test )
			{
				memory_merger<t, st>::Init( mmg::rRecovery );
				PiloteFichier_.Mode( FileMode );
			}
			else
			{
				memory_merger<t, st>::Init( mmg::rCreation );
				PiloteFichier_.Mode( fil::mReadWrite );
			}

			memory_merger<t, st>::State( mmg::sImmortal );

			return ( Test ? mmg::rRecovery : mmg::rCreation );
		}
		//f The object is placed in 'ObjectMode', the file in 'FileMode'.
		void Mode( fil::mode__ Mode  )
		{
			PiloteFichier_.Mode( Mode );
		}
		void plug( qSD__ & )
		{
			qRFwk();
		}
		void State( mmg::state )
		{
			qRFwk();
		}
	};

	#define E_FILE_MERGER___( t )		file_merger___<t, t::s>
}

/*$END$*/
#endif
