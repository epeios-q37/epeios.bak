/*
	'fblcmd' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'fblcmd' header file ('fblcmd.h').
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



//	$Id: fblcmd.cpp,v 1.4 2013/04/14 14:37:21 csimon Exp $

#define FBLCMD__COMPILATION

#include "fblcmd.h"

class fblcmdtutor
: public ttr_tutor
{
public:
	fblcmdtutor( void )
	: ttr_tutor( FBLCMD_NAME )
	{
#ifdef FBLCMD_DBG
		Version = FBLCMD_VERSION "\b\bD $";
#else
		Version = FBLCMD_VERSION;
#endif
		Owner = FBLCMD_OWNER;
		Date = "$Date: 2013/04/14 14:37:21 $";
	}
	virtual ~fblcmdtutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace fblcmd;

const char *fblcmd::CommandsNames[c_amount] = {
	"_ThrowUError$1",
	"_ThrowIError$1",
	"_TestNotification$1",
	"_GetNewObject$1",
	"_GetType$1",
//	"_GetRawMessages$1",
	"_GetCommand$1",
	"_GetCommands$1",
	"_GetTypeAndCommands$1",
	"_RemoveObject$1",
	"_About$3",
	"_Disconnect$1",
	"_GetTypesIDAndPrefixAndName$1",
	"_GetCommandsIDAndName$1",
	"_GetParameters$1",
	"_GetLanguage$1",
	"_SetLanguage$1"
};

static const cast__ ThrowUError[] = {
	cEnd,
	cEnd };
static const cast__ ThrowIError[] = {
	cEnd,
	cEnd };
static const cast__ TestNotification[] = {
		cString,	// Un message devant tre retourn comme notification.
	cEnd,
	cEnd };
static const cast__ GetNewObject[] = {
		cId16,		// The type of the new object.
	cEnd,
		cObject,	// The asked object.
	cEnd };
static const cast__ GetType[] = {
		cString,	// The name of the type.
	cEnd,
		cId16,		// The type asked.
	cEnd };
static const cast__ GetCommand[] = {
		cId16,		// The object type.
		cString,	// The command name.
		cId8s,	// The casts of the command.
	cEnd,
		cId16,	// The asked command.
	cEnd };
static const cast__ GetCommands[] = {
		cId16,		// The object type.
		cCommandsDetails,	// The wanted commands caracteristics.
	cEnd,
		cId16s,		// The asked commands.
	cEnd };
static const cast__ GetTypeAndCommands[] = {
		cString,	// The name of the type.
		cCommandsDetails,		// The wanted commands caracteristics.
	cEnd,
		cId16,		// The asked type.
		cId16s,		// The asked commands.
	cEnd };
static const cast__ RemoveObject[] = {
		cObject,	// The object to remove.
	cEnd,
	cEnd };
static const cast__ About[] = {
	cEnd,
		cString,	// Version du protocol.
		cString,	// libell de la cible.
		cString,	// Version de l'API.
		cString,	// Informations tendues au sujet du 'backend'.
		cString,	// Informations de copyright sur le 'backend'.
		cString,	// Informations au sujet du 'software'.
	cEnd };
static const cast__ Disconnect[] = {
	cEnd,
	cEnd };
static const cast__ GetTypesIDAndPrefixAndName[] = {
	cEnd,
		cXItem16s,	// The types ID, prefix and name.
	cEnd };
static const cast__ GetCommandsIDAndName[] = {
		cId16,		// Type of objects from which you want the commands name.
	cEnd,
		cItem16s,	// The ids and names of the commands.
	cEnd };
static const cast__ GetParameters[] = {
		cId16,		// Type of objects from which you want the parameters.
		cId16,	// Command of objects from which you want the parameters.
	cEnd,
		cId8s,	// The parameters of the commands.
	cEnd };
static const cast__ GetLanguage[] = {
	cEnd,
		cString,	// Language label.
	cEnd };
static const cast__ SetLanguage[] = {
		cString,	// Language label.
	cEnd,
	cEnd };


const cast__ *fblcmd::CommandsParameters[c_amount] = {
	ThrowUError,
	ThrowIError,
	TestNotification,
	GetNewObject,
	GetType,
//	GetRawMessages,
	GetCommand,
	GetCommands,
	GetTypeAndCommands,
	RemoveObject,
	About,
	Disconnect,
	GetTypesIDAndPrefixAndName,
	GetCommandsIDAndName,
	GetParameters,
	GetLanguage,
	SetLanguage
};


/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class fblcmdpersonnalization
: public fblcmdtutor
{
public:
	fblcmdpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~fblcmdpersonnalization( void )
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

static fblcmdpersonnalization Tutor;

ttr_tutor &FBLCMDTutor = Tutor;
