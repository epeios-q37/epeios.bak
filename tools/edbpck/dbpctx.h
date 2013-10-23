/*
	Header for the 'dbpctx' module by Claude SIMON (http://zeusw.org/epeios/contact.html)
	Copyright (C) 2010 Claude SIMON.
*/

// $Id: dbpctx.h,v 1.8 2013/05/22 17:40:12 csimon Exp $

#ifndef DBPCTX__INC
# define DBPCTX__INC

# include "bch.h"
# include "dbpbsc.h"
# include "xml.h"

namespace dbpctx {
	using namespace dbpbsc;

	typedef bso::u8__ coeff__;

	typedef sdr::size__ amount__;
#	define DBPCTX_AMOUNT_MAX	EPEIOS_SIZE_MAX

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
		void plug( sdr::E_SDRIVER__ &SD )
		{
			Pool.plug( SD );
		}
		void plug( ags::E_ASTORAGE_ &AS )
		{
			Pool.plug( AS );
		}
		context_ &operator =( const context_ &C )
		{
			Pool = C.Pool;
			S_.Cycle = C.S_.Cycle;
			S_.Session = C.S_.Session;
			S_.TimeStamp = C.S_.TimeStamp;
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

