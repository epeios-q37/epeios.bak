/*
	Copyright (C) 1999 Claude SIMON (http://q37.info/contact/).

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

#ifndef TAGSBS_INC_
# define TAGSBS_INC_

# define TAGSBS_NAME		"TAGSBS"

# if defined( E_DEBUG ) && !defined( TAGSBS_NODBG )
#  define TAGSBS_DBG
# endif

// TAG SuBStitution

# include "err.h"
# include "flw.h"
# include "str.h"
# include "sdr.h"
# include "xtf.h"

/**************/
/**** OLD *****/
/**************/

namespace tagsbs {
	E_CDEF( char, DefaultTagMarker, '%' );

	typedef bso::u8__ indice__;

	bso::bool__ SubstituteShortTag(
		xtf::sRFlow &Input,
		indice__ Indice,
		const str::string_ &Value,
		flw::oflow__ &Output,
		char TagMarker = DefaultTagMarker );

	tol::E_XROW SubstituteShortTag(
		const str::string_ &String,
		indice__ Indice,
		const str::string_ &Value,
		str::string_ &Result,
		char TagMarker = DefaultTagMarker );

	tol::E_XROW SubstituteShortTag(
		str::string_ &String,
		indice__ Indice,
		const str::string_ &Value,
		char TagMarker = DefaultTagMarker );

	bso::bool__ SubstituteLongTag(
		xtf::sRFlow &Input,
		const str::string_ &Tag,
		const str::string_ &Value,
		flw::oflow__ &Output,
		char TagMarker = DefaultTagMarker );

	tol::E_XROW SubstituteLongTag(
		const str::string_ &String,
		const str::string_ &Tag,
		const str::string_ &Value,
		str::string_ &Result,
		char TagMarker = DefaultTagMarker );

	tol::E_XROW SubstituteLongTag(
		str::string_ &String,
		const str::string_ &Tag,
		const str::string_ &Value,
		char TagMarker = DefaultTagMarker );

	class cShortTags
	{
	protected:
		virtual bso::bool__ TAGSBSHandleTag(
			indice__ Indice,
			flw::rWFlow &Output ) = 0;
	public:
		qCALLBACK(ShortTags);
		bso::bool__ HandleTag(
			indice__ Indice,
			flw::rWFlow &Output )
		{
			return TAGSBSHandleTag( Indice, Output );
		}
	};

	bso::bool__ SubstituteShortTags(
		xtf::sRFlow &Input,
		cShortTags &Callback,
		flw::oflow__ &OFlow,
		char TagMarker = DefaultTagMarker );

	tol::E_XROW SubstituteShortTags(
		const str::string_ &String,
		cShortTags &Callback,
		str::dString &Result,
		char TagMarker = DefaultTagMarker );

	tol::E_XROW SubstituteShortTags(
		str::string_ &String,
		cShortTags &Callback,
		char TagMarker = DefaultTagMarker );

	bso::bool__ SubstituteShortTags(
		xtf::sRFlow &Input,
		const str::dStrings &Values,
		flw::oflow__ &Output,
		char TagMarker = DefaultTagMarker );

	tol::E_XROW  SubstituteShortTags(
		const str::string_ &String,
		const str::dStrings &Values,
		str::string_ &Result,
		char TagMarker = DefaultTagMarker );

	tol::E_XROW  SubstituteShortTags(
		str::string_ &String,
		const str::dStrings &Values,
		char TagMarker = DefaultTagMarker );

  void GetLongTags(
    xtf::sRFlow &Flow,
    str::dStrings &Tags,
    bso::sChar TagMarker = DefaultTagMarker );

  void GetLongTags(
    const str::dString &String,
    str::dStrings &Tags,
    bso::sChar TagMarker = DefaultTagMarker );

	class cLongTagsString
	{
	protected:
    // Returns false for unexpected tag. Aborts then
    // the substitution and reports failure to caller.
		virtual bso::bool__ TAGSBSHandleTag(
      const str::string_ &Tag,
			flw::rWFlow &Output) = 0;
	public:
		qCALLBACK( LongTagsString );
		bso::bool__ HandleTag(
			const str::string_ &Tag,
			flw::rWFlow &Output)
		{
			return TAGSBSHandleTag(Tag, Output);
		}
	};

	bso::bool__ SubstituteLongTags(
		xtf::sRFlow& Input,
		cLongTagsString &Callback,
		flw::oflow__ &OFlow,
		char TagMarker = DefaultTagMarker);

	// '%TagName%", en prenant '%' comme marqueur.
	tol::E_XROW SubstituteLongTags(
		const str::string_ &String,
		cLongTagsString &Callback,
		str::string_ &Result,
		char TagMarker = DefaultTagMarker);	// Si la valeur retourne != 'qNIL', elle indique la position problmatique dans la chane.

	tol::E_XROW SubstituteLongTags(
		str::string_ &String,
		cLongTagsString &Callback,
		char TagMarker = DefaultTagMarker);	// Si la valeur retourne != 'qNIL', elle indique la position problmatique dans la chane.

