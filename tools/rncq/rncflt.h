/*
	Copyright (C) 2016 by Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of 'RNCq.

    'RNCq' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'RNCq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'RNCq'.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef RNCFLT_INC_
# define RNCFLT_INC_

# include "stkcrt.h"
# include "str.h"
# include "tol.h"
# include "xtf.h"

namespace rncflt {
	typedef tol::dObject<bso::sLFloat> dFloat_;

	class dFloat
	: public dFloat_ {
	public:
		struct s
		: public dFloat_::s {};
		dFloat( s &S )
		: dFloat_( S )
		{}
		void Init( void )
		{
			S_.Object = 0;
		}
		bso::sBool Init( const str::dString &Value )
		{
			sdr::sRow P = qNIL;

			S_.Object = Value.ToLF( &P );

			return  P == qNIL;
		}
	};

	qW( Float );

	typedef stkcrt::qMCSTACKdl( dFloat_ ) dFloats;
	qW( Floats );

	inline void Print(
		bso::lfloat__ Float,
		txf::sOFlow &Flow )
	{
		char Buffer[1000];

		sprintf( Buffer, "%LF", Float );
		Flow << Buffer << txf::nl;

		sprintf( Buffer, "%LG", Float );
		Flow << Buffer;
	}
}

namespace rnc{
	using rncflt::dFloat;

	inline bso::sBool GetNumber(
		xtf::sIFlow &Flow,
		str::dString &Number )
	{
		if ( Flow.EndOfFlow() )
			return true;

		switch ( Flow.View() ) {
		case '-':
			Number.Append( '-' );
		case '+':
			Flow.Get();
		default:
			break;
		}

		while ( !Flow.EndOfFlow() && isdigit( Flow.View() ) )
			Number.Append( Flow.Get() );

		if ( !Flow.EndOfFlow() ) {
			bso::sChar C = Flow.View();
			if ( ( C == '.' ) || ( C == ',' ) ) {
				Number.Append( Flow.Get() );

				while ( !Flow.EndOfFlow() && isdigit( Flow.View() ) )
					Number.Append( Flow.Get() );
			}
		}

		return true;
	}

	inline bso::sBool GetNumber(
		xtf::sIFlow &Flow,
		rncflt::dFloat &Number )
	{
		bso::sBool Success = false;
	qRH
		str::wString String;
	qRB
		String.Init();

		if ( ( Success = GetNumber( Flow, String ) ) )	// '( ( ... ) )' to avoid a warning by 'clang'.
			Success = Number.Init( String );
	qRR
	qRT
	qRE
		return Success;
	}

	inline void Add(
		const dFloat &Op1,
		const dFloat &Op2,
		dFloat &Result )
	{
		Result.S_.Object = Op1.S_.Object + Op2.S_.Object;
	}

	inline void Sub(
		const dFloat &Op1,
		const dFloat &Op2,
		dFloat &Result )
	{
		Result.S_.Object = Op1.S_.Object - Op2.S_.Object;
	}

	inline void Mul(
		const dFloat &Op1,
		const dFloat &Op2,
		dFloat &Result )
	{
		Result.S_.Object = Op1.S_.Object * Op2.S_.Object;
	}

	inline void Div(
		const dFloat &Op1,
		const dFloat &Op2,
		dFloat &Result )
	{
		Result.S_.Object = Op1.S_.Object / Op2.S_.Object;
	}
}

#endif
