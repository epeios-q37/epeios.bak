/*
	Header for the 'ttr' library by Claude SIMON (http://zeusw.org/intl/contact.html)
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

//	$Id: ttr.h,v 1.15 2013/03/26 15:57:06 csimon Exp $

#ifndef TTR__INC
#define TTR__INC

#define TTR_NAME		"TTR"

#define	TTR_VERSION	"$Revision: 1.15 $"

#define TTR_OWNER		"Claude SIMON (http://zeusw.org/intl/contact.html)"

#include "ttr.h"

extern class ttr_tutor &TTRTutor;

#if defined( E_DEBUG ) && !defined( TTR_NODBG )
#define TTR_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.15 $
//C Claude SIMON (http://zeusw.org/intl/contact.html)
//R $Date: 2013/03/26 15:57:06 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D TuToR 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include <stdlib.h>

#define	TTR_ADV_SIZE_MAX	200

namespace txf {
	class text_oflow__;	// Pre-decleration, to avoid inclusion of 'tfx.h".
}

//c Chief off all tutors.
class ttr_chief
{
private:
	// the first tutor
	class ttr_tutor *First_;
	// add and sort 'Tutor'.
	void Sort_( ttr_tutor *Tutor );
public:
	//f Initialization
	void Init( void )
	{
		First_ = NULL;
	}
	// shifted function.
	//f Add a tutor.
	inline void Add( class ttr_tutor *Tutor );
	//f Give the first tutor.
	ttr_tutor *First( void )
	{
		return First_;
	}
	//f Print advertise to the console.
	void Advertise( txf::text_oflow__ &Flow );	 // Would be modified.
};

extern ttr_chief TTR;

//c A tutor for one library.
class ttr_tutor
{
	friend class ttr_chief;
private:
	// Pour le chanage des diffrents tuteurs.
	ttr_tutor *Next_;
	// Chaine pour afficher la publicite
	static char Adv_[TTR_ADV_SIZE_MAX + 1];
	// True if the chief is ready.
	static char ChiefReady_;
protected:
	virtual void TTRMore( class txf::text_oflow__ &OFlow ) {}
public:
	//o Name of the library.
	const char *Name;
	//o Version of the library.
	const char *Version;
	//o Owner of the library.
	const char *Owner;
	//o Compilation date & time..
	const char *Date;
	ttr_tutor( const char *Name )
	{
		Next_ = NULL;
		Owner = Version = Date = NULL;

		this->Name = Name;

		if ( !ChiefReady_ )
		{
			TTR.Init();
			ChiefReady_ = true;
		}

		TTR.Add( this );
	}
	//f Return the next tutor.
	ttr_tutor *Next( void )
	{
		return Next_;
	}
	//f Print advertising of the library.
	void Advertise( txf::text_oflow__ &Flow );	// Should be modified.
};

inline void ttr_chief::Add( class ttr_tutor *Tutor )
{
	if ( First_ )
		Sort_( Tutor );
	else
		First_ = Tutor;
}

extern class ttr_tutor &TTRTutor;


/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
