/*
	'core.h' by Claude SIMON (http://zeusw.org/).

	 This file is part of 'xdhdq' software.

    'xdhdq' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'xdhdq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'xdhdq'.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef CORE__INC
# define CORE__INC

# include "global.h"
# include "prolog.h"
# include "login.h"
# include "main.h"
# include "fields.h"

# include "frdinstc.h"

namespace core {

	enum page__ {
		pProlog,
		pLogin,
		pMain,
		p_amount,
		p_Undefined
	};

	struct action_callbacks__
	{
	private:
		global::action_callbacks__ _Global;
		prolog::action_callbacks__ _Prolog;
		login::action_callbacks__ _Login;
		main::action_callbacks__ _Main;
		fields::action_callbacks__ _Fields;
	public:
		void reset( bso::bool__ P = true )
		{
			_Global.reset( P );
			_Prolog.reset( P );
			_Login.reset( P );
			_Main.reset( P );
			_Fields.reset( P );
		}
		E_CVDTOR( action_callbacks__ );
		void Init( void )
		{
			_Prolog.Init();
			_Login.Init();
			_Main.Init();
			_Fields.Init();
			_Global.Init();
		}
	};

	class instances_core___
	{
	public:
		frdinstc::user___ User;
		void reset( bso::bool__ P = true )
		{
			User.reset( P );
		}
		E_CVDTOR( instances_core___ );
		void Init( frdfrntnd::frontend___ &Frontend );
	};

	typedef sclxdhtml::session___<instances_core___, frdfrntnd::frontend___, page__, p_Undefined> _session___;

	class session___
	: public _session___
	{
	protected:
		virtual bso::bool__ XDHCBKLaunch(
			const char *Id,
			const char *Action ) override;
		virtual void SCLXDHTMLRefresh( page__ Page ) override;
	public:
		E_CVDTOR( session___ );
	};

	typedef sclxdhtml::core___<session___> _core___;

	class core___
	: public _core___
	{
	private:
		action_callbacks__ _ActionCallbacks;
		base::action_helper_callback__ _ActionHelperCallback;
	public:
		void reset( bso::bool__ P = true )
		{
			_ActionCallbacks.reset( P );
			_ActionHelperCallback.reset( P );
		}
		E_CDTOR( core___ );
		void Init( frdbse::mode__ Mode )
		{
			_ActionHelperCallback.Init();
			_core___::Init( Mode, _ActionHelperCallback );
			_ActionCallbacks.Init();
		}
	};

	extern core___ Core;

	void About(
		session___ &Session,
		xml::writer_ &Writer );

	void About(
		core::session___ &Session,
		xml::writer_ &Writer );
}

#endif