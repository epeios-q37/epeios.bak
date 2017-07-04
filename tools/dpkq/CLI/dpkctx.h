/*
	Copyright (C) 2010-2015 Claude SIMON (http://q37.info/contact/).

	This file is part of dpkq.

    dpkq is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

    dpkq is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with dpkq.  If not, see <http://www.gnu.org/licenses/>
*/

#ifndef DPKCTX__INC
# define DPKCTX__INC

# include "bch.h"
# include "dpkbsc.h"
# include "xml.h"

namespace dpkctx {
	using namespace dpkbsc;

	typedef bso::u8__ coeff__;

	typedef sdr::size__ amount__;
#	define DPKCTX_AMOUNT_MAX	EPEIOS_SIZE_MAX

	E_TMIMIC( rrows, pool );

	class context_ {
	public:
		struct s {
			pool_::s Pool;
			amount__
				Session,	// Amount of record of the current session.
				Cycle;		// To ensure that, inside a cycle, a record is only picked once.
			time_t TimeStamp;
		} &S_;
		pool_ Pool;
		context_( s &S )
		: S_( S ),
		  Pool( S.Pool )
		{}
		void reset( bso::bool__ P = true )
		{
			Pool.reset( P );

			S_.Session = S_.Cycle = 0;
			S_.TimeStamp = 0;
		}
		void plug( bch::sHook &Hook )
		{
			Pool.plug( Hook );
		}
		void plug( qASd *AS )
		{
			Pool.plug( AS );
		}
		context_ &operator =( const context_ &C )
		{
			Pool = C.Pool;
			S_.Cycle = C.S_.Cycle;
			S_.Session = C.S_.Session;
			S_.TimeStamp = C.S_.TimeStamp;

			return *this;
		}
		void Init( void )
		{
			Pool.Init();

			S_.Session = S_.Cycle = 0;
			S_.TimeStamp = 0;
		}
		rrow__ Pick(
			amount__ Amount,
			bso::uint__ SessionDuration );	// In minute; '0' for infinite.
	};

	E_AUTO( context );

	void Dump(
		const context_ &Context,
		xml::writer_ &Writer );

	void Retrieve(
		xml::parser___ &Parser,
		context_ &Context );

};

#endif

