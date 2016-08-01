/*
	Copyright (C) 1999-2016 Claude SIMON (http://q37.info/contact/).

	This file is part of the Epeios framework.

	The Epeios framework is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

	The Epeios framework is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with the Epeios framework.  If not, see <http://www.gnu.org/licenses/>
*/

// Frontend/Backend Layout Backend Universal Request 

#ifndef FBLBUR__INC
# define FBLBUR__INC

# define FBLBUR_NAME		"FBLBUR"

# if defined( E_DEBUG ) && !defined( FBLBUR_NODBG )
#  define FBLBUR_DBG
# endif

# include "err.h"
# include "flw.h"

# include "fblbrr.h"
# include "fblber.h"
# include "fblbrq.h"

namespace fblbur {

	using namespace fblovl;

	typedef fblbrq::rRequest rRequest_;

	class universal_request___
	: public rRequest_
	{
	private:
		fblber::embedded_callbacks___ _Embedded;
		fblbrr::remote_callbacks___ _Remote;
	public:
		void reset( bso::bool__ P = true )
		{
			rRequest_::reset( P );

			_Embedded.reset( P );
			_Remote.reset( P );
		}
		universal_request___( void )
		{
			reset( false );
		}
		~universal_request___( void )
		{
			reset();
		}
		void Init(
			eMode Mode,
			flw::ioflow__ &Flow )
		{
			reset();

			switch ( Mode ) {
			case mReferenced:
				_Embedded.Init();
				rRequest_::Init( _Embedded, Flow );
				break;
			case mSerialized:
				_Remote.Init();
				rRequest_::Init( _Remote, Flow );
				break;
			default:
				qRFwk();
				break;
			}
		}
	};
}

#endif
