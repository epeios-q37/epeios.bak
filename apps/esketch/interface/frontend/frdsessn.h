/*
	'frdrinstc.h' by Claude SIMON (http://zeusw.org/).

	 This file is part of 'eSketch' software.

    'eSketch' is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'eSketch' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with 'eSketch'.  If not, see <http://www.gnu.org/licenses/>.
*/

// $Id: frdrgstry.h,v 1.2 2013/02/06 09:08:11 csimon Exp $

#ifndef FRDINSTC__INC
# define FRDINSTC__INC

# include "frdkernl.h"

# include "frdssn.h"

namespace frdsessn {

	typedef frdssn::session___ _session___;

	class session___
	: public _session___
	{
	private:
		frdkernl::kernel___ *_Kernel;
		esketch::skt_myobject___ _Object;
		const frdkernl::kernel___ &_K( void ) const
		{
			if ( _Kernel == NULL )
				ERRFwk();

			return *_Kernel;
		}
		frdkernl::kernel___ &_K( void )
		{
			if ( _Kernel == NULL )
				ERRFwk();

			return *_Kernel;
		}
	protected:
		virtual void FRDSSNOpen( const char *Language ) override
		{
			_Object.Init( _Kernel->MyObject );
		}
		virtual void FRDSSNClose( void ) override
		{
			_Object.reset();
		}
	public:
		void reset( bso::bool__ P = true )
		{
			_Kernel = NULL;
			_session___::reset( P );
		}
		E_CVDTOR( session___ );
		void Init( frdkernl::kernel___ &Kernel )
		{
			_Kernel = &Kernel;
			_session___::Init( Kernel );
		}
	};
}

#endif
