/* begin of 'entete.txt' template file V1.1 */
/* Best viewed with a tab size of 4 */
/*
	This file is part of the Epeios project. For all information
	concerning the Epeios project, this file and its conditions of use,
	consult the site: 

			http://www.epeios.org/
*/
/* end of 'entete.txt' template file */
/* begin of 'xxx.cpp' template file V2.6 */
/* 
	Pour plus de détails, consultez le fichier 'txmhch.h'.
	See the 'txmhch.h' file  for more details.
*/

#define TXMHCH__COMPILATION

#include "txmhch.h"

#ifdef TXMHCH__LINE
#line 12
#endif

class txmhchtutor
: public ttr_tutor
{
public:
	txmhchtutor( void )
	: ttr_tutor( TXMHCH_NAME )
	{
		Version = TXMHCH_VERSION;
		Owner = TXMHCH_OWNER;
		Date = "07/04/2000";
	}
	virtual ~txmhchtutor( void ){}
};

//  	Substitution is not applied to the rest.
/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class txmhchpersonnalization
: public txmhchtutor
{
public:
	txmhchpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~txmhchpersonnalization( void )
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

static txmhchpersonnalization Tutor;

ttr_tutor &TXMHCHTutor = Tutor;

/* end of 'xxx.cpp' template file */