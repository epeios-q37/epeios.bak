/*
	Copyright (C) 1999-2017 Claude SIMON (http://q37.info/contact/).

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

// Native 4 (for) ALL Wrapper part

#ifndef N4ALLW_INC_
# define N4ALLW_INC_

# define N4ALLW_NAME		"N4ALLW"

# if defined( E_DEBUG ) && !defined( N4ALLW_NODBG )
#  define N4ALLW_DBG
# endif

# include "err.h"
# include "n4all.h"

namespace n4allw {
	typedef bch::qBUNCHdl( void * ) dFunctions_;
	qW( Functions_ );

	class rLauncher
	{
	private:
		dlbrry::rDynamicLibrary Library_;
		qRMV( n4all::cLauncher, L_, Launcher_ );
		void *GetFunction_( sdr::sRow Row );
		wFunctions_ Functions_;
		bso::bool__ Register_(
			const fnm::rName &ComponentFilename,
			dlbrry::eNormalization Normalization,
			n4all::cRegistrar &Registrar,
			sclmisc::sRack & Rack,
			void * UP,
			bso::sBool SkipComponentUnloading );
	public:
		void reset( bso::sBool P = true )
		{
			if ( P ) {
				if ( Launcher_ != NULL )
					delete Launcher_;
			}

			tol::reset( P, Library_, Functions_ );
			Launcher_ = NULL;
		}
		qCDTOR( rLauncher );
		void Init( void )
		{
			reset();

			Functions_.Init();
			// 'Library' will be 'Init(...)' later.
		}
		bso::sBool Init(
			const fnm::rName &ComponentFilename,
			dlbrry::eNormalization Normalization,
			sclmisc::sRack &Rack,
			void *UP,
			bso::sBool SkipComponenentUnloading,	// The component will be unloaded when quitting the program, and not explicitly (at true only for a 'PHP' workaround under 'GNU/Linux').
			qRPD );
		inline void Launch(
			sdr::sRow Row,
			n4all::cCaller &Caller )
		{
			return L_().Launch( GetFunction_( Row ), Caller );
		}
		bso::sBool GetInfo( str::dString &Info ) const
		{
			if ( Launcher_ != NULL ) {
				L_().Info( Info );
				return true;
			} else
				return false;
		}
	};
}

#endif
