/*
	Copyright (C) 2010-2015 Claude SIMON (http://q37.info/contact/).

	This file is part of dpkq.

    dpkq is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

    dpkq is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with dpkq.  If not, see <http://www.gnu.org/licenses/>
*/

#ifndef DPKTBL__INC
# define DPKTBL__INC

# include "dpkals.h"
# include "dpkrcd.h"

namespace dpktbl {

	using namespace dpkals;
	using namespace dpkrcd;

	class table_ {
	public:
		struct s {
			str::string_::s Label;
			records_::s Records;
			counter__ Skipped;
	//		aliases_::s Aliases;
		} &S_;
		str::string_ Label;
		records_ Records;
	//	aliases_ Aliases;
		table_( s &S )
		: S_( S ),
		  Label( S.Label ),
		  Records( S.Records )/*,
		  Aliases( S.Aliases ) */
		{};
		void reset( bso::bool__ P = true )
		{
			Label.reset( P );
			Records.reset( P );
	//		Aliases.reset( P );

			S_.Skipped = 0;

		}
		void plug( qASd *AS )
		{
			Label.plug( AS );
			Records.plug( AS );
	//		Aliases.plug( MM );
		}
		table_ &operator =(const table_ &T )
		{
			Label = T.Label;
			Records = T.Records;
	//		Aliases = T.Aliases;
			S_.Skipped = T.S_.Skipped;

			return *this;
		}
		void Init( void )
		{
			Label.Init();
			Records.Init();
	//		Aliases.Init();
			S_.Skipped = 0;
		}
		E_RWDISCLOSE_( counter__, Skipped );
	};

	E_AUTO( table )

	typedef ctn::E_CONTAINERt_( table_, trow__ ) tables_;
	E_AUTO( tables );

	inline trow__ SearchTable(
		const str::string_ &Label,
		const tables_ &Tables )
	{
		return SearchInMulti<tables_, table_, trow__>( Label, Tables );
	}

	inline rrow__ SearchRecord(
		const str::string_ &Label,
		trow__ TableRow,
		const tables_ &Tables )
	{
		ctn::qCITEMs( table_, trow__ ) Table;

		Table.Init( Tables );

		return SearchRecord( Label, Table( TableRow ).Records );
	}

	inline void Insert(
		rrow__ RecordRow,
		trow__ TableRow,
		const tables_ &Tables,
		record_ &Record )
	{
		ctn::qCITEMs( table_, trow__ ) Table;

		Table.Init( Tables );

		Insert( RecordRow, Table( TableRow ).Records, Record );
	}

	inline void Insert(
		sdr::row__ AliasRow,
		const record_aliases_ &Aliases,
		const tables_ &Tables,
		record_ &Record )
	{
		ctn::qCMITEMsl( record_alias_ ) Alias;

		Alias.Init( Aliases );

		Insert( Alias( AliasRow ).RecordRow(), Alias( AliasRow ).TableRow(), Tables, Record );
	}
};

#endif
