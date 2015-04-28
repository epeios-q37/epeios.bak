/*
	'xulfkl' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'xulfkl' header file ('xulfkl.h').
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
#define XULFKL__COMPILATION

#include "xulfkl.h"

using namespace xulfkl;

#define CASE( n )\
	case m##n:\
	return XULFKL_NAME "_" #n;\
	break


const char *xulfkl::GetLabel( xulfkl::message__ MessageId )
{

#if XULFKL_M_AMOUNT != 2
#	error "'message__' changed !"
#endif

	const char *Message = NULL;

	switch ( MessageId ) {
	CASE( SelectProjectFile );
	CASE( ExitConfirmation );
	default:
		ERRPrm();
		break;
	}

	return Message;
}

