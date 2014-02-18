/*
	'xulfsv' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'xulfsv' header file ('xulfsv.h').
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



//	$Id: xulfsv.cpp,v 1.3 2013/06/11 10:32:27 csimon Exp $

#define XULFSV__COMPILATION

#include "xulfsv.h"

class xulfsvtutor
: public ttr_tutor
{
public:
	xulfsvtutor( void )
	: ttr_tutor( XULFSV_NAME )
	{
#ifdef XULFSV_DBG
		Version = XULFSV_VERSION "\b\bD $";
#else
		Version = XULFSV_VERSION;
#endif
		Owner = XULFSV_OWNER;
		Date = "$Date: 2013/06/11 10:32:27 $";
	}
	virtual ~xulfsvtutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

#include "xulftk.h"
#include "xulfrg.h"

using namespace xulfsv;

using xulftk::trunk___;

void xulfsv::_session_view_core__::_Refresh( xml::writer_ & )
{
}

static void Register_(
	trunk___ &Trunk,
	xulwdg::event_handler__<xulftk::trunk___> &EventHandler,
	const char *Id )
{
	EventHandler.Init( Trunk );
	nsxpcm::AttachEventHandler( Trunk.UI().SessionForm().Document(), Id, EventHandler );
}

#undef R

#define A( name ) Register_( Trunk, EventHandlers.name, "eh" #name )

#undef R

#define R( name, suffix, prefix ) Widgets.name##suffix.Init( Trunk, Trunk.UI().SessionForm().Window(), #prefix #name );

static void Register_(
	trunk___ &Trunk,
	widgets__ &Widgets )
{
}

void xulfsv::_session_view_core__::Attach( nsIDOMWindow *Window )
{
	Register_( Trunk(), Trunk().UI().SessionView().Widgets );
}


/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class xulfsvpersonnalization
: public xulfsvtutor
{
public:
	xulfsvpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~xulfsvpersonnalization( void )
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

static xulfsvpersonnalization Tutor;

ttr_tutor &XULFSVTutor = Tutor;
