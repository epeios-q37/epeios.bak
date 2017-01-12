/*
	Copyright (C) 2016-2017 by Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of 'MMUAq'.

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

#include "imap.h"

#include "misc.h"
#include "registry.h"

#include "muaima.h"

#include "sclmisc.h"

#include "csdbnc.h"

using namespace imap;

namespace {
	using namespace muaima;
	using namespace base;

	void GetUsernameAndPassword_(
		str::dString &Username,
		str::dString &Password )
	{
		sclmisc::MGetValue( registry::parameter::imap::Username, Username );
		sclmisc::MGetValue( registry::parameter::imap::Password, Password );
	}

	void DumpPending_(
		muaima::eCode Awaited,
		bso::sBool Verbose,
		muaima::rSession &Session )
	{
		muaima::eCode Code = muaima::c_Undefined;

		while ( ( Code = Session.GetCode()) != muaima::c_None ) {
			if ( Verbose || ( Code == Awaited ) ) {
				fdr::rIDriver &Driver = Session.GetResponseDriver();

				cio::COut << muaima::GetLabel( Code );

				if ( !Driver.EndOfFlow() )
					cio::COut << ": ";

				misc::Dump( Driver );
				cio::COut << txf::nl << txf::commit;
			} else
				Session.SkipResponse();
		}
	}

	void Dump_(
		eStatus Status,
		muaima::eCode Awaited,
		bso::sBool Verbose,
		muaima::rSession &Session )
	{
		DumpPending_( Awaited, Verbose, Session );
		Status = muaima::base::GetCompletionStatus( Session );

		if ( Verbose ) {
			cio::COut << muaima::base::GetLabel(Status) << ": ";
			misc::Dump( Session.GetResponseDriver() );
			cio::COut << txf::nl << txf::commit;
		} else
			Session.SkipResponse();

		if ( ( Status == muaima::cNo ) || ( Status == muaima::cBad ) )
			qRAbort();
	}

	void Login_(
		bso::sBool Verbose,
		muaima::rSession &Session )
	{
	qRH
		str::wString Username, Password;
	qRB
		tol::Init( Username, Password );

		GetUsernameAndPassword_( Username, Password );

		Dump_( Login( Username, Password, Session  ), muaima::c_Undefined, Verbose, Session );
	qRR
	qRT
	qRE
	}
}

namespace{
	class rVerboseRack_
	: public misc::rVerboseIODriver,
	  public muaima::rSession
	{
		bso::sBool Connected_ = true;
	public:
		void reset( bso::sBool P = true )
		{
			if ( P ) {
				if ( IsConnected() )
					muaima::base::Logout( *this );
			}

			rSession::reset( P );
			misc::rVerboseIODriver::reset( P );
			Connected_ = false;
		}
		qCVDTOR( rVerboseRack_ );
		void Init( bso::sBool Activate )
		{
			Connected_ = false;
			misc::rVerboseIODriver::Init( registry::parameter::imap::HostPort, Activate );
			Connected_ = true;
			muaima::rSession::Init( *this );

		}
	};
}

void imap::Capability( void )
{
qRH
	bso::sBool Verbose = false;
	rVerboseRack_ Rack;
qRB
	Verbose = misc::IsVerboseActivated();

	Rack.Init( Verbose );

	Dump_( muaima::base::Connect( Rack ), muaima::c_Undefined, Verbose, Rack );

	Login_( Verbose, Rack );

	Dump_( muaima::base::Capability( Rack ), muaima::cCapability, Verbose, Rack );
qRR
qRT
qRE
}

void imap::Select( void )
{
qRH
	bso::sBool Verbose = false;
	rVerboseRack_ Rack;
qRB
	Verbose = misc::IsVerboseActivated();

	Rack.Init( Verbose );

	Dump_( muaima::base::Connect( Rack ), muaima::c_Undefined, Verbose, Rack );

	Login_( Verbose, Rack );

	Dump_( muaima::base::Select( str::wString( "inbox" ), Rack ), muaima::cCapability, Verbose, Rack );
qRR
qRT
qRE
}
