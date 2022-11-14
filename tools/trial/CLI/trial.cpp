/*
	Copyright (C) 2019 Claude SIMON (http://q37.info/contact/).

	This file is part of the 'Trial' tool.

    'Trial' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'Trial' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'Trial'.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "trial.h"

#include "registry.h"
#include "trial3.h"

#include "sclt.h"
#include "scle.h"
#include "sclm.h"

#include "err.h"
#include "cio.h"
#include "epsmsc.h"
#include "xpp.h"
#include "fnm.h"
#include "flf.h"
#include "csdcmn.h"

using cio::CErr;
using cio::COut;
using cio::CIn;

SCLI_DEF( trial, NAME_LC, NAME_MC );

const scli::sInfo &sclt::SCLTInfo( void )
{
	return trial::Info;
}

namespace {
	void PrintHeader_( void )
	{
		COut << NAME_MC " V" VERSION << " (" WEBSITE_URL ")" << txf::nl;
		COut << "Copyright (C) " COPYRIGHT << txf::nl;
		COut << txf::pad << "Build : " __DATE__ " " __TIME__ << " (" << cpe::GetDescription() << ')' << txf::nl;
	}

	namespace t1 {
        typedef bso::sS16 sValue;

        void Write(
            sValue Value,
            str::dString &Buffer)
        {
        qRH
            flx::rStringWFlow Flow;
        qRB
            Flow.Init(Buffer);
            csdcmn::Put(Value, Flow);
        qRR
        qRT
        qRE
        }

        sValue Read(const str::dString &Buffer)
        {
            sValue Value = 0;

            flx::sStringRFlow Flow;

            Flow.Init( Buffer);

            return csdcmn::Get(Flow, Value);
        }
	}

	void T1_( void )
	{
	qRH
        str::wString Buffer;
	qRB
        Buffer.Init();

        t1::Write(sclm::MGetS16(registry::parameter::t1::Value), Buffer);

        cio::COut << t1::Read(Buffer) << txf::nl << txf::commit;
    qRR
    qRT
    qRE
	}

	namespace t2 {
		class child {
		protected:
			char c_;
		public:
			child(char c) {
				c_ = c;
				cio::COut << ">> Child Ctor: " << c_ << txf::nl << txf::commit;
			}
			virtual ~child() {
				cio::COut << ">> Child Dtor: " << c_ << txf::nl << txf::commit;
			}
		};

		class parent
		: public child
		{
		public:
			parent(char c)
			: child(c)
			{
				cio::COut << ">> Parent Ctor: " << c_ << txf::nl << txf::commit;
			}
			~parent() {
				cio::COut << ">> Parent Dtor: " << c_ << txf::nl << txf::commit;
			}
		};

		void DoNothing(const child &){}

		const child &Convert(const child &C)
		{
			return C;
		}
	}

	#define M(e)\
		cio::COut << txf::nl << #e << txf::nl << txf::commit;\
		DoNothing(true ? e)

	void T2_(void) {
		t2::parent A('A'), B('B');

		M(A : B);
		M( Convert(A) : B );
		M( Convert(A) : Convert(B) );
		M( A : t2::parent('C'));
		M( Convert(A) : t2::parent('D'));
		M( t2::parent('E') : A );
		M( t2::parent('F') : Convert(A));
		M( Convert(A) : Convert(t2::parent('G')));
		M( Convert(t2::parent('H')) : A );
		M( Convert(t2::parent('I')) : Convert(A));

		cio::COut << txf::nl << "End" << txf::nl << txf::commit;
	}

	void T3_(void)
	{
	  trial3::Launch();
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
qRH;
qRB;
	if ( Command == "Version" )
		PrintHeader_();
	else if ( Command == "License" )
		epsmsc::PrintLicense( NAME_MC );
	C( T1 );
	C( T2 );
	C( T3 );
	else
		qRGnr();

	ExitValue = EXIT_SUCCESS;
qRR;
qRT;
qRE;
	return ExitValue;
}
