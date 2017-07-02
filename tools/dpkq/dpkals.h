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


#ifndef DPKALS__INC
# define DPKALS__INC

# include "str.h"

# include "dpkbsc.h"

namespace dpkals {

	using namespace dpkbsc;

	class record_alias_
	{
	public:
		struct s {
			str::string_::s Label;
			trow__ TableRow;
			rrow__ RecordRow;
		}&S_;
		str::string_ Label;
		record_alias_( s &S )
		: S_( S ),
		  Label( S.Label )
		{}
		void reset( bso::bool__ P = true )
		{
			Label.reset( P );

			S_.TableRow = qNIL;
			S_.RecordRow = qNIL;
		}
		void plug( uys::sHook &Hook )
		{
			Label.plug( Hook );
		}
		void plug( qASd *AS )
		{
			tol::plug( AS, Label );
		}
		record_alias_ &operator =( const record_alias_ &RA )
		{
			Label = RA.Label;

			S_.TableRow = RA.S_.TableRow;
			S_.RecordRow = RA.S_.RecordRow;

			return *this;
		}
		void Init( void )
		{
			reset();

			Label.Init();
		}
		void Init(
			const str::string_ &Label,
			trow__ TableRow,
			rrow__ RecordRow )
		{
			reset();

			this->Label.Init( Label );

			S_.TableRow = TableRow;
			S_.RecordRow = RecordRow;
		}
		E_RODISCLOSE_( trow__, TableRow );
		E_RODISCLOSE_( rrow__, RecordRow );
	};

	E_AUTO( record_alias );

	typedef ctn::E_MCONTAINER_( record_alias_ ) record_aliases_;
	E_AUTO( record_aliases )

	class table_alias_
	{
	public:
		struct s {
			str::string_::s Label;
			trow__ TableRow;
		}&S_;
		str::string_ Label;
		table_alias_( s &S )
		: S_( S ),
		  Label( S.Label )
		{}
		void reset( bso::bool__ P = true )
		{
			Label.reset( P );

			S_.TableRow = qNIL;
		}
		void plug( str::sHook &Hook )
		{
			Label.plug( Hook );
		}
		void plug( qASd *AS )
		{
			Label.plug( AS );
		}
		table_alias_ &operator =( const table_alias_ &TA )
		{
			Label = TA.Label;
			S_.TableRow = TA.S_.TableRow;

			return *this;
		}
		void Init( void )
		{
			reset();

			Label.Init();
		}
		void Init(
			const str::string_ &Label,
			trow__ TableRow )
		{
			reset();

			this->Label.Init( Label );
			S_.TableRow = TableRow;
		}
		E_RODISCLOSE_( trow__, TableRow );
	};

	E_AUTO( table_alias );


	typedef ctn::E_MCONTAINER_( table_alias_ ) table_aliases_;
	E_AUTO( table_aliases )

	class aliases_ {
	public:
		struct s {
			record_aliases_::s Records;
			table_aliases_::s Tables;
		};
		record_aliases_ Records;
		table_aliases_ Tables;
		aliases_ ( s &S )
		: Records( S.Records ),
		  Tables( S.Tables )
		{}
		void reset( bso::bool__ P = true )
		{
			Records.reset( P );
			Tables.reset( P );
		}
		void plug( qASd *AS )
		{
			Records.plug( AS );
			Tables.plug( AS );
		}
		aliases_ &operator =( const aliases_ &A )
		{
			Records = A.Records;
			Tables = A.Tables;

			return *this;
		}
		void Init( void )
		{
			reset();

			Records.Init();
			Tables.Init();
		}
	};

	E_AUTO( aliases )

	inline sdr::row__ FindAlias(
		const str::string_ &Label,
		const record_aliases_ &Aliases )
	{
		return SearchInMono<record_aliases_, record_alias_, sdr::row__>( Label, Aliases );
	}

	inline sdr::row__ FindAlias(
		const str::string_ &Label,
		const table_aliases_ &Aliases )
	{
		return SearchInMono<table_aliases_, table_alias_, sdr::row__>( Label, Aliases );
	}

	inline sdr::row__ Store(
		const record_alias_ &Alias,
		record_aliases_ &Aliases )
	{
		sdr::row__ Row = FindAlias( Alias.Label, Aliases );

		if ( Row != qNIL )
			Aliases.Store( Alias, Row );
		else
			Row = Aliases.Append( Alias );

		return Row;
	}

	trow__ FindTable(
		const str::string_ &Label,
		const table_aliases_ &Aliases );
};

#endif
