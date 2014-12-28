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

// FRontenD INSTanCes

#ifndef FRDINSTC__INC
# define FRDINSTC__INC

# include "frdkernl.h"

namespace frdinstc {

	class user___
	{
	private:
		frdkernl::kernel___ *_Kernel;
		esketch::skt_myobject___ _Object;
		bso::bool__ _TestButtonIsVisible;
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
	public:
		void reset( bso::bool__ P = true )
		{	
			_Kernel = NULL;

			_Object.reset( P );
			_TestButtonIsVisible = false;
		}
		E_CVDTOR( user___ );
		void Init( frdkernl::kernel___ &Kernel )
		{
			_Kernel = &Kernel;

			_Object.Init( Kernel.MyObject );
			_TestButtonIsVisible = false;
		}
		str::string_ &ToUpper( str::string_ &String )
		{
			str::string Result;

			Result.Init();

			_Object.ToUC( String, Result );

			String = Result;

			return String;
		}
		frdkernl::kernel___ &Kernel( void )
		{
			return _K();
		}
		E_RWDISCLOSE__( bso::bool__, TestButtonIsVisible );
	};
}

#endif
