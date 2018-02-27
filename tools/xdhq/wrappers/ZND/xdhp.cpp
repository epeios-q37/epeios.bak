/*
	Copyright (C) 2017 by Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of XDHq.

    XDHq is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    XDHq is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with XDHq.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "xdhp.h"

#include "registry.h"
#include "treep.h"

#include "proxy.h"

#include "sclargmnt.h"

#include "csdmns.h"
#include "csdcmn.h"
#include "mtk.h"

using namespace xdhp;

namespace {
	namespace {
		typedef proxy::rData rPData_;
	}

	class rData_
	: public rPData_
	{
	public:
		bso::sBool FirstCall;
		void reset( bso::sBool P = true )
		{
			rPData_::reset( P );
			FirstCall = false;
		}
		qCDTOR( rData_ );
		void Init( void )
		{
			rPData_::Init();
			FirstCall = true;
		}
	};

	namespace {
		typedef proxy::rProcessing<rData_> rPProcessing_;
	}

	proxy::rSharing<rData_> Sharing_;

	class rProcessing_
	: public rPProcessing_
	{
	protected:
		rData_ *PRXYNew( void ) override
		{
			rData_ *Data = new rData_;

			Data->Init();

			Sharing_.Write( Data );

			return Data;
		}
		void PRXYOnAction( rData_ *Data ) override
		{}
		void PRXYOnPending( rData_ *Data ) override
		{}
	} Processing_;

	csdmns::rServer Server_;

	void Process_( void * )
	{
	qRFH;
	qRFB;
		Server_.Process();
	qRFR;
	qRFT;
	qRFE( sclmisc::ErrFinal() );
	}
}

SCLZND_F( xdhp::Listen )
{
qRH;
	str::wString Arguments, NewSessionAction;
qRB;
	tol::Init( Arguments, NewSessionAction );
	Caller.Get( Arguments, NewSessionAction );

	sclargmnt::FillRegistry( Arguments, sclargmnt::faIsArgument, sclargmnt::uaReport );

	Processing_.Init( NewSessionAction );
	Server_.Init( sclmisc::MGetU16( registry::parameter::Service ), Processing_ );

	mtk::RawLaunch( Process_, NULL );
qRR;
qRT;
qRE;
}

SCLZND_F( xdhp::New )
{
	rData_ *Data = Sharing_.Read();
	Caller.SetReturnValue( (bso::sS64)Data );
	Data->Recv.ReadDismiss();
}

namespace {
	rData_ &GetData_( sclznd::sCaller &Caller )
	{
		bso::sS64 Long = 0;

		Caller.Get( Long );

		if ( Long == 0 )
			qRGnr();

		return *(rData_ *)Long;
	}
}

SCLZND_F( xdhp::GetAction )
{
qRH;
	str::wStrings Strings;
qRB;
	rData_ &Data = GetData_( Caller );

	if ( !Data.FirstCall ) {
		Data.Sent.WriteDismiss();
	} else
		Data.FirstCall = false;
	
	Data.Recv.ReadBegin();
	Strings.Init();
	Strings.Append( Data.Recv.Id );
	Strings.Append( Data.Recv.Action );

	Data.Recv.ReadEnd();

	Caller.SetReturnValue( Strings );
qRR;
qRT;
qRE;
}

#define BEGIN( request )\
	rData_ &Data = GetData_( Caller );\
	Data.Sent.WriteBegin();\
	Data.Request = prxy_cmn::r##request;\
	proxy::rArguments &Arguments = Data.Sent.Arguments;\
	Arguments.Init();

#define SWITCH	Data.Sent.WriteEnd();Data.Recv.ReadBegin(); proxy::rReturn &Return = Data.Recv.Return

#define END	Data.Recv.ReadEnd()

SCLZND_F( xdhp::Execute )
{
	BEGIN( Execute );

	Caller.Get( Arguments.Script );

	SWITCH;

	Caller.SetReturnValue( Return.GetString() );

	END;
}



SCLZND_F( xdhp::Alert )
{
	BEGIN( Alert );

	Caller.Get( Arguments.Message );

	SWITCH;
	END;
}

SCLZND_F( xdhp::Confirm )
{
	BEGIN( Confirm );

	Caller.Get( Arguments.Message );
	
	SWITCH;

	Caller.SetReturnValue( Return.GetString() == "true" );

	END;
}

SCLZND_F( xdhp::SetLayout )
{
	BEGIN( SetLayout );

	Caller.Get( Arguments.Id );
	treep::GetXML( Caller, Arguments.XML );
	Caller.Get( Arguments.XSLFilename );

	SWITCH;
	END;
}

SCLZND_F( xdhp::GetContents )
{
	BEGIN( GetContents );

	Caller.Get( Arguments.Ids );

	SWITCH;

	Caller.SetReturnValue( Return.GetStrings() );

	END;
}

SCLZND_F( xdhp::SetContents )
{
	BEGIN( SetContents );

	Caller.Get( Arguments.Ids, Arguments.Contents );

	SWITCH;

	END;
}

SCLZND_F( xdhp::DressWidgets )
{
	BEGIN( DressWidgets );

	Caller.Get( Arguments.Id );

	SWITCH;
	END;
}

SCLZND_F( xdhp::AddClasses )
{
	BEGIN( AddClasses );

	Caller.Get( Arguments.Ids, Arguments.Classes );

	SWITCH;
	END;
}

SCLZND_F( xdhp::RemoveClasses )
{
	BEGIN( RemoveClasses );

	Caller.Get( Arguments.Ids, Arguments.Classes );

	SWITCH;
	END;
}

SCLZND_F( xdhp::ToggleClasses )
{
	BEGIN( ToggleClasses );

	Caller.Get( Arguments.Ids, Arguments.Classes );

	SWITCH;
	END;
}

SCLZND_F( xdhp::EnableElements )
{
	BEGIN( EnableElements );

	Caller.Get( Arguments.Ids );

	SWITCH;
	END;
}

SCLZND_F( xdhp::DisableElements )
{
	BEGIN( DisableElements );

	Caller.Get( Arguments.Ids );

	SWITCH;
	END;
}

SCLZND_F( xdhp::SetAttribute )
{
	BEGIN( SetAttribute );

	Caller.Get( Arguments.Id, Arguments.Name, Arguments.Value );

	SWITCH;
	END;
}

SCLZND_F( xdhp::GetAttribute )
{
	BEGIN( GetAttribute );

	Caller.Get( Arguments.Id, Arguments.Name );

	SWITCH;

	Caller.SetReturnValue( Return.GetString() );

	END;
}

SCLZND_F( xdhp::RemoveAttribute )
{
	BEGIN( RemoveAttribute );

	Caller.Get( Arguments.Id, Arguments.Name );

	SWITCH;
	END;
}

SCLZND_F( xdhp::SetProperty )
{
	BEGIN( SetAttribute );

	Caller.Get( Arguments.Id, Arguments.Name, Arguments.Value );

	SWITCH;
	END;
}

SCLZND_F( xdhp::GetProperty )
{
	BEGIN( GetProperty );

	Caller.Get( Arguments.Id, Arguments.Name );

	SWITCH;

	Caller.SetReturnValue( Return.GetString() );

	END;
}

SCLZND_F( xdhp::Focus )
{
	BEGIN( Focus );

	Caller.Get( Arguments.Id );

	SWITCH;
	END;
}

qGCTOR( xdhp )
{
	tol::Init( Sharing_ );
}



