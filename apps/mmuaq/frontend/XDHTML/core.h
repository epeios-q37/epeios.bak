/*
	Copyright (C) 2016 Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of 'MMUAq' software.

    'MMUAq' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'MMUAq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'MMUAq'.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef CORE__INC
# define CORE__INC

# include "agent.h"
# include "agents.h"
# include "config.h"
# include "global.h"
# include "prolog.h"
# include "login.h"
# include "main.h"
# include "config.h"

# include "frdinstc.h"

namespace core {
	extern sclxdhtml::rActionHelper OnNotConnectedAllowedActions;

	enum page__ {
		pProlog,
		pLogin,
		pMain,
		pConfig,
		p_amount,
		p_Undefined
	};

	inline void Register_( void )
	{
		agent::Register();
		agents::Register();
		config::Register();
		global::Register();
		prolog::Register();
		login::Register();
		main::Register();

		OnNotConnectedAllowedActions.Add(
			xdhcmn::CloseActionLabel,
			global::About, global::Test,
			prolog::DisplayProjectFilename, prolog::LoadProject, prolog::SwitchProjectType,	// All 'prolog'-related actions are allowed.
			login::Dismiss, login::DisplayEmbeddedBackendFilename, login::Connect, login::SwitchBackendType );	// All 'login'-related actions too.
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
	public:
		qCVDTOR( rSession );
		frdfrntnd::rFrontend &Frontend( void )
		{
			return *this;
		}
	};

	typedef sclxdhtml::core___<rSession> _core___;

	class core___
	: public _core___
	{
	private:
		base::sActionHelper _ActionHelperCallback;
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

	sclfrntnd::rKernel &Kernel( void );

	void About(
		rSession &Session,
		xml::writer_ &Writer );

	void About(
		core::rSession &Session,
		xml::writer_ &Writer );

	void SetAgentsLayout( rSession &Session );

	void SetAgentCasting( rSession &Session );
}

#endif