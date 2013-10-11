/*
	'sclfrntnd' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'sclfrntnd' header file ('sclfrntnd.h').
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



//	$Id: xxx.cpp,v 1.9 2012/11/14 16:06:23 csimon Exp $

#define SCLFRNTND__COMPILATION

#include "sclfrntnd.h"

class sclfrntndtutor
: public ttr_tutor
{
public:
	sclfrntndtutor( void )
	: ttr_tutor( SCLFRNTND_NAME )
	{
#ifdef SCLFRNTND_DBG
		Version = SCLFRNTND_VERSION "\b\bD $";
#else
		Version = SCLFRNTND_VERSION;
#endif
		Owner = SCLFRNTND_OWNER;
		Date = "$Date: 2012/11/14 16:06:23 $";
	}
	virtual ~sclfrntndtutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace sclfrntnd;

static const lcl::meaning_ &GetMeaning_(
	const char *Message,
	lcl::meaning &Meaning )
{
ERRProlog
	str::string RefinedMessage;
ERRBegin
	RefinedMessage.Init( "FRD_" );
	RefinedMessage.Append( Message );

	Meaning.SetValue( RefinedMessage );
ERRErr
ERREnd
ERREpilog
	return Meaning;
}

void sclfrntnd::Report(
	frdkrn::kernel___ &Kernel,
	const char *Message )
{
ERRProlog
	lcl::meaning Meaning;
ERRBegin
	Meaning.Init();

	Kernel.Report( GetMeaning_( Message, Meaning ) );
ERRErr
ERREnd
ERREpilog
}

void sclfrntnd::Notify(
	frdkrn::kernel___ &Kernel,
	const char *Message )
{
ERRProlog
	lcl::meaning Meaning;
ERRBegin
	Meaning.Init();

	Kernel.Notify( GetMeaning_( Message, Meaning ) );
ERRErr
ERREnd
ERREpilog
}


/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class sclfrntndpersonnalization
: public sclfrntndtutor
{
public:
	sclfrntndpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~sclfrntndpersonnalization( void )
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

static sclfrntndpersonnalization Tutor;

ttr_tutor &SCLFRNTNDTutor = Tutor;
