/*
	Header for the 'dbptbl' module by Claude SIMON (http://zeusw.org/epeios/contact.html)
	Copyright (C) 2010 Claude SIMON.
*/

#ifndef DBPTBL__INC
# define DBPTBL__INC

# include "dbpals.h"
# include "dbprcd.h"

namespace dbptbl {

	using namespace dbpals;
	using namespace dbprcd;

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
		void plug( ags::E_ASTORAGE_ &AS )
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
		ctn::E_CITEMt( table_, trow__ ) Table;

		Table.Init( Tables );

		return SearchRecord( Label, Table( TableRow ).Records );
	}

	inline void Insert(
		rrow__ RecordRow,
		trow__ TableRow,
		const tables_ &Tables,
		record_ &Record )
	{
		ctn::E_CITEMt( table_, trow__ ) Table;

		Table.Init( Tables );

		Insert( RecordRow, Table( TableRow ).Records, Record );
	}

	inline void Insert(
		sdr::row__ AliasRow,
		const record_aliases_ &Aliases,
		const tables_ &Tables,
		record_ &Record )
	{
		ctn::E_CMITEM( record_alias_ ) Alias;

		Alias.Init( Aliases );

		Insert( Alias( AliasRow ).RecordRow(), Alias( AliasRow ).TableRow(), Tables, Record );
	}
};

#endif
