/*
	Copyright (C) 2016-2017 Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of 'MMUAq' software.

	'MUAq' is free software: you can redistribute it and/or modify it
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

// MUA BaSiCs

#ifndef MUABSC_INC_
# define MUABSC_INC_

# include "bch.h"
# include "bso.h"
# include "err.h"
# include "flx.h"
# include "str.h"

namespace muabsc {
	const str::dString &GetNextIMAPTag( str::dString &Tag );

	typedef bso::sUInt sIndex;

	typedef bch::qBUNCHdl( sIndex ) dIndexes;
	qW( Indexes );

	void Dump(
		const dIndexes &Indexes,
		txf::sWFlow &Flow );

	class cIndex
	{
	protected:
		virtual void MUABSCOnIndex( sIndex Index ) = 0;
	public:
		qCALLBACK( Index );
		void OnIndex( sIndex Index )
		{
			return MUABSCOnIndex( Index );
		}
	};

	class sDefaultIndexCallback
	: public cIndex
	{
	private:
		qRMV( dIndexes, I_, Indexes_ );
	protected:
		virtual void MUABSCOnIndex( sIndex Index ) override
		{
			I_().Add( Index );
		}
	public:
		void reset( bso::sBool P = true )
		{
			tol::reset( P, Indexes_ );
		}
		qCVDTOR( sDefaultIndexCallback );
		void Init( dIndexes &Indexes )
		{
			Indexes_ = &Indexes;
		}
	};
}

#endif
