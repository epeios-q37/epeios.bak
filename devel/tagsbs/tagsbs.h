/*
	'tagsbs.h' by Claude SIMON (http://zeusw.org/).

	'tagsbs' is part of the Epeios framework.

    The Epeios framework is free software: you can redistribute it and/or
	modify it under the terms of the GNU General Public License as published
	by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Epeios framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The Epeios framework.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef TAGSBS__INC
# define TAGSBS__INC

# define TAGSBS_NAME		"TAGSBS"

# if defined( E_DEBUG ) && !defined( TAGSBS_NODBG )
#  define TAGSBS_DBG
# endif

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

// TAG SuBStitution

# include "err.h"
# include "flw.h"
# include "str.h"
# include "stsfsm.h"
# include "sdr.h"

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
		const bso::bool__ GetTagValue(
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

	class long_tags_callback__
	{
	protected:
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
		const bso::bool__ GetTagValue(
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
		char TagMarker = DefaultTagMarker);	// Si la valeur retournée != 'E_NIL', elle indique la position problématique dans la chaîne.

	// '%TagName%", en prenant '%' comme marqueur.
	tol::E_XROW SubstituteLongTags(
		const str::string_ &String,
		long_tags_callback__ &Callback,
		str::string_ &Result,
		char TagMarker = DefaultTagMarker);	// Si la valeur retournée != 'E_NIL', elle indique la position problématique dans la chaîne.

	bso::bool__ SubstituteLongTags(
		flw::iflow__ &IFlow,
		const str::strings_ &Tags,
		const str::strings_ &Values,
		flw::oflow__ &OFlow,
		char TagMarker = DefaultTagMarker);// Si la valeur retournée != 'E_NIL', elle indique la position problématique dans la chaîne.

	tol::E_XROW SubstituteLongTags(
		const str::string_ &String,
		const str::strings_ &Tags,
		const str::strings_ &Values,
		str::string_ &Result,
		char TagMarker = DefaultTagMarker);// Si la valeur retournée != 'E_NIL', elle indique la position problématique dans la chaîne.
}

				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
