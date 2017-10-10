/*
	Copyright (C) 1999-2017 Claude SIMON (http://q37.info/contact/).

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

// SoCLe ZeND (PHP)

#ifndef SCLZND_INC_
# define SCLZND_INC_

# define SCLZND_NAME		"SCLZND"

# if defined( E_DEBUG ) && !defined( SCLZND_NODBG )
#  define SCLZND_DBG
# endif

# include "bso.h"
# include "err.h"
# include "n4znd.h"
# include "scln4a.h"
# include "tol.h"

namespace sclznd {
	typedef scln4a::sCaller sCaller_;

	class sCaller
	: public sCaller_ {
	private:
		bso::sU8 Index_;
	public:
		void reset( bso::sBool P = true )
		{
			sCaller_::reset( P );
			Index_ = 0;
		}
		qCDTOR( sCaller );
		void Init( n4all::cCaller &Caller )
		{
			sCaller_::Init( Caller );

			Index_ = 0;
		}
		void Get( str::dString &String )
		{
			C_().GetArgument( Index_++, n4znd::tString, &String );
		}
		void SetReturnValue( const str::dString &String )
		{
			C_().SetReturnValue( n4znd::tString, &String );
		}
	};

	typedef void ( fFunction )( sCaller &Caller );

	typedef scln4a::sRegistrar<fFunction> sRegistrar;

	void SCLZNDInfo( txf::sOFlow &Flow );	// To define by user.
	void SCLZNDRegister( sRegistrar &Registrar );	// To define by user
}

# define SCLZND_F( name ) void name( sclznd::sCaller &Caller )

#endif
