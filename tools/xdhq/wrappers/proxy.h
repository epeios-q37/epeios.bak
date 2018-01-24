/*
	Copyright (C) 2017 Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of 'XDHq' software.

    'XDHq' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'XDHq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'XDHq'.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef PROXY_INC_
# define PROXY_INC_

# include "prtcl.h"

# include "prxy_send.h"
# include "prxy_recv.h"

# include "csdscb.h"
# include "flw.h"
# include "str.h"

namespace proxy {
	using prxy_send::rArguments;
	using prxy_recv::rReturn;

	struct rData
	{
	private:
		mtx::rHandler Lock_;
	public:
		rReturn Return;
		str::wString	// For the launching of an action.
			Id,
			Action;
		prxy_cmn::eRequest Request;
		rArguments Arguments;
		str::wString Language;
		void reset( bso::sBool P = true )
		{
			if ( P ) {
				if ( Lock_ != mtx::UndefinedHandler )
					mtx::Delete( Lock_ );
			}

			tol::reset( P, Return, Id, Action, Arguments, Language );
			Request = prxy_cmn::r_Undefined;
			Lock_ = NULL;
		}
		qCDTOR( rData );
		void Init( void )
		{
			reset();

			tol::Init( Return, Id, Action, Arguments, Language );
			Request = prxy_cmn::r_Undefined;
			Lock_ = mtx::Create();
		}
		void Lock( void )
		{
			mtx::Lock( Lock_ );
		}
		void Unlock( void )
		{
			mtx::Unlock( Lock_ );
		}
	};

	class rProcessing
	: public csdscb::cProcessing
	{
	protected:
		virtual void *CSDSCBPreProcess(
			fdr::rRWDriver *IODriver,
			const ntvstr::char__ *Origin ) override;
		virtual csdscb::eAction CSDSCBProcess(
			fdr::rRWDriver *IODriver,
			void *UP ) override;
		virtual bso::sBool CSDSCBPostProcess( void *UP ) override;
		virtual rData *PRXYNew( void ) = 0;
		virtual void PRXYOnAction( rData *Data ) = 0;
		virtual void PRXYOnPending( rData *Data ) = 0;
	public:
		void reset( bso::sBool P = true )
		{}
		qCVDTOR( rProcessing );
		void Init( void )
		{}
	};
}

#endif