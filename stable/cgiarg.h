/*
	Copyright (C) 2000-2015 Claude SIMON (http://q37.info/contact/).

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

//	$Id: cgiarg.h,v 1.5 2013/05/15 14:40:36 csimon Exp $

#ifndef CGIARG__INC
#define CGIARG__INC

#define CGIARG_NAME		"CGIARG"

#define	CGIARG_VERSION	"$Revision: 1.5 $"

#define CGIARG_OWNER		"Claude SIMON (http://zeusw.org/intl/contact.html)"

#if defined( E_DEBUG ) && !defined( CGIARG_NODBG )
#define CGIARG_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.5 $
//C Claude SIMON (http://zeusw.org/intl/contact.html)
//R $Date: 2013/05/15 14:40:36 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D CGI (Common Gateway Interface) ARGument 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"
#include "ctn.h"
#include "str.h"
#include "bch.h"

namespace cgiarg {
	//d The default suffix delimiter.
	#define CGIARG_SUFFIX_DELIMITER	'.'


	typedef str::string_	base_;
	typedef str::string		base;

	//t Name.
	typedef base_	name_;
	typedef base	name;

	//t Suffix.
	typedef base	suffix_;
	typedef base	suffix;

	//t Value.
	typedef base_	value_;
	typedef base	value;


	// Row.
	E_ROW( row__ );

	// Data row.
	E_ROW( data_row__ );

	// The value indicating there is no indice.
	#define CGIARG_NO_INDICE	BSO_ULONG_MAX


	//e Comparison methods.
	enum comparison_method__ {
		//i Unknown method.
		cmUnknow,
		//i Begins with.
		cmBegins,
		//i Strictly equal.
		cmStrictly,
		//i Amount of comparison methods.
		cm_amount
	};

	// Internal use.
	struct link__
	{
		data_row__ Name;
		data_row__ Suffix;
		data_row__ Value;
		link__(
			data_row__ Name = qNIL,
			data_row__ Suffix = qNIL,
			data_row__ Value = qNIL )
		{
			this->Name = Name;
			this->Suffix = Suffix;
			this->Value = Value;
		}
	};

	//c The arguments of a URL.
	class arguments_
	{
	public:
		struct s{
			ctn::E_MCONTAINERt_( str::string_, data_row__ )::s Data;
			bch::E_BUNCHt_( link__, row__ )::s Links;
		};
		//o The different strings.
		ctn::E_MCONTAINERt_( str::string_, data_row__ ) Data;
		//o The links.
		bch::E_BUNCHt_( link__, row__ ) Links;
		arguments_( s &S )
		: Data( S.Data ),
		  Links( S.Links )
		{}
		void reset( bool P = true )
		{
			Data.reset( P );
			Links.reset( P );
		}
		void plug( qAS_ &AS )
		{
			Data.plug( AS );
			Links.plug( AS );
		}
		arguments_ &operator =( const arguments_ &A )
		{
			Data = A.Data;
			Links = A.Links;

			return *this;
		}
		//f Initialization.
		void Init( void )
		{
			Data.Init();
			Links.Init();
		}
		//f Add the argument of name 'Name', suffix 'Suffix' and value 'Value'. Return the position where added.
		row__ Add(
			const name_ &Name,
			const suffix_ &Suffix,
			const value_ &Value )
		{
			return Links.Append( link__( Data.Append( Name ), Data.Append( Suffix ), Data.Append( Value ) ) );
		}
		//f Add the argument of name 'Name', suffix 'Suffix' and value 'Value'. Return the position where added.
		row__ Add(
			const char *Name,
			const char *Suffix,
			const char *Value )
		{
			return Add( name( Name ), suffix( Suffix ), value( Value ) );
		}
		//f Add the argument of name 'Name' and value 'Value'. Return the position where added.
		row__ Add(
			const name_ &Name,
			const value_ &Value )
		{
			return Links.Append( link__( Data.Append( Name ), qNIL, Data.Append( Value ) ) );
		}
		//f Add the argument of name 'Name', and value 'Value'. Return the position where added.
		row__ Add(
			const char *Name,
			const char *Value )
		{
			return Add( name( Name ), value( Value ) );
		}
		//f Add the argument of name 'Name'. Return the position where added.
		row__ Add( const name_ &Name )
		{
			return Links.Append( link__( Data.Append( Name ), qNIL, qNIL ) );
		}
		//f Add the argument of name 'Name'. Return the position where added.
		row__ Add( const bso::char__ *Name )
		{
			return Add( name( Name ) );
		}
		//f Put value 'Value' for the argument of position 'Position'.
		void PutValue(
			const value_ &Value,
			row__ Position )
		{
			link__ L = Links( Position );

			if ( L.Value == qNIL )
			{
				L.Value = Data.New();
				Links.Store( L, Position );
			}

			Data( L.Value ) = Value;
			Data.Flush();
		}
		/*f Return the position of the first argument of name 'Name', beginning at 'Begin' (not included),
		using comparison method 'Method', 'qNIL' if inexistant. */
		row__ Locate(
			const name_ &Name,
			row__ Begin = qNIL,
			comparison_method__ Method = cmStrictly ) const;
		/*f Return the position of the first argument of name 'Name', beginning at 'Begin' (not included),
		using comparison method 'Method', 'qNIL' if inexistant. */
		row__ Locate(
			const char *Name,
			row__ Begin = qNIL,
			comparison_method__ Method = cmStrictly) const
		{
			return Locate( name( Name ), Begin, Method );
		}
		/*f Return the position of the first argument of name 'Name', and suffix 'Suffix'
		using comparison method 'Method', 'qNIL' if inexistant. */
		row__ Locate(
			const name_ &Name,
			const suffix_ &Suffix,
			comparison_method__ Method = cmStrictly ) const;
		/*f Return the position of the first argument of name 'Name', beginning at 'Begin' (not included),
		using comparison method 'Method', 'qNIL' if inexistant. */
		row__ Locate(
			const char *Name,
			const char *Suffix,
			comparison_method__ Method = cmStrictly) const
		{
			return Locate( name( Name ), suffix( Suffix ), Method );
		}
		//f Put in 'Name' the name of the argument at position 'Position'. Return 'Name'.
		const name_ &GetName(
			row__ Position,
			name_ &Name ) const
		{
			Data.Recall( Links( Position ).Name, Name );

			return Name;
		}
