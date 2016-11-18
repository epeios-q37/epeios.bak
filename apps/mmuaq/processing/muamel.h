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

// MUA MAIL (somehow phonetically).

#ifndef MUAMEL__INC
# define MUAMEL__INC

# ifdef XXX_DBG
#	define MUAXMP__DBG
# endif

# include "muabsc.h"
# include "muaagt.h"

namespace muamel {
	typedef str::dString dId;
	typedef str::wString wId;

	class dMail {
	public:
		struct s {
			dId::s Id;
			muaagt::sRow Agent;
		} &S_;
		dId Id;
		dMail( s &S )
		: S_( S ),
		  Id( S.Id )
		{}
		void reset( bso::sBool P = true )
		{
			tol::reset( P, Id );
			S_.Agent = qNIL;
		}
		void plug( uys::sHook &Hook )
		{
			Id.plug( Hook );
		}
		void plug( qASd *AS )
		{
			tol::plug( AS, Id );
		}
		dMail &operator = (const dMail &M)
		{
			S_.Agent = M.S_.Agent;
			Id = M.Id;

			return *this;
		}
		void Init( void )
		{
			tol::Init( Id );
			S_.Agent = qNIL;
		}
		void Init(
			const dId &Id,
			muaagt::sRow Agent )
		{
			this->Id.Init( Id );
			S_.Agent = Agent;
		}
	};

	qROW( Row );

	typedef lstcrt::qLMCRATEd( dMail, sRow ) dMails;
	qW( Mails );
}


#endif
