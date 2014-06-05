/*
	Header for the 'frdssn' library by Claude SIMON (csimon at zeusw dot org)
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

//	$Id: xxx.h,v 1.9 2012/11/14 16:06:23 csimon Exp $

#ifndef FRDSSN__INC
#define FRDSSN__INC

#define FRDSSN_NAME		"FRDSSN"

#define	FRDSSN_VERSION	"$Revision: 1.9 $"

#define FRDSSN_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &FRDSSNTutor;

#if defined( E_DEBUG ) && !defined( FRDSSN_NODBG )
#define FRDSSN_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.9 $
//C Claude SIMON (csimon at zeusw dot org)
//R $Date: 2012/11/14 16:06:23 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D FRontenD SeSSioN 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

# include "err.h"
# include "flw.h"

# include "frdkrn.h"
# include "frdrgy.h"

namespace frdssn {
	using namespace frdkrn;

	class session___
	{
	private:
		frdkrn::kernel___ *_Kernel;
		const char *_Language;
		frdrgy::registry _Registry; 
		lcl::meaning _Meaning;
	protected:
		virtual void FRDSSNOpenSession( const char *Language ) = 0;
		virtual void FRDSSNCloseSession( void ) = 0;
	public:
		void reset( bso::bool__ P = true )
		{
			_Kernel = NULL;
			_Language = NULL;
			_Registry.reset( P );
			_Meaning.reset( P );
		}
		E_CVDTOR( session___ );
		void Init( frdkrn::kernel___ &Kernel )
		{
			_Kernel = &Kernel;
			_Registry.Init( Kernel.Registry() );
			_Meaning.Init();
			_Language = NULL;
		}
		void OpenSession( const char *Language )
		{
			_Registry.Init( _Kernel->Registry() );
			_Language = Language;
			FRDSSNOpenSession( Language );
		}
		void CloseSession( void )
		{
			_Registry.reset();
			_Language = NULL;
			FRDSSNCloseSession();
		}
/*		const frdkrn::kernel___ &Kernel( void ) const
		{
			if ( _Kernel == NULL )
				ERRFwk();

			return *_Kernel;
		}
		frdkrn::kernel___ &Kernel( void )
		{
			if ( _Kernel == NULL )
				ERRFwk();

			return *_Kernel;
		}
*/		const frdrgy::registry_ &Registry( void ) const
		{
			if ( !IsOpen() )
				ERRFwk();

			return _Registry;
		}
		frdrgy::registry_ &Registry( void )
		{
			if ( !IsOpen() )
				ERRFwk();

			return _Registry;
		}
		const lcl::locale_ &Locale( void ) const
		{
			if ( !IsOpen() )
				ERRFwk();

			return _Kernel->Locale();
		}
		bso::bool__ IsOpen( void ) const
		{
			return _Language != NULL;
		}
		const char *Language( void ) const
		{
			if ( !IsOpen() )
				ERRFwk();

			return _Language;
		}
		recap__ DumpSetupRegistry( xml::writer_ &Writer ) const
		{
			if ( !IsOpen() )
				ERRFwk();

			_Registry.DumpSetup( E_NIL, true, Writer);

			return r_OK;
		}
		recap__ FillSetupRegistry(
			xtf::extended_text_iflow__ &SetupXFlow,
			const xpp::criterions___ &Criterions,
			error_set___ &ErrorSet );
		status__ FillSetupRegistry(
			xtf::extended_text_iflow__ &SetupXFlow,
			const xpp::criterions___ &Criterions );
	};
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
