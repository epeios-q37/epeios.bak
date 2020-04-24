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

#include "session.h"

using namespace session;

csdmnc::rCore session::Core;
logq::rFDriver<> session::LogDriver;

void session::rSession::Log_( const str::dString &Message )
{
qRH;
    logq::rLogRack<> Log;
qRB;
    Log.Init( LogDriver );

    Log << *Logging_.Id;

    if ( Logging_.IP.Amount() != 0 )
        Log << " (" << Logging_.IP << ")";

    Log << " : " <<  Message;
qRR;
qRT;
qRE;
}

void session::rSession::ReportToFrontend_(const str::dString &HTML)
{
qRH
    str::wStrings Values;
qRB
    Values.Init("Set");
    Values.Append("");
    Values.Append(HTML);
    Values.Append("");

    Process("HandleLayout_1", Values);
qRR
qRT
qRE
}

void session::rSession::ReportErrorToFrontend_(const str::dString &Message)
{
qRH
    str::wStrings Values;
qRB
    Values.Init(Message);

    Process("Alert_1", Values);
qRR
qRT
qRE
}

namespace {
	qENUM( Type_ ) {
        tVoid,
        tString,
        tStrings,
        t_amount,
        t_Undefined
	};

	eType_ GetType_( flw::rRFlow &Flow ) {
        bso::sU8 Type;
        prtcl::Get(Flow, Type );

        if ( Type >= t_amount )
            qRGnr();

        return (eType_)Type;
	}

	void GetParameters_(
        flw::rRFlow &Flow,
        str::wStrings &Parameters )
    {
    qRH
        str::wString Value;
        str::wStrings Values;
        bso::sBool Continue = true;
    qRB
        while ( Continue ) {
            switch( GetType_( Flow ) ) {
            case tVoid:
                Continue = false;
                break;
            case tString:
                Value.Init();
                prtcl::Get(Flow, Value );

                Parameters.Append( Value );
                break;
            case tStrings:
                Values.Init();
                prtcl::Get(Flow, Values);

                Value.Init();
                xdhcmn::FlatMerge(Values, Value, true );

                Parameters.Append(Value);

                break;
            default:
                qRGnr();
                break;
            }
        }
    qRR
    qRE
    qRT
    }
}

bso::bool__ session::rSession::Launch_(
    const char *Id,
    const char *Action )
{
    bso::sBool Return = false;
qRH;
    bso::sBool Continue = true;
    flw::rDressedRWFlow<> Flow;
    str::wString ScriptName, ReturnValue;
    str::wStrings Parameters, SplitedReturnValue;
    eType_ ReturnType = t_Undefined;
qRB;
    Flow.Init( D_() );

    prtcl::Put( Id, Flow );
    prtcl::Put( Action, Flow );
    Flow.Commit();


# define H( name )\
case c##name##_1:\
::name##_( Flow, *this );\
break

    while ( Continue ) {
        ScriptName.Init();
        prtcl::Get(Flow,ScriptName);

        Log_( ScriptName );

        if ( ScriptName == "StandBy_1") {
            Return = true;
            Continue = false;
            Flow.Dismiss();
        } else {
            ReturnType = GetType_( Flow );

            Parameters.Init();
            GetParameters_(Flow, Parameters);

            Flow.Dismiss();

            if ( ReturnType == tVoid ) {
                Process(ScriptName, Parameters);
            } else {
                ReturnValue.Init();
                Process(ScriptName, Parameters, ReturnValue);
            }

            switch ( ReturnType ) {
            case tVoid:
                break;
            case tString:
                prtcl::Put(ReturnValue, Flow);
                Flow.Commit();
                break;
            case tStrings:
                SplitedReturnValue.Init();
                xdhcmn::FlatSplit(ReturnValue,SplitedReturnValue);
                prtcl::Put(SplitedReturnValue, Flow);
                Flow.Commit();
                break;
            default:
                qRGnr();
                break;
            }
        }
    }
#undef H
qRR;
    ReportErrorToFrontend_( str::wString("Connection to backend lost!"));
qRT;
qRE;
    return Return;
}

namespace {
	idsq::wIdStore <sId_> Ids_;
}

bso::sBool session::rSession::XDHCMNInitialize(
    xdhcmn::cUpstream &Callback,
    const char *Language,
    const str::dString &Token )
{
    bso::sBool Success = false;
qRFH;
    flw::rDressedRWFlow<> Flow;
    csdcmn::sVersion Version = csdcmn::UndefinedVersion;
    str::wString LogMessage;
qRFB;
    LogMessage.Init();

    if ( Token.Amount() == 0 ) {
        ProdDriver_.Init( Core, fdr::ts_Default );
        _Mode_ = mProd;
        Success = true;
        LogMessage.Append( "PROD" );
    } else {
        LogMessage.Append( Token );

        FaaSDriver_.Init();

        if ( faaspool::GetConnection( Token, Logging_.IP, FaaSDriver_.GetShared() ) ) {
            _Mode_ = mFaaS;
            Success = true;
        }
    }

    LogMessage.Append( " - " );

    if ( Success ) {
        Flow.Init( D_() );

        Version = csdcmn::GetProtocolVersion( prtcl::ProtocolId, Flow );
        Flow.Dismiss();

        switch ( Version ) {
        case 0:
            ReportNoErrorToBackend_( Flow );
            break;
        case 1:
            ReportNoErrorToBackend_( Flow );
            break;
        case csdcmn::UndefinedVersion:
            ReportErrorToBackend_( "\nIncompatible protocol! Please update your software.\n", Flow );
            break;
        default:
            ReportErrorToBackend_( "\nUnknown protocol version!\n", Flow );
            break;
        }

        prtcl::Put( "", Flow );
        Flow.Commit();

        csdcmn::Get( Flow, LogMessage );

        Logging_.Id = Ids_.New();
        Log_( LogMessage );

        xdhdws::sProxy::Init( Callback );	// Has to be last, otherwise, if an error occurs, 'Callback' will be freed twice!
    }
qRR;
qRT;
qRE;
    return Success;
}

bso::bool__ session::rSession::XDHCMNLaunch(
    const char *Id,
    const char *Action )
{
    bso::sBool Return = false;
qRFH;
qRFB;
    Return = Launch_( Id, Action );
qRFR;
qRFT;
qRFE(sclm::ErrFinal());
    return Return;
}

