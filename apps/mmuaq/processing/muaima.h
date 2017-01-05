/*
	Copyright (C) 2016-2017 Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of 'MMUAq' software.

    'MMUAq' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'MMUAq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'MMUAq'.  If not, see <http://www.gnu.org/licenses/>.
*/

// MUA Internet Message Access protocol (IMAP 4rev1 - RFC 3501).

#ifndef MUAIMA__INC
# define MUAIMA__INC

# ifdef XXX_DBG
#	define MUAIMA__DBG
# endif

# include "muabsc.h"

namespace muaima {

	class rSession
	{
	private:
		str::wString Tag_;
		qRMV( fdr::rIODriver, D_, Driver_ );
	public:
		void reset( bso::sBool P = true )
		{
			tol::reset( P, Tag_, Driver_ );
		}
		qCDTOR( rSession );
		void Init( fdr::rIODriver &Driver )
		{
			tol::Init( Tag_ );
			Driver_ = &Driver;
		}
		const str::dString &GetNextTag( void )
		{
			return muabsc::GetNextIMAPTag( Tag_ );
		}
		fdr::rIODriver &Driver( void ) const
		{
			return D_();
		}
	};

	namespace base {

		qENUM( Indicator_ ) {
			iOK,
			i_True = iOK,
			iBAD,
			i_False = iBAD,
			i_Error,
			iErroneous = i_Error,	// Server returned a not 'POP3' compliant answer.
			i_amount,
			i_Undefined
		};

		qXENUM( Indicator, i );

		const str::dString &Capability(
			rSession &Session,
			str::dString &Capability );
	}
}


#endif
