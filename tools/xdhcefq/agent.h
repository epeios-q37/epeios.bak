/*
	Copyright (C) 2014-2016 by Claude SIMON (http://zeusw.org/epeios/contact.html).

    This file is part of 'xdhcefq'.

    'xdhcefq' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'xdhcefq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'xdhcefq'.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef AGENT_INC_
# define AGENT_INC_

# include "misc.h"

# include "xdhups.h"

# include "dlbrry.h"
# include "flx.h"
# undef _UP

namespace agent {
	typedef xdhups::agent___ _agent___;

    class agent___
	: public _agent___
    {
    private:
		xdhups::sSession _Session;
		cef_browser_t *_Browser;
		TOL_CBUFFER___ _LanguageBuffer;
		void _Release( void )
		{
			xdhcmn::cSession *Callback = _Session.Callback();

			if ( Callback != NULL )
				_agent___::ReleaseCallback( Callback );

			if ( _Browser != NULL  )
				misc::Release( _Browser );

			_Browser = NULL;
		}
		cef_browser_t *_B( void ) const
		{
			if ( _Browser == NULL )
				qRGnr();

			return _Browser;
		}
		cef_frame_t *_F( void ) const
		{
			return misc::GetFrame(_B() );
		}
		void _InitializeSession( void );
    public:
        void reset( bso::bool__ P = true )
        {
			if ( P ) {

				_Release();
			}

			_Session.reset( P );
			_agent___::reset( P );
			_Browser = NULL;
			_LanguageBuffer.reset( P );
        }
        E_CDTOR( agent___ );
		bso::bool__ Init(
			cef_browser_t *Browser,
			const str::string_ &ModuleFileName,
			const char *Identification )
		{
			_Release();

			_Browser = Browser;

			misc::AddRef( _Browser );

			if ( _agent___::Init( xdhcmn::mMonoUser, ModuleFileName, Identification ) ) {
				_InitializeSession();
				return true;
			} else
				return false;

		}
		cef_browser_t *Browser( void ) const
		{
			return _B();
		}
		cef_frame_t *Frame( void ) const
		{
			return _F();
		}
		const char *Language( TOL_CBUFFER___ &Buffer )
		{
			return BaseLanguage( Buffer );
		}
		bso::bool__ Launch(
			const char *Id,
			const char *Action )
		{
			return _Session.Launch( Id, Action );
		}
		bso::bool__ HandleEvent( const str::string_ &Digest );	// If returned value == 'true', the event bubbling is stopped.
    };
}

#endif
