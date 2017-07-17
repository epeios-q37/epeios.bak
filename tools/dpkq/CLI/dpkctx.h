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
	typedef bso::sU8 sBoxId;

#	define DPKCTX_AMOUNT_MAX	EPEIOS_SIZE_MAX

	// Related to spaced repetition implementation.
	// List of records which are in a box.
	qTMIMICdw( RRows, Box );

	// Box row.
	qROW( BRow );

	// The second box is at row 0, the third at row 1 and so on.
	/* The content of the first box is not stored, as all records
	which are not in the other boxes are considered as stored in the first box. */
	typedef crt::qCRATEd( dBox, sBRow ) dBoxes;
	qW( Boxes );

	qTMIMICdw( RRows, Pool );

	class context_ {
	private:
		void MoveToBox_(
			const dRRows & Records,
			dBox& Box );
		void MoveRecordsToBox_(
			sBRow Source,
			sBRow Target );
		void BringBackRecordsToBox_( sBRow Target );	// Move all records in boxes after box 'Target', to box 'Target'.
	public:
		struct s {
			dBoxes::s Boxes;
			dPool::s Pool;
			sBoxId BoxId;
			amount__
				Session,	// Amount of record of the current session.
				Cycle;		// To ensure that, inside a cycle, a record is only picked once.
			time_t TimeStamp;
		} &S_;
		dBoxes Boxes;
		dPool Pool;
		context_( s &S )
		: S_( S ),
		  Boxes( S_.Boxes ),
		  Pool( S.Pool )
		{}
		void reset( bso::bool__ P = true )
		{
			tol::reset( P, Boxes, Pool );

			S_.BoxId = 0;
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
			tol::Init( Boxes, Pool );

			S_.Session = S_.Cycle = 0;
			S_.TimeStamp = 0;
		}
		void AdjustBoxesAmount( amount__ Amount );	// '0' and '1' are identical : all boxes are erased, so all records are randomly picked.
		sRRow Pick(
			amount__ Amount,
			bso::uint__ SessionDuration ); // In minute; '0' for infinite.
		
	};

	E_AUTO( context );

	void Dump(
		const context_ &Context,
		xml::writer_ &Writer );

	void Retrieve(
		xml::parser___ &Parser,
		amount__ BoxesAmount,	// '0' and '1' are identical : all record are randomly picked.
		context_ &Context );

};

#endif

