/*
	Copyright (C) 2014-2016 by Claude SIMON (http://zeusw.org/epeios/contact.html).

    This file is part of 'xdhcefq'.

    'xdhcefq' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'xdhcefq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'xdhcefq'.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "agent.h"

#include "registry.h"

#include "xdhujp.h"

#include "scltool.h"

using namespace agent;

using xdhcmn::nstring___;
using xdhcmn::nchar__;

namespace {

	/*
	typedef void (CEF_CALLBACK *visit__)(
		struct _cef_domvisitor_t* self,
		struct _cef_domdocument_t* document );

	class layout_dom_visitor__
	: public cef_domvisitor_t
	{
	private:
		Q37_MRMDF( agent___, _A, _Agent );
		const nchar__ *_Id;
	public:
		void reset( bso::bool__ P = true )
		{
			misc::Set<cef_domvisitor_t>( this );
			_Agent = NULL;
			_Id = NULL;
		}
		E_CDTOR( layout_dom_visitor__ );
		void Init(
			visit__ Visit,
			const nchar__ *Id,
			agent___ &Agent )
		{
			misc::Set<cef_domvisitor_t>( this );
			visit = Visit;
			_Id = Id;
			_Agent = &Agent;
		}
		const nchar__ *Id( void ) const
		{
			if ( _Id == NULL )
				qRGnr();

			return _Id;
		}
		agent___ &Agent( void ) const
		{
			return _A();
		}
		cef_browser_t *Browser( void )
		{
			return Agent().Browser();
		}
		operator cef_domvisitor_t *()
		{
			return this;
		}
	};

	class crawler__
	{
	private:
		cef_domnode_t *_Root, *_Current;
	public:
		void reset( bso::bool__ = true )
		{
			_Root = _Current = NULL;
		}
		crawler__( void )
		{
			reset( false );
		}
		~crawler__( void )
		{
			reset();
		}
		void Init( cef_domnode_t *Root )
		{
			reset();

			_Current = _Root = Root;
		}
		// Returns next node, or 'NULL' if all were red.
		cef_domnode_t *GetNext( void )
		{
			cef_domnode_t *Buffer = NULL;
			int Same = 0;

			if ( _Current != NULL ) {
				if ( _Current == _Root )
					_Current = _Root->get_first_child( _Root );
				else if ( ( Buffer = _Current->get_first_child( _Current ) ) != NULL ) {
					misc::Release( _Current );
					_Current = Buffer;
				} else if ( ( Buffer = _Current->get_next_sibling( _Current ) ) != NULL ) {
					misc::Release( _Current );
					_Current = Buffer;
				} else {
					do {
						Buffer = _Current;
						_Current = Buffer->get_parent( Buffer );
						misc::Release( Buffer );
						_Root->base.add_ref( &_Root->base );	// 'is_same' eats a ref.
					} while ( ( !( Same = _Current->is_same( _Current, _Root ) ) ) && ( ( Buffer = _Current->get_next_sibling( _Current ) ) == NULL) );

					if ( Same ) {
						misc::Release( _Current );
						_Current = NULL;
					} else {
						misc::Release( _Current );
						_Current = Buffer;
					}
				}
			}

			return _Current;
		}
	};

	void FillEventsDigest_(
		const str::string_ &Id,
		cef_frame_t *Frame,
		xdhcbk::args_ &Digest )
	{
	qRH
		str::string TagName;
		xdhcbk::args Events;
	qRB
		TagName.Init();
		misc::GetName( Id, Frame, TagName );

		Events.Init();
		misc::GetEvents( Id, Frame, Events );

		Digest.Append( Id );
		Digest.Append( TagName );
		Digest.AppendMono( Events );
	qRR
	qRT
	qRE
	}

	void FillWidgetDigest_(
		const str::string_ &Id,
		cef_frame_t *Frame,
		xdhcbk::args_ &Digest )
	{
	qRH
		str::string RawFeatures;
		xdhcbk::args Features;
		TOL_CBUFFER___ Buffer;
	qRB
		RawFeatures.Init();
		misc::MGetAttribute( Id, sclmisc::MGetValue( registry::custom_item::attribute_name::Widget, Buffer ), Frame, RawFeatures );

		Features.Init();
		xdhcbk::Split( RawFeatures, Features );

		Digest.Append( Id );
		Digest.AppendMono( Features );
	qRR
	qRT
	qRE
	}

	void CEF_CALLBACK LayoutVisit_(
		struct _cef_domvisitor_t* self,
		struct _cef_domdocument_t* document )
	{
	qRH
		layout_dom_visitor__ &Visitor = *(layout_dom_visitor__ *)self;
		cef_domnode_t *Root = NULL, *Node = NULL;
		crawler__ Crawler;
		str::string Name, Script;
		misc::cef_string___ OnEventAttributeName, OnEventsAttributeName, WidgetAttributeName;
		str::string Id;
		TOL_CBUFFER___ Buffer;
		xdhcbk::args EventsDigest, WidgetDigest;
		cef_frame_t *Frame = NULL;
	qRB
		Frame = misc::GetFrame( Visitor.Browser() );

		OnEventAttributeName.Init( sclmisc::MGetValue( registry::custom_item::attribute_name::OnEvent, Buffer ) );
		OnEventsAttributeName.Init( sclmisc::MGetValue( registry::custom_item::attribute_name::OnEvents, Buffer ) );
		WidgetAttributeName.Init( sclmisc::MGetValue( registry::custom_item::attribute_name::Widget, Buffer ) );

		Crawler.Init( Root = document->get_element_by_id( document, misc::cef_string___( Visitor.Id() ) ) );

		Script.Init();
		
		while ( ( Node = Crawler.GetNext() ) != NULL) {
			Name.Init();
			misc::GetName( Node, Name );
			str::ToLower( Name );

			if ( Node->is_element( Node ) ) {
				if ( ( Node->has_element_attribute(Node, OnEventAttributeName ) )
					 || (Node->has_element_attribute(Node, OnEventsAttributeName)) ) {
					EventsDigest.Init();
					Id.Init();
					FillEventsDigest_( misc::GetOrGenerateId( Node, Id ), Frame, EventsDigest );
					xdhjst::scripter::HandleEventsDigest( EventsDigest, Script );

				}
			
				if ( Node->has_element_attribute(Node, WidgetAttributeName) ) {
					WidgetDigest.Init();
					FillWidgetDigest_( misc::GetOrGenerateId( Node, Id ), Frame, WidgetDigest );
					xdhjst::scripter::HandleWidgetDigest( WidgetDigest, Script );
				}
			}
		}

		misc::ExecuteJavascript( Script, Frame );
	qRR
	qRT
		if ( Root != NULL )
			misc::Release( Root );
		misc::Release( document );
		misc::Release( self );
	qRE
	}

	void HandleExtensions_(
		const nchar__ *Id,
		agent___ &Agent )
	{
		layout_dom_visitor__ Visitor;
		cef_frame_t *Frame = Agent.Frame();

		Visitor.Init( LayoutVisit_, Id, Agent );

		Frame->visit_dom( Frame, &Visitor );
	}

	class casting_dom_visitor__
	: public cef_domvisitor_t
	{
	private:
		Q37_MRMDF( agent___, _A, _Agent );
		const nchar__ *_Id;
	public:
		void reset( bso::bool__ P = true )
		{
			misc::Set<cef_domvisitor_t>( this );
			_Agent = NULL;
			_Id = NULL;
		}
		E_CDTOR( casting_dom_visitor__ );
		void Init(
			visit__ Visit,
			const nchar__ *Id,
			agent___ &Agent )
		{
			misc::Set<cef_domvisitor_t>( this );
			visit = Visit;
			_Id = Id;
			_Agent = &Agent;
		}
		const nchar__ *Id( void ) const
		{
			if ( _Id == NULL )
				qRGnr();

			return _Id;
		}
		agent___ &Agent( void ) const
		{
			return _A();
		}
		cef_browser_t *Browser( void )
		{
			return Agent().Browser();
		}
		operator cef_domvisitor_t *()
		{
			return this;
		}
	};

	inline void GetCast_(
		const xdhcbk::args_ &Args,
		str::string_ &Cast )
	{
		xdhcbk::retriever__ Retriever;

		if ( Args.Amount() == 1 ) {
			Retriever.Init( Args );
			Retriever.GetString( Cast );
		} else
			qRGnr();
	}

	void GetMonoCast_(
		const xdhcbk::args_ &Args,
		str::strings_ &Casts )
	{
	qRH
		str::string Cast;
	qRB
		Cast.Init();

		GetCast_( Args, Cast );

		Casts.Append( Cast );
	qRR
	qRT
	qRE
	}

	void GetMultiCasts_(
		const xdhcbk::args_ &Args,
		str::strings_ &Casts )
	{
	qRH
		xdhcbk::args SubArgs;
		strmrg::retriever__ Retriever;
	qRB
		Retriever.Init( Args );

		while ( Retriever.Availability() != strmrg::aNone ) {
			SubArgs.Init();

			Retriever.GetTable( SubArgs );

			GetMonoCast_( SubArgs, Casts );
		}
	qRR
	qRT
	qRE
	}

	void GetCasts_(
		cef_domnode_t *Node,
		str::strings_ &Casts )
	{
	qRH
		str::string Value;
		xdhcbk::args Args;
		TOL_CBUFFER___ Buffer;
	qRB
		Value.Init();
		misc::OGetAttribute( Node, sclmisc::MGetValue( registry::custom_item::attribute_name::Cast, Buffer ), Value );

		if ( Value.Amount() != 0 ) {
			Args.Init();
			xdhcbk::Split( Value, Args );
			GetMonoCast_( Args, Casts );
		}

		Value.Init();
		misc::OGetAttribute( Node, sclmisc::MGetValue( registry::custom_item::attribute_name::Casts, Buffer ), Value );

		if ( Value.Amount() != 0 ) {
			Args.Init();
			xdhcbk::Split( Value, Args );
			GetMultiCasts_( Args, Casts );
		}
	qRR
	qRT
	qRE
	}

	void FillCastsDigest_(
		const str::string_ &Id,
		cef_frame_t *Frame,
		xdhcbk::args_ &Digest )
	{
	qRH
		xdhcbk::args Casts;
	qRB
		Casts.Init();
		misc::GetCasts( Id, Frame, Casts );

		Digest.Append( Id );
		Digest.AppendMono( Casts );
	qRR
	qRT
	qRE
	}

	void CEF_CALLBACK CastingVisit_(
		struct _cef_domvisitor_t* self,
		struct _cef_domdocument_t* document )
	{
	qRH
		casting_dom_visitor__ &Visitor = *(casting_dom_visitor__ *)self;
		cef_domnode_t *Root = NULL, *Node = NULL;
		cef_frame_t *Frame = NULL;
		crawler__ Crawler;
		misc::cef_string___ CastAttributeName, CastsAttributeName;
		TOL_CBUFFER___ Buffer;
		str::string Script, Id;
		xdhcbk::args Digest;
	qRB
		CastAttributeName.Init( sclmisc::MGetValue( registry::custom_item::attribute_name::Cast, Buffer ) );
		CastsAttributeName.Init( sclmisc::MGetValue( registry::custom_item::attribute_name::Casts, Buffer ) );

		Frame = misc::GetFrame( Visitor.Browser() );

		Crawler.Init( Root = document->get_element_by_id( document, misc::cef_string___( Visitor.Id() ) ) );

		Script.Init();

		while ( ( Node = Crawler.GetNext() ) != NULL) {
			if ( Node->is_element( Node ) ) {
				if ( ( Node->has_element_attribute( Node, CastAttributeName ))
					 || (Node->has_element_attribute( Node, CastsAttributeName )) ) {
					Digest.Init();
					Id.Init();
					FillCastsDigest_( misc::GetOrGenerateId( Node, Id ), Frame, Digest );

					xdhjst::scripter::HandleCastsDigest( Digest, Script );
				}
			}
		}

		
		misc::ExecuteJavascript( Script, Frame );

	qRR
	qRT
		if ( Root != NULL )
			misc::Release( Root );
		misc::Release( document );
		misc::Release( self );
	qRE
	}

	void HandleCastings_(
		const nchar__ *Id,
		agent___ &Agent )
	{
		casting_dom_visitor__ Visitor;
		cef_frame_t *Frame = Agent.Frame();

		Visitor.Init( CastingVisit_, Id, Agent );

		Frame->visit_dom( Frame, &Visitor );
	}
*/
	void HandleFileRelatedPredefinedAction_(
		misc::client_message__ Message,
		const str::string_ &UserAction,
		cef_browser_t *Browser,
		const str::string_ &TargetId,
		const xdhcmn::digest_ &Args )
	{
	qRH
		xdhcmn::digest MetaArgs;
		TOL_CBUFFER___ Buffer;
	qRB
		MetaArgs.Init();

		MetaArgs.Append( TargetId );
		MetaArgs.Append( UserAction );

		MetaArgs.AppendMono( Args );

		misc::SendMessage( Message, Browser, MetaArgs );
	qRR
	qRT
	qRE
	}

	void HandlePredefinedAction_(
		misc::action__ Action,
		const str::string_ &UserAction,
		cef_browser_t *Browser,
		const str::string_ &TargetId,
		const xdhcmn::digest_ &Args )
	{
		switch ( Action ) {
		case misc::aOpenFile:
			HandleFileRelatedPredefinedAction_( misc::cmOpenFile, UserAction, Browser, TargetId, Args );
			break;
		case misc::aOpenFiles:
			HandleFileRelatedPredefinedAction_( misc::cmOpenFiles, UserAction, Browser, TargetId, Args );
			break;
		case misc::aSaveFile:
			HandleFileRelatedPredefinedAction_( misc::cmSaveFile, UserAction, Browser, TargetId, Args );
			break;
		default:
			qRGnr();
			break;
		}
	}

}

