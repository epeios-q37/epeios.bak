/*
	Copyright (C) 2017 Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of 'XDHq' software.

	'XDHq' is free software: you can redistribute it and/or modify it
	under the terms of the GNU Affero General Public License as published
	by the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	'XDHq' is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with 'XDHq'.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "common.h"

#include "prtcl.h"

#include "xdhdws.h"

using namespace common;

logq::rFDriver<> common::LogDriver;

namespace {
	xdhcuc::cGlobal *Callback_ = NULL;

	xdhcuc::cGlobal &C_(void)
	{
		if ( Callback_ == NULL )
			qRGnr();

		return *Callback_;
	}
}

void common::SetCallback(xdhcuc::cGlobal &Callback)
{
	::Callback_ = &Callback;
}

xdhcuc::cGlobal &common::GetCallback(void)
{
	return C_();
}

namespace {
  void ReportErrorToBackend_(
    const char *Message,
    flw::rWFlow &Flow )
  {
    if ( Message == NULL )
      Message = "";

    prtcl::Put(Message, Flow);	// If 'Message' not empty, client will display content and abort.

    if ( Message[0] ) {
      Flow.Commit();
      qRGnr();
    }
  }

  void ReportNoErrorToBackend_( flw::rWFlow &Flow )
  {
    ReportErrorToBackend_(NULL, Flow);
  }

  void HandleScriptsVersion_(flw::rRWFlow &Flow)
  {
    switch( csdcmn::GetVersion(xdhdws::GetScriptsVersion(), Flow) ) {
    case csdcmn::UnknownVersion:
      ReportErrorToBackend_( "\nUnknown scripts version!\n", Flow );
      break;
    case csdcmn::BadProtocol:
      ReportErrorToBackend_( "\nBad protocol!\n", Flow );
      break;
    default:
      // Nothing to do. Handled Upstream.
      break;
    }
  }
}

void common::Handshake(flw::rRWFlow &Flow)
{
  csdcmn::sVersion Version = csdcmn::GetProtocolVersion( prtcl::ProtocolId, prtcl::ProtocolLastVersion, Flow );

  Flow.Dismiss();

  switch ( Version ) {
  case csdcmn::UnknownVersion:
    ReportErrorToBackend_( "\nUnknown protocol version!\n", Flow );
    break;
  case csdcmn::BadProtocol:
    ReportErrorToBackend_( "\nUnknown protocol!\n", Flow );
    break;
  default:
    if ( Version > prtcl::ProtocolLastVersion ) // Case handled by 'csdcmn::GetProtocolVersion(…)'.
      qRUnx();

    HandleScriptsVersion_(Flow);

    ReportNoErrorToBackend_( Flow );

    prtcl::Put("", Flow); // No notification yet ; for future use.
    break;
  }
}

