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

// PrOXY BASE

#ifndef PRXYBASE__INC
# define PRXYBASE__INC

# define PRXYBASE_NAME		"PRXYBASE"

# if defined( E_DEBUG ) && !defined( PRXYBASE_NODBG )
#  define PRXYBASE_DBG
# endif

# include "tol.h"
# include "flw.h"
# include "str.h"

# include "err.h"

namespace prxybase {
	qCDEF(char *, ProtocolId, "e8ae7bb1-c950-4102-b420-c38e3b870cfb" );
	qCDEF(bso::sU8, ProtocolVersion, 0 );

	qENUM( Request ) {
		rPlug,
		rPing,
		rFreeze,
		rCrash,
		r_amount,
		r_Undefined
	};

	const char *GetLabel( eRequest Request );

	eRequest GetRequest( const str::dString &Pattern );

	eRequest GetRequest( flw::iflow__ &Flow );

	void PutRequest(
		eRequest Request,
		flw::oflow__ &Flow );

	qENUM( Answer ) {
		aForbidden,
		aPlugged,
		aPong,
		aFrozen,
		// NOTA : no answer for 'crash', as the program can not answer because it is crashed !
		a_amount,
		a_Undefined
	};

	const char *GetLabel( eAnswer Answer );

	eAnswer GetAnswer( const str::dString &Pattern );

	eAnswer GetAnswer( flw::iflow__ &Flow );

	void PutAnswer(
		eAnswer Answer,
		flw::oflow__ &Flow );

	qENUM( Type ) {
		tClient,
		tServer,
		t_amount,
		t_Undefined
	};

	const char *GetLabel( eType Type );

	eType GetType( const str::dString &Pattern );

	eType GetType( flw::iflow__ &Flow );

	void PutType(
		eType Type,
		flw::oflow__ &Flow );

	const str::dString &GetId(
		flw::iflow__ &Flow,
		str::dString &Id );

	inline void PutId(
			const char *Identifier,
			flw::oflow__ &Flow )
		{
			Flow.Write( Identifier, strlen( Identifier ) + 1 );	// '+1' to put the final 0.
		};
}

#endif
