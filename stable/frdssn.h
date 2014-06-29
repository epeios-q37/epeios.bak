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
		bso::bool__ _IsOpen( void ) const
		{
			return _Language != NULL;
		}
		void _Test( void ) const
		{
			if ( !_IsOpen() )
				ERRFwk();
		}
		const frdkrn::kernel___ &_K( void ) const
		{
			if ( _Kernel == NULL )
				ERRFwk();

			return *_Kernel;
		}
		frdkrn::kernel___ &_K( void )
		{
			if ( _Kernel == NULL )
				ERRFwk();

			return *_Kernel;
		}
	protected:
		virtual void FRDSSNOpen( const char *Language ) = 0;
		virtual void FRDSSNClose( void ) = 0;
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
		void Open( const char *Language )
		{
			_Registry.Init( _Kernel->Registry() );
			_Language = Language;
			FRDSSNOpen( Language );
		}
		void Close( void )
		{
			_Registry.reset();
			_Language = NULL;
			FRDSSNClose();
		}
		void DismissRequest( void )	// A appeler uniquement lorsque l'un des paramètres de sortie est un 'flow', dés que tout son contenu ('EndOfFlow()' retourne 'true') est lu.
		{
			_Test();

			_K().DismissRequest();
		}
		const frdrgy::registry_ &Registry( void ) const
		{
			_Test();

			return _Registry;
		}
		frdrgy::registry_ &Registry( void )
		{
			_Test();

			return _Registry;
		}
		const lcl::locale_ &Locale( void ) const
		{
			_Test();

			return _K().Locale();
		}
		bso::bool__ IsOpen( void ) const
		{
			return _IsOpen();
		}
		const char *Language( void ) const
		{
			_Test();

			return _Language;
		}
		recap__ DumpSetupRegistry( xml::writer_ &Writer ) const
		{
			_Test();

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
