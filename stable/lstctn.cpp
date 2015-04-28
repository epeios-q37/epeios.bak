/*
	'lstctn' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Requires the 'lstctn' header file ('lstctn.h').
	Copyright (C) 2004 Claude SIMON (http://zeusw.org/intl/contact.html).

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

#define LSTCTN__COMPILATION

#include "lstctn.h"

using namespace lstctn;

void lstctn::hook_filenames___::Init(
	const fnm::name___ &Path,
	const fnm::name___ &Basename )
{
ERRProlog
	fnm::name___ Container, List;
ERRBegin
	Container.Init();
	fnm::BuildPath( Path, Basename, "c", Container );

	List.Init();
	fnm::BuildPath( Path, Basename, "l", List );

	this->Container.Init( NULL, Container );
	this->List.Init( NULL, List );
ERRErr
ERREnd
ERREpilog
}
