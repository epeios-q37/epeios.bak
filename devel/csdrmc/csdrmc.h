/*
	Copyright (C) 2000-2015 Claude SIMON (http://q37.info/contact/).

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

#ifndef CSDRMC__INC
# define CSDRMC__INC

# define CSDRMC_NAME		"CSDRMC"

# if defined( E_DEBUG ) && !defined( CSDRMC_NODBG )
#  define CSDRMC_DBG
# endif

// Client-Server Devices ReMote Client

# include "err.h"
# include "flw.h"

#define CSDRMC_PLUGIN_NAME			"Remote"
#define CSDRMC_PLUGIN_API_VERSION	"1"

namespace csdrmc {

	typedef fdr::ioflow_driver___<> _driver___;

	class driver___
	: public _driver___
	{
	public:
		void reset( bso::bool__ P = true )
		{
			_driver___::reset( P );
		}
		E_CVDTOR( driver___ );
		void Init( void )
		{
			_driver___::Init( fdr::ts_Default );
		}
		static const char *Identification( void );
	};

	class core___
	: public _driver___
	{
	private:
		Q37_MRMDF( driver___, D_, Driver_ );
	protected:
		virtual fdr::size__ FDRWrite(
			const fdr::byte__ *Buffer,
			fdr::size__ Maximum ) override
		{
			return D_().Write( NULL, Buffer, Maximum );
		}
		virtual void FDRCommit( void ) override
		{
			return D_().Commit( NULL );
		}
		virtual fdr::size__ FDRRead(
			fdr::size__ Maximum,
			fdr::byte__ *Buffer ) override
		{
			return D_().Read( NULL, Maximum, Buffer, fdr::bNonBlocking );
		}
		virtual void FDRDismiss( void ) override
		{
			return D_().Dismiss( NULL );
		}
	public:
		void reset( bso::bool__ P = true )
		{
			_driver___::reset( P );
			Driver_ = NULL;
		}
		E_CDTOR( core___ );
		void Init( driver___ &Driver )
		{
			_driver___::Init( fdr::ts_Default );
			Driver_ = &Driver;
		}
	};

	typedef flw::standalone_ioflow__<> client__;
}

#endif
