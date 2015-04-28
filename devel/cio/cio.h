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

namespace cio {

	extern iop::descriptor__ CInDescriptor, COutDescriptor, CErrDescriptor;

	namespace {
		template <typename d> d &_TestAndGet( d* D )
		{
			if ( D == NULL )
				ERRFwk();

			return *D;
		}
	}

	class set__
	{
	private:
		fdr::iflow_driver_base___ *_In;
		fdr::oflow_driver_base___
			*_Out,
			*_Err;
	public:
		void reset( bso::bool__ = true )
		{
			_In = NULL;
			_Out = NULL;
			_Err = NULL;
		}
		E_CDTOR( set__ );
		set__(
			fdr::iflow_driver_base___ &In,
			fdr::oflow_driver_base___ &Out,
			fdr::oflow_driver_base___ &Err )
		{
			reset( false );

			Init( In, Out, Err );
		}
		void Init(
			fdr::iflow_driver_base___ &In,
			fdr::oflow_driver_base___ &Out,
			fdr::oflow_driver_base___ &Err )
		{
			_In = &In;
			_Out = &Out;
			_Err = &Err;
		}
		fdr::iflow_driver_base___ &In( void ) const
		{
			return _TestAndGet( _In );
		}
		fdr::oflow_driver_base___ &Out( void ) const
		{
			return _TestAndGet( _Out );
		}
		fdr::oflow_driver_base___ &Err( void ) const
		{
			return _TestAndGet( _Err );
		}
	};

	const set__ &GetConsoleSet( void );	// Returns a set to read/write to/from console. NOT ALWAYS USABLE !

	const set__ &GetVoidSet( void );	// Returns a set to read/write to/from void.

	const set__ &GetCurrentSet( void );	// Returns current set.

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
				// Une ancienne version de cette bibliothque avait une mthode avec ces paramtres. Un appel  cette mthode appelait la mthode ci-dessus, ce qui provoque une erreur.
				// Cette mthode a donc t mise en palce pour dtecter et corrger ce cas de figure...
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
			fdr::thread_safety__ )
				// Une ancienne version de cette bibliothque avait une mthode avec ces paramtres. Un appel  cette mthode appelait la mthode ci-dessus, ce qui provoque une erreur.
				// Cette mthode a donc t mise en place pour dtecter et corrger ce cas de figure...
		{
			ERRPrm();
		}
		void Init(
			fdr::iflow_driver_base___ &,
			fdr::thread_safety__ )
				// Idem que ci-dessus.
		{
			ERRPrm();
		}
	};

	// 'thread safe'.
	extern cif__ CInF;
	extern cof___ COutF, CErrF;


	// 'thread unsafe'.
	extern txf::text_iflow__ CIn;
	extern txf::text_oflow__ COut, CErr;

	enum target__
	{
		tConsole,	// Lecture/criture de/dans la console.
		tVoid,		// Lecture/criture de/dans rien (utile pour les service Windows).
		tUser,		// Lecture/criture de/dans des dispositifs fournis par l'utilisateur (qui doit initialiser 'C(Out|In|Err)Driver').
		tCurrent = tUser,
		t_amount,
		t_Undefined,
		t_Default = tConsole
	};

	const set__ &GetSet( target__ Target );

	target__ GetTarget( const set__ &Set );

	void Initialize( target__ Target );

	void Initialize(
		const set__ &Set );

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
