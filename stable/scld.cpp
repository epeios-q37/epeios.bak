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

#define SCLD_COMPILATION_

#include "scld.h"

#include "sclm.h"

#include "csdles.h"

using namespace scld;

namespace {
	bso::bool__ IsInitialized_ = false;

	typedef csdleo::callback__ _callback__;

	class callback__
	: public _callback__
	{
	protected:
		virtual void CSDLEOInitialize( const csdleo::shared_data__ *Data ) override
		{
		qRH
			fnm::name___ Directory;
			TOL_CBUFFER___ Buffer;
			str::string Error;
		qRB
			if ( Data == NULL )
				qRFwk();

			if ( IsInitialized_ )
				qRFwk();

			if ( true )	{

				// Does not work when placed in 'global_cdtor'.
				Directory.Init();
				fnm::GetLocation( Data->LibraryLocationAndName, Directory );
				sclm::Initialize( *(sclm::sRack *)Data->UP, Directory.UTF8( Buffer ), SCLDInfo() );
				IsInitialized_ = true;
			}
		qRR
		qRT
		qRE
		}
		virtual csdscb::callback__ *CSDLEORetrieveCallback(
			csdleo::context__ Context,
			csdleo::mode__ Mode )  override
		{
			return SCLDGetCallback( Context, Mode );
		}
	} _Callback;
}

csdleo::callback__ &csdles::CSDLESRetrieveCallback( void )
{
	_Callback.Init();

	return _Callback;
}
