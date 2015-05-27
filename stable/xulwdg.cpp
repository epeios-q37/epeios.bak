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

#define XULWDG__COMPILATION

#include "xulwdg.h"

using namespace xulwdg;

static void UpdateAccessibility_(
	const str::string_ &XMLDigest,
	nsIDOMElement *Broadcasters,
	nsIDOMWindow *Window,
	const str::string_ &XSLFileName )
{
	nsxpcm::RemoveChildren( Broadcasters );

	nsIDOMDocumentFragment *Fragment = nsxpcm::XSLTransformByFileName( XMLDigest, XSLFileName, Window, nsxpcm::xslt_parameters() );
	
	nsxpcm::AppendChild( Broadcasters, Fragment );

	nsxpcm::RefreshObservers( nsxpcm::GetElement( Window ) );
}


void xulwdg::_wp_core__::Refresh( void )
{
ERRProlog
	str::string XMLDigest;
	str::string XSLFileName;
	flx::E_STRING_OFLOW___ Flow;
	txf::text_oflow__ TFlow;
	xml::writer Writer;
	nsIDOMWindow *Window = NULL;
	nsIDOMElement *Broadcasters = NULL;
ERRBegin
	XSLFileName.Init();
	XMLDigest.Init();
	Flow.Init( XMLDigest );
	TFlow.Init( Flow );
	Writer.Init( TFlow, xml::oIndent, xml::e_Default );
	XULWDGRefresh( Window, Broadcasters, Writer, XSLFileName );

	Writer.reset();
	TFlow.reset();
	Flow.reset();

	UpdateAccessibility_( XMLDigest, Broadcasters, Window, XSLFileName );

	nsxpcm::Log( XMLDigest );
	nsxpcm::Log( XSLFileName );
ERRErr
ERREnd
ERREpilog
}

