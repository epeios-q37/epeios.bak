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

#include "mtk.h"

using namespace session;

csdmnc::rCore session::Core;
logq::rFDriver<> session::LogDriver;

namespace {
	qMIMICs( bso::sU32, sId_ );	// Assigned per session.
	qCDEF(sId_, UndefinedId_, bso::U32Max);
}

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

		Proxy.Process("HandleLayout_1", Values);
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

		Proxy.Process("Alert_1", Values);
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

bso::bool__ session::rSession::Launch_(
	const char *Id,
	const char *Action )
{
	bso::sBool Return = false;
qRH;
	flw::rDressedWFlow<> Flow;
qRB;
	Flow.Init( D_() );

	prtcl::Put( Id, Flow );
	prtcl::Put( Action, Flow );
	Flow.Commit();

	Blocker_.Wait();
qRR;
	ReportErrorToFrontend_(*this, str::wString("Connection to backend lost!"));
qRT;
qRE;
	return Return;
}

namespace IdStore_ {
	namespace {
		idsq::wIdStore <sId_> Ids_;
		mtx::rHandler Mutex_ = mtx::Undefined;
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
	qRB
		mtx::Lock(Mutex_);
		Id = Ids_.New();
	qRR
	qRT
		mtx::Unlock(Mutex_);
	qRE
		return Id;
	}

	void Release(sId_ Id)
	{
	qRH
	qRB
		mtx::Lock(Mutex_);
		Ids_.Release(Id);
	qRR
	qRT
		mtx::Unlock(Mutex_);
	qRE
	}
}

namespace {
	struct sData {
		fdr::rRWDriver *Driver;
		xdhdws::sProxy *Proxy;
		sId_ Id;
		const str::dString
		*IP,
		*Token;
		tht::rBlocker *Blocker;
	};

	namespace {
		namespace {
			void Log_(
				sId_ Id,
				const str::dString &IP,
				const str::dString &Message )
			{
			qRH;
				logq::rLogRack<> Log;
			qRB;
				Log.Init( LogDriver );

				Log << *Id;

				if ( IP.Amount() != 0 )
					Log << " (" << IP << ")";

				Log << " : " <<  Message;
			qRR;
			qRT;
			qRE;
			}
		}

		namespace {
			void Broadcast_(
				flw::rRFlow &Flow,
				xdhdws::sProxy &Proxy,
				const str::dString &Token)
			{
			qRH
				str::wString Script;
			qRB
				Script.Init();

				prtcl::Get(Flow, Script);

				Flow.Dismiss();

				Proxy.Broadcast(Script, Token);
			qRR
			qRT
			qRE
			}

			void BroadcastAction_(
				flw::rRFlow &Flow,
				xdhdws::sProxy &Proxy,
				const str::dString &Token)
			{
			qRH
				str::wString Action, Id;
			qRB
				tol::Init(Action, Id);

				if ( GetType_(Flow )!= tVoid )
					qRGnr();


				if ( GetType_(Flow) != tString )
					qRGnr();

				prtcl::Get(Flow, Action);


				if ( GetType_(Flow) != tString )
					qRGnr();

				prtcl::Get(Flow, Id);


				if ( GetType_(Flow) != tVoid )
					qRGnr();


				Flow.Dismiss();

				Proxy.BroadcastAction(Action, Id, Token);
			qRR
			qRT
			qRE
			}

		}

		void Routine_(
			void *UP,
			mtk::gBlocker &DataBlocker)
		{
			sData &Data = *(sData *)UP;
			xdhdws::sProxy &Proxy = *Data.Proxy;
		qRH;
			sId_ Id = UndefinedId_;
			flw::rDressedRWFlow<> Flow;
			str::wString IP, Token, ScriptName, ReturnValue;
			str::wStrings Parameters, SplitedReturnValue;
			eType_ ReturnType = t_Undefined;
		qRB;
			fdr::rRWDriver &Driver = *Data.Driver;
			Id = Data.Id;
			IP.Init(*Data.IP);
			Token.Init(*Data.Token);
			tht::rBlocker &Blocker = *Data.Blocker;

			DataBlocker.Release();

			Flow.Init( Driver );

			while ( true ) {
				ScriptName.Init();
				prtcl::Get(Flow,ScriptName);

				Log_( Id, IP, ScriptName );

				if ( ScriptName == "StandBy_1") {
					Flow.Dismiss();
					Blocker.Unblock();
				} else if ( ScriptName == "Broadcast_1" ) { // Issued by XDH front-ends (Epeios XDH-based front-ends).
					Broadcast_(Flow, Proxy, Token);
				} else if ( ScriptName == "BroadcastAction_1" ) { // Issued by Atlas toolkit front-ends (Python, Java, Node.js…).
					BroadcastAction_(Flow, Proxy, Token);
				} else if ( ScriptName == "Quit_1" ) { // Issued by Atlas toolkit front-ends (Python, Java, Node.js…).
					IdStore_::Release(Id);
					Flow.Dismiss();
					Blocker.Unblock();
					break;
				} else {
					ReturnType = GetType_( Flow );

					Parameters.Init();
					GetParameters_(Flow, Parameters);

					Flow.Dismiss();

					if ( ReturnType == tVoid ) {
						Proxy.Process(ScriptName, Parameters);
					} else {
						ReturnValue.Init();
						Proxy.Process(ScriptName, Parameters, ReturnValue);
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
			ReportErrorToFrontend_(Proxy, str::wString("Connection to backend lost!"));
		qRT;
		qRE;
		}
	}
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
	str::wString LogMessage, IP;
	sId_ Id = UndefinedId_;
	sData Data;
qRB;
	tol::Init(LogMessage, IP);

	if ( Token.Amount() == 0 ) {
		ProdDriver_.Init( Core, fdr::ts_Default );
		Mode_ = mProd;
		Success = true;
		LogMessage.Append( "PROD" );
	} else {
		LogMessage.Append( Token );

		FaaSDriver_.Init();

		if ( faaspool::GetConnection( Token, IP, FaaSDriver_.GetShared() ) ) {
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

		Id = IdStore_::Fetch();
		Log_(Id, IP, LogMessage);

		Data.Driver = &D_();
		Data.Proxy = this;
		Data.IP = &IP;
		Data.Token = &Token;
		Data.Id = Id;
		Data.Blocker = &Blocker_;

		mtk::Launch(Routine_, &Data);

		xdhdws::sProxy::Init(Callback, Token);	// Has to be last, otherwise, if an error occurs, 'Callback' will be freed twice!
	}
qRR;
qRT;
qRE;
	return Success;
}

bso::bool__ session::rSession::XDHCDCLaunch(
	const char *Id,
	const char *Action )
{
	bso::sBool Return = false;
qRFH;
qRFB;
	Return = Launch_( Id, Action );
qRFR;
qRFT;
qRFE(sclm::ErrorDefaultHandling());
	return Return;
}

qGCTOR(session) {
	IdStore_::Init();
}

