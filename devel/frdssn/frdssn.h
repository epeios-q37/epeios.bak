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

//	$Id: xxx.h,v 1.9 2012/11/14 16:06:23 csimon Exp $

#ifndef FRDSSN_INC_
#define FRDSSN_INC_

#define FRDSSN_NAME		"FRDSSN"

#define	FRDSSN_VERSION	"$Revision: 1.9 $"

#define FRDSSN_OWNER		"Claude SIMON"


#if defined( E_DEBUG ) && !defined( FRDSSN_NODBG )
#define FRDSSN_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.9 $
//C Claude SIMON (csimon at zeusw dot org)
//R $Date: 2012/11/14 16:06:23 $

/* End of automatic documentation generation part. */

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
		frdrgy::registry _Registry; 
		bso::bool__ _IsOpen;
		void _Test( void ) const
		{
			if ( !_IsOpen )
				qRFwk();
		}
		const frdkrn::kernel___ &_K( void ) const
		{
			if ( _Kernel == NULL )
				qRFwk();

			return *_Kernel;
		}
		frdkrn::kernel___ &_K( void )
		{
			if ( _Kernel == NULL )
				qRFwk();

			return *_Kernel;
		}
	protected:
		virtual void FRDSSNOpen( const char *Language ) = 0;
		virtual void FRDSSNClose( void ) = 0;
		virtual const char *FRDSSNLanguage( TOL_CBUFFER___ &Buffer ) = 0;
	public:
		void reset( bso::bool__ P = true )
		{
			_Kernel = NULL;
			_Registry.reset( P );
			_IsOpen = false;
		}
		E_CVDTOR( session___ );
		void Init(
			frdkrn::kernel___ &Kernel,
			const rgstry::multi_level_registry_ &Registry )
		{
			_Kernel = &Kernel;
			_Registry.Init( Registry );
			_IsOpen = false;
		}
		void Open( const char *Language )
		{
qRH
qRB
			_IsOpen = true;
			FRDSSNOpen( Language );
qRR
			_IsOpen = false;
			_Registry.reset();
qRT
qRE

		}
		void Close( void )
		{
			if ( _IsOpen )
				FRDSSNClose();
			_IsOpen = false;
		}
		const char *Language( TOL_CBUFFER___ &Buffer )
		{
			return FRDSSNLanguage( Buffer );
		}
		void DismissRequest( void )	// A appeler uniquement lorsque l'un des paramtres de sortie est un 'flow', ds que tout son contenu ('EndOfFlow()' retourne 'true') est lu.
		{
			_Test();

			_K().DismissRequest();
		}
		const frdrgy::registry_ &Registry( void ) const
		{
			return _Registry;
		}
		frdrgy::registry_ &Registry( void )
		{
			return _Registry;
		}
		bso::bool__ IsOpen( void ) const
		{
			return _IsOpen;
		}
		void DumpSessionRegistry( xml::writer_ &Writer ) const
		{
			_Registry.DumpSession( qNIL, true, Writer);
		}
		bso::bool__ FillSessionRegistry(
			xtf::extended_text_iflow__ &SetupXFlow,
			const xpp::criterions___ &Criterions,
			error_set___ &ErrorSet );
		bso::bool__ FillSessionRegistry(
			xtf::extended_text_iflow__ &SetupXFlow,
			const xpp::criterions___ &Criterions );
	};
}

/*$END$*/
#endif
