/*
	Copyright (C) 2016 Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of 'XDHDq' software.

    'XDHDq' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'XDHDq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'XDHDq'.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef CORE__INC
# define CORE__INC

# include "base.h"
# include "instc.h"

# include "sclxdhtml.h"

namespace core {
	extern sclxdhtml::rActionHelper OnNotConnectedAllowedActions;

	qENUM( Page ) {
		pProlog,
		pLogin,
		pMain,
		p_amount,
		p_Undefined
	};

	class rInstances
	{
	public:
		instc::rUser User;
		void reset( bso::bool__ P = true )
		{
			User.reset( P );
		}
		E_CVDTOR( rInstances );
		void Init( frdfrntnd::rFrontend &Frontend );
	};

	class sDump {
	public:
		static void Corpus(
			rInstances &Instances,
			xml::dWriter &Writer );
		static void Common(
			rInstances &Instances,
			xml::dWriter &Writer );
	};

	typedef sclxdhtml::rSession<rInstances, frdfrntnd::rFrontend, ePage, p_Undefined, sDump> rSession;

	class rSession_
	: public rSession
	{
	protected:
		virtual bso::bool__ XDHCMNLaunch(
			const char *Id,
			const char *Action ) override;
	public:
		qCVDTOR( rSession_ );
	};

	typedef sclxdhtml::rCore<rSession_> rCore_;

	class rCore
	: public rCore_
	{
	private:
		base::sActionHelper ActionHelperCallback_;
	public:
		void reset( bso::bool__ P = true )
		{
			ActionHelperCallback_.reset( P );
		}
		E_CDTOR( rCore );
		void Init( xdhcmn::mode__ Mode );
	};

	extern rCore Core;

	sclfrntnd::rKernel &Kernel( void );

	void About(
		rSession &Session,
		xml::writer_ &Writer );
}

#endif