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

#include "xdhqxdh.h"

#include "faaspool.h"
#include "registry.h"

#include "prtcl.h"

#include "logq.h"
#include "idsq.h"

#include "csdmnc.h"
#include "csdcmn.h"
#include "csdbns.h"

#include "sclmisc.h"
#include "sclxdhtml.h"

SCLI_DEF( xdhqxdh, PROGRAM_NAME, SOFTWARE_NAME );

namespace {
	csdmnc::rCore Core_;
	logq::rFDriver<> LogDriver_;
	qMIMICs( bso::sU32, sId_ );
	sId_ UndefinedId_ = bso::U32Max;
	idsq::wIdStore <sId_> Ids_;
}

namespace {

	stsfsm::wAutomat CommandAutomat_;

	// Deprecated.
	qENUM( _Command_ )
	{
		cStandBy_1,	// Send as command to report that there is no more command to handle.
		cExecute_1,
		cAlert_1,
		cConfirm_1,
		cSetContents_1,
		cSetTimeout_1,
		cPrependLayout_1,
		cSetLayout_1,
		cAppendLayout_1,
		cGetContents_1,
		cParent_1,
		cFirstChild_1,
		cLastChild_1,
		cPreviousSibling_1,
		cNextSibling_1,
		cCreateElement_1,
		cInsertChild_1,
		cAppendChild_1,
		cInsertBefore_1,
		cInsertAfter_1,
		cInsertCSSRule_1,
		cAppendCSSRule_1,
		cRemoveCSSRule_1,
		cDressWidgets_1,
		cAddClasses_1,
		cRemoveClasses_1,
		cToggleClasses_1,
		cEnableElements_1,
		cDisableElements_1,
		cSetAttribute_1,
		cGetAttribute_1,
		cRemoveAttribute_1,
		cSetProperty_1,
		cGetProperty_1,
		cFocus_1,
		c_amount,
		c_Undefined
	};

	#define C( name ) case c##name : return #name ; break

	const char *GetLabel_( e_Command_ Command )
	{
		switch ( Command ) {
			case cStandBy_1:
				return prtcl::StandBy;
				break;
			C( Execute_1 );
			C( Alert_1 );
			C( Confirm_1 );
			C( GetContents_1 );
			C( SetContents_1 );
			C( SetTimeout_1 );
			C( PrependLayout_1);
			C( SetLayout_1);
			C( AppendLayout_1);
			C( Parent_1 );
			C( FirstChild_1 );
			C( LastChild_1 );
			C( PreviousSibling_1 );
			C( NextSibling_1 );
			C( CreateElement_1 );
			C( InsertChild_1 );
			C( AppendChild_1 );
			C( InsertBefore_1 );
			C( InsertAfter_1 );
			C( InsertCSSRule_1);
			C( AppendCSSRule_1);
			C( RemoveCSSRule_1);
			C( DressWidgets_1 );
			C( AddClasses_1 );
			C( RemoveClasses_1 );
			C( ToggleClasses_1 );
			C( EnableElements_1 );
			C( DisableElements_1 );
			C( SetAttribute_1 );
			C( GetAttribute_1 );
			C( RemoveAttribute_1 );
			C( SetProperty_1 );
			C( GetProperty_1 );
			C( Focus_1 );
		default:
			qRFwk();
			break;
		}

		return NULL; // To avoid a 'warning'.
	}

#undef C

	void FillCommandAutomat_( void )
	{
		CommandAutomat_.Init();
		stsfsm::Fill<e_Command_>( CommandAutomat_, c_amount, GetLabel_ );
	}

	e_Command_ GetCommand_( flw::iflow__ &Flow )
	{
		return stsfsm::GetId( Flow, CommandAutomat_, c_Undefined, c_amount );
	}
}

void sclxdhtml::SCLXDHTMLInitialization( xdhcmn::eMode Mode )
{
qRH;
	qCBUFFERr Buffer;
	str::wString HostService;
qRB;
	HostService.Init();

	if ( sclmisc::OGetValue( ::registry::parameter::HostService, HostService ) )
		Core_.Init( HostService.Convert( Buffer ), 0, sck::NoTimeout );

	faaspool::Initialize();
	LogDriver_.Init( cio::COut );
qRR;
qRT;
qRE;
}

namespace {
	// As we do not need a backend, most of below items are only placeholders.
	class sDummy_
	{
	public:
		void reset( bso::sBool = true )
		{
			// Standardization.
		}
		qCVDTOR( sDummy_ );
		void Init( void )
		{
			// Standardization.
		}
	};


