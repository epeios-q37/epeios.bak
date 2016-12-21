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

# include "base.h"
# include "instc.h"

# include "sclxdhtml.h"

namespace core {
	extern sclxdhtml::rActionHelper OnNotConnectedAllowedActions;

	enum page__ {
		pProlog,
		pLogin,
		pMain,
		p_amount,
		p_Undefined
	};

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
	};

	typedef sclxdhtml::core___<rSession> _core___;

	class core___
	: public _core___
	{
	private:
		base::sActionHelper ActionHelperCallback_;
	public:
		void reset( bso::bool__ P = true )
		{
			ActionHelperCallback_.reset( P );
		}
		E_CDTOR( core___ );
		void Init( xdhcmn::mode__ Mode );
	};

	extern core___ Core;

	sclfrntnd::rKernel &Kernel( void );

	void About(
		rSession &Session,
		xml::writer_ &Writer );
}

#endif