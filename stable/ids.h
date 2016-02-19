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

// ID Store 

# ifndef IDS__INC
# define IDS__INC

# define IDS_NAME		"IDS"

# if defined( E_DEBUG ) && !defined( IDS_NODBG )
#  define IDS_DBG
# endif

# include "err.h"
# include "flw.h"
# include "stkbch.h"
# include "ags.h"

namespace ids {

	class fCore
	{
	protected:
		virtual stkbch::fCore &IDSGetStack( void ) = 0;
	public:
		qCALLBACK_DEF( Core );
		stkbch::fCore &GetStack( void )
		{
			return IDSGetStack();
		}
	};

	//c Store of ids of type 'id__'. Use 'ID_STORE' rather than directly this class.
	template <typename id__> class ids_store_ 
	: public fCore
	{
	private:
		// Return true if 'Id' available, false otherwise.
		bso::bool__ IsAvailable_( id__ ID ) const
		{
			return Released.Exists( ID ) || ( *ID >= *S_.FirstUnused );
		}
	protected:
		virtual stkbch::fCore &IDSGetStack( void ) override
		{
			return Released;
		}
	public:
		struct s
		{
			typename stkbch::qBSTACKvl( id__ )::s Released;
			//r First unused ID.
			id__ FirstUnused;
		} &S_;
		//o Released IDs
		stkbch::qBSTACKvl( id__ ) Released;
		ids_store_( s &S )
		: S_( S ),
		  Released( S.Released )
		{}
		void reset( bso::bool__ P = true )
		{
			fCore::reset( P );
			Released.reset( P );
			S_.FirstUnused = 0;
		}
		void plug( qSD__ &SD )
		{
			Released.plug( SD );
		}
		void plug( qAS_ &AS )
		{
			Released.plug( AS );
		}
		ids_store_ &operator =( const ids_store_ &IS )
		{
			Released = IS.Released;
			S_.FirstUnused = IS.S_.FirstUnused;

			return *this;
		}
		//f Initialization with 'First' as first unused.
		void Init( id__ FirstUnused = 0 )
		{
			fCore::Init();

			S_.FirstUnused = FirstUnused;

			Released.Init();
		}
		void SetFirstUnused( id__ FirstUnused )
		{
			S_.FirstUnused = FirstUnused;
		}
		//f Return a new unused id. 'Released' is set at 'true' if the id is a released one.
		id__ New( bso::bool__ &Released )
		{
			id__ ID;

			if ( this->Released.Amount() ) {
				ID = this->Released.Pop();
				Released = true;
			} else {
				ID = (*S_.FirstUnused)++;

				if ( S_.FirstUnused == 0 )
					qRLmt();

				Released = false;
			}

			return ID;
		}
		//f Return a new unused id.
		id__ New( void )
		{
			bso::bool__ Released;

			return New( Released );
		}
		//f Mark 'ID' as used. Return true if 'ID' out of range. For restoration purpose only.
		bso::bool__ New( id__ ID )
		{
			if ( *ID >= *S_.FirstUnused ) {
				S_.FirstUnused = *ID + 1;
				return true;
			} else {
#ifdef IDS_DBG
				if ( !IsAvailable_( ID ) )
					qRFwk();
#endif
				Released.Remove( Released.Search( ID ) );

				return false;
			}
		}
		//f Released id 'ID'.
		void Release( id__ ID )
		{
#ifdef IDS_DBG
			if ( IsAvailable_( ID ) )
				qRFwk();
#endif
			Released.Push( ID );
		}
		//f Modify the whole object so that 'ID' appears as available. For restoration purpose only.
		void RestorationRelease( id__ ID )
		{
			if ( *ID >= *S_.FirstUnused )
				S_.FirstUnused = *ID + 1;

			Released.Push( ID );
		}
		//f Return the first available id.
		id__ GetFirstAvailable( void ) const
		{
			return S_.FirstUnused;
		}
		//f Return the amount of released IDs.
		sdr::size__ Amount( void ) const
		{
			return Released.Amount();
		}
		//f Return true' if 'ID' is available, false otherwize.
		bso::bool__ IsAvailable( id__ ID ) const
		{
			return IsAvailable_( ID );
		}
	};

	E_AUTO1( ids_store )

	using stkbch::fHook;

	inline bso::fBool Plug(
		fCore &Core,
		fHook &Hook )
	{
		return stkbch::Plug( Core.GetStack(), Hook );
	}

	using stkbch::rHF;
	typedef stkbch::rFH rFH_;

	class rFH
	: public rFH_
	{
	public:
		uys::eState Init(
			const rHF &Filenames,
			fCore &Core,
			uys::mode__ Mode,
			uys::behavior__ Behavior,
			flsq::id__ ID,
			time_t ReferenceTime = 0 )
		{
			return rFH_::Init( Filenames, Core.GetStack(), Mode, Behavior, ID, ReferenceTime );
		}
	};
}

# define E_IDS_STORE_( t )	ids_store_<t>
# define E_IDS_STORE( t )	ids_store<t>


#endif
