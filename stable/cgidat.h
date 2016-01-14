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

//	$Id: cgidat.h,v 1.4 2013/05/15 14:40:37 csimon Exp $

#ifndef CGIDAT__INC
#define CGIDAT__INC

#define CGIDAT_NAME		"CGIDAT"

#define	CGIDAT_VERSION	"$Revision: 1.4 $"

#define CGIDAT_OWNER		"Claude SIMON (http://zeusw.org/intl/contact.html)"

#if defined( E_DEBUG ) && !defined( CGIDAT_NODBG )
#define CGIDAT_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.4 $
//C Claude SIMON (http://zeusw.org/intl/contact.html)
//R $Date: 2013/05/15 14:40:37 $

/* End of automatic documentation generation part. */

/* Addendum to the automatic documentation generation part. */
//D CGI (Common Gateway Interface) DATa 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"
#include "sdr.h"
#include "str.h"
#include "lstbch.h"
#include "lstctn.h"
#include "cgiarg.h"

namespace cgidat {
	//t Row.
	E_ROW( row__ );

	//t Content row.
	E_ROW( content_row__ );

	//t Content.
	typedef str::string_	content_;
	typedef str::string		content;

	//c Relationship. Internal use.
	struct _relationship__
	{
	public:
		//o Name.
		content_row__ Name;
		//o Suffix (to distinguish datum with same name).
		content_row__ Suffix;
		//o Value.
		content_row__ Value;
		void reset( bso::bool__ P = true )
		{
			Name = Suffix = Value = qNIL;
		}
		_relationship__( void )
		{
			reset( false );
		}
	};

	//t Relationships. Internal use.
	typedef lstbch::E_LBUNCHt_( _relationship__, row__ ) _relationships_;
	E_AUTO( _relationships )

	//t Contents. Internal use.
	typedef lstctn::E_LXMCONTAINERt_( content_, content_row__ ) _contents_;
	E_AUTO( _contents )


