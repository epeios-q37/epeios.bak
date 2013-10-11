/* begin of 'entete.txt' template file V1.1 */
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

#include "ttr.h"

extern class ttr_tutor &SALJNITutor;

#if defined( E_DEBUG ) && !defined( SALJNI_NODBG )
#define SALJNI_DBG 
#endif

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
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
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
/* end of 'xxx.h' template file */