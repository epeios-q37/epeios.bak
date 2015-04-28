/*
	'csdlec.cpp' by Claude SIMON (http://zeusw.org/).

	'csdlec' is part of the Epeios framework.

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

#define CSDLEC__COMPILATION

#include "csdlec.h"

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

using namespace csdlec;

# define LOC	cio::COut << txf::nl << "------------------------->" << __LOC__ << txf::nl << txf::commit

bso::bool__ csdlec::library_embedded_client_core__::Init(
	const char *LibraryName,
	library_data__ &Data,
	err::handling__ ERRHandling )
{
	csdleo::shared_data__ SharedData;

	SharedData.Init( Data );

	reset();

	LOC;

	if ( _Library.Init(LibraryName, ERRHandling) ) {
		LOC;

		if ( _RetrieveCallback( &SharedData ) )
			return true;
		else
			reset();	// Sinon le bibliothque n'est pas dcharge correctement  la fermeture de l'application.

		LOC;
	}

	LOC;

	if ( ERRHandling != err::hUserDefined )
		ERRSys();

	return false;
}


extern "C" typedef csdleo::retrieve_callback retrieve_callback;

bso::bool__ csdlec::library_embedded_client_core__::_RetrieveCallback( csdleo::shared_data__ *Data )
{
	retrieve_callback *RetrieveCallback = dlbrry::GetFunction<retrieve_callback *>( E_STRING( CSDLEO_RETRIEVE_CALLBACK_FUNCTION_NAME ), _Library );

	if ( RetrieveCallback == NULL )
		return false;

	csdleo::callback__ &Callback = RetrieveCallback();

	if ( _Callback != NULL )
		ERRFwk();

	Callback.Initialize( Data, NULL );

	if ( ( _Callback = Callback.RetrieveCallback( csdleo::mEmbedded, Data->Context ) ) == NULL )
		return false;

	return true;
}

bso::bool__ csdlec::library_embedded_client_core__::_ReleaseCallback( void )
{
	if ( _Callback != NULL )
		delete _Callback;

	_Callback = NULL;

	return true;
}
