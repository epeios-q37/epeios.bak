/*
	Copyright (C) 2016-2017 Claude SIMON (http://zeusw.org/epeios/contact.html).

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

#ifndef CORE_INC_
# define CORE_INC_

# include "base.h"

# include "instc.h"

# include "sclxdhtml.h"

namespace core {
	extern sclxdhtml::rActionHelper OnNotConnectedAllowedActions;
	extern sclxdhtml::rActionHelper OnFolderEditionIgnoredActions;

	qENUM( Page ) {
		pProlog,
		pLogin,
		pMain,
		pConfig,
		p_amount,
		p_Undefined
	};

	sclfrntnd::rKernel &Kernel( void );

	class rInstancesCore
	{
	public:
		instc::rUser User;
		void reset( bso::bool__ P = true )
		{
			User.reset( P );
		}
		E_CVDTOR( rInstancesCore );
		void Init( frdfrntnd::rFrontend &Frontend );
	};

	typedef sclxdhtml::rSession<rInstancesCore, frdfrntnd::rFrontend, ePage, p_Undefined> rSession_;

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
		void Init( xdhcmn::mode__ Mode );
	};

	extern core___ Core;

	void About(
		rSession &Session,
		xml::writer_ &Writer );

	void About(
		core::rSession &Session,
		xml::writer_ &Writer );

	void SetAgentsLayout( rSession &Session );

	void SetAgentCasting( rSession &Session );

	namespace folder {
		extern const char *NameInputId;

		bso::sBool Apply( rSession &Session );
	}
}

#endif