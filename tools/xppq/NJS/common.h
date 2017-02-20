/*
	Copyright (C) 2007-2017 Claude SIMON (http://q37.info/contact/).

	This file is part of xppq.

	xppq is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

	xppq is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with xppq. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef COMMON_INC_
# define COMMON_INC_

# include "v8q.h"

# include "flx.h"

namespace common {
	struct sFRelay {
	private:
		flx::rDRelay<> Core_;
	public:
		flx::sIRelay In;
		flx::sORelay Out;
		void reset( bso::sBool P = true )
		{
			tol::reset( P, Core_, In, Out );
		}
		qCDTOR( sFRelay );
		void Init( void )
		{
			Core_.Init();
			In.Init( Core_ );
			Out.Init( Core_ );
		}
	};

	class cASync {
	protected:
		virtual bso::sBool COMMONProcess( void ) = 0;
		virtual void COMMONDisclose( void ) = 0;
	public:
		qCALLBACK( ASync );
		bso::sBool Process( void )
		{
			return COMMONProcess();
		}
		void Disclose( void )
		{
			return COMMONDisclose();
		}
	};

	void HandleASync(
		cASync &Callbacks,
		bso::sBool Skip );
}

#endif
