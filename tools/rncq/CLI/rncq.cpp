/*
	Copyright (C) 2016 by Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of 'RNCq'.

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

# include "rncq.h"

#include "registry.h"

#include "scltool.h"
#include "sclerror.h"

#include "err.h"
#include "cio.h"
#include "epsmsc.h"
#include "xpp.h"
#include "fnm.h"
#include "flf.h"
#include "mthrtn.h"
#include "stkcrt.h"

using cio::CErr;
using cio::COut;
using cio::CIn;

namespace {
	void PrintHeader_( void )
	{
		COut << NAME_MC " V" VERSION << " (" WEBSITE_URL ")" << txf::nl;
		COut << "Copyright (C) " COPYRIGHT << txf::nl;
		COut << txf::pad << "Build : " __DATE__ " " __TIME__ << " (" << cpe::GetDescription() << ')' << txf::nl;
	}

	namespace {
		typedef stkcrt::qCSTACKdl( mthrtn::dRational ) dRationals;
		qW( Rationals );

		typedef stkbch::qBSTACKdl( bso::lfloat__ ) dFloats;
		qW( Floats );

		typedef stkbch::qBSTACKdl( bso::sChar ) dOperators;
		qW( Operators );

		bso::sBool GetInteger_(
			xtf::sIFlow &Flow,
			str::dString &Number )
		{
			if ( Flow.EndOfFlow() )
				return false;

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

			return true;
		}

		bso::lfloat__ GetInteger_(
			xtf::sIFlow &Flow,
			mthitg::dInteger &Integer )
		{
			bso::lfloat__ Result = 0;
		qRH
			str::wString String;
		qRB
			String.Init();
			GetInteger_( Flow, String );

			Integer.Init( String );

			Result = String.ToLF();

//			cio::COut << String << txf::tab << Integer << txf::nl;
		qRR
		qRT
		qRE
			return Result;
		}

		void Handle_(
			dRationals &Rationals,
			dFloats &Floats,
			dOperators &Operators )
		{
		qRH
			mthrtn::wRational ROp1, ROp2, RResult;
			bso::lfloat__ FOp1, FOp2, FResult;
		qRB
			if ( Operators.Amount() == 0 )
				qRFwk();

			if ( Rationals.Amount() < 2 )
				qRFwk();

			ROp1.Init();
			ROp2.Init();

			RResult.Init();

			Rationals.Pop( ROp2 );
			Rationals.Pop( ROp1 );

			Floats.Pop( FOp2 );
			Floats.Pop( FOp1 );

			switch ( Operators.Pop() ) {
			case '+':
				mthrtn::Add( ROp1, ROp2, RResult );
				FResult = FOp1 + FOp2;
				break;
			case '-':
				mthrtn::Sub( ROp1, ROp2, RResult );
				FResult = FOp1 - FOp2;
				break;
			case '*':
			case 'x':
				mthrtn::Mul( ROp1, ROp2, RResult );
				FResult = FOp1 * FOp2;
				break;
			case ':':
			case '/':
				mthrtn::Div( ROp1, ROp2, RResult );
				FResult = FOp1 / FOp2;
				break;
			default:
				qRFwk();
				break;
			}

			Rationals.Push( RResult );
			Floats.Push( FResult );
		qRR
		qRT
		qRE
		}

		namespace {
			bso::lfloat__ Evaluate_(
				xtf::sIFlow &Flow,
				mthrtn::dRational &Rational )
			{
				bso::lfloat__ Float = 0;
			qRH
				wRationals Rationals;
				wFloats Floats;
				mthitg::wInteger Integer;
				wOperators Operators;
				bso::sChar Operator = 0;
			qRB
				Rationals.Init();
				Floats.Init();
				Operators.Init();

				Integer.Init();
				Floats.Push( GetInteger_( Flow, Integer ) );
				Rationals.Push( mthrtn::wRational( Integer ) );

				while ( !Flow.EndOfFlow() ) {
					Operator = Flow.Get();
					Operators.Push( Operator );

					Integer.Init();
					Floats.Push( GetInteger_( Flow, Integer ) );
					Rationals.Push( mthrtn::wRational( Integer ) );

					Handle_( Rationals, Floats, Operators );
				}

				if ( Rationals.Amount() != 1 )
					qRFwk();

				Rationals.Pop( Rational );
				Rational.Simplify();

				Floats.Pop( Float );
			qRR
			qRT
			qRE
				return Float;
			}
		}

		bso::lfloat__ Evaluate_(
			const str::dString &Expression,
			mthrtn::dRational &Rational )
		{
			bso::lfloat__ Float = 0;
		qRH
			flx::sStringIFlow Flow;
			xtf::sIFlow XFlow;
		qRB
			Flow.Init( Expression );
			XFlow.Init( Flow, utf::f_Guess );

			Float = Evaluate_( XFlow, Rational );
		qRR
		qRT
		qRE
			return Float;
		}
	}

	void Test_( void )
	{
	qRH
		str::wString Expression;
		mthrtn::wRational Result;
		bso::lfloat__ Float = 0;
	qRB
		Expression.Init();
		sclmisc::MGetValue( registry::parameter::Expression, Expression );

		Result.Init();
		Float = Evaluate_( Expression, Result );
		cio::COut << '=' << Result.N << '/' << txf::nl << Result.D << txf::nl << Result.ToLongFloat() << txf::tab << Float << txf::nl;
	qRR
	qRT
	qRE
	}
}

#define C( name )\
	else if ( Command == #name )\
		name##_()

int scltool::SCLTOOLMain(
	const str::dString &Command,
	const scltool::fOddities &Oddities )
{
	int ExitValue = EXIT_FAILURE;
qRH
qRB
	if ( Command == "Version" )
		PrintHeader_();
	else if ( Command == "License" )
		epsmsc::PrintLicense( NAME_MC );
	C( Test );
	else
		qRGnr();

	ExitValue = EXIT_SUCCESS;
qRR
qRT
qRE
	return ExitValue;
}

const char *sclmisc::SCLMISCTargetName = NAME_LC;
const char *sclmisc::SCLMISCProductName = NAME_MC;

