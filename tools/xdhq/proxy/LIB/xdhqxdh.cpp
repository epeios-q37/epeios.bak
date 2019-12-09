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

#include "dmopool.h"
#include "registry.h"

#include "prtcl.h"

#include "logq.h"
#include "idsq.h"

#include "csdmnc.h"
#include "csdcmn.h"
#include "csdbns.h"

#include "sclmisc.h"

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

	qENUM( Command_ )
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

	const char *GetLabel_( eCommand_ Command )
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
		stsfsm::Fill<eCommand_>( CommandAutomat_, c_amount, GetLabel_ );
	}

	eCommand_ GetCommand_( flw::iflow__ &Flow )
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

	dmopool::Initialize();
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

	void Execute_(
		flw::rRWFlow &Flow,
		xdhdws::sProxy &Proxy )
	{
	qRH;
		str::wString Script;
		qCBUFFERr Buffer;
	qRB;
		Script.Init();
		prtcl::Get( Flow, Script );

		Flow.Dismiss();

		Proxy.Execute( Script, Buffer );

		prtcl::Put( Buffer(), Flow );

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
		qCBUFFERr Buffer;
	qRB;
		Message.Init();
		prtcl::Get( Flow, Message );

		Flow.Dismiss();

		Script.Init( "window.alert(\"");
		xdhcmn::Escape( Message, Script, '"' );
		Script.Append( "\");'';");

		Proxy.Execute( Script, Buffer );

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
		qCBUFFERr Buffer;
	qRB;
		Message.Init();
		prtcl::Get( Flow, Message );

		Flow.Dismiss();

		Script.Init( "if ( window.confirm(\"");
		xdhcmn::Escape( Message, Script, '"' );
		Script.Append( "\") ) 'true'; else 'false';");

		Proxy.Execute( Script, Buffer );

		prtcl::Put( Buffer(), Flow );

		Flow.Commit();
	qRR;
	qRT;
	qRE;
	}

	namespace {
		void EscapeNULChars_(
			const str::dString &Source,
			str::dString &Target )
		{
			sdr::sRow Row = Source.First();
			bso::sChar C = 0;

			while ( Row != qNIL ) {
				C = Source( Row );

				if ( C == 0 )
					Target.Append( "\\x00" );
				else
					Target.Append( C );

				Row = Source.Next( Row );
			}
		}
	}

	void HandleLayout_(
		flw::rRFlow &Flow,
		xdhdws::sProxy &Proxy,
		void (xdhdws::sProxy::* Method)(
			const ntvstr::rString &,
			const ntvstr::rString &L,
			const ntvstr::rString &) )
	{
	qRH;
		str::wString Id, RawXML, EscapedXML, XSL;
	qRB;
		tol::Init( Id, RawXML, XSL );

		prtcl::Get( Flow, Id );
		prtcl::Get( Flow, RawXML );
		prtcl::Get( Flow, XSL );

		Flow.Dismiss();

		if ( RawXML.Amount() == 0 )
			qRGnr();
		else if ( RawXML( RawXML.First() ) == '<' )
			(Proxy.*Method)( Id, RawXML, XSL );
		else {
			EscapedXML.Init();

			EscapeNULChars_( RawXML, EscapedXML );

			(Proxy.*Method)( Id, EscapedXML, XSL );
		}
	qRR;
	qRT;
	qRE;
	}

	void PrependLayout_(
		flw::rRFlow &Flow,
		xdhdws::sProxy &Proxy)
	{
		HandleLayout_(Flow, Proxy, &xdhdws::sProxy::PrependLayout);
	}

	void SetLayout_(
		flw::rRFlow &Flow,
		xdhdws::sProxy &Proxy)
	{
		HandleLayout_(Flow, Proxy, &xdhdws::sProxy::SetLayout);
	}

	void AppendLayout_(
		flw::rRFlow &Flow,
		xdhdws::sProxy &Proxy)
	{
		HandleLayout_(Flow, Proxy, &xdhdws::sProxy::AppendLayout);
	}

	namespace {
		void GetContents_(
			const str::dStrings &Ids,
			xdhdws::sProxy &Proxy,
			str::dStrings &Contents )
		{
		qRH;
			str::wString Content;
			sdr::sRow Row = qNIL;
		qRB;
			Row = Ids.First();

			while ( Row != qNIL ) {
				Content.Init();
				Proxy.GetValue( Ids( Row ), Content );

				Contents.Append( Content );

				Row = Ids.Next( Row );
			}
		qRR;
		qRT;
		qRE;
		}
	}

	void GetContents_(
		flw::rRWFlow &Flow,
		xdhdws::sProxy &Proxy )
	{
	qRH;
		str::wStrings Ids, Contents;
	qRB;
		Ids.Init();
		prtcl::Get( Flow, Ids );

		Flow.Dismiss();

		Contents.Init();
		GetContents_( Ids, Proxy, Contents );

		prtcl::Put( Contents, Flow );

		Flow.Commit();
	qRR;
	qRT;
	qRE;
	}

	void SetContents_(
		flw::rRFlow &Flow,
		xdhdws::sProxy &Proxy )
	{
	qRH;
		str::wStrings Ids, Contents;
		str::wString MergedIds, MergedContents;
	qRB;
		tol::Init( Ids, Contents );
		prtcl::Get( Flow, Ids );
		prtcl::Get( Flow, Contents );

		Flow.Dismiss();

		tol::Init( MergedIds, MergedContents );

		xdhcmn::FlatMerge( Ids, MergedIds, true );
		xdhcmn::FlatMerge( Contents, MergedContents, true );

		Proxy.SetContents( MergedIds, MergedContents );
	qRR;
	qRT;
	qRE;
	}

	void SetTimeout_(
		flw::rRFlow &Flow,
		xdhdws::sProxy &Proxy )
	{
	qRH;
		str::wString Delay, Action;
	qRB;
		tol::Init( Delay, Action );
		prtcl::Get( Flow, Delay );
		prtcl::Get( Flow, Action );

		Flow.Dismiss();

		Proxy.SetTimeout( Delay, Action );
	qRR;
	qRT;
	qRE;
	}

	void Parent_(
		flw::rRWFlow &Flow,
		xdhdws::sProxy &Proxy )
	{
	qRH;
		str::wString Id;
		qCBUFFERr Parent;
	qRB;
		tol::Init( Id );
		prtcl::Get( Flow, Id );

		Flow.Dismiss();

		Proxy.Parent( Id, Parent );

		prtcl::Put( Parent(), Flow );

		Flow.Commit();
	qRR;
	qRT;
	qRE;
	}

	void FirstChild_(
		flw::rRWFlow &Flow,
		xdhdws::sProxy &Proxy )
	{
	qRH;
		str::wString Id;
		qCBUFFERr Child;
	qRB;
		tol::Init( Id );
		prtcl::Get( Flow, Id );

		Flow.Dismiss();

		Proxy.FirstChild( Id, Child );

		prtcl::Put( Child(), Flow );

		Flow.Commit();
	qRR;
	qRT;
	qRE;
	}

	void LastChild_(
		flw::rRWFlow &Flow,
		xdhdws::sProxy &Proxy )
	{
	qRH;
		str::wString Id;
		qCBUFFERr Child;
	qRB;
		tol::Init( Id );
		prtcl::Get( Flow, Id );

		Flow.Dismiss();

		Proxy.LastChild( Id, Child );

		prtcl::Put( Child(), Flow );

		Flow.Commit();
	qRR;
	qRT;
	qRE;
	}

	void PreviousSibling_(
		flw::rRWFlow &Flow,
		xdhdws::sProxy &Proxy )
	{
	qRH;
		str::wString Id;
		qCBUFFERr Sibling;
	qRB;
		tol::Init( Id );
		prtcl::Get( Flow, Id );

		Flow.Dismiss();

		Proxy.PreviousSibling( Id, Sibling );

		prtcl::Put( Sibling(), Flow );

		Flow.Commit();
	qRR;
	qRT;
	qRE;
	}

	void NextSibling_(
		flw::rRWFlow &Flow,
		xdhdws::sProxy &Proxy )
	{
	qRH;
		str::wString Id;
		qCBUFFERr Sibling;
	qRB;
		tol::Init( Id );
		prtcl::Get( Flow, Id );

		Flow.Dismiss();

		Proxy.NextSibling( Id, Sibling );

		prtcl::Put( Sibling(), Flow );

		Flow.Commit();
	qRR;
	qRT;
	qRE;
	}

	void CreateElement_(
		flw::rRWFlow &Flow,
		xdhdws::sProxy &Proxy )
	{
	qRH;
		str::wString Name, SuggestedId;
		qCBUFFERr Id;
	qRB;
		tol::Init( Name, SuggestedId );

		prtcl::Get( Flow, Name );
		prtcl::Get( Flow, SuggestedId );

		Flow.Dismiss();

		Proxy.CreateElement( Name, SuggestedId, Id );	// If 'SuggestedId' is empty, 'Id' will contain a computer generated one, otherwise it will contain 'SuggestedId'.

		prtcl::Put( Id(), Flow );

		Flow.Commit();
	qRR;
	qRT;
	qRE;
	}

	void InsertChild_(
		flw::rRFlow &Flow,
		xdhdws::sProxy &Proxy )
	{
	qRH;
		str::wString Child, Id;
	qRB;
		tol::Init( Child, Id );

		prtcl::Get( Flow, Child );
		prtcl::Get( Flow, Id );

		Flow.Dismiss();

		Proxy.InsertChild( Child, Id );
	qRR;
	qRT;
	qRE;
	}

	void AppendChild_(
		flw::rRFlow &Flow,
		xdhdws::sProxy &Proxy )
	{
	qRH;
		str::wString Child, Id;
	qRB;
		tol::Init( Child, Id );

		prtcl::Get( Flow, Child );
		prtcl::Get( Flow, Id );

		Flow.Dismiss();

		Proxy.AppendChild( Child, Id );
	qRR;
	qRT;
	qRE;
	}

	void InsertBefore_(
		flw::rRFlow &Flow,
		xdhdws::sProxy &Proxy )
	{
	qRH;
		str::wString Sibling, Id;
	qRB;
		tol::Init( Sibling, Id );

		prtcl::Get( Flow, Sibling );
		prtcl::Get( Flow, Id );

		Flow.Dismiss();

		Proxy.InsertBefore( Sibling, Id );
	qRR;
	qRT;
	qRE;
	}

	void InsertAfter_(
		flw::rRFlow &Flow,
		xdhdws::sProxy &Proxy )
	{
	qRH;
		str::wString Sibling, Id;
	qRB;
		tol::Init( Sibling, Id );

		prtcl::Get( Flow, Sibling );
		prtcl::Get( Flow, Id );

		Flow.Dismiss();

		Proxy.InsertAfter( Sibling, Id );
	qRR;
	qRT;
	qRE;
	}

	void InsertCSSRule_(
		flw::rRFlow &Flow,
		xdhdws::sProxy &Proxy)
	{
	qRH;
		str::wString Id, Rule, Index;
	qRB;
		tol::Init(Id, Rule, Index);

		prtcl::Get(Flow, Id);
		prtcl::Get(Flow, Rule);
		prtcl::Get(Flow, Index);

		Flow.Dismiss();

		Proxy.InsertCSSRule(Id, Rule, Index);
	qRR;
	qRT;
	qRE;
	}

	void AppendCSSRule_(
		flw::rRWFlow &Flow,
		xdhdws::sProxy &Proxy)
	{
	qRH;
		str::wString Id, Rule;
		qCBUFFERr Index;
	qRB;
		tol::Init(Id, Rule);

		prtcl::Get(Flow, Id);
		prtcl::Get(Flow, Rule);

		Flow.Dismiss();

		Proxy.AppendCSSRule(Id, Rule, Index);

		prtcl::Put(Index(), Flow);

		Flow.Commit();
	qRR;
	qRT;
	qRE;
	}

	void RemoveCSSRule_(
		flw::rRFlow &Flow,
		xdhdws::sProxy &Proxy)
	{
	qRH;
		str::wString Id, Index;
	qRB;
		tol::Init(Id, Index);

		prtcl::Get(Flow, Id);
		prtcl::Get(Flow, Index);

		Flow.Dismiss();

		Proxy.RemoveCSSRule(Id, Index);
	qRR;
	qRT;
	qRE;
	}

	void DressWidgets_(
		flw::rRFlow &Flow,
		xdhdws::sProxy &Proxy )
	{
	qRH;
		str::wString Id;
	qRB;
		Id.Init();
		prtcl::Get( Flow, Id );

		Flow.Dismiss();

		Proxy.DressWidgets( Id );
	qRR;
	qRT;
	qRE;
	}

	namespace {
		void HandleClasses_(
			flw::rRFlow &Flow,
			void (xdhdws::sProxy::* Method)(
				const xdhdws::rNString &Ids,
				const xdhdws::rNString &Classes),
			xdhdws::sProxy &Proxy )
		{
		qRH;
			str::wStrings Ids, Classes;
			str::wString MergedIds, MergedClasses;
		qRB;
			tol::Init( Ids, Classes );

			prtcl::Get( Flow, Ids );
			prtcl::Get( Flow, Classes );

			Flow.Dismiss();

			tol::Init( MergedIds, MergedClasses );
			xdhcmn::FlatMerge( Ids, MergedIds, true );
			xdhcmn::FlatMerge( Classes, MergedClasses, true );

			(Proxy.*Method)( MergedIds, MergedClasses );
		qRR;
		qRT;
		qRE;
		}
	}

	void AddClasses_(
		flw::rRFlow &Flow,
		xdhdws::sProxy &Proxy )
	{
		HandleClasses_( Flow, &xdhdws::sProxy::AddClasses, Proxy );
	}

	void RemoveClasses_(
		flw::rRFlow &Flow,
		xdhdws::sProxy &Proxy )
	{
		HandleClasses_( Flow, &xdhdws::sProxy::RemoveClasses, Proxy );
	}

	void ToggleClasses_(
		flw::rRFlow &Flow,
		xdhdws::sProxy &Proxy )
	{
		HandleClasses_( Flow, &xdhdws::sProxy::ToggleClasses, Proxy );
	}

	namespace {
		void HandleElements_(
			flw::rRFlow &Flow,
			void (xdhdws::sProxy::* Method)( const xdhdws::rNString &Ids ),
			xdhdws::sProxy &Proxy )
		{
		qRH;
			str::wStrings Ids;
			str::wString MergedIds;
		qRB;
			tol::Init( Ids );

			prtcl::Get( Flow, Ids );

			Flow.Dismiss();

			tol::Init( MergedIds );
			xdhcmn::FlatMerge( Ids, MergedIds, true );

			(Proxy.*Method)( MergedIds );
		qRR;
		qRT;
		qRE;
		}
	}

	void EnableElements_(
		flw::rRFlow &Flow,
		xdhdws::sProxy &Proxy )
	{
		HandleElements_( Flow, &xdhdws::sProxy::EnableElements, Proxy );
	}

	void DisableElements_(
		flw::rRFlow &Flow,
		xdhdws::sProxy &Proxy )
	{
		HandleElements_( Flow, &xdhdws::sProxy::DisableElements, Proxy );
	}

	void SetAttribute_(
		flw::rRFlow &Flow,
		xdhdws::sProxy &Proxy )
	{
	qRH;
		str::wString Id, Name, Value;
	qRB;
		tol::Init( Id, Name, Value );
		prtcl::Get( Flow, Id );
		prtcl::Get( Flow, Name );
		prtcl::Get( Flow, Value );

		Flow.Dismiss();

		Proxy.SetAttribute( Id, Name, Value );
	qRR;
	qRT;
	qRE;
	}

	void GetAttribute_(
		flw::rRWFlow &Flow,
		xdhdws::sProxy &Proxy )
	{
	qRH;
		str::wString Id, Name, Value;
	qRB;
		tol::Init( Id, Name );
		prtcl::Get( Flow, Id );
		prtcl::Get( Flow, Name );

		Flow.Dismiss();

		Value.Init();
		Proxy.GetAttribute( Id, Name, Value );

		prtcl::Put( Value, Flow );
		Flow.Commit();
	qRR;
	qRT;
	qRE;
	}

	void RemoveAttribute_(
		flw::rRFlow &Flow,
		xdhdws::sProxy &Proxy )
	{
	qRH;
		str::wString Id, Name;
	qRB;
		tol::Init( Id, Name );
		prtcl::Get( Flow, Id );
		prtcl::Get( Flow, Name );

		Flow.Dismiss();

		Proxy.RemoveAttribute( Id, Name );
	qRR;
	qRT;
	qRE;
	}

	void SetProperty_(
		flw::rRFlow &Flow,
		xdhdws::sProxy &Proxy )
	{
	qRH;
		str::wString Id, Name, Value;
	qRB;
		tol::Init( Id, Name, Value );
		prtcl::Get( Flow, Id );
		prtcl::Get( Flow, Name );
		prtcl::Get( Flow, Value );

		Flow.Dismiss();

		Proxy.SetProperty( Id, Name, Value );
	qRR;
	qRT;
	qRE;
	}

	void GetProperty_(
		flw::rRWFlow &Flow,
		xdhdws::sProxy &Proxy )
	{
	qRH;
		str::wString Id, Name, Value;
	qRB;
		tol::Init( Id, Name );
		prtcl::Get( Flow, Id );
		prtcl::Get( Flow, Name );

		Flow.Dismiss();

		Value.Init();
		Proxy.GetProperty( Id, Name, Value );

		prtcl::Put( Value, Flow );

		Flow.Commit();
	qRR;
	qRT;
	qRE;
	}

	void Focus_(
		flw::rRFlow &Flow,
		xdhdws::sProxy &Proxy )
	{
	qRH;
		str::wString Id;
	qRB;
		tol::Init( Id );
		prtcl::Get( Flow, Id );

		Flow.Dismiss();

		Proxy.Focus( Id );
	qRR;
	qRT;
	qRE;
	}

	qENUM( Mode_ ) {
		mDemo,	// Demo mode, direct connexion (not muxed) through the easy to install component.
		mProd,	// Production mode, muxed connexion through the native component.
		m_amount,
		m_Undefined
	};

	class rSession_
	: public xdhcmn::cSession,
	  public xdhdws::sProxy
	{
	private:
		eMode_ Mode_;
		dmopool::rRWDriver DemoDriver_;
		csdmnc::rRWDriver ProdDriver_;
		struct {
			sId_ Id;
			str::wString IP;
		} Logging_;
		fdr::rRWDriver &D_( void )
		{
			switch ( Mode_ ) {
			case mDemo:
				return DemoDriver_;
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
			eCommand_ Command = c_Undefined;
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
				Command = GetCommand_( Flow );

				Log_( str::wString( GetLabel_( Command ) ) );

				switch ( Command ) {
				case cStandBy_1:
					Return = true;
					Continue = false;
					Flow.Dismiss();
					break;
				H( Execute );
				H( Alert );
				H( Confirm );
				H( PrependLayout);
				H( SetLayout);
				H( AppendLayout);
				H( GetContents );
				H( SetContents );
				H( SetTimeout );
				H( Parent );
				H( FirstChild );
				H( LastChild );
				H( PreviousSibling );
				H( NextSibling );
				H( CreateElement );
				H( InsertChild );
				H( AppendChild );
				H( InsertBefore );
				H( InsertAfter );
				H( InsertCSSRule);
				H( AppendCSSRule);
				H( RemoveCSSRule);
				H( DressWidgets );
				H( AddClasses );
				H( RemoveClasses );
				H( ToggleClasses );
				H( EnableElements );
				H( DisableElements );
				H( SetAttribute );
				H( GetAttribute );
				H( RemoveAttribute );
				H( SetProperty );
				H( GetProperty );
				H( Focus );
				default:
					qRGnr();
					break;
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

				DemoDriver_.Init();

				if ( dmopool::GetConnection( Token, Logging_.IP, DemoDriver_.GetShared() ) ) {
					Mode_ = mDemo;
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
					ReportError_( "Unknown protocol !!!", Flow );
					break;
				default:
					ReportError_( "Unknown protocol version !!!", Flow );
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
			tol::reset( P, DemoDriver_, ProdDriver_ );
			Mode_ = m_Undefined;
			xdhdws::sProxy::reset( P );
			Logging_.Id = UndefinedId_;
			Logging_.IP.reset( P );
		}
		qCVDTOR( rSession_ )
		bso::sBool Init( void )
		{
			tol::reset( DemoDriver_, ProdDriver_ );
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

xdhcmn::cSession *sclxdhtml::SCLXDHTMLRetrieveCallback(
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

void sclxdhtml::SCLXDHTMLReleaseCallback( xdhcmn::cSession *Callback )
{
	if ( Callback == NULL )
		qRGnr();

	delete Callback;
}

qGCTOR( xdhqxdh )
{
	FillCommandAutomat_();
}