namespace {
	typedef xdhujp::callback__ _callback__;

	class callback__
	: public _callback__
	{
	public:
		Q37_MRMDF( agent___, _A, _Agent );
		cef_browser_t *_B( void ) const
		{
			return _A().Browser();
		}
		cef_frame_t *_F( void ) const
		{
			return _A().Frame();
		}
	protected:
		virtual void XDHUJPExecute(
			const str::string_ &Script,
			TOL_CBUFFER___ *Buffer ) override
		{
			misc::ExecuteJavascript( Script, Buffer, _F() );
		}
		virtual void XDHUJPGetWidgetAttributeName( TOL_CBUFFER___ &Buffer ) override
		{
			sclmisc::MGetValue( registry::custom_item::attribute_name::Widget, Buffer);
		}
		virtual void XDHUJPGetResultAttributeName( TOL_CBUFFER___ &Buffer ) override
		{
			sclmisc::MGetValue( registry::custom_item::attribute_name::Result, Buffer );
		}
		/*
		virtual void XDHJSPHandleExtensions( const xdhcbk::nstring___ &Id ) override
		{
			HandleExtensions_( Id, _A() );
		}
		virtual void XDHJSPHandleCastings( const xdhcbk::nstring___ &Id ) override
		{
			HandleCastings_(Id, _A() );
		}
		*/
	public:
		void reset( bso::bool__ P = true )
		{
			_callback__::reset( P );
			_Agent = NULL;
		}
		E_CVDTOR( callback__);
		void Init( agent___ &Agent )
		{
			_callback__::Init();
			_Agent = &Agent;
		}
	};

