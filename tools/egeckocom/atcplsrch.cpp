/*
	'atcplrslt.cpp' by Claude SIMON (http://q37.info/contact/).

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

// $Id: registry.cpp,v 1.5 2012/02/24 18:50:15 csimon Exp $

#include "atcplsrch.h"

using namespace atcplsrch;

NS_IMPL_ISUPPORTS2( atcplsrch::autocomplete_search__, atcplsrch::autocomplete_search__, nsIAutoCompleteSearch );

NS_IMETHODIMP atcplsrch::autocomplete_search__::StartSearch(
	const nsAString & searchString,
	const nsAString & RawSearchParam,
	nsIAutoCompleteResult *previousResult,
	nsIAutoCompleteObserver *listener)
{
ERRProlog
	sdr::row__ Error = E_NIL;
	str::string SearchParam;
ERRBegin
	SearchParam.Init();

	nsxpcm::Transform( RawSearchParam, SearchParam );

	_Result = (nsxpcm::autocomplete_result___ *)SearchParam.ToPointer( &Error );

	if ( Error != E_NIL )
		ERRFwk();

	_Result->SetSearchString( searchString );

	listener->OnSearchResult( this, _Result );
ERRErr
ERREnd
ERREpilog
    return NS_OK;
}

/* void stopSearch (); */
NS_IMETHODIMP atcplsrch::autocomplete_search__::StopSearch()
{
    return NS_OK;
}
