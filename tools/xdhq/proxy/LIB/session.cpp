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

#include "common.h"

#include "mtk.h"

using namespace session;

namespace {
	void ReportToFrontend_(
		xdhdws::sProxy &Proxy,
		const str::dString &HTML)
	{
	qRH
		str::wStrings Values;
	qRB
		Values.Init("Set");
		Values.Append("");
		Values.Append(HTML);
		Values.Append("");

		if ( !Proxy.Process("HandleLayout_1", Values) )
			qRGnr();
	qRR
	qRT
	qRE
	}

	void ReportErrorToFrontend_(
		xdhdws::sProxy &Proxy,
		const str::dString &Message)
	{
	qRH
		str::wStrings Values;
	qRB
		Values.Init(Message);

		if ( !Proxy.Process("Alert_1", Values) )
			qRGnr();
	qRR
	qRT
	qRE
	}
}

namespace {
	qENUM( Type_ )
	{
		tVoid,
		tString,
		tStrings,
		t_amount,
		t_Undefined
	};

	eType_ GetType_( flw::rRFlow &Flow )
	{
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

// #define LOG cio::COut << __LOC__ << tol::DateAndTime(DT) << txf::nl << txf::commit;

namespace {
	namespace id_store_ {
		namespace {
			idsq::wIdStore <sId_> Ids_;
			mtx::rMutex Mutex_ = mtx::Undefined;
		}

		void Init(void)
		{
			if ( Mutex_ != mtx::Undefined )
				mtx::Delete(Mutex_);

			Mutex_ = mtx::Create();
			Ids_.Init();
		}

		sId_ Fetch(void)
		{
			sId_ Id = UndefinedId_;
		qRH
			mtx::rHandle Mutex;
		qRB
			Mutex.InitAndLock(Mutex_);
			Id = Ids_.New();
		qRR
		qRT
		qRE
			return Id;
		}

		void Release(sId_ Id)
		{
		qRH
			mtx::rHandle Mutex;
		qRB
			if ( Id == UndefinedId_ )
				qRGnr();

			Mutex.InitAndLock(Mutex_);
			Ids_.Release(Id);
		qRR
		qRT
		qRE
		}
	}
}

namespace {
	template <typename string> void Log_(
		sId_ Id,
		const str::dString &IP,
		const string &Message )
	{
	qRH;
		logq::rLogRack<> Log;
	qRB;
		Log.Init( common::LogDriver );

		Log << *Id;

		if ( IP.Amount() != 0 )
			Log << " (" << IP << ")";

		Log << " : " <<  Message;
	qRR;
	qRT;
	qRE;
	}
}

void session::Release_(
	sId_ Id,
	const str::dString &IP )
{
	Log_(Id, IP, "RELEASED");
	id_store_::Release(Id);
}

bso::sBool session::rSession::XDHCDCInitialize(
	xdhcuc::cSingle &Callback,
	const char *Language,
	const str::dString &Token )
{
	bso::sBool Success = false;
qRH;
	flw::rDressedRWFlow<> Flow;
	csdcmn::sVersion Version = csdcmn::UndefinedVersion;
	str::wString LogMessage;
qRB;
	tol::Init(LogMessage);

	if ( Token.Amount() == 0 ) {
		SlfHDriver_.Init( common::Core, fdr::ts_Default );
		Mode_ = mSlfH;
		Success = true;
		LogMessage.Append( "SlfH" );
	} else {
		LogMessage.Append( Token );

		if ( ( TRow_ = FaaSDriver_.Init(Token, IP_) ) != qNIL ) {
			Mode_ = mFaaS;
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

		Id_ = id_store_::Fetch();
		Token_ = Token;
		Log_(Id_, IP_, LogMessage);

		xdhdws::sProxy::Init(Callback, Token);	// Has to be last, otherwise, if an error occurs, 'Callback' will be freed twice!
	}
qRR;
qRT;
qRE;
	return Success;
}

bso::bool__ session::rSession::Launch_(
	const char *Id,
	const char *Action )
{
	bso::sBool Cont = true;
qRH;
	flw::rDressedRWFlow<> Flow;
	str::wString ScriptName, ReturnValue;
	eType_ ReturnType = t_Undefined;
	str::wStrings Parameters, SplitedReturnValue;
qRB;
	Flow.Init(D_());

	prtcl::Put(Id, Flow);
	prtcl::Put(Action, Flow);
	Flow.Commit();

	while ( true ) {
		ScriptName.Init();
		prtcl::Get(Flow, ScriptName);

		Log_( Id_, IP_, ScriptName );

		if ( ScriptName == faas_::StandByScriptName ) {
			Flow.Dismiss();
			break;
		} else {
			ReturnType = GetType_( Flow );

			Parameters.Init();
			GetParameters_(Flow, Parameters);

			Flow.Dismiss();

			if ( ReturnType == tVoid ) {
				if ( !Process(ScriptName, Parameters) ) {
					Cont = false;
					break;
				}
			} else {
				ReturnValue.Init();
				if ( !Process(ScriptName, Parameters, ReturnValue) ) {
					Cont = false;
					break;
				}
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
qRR;
	// ReportErrorToFrontend_(*this, str::wString("Connection to backend lost!!!"));
qRT;
qRE;
	return Cont;
}

bso::bool__ session::rSession::XDHCDCLaunch(
	const char *Id,
	const char *Action )
{
	bso::sBool Cont = false;
qRFH;
qRFB;
	Cont = Launch_( Id, Action );
qRFR;
qRFT;
qRFE(sclm::ErrorDefaultHandling());
	return Cont;
}

qGCTOR(session) {
	id_store_::Init();
}

