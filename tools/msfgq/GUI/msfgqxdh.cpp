/*
	Copyright (C) 2022 Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of 'MSFGq' software.

  'MSFGq' is free software: you can redistribute it and/or modify it
  under the terms of the GNU Affero General Public License as published
  by the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  'MSFGq' is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Affero General Public License for more details.

  You should have received a copy of the GNU Affero General Public License
  along with 'MSFGq'.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "msfgqxdh.h"

#include "sclx.h"

SCLI_DEF( msfgqxdh, NAME_LC	SCLX_DEFAULT_SUFFIX, NAME_MC );

const scli::sInfo &sclx::SCLXInfo( void )
{
	return msfgqxdh::Info;
}

void sclx::SCLXInitialization( xdhcdc::eMode Mode )
{
}

namespace {
  typedef xdhcdc::cSingle cXDHCallback_;

	class rXDHCallback
	: public xdhcdc::cSingle
	{
	protected:
		virtual bso::sBool XDHCDCInitialize(
			xdhcuc::cSingle &Callback,
			const char *Language,
			const str::dString &Token) override
			{
			  return true;
			}
		virtual bso::bool__ XDHCDCHandle(
      const str::dString &Id,
      const str::dString &Action) override
			{
			  qRVct();
			}
  public:
    void reset(bso::sBool = true)
    {}
    qCVDTOR(rXDHCallback);
    void Init(void)
    {
      reset();
    }
	};
}


xdhcdc::cSingle *sclx::SCLXFetchCallback(void)
{
	rXDHCallback *Callback = new rXDHCallback;

	if ( Callback == NULL )
		qRGnr();

	Callback->Init();

	return Callback;
}

void sclx::SCLXDismissCallback( xdhcdc::cSingle *Callback )
{
	if ( Callback == NULL )
		qRGnr();

	delete Callback;
}

