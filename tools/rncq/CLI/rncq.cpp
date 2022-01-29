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

#include "sclt.h"
#include "scle.h"
#include "sclm.h"

#include "err.h"
#include "cio.h"
#include "epsmsc.h"
#include "xpp.h"
#include "fnm.h"
#include "flf.h"
#include "mthrtn.h"
#include "stkcrt.h"

SCLI_DEF( rncq, NAME_LC, NAME_MC );

const scli::sInfo &sclt::SCLTInfo( void )
{
	return rncq::Info;
}

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

# define REPORT( m )	sclm::ReportAndAbort( message_::m )

		namespace {
			template <typename dnumber, typename wnumber, typename dnumbers, typename wnumbers> bso::sBool Evaluate_(
				xtf::sRFlow &Flow,
				dnumber &Number )
			{
				if ( sclm::MGetBoolean( registry::parameter::RPN ) )
					return rncrpn::Evaluate<dnumber, wnumber, dnumbers, wnumbers>( Flow, Number );
				else
					return rncalg::Evaluate<dnumber, wnumber, dnumbers, wnumbers>( Flow, Number );
			}
		}

		namespace rational_ {
			bso::sBool Evaluate(
				xtf::sRFlow &IFlow,
				txf::sWFlow &OFlow )
			{
				bso::sBool Success = false;
			qRH
				mthrtn::wRational Result;
			qRB
				Result.Init();

				if ( ( Success = Evaluate_<mthrtn::dRational, mthrtn::wRational, rncrtn::dRationals, rncrtn::wRationals>( IFlow, Result ) ) ) {	// '( ( ... ) )' to avoid a warning by 'clang'.
					Result.Simplify();

					if ( sclm::MGetBoolean( registry::parameter::ToFloat ) )
						rncflt::Print( Result.GetLongFloat(), OFlow );
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
				xtf::sRFlow &IFlow,
				txf::sWFlow &OFlow )
			{
				bso::sBool Success = false;
			qRH
				rncflt::wFloat Result;
			qRB
				Result.Init();
				Success = Evaluate_<rncflt::dFloat, rncflt::wFloat, rncflt::dFloats, rncflt::wFloats>( IFlow, Result );

				rncflt::Print( Result.S_.Object, OFlow );
			qRR
			qRT
			qRE
				return Success;
			}
		}

		void Evaluate_(
			const str::dString &Expression,
			txf::sWFlow &Flow )
		{
		qRH
			flx::sStringIFlow SFlow;
			xtf::sRFlow XFlow;
			bso::sBool Success = false;
		qRB
			SFlow.Init( Expression );
			XFlow.Init( SFlow, utf::f_Guess );

			if ( sclm::MGetBoolean( registry::parameter::UseFloat ) )
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

	void Evaluate_( void )
	{
	qRH
		str::wString Expression;
	qRB
		Expression.Init();
		sclm::MGetValue( registry::parameter::Expression, Expression );

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

int sclt::SCLTMain(
	const str::dString &Command,
	const sclt::fOddities &Oddities )
{
	int ExitValue = EXIT_FAILURE;
qRH
qRB
	if ( Command == "Version" )
		PrintHeader_();
	else if ( Command == "License" )
		epsmsc::PrintLicense( NAME_MC );
	C( Evaluate );
	else
		qRGnr();

	ExitValue = EXIT_SUCCESS;
qRR
qRT
qRE
	return ExitValue;
}
