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



//	$Id: xulfmn.cpp,v 1.33 2013/06/11 10:32:26 csimon Exp $

#define XULFMN__COMPILATION

#include "xulfmn.h"

class xulfmntutor
: public ttr_tutor
{
public:
	xulfmntutor( void )
	: ttr_tutor( XULFMN_NAME )
	{
#ifdef XULFMN_DBG
		Version = XULFMN_VERSION "\b\bD $";
#else
		Version = XULFMN_VERSION;
#endif
		Owner = XULFMN_OWNER;
		Date = "$Date: 2013/06/11 10:32:26 $";
	}
	virtual ~xulfmntutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

#include "xulftk.h"

#include "fnm.h"

using xulftk::trunk___;

using namespace xulfmn;

const char *xulfmn::main__::XULFBSGetDigest( xml::writer_ &Digest )
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
	nsIDOMDocument *Document )
{
ERRProlog
	nsIDOMDocumentFragment *Fragment;
	fnm::name___ FileName;
	str::string Buffer;
ERRBegin
	nsxpcm::RemoveChildren( Node );

	FileName.Init();
	Buffer.Init();
	Fragment = nsxpcm::XSLTransformByFileName( XML, fnm::BuildFileName( DefaultXSLRootPath, XSLFileNameAffix, ".xsl", FileName ).UTF8( Buffer ), Document, nsxpcm::xslt_parameters() );

	nsxpcm::AppendChild( Node, Fragment );
ERRErr
ERREnd
ERREpilog
}


static void FillPredefinedProjectsMenu_( trunk___ &Trunk )
{
ERRProlog
	str::string PredefinedProjects;
ERRBegin
	PredefinedProjects.Init();

	GetPredefinedProjects_( Trunk.Kernel().Registry(), Trunk.Kernel().Locale(), Trunk.Kernel().Language(), PredefinedProjects );

	Trunk.UI().LogQuietly( PredefinedProjects );

	FillWidget_( PredefinedProjects, Trunk.DefaultXSLRootPath(), Trunk.UI().Main().Widgets.mnuPredefinedProject, "PredefinedProjectsMenu", Trunk.UI().Main().Document() );
	FillWidget_( PredefinedProjects, Trunk.DefaultXSLRootPath(), Trunk.UI().Main().Widgets.mnlPredefinedProjectList, "PredefinedProjectsMenuList", Trunk.UI().Main().Document() );

	nsxpcm::SetSelectedItem( Trunk.UI().Main().Widgets.mnlPredefinedProjectList );

	// La sélection de l'item de 'Trunk.UI().Main().Widgets.mnuPredefinedProject'  est réalisé directement en XSL.
ERRErr
ERREnd
ERREpilog
}

#define A( name )\
	Widgets.name.Attach( nsxpcm::supports__( Trunk.UI().Main().Window(), #name ) );


void Register_(
	widgets__ &Widgets,
	trunk___ &Trunk )
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

	FillPredefinedProjectsMenu_( Trunk );	// N'a à être fait qu'une seule fois.
}

#undef A

#define A( name )\
	nsxpcm::AttachEventHandler( Document, "eh" #name, EventHandlers.name );

static void Register_(
	xulfeh::event_handlers__::m__ &EventHandlers,
	nsIDOMNode *Document )
{
	A( About );
	A( Apply );
	A( CloseProject );
	A( Debug );
	A( Exit );
	A( NewProject );
	A( ProjectTypeSelection );
	A( UserProject );
	A( UserProjectSelection );
	A( WebSite );
}

void xulfmn::main__::Register( nsIDOMWindow *Window )
{
	Register_( Widgets, Trunk() );
	Register_( Trunk().UI().EventHandlers.M, Trunk().UI().Main().Document() );

	Trunk().UI().EventHandlers.M.Exit.Add( Window, nsxpcm::efClose ); // Parce que 'xex:onclose=...' est inopérant sur 'window'.
}


/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class xulfmnpersonnalization
: public xulfmntutor
{
public:
	xulfmnpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~xulfmnpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the ending of the application  */
	}
};


/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

// 'static' by GNU C++.

static xulfmnpersonnalization Tutor;

ttr_tutor &XULFMNTutor = Tutor;
