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

// Node.JS CoMmoN

#ifndef NJSCMN_INC_
# define NJSCMN_INC_

# define NJSCMN_NAME		"NJSCMN"

# if defined( E_DEBUG ) && !defined( NJSCMN_NODBG )
#  define NJSCMN_DBG
# endif

# include "err.h"
# include "tol.h"

namespace njscmn {
	qENUM( Type )
	{
		tString,
		tamount,
		t_Undefined
	};

	class cArguments {
	protected:
		virtual void NJSCMNGetValue(
			int Index,
			eType Type,
			void *Value ) = 0;
		virtual void NJSCMNSetReturnValue(
			eType Type,
			const void *Value ) = 0;
	public:
		qCALLBACK( Arguments );
		void GetValue(
			int Index,
			eType Type,
			void *Value )
		{
			return NJSCMNGetValue( Index, Type, Value );
		}
		void SetReturnValue(
			eType Type,
			const void *Value )
		{
			return NJSCMNSetReturnValue( Type, Value );
		}
	};
}

#endif
