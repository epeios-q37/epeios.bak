/*
	Copyright (C) 2016 Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of 'MMUAq' software.

    'MMUAq' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'MMUAq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'MMUAq'.  If not, see <http://www.gnu.org/licenses/>.
*/

// MUA TRacKer (keeps trace of the mails between agents)

#ifndef MUATRK__INC
# define MUATRK__INC

# ifdef XXX_DBG
#	define MUATRK__DBG
# endif

# include "muaagt.h"
# include "muabsc.h"
# include "muamel.h"

# include "crt.h"

namespace muatrk {

	// Link from mail to agent.
	typedef bch::qBUNCHd( muaagt::sRow, muamel::sRow ) dM2A_;

	// Link from gent to mails.
	typedef crt::qCRATEd( muamel::dRows, muaagt::sRow ) dA2M_;

	class dTracker
	{
	private:
		void AllocateM2A_( muamel::sRow Row )	// Allocate what's necessary to store item for 'Row'.
		{
			sdr::sSize Previous = M2A.Amount();

			M2A.Allocate( *Row + 1 );

			M2A.FillWith( qNIL, Previous );
		}
	public:
		struct s {
			dM2A_::s M2A;
			dA2M_::s A2M;
		};
		dM2A_ M2A;
		dA2M_ A2M;
		dTracker( s &S )
		: M2A( S.M2A ),
		  A2M( S.A2M )
		{}
		void reset( bso::bool__ P = true )
		{
			tol::reset( P, M2A, A2M );
		}
		void plug( qASd *AS )
		{
			tol::plug( AS, M2A, A2M );
		}
		dTracker &operator =( const dTracker &T )
		{
			M2A = T.M2A;
			A2M = T.A2M;

			return *this;
		}
		void Init( void )
		{
			tol::Init( M2A, A2M );
		}
		void Link(
			muamel::sRow Mail,
			muaagt::sRow Agent )
		{
			if ( M2A.Exists( Mail ) ) {
				if ( M2A( Mail ) != qNIL )
					qRGnr();	// A 'Remove(...)' for that mail should have be called before.
			} else
				AllocateM2A_( Mail );

			M2A.Store( Agent, Mail );
			A2M( Agent ).Add( Mail );
		}
		void Remove( muamel::sRow Mail )
		{
			muaagt::sRow Agent = M2A( Mail );

			if ( Agent == qNIL )
				qRGnr();	// We should not remove twice the same mail/

			M2A.Store( qNIL, Mail );
			A2M(Agent).Remove( Mail );
		}
		const muamel::dRows &GetMails(
			muaagt::sRow Agent,
			muamel::dRows &Mails ) const
		{
			A2M.Recall( Agent, Mails );

			return Mails;
		}
		const muamel::dRows &FilterOutMails(
			const muamel::dRows &List,
			muaagt::sRow Agent,
			muamel::dRows &Owned ) const
		{
			sdr::sRow Row = List.First();

			while ( Row != qNIL ) {
				if ( M2A( List( Row  ) ) == Agent )
					Owned.Add( List( Row ) );

				Row = List.Next( Row );
			}

			return Owned;
		}
		void NewAgent( muaagt::sRow Agent )
		{
			if ( !A2M.Exists( Agent ) ) {
				if ( A2M.New() != Agent )
					qRGnr();
			}

			A2M(Agent).Init();
		}
		muaagt::sRow GetMailAgent( muamel::sRow Mail ) const
		{
			return M2A( Mail );
		}
	};
}


#endif
