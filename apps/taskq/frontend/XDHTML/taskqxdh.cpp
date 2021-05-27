/*
	Copyright (C) 2018 Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of 'TaskQ' software.

    'TaskQ' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'TaskQ' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'TaskQ'.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "taskqxdh.h"

#include "sclx.h"
#include "sclm.h"

#include "frdrgstry.h"

#include "core.h"
#include "login.h"
#include "main.h"
#include "prolog.h"

SCLI_DEF( taskqxdh, NAME, TSKINF_MC );

const scli::sInfo &sclx::SCLXInfo( void )
{
	return taskqxdh::Info;
}

void sclx::SCLXInitialization( xdhcdc::eMode Mode )
{
qRH
	sclf::rBackendFeatures Features;
qRB

		cio::COut << getpid() << txf::nl << txf::commit;


	core::Core.Init( Mode );

	if ( Mode == xdhcdc::mMultiUser ) {
		Features.Init();
		sclf::GuessBackendFeatures( Features );
		core::Kernel().Init( Features, plgn::EmptyAbstracts );
	}
qRR
qRT
qRE
}

xdhcdc::cSingle *sclx::SCLXFetchCallback(void)
{
	core::rSession *Session = new core::rSession;

	if ( Session == NULL )
		qRGnr();

		cio::COut << ">>>>>> " << getpid() << txf::nl << txf::commit;

	Session->Init(core::Kernel(), core::Core, taskqxdh::Info);

//	Session->Connect();

	return Session;
}

void sclx::SCLXDismissCallback( xdhcdc::cSingle *Callback )
{
	if ( Callback == NULL )
		qRGnr();

	delete Callback;
}
