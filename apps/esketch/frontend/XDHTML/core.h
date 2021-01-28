/*
	Copyright (C) 2018 Claude SIMON (http://zeusw.org/epeios/contact.html).

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

#ifndef CORE_INC_
# define CORE_INC_

# include "base.h"
# include "instc.h"

# include "sclx.h"

namespace core {
	extern sclx::rActionHelper OnNotConnectedAllowedActions;

	using instc::rInstances;

	class sDump {
	public:
		static void Corpus(
			rInstances &Instances,
			xml::rWriter &Writer );
		static void Common(
			rInstances &Instances,
			xml::rWriter &Writer );
	};

	typedef sclx::rSession < rInstances, frdfrntnd::rFrontend, base::ePage, base::p_Undefined, sDump > rSession;

	class sActionHelper
	: public sclx::cActionHelper<core::rSession>
	{
	protected:
		virtual bso::bool__ SCLXOnBeforeAction(
			core::rSession &Session,
			const char *Id,
			const char *Action ) override;
		virtual void SCLXOnRefresh( core::rSession &Session ) override;
		virtual bso::sBool SCLXOnClose( core::rSession &Session ) override;
	};

	class rCore
	: public sclx::rCore<rSession>
	{
	private:
		sActionHelper ActionHelperCallback_;
	public:
		void reset( bso::bool__ P = true )
		{
			ActionHelperCallback_.reset( P );
		}
		E_CDTOR( rCore );
		void Init( xdhcdc::eMode Mode );
	};

	extern rCore Core;

	typedef sclx::cAction<rSession> cAction;

	void Register(
		const char *Name,
		cAction &Callback );

	sclf::rKernel &Kernel( void );

	void Connect(
		xdhcdc::eMode Mode,
		bso::sBool FromUI,	// If true, retrieve backend features from UI, otherwise from registry.
		rSession &Session);

	void About(
		rSession &Session,
		xml::rWriter &Writer );
}

#endif
