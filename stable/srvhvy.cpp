/*
	'srvhvy' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Requires the 'srvhvy' header file ('srvhvy.h').
	Copyright (C) 2004 Claude SIMON (http://zeusw.org/intl/contact.html).

	This file is part of the Epeios (http://zeusw.org/epeios/) project.

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 3
	of the License, or (at your option) any later version.
 
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, go to http://www.fsf.org/
	or write to the:
  
         	         Free Software Foundation, Inc.,
           59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#define SRVHVY__COMPILATION

#include "srvhvy.h"

using namespace srvhvy;

#define CASE( n )\
	case l##n:\
		return #n;\
		break


const char *srvhvy::GetLogLabel( log__ Log )
{
	switch ( Log ) {
		CASE( New );
		CASE( Store );
		CASE( TestAndGet );
		CASE( Delete );
	default:
		ERRu();
		return NULL;	// Pour viter un 'warning'.
		break;
	}
}

namespace {

	class functions__
	: public srv::flow_functions__
	{
	private:
		core_ *_Core;
		user_functions__ *_Functions;
	protected:
		virtual void *SRVPreProcess( flw::ioflow__ &Flow )
		{
			return NULL;
		}
		virtual action__ SRVProcess(
			flw::ioflow__ &Flow,
			void *UP )
		{
#ifdef SRVHVY_DBG
			if ( UP != NULL )
				ERRc();
#endif
			id__ Id = SRVHVY_UNDEFINED;
			action__ Action = aContinue;

			UP = NULL;

			Flow.Read( sizeof( Id ), &Id );

			if ( Id == SRVHVY_UNDEFINED ) {
				Id = _Core->New();
				Flow.Write( &Id, sizeof( Id ) );
				UP = _Functions->PreProcess( Flow );
				_Core->Store( UP, Id );
				_Functions->Process( Flow, UP );
			} else {
				if ( !_Core->TestAndGet( Id, UP ) ) {
					Flow.Put( (flw::datum__)-1 );
					Flow.Synchronize();
					Action = aStop;
				} else {
					Flow.Put( 0 );
					Action = _Functions->Process( Flow, UP );
				}

				switch ( Action ) {
				case aContinue:
					break;
				case aStop:
					if ( UP != NULL )
						_Functions->PostProcess( UP );
					if ( Id != SRVHVY_UNDEFINED )
						_Core->Delete( Id );
					break;
				default:
					ERRu();
					break;
				}
			}

			return aContinue;
		}
		virtual void SRVPostProcess( void *UP )
		{
#ifdef SRVHVY_DBG
			ERRc();
#endif
		}
	public:
		void Init(
			core_ &Core,
			user_functions__ &Functions )
		{
			_Core = &Core;
			_Functions = &Functions;
		}
	};
}

void srvhvy::server___::Process(
		service__ Service,
		user_functions__ &UFunctions,
		core_ &Core )
{
	functions__ Functions;

	Functions.Init( Core, UFunctions );

	_Server.Init( Service );

	_Server.Process(  Functions );
}
