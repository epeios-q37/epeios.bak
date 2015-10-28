/*
	Copyright (C) 2000-2015 Claude SIMON (http://q37.info/contact/).

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

//	$Id: xxx.h,v 1.9 2012/11/14 16:06:23 csimon Exp $

#ifndef FBLFUP__INC
#define FBLFUP__INC

#define FBLFUP_NAME		"FBLFUP"

#define	FBLFUP_VERSION	"$Revision: 1.9 $"

#define FBLFUP_OWNER		"Claude SIMON"


#if defined( E_DEBUG ) && !defined( FBLFUP_NODBG )
#define FBLFUP_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.9 $
//C Claude SIMON (csimon at zeusw dot org)
//R $Date: 2012/11/14 16:06:23 $

/* End of automatic documentation generation part. */

/* Addendum to the automatic documentation generation part. */
//D Frontend/Backend Layout Frontend Universal Parameters 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

# error "Obsolete !"

# include "fblfep.h"
# include "fblfrp.h"
# include "fblfph.h"
# include "fblovl.h"

# include "err.h"
# include "flw.h"

namespace fblfup {
	using namespace fblovl;

	typedef fblfph::callbacks__ _callbacks__;

	class universal_callbacks__
	: public _callbacks__
	{
	private:
		fblfep::embedded_parameters_base__ _Embedded;
		fblfrp::remote_parameters_base__ _Remote;
		mode__ _Mode;
	protected:
		virtual void FBLFPHPreProcess( void )
		{
			switch ( _Mode ) {
			case mEmbedded:
				_Embedded.PreProcess();
				break;
			case mRemote:
				_Remote.PreProcess();
				break;
			default:
				qRFwk();
				break;
			}
		}
		virtual void FBLFPHIn(
			fblcst::cast__ Cast,
			const void *Pointer,
			flw::ioflow__ &Channel )
		{
			switch ( _Mode ) {
			case mEmbedded:
				_Embedded.In( Cast, Pointer, Channel );
				break;
			case mRemote:
				_Remote.In( Cast, Pointer, Channel );
				break;
			default:
				qRFwk();
				break;
			}
		}
		virtual void FBLFPHOut(
			flw::ioflow__ &Channel,
			fblcst::cast__ Cast,
			void *Pointer )
		{
			switch ( _Mode ) {
			case mEmbedded:
				_Embedded.Out( Channel, Cast, Pointer );
				break;
			case mRemote:
				_Remote.Out( Channel, Cast, Pointer );
				break;
			default:
				qRFwk();
				break;
			}
		}
		virtual void FBLFPHFlowIn(
			bso::bool__ FirstCall,
			flw::iflow__ &Flow,
			flw::ioflow__ &Channel )
		{
			switch ( _Mode ) {
			case mEmbedded:
				_Embedded.FlowIn( Flow, Channel );
				break;
			case mRemote:
				_Remote.FlowIn( Flow, Channel );
				break;
			default:
				qRFwk();
				break;
			}
		}
		virtual void FBLFPHFlowOut(
			flw::ioflow__ &Channel,
			flw::iflow__ *&Flow )
		{
			switch ( _Mode ) {
			case mEmbedded:
				_Embedded.FlowOut( Channel, Flow );
				break;
			case mRemote:
				_Remote.FlowOut( Channel, Flow );
				break;
			default:
				qRFwk();
				break;
			}
		}
		virtual void FBLFPHPostProcess( flw::ioflow__ &Flow )
		{
			switch ( _Mode ) {
			case mEmbedded:
				_Embedded.PostProcess( Flow );
				break;
			case mRemote:
				_Remote.PostProcess( Flow );
				break;
			default:
				qRFwk();
				break;
			}
		}
	public:
		void reset( bso::bool__ P = true )
		{
			_Embedded.reset( P );
			_Remote.reset( P );
			_callbacks__::reset( P );
			_Mode = m_Undefined;
		}
		E_CVDTOR( universal_callbacks__ );
		void Init( mode__ Mode )
		{
			reset();

			switch ( Mode ) {
			case mEmbedded:
				_Embedded.Init();
				break;
			case mRemote:
				_Remote.Init();
				break;
			default:
				qRFwk();
				break;
			}

			_Mode = Mode;

			_callbacks__::Init();
		}
	};

}

/*$END$*/
#endif
