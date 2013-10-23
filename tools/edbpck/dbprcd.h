/*
	Header for the 'dbprcd' module by Claude SIMON (http://zeusw.org/epeios/contact.html)
	Copyright (C) 2010 Claude SIMON.
*/

#ifndef DBPRCD__INC
# define DBPRCD__INC

# include "str.h"

# include "dbpbsc.h"
# include "dbpals.h"

namespace dbprcd {

	using namespace dbpbsc;
	using namespace dbpals;

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
		void plug( ags::E_ASTORAGE_ &AS )
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
		ctn::E_CITEMt( record_, rrow__ ) SourceRecord;

		SourceRecord.Init( Records );

		Record.Content.Append( SourceRecord( Row ).Content );
	}
};

#endif
