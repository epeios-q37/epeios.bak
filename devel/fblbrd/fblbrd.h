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


#ifndef FBLBRD_INC_
# define FBLBRD_INC_

# define FBLBRD_NAME		"FBLBRD"

# if defined( E_DEBUG ) && !defined( FBLBRD_NODBG )
#  define FBLBRD_DBG
# endif

//D Frontend/Backend Layout Backend Request Description

# include <stdarg.h>

#include "err.h"
#include "flw.h"
#include "ctn.h"

#include "fblcst.h"
#include "fbltyp.h"

namespace fblbrd {
	namespace {
		using namespace fblcst;
		using namespace ctn;
	}

	using namespace fbltyp;

	//c The description of a request.
	class description_
	{
	private:
        void Fill_(
            bso::sBool EndOfInParamsDetected,
            cast__ *Entry,
            cast__ Cast )
        {
            if ( !EndOfInParamsDetected ) {
                *Entry = fblcst::cEnd;
                Entry++;
            }

            *Entry = Cast;
            Entry++;

            if ( Cast != fblcst::cEnd ) {
                *Entry = fblcst::cEnd;
            }
        }
        template <typename... t> void Fill_(
            bso::sBool EndOfInParamsDetected,
            cast__ *Entry,
            cast__ Cast,
            const t&... T )
         {
            if ( Cast == fblcst::cEnd ) {
                if ( EndOfInParamsDetected )
                    qRFwk();

                EndOfInParamsDetected = true;
            }

            *Entry = Cast;

            Entry++;

            Fill_( EndOfInParamsDetected, Entry, T...);
        }
	public:
		struct s
		{
			str::string_::s Name;
			casts_::s Casts;
		} &S_;
		//o Name of the command.
		str::string_ Name;
		/*o Parameters and return value types. Separated by 'cEnd'. The 'cEnd'
		to signalize the end of the request must NOT be put. */
		casts_ Casts;
		description_( s &S )
		: S_( S ),
		  Name( S.Name ),
		  Casts( S.Casts )
		{}
		void reset( bool P = true )
		{
			Name.reset( P );
			Casts.reset( P );
		}
		description_ &operator =( const description_ &D )
		{
			Name = D.Name;
			Casts = D.Casts;

			return *this;
		}
		void plug( qASd *AS )
		{
			Name.plug( AS );
			Casts.plug( AS );
		}
		//f Initialization. The name ans casts are added one per one.
		void Init( void )
		{
			Name.Init();
			Casts.Init();
		}
		/*f Initializing with name 'Name' and a the list of casts 'Casts'. The list
		must contain 2 'cEnd', the first at the end of the parameters casts,
		and 1 of the end of returned values casts. */
		void Init(
			const char *Name,
			const cast__ *Casts );
        template <typename... t> void Init(
			const char *Name,
            cast__ Cast,
            const t&... T )
        {
            bso::sBool EndOfInParamsDetected = false;
            cast__ Casts[sizeof...(T)+3];   // '+2' in case of there are no out parameters,
                                            // and the delimter of in params is missing.
                                            // '+1' to store 'Cast'.
            Fill_( EndOfInParamsDetected, Casts, Cast, T... );

            Init( Name, Casts );
        }
/*		void Init(
			const char *Name,
			cast__ Cast,	// This parameter was added to avoid ambiguity with some other method.
			va_list Casts );
		void Init(
			const char *Name,
			cast__ Cast,
			... )
		{
			va_list VL;

			va_start( VL, Cast );

			Init( Name, Cast, VL );

			va_end( VL );
		}
*/		//f Initialization with name 'Name'. The casts would be given using 'New()'.
		void Init( const char *Name )
		{
			this->Name.Init();
			Casts.Init();

			this->Name = Name;
		}
		//f Append 'Cast'.
		void Append( fblcst::cast__ Cast )
		{
			Casts.Append( Cast );
		}
	};

	E_AUTO( description )

	//c Requests descriptions.
	class descriptions_
	: public E_CONTAINER_( description_ )
	{
	public:
		struct s
		: public E_CONTAINER_( description_ )::s
		{};
		descriptions_( s &S )
		: ctn::E_CONTAINER_( description_ )( S )
		{}
		void reset( bool P = true )
		{
			E_CONTAINER_( description_ )::reset( P );
		}
		void plug( qASd *AS )
		{
			E_CONTAINER_( description_ )::plug( AS );
		}
		descriptions_ &operator =( const descriptions_ &D )
		{
			E_CONTAINER_( description_ )::operator =( D );

			return *this;
		}
		//f Initialization.
		void Init( void )
		{
			E_CONTAINER_( description_ )::Init();
		}
		//f Adding 'Description'. Return the position where added.
		sdr::row__ Add( const description_ &Description )
		{
			return E_CONTAINER_( description_ )::Append( Description );
		}
		/*f See 'descriptions_::Add()' for the descriptions of the parameters.
		return the position where request description added. */
		sdr::row__ Add(
			const char *Name,
			const cast__ *Casts )
		{
			description Description;

			Description.Init( Name, Casts );

			return Add( Description );
		}
        template <typename... t> sdr::row__ Add(
			const char *Name,
            const t&... T )
         {
			description Description;

			Description.Init( Name, T... );

			return Add( Description );
         }
/*		sdr::row__ Add(
			const char *Name,
			cast__ Cast,
			va_list VL )
		{
			description Description;

			Description.Init( Name, Cast, VL );

			return Add( Description );
		}
		sdr::row__ Add(
			const char *Name,
			cast__ Cast,
			... )
		{
			sdr::row__ Row = qNIL;
			va_list VL;

			va_start( VL, Cast );

			Row = Add( Name, Cast, VL);

			va_end( VL );

			return Row;
		}
*/
        /*f Return the position of the description 'Description'. */
		sdr::row__ Position( const description_ &Description ) const;
	};

	E_AUTO( descriptions )
}

/*$END$*/
#endif
