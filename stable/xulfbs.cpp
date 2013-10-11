/*
	'xulfbs' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'xulfbs' header file ('xulfbs.h').
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



//	$Id: xulfbs.cpp,v 1.6 2013/04/09 17:57:26 csimon Exp $

#define XULFBS__COMPILATION

#include "xulfbs.h"

class xulfbstutor
: public ttr_tutor
{
public:
	xulfbstutor( void )
	: ttr_tutor( XULFBS_NAME )
	{
#ifdef XULFBS_DBG
		Version = XULFBS_VERSION "\b\bD $";
#else
		Version = XULFBS_VERSION;
#endif
		Owner = XULFBS_OWNER;
		Date = "$Date: 2013/04/09 17:57:26 $";
	}
	virtual ~xulfbstutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

#include "xulftk.h"

using namespace xulfbs;

static void _RetrieveErrAndReport( xulftk::trunk___ &Trunk )
{
	err::buffer__ Buffer;

	Trunk.UI().LogAndPrompt( err::Message( Buffer ) );
}
 

void xulfbs::_Report(
	xulftk::trunk___ &Trunk,
	const char *Message )
{
	if ( Message != NULL )
		Trunk.UI().LogAndPrompt( Message );
	else {	// L'erreur a été détectée dans un contexte où les informations ka concernant ne sont pas disponibles, d'où traitement ici.
		switch (  ERRType ) {
		case err::t_Exit:
			ERRFwk();
			break;
		case err::t_Abort:
			break;
		case err::t_Return:
			ERRFwk();
			break;
		default:
			_RetrieveErrAndReport( Trunk );
			break;
		}

		ERRRst();
	}
}

static void UpdateAccessibility_(
	const str::string_ &XMLDigest,
	nsIDOMElement *Broadcasters,
	nsIDOMDocument *Document,
	const str::string_ &XSLFileName )
{
	nsxpcm::RemoveChildren( Broadcasters );

	nsIDOMDocumentFragment *Fragment = nsxpcm::XSLTransformByFileName( XMLDigest, XSLFileName, Document, nsxpcm::xslt_parameters() );
	
	nsxpcm::AppendChild( Broadcasters, Fragment );

	nsxpcm::RefreshObservers( Document );
}


void xulfbs::_wp_core__::Refresh( void )
{
ERRProlog
	str::string XMLDigest;
	const char *XSLFileNameAffix = NULL;
	str::string XSLFileName;
	flx::E_STRING_OFLOW___ Flow;
	txf::text_oflow__ TFlow;
	xml::writer Writer;
ERRBegin
	XSLFileName.Init();
	XMLDigest.Init();
	Flow.Init( XMLDigest );
	TFlow.Init( Flow );
	Writer.Init( TFlow, xml::oIndent, xml::e_Default );

	if ( _Callback == NULL )
		ERRFwk();

	if ( ( XSLFileNameAffix = _Callback->Refresh( Writer ) ) == NULL )
		ERRReturn;

	XSLFileName.Init();

	_Trunk().BuildXSLDigestFileName( XSLFileNameAffix, XSLFileName );

	Writer.reset();
	TFlow.reset();
	Flow.reset();

	UpdateAccessibility_( XMLDigest, _Broadcasterset, _Document, XSLFileName );

	nsxpcm::Log( XMLDigest );
	nsxpcm::Log( XSLFileName );
ERRErr
ERREnd
ERREpilog
}




/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class xulfbspersonnalization
: public xulfbstutor
{
public:
	xulfbspersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~xulfbspersonnalization( void )
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

static xulfbspersonnalization Tutor;

ttr_tutor &XULFBSTutor = Tutor;
