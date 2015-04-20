/*
	'bkdcmd' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'bkdcmd' header file ('bkdcmd.h').
	Copyright (C) 2001-2004 Claude SIMON.

	This file is part of the Epeios (http://zeusw.org/epeios/) project.

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 3
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



//	$Id: bkdcmd.cpp,v 1.8 2011/06/11 18:16:00 csimon Exp $

#define BKDCMD__COMPILATION

#include "bkdcmd.h"

class bkdcmdtutor
: public ttr_tutor
{
public:
	bkdcmdtutor( void )
	: ttr_tutor( BKDCMD_NAME )
	{
#ifdef BKDCMD_DBG
		Version = BKDCMD_VERSION "\b\bD $";
#else
		Version = BKDCMD_VERSION;
#endif
		Owner = BKDCMD_OWNER;
		Date = "$Date: 2011/06/11 18:16:00 $";
	}
	virtual ~bkdcmdtutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace bkdcmd;

const char *bkdcmd::CommandsNames[bkdcmd::c_amount] = {
	"_ThrowUError$1",
	"_ThrowIError$1",
	"_ThrowUserDefinedError",
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

static const cast ThrowUError[] = {
	cEnd,
	cEnd };
static const cast ThrowIError[] = {
	cEnd,
	cEnd };
static const cast ThrowUserDefinedError[] = {
		cString,	// Un message devant tre retourn comme message d'erreur utilisateur.
	cEnd,
	cEnd };
static const cast GetNewObject[] = {
		cId16,		// The type of the new object.
	cEnd,
		cObject,	// The asked object.
	cEnd };
static const cast GetType[] = {
		cString,	// The name of the type.
	cEnd,
		cId16,		// The type asked.
	cEnd };
static const cast GetCommand[] = {
		cId16,		// The object type.
		cString,	// The command name.
		cIds8,	// The casts of the command.
	cEnd,
		cId16,	// The asked command.
	cEnd };
static const cast GetCommands[] = {
		cId16,		// The object type.
		cCommandsDetails,	// The wanted commands caracteristics.
	cEnd,
		cIds16,		// The asked commands.
	cEnd };
static const cast GetTypeAndCommands[] = {
		cString,	// The name of the type.
		cCommandsDetails,		// The wanted commands caracteristics.
	cEnd,
		cId16,		// The asked type.
		cIds16,		// The asked commands.
	cEnd };
static const cast RemoveObject[] = {
		cObject,	// The object to remove.
	cEnd,
	cEnd };
static const cast About[] = {
	cEnd,
		cString,	// Version du protocol.
		cString,	// libell de la cible.
		cString,	// Version de l'API.
		cString,	// Informations au sujet du 'backend'.
		cString,	// Informations au sujet du 'publisher'.
	cEnd };
static const cast Disconnect[] = {
	cEnd,
	cEnd };
static const cast GetTypesIDAndPrefixAndName[] = {
	cEnd,
		cXItems16,	// The types ID, prefix and name.
	cEnd };
static const cast GetCommandsIDAndName[] = {
		cId16,		// Type of objects from which you want the commands name.
	cEnd,
		cItems16,	// The ids and names of the commands.
	cEnd };
static const cast GetParameters[] = {
		cId16,		// Type of objects from which you want the parameters.
		cId16,	// Command of objects from which you want the parameters.
	cEnd,
		cIds8,	// The parameters of the commands.
	cEnd };
static const cast GetLanguage[] = {
	cEnd,
		cString,	// Language label.
	cEnd };
static const cast SetLanguage[] = {
		cString,	// Language label.
	cEnd,
	cEnd };


const cast *bkdcmd::CommandsParameters[bkdcmd::c_amount] = {
	ThrowUError,
	ThrowIError,
	ThrowUserDefinedError,
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

class bkdcmdpersonnalization
: public bkdcmdtutor
{
public:
	bkdcmdpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~bkdcmdpersonnalization( void )
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

static bkdcmdpersonnalization Tutor;

ttr_tutor &BKDCMDTutor = Tutor;
