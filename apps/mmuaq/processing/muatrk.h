/*
	Copyright (C) 2016-2017 Claude SIMON (http://zeusw.org/epeios/contact.html).

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

#ifndef MUATRK_INC_
# define MUATRK_INC_

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

	// Link from agent to mails.
	typedef crt::qCRATEd( muamel::dRows, muaagt::sRow ) dA2M_;

	// Tracker of the links between mails and agents.
	class dTracker
	{
	private:
		dM2A_ M2A_;
		dA2M_ A2M_;
		void AllocateM2A_( muamel::sRow Row )	// Allocate what's necessary to store item for 'Row'.
		{
			sdr::sSize Previous = M2A_.Amount();

			M2A_.Allocate( *Row + 1 );

			M2A_.FillWith( qNIL, Previous );
		}
	public:
		struct s {
			dM2A_::s M2A;
			dA2M_::s A2M;
		};
		dTracker( s &S )
		: M2A_( S.M2A ),
		  A2M_( S.A2M )
		{}
		void reset( bso::bool__ P = true )
		{
			tol::reset( P, M2A_, A2M_ );
		}
		void plug( qASd *AS )
		{
			tol::plug( AS, M2A_, A2M_ );
		}
		dTracker &operator =( const dTracker &T )
		{
			M2A_ = T.M2A_;
			A2M_ = T.A2M_;

			return *this;
		}
		void Init( void )
		{
			tol::Init( M2A_, A2M_ );
		}
		void Link(
			muamel::sRow Mail,
			muaagt::sRow Agent )
		{
			if ( M2A_.Exists( Mail ) ) {
				if ( M2A_( Mail ) != qNIL )
					qRGnr();	// A 'Remove(...)' for that mail should have be called before.
			} else
				AllocateM2A_( Mail );

			M2A_.Store( Agent, Mail );
			A2M_( Agent ).Add( Mail );
		}
		void Remove( muamel::sRow Mail )
		{
			muaagt::sRow Agent = M2A_( Mail );

			if ( Agent == qNIL )
				qRGnr();	// We should not remove twice the same mail/

			M2A_.Store( qNIL, Mail );
			A2M_(Agent).Remove( Mail );
		}
		void Remove( const muamel::dRows &Mails )
		{
			sdr::sRow Row = Mails.First();

			while ( Row != qNIL ) {
				Remove( Mails( Row ) );

				Row = Mails.Next( Row );
			}
		}
		const muamel::dRows &GetMails(
			muaagt::sRow Agent,
			muamel::dRows &Mails ) const
		{
			A2M_.Recall( Agent, Mails );

			return Mails;
		}
		const muamel::dRows &FilterOutMails(
			const muamel::dRows &List,
			muaagt::sRow Agent,
			muamel::dRows &Owned ) const
		{
			sdr::sRow Row = List.First();

			while ( Row != qNIL ) {
				if ( M2A_( List( Row  ) ) == Agent )
					Owned.Add( List( Row ) );

				Row = List.Next( Row );
			}

			return Owned;
		}
		void NewAgent( muaagt::sRow Agent )
		{
			if ( !A2M_.Exists( Agent ) ) {
				if ( A2M_.New() != Agent )
					qRGnr();
			}

			A2M_(Agent).Init();
		}
		void Remove( muaagt::sRow Agent );
		muaagt::sRow GetMailAgent( muamel::sRow Mail ) const
		{
			return M2A_( Mail );
		}
	};
}


#endif
