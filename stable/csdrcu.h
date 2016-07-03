/*
	Copyright (C) 1999-2016 Claude SIMON (http://q37.info/contact/).

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

#ifndef CSDRCU__INC
# define CSDRCU__INC

# define CSDRCU_NAME		"CSDRCU"

# if defined( E_DEBUG ) && !defined( CSDRCU_NODBG )
#  define CSDRCU_DBG
# endif

// Client-Server Devices Remote Client Upstream

# include "csdrcc.h"

# include "flw.h"
# include "plgn.h"

namespace csdrcu {
	using csdrcc::cDriver;
	using csdrcc::rDriver;

	class core___
	{
	private:
		plgn::retriever___<cDriver> Retriever_;
	public:
		void reset( bso::bool__ P = true )
		{
			Retriever_.reset( P );
		}
		E_CDTOR( core___ );
		sdr::sRow Init(
			const str::string_ &PluginPath,
			const char *Identifier,
			const str::string_ &Parameters,
			const plgn::dAbstracts &Abstracts )
		{
			return Retriever_.Initialize( PluginPath, Identifier, Parameters, Abstracts );
		}
		rDriver *New( void )
		{
			return Retriever_.Plugin().New();
		}
		void Delete( rDriver *D )
		{
			return Retriever_.Plugin().Delete( D );
		}
		const char *Identifier( void )
		{
			return Retriever_.Identifier();
		}
		const char *Details( void )
		{
			return Retriever_.Details();
		}
	};

	typedef flw::standalone_ioflow__<> _ioflow__;

	class client__
	: public _ioflow__
	{
	private:
		qRMV( core___, C_, Core_ );
		qRMV( rDriver, D_, Driver_ );
	public:
		void reset( bso::sBool P = true )
		{
			_ioflow__::reset( P );

			if ( P ) {
				if ( Driver_ != NULL ) {
					C_().Delete( Driver_ );
				}
			}

			Driver_ = NULL;
			Core_ = NULL;
		}
		qCDTOR( client__ );
		void Init( core___ &Core )
		{
			reset();

			Core_ = &Core;

			Driver_ = C_().New();

			_ioflow__::Init( D_() );
		}
	};

}

#endif
