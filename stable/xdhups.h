/*
	Copyright (C) 2000-2015 Claude SIMON (http://q37.info/contact/).

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

# include "xdhcbk.h"

# include "err.h"
# include "dlbrry.h"

namespace xdhups {

	typedef xdhcbk::session_callback__ _session_callback__;

	class session__
	{
	private:
		Q37_MRMDF( xdhcbk::session_callback__, _C, _Callback );
	public:
		void reset( bso::bool__ P = true )
		{
			if ( P )
				if ( _Callback != NULL )
					delete _Callback;

			_Callback = NULL;
		}
		E_CVDTOR( session__ );
		void Init( xdhcbk::session_callback__ *Callback )
		{
			reset();

			_Callback = Callback;
		}
		bso::bool__ Launch(
			const char *Id,
			const char *Action )
		{
			return _C().Launch( Id, Action );
		}
	};

    class agent___
    {
    private:
		dlbrry::dynamic_library___ _Library;
		Q37_MRMDF( xdhcbk::downstream_callback__, _C, _Callback );
		TOL_CBUFFER___ Buffer;
    public:
        void reset( bso::bool__ P = true )
        {
			_Library.reset( P );
			_Callback = NULL;
        }
        E_CDTOR( agent___ );
		bso::bool__ Init(
			frdbse::mode__ Mode,
			const str::string_ &ModuleFileName,
			const char *Identification );
		xdhcbk::session_callback__ *New(
			const char *Language,
			xdhcbk::proxy_callback__ *Callback )
		{
			return _C().New( Language, Callback );
		}
		const char *BaseLanguage( TOL_CBUFFER___ &Buffer ) const
		{
			return _C().BaseLanguage( Buffer );
		}
	};
}

#endif
