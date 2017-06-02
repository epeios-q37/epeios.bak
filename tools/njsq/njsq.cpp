/*
	Copyright (C) 2016 by Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of 'NJSq'.

    'NJSq' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'NJSq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'NJSq'.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "registry.h"

#include "njscmn.h"
#include "nodeq.h"

#include "scltool.h"
#include "sclerror.h"

#include "err.h"
#include "cio.h"
#include "epsmsc.h"
#include "xpp.h"
#include "fnm.h"
#include "flf.h"

using cio::CErr;
using cio::COut;
using cio::CIn;

# define NAME_MC			"NJSq"
# define NAME_LC			"njsq"
# define NAME_UC			"NJSQ"
# define WEBSITE_URL		"http://q37.info"
# define AUTHOR_NAME		"Claude SIMON"
# define AUTHOR_CONTACT		"http://q37.info/contact/"
# define OWNER_NAME			"Claude SIMON"
# define OWNER_CONTACT		"http://q37.info/contact/"
# define COPYRIGHT			COPYRIGHT_YEARS " " OWNER_NAME " (" OWNER_CONTACT ")"	

namespace {
	void PrintHeader_( void )
	{
		COut << NAME_MC " V" VERSION << " (" WEBSITE_URL ")" << txf::nl;
		COut << "Copyright (C) " COPYRIGHT << txf::nl;
		COut << txf::pad << "Build : " __DATE__ " " __TIME__ << " (" << cpe::GetDescription() << ')' << txf::nl;
	}

	typedef njscmn::cArguments cArguments_;

	inline void GetString_(
		int Index,
		const v8::FunctionCallbackInfo<v8::Value> &Info,
		str::dString &Value )
	{
	qRH
		v8q::sString String;
	qRB
		String.Init( Info[Index] );

		String.Get( Value );
	qRR
	qRT
	qRE
	}

	void SetReturnValue_(
		const v8::FunctionCallbackInfo<v8::Value> &Info,
		const str::dString &Value )
	{
		Info.GetReturnValue().Set( v8q::sString( Value ).Core() );
	}

	class sArguments
	: public cArguments_
	{
	private:
		qRMV( const v8::FunctionCallbackInfo<v8::Value>, A_, Arguments_ );
	protected:
		virtual void NJSCMNGetValue(
			int Index,
			njscmn::eType Type,
			void *Value ) override
		{
			if ( Index >= A_().Length() )
				qRGnr();

			switch ( Type ) {
			case njscmn::tString:
				GetString_( Index, A_(), *(str::dString * )Value );
				break;
			default:
				qRGnr();
				break;
			}
		}
		virtual void NJSCMNSetReturnValue(
			njscmn::eType Type,
			const void *Value ) override
		{
			switch ( Type ) {
			case njscmn::tString:
				SetReturnValue_( A_(), *( const str::dString * )Value );
				break;
			default:
				qRGnr();
				break;
			}
		}
	};

	void Launch_( void )
	{
		sArguments Arguments;


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
	C( Launch );
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