	//c Data.
	class data_ 
	{
	public:
		struct s {
			_relationships_::s Relationships;
			_contents_::s Contents;
		};
		//o Relationships between contents.
		_relationships_ Relationships;
		//o Contents.
		_contents_ Contents;
		data_( s &S )
		: Relationships( S.Relationships ),
		  Contents( S.Contents )
		{}
		void reset( bso::bool__ P = true )
		{
			Relationships.reset( P );
			Contents.reset( P );
		}
		void plug( qAS_ &AS )
		{
			Relationships.plug( AS );
			Contents.plug( AS );
		}
		data_ &operator =( const data_ &D )
		{
			Relationships = D.Relationships;
			Contents = D.Contents;

			return *this;
		}
		//f Initialization.
		void Init( void )
		{
			Relationships.Init();
			Contents.Init();
		}
		//f Set data with 'Name', 'Suffix' and 'Value'.
		row__ Set(
			const content_ &Name,
			const content_ &Suffix,
			const content_ &Value )
		{
			_relationship__ Relationship;
			row__ Row = qNIL;

			if ( ( Row = GetDataWithNameAndSuffix( Name, Suffix ) ) != qNIL ) {
				Relationships.Recall( Row, Relationship );
				Contents.Store( Suffix, Relationship.Suffix );
				Contents.Store( Value, Relationship.Value );
				Contents.Flush();
			} else {
				Relationship.Name = Contents.Add( Name );
				Relationship.Suffix = Contents.Add( Suffix );
				Relationship.Value = Contents.Add( Value );

				Row = Relationships.Add( Relationship );
			}

			return Row;
		}
		//f Set data with 'Name', 'Suffix' and 'Value'. Return its row.
		row__ Set(
			const char *Name,
			const content_ &Suffix,
			const content_ &Value )
		{
			return Set( content( Name ), Suffix, Value );
		}
		//f Set data with 'Name', 'Suffix' and 'Value'. Return its row.
		row__ Set(
			const char *Name,
			const char *Suffix,
			const content_ &Value )
		{
			return Set( Name, content( Suffix ), Value );
		}
		//f Set data with 'Name', 'Suffix' and 'Value'. Return its row.
		row__ Set(
			const char *Name,
			const char *Suffix,
			const char *Value )
		{
			return Set( Name, Suffix, content( Value ) );
		}
		//f Delete datum of row 'Row'.
		void Delete( row__ Row )
		{
			_relationship__ Relationship;
				
			Relationships.Recall( Row, Relationship );

			Contents.Delete( Relationship.Name );
			Contents.Delete( Relationship.Suffix );
			Contents.Delete( Relationship.Value );

			Relationships.Delete( Row );
		}
		//f Delete datum of 'Name' and 'Suffix'.
		void Delete(
			const content_ &Name,
			const content_ &Suffix )
		{
			row__ Row = GetDataWithNameAndSuffix( Name, Suffix );

			if ( Row != qNIL )
				Delete( Row );
		}
		//f Delete datum of 'Name' and 'Suffix'.
		void Delete(
			const char *Name,
			const char *Suffix )
		{
			Delete( content( Name ), content( Suffix ) );
		}
		const content_ &GetContent(
			content_row__ Row,
			str::string_ &Content ) const	// Place dans 'Content' le contenu  la position 'Row'. Retourne 'Content'.
		{
			Contents.Recall( Row, Content );

			return Content;
		}
#ifndef CPE__MT
		//f Return the content at row 'Row'. The returned value is only valid until next call of this fuction.
		const content_ &GetContent( content_row__ Row ) const;
#endif
		//f Return the row of the name for data at 'Row'.
		content_row__ GetNameRow( row__ Row ) const
		{
			return Relationships( Row ).Name;
		}
		//f Return the row of the stamp for data at 'Row'.
		content_row__ GetSuffixRow( row__ Row ) const
		{
			return Relationships( Row ).Suffix;
		}
		//f Return the row of the value for data at 'Row'.
		content_row__ GetValueRow( row__ Row ) const
		{
			return Relationships( Row ).Value;
		}
		//f Return the row for data with name 'Name'.
		row__ GetDataWithName( const content_ &Name ) const;
		//f Return the row for data with name 'Name'.
		row__ GetDataWithName( const char *Name ) const
		{
			return GetDataWithName( content( Name ) );
		}
		//f Return the row for data with name 'Name', type 'Type' and stamp 'Suffix'.
		row__ GetDataWithNameAndSuffix(
			const content_ &Name,
			const content_ &Suffix ) const;
		//f Return the row for data with name 'Name', type 'Type' and stamp 'Suffix'.
		row__ GetDataWithNameAndSuffix(
			const char *Name,
			const char *Suffix ) const
		{
			return GetDataWithNameAndSuffix( content( Name ), content( Suffix ) );
		}
		E_NAVt( Relationships., row__ );
	};

	E_AUTO( data )

	//f Dump into xml filler XMLF.
	template <typename xmlf> inline void Dump(
		const data_ &Data,
		xmlf &XMLF )
	{
qRH
		row__ Row = qNIL;
		content Content;
qRB
		Row = Data.First();

		while( Row != qNIL ) {
			XMLF.PushTag( "Datum" );
			Content.Init();
			XMLF.PutAttribute( "Name", Data.GetContent( Data.GetNameRow( Row ), Content ) );
			Content.Init();
			XMLF.PutAttribute( "Suffix", Data.GetContent( Data.GetSuffixRow( Row ), Content ) );
			Content.Init();
			XMLF.PutValue( Data.GetContent( Data.GetValueRow( Row ), Content ) );

			Row = Data.Next( Row );
			XMLF.PopTag();
		}
qRR
qRT
qRE
	}

	//f Fill 'Data' with 'Args'.
	void Fill(
		const cgiarg::arguments_ &Args,
		data_ &Data );

}

/*$END$*/
#endif