#ifndef CPE__MT
		//f Return the name at position 'Position'. Only valid until another call of this method.
		const name_ &GetName( row__ Position ) const;
#endif
		//f Put in 'Value' the value of the argument at position 'Position'. Return 'Value'.
		const value_ &GetValue(
			row__ Position,
			value_ &Value ) const
		{
			Data.Recall( Links( Position ).Value, Value );

			return Value;
		}
#ifndef CPE__MT
		//f Return the value at position 'Position'. Only valid until another call of this method.
		const name_ &GetValue( row__ Position ) const;
#endif
		//f Return the position of data of suffix of argument at position 'Position', or qNIL if this argument hasn't a suffix.
		data_row__ GetSuffixPosition( row__ Position ) const
		{
			return Links( Position ).Suffix;
		}
		//f Put in 'suffix' the suffix of the argument at position 'Position'. Return 'Suffix'.
		const suffix_ &GetSuffix(
			row__ Position,
			suffix_ &Suffix ) const
		{
			data_row__ R = GetSuffixPosition( Position );

			if ( R != qNIL )
				Data.Recall( R, Suffix );
			else
				Suffix.Init();

			return Suffix;
		}
#ifndef CPE__MT
		//f Return the suffix at position 'Position'. Only valid until another call of this method.
		const suffix_ &GetSuffix( row__ Position ) const;
#endif
		//f Put in 'Value' the value of argument named 'Name'. Return true if such an argument exists, 'false' otherwise'.
		bso::bool__ GetValue(
			const name_ &Name,
			value_ &Value ) const;
		//f Put in 'Value' the value of argument named 'Name'. Return true if such an argument exists, 'false' otherwise'.
		bso::bool__ GetValue(
			const char *Name,
			value_ &Value ) const
		{
			return GetValue( name( Name ), Value );
		}
		//f Put in 'Value' the value of argument of name 'Name' and suffix 'Suffix'. Return true if such an argument exists, 'false' otherwise'.
		bso::bool__ GetValue(
			const name_ &Name,
			const suffix_ &Suffix,
			value_ &Value ) const
		{
			row__ P = Locate( Name, Suffix );

			if ( P != qNIL ){
				GetValue( P, Value );
				return true;
			} else
				return false;
		}
		//f Put in 'Value' the value of argument named 'Name'. Return true if such an argument exists, 'false' otherwise'.
		bso::bool__ GetValue(
			const char *Name,
			const char *Suffix,
			value_ &Value ) const
		{
			return GetValue( name( Name ), suffix( Suffix ), Value );
		}
		E_NAVt( Links., row__ )
		//f Fill with arguments contained in 'IFLow' using 'SuffixDelimiter' as suffix delimiter.
		void Parse(
			flw::iflow__ &IFlow,
			char SuffixDelimiter = CGIARG_SUFFIX_DELIMITER );
	};

	E_AUTO( arguments )
}
		
txf::text_oflow__ &operator <<(
	txf::text_oflow__ &F,
	const cgiarg::arguments_ &A );



/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
