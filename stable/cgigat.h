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

#ifndef CGIGAT__INC
# define CGIGAT__INC

# define CGIGAT_NAME		"CGIGAT"

# if defined( E_DEBUG ) && !defined( CGIGAT_NODBG )
#  define CGIGAT_DBG
# endif

// CGI (Common Gateway Interface) GATe

# include "err.h"
# include "flw.h"

# include "cgiarg.h"

# include "xml.h"

# include "csdbns.h"


namespace cgigat {

	typedef csdscb::callback__ _callback__;
		
	class cgi_gate__
	: public _callback__
	{
	private:
		void _Process(
			const cgiarg::arguments_ &Arguments,
			xml::writer_ &Writer,
			str::string_ &XMLFileName )
		{
			CGIDATProcess( Arguments, Writer, XMLFileName );
		}
		void _Process(
			const cgiarg::arguments_ &Arguments,
			str::string_ &XML,
			str::string_ &XMLFileName );
		void *CSDSCBPreProcess( const char *Origin )
		{
			return NULL;
		}
		csdscb::action__ CSDSCBProcess(
			flw::ioflow__ &Flow,
			void *UP );
		void CSDSCBPostProcess( void *UP )
		{}
	protected:
		virtual void CGIDATProcess(
			const cgiarg::arguments_ &Arguments,
			xml::writer_ &Writer,
			str::string_ &XMLFileName ) = 0;
	public:
		void reset( bso::bool__ P = true )
		{
			_callback__::reset( P );
		}
		E_CVDTOR( cgi_gate__ )
		void Init( void )
		{
			_callback__::Init();
		}
	};
}

#endif
