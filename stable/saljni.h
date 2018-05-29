/*
	Copyright (C) 1999 Claude SIMON (http://q37.info/contact/).

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
/* Best viewed with a tab size of 4 */
/*
	This file is part of the Epeios project. For all information
	concerning the Epeios project, this file and its conditions of use,
	consult the site: 

			http://www.epeios.org/
*/
/* end of 'entete.txt' template file */
/* begin of 'entete.h' template file V1.3 */
/*
Version:
	1.2.0 20/05/2000 12:56:37

Instigators:
	Claude SIMON

Authors:
	Claude SIMON

Contributors:
	/

Coordinator:
	Claude SIMON

Description:
	Stand ALone JNI (Java Native Interface)
*/

/* Begin of automatic documentation generation part. */

//V 1.2.0
//C Claude SIMON
//D Stand ALone JNI (Java Native Interface)
//R 20/05/2000 12:56:37

/* End of automatic documentation generation part. */

/* end of 'entete.h' template file */
/* begin of 'xxx.h' template file V2.6 */

#ifndef SALJNI__INC
#define SALJNI__INC

#define SALJNI_NAME		"SALJNI"

#define	SALJNI_VERSION		"1.2.0"	
#define SALJNI_OWNER		"The Epeios Project (www.epeios.org)"	

//  	Substitution is not applied to the rest when emancipated

#ifdef SALJNI__LINE
#line 14
#endif

#if defined( E_DEBUG ) && !defined( SALJNI_NODBG )
#define SALJNI_DBG 
#endif
/*$BEGIN$*/

#include "err.h"
#include "salcsm.h"

#define SALJNICP	SALCSMCP
#define SALJNICI	SALCSMCI
#define SALJNISP	SALCSMSP
#define SALJNISI	SALCSMSI

//c
class saljni__manager
: public salcsm__manager
{
public:
	//f Initialization.
	void Init( void )
	{
		// For standardization.
	}
	//f Process.
	void Process( void );
};



/*$END$*/
#endif
/* end of 'xxx.h' template file */