	class rFrontend_
	: public sclfrntnd::rFrontend,
	  public fblfrd::cFrontend
	{
	protected:
		virtual void FBLFRDOnConnection( void ) override {}
		virtual void FBLFRDOnDisconnection( void ) override {}
	public:
		void reset( bso::sBool = true )
		{
			// Standardization.
		}
		qCVDTOR( rFrontend_ );
		void Init( void )
		{
			// Standardization.
		}
	};

	qENUM( Dummy )
	{
		dAmount,
		d_Undefined
	};

	namespace {
		void Execute_(
            const str::dString &Script,
            xdhdws::sProxy &Proxy,
            str::dString &ReturnValue )
        {
        qRH;
            str::wStrings Values;
        qRB;
            Values.Init();
            Values.Append(Script);

            Proxy.Process( "Execute_1", Values, ReturnValue );
        qRR;
        qRT;
        qRE;
        }
    }

	void Execute_(
		flw::rRWFlow &Flow,
		xdhdws::sProxy &Proxy )
	{
	qRH;
		str::wString Script;
		str::wStrings Values;
		str::wString Result;
	qRB;
		Script.Init();
		prtcl::Get( Flow, Script );

		Flow.Dismiss();

		Values.Init();
		Values.Append(Script);

        Result.Init();
        Execute_(Script, Proxy, Result );

		prtcl::Put( Result, Flow );

		Flow.Commit();
	qRR;
	qRT;
	qRE;
	}

	void Alert_(
		flw::rRWFlow &Flow,
		xdhdws::sProxy &Proxy )
	{
	qRH;
		str::wString Message, Script;
		str::wString ReturnValue;
	qRB;
		Message.Init();
		prtcl::Get( Flow, Message );

		Flow.Dismiss();

		Script.Init( "window.alert(\"");
		xdhcmn::Escape( Message, Script, '"' );
		Script.Append( "\");'';");

        ReturnValue.Init();
		Execute_( Script, Proxy, ReturnValue );

		if (ReturnValue.Amount())
            qRGnr();

/*
	Despite the fact that this primitive does not need a return value,
	an empty string is returned in order for the back-end to wait
	that this primitive is achieved before launching another primitive.
	Without this, all other threads of the same back-end will be blocked
	until this primitive will return.
*/

		prtcl::Put( "", Flow );

		Flow.Commit();
	qRR;
	qRT;
	qRE;
	}

	void Confirm_(
		flw::rRWFlow &Flow,
		xdhdws::sProxy &Proxy )
	{
	qRH;
		str::wString Message, Script;
		str::wString ReturnValue;
	qRB;
		Message.Init();
		prtcl::Get( Flow, Message );

		Flow.Dismiss();

		Script.Init( "if ( window.confirm(\"");
		xdhcmn::Escape( Message, Script, '"' );
		Script.Append( "\") ) 'true'; else 'false';");

        ReturnValue.Init();
		Execute_( Script, Proxy, ReturnValue );

		prtcl::Put( ReturnValue, Flow );

		Flow.Commit();
	qRR;
	qRT;
	qRE;
	}

	qENUM( Mode_ ) {
		mFaaS,	// FaaS mode, direct connexion (not muxed) through the easy to install component.
		mProd,	// Production mode, muxed connexion through the native component.
		m_amount,
		m_Undefined
	};

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

