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

#ifndef IDSQ__INC
# define IDSQ__INC

# define IDSQ_NAME		"IDSQ"

# if defined( E_DEBUG ) && !defined( IDSQ_NODBG )
#  define IDSQ_DBG
# endif

# include "err.h"

# include "stkbch.h"

namespace idsq {

	class fCore
	{
	protected:
		virtual stkbch::fCore &IDSQGetStack( void ) = 0;
		virtual void IDSQSetFirstUnused( bso::uint__ Id )= 0;
	public:
		qCALLBACK_DEF( Core );
		stkbch::fCore &GetStack( void )
		{
			return IDSQGetStack();
		}
		void SetFirstUnused( bso::uint__ Id )
		{
			return IDSQSetFirstUnused( Id );
		}
	};

	template <typename id> class vIdStore
	: public fCore
	{
	private:
		// Return true if 'Id' available, false otherwise.
		bso::bool__ IsAvailable_( id ID ) const
		{
			return Released.Exists( ID ) || ( *ID >= *S_.FirstUnused );
		}
	protected:
		virtual stkbch::fCore &IDSQGetStack( void ) override
		{
			return Released;
		}
		virtual void IDSQSetFirstUnused( bso::uint__ Id ) override
		{
			SetFirstUnused( Id );
		}
	public:
		struct s
		{
			typename stkbch::qBSTACKvl( id )::s Released;
			//r First unused ID.
			id FirstUnused;
		} &S_;
		//o Released IDs
		stkbch::qBSTACKvl( id ) Released;
		vIdStore( s &S )
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
		vIdStore &operator =( const vIdStore &IS )
		{
			Released = IS.Released;
			S_.FirstUnused = IS.S_.FirstUnused;

			return *this;
		}
		//f Initialization with 'First' as first unused.
		void Init( id FirstUnused = 0 )
		{
			fCore::Init();

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

	using stkbch::fHook;

	inline bso::fBool Plug(
		fCore &Core,
		fHook &Hook,
		bso::uint__ FirstUnused )
	{
		Core.SetFirstUnused( FirstUnused );

		return stkbch::Plug( Core.GetStack(), Hook );
	}

	using stkbch::rRH;

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
			flsq::id__ ID )
		{
			return rFH_::Init( Filenames, Core.GetStack(), Mode, Behavior, ID );
		}
	};
}

# define qIDSv( t )	vIdStore<t>
# define qIDSi( t )	iIdStore<t>


#endif
