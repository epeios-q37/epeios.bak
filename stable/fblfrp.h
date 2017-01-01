/*
	Copyright (C) 1999-2017 Claude SIMON (http://q37.info/contact/).

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

// Frontend/Backend Layout Frontend Remote Parameters 

#ifndef FBLFRP__INC
# define FBLFRP__INC

# define FBLFRP_NAME		"FBLFRP"

# if defined( E_DEBUG ) && !defined( FBLFRP_NODBG )
#  define FBLFRP_DBG
# endif

# include "fblfph.h"

# include "err.h"
# include "flw.h"
# include "tol.h"
# include "flx.h"

# define FBLFRP__OUT_PARAMETERS_AMOUNT_MAX		20	// Le nombre maximum de paramtres en sortie.

namespace fblfrp {

	struct datum__ {
		fblcst::cast__ Cast;
		void *Pointer;
		datum__( 
			fblcst::cast__ Cast = fblcst::c_Undefined,
			void *Pointer = NULL )
		{
			this->Cast = Cast;
			this->Pointer= Pointer;
		}
	};


	typedef fblfph::callbacks__ _callbacks__;

	class remote_callbacks___
	: public _callbacks__
	{
	private:
		bch::E_BUNCH__( datum__, FBLFRP__OUT_PARAMETERS_AMOUNT_MAX ) _Data;
		flx::size_embedded_iflow___ _IFlow;
	protected:
		virtual void FBLFPHPreProcess( void )
		{
			_Data.Init();
		}
		virtual void FBLFPHIn(
			fblcst::cast__ Cast,
			const void *Pointer,
			flw::ioflow__ &Channel );
		virtual void FBLFPHOut(
			flw::ioflow__ &Channel,
			fblcst::cast__ Cast,
			void *Pointer );
		virtual void FBLFPHFlowIn(
			bso::bool__ FirstCall,
			flw::iflow__ &Flow,
			flw::ioflow__ &Channel );
		virtual void FBLFPHFlowOut(
			flw::ioflow__ &Channel,
			flw::iflow__ *&Flow );
		virtual void FBLFPHPostProcess( flw::ioflow__ &Flow );
	public:
		void reset( bso::bool__ P = true )
		{
			_Data.reset( P );
			_callbacks__::reset( P );
			_IFlow.reset( P );
		}
		E_CVDTOR( remote_callbacks___ );
		void Init( void )
		{
			_Data.Init();
			_callbacks__::Init();
			// '_IFlow' initialis seulement lorsque utilis.
		}
	};
}

#endif
