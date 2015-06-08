/*
	Header for the 'dbpals' module by Claude SIMON (http://zeusw.org/epeios/contact.html)
	Copyright (C) 2010 Claude SIMON.
*/

#ifndef DBPALS__INC
# define DBPALS__INC

# include "str.h"

# include "dbpbsc.h"

namespace dbpals {

	using namespace dbpbsc;

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
		void plug( qSD__ &SD )
		{
			Label.plug( SD );
		}
		void plug( qAS_  &AS )
		{
			Label.plug( AS );
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
		void plug( qSD__ &SD )
		{
			Label.plug( SD );
		}
		void plug( qAS_ &AS )
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
		void plug( qAS_ &AS )
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

	inline sdr::row__ FindRecordAlias_(
		const str::string_ &Label,
		const record_aliases_ &Aliases )
	{
		return SearchInMono<record_aliases_, record_alias_, sdr::row__>( Label, Aliases );
	}

	inline sdr::row__ FindTableAlias_(
		const str::string_ &Label,
		const table_aliases_ &Aliases )
	{
		return SearchInMono<table_aliases_, table_alias_, sdr::row__>( Label, Aliases );
	}

	trow__ SearchTable(
		const str::string_ &Label,
		const table_aliases_ &Aliases );
};

#endif
