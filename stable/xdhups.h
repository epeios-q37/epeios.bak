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

#ifndef XDHUPS__INC
# define XDHUPS__INC

# define XDHUPS_NAME		"XDHUPS"

# if defined( E_DEBUG ) && !defined( XDHUPS_NODBG )
#  define XDHUPS_DBG
# endif

// X(SL)/DH(TML) UPStream

# include "xdhcmn.h"

# include "err.h"
# include "dlbrry.h"

namespace xdhups {
	typedef ntvstr::char__ nchar__;
	typedef ntvstr::string___ nstring___;

	typedef xdhcmn::cSession cSession_;

	class sSession
	{
	private:
		Q37_MRMDF( cSession_, C_, Callback_ );
	public:
		void reset( bso::bool__ P = true )
		{
			Callback_ = NULL;
		}
		E_CVDTOR( sSession );
		void Init( cSession_ *Callback )
		{
			reset();

			Callback_ = Callback;
		}
		cSession_ *Callback( void ) const
		{
			return Callback_;
		}
		bso::bool__ Launch(
			const char *Id,
			const char *Action )
		{
			return C_().Launch( Id, Action );
		}
	};

    class agent___
    {
    private:
		dlbrry::dynamic_library___ Library_;
		Q37_MRMDF( xdhcmn::cDownstream, C_, Callback_ );
		TOL_CBUFFER___ Buffer_;
    public:
        void reset( bso::bool__ P = true )
        {
			Library_.reset( P );
			Callback_ = NULL;
        }
        E_CDTOR( agent___ );
		bso::bool__ Init(
			xdhcmn::eMode Mode,
			const str::string_ &ModuleFileName,
			dlbrry::eNormalization Normalization,	// Usually 'dlbrry::n_Default', except when used for 'Node.js' (set to 'dlbrry::nExtOnly').
			const char *Identification );
		cSession_ *RetrieveCallback(
			const char *Language,
			const str::dString &Token,	// If empty, PROD, otherwise DEMO.
			xdhcmn::cProxy *Callback )
		{
			return C_().RetrieveCallback( Language, Token, Callback );
		}
		const char *BaseLanguage( TOL_CBUFFER___ &Buffer ) const
		{
			return C_().BaseLanguage( Buffer );
		}
		void ReleaseCallback( cSession_ *Callback )
		{
			return C_().ReleaseCallback( Callback );
		}
	};
}

#endif
