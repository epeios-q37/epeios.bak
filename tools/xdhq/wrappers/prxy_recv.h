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

#ifndef PRXY_RECV_INC_
# define PRXY_RECV_INC_

# include "prxy_cmn.h"

namespace prxy_recv {
	qENUM( Type )
	{
		tString,
		tStrings,
		t_amount,
		t_Undefined
	};

	class rReturn
	{
	private:
		eType Type_;
		str::wString String_;
		str::wStrings Strings_;
		void Test_( eType Type ) const
		{
			if ( Type_ != Type )
				qRGnr();
		}
	public:
		void reset( bso::sBool P = true )
		{
			tol::reset( P, String_, Strings_ );
			Type_ = t_Undefined;
		}
		qCDTOR( rReturn );
		void Init( void )
		{
			tol::Init( String_, Strings_ );
			Type_ = t_Undefined;
		}
		str::dString &StringToSet( void )
		{
			Test_( t_Undefined );

			Type_ = tString;

			return String_;
		}
		str::dStrings &StringsToSet( void )
		{
			Test_( t_Undefined );

			Type_ = tStrings;

			return Strings_;
		}
		eType GetType( void ) const
		{
			return Type_;
		}
		const str::dString &GetString( void ) const
		{
			Test_( tString );

			return String_;
		}
		const str::dStrings &GetStrings( void ) const
		{
			Test_( tStrings );

			return Strings_;
		}
	};

	void Recv(
		prxy_cmn::eRequest Request,
		flw::sRFlow &Flow,
		rReturn &Return );
}

#endif