 	class cLongTagsRow
	{
	protected:
    // Returns false for unexpected tag. Aborts then
    // the substitution and reports failure to caller.
		virtual bso::bool__ TAGSBSHandleTag(
      sdr::sRow TagRow,
			flw::rWFlow &Output) = 0;
	public:
		qCALLBACK( LongTagsRow );
		bso::bool__ HandleTag(
			sdr::sRow TagRow,
			flw::rWFlow &Output)
		{
			return TAGSBSHandleTag(TagRow, Output);
		}
	};

	bso::bool__ SubstituteLongTags(
		xtf::sRFlow &Input,
		const str::dStrings &Tags,
		cLongTagsRow &Callback,
		flw::oflow__ &OFlow,
		char TagMarker = DefaultTagMarker);// Si la valeur retourne != 'qNIL', elle indique la position problmatique dans la chane.

	tol::E_XROW SubstituteLongTags(
		const str::string_ &String,
		const str::dStrings &Tags,
		cLongTagsRow &Callback,
		str::string_ &Result,
		char TagMarker = DefaultTagMarker);	// Si la valeur retourne != 'qNIL', elle indique la position problmatique dans la chane.

	tol::E_XROW SubstituteLongTags(
		str::string_ &String,
		const str::dStrings &Tags,
		cLongTagsRow &Callback,
		char TagMarker = DefaultTagMarker);	// Si la valeur retourne != 'qNIL', elle indique la position problmatique dans la chane.

	bso::bool__ SubstituteLongTags(
		xtf::sRFlow &Input,
		const str::dStrings &Tags,
		const str::dStrings &Values,
		flw::oflow__ &OFlow,
		char TagMarker = DefaultTagMarker);// Si la valeur retourne != 'qNIL', elle indique la position problmatique dans la chane.

	tol::E_XROW SubstituteLongTags(
		const str::string_ &String,
		const str::dStrings &Tags,
		const str::dStrings &Values,
		str::string_ &Result,
		char TagMarker = DefaultTagMarker);// Si la valeur retourne != 'qNIL', elle indique la position problmatique dans la chane.

	tol::E_XROW SubstituteLongTags(
		str::string_ &String,
		const str::dStrings &Tags,
		const str::dStrings &Values,
		char TagMarker = DefaultTagMarker);// Si la valeur retourne != 'qNIL', elle indique la position problmatique dans la chane.

	class tvalues_	// Tagged values.
	{
	public:
		struct s {
			str::dStrings::s
				Tags,
				Values;
		};
		str::dStrings
			Tags,
			Values;
		tvalues_( s &S )
			: Tags( S.Tags ),
			  Values( S.Values )
		{}
		void reset( bso::bool__ P = true )
		{
			Tags.reset( P );
			Values.reset( P );
		}
		void plug( qASd *AS )
		{
			Tags.plug( AS );
			Values.plug( AS );
		}
		tvalues_ &operator =(const tvalues_ &TV)
		{
			Tags = TV.Tags;
			Values = TV.Values;

			return *this;
		}
		void Init( void )
		{
			Tags.Init();
			Values.Init();
		}
		void Append(
			const str::string_ &Tag,
			const str::string_ &Value )
		{
			if ( Tags.Append( Tag ) != Values.Append( Value ) )
				qRFwk();
		}
		void Append(
			const char *Tag,
			const char *Value )
		{
			Append( str::string(Tag), str::string( Value ) );
		}
		void Append(
			const char *Tag,
			const str::string_ &Value )
		{
			Append( str::string( Tag ), Value );
		}
		bso::sBool IsEmpty( void ) const
		{
			return Tags.Amount() != 0;
		}
		void Append( void )	// Called by below method.
		{}
		template <typename tag1, typename value, typename tag2, typename... others> void Append(	// 'tag1'/'tag2' to avod recusirve calls.
			const tag1 &Tag1,
			const value &Value,
			const tag2 &Tag2,
			const others &...Others )
		{
			Append( Tag1, Value );
			Append( Tag2, Others... );
		}
	};

	E_AUTO( tvalues );

	inline bso::bool__ SubstituteLongTags(
		xtf::sRFlow &Input,
		const tvalues_ &TaggedValues,
		flw::oflow__ &Output,
		char TagMarker = DefaultTagMarker )
	{
		return SubstituteLongTags(Input, TaggedValues.Tags, TaggedValues.Values, Output, TagMarker);
	}

	inline tol::E_XROW SubstituteLongTags(
		const str::string_ &String,
		const tvalues_ &TaggedValues,
		str::string_ &Result,
		char TagMarker = DefaultTagMarker)
	{
		return SubstituteLongTags( String, TaggedValues.Tags, TaggedValues.Values, Result, TagMarker );
	}

	inline tol::E_XROW SubstituteLongTags(
		str::string_ &String,
		const tvalues_ &TaggedValues,
		char TagMarker = DefaultTagMarker)
	{
		return SubstituteLongTags( String, TaggedValues.Tags, TaggedValues.Values, TagMarker );
	}
}

/**************/
/**** NEW *****/
/**************/

namespace tagsbs {
	typedef tvalues_ dTaggedValues;
	qW( TaggedValues );

	typedef indice__ sIndice;
}

#endif
