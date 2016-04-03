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

#ifndef FBLOVL__INC
#define FBLOVL__INC

#define FBLOVL_NAME		"FBLOVL"

#define	FBLOVL_VERSION	"$Revision: 1.5 $"

#define FBLOVL_OWNER		"Claude SIMON"


#if defined( E_DEBUG ) && !defined( FBLOVL_NODBG )
#define FBLOVL_DBG
#endif

//D Frontend/Backend Layout OVerLapping 

#define FBLOVL_PROTOCOL_VERSION	"11"

namespace fblovl {

	enum mode__ {
		mNone,
		mEmbedded,
		mRemote,
		m_Amount,
		m_Undefined
	};

	enum reply__ {
		rOK,
		rSoftwareError,	// Lorsque survient un 'ERR[x|X](...)'.
		rRequestError,	// Lorsque survient un problme lors de l'excution de la requte.
		rDisconnected,	// Connection was lost.
		r_amount,
		r_Undefined
	};

	const char *GetLabel( reply__ Reply );

}

/*$END$*/
#endif
