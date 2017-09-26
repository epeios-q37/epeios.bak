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

#include "rncalg.h"
#include "rncrpn.h"

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
			D( BadExpression );
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

			inline void SkipSpaces_( xtf::sIFlow &Flow )
			{
				while ( !Flow.EndOfFlow() && ( Flow.View() == ' ' ) )
					Flow.Get();
			}

			bso::sBool GetNumber_(
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

			bso::sBool GetNumber_(
				xtf::sIFlow &Flow,
				dFloat &Number )
			{
				bso::sBool Success = false;
			qRH
				str::wString String;
			qRB
				String.Init();

				if ( Success = GetNumber_( Flow, String ) )
					Number.Init( String );
			qRR
			qRT
			qRE
				return Success;
			}

			bso::sBool GetNumber_(
				xtf::sIFlow &Flow,
				mthrtn::dRational &Number )
			{
				Number.Init( Flow.UndelyingFlow() );

				return true;
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


			template <typename dnumber, typename wnumber, typename dnumbers, typename wnumbers> bso::sBool Evaluate_(
				xtf::sIFlow &Flow,
				dnumber &Number )
			{
				if ( sclmisc::BGetBoolean( registry::parameter::RPN ) )
					return rncrpn::Evaluate<dnumber, wnumber, dnumbers, wnumbers>( Flow, Number );
				else
					return rncalg::Evaluate<dnumber, wnumber, dnumbers, wnumbers>( Flow, Number );
			}

			typedef stkcrt::qCSTACKdl( mthrtn::dRational ) dRationals_;

			qW( Rationals_ );
			typedef stkcrt::qMCSTACKdl( dFloat_ ) dFloats_;
			qW( Floats_ );
		}

		namespace rational_ {
			bso::sBool Evaluate(
				xtf::sIFlow &IFlow,
				txf::sOFlow &OFlow )
			{
				bso::sBool Success = false;
			qRH
				mthrtn::wRational Result;
			qRB
				Result.Init();

				if ( Success = Evaluate_<mthrtn::dRational, mthrtn::wRational, dRationals_, wRationals_>( IFlow, Result ) ) {
					Result.Simplify();

					if ( sclmisc::BGetBoolean( registry::parameter::ToFloat ) )
						Print_( Result.GetLongFloat(), OFlow );
					else
						OFlow << Result.N << " / " << Result.D << txf::nl;
					}
				qRR
					qRT
					qRE
					return Success;
			}
		}

		namespace float_ {
			bso::sBool Evaluate(
				xtf::sIFlow &IFlow,
				txf::sOFlow &OFlow )
			{
				bso::sBool Success = false;
			qRH
				wFloat Result;
			qRB
				Result.Init();
				Success = Evaluate_<dFloat, wFloat, dFloats_, wFloats_>( IFlow, Result );

				Print_( Result.S_.Object, OFlow );
			qRR
			qRT
			qRE
				return Success;
			}
		}

		void Evaluate_(
			const str::dString &Expression,
			txf::sOFlow &Flow )
		{
		qRH
			flx::sStringIFlow SFlow;
			xtf::sIFlow XFlow;
			bso::sBool Success = false;
		qRB
			SFlow.Init( Expression );
			XFlow.Init( SFlow, utf::f_Guess );

			if ( sclmisc::BGetBoolean( registry::parameter::UseFloat ) )
				Success = float_::Evaluate( XFlow, cio::COut );
			else
				Success = rational_::Evaluate( XFlow, Flow );

			if ( !Success )
				REPORT( BadExpression );
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

