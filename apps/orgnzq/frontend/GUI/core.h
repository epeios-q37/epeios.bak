/*
	Copyright (C) 2015-2016 Claude SIMON (http://q37.info/contact/).

	This file is part of 'orgnzq' software.

    'orgnzq' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'orgnzq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'orgnzq'.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef CORE_INC_
# define CORE_INC_

# include "global.h"
# include "prolog.h"
# include "login.h"
# include "main.h"
# include "column.h"
# include "field.h"
# include "fields.h"
# include "record.h"
# include "records.h"

# include "frdinstc.h"

namespace core {
	extern sclxdhtml::rActionHelper OnNotConnectedAllowedActions;
	extern sclxdhtml::rActionHelper OnFieldDefiningAllowedActions;

	qENUM( Page ) {
		pProlog,
		pLogin,
		pMain,
		p_amount,
		p_Undefined
	};

	inline void Register( void ) {
		global::Register();
		prolog::Register();
		login::Register();
		main::Register();
		column::Register();
		field::Register();
		fields::Register();
		record::Register();
		records::Register();

		OnNotConnectedAllowedActions.Add(
			xdhcmn::CloseActionLabel,
			global::About, global::Test,
			prolog::DisplayProjectFilename, prolog::LoadProject, prolog::SwitchProjectType,	// All 'prolog'-related actions are allowed.
			login::Dismiss, login::DisplayEmbeddedBackendFilename, login::Connect, login::SwitchBackendType );	// All 'login'-related actions too.

		OnFieldDefiningAllowedActions.Add( field::DefineEntry, field::DefineNewEntry );
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
	};

	SCLXDHTML_DEF( core::rSession );

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
			Register();
		}
	};

	extern core___ Core;

	sclfrntnd::rKernel &Kernel( void );

	void About(
		rSession &Session,
		xml::writer_ &Writer );
}

#endif