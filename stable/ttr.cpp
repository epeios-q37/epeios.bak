/*
	'ttr' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Requires the 'ttr' header file ('ttr.h').
	Copyright (C) 2000-2004 Claude SIMON (http://zeusw.org/intl/contact.html).

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



//	$Id: ttr.cpp,v 1.14 2013/03/26 15:57:06 csimon Exp $

#define TTR__COMPILATION

#include "ttr.h"

class ttrtutor
: public ttr_tutor
{
public:
	ttrtutor( void )
	: ttr_tutor( TTR_NAME )
	{
#ifdef TTR_DBG
		Version = TTR_VERSION "\b\bD $";
#else
		Version = TTR_VERSION;
#endif
		Owner = TTR_OWNER;
		Date = "$Date: 2013/03/26 15:57:06 $";
	}
	virtual ~ttrtutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

#include "txf.h"

ttr_chief TTR;
char ttr_tutor::Adv_[TTR_ADV_SIZE_MAX+1];
char ttr_tutor::ChiefReady_ = false;

void ttr_chief::Advertise( txf::text_oflow__ &Flow )
{
	ttr_tutor *Current = First_;

	while( Current )
	{
		Current->Advertise( Flow );
		Current = Current->Next();
	}
}

void ttr_chief::Sort_( ttr_tutor *Tutor )
{
	ttr_tutor **Index = &First_;

	while ( *Index && ( strcmp( (*Index)->Name, Tutor->Name ) < 0 ) )
		Index = &((*Index)->Next_);

	Tutor->Next_ = *Index;

	*Index = Tutor;
}

	//f Print advertising of the library.
void ttr_tutor::Advertise( txf::text_oflow__ &Flow )	// Should be modified.
{
	char Format[30];

	if ( Version[0] != '$' )
		sprintf( Adv_, "%-7s V%-6s %s (c) %s\n",
			Name, Version, Date, Owner );
	else {
		sprintf( Format, "%s%i%s", "%-7s r%-7.",
			(bso::u8__)strlen( Version ) - 15,
							  	   "s %.10s (c) %s" );
		sprintf( Adv_, Format,
			Name, Version + 13, Date + 7, Owner );
	}
	

	Flow << Adv_ << txf::nl;

	TTRMore( Flow );
}

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */
class ttrpersonnalization
: public ttrtutor
{
public:
	ttrpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~ttrpersonnalization( void )
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

static ttrpersonnalization Tutor;

ttr_tutor &TTRTutor = Tutor;
