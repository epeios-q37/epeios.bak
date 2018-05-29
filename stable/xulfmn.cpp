/*
	Copyright (C) 1999 Claude SIMON (http://q37.info/contact/).

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

#define XULFMN__COMPILATION

#include "xulfmn.h"


#include "xulftk.h"

#include "fnm.h"

using xulftk::trunk___;

using namespace xulfmn;

void xulfmn::_main_core__::_Refresh( xml::writer_ &Digest )
{
	xulfbs::_PushDigestTag( Digest );
	Digest.PushTag( "ProjectInProgress" );
	Digest.PutValue( Trunk().Kernel().IsProjectInProgress() ? "true" : "false" );
	Digest.PopTag();
	Digest.PopTag();
}


/* Registrations */

static void GetPredefinedProjects_(
	const frdkrn::registry_ &Registry,
	const lcl::locale_ &Locale,
	const char *Language,
	str::string_ &PredefinedProjects )
{
qRH
	flx::E_STRING_OFLOW___ OFlow;
	txf::text_oflow__ TFlow;
	xml::writer Writer;
	str::string Default;
qRB
	OFlow.Init( PredefinedProjects );
	TFlow.Init( OFlow );
	Writer.Init( TFlow, xml::oIndent, xml::e_Default );

	Writer.PushTag( "PredefinedProjects" );

	Default.Init();
	Registry.GetValue( frdrgy::DefaultPredefinedProject, Default );

	if ( Default.Amount() != 0 )
		Writer.PutAttribute( "Default", Default );

	frdkrn::GetPredefinedProjects( Registry, Locale, Language, Writer );

	Writer.PopTag();
qRR
qRT
qRE
}

static void FillWidget_(
	const str::string_ &XML,
	const char *DefaultXSLRootPath,
	nsIDOMNode *Node,
	const char *XSLFileNameAffix,
	nsIDOMWindow *Window )
{
qRH
	nsIDOMDocumentFragment *Fragment;
	fnm::name___ FileName;
	str::string Buffer;
qRB
	nsxpcm::RemoveChildren( Node );

	FileName.Init();
	Buffer.Init();
	Fragment = nsxpcm::XSLTransformByFileName( XML, fnm::BuildFileName( DefaultXSLRootPath, XSLFileNameAffix, ".xsl", FileName ).UTF8( Buffer ), Window, nsxpcm::xslt_parameters() );

	nsxpcm::AppendChild( Node, Fragment );
qRR
qRT
qRE
}


static void FillPredefinedProjectsMenu_(
	trunk___ &Trunk,
	nsIDOMWindow *Window )
{
qRH
	str::string PredefinedProjects;
qRB
	PredefinedProjects.Init();

	GetPredefinedProjects_( Trunk.Kernel().Registry(), Trunk.Kernel().Locale(), Trunk.Kernel().Language(), PredefinedProjects );

	Trunk.UI().LogQuietly( PredefinedProjects );

	FillWidget_( PredefinedProjects, Trunk.DefaultXSLRootPath(), Trunk.UI().Main().Widgets.mnuPredefinedProject, "PredefinedProjectsMenu", Window );
	FillWidget_( PredefinedProjects, Trunk.DefaultXSLRootPath(), Trunk.UI().Main().Widgets.mnlPredefinedProjectList, "PredefinedProjectsMenuList", Window );

	nsxpcm::SetSelectedItem( Trunk.UI().Main().Widgets.mnlPredefinedProjectList );

	// La slection de l'item de 'Trunk.UI().Main().Widgets.mnuPredefinedProject'  est ralis directement en XSL.
qRR
qRT
qRE
}

#define A( name )\
	name.Attach( nsxpcm::supports__( Window, #name ) );

void xulfmn::widgets__::Attach( nsIDOMWindow *Window )
{
	A( mnuPredefinedProject );
	A( dckMain );
	A( vewHome );
	A( dckProjectType );
	A( mnlPredefinedProjectList );
	A( txbUserProjectLocation );
	A( mnlProjectType );
	A( vewSessionForm );
	A( vewSessionView );
}

#undef A


void xulfmn::_main_core__::Attach( nsIDOMWindow *Window )
{
	Widgets.Attach( Window );

	Trunk().UI().EventHandlers.M.Attach( Window );

	FillPredefinedProjectsMenu_( Trunk(), Window );	// N'a  tre fait qu'une seule fois.

	Trunk().UI().EventHandlers.M.Exit.Add( Window, nsxpcm::efClose ); // Parce que 'xex:onclose=...' est inoprant sur 'window'.
}

