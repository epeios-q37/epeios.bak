/*
	Header for the 'cio' library by Claude SIMON (http://zeusw.org/intl/contact.html)
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

//	$Id: cio.h,v 1.48 2013/04/06 14:50:43 csimon Exp $

#ifndef CIO__INC
#define CIO__INC

#define CIO_NAME		"CIO"

#define	CIO_VERSION	"$Revision: 1.48 $"

#define CIO_OWNER		"Claude SIMON (http://zeusw.org/intl/contact.html)"

#include "ttr.h"

extern class ttr_tutor &CIOTutor;

#if defined( E_DEBUG ) && !defined( CIO_NODBG )
#define CIO_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.48 $
//C Claude SIMON (http://zeusw.org/intl/contact.html)
//R $Date: 2013/04/06 14:50:43 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D Console Input/Output 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

# include "err.h"
# include "iof.h"
# include "txf.h"
# include "cpe.h"
# include "flx.h"

# ifndef E_NO_CIO_AUTOMATIC_INITIALIZATION	// Utilise dans le cadre d'un service Windows.
# ifndef CPE_LIBRARY
#  define CIO__AUTOMATIC_INITIALIZATION 	// Si modifié, modifier 'WINTOL' en conséquence !
#  endif
# endif

namespace cio {

	extern iop::descriptor__ CInDescriptor, COutDescriptor, CErrDescriptor;

	extern iof::io_oflow_driver___ SOutDriver;
	extern 	iof::io_oflow_driver___ SErrDriver;
	extern iof::io_iflow_driver___ SInDriver;

	class cof___
	: public flw::oflow__
	{
	private:
		flw::datum__ _Cache[IOP__BUFFER_SIZE];
	public:
		void Init(
			fdr::oflow_driver_base___ &Driver,
			flw::size__ AmountMax = FLW_AMOUNT_MAX )
		{
			oflow__::Init( Driver, _Cache, sizeof( _Cache ), AmountMax );
		}
		void Init(
			fdr::oflow_driver_base___ &,
			fdr::thread_safety__ )
				// Une ancienne version de cette bibliothèque avait une méthode avec ces paramètres. Un appel à cette méthode appelait la méthode ci-dessus, ce qui provoque une erreur.
				// Cette méthode a donc été mise en palce pour détecter et corrger ce cas de figure...
		{
			ERRPrm();
		}
	};


	class cif__
	: public flw::iflow__
	{
	public:
		void Init(
			fdr::iflow_driver_base___ &Driver,
			flw::size__ AmountMax = FLW_AMOUNT_MAX )
		{
			iflow__::Init( Driver, AmountMax );
		}
		void Init(
			fdr::oflow_driver_base___ &Driver,
			fdr::thread_safety__ )	// A cause d'une 
		{
			ERRPrm();
		}
		void Init(
			fdr::iflow_driver_base___ &,
			fdr::thread_safety__ )
				// Une ancienne version de cette bibliothèque avait une méthode avec ces paramètres. Un appel à cette méthode appelait la méthode ci-dessus, ce qui provoque une erreur.
				// Cette méthode a donc été mise en palce pour détecter et corrger ce cas de figure...
		{
			ERRPrm();
		}
	};

	// 'thread safe'.
	extern cof___ COutF, CErrF;
	extern cif__ CInF;


	// 'thread unsafe'.
	extern txf::text_oflow__ COut, CErr;
	extern txf::text_iflow__ CIn;

	enum target__
	{
		tConsole,	// Lecture/écriture de/dans la console.
		tVoid,		// Lecture/écriture de/dans rien (utile pour les service Windows).
		tUser,		// Lecture/écriture de/dans des dispositifs fournis par l'utilisateur (qui doit initialiser 'C(Out|In|Err)Driver').
		t_amount,
		t_Undefined,
		t_Default = tConsole
	};

	void Initialize( target__ Target );
	/* Peform the initialization needed by this library if not
	automatically done (defining of 'CIO_NO_AUTOMATIC_INITIALIZATION' ). */

	target__ Target( void );

	inline bso::bool__ IsInitialized( void )
	{
		return Target() != t_Undefined;
	}
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
