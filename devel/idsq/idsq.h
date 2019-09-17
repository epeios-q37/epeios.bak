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

// ID Store

#ifndef IDSQ_INC_
# define IDSQ_INC_

# define IDSQ_NAME		"IDSQ"

# if defined( E_DEBUG ) && !defined( IDSQ_NODBG )
#  define IDSQ_DBG
# endif

# include "err.h"

# include "stkbch.h"

namespace idsq {

	using stkbch::sHook;

	template <typename id> class dIdStore
	{
	private:
		bso::bool__ IsAvailable_( id ID ) const
		{
			return Released.Exists( ID ) || ( *ID >= *S_.FirstUnused );
		}
	public:
		struct s
		{
			typename stkbch::qBSTACKdl( id )::s Released;
			//r First unused ID.
			id FirstUnused;
		} &S_;
		//o Released IDs
		stkbch::qBSTACKdl( id ) Released;
		dIdStore( s &S )
		: S_( S ),
		  Released( S.Released )
		{}
		void reset( bso::bool__ P = true )
		{
			Released.reset( P );
			S_.FirstUnused = 0;
		}
		void plug(
			sHook &Hook,
			id FirstUnused )
		{
			S_.FirstUnused = FirstUnused,
			Released.plug( Hook );
		}
		void plug( qASd *AS )
		{
			Released.plug( AS );
		}
		dIdStore &operator =( const dIdStore &IS )
		{
			Released = IS.Released;
			S_.FirstUnused = IS.S_.FirstUnused;

			return *this;
		}
		//f Initialization with 'First' as first unused.
		void Init( id FirstUnused = 0 )
		{
			S_.FirstUnused = FirstUnused;

			Released.Init();
		}
		void SetFirstUnused( id FirstUnused )
		{
			S_.FirstUnused = FirstUnused;
		}
		//f Return a new unused id. 'Released' is set at 'true' if the id is a released one.
		id New( bso::bool__ &Released )
		{
			id Id;

			if ( this->Released.Amount() ) {
				Id = this->Released.Pop();
				Released = true;
			} else {
				Id = (*S_.FirstUnused)++;

				if ( S_.FirstUnused == 0 )
					qRLmt();

				Released = false;
			}

			return Id;
		}
		id New( void )
		{
			bso::bool__ Released;

			return New( Released );
		}
		bso::bool__ New( id Id )
		{
			if ( *Id >= *S_.FirstUnused ) {
				S_.FirstUnused = *Id + 1;
				return true;
			} else {
#ifdef IDSQ_DBG
				if ( !IsAvailable_( Id ) )
					qRFwk();
#endif
				Released.Remove( Released.Search( Id ) );

				return false;
			}
		}
		void Release( id Id )
		{
#ifdef IDSQ_DBG
			if ( IsAvailable_( Id ) )
				qRFwk();
#endif
			Released.Push( Id );
		}
		void RestorationRelease( id Id )
		{
			if ( *Id >= *S_.FirstUnused )
				S_.FirstUnused = *Id + 1;

			Released.Push( Id );
		}
		//f Return the first available id.
		id GetFirstAvailable( void ) const
		{
			return S_.FirstUnused;
		}
		//f Return the amount of released IDs.
		sdr::size__ Amount( void ) const
		{
			return Released.Amount();
		}
		//f Return true' if 'ID' is available, false otherwize.
		bso::bool__ IsAvailable( id Id ) const
		{
			return IsAvailable_( Id );
		}
	};

	qW1( IdStore );

	using stkbch::rRH;

	using stkbch::rHF;
	using stkbch::rFH;
}

# define qIDSd( t )	dIdStore<t>
# define qIDSw( t )	wIdStore<t>


#endif
