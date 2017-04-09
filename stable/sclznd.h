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
		int Index_;
		void SGet_( void )
		{}
		void SGet_(
			zval **Val,
			long &Long )
		{
			zendq::Get( *Val, Long );
		}
		void SGet_(
			zval **Val,
			zendq::sArray &Array )
		{
			zendq::Get( Val, Array );;
		}
		template <typename t, int (* type_id)( void )> void SGet_(
			zval **Val,
			zendq::sResource<t, type_id> &Resource )
		{
# ifdef ZTS
			zendq::Get( Val, tsrm_ls_, Resource );
# else
			zendq::Get( Val, Resource );
# endif
		}
		void MGet_( void )
		{}
		template <typename arg, typename ...args> void MGet_(
			arg &Arg,
			args &...Args )
		{
			SGet_( varargs_[Index_++], Arg );

			MGet_( Args... );
		}
	public:
		void reset( bso::sBool P = true )
		{
			tsrm_ls_ = NULL;
			num_varargs_ = 0;
			varargs_ = NULL;
			Index_ = 0;
		}
		qCDTOR( sArguments );
		void Init(
			void ***tsrm_ls,
			int num_varargs,
			zval ***varargs )
 		{
			tsrm_ls_ = tsrm_ls;
			num_varargs_ = num_varargs;
			varargs_ = varargs;
			Index_ = 0;
		}
		template <typename ...args> void Get( args &...Args )
		{
			MGet_( Args... );
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
