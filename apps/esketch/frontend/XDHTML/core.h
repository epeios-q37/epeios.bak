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

	inline void Register_( void )
	{
		global::Register();
		prolog::Register();
		login::Register();
		main::Register();

		base::AddAllowedActionsOnWhenNotConnectedToBackend( xdhcmn::CloseActionLabel, NULL );

		base::AddAllowedActionsOnWhenNotConnectedToBackend(
			global::About.Name, global::Refresh.Name, global::Test.Name,
			NULL );

		base::AddAllowedActionsOnWhenNotConnectedToBackend(
			prolog::DisplayProjectFilename.Name, prolog::LoadProject.Name, prolog::SwitchProjectType.Name,	// All 'prolog'-related actions are allowed.
			NULL );

		base::AddAllowedActionsOnWhenNotConnectedToBackend(
			login::Dismiss.Name, login::DisplayEmbeddedBackendFilename.Name, login::Connect.Name, login::SwitchBackendType.Name,	// All 'login'-related actions too.
			NULL );

		base::AddAllowedActionsOnWhenNotConnectedToBackend(
			main::HideTestButton.Name, main::ShowTestButton.Name,
			NULL );

	};

	class rInstancesCore
	{
	public:
		frdinstc::rUser User;
		void reset( bso::bool__ P = true )
		{
			User.reset( P );
		}
		E_CVDTOR( rInstancesCore );
		void Init( frdfrntnd::rFrontend &Frontend );
	};

	typedef sclxdhtml::rSession<rInstancesCore, frdfrntnd::rFrontend, page__, p_Undefined> rSession_;

	class rSession
	: public rSession_
	{
	protected:
		virtual bso::bool__ XDHCMNLaunch(
			const char *Id,
			const char *Action ) override;
		virtual void SCLXDHTMLRefresh( page__ Page ) override;
	public:
		qCVDTOR( rSession );
	};

	typedef sclxdhtml::core___<rSession> _core___;

	class core___
	: public _core___
	{
	private:
		base::action_helper_callback__ _ActionHelperCallback;
	public:
		void reset( bso::bool__ P = true )
		{
			_ActionHelperCallback.reset( P );
		}
		E_CDTOR( core___ );
		void Init( xdhcmn::mode__ Mode )
		{
			_ActionHelperCallback.Init();
			_core___::Init( Mode, _ActionHelperCallback );
			Register_();
		}
	};

	extern core___ Core;

	sclfrntnd::kernel___ &Kernel( void );

	void About(
		rSession &Session,
		xml::writer_ &Writer );

	void About(
		core::rSession &Session,
		xml::writer_ &Writer );
}

#endif