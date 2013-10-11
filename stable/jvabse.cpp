/*
	'jvabse' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'jvabse' header file ('jvabse.h').
	Copyright (C) 20122004 Claude SIMON.

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



//	$Id: jvabse.cpp,v 1.12 2013/05/05 16:04:30 csimon Exp $

#define JVABSE__COMPILATION

#include "jvabse.h"

class jvabsetutor
: public ttr_tutor
{
public:
	jvabsetutor( void )
	: ttr_tutor( JVABSE_NAME )
	{
#ifdef JVABSE_DBG
		Version = JVABSE_VERSION "\b\bD $";
#else
		Version = JVABSE_VERSION;
#endif
		Owner = JVABSE_OWNER;
		Date = "$Date: 2013/05/05 16:04:30 $";
	}
	virtual ~jvabsetutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace jvabse;

const str::string_ &jvabse::Convert(
	jstring JString,
	JNIEnv *Env,
	str::string_ &String )
{
ERRProlog
	const char *Buffer = NULL;
ERRBegin
	if ( ( Buffer = Env->GetStringUTFChars( JString, NULL ) ) == NULL )
		ERRLbr();

	String.Append( Buffer );
ERRErr
ERREnd
	if ( Buffer != NULL )
		Env->ReleaseStringUTFChars( JString, Buffer );
ERREpilog
	return String;
}

const char *jvabse::Convert(
	jstring JString,
	JNIEnv *Env,
	STR_BUFFER___ &Buffer )
{
ERRProlog
	str::string String;
ERRBegin
	String.Init();

	Convert( JString, Env, String );

	String.Convert( Buffer );
ERRErr
ERREnd
ERREpilog
	return Buffer;
}





/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class jvabsepersonnalization
: public jvabsetutor
{
public:
	jvabsepersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~jvabsepersonnalization( void )
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

static jvabsepersonnalization Tutor;

ttr_tutor &JVABSETutor = Tutor;
