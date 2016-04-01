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

#ifndef DPKRCD__INC
# define DPKRCD__INC

# include "str.h"

# include "dpkbsc.h"
# include "dpkals.h"

namespace dpkrcd {

	using namespace dpkbsc;
	using namespace dpkals;

	class record_
	{
	public:
		struct s
		{
			str::string_::s
				Label,
				Content;
			weight__ Weight;
			bso::bool__ Skip;
		} &S_;
		str::string_
			Label,
			Content;
		record_( s &S )
		: S_( S ),
		  Label( S.Label ),
		  Content( S.Content )
		{
		}
		void reset( bso::bool__ P = true )
		{
			Label.reset( P );
			Content.reset( P );

			S_.Weight = DEFAULT_WEIGHT;
			S_.Skip = false;
		}
		void plug( qASd *AS )
		{
			Label.plug( AS );
			Content.plug( AS );
		}
		record_ &operator =( const record_ &R )
		{
			Label = R.Label;
			Content = R.Content;

			S_.Weight = R.S_.Weight;
			S_.Skip = R.S_.Skip;

			return *this;
		}
		void Init( void )
		{
			S_.Weight = DEFAULT_WEIGHT;
			S_.Skip = false;

			Label.Init();
			Content.Init();
		}
		void Init(
			const str::string_ &Label,
			const str::string_ &Content,
			weight__ Weight )
		{
			reset();

			this->Label.Init( Label );
			this->Content.Init( Content );

		}
		E_RWDISCLOSE_( weight__, Weight );
		E_RWDISCLOSE_( bso::bool__, Skip );
	};

	E_AUTO( record )

	typedef ctn:: E_CONTAINERt_( record_, rrow__ ) records_;
	E_AUTO( records )

	inline rrow__ SearchRecord(
		const str::string_ &Label,
		const records_ &Records )
	{
		return SearchInMulti<records_, record_, rrow__>( Label, Records );
	}

	inline void Insert(
		rrow__ Row,
		const records_ &Records,
		record_ &Record )
	{
		ctn::qCITEMs( record_, rrow__ ) SourceRecord;

		SourceRecord.Init( Records );

		Record.Content.Append( SourceRecord( Row ).Content );
	}
};

#endif
