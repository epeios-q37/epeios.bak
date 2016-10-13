/*
	Copyright (C) 2016 Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of 'orgnzq' software.

    'orgnzq' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'orgnzq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'orgnzq'.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "sclbacknd.h"
#include "sclmisc.h"

#include "registry.h"
#include "ogzfbc.h"
#include "ogzinf.h"

#include "fdr.h"

#include "fblbkd.h"

#include "wrpunbound.h"
#include "wrpcolumn.h"
#include "wrpfield.h"
#include "wrpexample.h"

#define BACKEND_NAME	OGZINF_LC_AFFIX	"bkd"
#define COPYRIGHT		COPYRIGHT_YEARS " " OGZINF_OWNER_NAME " (" OGZINF_OWNER_CONTACT ")"	
#define API_VERSION		"1"

typedef sclbacknd::rCallback rCallback_;

typedef sclbacknd::rBackend	rBackend_;

class rBackend
: public rBackend_
{
private:
	FBLBKD_RAM_MODULE( wrpcolumn::wColumn ) Column_;
	FBLBKD_RAM_MODULE( wrpfield::wField ) Field_;
	FBLBKD_RAM_MODULE( wrpexample::wMyObject ) MyObject_;
	common::rStuff Stuff_;
	void *SCLBACKNDStuff( void ) override
	{
		return &Stuff_;
	}
public:
	void reset( bso::bool__ P = true )
	{
		rBackend_::reset( P );
		Stuff_.reset( P );
		Column_.reset( P );
		Field_.reset( P );
		MyObject_.reset( P );
	}
	qCVDTOR( rBackend );
	void Init(
		fblbur::eMode Mode,
		const ntvstr::char__ *ClientOrigin )
	{
		Stuff_.Init( *this );

		rBackend_::Init(
			Mode,
			API_VERSION,
			ClientOrigin,
			OGZINF_LC_AFFIX,
			BACKEND_NAME " " VERSION,
			COPYRIGHT,
			OGZINF_SOFTWARE_NAME " V" OGZINF_SOFTWARE_VERSION );

		wrpunbound::Inform( *this );

		Column_.Init( *this );
		Add( Column_ );

		Field_.Init( *this );
		Add( Field_ );

		MyObject_.Init( *this );
		Add( MyObject_ );
	}
};

// Dans cette classe, on place ce qui est commun  tous les 'backend's.
class rCallback
: public rCallback_
{
protected:
	virtual rBackend_ *SCLBACKNDNew( 
		fblbur::eMode Mode,
		const ntvstr::char__ *Origin ) override
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
	void Init( fblbur::eMode Mode )
	{
		rCallback_::Init( Mode );
	}
};

rCallback_ *sclbacknd::SCLBACKNDGetCallback(
	csdleo::context__ Context,
	fblbur::eMode Mode )
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
const char *sclmisc::SCLMISCProductName = OGZINF_MC_AFFIX;

