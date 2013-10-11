/*
	'geckoo' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'geckoo' header file ('geckoo.h').
	Copyright (C) 20112004 Claude SIMON.

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



//	$Id: geckoo.cpp,v 1.3 2013/04/05 16:33:29 csimon Exp $

#define GECKOO__COMPILATION

#include "geckoo.h"

class geckootutor
: public ttr_tutor
{
public:
	geckootutor( void )
	: ttr_tutor( GECKOO_NAME )
	{
#ifdef GECKOO_DBG
		Version = GECKOO_VERSION "\b\bD $";
#else
		Version = GECKOO_VERSION;
#endif
		Owner = GECKOO_OWNER;
		Date = "$Date: 2013/04/05 16:33:29 $";
	}
	virtual ~geckootutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace geckoo;

const char *geckoo::shared_data__::Version = GECKOO_OVERLAPPING_VERSION;
bso::uint__ geckoo::shared_data__::Control = shared_data__::ControlComputing();

void geckoo::AddPseudoEventHandler(
	nsIDOMNode *Node,
	const char *PseudoEventName,
	pseudo_event_callback__ &Handler )
{
ERRProlog
	str::string Value;
	bso::pointer_buffer__ Buffer;
ERRBegin
	Value.Init( "handlePseudoEvent( this, '" );

	Value.Append( bso::Convert( &Handler, Buffer ) );

	Value.Append( "');" );

	nsxpcm::SetAttribute( Node, PseudoEventName, Value );
ERRErr
ERREnd
ERREpilog
}



/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class geckoopersonnalization
: public geckootutor
{
public:
	geckoopersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~geckoopersonnalization( void )
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

static geckoopersonnalization Tutor;

ttr_tutor &GECKOOTutor = Tutor;
