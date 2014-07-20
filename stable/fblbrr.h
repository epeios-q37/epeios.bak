/*
	Header for the 'fblbrr' library by Claude SIMON (csimon at zeusw dot org)
	Copyright (C) 2004 Claude SIMON.

	This file is part of the Epeios (http://zeusw.org/epeios/) project.

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
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

//	$Id: fblbrr.h,v 1.10 2013/04/15 10:50:50 csimon Exp $

#ifndef FBLBRR__INC
#define FBLBRR__INC

#define FBLBRR_NAME		"FBLBRR"

#define	FBLBRR_VERSION	"$Revision: 1.10 $"

#define FBLBRR_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &FBLBRRTutor;

#if defined( E_DEBUG ) && !defined( FBLBRR_NODBG )
#define FBLBRR_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.10 $
//C Claude SIMON (csimon at zeusw dot org)
//R $Date: 2013/04/15 10:50:50 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D Frontend/Backend Layout Backend Remote Request 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

# include "err.h"
# include "flw.h"
# include "flx.h"
# include "fblbrq.h"


namespace fblbrr {
	using namespace fblbrq;

	struct parameter__
	{
		void *Content;
		cast__ Cast;
		void reset( bso::bool__ = true )
		{
			Content = NULL;
			Cast = c_Undefined;
		}
		parameter__( cast__ Cast = c_Undefined )
		{
			reset( false );

			this->Cast = Cast;
		}
		~parameter__( void )
		{
			reset();
		}
		void Init(
			void *Content,
			cast__ Cast )
		{
			reset();

			this->Content = Content;
			this->Cast = Cast;
		}
	};

	typedef bch::E_BUNCH_( parameter__ ) parameters_;
	E_AUTO( parameters );


	class remote_callbacks___
	: public callbacks__
	{
	private:
		parameters _Parameters;
		flx::size_embedded_iflow___ _IFlow;
		void *_Get(
			sdr::row__ Row,
			cast__ Cast )
		{
			parameter__ Parameter = _Parameters( Row );

			if ( Parameter.Cast != Cast )
				ERRPrm();

			return Parameter.Content;
		}
	protected:
		virtual void FBLBRQPopIn(
			sdr::row__ CRow,
			flw::iflow__ &Flow,
			cast__ Cast );
		virtual void FBLBRQPopInEnd(
			sdr::row__ CRow,
			flw::iflow__ &Flow );
		virtual void FBLBRQPopOut(
			sdr::row__ CRow,
			flw::iflow__ &Flow,
			cast__ Cast );
		virtual void FBLBRQPush(
			bso::bool__ FirstCall,
			const casts_ &Casts,
			flw::oflow__ &Flow );
		virtual const void *FBLBRQGet(
			sdr::row__ Row,
			cast__ Cast )
		{
			return _Get( Row, Cast );
		}
		virtual void *FBLBRQPut(
			sdr::row__ Row,
			cast__ Cast )
		{
			return _Get( Row, Cast );
		}
		flw::iflow__ &FBLBRQGetFlow( sdr::row__ Row )
		{
			return *(flw::iflow__ *)_Get( Row, cFlow );
		}
		void FBLBRQPutFlow(
			sdr::row__ Row,
			flw::iflow__ &Flow )
		{
			parameter__ Parameter;

			Parameter.Init( &Flow, cFlow );

			if ( _Parameters.Append( Parameter) != Row )
				ERRFwk();
		}
	public:
		void reset( bso::bool__ P = true )
		{
			callbacks__::reset( P );
			_Parameters.reset( P );
			_IFlow.reset( P );
		}
		remote_callbacks___( void ) 
		{
			reset( false );
		}
		virtual ~remote_callbacks___( void ) 
		{
			reset();
		}
		void Init( void )
		{
			reset();

			_Parameters.Init();
			callbacks__::Init();
			// '_IFlow(Driver)' initialisé seulement lorsque utilisé.
		}
	};
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
