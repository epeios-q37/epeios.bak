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
#define D( name )	qCDEF( char *, name, #name )
			D( Overflow );
			D( SyntaxError );
			D( BadFloatInvolvement );
#undef D
		}

# define REPORT( m )	sclmisc::ReportAndAbort( message_::m )

		namespace {
			typedef tol::dObject<bso::sLFloat> dFloat_;

			class dFloat
			: public dFloat_
			{
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
				void Init( const str::dString &Value )
				{
					sdr::sRow P = qNIL;

					S_.Object = Value.ToLF( &P );

					if ( P != qNIL ) {
						if ( isdigit( *P ) )
							REPORT( Overflow );
						else
							REPORT( SyntaxError );
					}
				}
			};

			qW( Float );

			typedef stkbch::qBSTACKdl( bso::sChar ) dOperators_;
			qW( Operators_ );

			void GetNumber_(
				xtf::sIFlow &Flow,
				str::dString *Number )
			{
				if ( Flow.EndOfFlow() )
					return;

				switch ( Flow.View() ) {
				case '-':
					Number->Append( '-' );
				case '+':
					Flow.Get();
				default:
					break;
				}

				while ( !Flow.EndOfFlow() && isdigit( Flow.View() ) )
					Number->Append( Flow.Get() );
			} 

			inline void GetNumber_(
				xtf::sIFlow &Flow,
				str::dString &Number )
			{
				return GetNumber_( Flow, &Number );
			}

			inline void GetNumber_(
				xtf::sIFlow &Flow,
				str::wString &Number )
			{
				return GetNumber_( Flow, &Number );
			}

			template <typename type> void  GetNumber_(
				xtf::sIFlow &Flow,
				type &Number )
			{
			qRH
				str::wString String;
			qRB
				String.Init();
				GetNumber_( Flow, String );

				Number.Init( String );
			qRR
			qRT
			qRE
			}

			inline void Add_(
				const mthrtn::dRational &Op1,
				const mthrtn::dRational &Op2,
				mthrtn::dRational &Result )
			{
				mthrtn::Add( Op1, Op2, Result );
			}

			inline void Sub_(
				const mthrtn::dRational &Op1,
				const mthrtn::dRational &Op2,
				mthrtn::dRational &Result )
			{
				mthrtn::Sub( Op1, Op2, Result );
			}

			inline void Mul_(
				const mthrtn::dRational &Op1,
				const mthrtn::dRational &Op2,
				mthrtn::dRational &Result )
			{
				mthrtn::Mul( Op1, Op2, Result );
			}

			inline void Div_(
				const mthrtn::dRational &Op1,
				const mthrtn::dRational &Op2,
				mthrtn::dRational &Result )
			{
				mthrtn::Div( Op1, Op2, Result );
			}

			inline void Add_(
				const dFloat &Op1,
				const dFloat &Op2,
				dFloat &Result )
			{
				Result.S_.Object = Op1.S_.Object + Op2.S_.Object;
			}

			inline void Sub_(
				const dFloat &Op1,
				const dFloat &Op2,
				dFloat &Result )
			{
				Result.S_.Object = Op1.S_.Object - Op2.S_.Object;
			}

			inline void Mul_(
				const dFloat &Op1,
				const dFloat &Op2,
				dFloat &Result )
			{
				Result.S_.Object = Op1.S_.Object * Op2.S_.Object;
			}

			inline void Div_(
				const dFloat &Op1,
				const dFloat &Op2,
				dFloat &Result )
			{
				Result.S_.Object = Op1.S_.Object / Op2.S_.Object;
			}

			inline void Print_(
				bso::lfloat__ Float,
				txf::sOFlow &Flow )
			{
				char Buffer[1000];

				sprintf( Buffer, "%LF", Float );
				Flow << Buffer << txf::nl;

				sprintf( Buffer, "%LG", Float );
				Flow << Buffer;
			}

			template <typename wnumber, typename dnumbers> void Handle_(
				dnumbers &Numbers,
				bso::sChar Operator )
			{
			qRH
				wnumber Op1, Op2, Result;
			qRB
				if ( Numbers.Amount() < 2 )
					qRFwk();

				Op1.Init();
				Op2.Init();

				Result.Init();

				Numbers.Pop( Op2 );
				Numbers.Pop( Op1 );

				switch ( Operator ) {
				case '+':
					Add_( Op1, Op2, Result );
					break;
				case '-':
					Sub_( Op1, Op2, Result );
					break;
				case '*':
				case 'x':
					Mul_( Op1, Op2, Result );
					break;
				case ':':
				case '/':
					Div_( Op1, Op2, Result );
					break;
				default:
					qRFwk();
					break;
				}

				Numbers.Push( Result );
			qRR
			qRT
			qRE
			}

			namespace algebraic_ {
				namespace {
					template <typename dnumber, typename wnumber, typename dnumbers> void Handle_(
						dnumbers &Numbers,
						dOperators_ &Operators )
					{
						::Handle_<wnumber, dnumbers>( Numbers, Operators.Pop() );
					}
				}

				template <typename dnumber, typename wnumber, typename dnumbers, typename wnumbers> void Evaluate(
					xtf::sIFlow &Flow,
					dnumber &Rational )
				{
				qRH
					wnumbers Numbers;
					wnumber Number;
					wOperators_ Operators;
					bso::sChar Operator = 0;
				qRB
					Numbers.Init();
					Operators.Init();

					Number.Init();
					GetNumber_( Flow, Number );
					Numbers.Push( Number );

					while ( !Flow.EndOfFlow() ) {
						Operator = Flow.Get();
						Operators.Push( Operator );

						Number.Init();
						GetNumber_( Flow, Number );
						Numbers.Push( Number );

						Handle_<dnumber, wnumber, dnumbers>( Numbers, Operators );
					}

					if ( Numbers.Amount() != 1 )
						qRFwk();

					Numbers.Pop( Rational );
				qRR
				qRT
				qRE
				}
			}

			namespace rpn_ {
				template <typename dnumber, typename wnumber, typename dnumbers, typename wnumbers> void Evaluate(
					xtf::sIFlow &Flow,
					dnumber &Rational )
				{
				qRH
					wnumbers Numbers;
					wnumber Number;
					wOperators_ Operators;
					bso::sChar Operator = 0;
				qRB
					Numbers.Init();
					Operators.Init();

					Number.Init();
					GetNumber_( Flow, Number );
					Numbers.Push( Number );

					while ( !Flow.EndOfFlow() ) {
						if ( isdigit( Flow.View() ) ) {
							Number.Init();
							GetNumber_( Flow, Number );
							Numbers.Push( Number );
						} else {
							Operator = Flow.Get();

							if ( Operator == '@' ) {
								Number.Init();
								GetNumber_( Flow, Number );
								Numbers.Push( Number );
							} else
								Handle_<wnumber, dnumbers>( Numbers, Operator );
						}
					}

					if ( Numbers.Amount() != 1 )
						qRFwk();

					Numbers.Pop( Rational );
				qRR
				qRT
				qRE
				}
			}

			template <typename dnumber, typename wnumber, typename dnumbers, typename wnumbers> void Evaluate_(
				xtf::sIFlow &Flow,
				dnumber &Number )
			{
				rpn_::Evaluate<dnumber, wnumber, dnumbers, wnumbers>( Flow, Number );
			}

			typedef stkcrt::qCSTACKdl( mthrtn::dRational ) dRationals_;

			qW( Rationals_ );
			typedef stkcrt::qMCSTACKdl( dFloat_ ) dFloats_;
			qW( Floats_ );
		}

		namespace rational_ {
			void Evaluate(
				xtf::sIFlow &IFlow,
				bso::sBool ResultWithFloat,
				txf::sOFlow &OFlow )
			{
			qRH
				mthrtn::wRational Result;
			qRB
				Result.Init();
				Evaluate_<mthrtn::dRational, mthrtn::wRational, dRationals_, wRationals_>( IFlow, Result );

				Result.Simplify();

				if ( ResultWithFloat )
					Print_( Result.GetLongFloat(), OFlow );
				else
					OFlow << Result.N << " / " << Result.D << txf::nl;
			qRR
			qRT
			qRE
			}
		}

		namespace float_ {
			void Evaluate(
				xtf::sIFlow &IFlow,
				txf::sOFlow &OFlow )
			{
			qRH
				wFloat Result;
			qRB
				Result.Init();
				Evaluate_<dFloat, wFloat, dFloats_, wFloats_>( IFlow, Result );

				Print_( Result.S_.Object, OFlow );
			qRR
			qRT
			qRE
			}
		}

		namespace floatp_ {
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

				const char *GetLabel_( eInvolvement Involvement )
				{
					switch ( Involvement ) {
#define C( name )	case i##name : return #name; break
						C( No );
						C( Result );
						C( All );
#undef C
					default:
						qRFwk();
						break;
					}

					return NULL;	// To avoid a warning.
				}
			}

			void Fill( void )
			{
				Automat_.Init();

				stsfsm::Fill( Automat_, i_amount, GetLabel_ );
			}

			eInvolvement GetInvolvement( const str::dString &Pattern )
			{
				return stsfsm::GetId( Pattern, Automat_, i_Undefined, i_amount );
			}
		}

		void Evaluate_(
			const str::dString &Expression,
			txf::sOFlow &Flow )
		{
		qRH
			flx::sStringIFlow SFlow;
			xtf::sIFlow XFlow;
			bso::sBool Float = false;
			str::wString Involvement;
		qRB
			SFlow.Init( Expression );
			XFlow.Init( SFlow, utf::f_Guess );

			floatp_::Fill();
			Involvement.Init();
			sclmisc::MGetValue( registry::parameter::Float, Involvement );

			switch ( floatp_::GetInvolvement( Involvement ) ) {
			case floatp_::iNo:
				rational_::Evaluate( XFlow, false, Flow );
				break;
			case floatp_::iResult:
				rational_::Evaluate( XFlow, true, Flow );
				break;
			case floatp_::iAll:
				float_::Evaluate( XFlow, cio::COut );
				break;
			default:
				REPORT( BadFloatInvolvement );
				break;
			}
		qRR
		qRT
		qRE
		}
	}

	void Test_( void )
	{
	qRH
		str::wString Expression;
	qRB
		Expression.Init();
		sclmisc::MGetValue( registry::parameter::Expression, Expression );

		Evaluate_( Expression, cio::COut );

		cio::COut.Commit();
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

