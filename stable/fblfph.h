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

//D Frontend/Backend Layout Frontend Parameters Handling 

#ifndef FBLFPH__INC
# define FBLFPH__INC

# define FBLFPH_NAME		"FBLFPH"

# if defined( E_DEBUG ) && !defined( FBLFPH_NODBG )
#  define FBLFPH_DBG
# endif

# include "fblcst.h"

# include "err.h"
# include "flw.h"

namespace fblfph {
	struct callbacks__
	{
	protected:
		virtual void FBLFPHPreProcess( void ) = 0;
		virtual void FBLFPHIn(
			fblcst::cast__ Cast,
			const void *Pointer,
			flw::ioflow__ &Channel ) = 0;
		virtual void FBLFPHOut(
			flw::ioflow__ &Channel,
			fblcst::cast__ Cast,
			void *Pointer ) = 0;
		virtual void FBLFPHFlowIn(
			bso::bool__ FirstCall,
			flw::iflow__ &Flow,
			flw::ioflow__ &Channel ) = 0;
		virtual void FBLFPHFlowOut(
			flw::ioflow__ &Channel,
			flw::iflow__ *&Flow ) = 0;
		virtual void FBLFPHPostProcess( flw::ioflow__ &Flow ) = 0;
	public:
		void reset( bso::bool__ = true )
		{
			// A des fins de standadisation.
		}
		callbacks__( void )
		{
			reset( false );
		}
		virtual ~callbacks__( void )
		{
			reset();
		}
		void Init( void )
		{
			// A des fins de standadisation.
		}
		void PreProcess( void )
		{
			FBLFPHPreProcess();
		}
		void In(
			fblcst::cast__ Cast,
			const void *Pointer,
			flw::ioflow__ &Channel )
		{
			FBLFPHIn( Cast, Pointer, Channel );
		}
		void Out(
			flw::ioflow__ &Channel,
			fblcst::cast__ Cast,
			void *Pointer )
		{
			FBLFPHOut( Channel, Cast, Pointer );
		}
		void FlowIn(
			bso::bool__ FirstCall,
			flw::iflow__ &Flow,
			flw::ioflow__ &Channel )
		{
			FBLFPHFlowIn( FirstCall, Flow, Channel );
		}
		void FlowOut(
			flw::ioflow__ &Channel,
			flw::iflow__ *&Flow )
		{
			FBLFPHFlowOut( Channel, Flow );
		}
		void PostProcess( flw::ioflow__ &Flow )
		{
			FBLFPHPostProcess( Flow );
		}
	};

}

#endif
