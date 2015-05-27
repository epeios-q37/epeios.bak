/*
	Copyright (C) 2000-2015 Claude SIMON (http://q37.info/contact/).

	This file is part of the Epeios framework.

	The Epeios framework is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

	The Epeios framework is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with the Epeios framework.  If not, see <http://www.gnu.org/licenses/>
*/

#define BKDCMD__COMPILATION

#include "bkdcmd.h"

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
