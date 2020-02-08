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

/**************/
/**** OLD *****/
/**************/

namespace tagsbs {
	E_CDEF( char, DefaultTagMarker, '%' );

	typedef bso::u8__ indice__;

	// '%1', '%2'... (en supposant '%' comme marqueur).
	bso::bool__ SubstituteShortTag(
		flw::iflow__ &IFlow,
		indice__ Indice,
		const str::string_ &Value,
		flw::oflow__ &OFlow,
		char TagMarker = DefaultTagMarker );

	// '%1', '%2'... (en supposant '%' comme marqueur).
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
		flw::iflow__ &IFlow,
		const str::string_ &Tag,
		const str::string_ &Value,
		flw::oflow__ &OFlow,
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

	class short_tags_callback__
	{
	protected:
		virtual bso::bool__ TAGSBSGetTagValue(
			indice__ Indice,
			str::string_ &Value ) = 0;
	public:
		void reset( bso::bool__ = true )
		{
			// Standardisation.
		}
		E_CVDTOR( short_tags_callback__ )
		void Init( void )
		{
			// Standadisation.
		}
		bso::bool__ GetTagValue(
			indice__ Indice,
			str::string_ &Value )
		{
			return TAGSBSGetTagValue( Indice, Value );
		}
	};

	bso::bool__ SubstituteShortTags(
		flw::iflow__ &IFlow,
		short_tags_callback__ &Callback,
		flw::oflow__ &OFlow,
		char TagMarker = DefaultTagMarker );

	tol::E_XROW  SubstituteShortTags(
		const str::string_ &String,
		short_tags_callback__ &Callback,
		str::string_ &Result,
		char TagMarker = DefaultTagMarker );

	tol::E_XROW  SubstituteShortTags(
		str::string_ &String,
		short_tags_callback__ &Callback,
		char TagMarker = DefaultTagMarker );

	bso::bool__ SubstituteShortTags(
		flw::iflow__ &IFlow,
		const str::strings_ &Values,
		flw::oflow__ &OFlow,
		char TagMarker = DefaultTagMarker );

	tol::E_XROW  SubstituteShortTags(
		const str::string_ &String,
		const str::strings_ &Values,
		str::string_ &Result,
		char TagMarker = DefaultTagMarker );

	tol::E_XROW  SubstituteShortTags(
		str::string_ &String,
		const str::strings_ &Values,
		char TagMarker = DefaultTagMarker );

    void GetLongTags(
        flw::rRFlow &Flow,
        str::dStrings &Tags,
		bso::sChar TagMarker = DefaultTagMarker );

    void GetLongTags(
        const str::dString &String,
        str::dStrings &Tags,
		bso::sChar TagMarker = DefaultTagMarker );

	class long_tags_callback__
	{
	protected:
        // Return false for unexpected tag. Aborts then
        // the substitution and reports failre to caller.
		virtual bso::bool__ TAGSBSGetTagValue(
			const str::string_ &Tag,
			str::string_ &Value ) = 0;
	public:
		void reset( bso::bool__ = true )
		{
			// Standardisation.
		}
		E_CVDTOR( long_tags_callback__ )
		void Init( void )
		{
			// Standadisation.
		}
		bso::bool__ GetTagValue(
			const str::string_ &Tag,
			str::string_ &Value )
		{
			return TAGSBSGetTagValue( Tag, Value );
		}
	};

	// '%TagName%", en prenant '%' comme marqueur.
	bso::bool__ SubstituteLongTags(
		flw::iflow__ &IFlow,
		long_tags_callback__ &Callback,
		flw::oflow__ &OFlow,
		char TagMarker = DefaultTagMarker);	// Si la valeur retourne != 'qNIL', elle indique la position problmatique dans la chane.

	// '%TagName%", en prenant '%' comme marqueur.
	tol::E_XROW SubstituteLongTags(
		const str::string_ &String,
		long_tags_callback__ &Callback,
		str::string_ &Result,
		char TagMarker = DefaultTagMarker);	// Si la valeur retourne != 'qNIL', elle indique la position problmatique dans la chane.

	tol::E_XROW SubstituteLongTags(
		str::string_ &String,
		long_tags_callback__ &Callback,
		char TagMarker = DefaultTagMarker);	// Si la valeur retourne != 'qNIL', elle indique la position problmatique dans la chane.

	bso::bool__ SubstituteLongTags(
		flw::iflow__ &IFlow,
		const str::strings_ &Tags,
		const str::strings_ &Values,
		flw::oflow__ &OFlow,
		char TagMarker = DefaultTagMarker);// Si la valeur retourne != 'qNIL', elle indique la position problmatique dans la chane.

	tol::E_XROW SubstituteLongTags(
		const str::string_ &String,
		const str::strings_ &Tags,
		const str::strings_ &Values,
		str::string_ &Result,
		char TagMarker = DefaultTagMarker);// Si la valeur retourne != 'qNIL', elle indique la position problmatique dans la chane.

	tol::E_XROW SubstituteLongTags(
		str::string_ &String,
		const str::strings_ &Tags,
		const str::strings_ &Values,
		char TagMarker = DefaultTagMarker);// Si la valeur retourne != 'qNIL', elle indique la position problmatique dans la chane.

	class tvalues_	// Tagged values.
	{
	public:
		struct s {
			str::strings_::s
				Tags,
				Values;
		};
		str::strings_
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
		flw::iflow__ &IFlow,
		const tvalues_ &TaggedValues,
		flw::oflow__ &OFlow,
		char TagMarker = DefaultTagMarker )
	{
		return SubstituteLongTags( IFlow, TaggedValues.Tags, TaggedValues.Values, OFlow, TagMarker );
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
	typedef long_tags_callback__ cLongTagCallback;
}

#endif