	class rSession_
	: public xdhcmn::cSession,
	  public xdhdws::sProxy
	{
	private:
		eMode_ Mode_;
		faaspool::rRWDriver FaaSDriver_;
		csdmnc::rRWDriver ProdDriver_;
		struct {
			sId_ Id;
			str::wString IP;
		} Logging_;
		fdr::rRWDriver &D_( void )
		{
			switch ( Mode_ ) {
			case mFaaS:
				return FaaSDriver_;
				break;
			case mProd:
				return ProdDriver_;
				break;
			default:
				qRGnr();
				break;
			}

#ifdef CPE_C_CLANG
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wnull-dereference"
#endif

			return *(fdr::rRWDriver *)NULL;	// To avoid a warning.

#ifdef CPE_C_CLANG
# pragma clang diagnostic pop
#endif

		}
		void Log_( const str::dString &Message )
		{
		qRH;
			logq::rLogRack<> Log;
		qRB;
			Log.Init( LogDriver_ );

			Log << *Logging_.Id;

			if ( Logging_.IP.Amount() != 0 )
				Log << " (" << Logging_.IP << ")";

			Log << " : " <<  Message;
		qRR;
		qRT;
		qRE;
		}
		void ReportError_(
			const char *Message,
			flw::rWFlow &Flow )
		{
			if ( Message == NULL )
				Message = "";

			prtcl::Put( Message, Flow );	// If 'Message' not empty, client will display content and abort.

			if ( Message[0] ) {
				Flow.Commit();
				qRGnr();
			}
		}
		void ReportNoError_( flw::rWFlow &Flow )
		{
			ReportError_( NULL, Flow );
		}
		bso::bool__ Launch_(
			const char *Id,
			const char *Action )
		{
			bso::sBool Return = false;
		qRH;
			bso::sBool Continue = true;
			flw::rDressedRWFlow<> Flow;
			str::wString ScriptName, ReturnValue;
			str::wStrings Parameters, SplitedReturnValue;;
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

                    ReturnValue.Init();
                    Process( ScriptName, Parameters, ReturnValue );

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
		qRT;
		qRE;
			return Return;
		}
	protected:
		virtual bso::sBool XDHCMNInitialize(
			xdhcmn::cUpstream *Callback,
			const char *Language,
			const str::dString &Token ) override
		{
			bso::sBool Success = false;
		qRFH;
			flw::rDressedRWFlow<> Flow;
			csdcmn::sVersion Version = csdcmn::UndefinedVersion;
			str::wString LogMessage;
		qRFB;
			LogMessage.Init();

			if ( Token.Amount() == 0 ) {
				ProdDriver_.Init( Core_, fdr::ts_Default );
				Mode_ = mProd;
				Success = true;
				LogMessage.Append( "PROD" );
			} else {
				LogMessage.Append( Token );

				FaaSDriver_.Init();

				if ( faaspool::GetConnection( Token, Logging_.IP, FaaSDriver_.GetShared() ) ) {
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
					ReportNoError_( Flow );
					break;
				case 1:
					ReportNoError_( Flow );
					break;
				case csdcmn::UndefinedVersion:
					ReportError_( "\nIncompatible protocol! Please update your software.\n", Flow );
					break;
				default:
					ReportError_( "\nUnknown protocol version!\n", Flow );
					break;
				}

				prtcl::Put( "", Flow );
				Flow.Commit();

				csdcmn::Get( Flow, LogMessage );

				Logging_.Id = Ids_.New();
				Log_( LogMessage );

				xdhdws::sProxy::Init( Callback );	// Has to be last, otherwise if an error occurs, 'Callback' will be freed twice!
			}
		qRR;
		qRT;
		qRE;
			return Success;
		}
		virtual bso::bool__ XDHCMNLaunch(
			const char *Id,
			const char *Action ) override
		{
			bso::sBool Return = false;
		qRFH;
		qRFB;
			Return = Launch_( Id, Action );
		qRFR;
		qRFT;
		qRFE(sclmisc::ErrFinal());
			return Return;
		}
	public:
		void reset( bso::sBool P = true )
		{
			tol::reset( P, FaaSDriver_, ProdDriver_ );
			Mode_ = m_Undefined;
			xdhdws::sProxy::reset( P );
			Logging_.Id = UndefinedId_;
			Logging_.IP.reset( P );
		}
		qCVDTOR( rSession_ )
		bso::sBool Init( void )
		{
			tol::reset( FaaSDriver_, ProdDriver_ );
			Mode_ = m_Undefined;

			Logging_.IP.Init();

			return true;
		}
		operator fdr::rRWDriver &( void )
		{
			return D_();
		}
		fdr::rRWDriver &Driver( void )
		{
			return D_();
		}
	};
}

xdhcmn::cSession *sclxdhtml::SCLXDHTMLRetrieveSession(
	const char *Language,
	xdhcmn::eMode Mode,
	const str::dString &Token,
	xdhcmn::cUpstream *UpstreamCallback )
{
	rSession_ *Session = NULL;
qRH;
qRB;
	if ( ( Session = new rSession_ ) == NULL )
		qRAlc();

	if ( !Session->Init() ) {
		delete Session;
		Session = NULL;
	}

	// WARNING ! In 'MultiUser' mode, 'UpstreamCallback' is not correctly set yet!
/*	if ( Mode == xdhcmn::mMonoUser ) {
		Session->Launch( "", "" );
	}
*/
qRR;
	if ( Session != NULL )
		delete Session;

	Session = NULL;
qRT;
qRE;
	return Session;
}

const scli::sInfo &sclxdhtml::SCLXDHTMLInfo( void )
{
	return xdhqxdh::Info;
}

void sclxdhtml::SCLXDHTMLReleaseSession( xdhcmn::cSession *Session )
{
	if ( Session == NULL )
		qRGnr();

	delete Session;
}

qGCTOR( xdhqxdh )
{
	FillCommandAutomat_();
}
