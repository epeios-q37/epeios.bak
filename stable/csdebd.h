/*
	Header for the 'csdebd' library by Claude SIMON (http://zeusw.org/intl/contact.html)
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

//	$Id: csdebd.h,v 1.25 2011/09/23 11:56:44 csimon Exp $

#ifndef CSDEBD__INC
#define CSDEBD__INC

#define CSDEBD_NAME		"CSDEBD"

#define	CSDEBD_VERSION	"$Revision: 1.25 $"

#define CSDEBD_OWNER		"Claude SIMON (http://zeusw.org/intl/contact.html)"

#include "ttr.h"

extern class ttr_tutor &CSDEBDTutor;

#if defined( E_DEBUG ) && !defined( CSDEBD_NODBG )
#define CSDEBD_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.25 $
//C Claude SIMON (http://zeusw.org/intl/contact.html)
//R $Date: 2011/09/23 11:56:44 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D Client-Server EmBeD 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#error "Obsolete ! Use "CSDLEC' instead."

#include "err.h"
#include "flw.h"
#include "csdscm.h"
#include "bch.h"

#define CSDEBD_CACHE_SIZE	1000


namespace csdebd {

	typedef bch::E_BUNCH_( flw::datum__ ) data_;
	E_AUTO( data )

	class _passive_generic_driver___
	: public fdr::ioflow_driver___<>
	{
	private:
		data_ &_Read;
		data_ &_Write;
		epeios::row__ _Row;
	protected:
		virtual fdr::size__ FDRRead(
			fdr::size__ Maximum,
			fdr::datum__ *Buffer )
		{
			if ( _Row == NONE )
				_Row = _Read.First();

			if ( _Row == NONE )
				ERRu();

			if ( ( _Read.Amount() - *_Row ) < (epeios::size__)Maximum )
				Maximum = _Read.Amount() - *_Row;

			_Read.Recall( _Row, Maximum, Buffer );

			_Row = _Read.Next( _Row, Maximum );

/* Concernant GESBIB, si l'on enlève le bloc ci-dessous, le logiciel est susceptible de se planter lorsque l'on manipule
une requête de manière trés intense (bombardage de 'push' 'join'). C'est comme si le 'Dismiss()' n'était pas lancé correctement. */
// Début bloc.
			if ( _Row == NONE )
				_Read.Init();
// Fin bloc.

			return Maximum;
		}
		virtual void FDRDismiss( void )
		{
#ifdef CSDEBD_DBG
			if ( _Row != NONE )
				ERRu();
#endif
			_Read.Init();
		}
		virtual fdr::size__ FDRWrite(
			const fdr::datum__ *Buffer,
			fdr::size__ Maximum )
		{
			_Write.Append( Buffer, Maximum );

			return Maximum;
		}
		virtual void FDRCommit( void )
		{}
	public:
		void reset( bso::bool__ P = true )
		{
			fdr::ioflow_driver___<>::reset( P );

			_Row = NONE;
		}
		_passive_generic_driver___(
			data_ &Read,
			data_ &Write )
		: _Read( Read ),
		  _Write( Write )
		{
			reset( false );
		}
		~_passive_generic_driver___( void )
		{
			reset();
		}
		void Init( fdr::thread_safety__ ThreadSafety )
		{
			reset();
			
			fdr::ioflow_driver___<>::Init( ThreadSafety );
		}
	};

	class _active_generic_driver___
	: public _passive_generic_driver___
	{
	private:
		csdscm::user_functions__ *_UserFunctions;
		void *_UP;
		flw::ioflow__ *_Flow;
		bso::bool__ _DataAvailable;
		void _Create( void )
		{
			_UP = _UserFunctions->PreProcess( "(embed)" );
		}
		void _Delete( void )
		{
			if( _UP != NULL )
				_UserFunctions->PostProcess( _UP );
		}
	protected:
		virtual fdr::size__ FDRWrite(
			const fdr::datum__ *Buffer,
			fdr::size__ Maximum )
		{
			_DataAvailable = true;

			return _passive_generic_driver___::FDRWrite( Buffer, Maximum );
		}
		virtual void FDRCommit( void )
		{
			if ( _DataAvailable )
				_UserFunctions->Process( *_Flow, _UP );

			_DataAvailable = false;
		}
	public:
		void reset( bso::bool__ P = true )
		{
			_passive_generic_driver___::reset( P );

			if ( P )
				_Delete();

			_UserFunctions = NULL;
			_UP = NULL;
			_Flow = NULL;
			_DataAvailable = false;
		}
		_active_generic_driver___(
			data_ &Read,
			data_ &Write )
		: _passive_generic_driver___( Read, Write )
		{
			reset( false );
		}
		~_active_generic_driver___( void )
		{
			reset();
		}
		void Init(
			csdscm::user_functions__ &UserFunctions,
			flw::ioflow__ &Flow,
			fdr::thread_safety__ ThreadSafety )
		{
			reset();

			_UserFunctions = &UserFunctions;
			_Flow = &Flow;

			_passive_generic_driver___::Init( ThreadSafety );

			_Create();
		}
	};


	class embed_client_server_ioflow___
	: public flw::ioflow__
	{
	private:
		flw::datum__ _Cache[CSDEBD_CACHE_SIZE];
		_active_generic_driver___ _Driver;
		data _Master, _Slave;
		struct backend {
			flw::datum__ Cache[CSDEBD_CACHE_SIZE];
			_passive_generic_driver___ Driver;
			flw::ioflow__ Flow;
			void reset( bso::bool__ P = true )
			{
				Driver.reset( P );
			}
			backend(
				data_ &Read,
				data_ &Write )
			: Driver( Read, Write )
			{
				reset( false );
			}
			~backend( void )
			{
				reset();
			}
			void Init( void )
			{
				Driver.Init( fdr::tsDisabled );
				Flow.Init( Driver, Cache, sizeof( Cache ), FLW_SIZE_MAX );
			}
		} _Backend;
	public:
		embed_client_server_ioflow___( void )
		: _Driver( _Master, _Slave ),
		  _Backend( _Slave ,_Master )
		{
			reset( false );
		}
		~embed_client_server_ioflow___( void )
		{
			reset();
		}
		void reset( bso::bool__ P = true )
		{
			_Driver.reset( P );

			_Backend.reset( P );

			_Master.reset( P );
			_Slave.reset( P );
		}
		void Init( csdscm::user_functions__ &UserFunctions)
		{
			reset();
/*
			ioflow__::Init();
			_Backend.Init();
*/
			_Master.Init();
			_Slave.Init();

			_Backend.Init();

			_Driver.Init( UserFunctions, _Backend.Flow, fdr::tsDisabled );
			ioflow__::Init( _Driver, _Cache, sizeof( _Cache ), FLW_SIZE_MAX );
		}
	};
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
