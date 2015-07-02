/*
	Copyright (C) 2000-2015 Claude SIMON (http://q37.info/contact/).

	This file is part of the Epeios framework.

	The Epeios framework is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

	The Epeios framework is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with the Epeios framework.  If not, see <http://www.gnu.org/licenses/>
*/

#define XDHDWS__COMPILATION

#include "xdhdws.h"

using namespace xdhdws;

namespace {
	void SetXML_(
		const nstring___ &Message,
		str::string_ &XML )
	{
	qRH
		flx::E_STRING_TOFLOW___ STOFlow;
		xml::writer Writer;
		str::string Buffer;
	qRB
		STOFlow.Init( XML );
		Writer.Init( STOFlow, xml::oCompact, xml::e_Default );

		Buffer.Init();
		Writer.PutValue( Message.UTF8( Buffer ), "Content" );
	qRR
	qRT
	qRE
	}

	inline void SetXSL_( str::string_ &XSL )
	{
		XSL.Append("<?xml version=\"1.0\" encoding=\"utf-8\"?>\
			<xsl:stylesheet version=\"1.0\"\
			                xmlns:xsl=\"http://www.w3.org/1999/XSL/Transform\">\
				<xsl:output method=\"text\"\
					        encoding=\"utf-8\"/>\
				<xsl:template match=\"/\">\
					<xsl:value-of select=\"Content\"/>\
				</xsl:template>\
			</xsl:stylesheet>\
		");
	}
}

void xdhdws::proxy__::Alert( const nstring___ &Message )
{
qRH
	str::string XML, XSL;
qRB
	XML.Init();
	SetXML_( Message, XML );

	XSL.Init();
	SetXSL_( XSL );

	Alert( XML, XSL, nstring___());
qRR
qRT
qRE
}

bso::bool__ xdhdws::proxy__::Confirm( const nstring___ &Message )
{
	bso::bool__ Confirmed = false;
qRH
	str::string XML, XSL;
qRB
	XML.Init();
	SetXML_( Message, XML );
	
	XSL.Init();
	SetXSL_( XSL );

	Confirmed = Confirm( XML, XSL, nstring___() );
qRR
qRT
qRE
	return Confirmed;
}

void xdhdws::prolog::GetContent(
	proxy__ &Proxy,
	xml::writer_ &Writer)
{
	// Rien  fournir.
}

static frdbse::project_type__ GetProjectType_( proxy__ &Proxy )
{
	frdbse::project_type__ ProjectType = frdbse::pt_Undefined;
qRH
	str::string Value;
qRB
	Value.Init();
	ProjectType = frdbse::GetProjectType( Proxy.GetContent( prolog::ProjectTypeId, Value ) );
qRR
qRT
qRE
	return ProjectType;
}

void xdhdws::prolog::GetContext(
	proxy__ &Proxy,
	xml::writer_ &Writer)
{
	Writer.PushTag( "ProjectType ");

	Writer.PutValue( frdbse::GetLabel( GetProjectType_( Proxy ) ) );

	Writer.PopTag();
}

frdbse::project_type__ xdhdws::prolog::GetProjectFeatures(
	proxy__ &Proxy,
	str::string_ &ProjectFeature )
{
	frdbse::project_type__ ProjectType = frdbse::pt_Undefined;
qRH
	TOL_CBUFFER___ Buffer;
qRB
	switch ( ProjectType = GetProjectType_( Proxy ) ) {
	case frdbse::ptNew:
		break;
	case frdbse::ptPredefined:
		ProjectFeature.Append( Proxy.GetContent( PredefinedProjectId, Buffer ) );
		break;
	case frdbse::ptUser:
		ProjectFeature.Append( Proxy.GetContent( UserProjectId, Buffer ) );
		break;
	default:
		qRFwk();
		break;
	}
qRR
qRT
qRE
	return ProjectType;
}

void xdhdws::prolog::DisplaySelectedProjectFileName(
	proxy__ &Proxy,
	const char *Id )
{
qRH
	TOL_CBUFFER___ Buffer;
	str::string FileName;
	xdhcbk::args Args;
	xdhcbk::retriever__ Retriever;
qRB
	Args.Init();
	xdhcbk::Split( str::string( Proxy.GetResult( Id, Buffer ) ), Args );

	Retriever.Init( Args );

	FileName.Init();

	if ( Retriever.Availability() != strmrg::aNone )
		Retriever.GetString( FileName );

	if ( FileName.Amount() != 0 )
		Proxy.SetContent( UserProjectId, FileName );
qRR
qRT
qRE
}

void xdhdws::login::GetContent(
	proxy__ &Proxy,
	xml::writer_ &Writer)
{
	// Rien  fournir.
}

static frdbse::backend_type__ GetBackendType_( proxy__ &Proxy )
{
	frdbse::backend_type__ BackendType = frdbse::bt_Undefined;
qRH
	str::string Value;
qRB
	Value.Init();
	BackendType = frdbse::GetBackendType( Proxy.GetContent( login::BackendTypeId, Value ) );
qRR
qRT
qRE
	return BackendType;
}

void xdhdws::login::GetContext(
	proxy__ &Proxy,
	xml::writer_ &Writer )
{
	Writer.PushTag( "BackendType" );

	Writer.PutValue( frdbse::GetLabel( GetBackendType_( Proxy ) ) );

	Writer.PopTag();
}

frdbse::backend_type__ xdhdws::login::GetBackendFeatures(
	proxy__ &Proxy,
	str::string_ &Feature )
{
	frdbse::backend_type__ Type = frdbse::bt_Undefined;
qRH
	TOL_CBUFFER___ Buffer;
qRB
	switch ( Type = GetBackendType_( Proxy ) ) {
	case frdbse::btDaemon:
		Feature.Append( Proxy.GetContent( DaemonBackendId, Buffer ) );
		break;
	case frdbse::btEmbedded:
		Feature.Append( Proxy.GetContent( EmbeddedBackendId, Buffer ) );
		break;
	case frdbse::btPredefined:
		Feature.Append( Proxy.GetContent( PredefinedBackendId, Buffer ) );
		break;
	default:
		qRFwk();
		break;
	}
qRR
qRT
qRE
	return Type;
}

void xdhdws::login::DisplaySelectedEmbeddedBackendFileName(
	proxy__ &Proxy,
	const char *Id )
{
qRH
	TOL_CBUFFER___ Buffer;
	str::string FileName;
	xdhcbk::args Args;
	xdhcbk::retriever__ Retriever;
qRB
	Args.Init();
	xdhcbk::Split( str::string( Proxy.GetResult( Id, Buffer ) ), Args );

	Retriever.Init( Args );

	FileName.Init();

	if ( Retriever.Availability() != strmrg::aNone )
		Retriever.GetString( FileName );

	if ( FileName.Amount() != 0 )
		Proxy.SetContent( EmbeddedBackendId, FileName );
qRR
qRT
qRE
}


