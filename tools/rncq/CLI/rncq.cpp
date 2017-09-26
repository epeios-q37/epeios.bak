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
#include "rncflt.h"
#include "rncrtn.h"

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
	using namespace rncflt;
	using namespace rncrtn;

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

			inline void SkipSpaces_( xtf::sIFlow &Flow )
			{
				while ( !Flow.EndOfFlow() && ( Flow.View() == ' ' ) )
					Flow.Get();
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

				if ( Success = Evaluate_<mthrtn::dRational, mthrtn::wRational, dRationals, wRationals>( IFlow, Result ) ) {
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
				Success = Evaluate_<dFloat, wFloat, dFloats, wFloats>( IFlow, Result );

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

