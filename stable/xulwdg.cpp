/*
	'xulwdg' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'xulwdg' header file ('xulwdg.h').
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



//	$Id: xulwdg.cpp,v 1.1 2010/10/26 14:59:53 csimon Exp $

#define XULWDG__COMPILATION

#include "xulwdg.h"

class xulwdgtutor
: public ttr_tutor
{
public:
	xulwdgtutor( void )
	: ttr_tutor( XULWDG_NAME )
	{
#ifdef XULWDG_DBG
		Version = XULWDG_VERSION "\b\bD $";
#else
		Version = XULWDG_VERSION;
#endif
		Owner = XULWDG_OWNER;
		Date = "$Date: 2010/10/26 14:59:53 $";
	}
	virtual ~xulwdgtutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

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

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class xulwdgpersonnalization
: public xulwdgtutor
{
public:
	xulwdgpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~xulwdgpersonnalization( void )
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

static xulwdgpersonnalization Tutor;

ttr_tutor &XULWDGTutor = Tutor;
