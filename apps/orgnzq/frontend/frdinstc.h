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

// FRontenD INSTanCes

#ifndef FRDINSTC__INC
# define FRDINSTC__INC

# include "frdfrntnd.h"

namespace frdinstc {

	using namespace frdfrntnd;

	class _user___
	{
	private:
		qRVM( frdfrntnd::rFrontend, F_,  Frontend_ );
		orgnzq::fStatics &S_( void )
		{
			return F_().Statics;
		}
		orgnzq::rOGZRecord Record_;
		orgnzq::rOGZMyObject Object_;
	public:
		void reset( bso::bool__ P = true )
		{	
			Frontend_ = NULL;

			Record_.reset( P );
			Object_.reset( P );
		}
		E_CVDTOR( _user___ );
		void Init( frdfrntnd::rFrontend &Frontend )
		{
			Frontend_ = &Frontend;

			Record_.Init( Frontend.Record );
			Object_.Init( Frontend.MyObject );
		}
		bso::fBool Login(
			const str::vString &Username,
			const str::vString &Password )
		{
			bso::fBool Success = false;

			S_().OGZLogin( Username, Password, Success );

			return Success;
		}
	};

	class user___
	{
	private:
		_user___ Core_;
	public:
		void reset( bso::bool__ P = true )
		{	
			Core_.reset( P );
		}
		E_CVDTOR( user___ );
		void Init( frdfrntnd::rFrontend &Frontend )
		{
			if ( Frontend.IsConnected() )
				Core_.Init( Frontend );
		}
		bso::fBool Login(
			const str::vString &Username,
			const str::vString &Password )
		{
			return Core_.Login( Username, Password );
		}
	};
}

#endif
