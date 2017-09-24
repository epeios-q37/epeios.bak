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
#include "sclmisc.h"

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
		namespace message_ {
			qCDEF( char *, Overflow, "Overflow" );
			qCDEF( char *, SyntaxError, "SyntaxError" );
		}

# define REPORT( m )	sclmisc::ReportAndAbort( message_::m )

		typedef stkbch::qBSTACKdl( bso::sChar ) dOperators_;
		qW( Operators_ );

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

		namespace float_ {
			namespace {
				typedef stkbch::qBSTACKdl( bso::lfloat__ ) dFloats_;
				qW( Floats_ );

				bso::lfloat__ GetInteger_( xtf::sIFlow &Flow )
				{
					bso::lfloat__ Result = 0;
				qRH
					str::wString String;
					sdr::sRow P = qNIL;
				qRB
					String.Init();
					::GetInteger_( Flow, String );

					Result = String.ToLF( &P );

					if ( P != qNIL ) {
						if ( isdigit( *P ) )
							REPORT( Overflow );
						else
							REPORT( SyntaxError );
					}
				qRR
				qRT
				qRE
					return Result;
				}

				void Handle_(
					dFloats_ &Floats,
					dOperators_ &Operators )
				{
					bso::lfloat__ FOp1, FOp2, FResult;

					if ( Operators.Amount() == 0 )
						qRFwk();

					if ( Floats.Amount() < 2 )
						qRFwk();

					Floats.Pop( FOp2 );
					Floats.Pop( FOp1 );

					switch ( Operators.Pop() ) {
					case '+':
						FResult = FOp1 + FOp2;
						break;
					case '-':
						FResult = FOp1 - FOp2;
						break;
					case '*':
					case 'x':
						FResult = FOp1 * FOp2;
						break;
					case ':':
					case '/':
						FResult = FOp1 / FOp2;
						break;
					default:
						qRFwk();
						break;
					}

					Floats.Push( FResult );
				}

				bso::lfloat__ Evaluate_( xtf::sIFlow &Flow )
				{
					bso::lfloat__ Float = 0;
					qRH
						wFloats_ Floats;
					wOperators_ Operators;
					bso::sChar Operator = 0;
				qRB
					Floats.Init();
					Operators.Init();

					Floats.Push( GetInteger_( Flow ) );

					while ( !Flow.EndOfFlow() ) {
						Operator = Flow.Get();
						Operators.Push( Operator );

						Floats.Push( GetInteger_( Flow ) );

						Handle_( Floats, Operators );
					}
					Floats.Pop( Float );
				qRR
				qRT
				qRE
					return Float;
				}
			}

			void Evaluate(
				xtf::sIFlow &IFlow,
				txf::sOFlow &OFlow )
			{
				OFlow << '=' << Evaluate_( IFlow ) << txf::nl;
			}
		}

	namespace rational_ {
		namespace {
			typedef stkcrt::qCSTACKdl( mthrtn::dRational ) dRationals_;
			qW( Rationals_ );
			
			void  GetInteger_(
				xtf::sIFlow &Flow,
				mthitg::dInteger &Integer )
			{
			qRH
				str::wString String;
			qRB
				String.Init();
				::GetInteger_( Flow, String );

				Integer.Init( String );
			qRR
			qRT
			qRE
			}

			void Handle_(
				dRationals_ &Rationals,
				dOperators_ &Operators )
			{
			qRH
				mthrtn::wRational ROp1, ROp2, RResult;
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

				switch ( Operators.Pop() ) {
				case '+':
					mthrtn::Add( ROp1, ROp2, RResult );
					break;
				case '-':
					mthrtn::Sub( ROp1, ROp2, RResult );
					break;
				case '*':
				case 'x':
					mthrtn::Mul( ROp1, ROp2, RResult );
					break;
				case ':':
				case '/':
					mthrtn::Div( ROp1, ROp2, RResult );
					break;
				default:
					qRFwk();
					break;
				}

				Rationals.Push( RResult );
			qRR
			qRT
			qRE
			}

			void Evaluate_(
					xtf::sIFlow &Flow,
					mthrtn::dRational &Rational )
				{
				qRH
					wRationals_ Rationals;
					mthitg::wInteger Integer;
					wOperators_ Operators;
					bso::sChar Operator = 0;
				qRB
					Rationals.Init();
					Operators.Init();

					Integer.Init();
					Rationals.Push( mthrtn::wRational( Integer ) );

					while ( !Flow.EndOfFlow() ) {
						Operator = Flow.Get();
						Operators.Push( Operator );

						Integer.Init();
						Rationals.Push( mthrtn::wRational( Integer ) );

						Handle_( Rationals, Operators );
					}

					if ( Rationals.Amount() != 1 )
						qRFwk();

					Rationals.Pop( Rational );
					Rational.Simplify();
				qRR
				qRT
				qRE
				}

			void Evaluate(
				xtf::sIFlow &IFlow,
				bso::sBool ResultWithFloat,
				txf::sOFlow &OFlow )
			{
			qRH
				mthrtn::wRational Result;
			qRB
				Result.Init();
				Evaluate_( IFlow, Result );

				OFlow << '=';

				if ( ResultWithFloat )
					OFlow << Result.GetLongFloat();
				else
					OFlow << Result.N << "/ " << Result.D << txf::nl;
			qRR
			qRT
			qRE
			}
		}

		namespace float_ {
			qENUM( Involvement )
			{
				iNo,		//	No involvement at all.
					iResult,	// Conversion of the result.
					iAll,		// All calculation are made with floats.
					i_amount,
					i_Undefined
			};

			namespace {
				stsfsm::wAutomat Automat_;

#define C( name )	case i##name : return #name; break
				const char *GetLabel_( eInvolvement Involvement )
				{
					switch ( Involvement ) {
					C( No );
					C( Result );
					C( All );
					default:
						qRFwk();
						break;
					}

					return NULL;	// To avoid a warning.
				}
#undef C

				void Fill( void )
				{
					Automat_.Init();

					stsfsm::Fill( Automat_, i_amount, GetLabel_ );
				}
			}

			eInvolvement GetInvolvement( const str::dString &Pattern )
			{
				return stsfsm::GetId( Pattern, Automat_, i_Undefined, i_amount );
			}
		}

		bso::lfloat__ Evaluate_(
			const str::dString &Expression,
			txf::sOFlow &Flow )
		{
			bso::lfloat__ Float = 0;
		qRH
			flx::sStringIFlow SFlow;
			xtf::sIFlow XFlow;
			bso::sBool Float = false;
			str::wString Involvement;
		qRB
			SFlow.Init( Expression );
			XFlow.Init( SFlow, utf::f_Guess );

			Involvement.Init();

			sclmisc::MGetValue( registry::parameter::Float, Involvement );

			switch ( float_::GetInvolvement( Involvement ) ) {
			case float_::iNo:
				rational_::Evaluate( XFlow, false, Flow );
				break;
			case float_::iResult:
				rational_::Evaluate( XFlow, true, Flow );
				break;
			case float_::iAll:
				float_::Evaluate( XFlow, cio::COut );
				break;
			default:
				REPORT( BadFlowInvolvement );
				break;
			}
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
	qRB
		Expression.Init();
		sclmisc::MGetValue( registry::parameter::Expression, Expression );

		Evaluate_( Expression, cio::Cout );
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