	callback__ Callback_;
}

void agent::agent___::_InitializeSession( void )
{
qRH
	xdhujp::proxy_callback__ *ProxyCallback;
qRB
	ProxyCallback = new xdhujp::proxy_callback__;	// Destruction is made by '_Session'.

	if ( ProxyCallback == NULL )
		qRGnr();

	::Callback_.Init( *this );

	ProxyCallback->Init( ::Callback_ );

	_Session.Init( RetrieveCallback( Language( _LanguageBuffer ), ProxyCallback ) );
qRR
	if ( ProxyCallback != NULL )
		delete ProxyCallback;
qRT
qRE
}

bso::bool__ agent::agent___::HandleEvent( const str::string_  &Digest )
{
	bso::bool__ Stop = true;
qRH
	str::string Id;
	misc::event_abstract Abstract;
	TOL_CBUFFER___ IdBuffer, ActionBuffer;
qRB
	Id.Init();
	Abstract.Init();
	if ( misc::FetchEventAbstract(Digest, Id, Abstract) ) {
		if ( misc::IsPredefined( Abstract.Action() ) )
			HandlePredefinedAction_( Abstract.Action(), Abstract.UserAction, _B(), Id, Abstract.Args );
		else if ( Abstract.Action() == misc::a_User )
			Stop = Launch( Id.Convert( IdBuffer ), Abstract.UserAction.Convert( ActionBuffer ) );
		else
			qRGnr();
	}
qRR
qRT
qRE
	return Stop;
}

