/*
	Copyright (C) 2016 Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of 'MMUAq' software.

    'MMUAq' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'MMUAq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'MMUAq'.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "sclbacknd.h"
#include "sclmisc.h"

#include "registry.h"
#include "muafbc.h"
#include "muainf.h"

#include "fdr.h"

#include "fblbkd.h"

#include "wrpunbound.h"
#include "wrpexample.h"

#define BACKEND_NAME	MUAINF_LC_AFFIX	"bkd"
#define COPYRIGHT		COPYRIGHT_YEARS " " MUAINF_OWNER_NAME " (" MUAINF_OWNER_CONTACT ")"	
#define API_VERSION		"1"

typedef sclbacknd::rCallback rCallback_;

typedef sclbacknd::rBackend	rBackend_;

class rBackend
: public rBackend_
{
private:
	FBLBKD_RAM_MODULE( wrpexample::wMyObject ) MyObject_;
	common::rStuff Stuff_;
protected:
	void *SCLBACKNDStuff( void ) override
	{
		return &Stuff_;
	}
public:
	void reset( bso::bool__ P = true )
	{
		rBackend_::reset( P );
		Stuff_.reset( P );
		MyObject_.reset( P );
	}
	qCVDTOR( rBackend );
	void Init(
		fblovl::eMode Mode,
		const ntvstr::char__ *ClientOrigin )
	{
		Stuff_.Init();

		rBackend_::Init(
			Mode,
			API_VERSION,
			ClientOrigin,
			MUAINF_LC_AFFIX,
			BACKEND_NAME " V" VERSION,
			COPYRIGHT,
			MUAINF_SOFTWARE_NAME " V" MUAINF_SOFTWARE_VERSION );

		wrpunbound::Inform( *this );

		MyObject_.Init( *this );
		Add( MyObject_ );
	}
};

// Put here what is common for all backends.
class rCallback
: public rCallback_
{
protected:
	virtual rBackend *SCLBACKNDNew( 
		fblovl::eMode Mode,
		const ntvstr:: char__ *Origin ) override
	{
		rBackend *Backend = NULL;
	qRH
	qRB
		if ( ( Backend = new rBackend ) == NULL )
			qRAlc();

		Backend->Init( Mode, Origin );
	qRR
		if ( Backend != NULL )
			delete Backend;

		Backend = NULL;
	qRT
	qRE
		return Backend;
	}
public:
	void reset( bso::bool__ P = true )
	{
		rCallback_::reset( P );
	}
	qCVDTOR( rCallback )
	void Init( fblovl::eMode Mode )
	{
		rCallback_::Init( Mode );
	}
};

rCallback_ *sclbacknd::SCLBACKNDGetCallback(
	csdleo::context__ Context,
	fblovl::eMode Mode )
{
	rCallback *Callback = NULL;
qRH
qRB
	if ( Context == csdleo::cRegular ) {
		if ( common::IsInitialized() )
			qRGnr();

		common::Initialize();
	}

	if ( ( Callback = new ::rCallback ) == NULL )
		qRAlc();

	Callback->Init( Mode );
qRR
	if ( Callback != NULL )
		delete Callback;

	Callback = NULL;
qRT
qRE
	return Callback;
}

const char *sclmisc::SCLMISCTargetName = BACKEND_NAME;
const char *sclmisc::SCLMISCProductName = MUAINF_MC_AFFIX;
