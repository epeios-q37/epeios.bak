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

# include "zendq.h"

# include "bso.h"
# include "err.h"
# include "tol.h"

namespace sclznd {

	class sArguments {
	private:
		void ***tsrm_ls_;
		int num_varargs_;
		zval ***varargs_;
		void Get_( int Index )
		{
		}
		void Get_(
			zval **Val,
			long &Long )
		{
			zendq::Get( *Val, Long );
		}
		void Get_(
			zval **Val,
			zendq::sArray &Array )
		{
			Array.Init( Z_ARRVAL_PP( Val ) );
		}
		template <typename arg, typename ...args> void Get_(
			int Index,
			arg &Arg,
			args &...Args )
		{
			Get_( varargs_[Index], Arg );

			Get_( Index+1, Args... );
		}
	public:
		void reset( bso::sBool P = true )
		{
			tsrm_ls_ = NULL;
			num_varargs_ = 0;
			varargs_ = NULL;
		}
		qCDTOR( sArguments );
		void Init(
			void ***tsrm_ls,
			int num_varargs,
			zval ***varargs )
 		{
			tsrm_ls = tsrm_ls;
			num_varargs_ = num_varargs;
			varargs_ = varargs;
		}
		template <typename ...args> void Get( args &...Args )
		{
			Get_( 0, Args... );
		}
	};

	typedef void (* sFunction_)( sArguments &Arguments );

	class sRegistrar
	{
	public:
		void reset( bso::sBool P = true )
		{
		}
		qCDTOR( sRegistrar );
		void Init( void )
		{
		}
		void Register( sFunction_ Function );
		template <typename function, typename ...functions> void Register(
			function Function,
			functions... Functions )
		{
			Register( Function );
			Register( Functions... );
		}
	};

	void SCLZNDRegister( sRegistrar &Registrar );	// To overload by user.
}

#endif
