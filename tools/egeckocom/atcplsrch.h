/*
	'atcplrslt.h' by Claude SIMON (http://zeusw.org/).

	 This file is part of 'eGeckoCOM' software.

    'eGeckoCOM' is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'eGeckoCOM' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with 'eGeckoCOM'.  If not, see <http://www.gnu.org/licenses/>.
*/

// $Id: registry.h,v 1.4 2012/02/24 18:50:15 csimon Exp $

// AuToComPLete SeaRCH.

#ifndef ATCPLSRCH__INC
# define ATCPLSRCH__INC

# include "nsxpcm.h"

# include "geckof.h"

# include "nsIAutoCompleteSearch.h"

# define ATCPLSRCH_AUTOCOMPLETE_SEARCH_IID_STR "6de89116-05d7-4968-a763-1b6d0680a6a6"

# define ATCPLSRCH_AUTOCOMPLETE_SEARCH_IID \
  {0x6de89116, 0x05d7, 0x4968, \
    { 0xa7, 0x63, 0x1b, 0x6d, 0x06, 0x80, 0xa6, 0xa6 }}

namespace atcplsrch {
	class autocomplete_search__
	: public nsIAutoCompleteSearch
	{
	private:
		nsxpcm::autocomplete_result___ *_Result;
	public:
		NS_DECLARE_STATIC_IID_ACCESSOR( ATCPLSRCH_AUTOCOMPLETE_SEARCH_IID )
		NS_DECL_ISUPPORTS
		NS_DECL_NSIAUTOCOMPLETESEARCH
		void reset( bso::bool__ P = true )
		{
			_Result = NULL;
		}
		E_CVDTOR( autocomplete_search__ );
	public:
		void Init( void )
		{
			reset();

			// Standardisation. A priori, jamais appelé.
		}
	};

	NS_GENERIC_FACTORY_CONSTRUCTOR( autocomplete_search__ )
}

NS_DEFINE_STATIC_IID_ACCESSOR( atcplsrch::autocomplete_search__, ATCPLSRCH_AUTOCOMPLETE_SEARCH_IID )

# define ATCPLSRCH_AUTOCOMPLETE_SEARCH_CONTRACTID "@mozilla.org/autocomplete/search;1?name=" GECKOF_AUTOCOMPLETE_COMPONENT_NAME
# define ATCPLSRCH_AUTOCOMPLETE_SEARCH_CLASSNAME "EAutocompleteSearch"
# define ATCPLSRCH_AUTOCOMPLETE_SEARCH_CID  ATCPLSRCH_AUTOCOMPLETE_SEARCH_IID
#endif
