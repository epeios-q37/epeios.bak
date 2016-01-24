/*
	Copyright (C) 2016 Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of 'eSketch' software.

    'eSketch' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'eSketch' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'eSketch'.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef CORE__INC
# define CORE__INC

# include "global.h"
# include "prolog.h"
# include "login.h"
# include "main.h"

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
		global::fActionCallbacks Global_;
		prolog::fActionCallbacks Prolog_;
		login::fActionCallbacks Login_;
		main::fActionCallbacks Main_;
	public:
		void reset( bso::bool__ P = true )
		{
			Global_.reset( P );
			Prolog_.reset( P );
			Login_.reset( P );
			Main_.reset( P );
		}
		E_CVDTOR( action_callbacks__ );
		void Init( void )
		{
			Prolog_.Init();
			Login_.Init();
			Main_.Init();
			Global_.Init();

			base::AddAllowedActionsOnWhenNotConnectedToBackend( xdhcmn::CloseActionLabel );

			base::AddAllowedActionsOnWhenNotConnectedToBackend(
				&Global_.About, &Global_.Refresh, &Global_.Test,
				NULL );

			base::AddAllowedActionsOnWhenNotConnectedToBackend(
				&Prolog_.DisplayProjectFilename, &Prolog_.LoadProject, &Prolog_.SwitchProjectType,	// All 'prolog'-related actions are allowed.
				NULL );

			base::AddAllowedActionsOnWhenNotConnectedToBackend(
				&Login_.Dismiss, &Login_.DisplayEmbeddedBackendFilename, &Login_.Connect, &Login_.SwitchBackendType,	// All 'login'-related actions too.
				NULL );

			base::AddAllowedActionsOnWhenNotConnectedToBackend(
				&Main_.HideTestButton, &Main_.ShowTestButton,
				NULL );

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

	class fSession
	: public _session___
	{
	protected:
		virtual bso::bool__ XDHCMNLaunch(
			const char *Id,
			const char *Action ) override;
		virtual void SCLXDHTMLRefresh( page__ Page ) override;
	public:
		qCVDTOR( fSession );
	};

	typedef sclxdhtml::core___<fSession> _core___;

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
		void Init( xdhcmn::mode__ Mode )
		{
			_ActionHelperCallback.Init();
			_core___::Init( Mode, _ActionHelperCallback );
			_ActionCallbacks.Init();
		}
	};

	extern core___ Core;

	sclfrntnd::kernel___ &Kernel( void );

	void About(
		fSession &Session,
		xml::writer_ &Writer );

	void About(
		core::fSession &Session,
		xml::writer_ &Writer );
}

#endif