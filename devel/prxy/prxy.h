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

// PRoXY

#ifndef PRXY__INC
# define PRXY__INC

# define PRXY_NAME		"PRXY"

# if defined( E_DEBUG ) && !defined( PRXY_NODBG )
#  define PRXY_DBG
# endif

# include "err.h"
# include "flw.h"

namespace prxy {
	class rProxy
	{
	private:
		qRVM( flw::ioflow__, F_, Flow_ );
	public:
		void reset( bso::bool__ P = true )
		{
			Flow_ = NULL;
		}
		qCDTOR( rProxy );
		bso::bool__ Init(
			flw::ioflow__ NakedFlow,
			flw::ioflow__ &Flow,
			const char *Identifier )
		{
			Flow_ = &Flow;

			NakedFlow.Write( Identifier, strlen( Identifier ) + 1 );	// '+1' to put the final 0.

			NakedFlow.Commit();

			if ( !NakedFlow.EndOfFlow() ) {
				if ( NakedFlow.Get() != 0 )
					qRGnr();

				NakedFlow.Dismiss();

				return true;
			} else
				return false;
		}
		fdr::size__ WriteUpTo(
			const fdr::byte__ *Buffer,
			fdr::size__ Maximum )
		{
			return F_().WriteUpTo( Buffer, Maximum );
		}
		void Commit( void )
		{
			F_().Commit();
		}
		fdr::size__ ReadUpTo(
			fdr::size__ Maximum,
			fdr::byte__ *Buffer )
		{
			return F_().ReadUpTo( Maximum, Buffer );
		}
		void Dismiss( void )
		{
			F_().Dismiss();
		}
	};
}

#endif
