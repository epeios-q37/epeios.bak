/*
	'mmi' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Requires the 'mmi' header file ('mmi.h').
	Copyright (C) 2000-2001, 2004 Claude SIMON (http://zeusw.org/intl/contact.html).

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



//	$Id: mmi.cpp,v 1.59 2013/03/15 14:11:53 csimon Exp $

#define MMI__COMPILATION

#include "mmi.h"

class mmitutor
: public ttr_tutor
{
public:
	mmitutor( void )
	: ttr_tutor( MMI_NAME )
	{
#ifdef MMI_DBG
		Version = MMI_VERSION "\b\bD $";
#else
		Version = MMI_VERSION;
#endif
		Owner = MMI_OWNER;
		Date = "$Date: 2013/03/15 14:11:53 $";
	}
	virtual ~mmitutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

#define capacite				mdr::size__
#define multimemoire_indexee	indexed_multimemory_

namespace mmi {
	void multimemoire_indexee::AllouerMoins_(
		capacite CapaciteCourante,
		capacite CapaciteDemandee,
		aem::mode__ Mode )
	{
		while ( CapaciteCourante-- > CapaciteDemandee )
			Multimemoire.Free( Descripteurs.Get( CapaciteCourante ).Descripteur );

		Descripteurs.Allocate( CapaciteDemandee, Mode );
	}

	void multimemoire_indexee::RemoveWithoutReallocating(
		mdr::row__ Position,
		mdr::size__ ActualCapacity,
		mdr::size__ Amount )
	{
		mdr::size__ Counter = Amount;

		while( Counter-- )
			Multimemoire.Free( Descripteurs.Get( *Position + Counter ).Descripteur );

		Descripteurs.Store( Descripteurs, ActualCapacity - *Position - Amount, *Position, *Position + Amount );

		Initialize_( ActualCapacity - Amount, ActualCapacity );
	}

}

	/* Although in theory this class is inaccessible to the different modules,
	it is necessary to personalize it, or certain compiler would not work properly */
	class mmipersonnalization
	: public mmitutor
	{
	public:
		mmipersonnalization( void )
		{
			/* place here the actions concerning this library
			to be realized at the launching of the application  */
		}
		~mmipersonnalization( void )
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

static mmipersonnalization Tutor;

ttr_tutor &MMITutor = Tutor;
