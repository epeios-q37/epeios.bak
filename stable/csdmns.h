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

// Client-Server Devices Muxed Network Server

#ifndef CSDMNS__INC
# define CSDMNS__INC

# define CSDMNS_NAME		"CSDMNS"

# if defined( E_DEBUG ) && !defined( CSDMNS_NODBG )
#  define CSDMNS_DBG
# endif

# include "csdmxs.h"
# include "csdscb.h"
# include "csdbns.h"

# include "sdr.h"
# include "lstbch.h"
# include "err.h"

namespace csdmns {
	using namespace csdmxs;

	typedef csdbns::port__ fPort;

	// Pour l'utilisation en tant que service Windows, voir csdbns::server__'. 
	class server___
	{
	private:
		csdbns::server___ Server_;
		rCallback Callback_;
	public:
		void reset( bso::bool__ P = true )
		{
			Callback_.reset( P );
			Server_.reset( P );
		}
		E_CDTOR( server___ );
		void Init(
			fPort Port,
			fCallback_ &Callback,
			fLogCallback *LogCallback = NULL )
		{
			Callback_.Init( Callback, LogCallback );

			Server_.Init( Port, Callback_ );
		}
		void Init(
			fPort Port,
			fCallback_ &Callback,
			fLogCallback &LogCallback )
		{
			Init( Port, Callback, &LogCallback );
		}
		bso::bool__ LaunchService( const char *ServiceName )
		{
			return Server_.LaunchService( ServiceName );
		}
		void Process( sck::duration__ Timeout = SCK_INFINITE )
		{
			Server_.Process( Timeout );
		}
	};
}

#endif
