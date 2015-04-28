/*
	'xulfmn' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'xulfmn' header file ('xulfmn.h').
	Copyright (C) 2004 Claude SIMON.

	This file is part of the Epeios (http://zeusw.org/epeios/) project.

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
	of the License, or (at your option) any later version.
 
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, go to http://www.fsf.org/
	or write to the:
  
         	         Free Software Foundation, Inc.,
           59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
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
ERRProlog
	flx::E_STRING_OFLOW___ OFlow;
	txf::text_oflow__ TFlow;
	xml::writer Writer;
	str::string Default;
ERRBegin
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
ERRErr
ERREnd
ERREpilog
}

static void FillWidget_(
	const str::string_ &XML,
	const char *DefaultXSLRootPath,
	nsIDOMNode *Node,
	const char *XSLFileNameAffix,
	nsIDOMWindow *Window )
{
ERRProlog
	nsIDOMDocumentFragment *Fragment;
	fnm::name___ FileName;
	str::string Buffer;
ERRBegin
	nsxpcm::RemoveChildren( Node );

	FileName.Init();
	Buffer.Init();
	Fragment = nsxpcm::XSLTransformByFileName( XML, fnm::BuildFileName( DefaultXSLRootPath, XSLFileNameAffix, ".xsl", FileName ).UTF8( Buffer ), Window, nsxpcm::xslt_parameters() );

	nsxpcm::AppendChild( Node, Fragment );
ERRErr
ERREnd
ERREpilog
}


static void FillPredefinedProjectsMenu_(
	trunk___ &Trunk,
	nsIDOMWindow *Window )
{
ERRProlog
	str::string PredefinedProjects;
ERRBegin
	PredefinedProjects.Init();

	GetPredefinedProjects_( Trunk.Kernel().Registry(), Trunk.Kernel().Locale(), Trunk.Kernel().Language(), PredefinedProjects );

	Trunk.UI().LogQuietly( PredefinedProjects );

	FillWidget_( PredefinedProjects, Trunk.DefaultXSLRootPath(), Trunk.UI().Main().Widgets.mnuPredefinedProject, "PredefinedProjectsMenu", Window );
	FillWidget_( PredefinedProjects, Trunk.DefaultXSLRootPath(), Trunk.UI().Main().Widgets.mnlPredefinedProjectList, "PredefinedProjectsMenuList", Window );

	nsxpcm::SetSelectedItem( Trunk.UI().Main().Widgets.mnlPredefinedProjectList );

	// La slection de l'item de 'Trunk.UI().Main().Widgets.mnuPredefinedProject'  est ralis directement en XSL.
ERRErr
ERREnd
ERREpilog
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

