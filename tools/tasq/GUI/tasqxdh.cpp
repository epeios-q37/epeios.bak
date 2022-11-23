/*
  Copyright (C) 2022 Claude SIMON (http://zeusw.org/epeios/contact.html).

  This file is part of 'TASq' software.

  'TASq' is free software: you can redistribute it and/or modify it
  under the terms of the GNU Affero General Public License as published
  by the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  'TASq' is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Affero General Public License for more details.

  You should have received a copy of the GNU Affero General Public License
  along with 'TASq'.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "tasqxdh.h"

#include "main.h"
#include "registry.h"

#include "tasqtasks.h"

SCLI_DEF( tasqxdh, NAME_LC	SCLX_DEFAULT_SUFFIX, NAME_MC );

const scli::sInfo &sclx::SCLXInfo( void )
{
	return tasqxdh::Info;
}

void sclx::SCLXInitialization( xdhcdc::eMode Mode )
{
qRH;
  str::wString DBFileAffix;
qRB;
  DBFileAffix.Init();
  sclm::MGetValue(::registry::parameter::DBFileAffix, DBFileAffix);

  tasqtasks::Initialize(DBFileAffix);
qRR;
qRT;
qRE;
}

namespace {
  typedef xdhcdc::cSingle cXDHCallback_;
}

namespace {
	class rXDHCallback
	: public xdhcdc::cSingle
	{
  private:
    main::sSession Session_;
	protected:
		virtual bso::sBool XDHCDCInitialize(
			xdhcuc::cSingle &Callback,
			const char *Language,
			const str::dString &Token) override
			{
			  Session_.Init(Callback, tasqxdh::Info);
			  return true;
			}
		virtual bso::bool__ XDHCDCHandle(
      const str::dString &Id,
      const str::dString &Action) override
      {
      qRH;
			  qCBUFFERh IdBuffer, ActionBuffer;
      qRB;
        Action.Convert(ActionBuffer);

			  main::Core.Launch(Session_, Id.Convert(IdBuffer), ActionBuffer == NULL || ActionBuffer[0] == 0 ? "OnNewSession" : ActionBuffer, xdhcdc::m_Undefined); // Last parameter is not used.
      qRR;
      qRT;
      qRE;
        return true;
      }
  public:
    void reset(bso::sBool P = true)
    {
      tol::reset(P, Session_);
    }
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

namespace {
  bso::sBool GetHead_(str::dString &Head)
  {
    bso::sBool Success = true;
  qRH;
  qRB;
    sclm::MGetValue(registry::definition::Head, Head);
  qRR;
  qRT;
  qRE;
    return Success;
  }
}

qGCTOR(msfgxdh) {
  sclx::SCLXGetHead = &GetHead_;
}
