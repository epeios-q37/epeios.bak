/*
	Header for the 'csducl' library by Claude SIMON (http://zeusw.org/intl/contact.html)
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

//	$Id: csducl.h,v 1.37 2013/04/09 17:57:28 csimon Exp $

#ifndef CSDUCL__INC
#define CSDUCL__INC

#define CSDUCL_NAME		"CSDUCL"

#define	CSDUCL_VERSION	"$Revision: 1.37 $"

#define CSDUCL_OWNER		"Claude SIMON (http://zeusw.org/intl/contact.html)"

#if defined( E_DEBUG ) && !defined( CSDUCL_NODBG )
#define CSDUCL_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.37 $
//C Claude SIMON (http://zeusw.org/intl/contact.html)
//R $Date: 2013/04/09 17:57:28 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D Client-Server Devices Universal CLient 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"
#include "csdsnc.h"
#include "csdlec.h"

#define CSDUCL_CACHE_SIZE	1000

namespace csducl {
	enum type__ {
		tDaemon,
		tPlugin,
		t_amount,
		t_Undefined
	};

	const char *GetLabel( type__ Type );

	type__ GetType( const str::string_ &Pattern );

	class universal_client_core
	{
	private:
		type__ _Type;
		csdsnc::core _DaemonAccess;
		csdlec::library_embedded_client_core__ _LibraryAccess;
	public:
		void reset( bso::bool__ P = true )
		{
			_LibraryAccess.reset( P );
			_DaemonAccess.reset( P );
			_Type = t_Undefined;
		}
		universal_client_core( void )
		{
			reset( false );
		}
		~universal_client_core( void )
		{
			reset();
		}
		bso::bool__ Init(
			const char *Backend,
			csdlec::library_data__ &LibraryData,	// Utilis uniquement si le backend est une bibliothque.
			csdsnc::log_functions__ &Log,
			type__ Type,
			bso::uint__ PingDelay );	// Dlai maximum d'inactivit entre deux 'ping'.
		type__ GetType( void ) const
		{
			return _Type;
		}
		friend class universal_client_ioflow_driver___;
	};

	class universal_client_ioflow_driver___
	: public fdr::ioflow_driver___<>
	{
	private:
		csdsnc::client_ioflow___ _DaemonFlow;
		csdlec::library_embedded_client_ioflow___ _LibraryFlow;
		universal_client_core *_Core;
		flw::ioflow__ &_Get( void )
		{
			switch ( _Core->_Type ) {
			case tDaemon:
				return _DaemonFlow;
				break;
			case tPlugin:
				return _LibraryFlow;
				break;
			default:
				ERRFwk();
				break;
			}

			return *(flw::ioflow__ *)NULL;	// Pour viter un 'warning'.
		}
	protected:
		virtual fdr::size__ FDRRead(
			fdr::size__ Maximum,
			fdr::datum__ *Buffer )
		{
			return _Get().ReadUpTo( Maximum, Buffer );
		}
		virtual void FDRDismiss( void )
		{
			_Get().Dismiss();
		}
		virtual fdr::size__ FDRWrite(
			const fdr::datum__ *Buffer,
			fdr::size__ Maximum )
		{
			return _Get().WriteUpTo( Buffer, Maximum );
		}
		virtual void FDRCommit( void )
		{
			_Get().Commit();
		}
	public:
		void reset( bso::bool__ P = true )
		{
			fdr::ioflow_driver___<>::reset( P );
			_DaemonFlow.reset( P );
			_LibraryFlow.reset( P );

			_Core = NULL;
		}
		universal_client_ioflow_driver___( void )
		{
			reset( true );
		}
		~universal_client_ioflow_driver___( void )
		{
			reset();
		}
		void Init(
			universal_client_core &Core,
			fdr::thread_safety__ ThreadSafety )
		{
			reset();

			_Core = &Core;

			switch ( Core._Type ) {
			case tDaemon:
				_DaemonFlow.Init( Core._DaemonAccess );
				break;
			case tPlugin:
				_LibraryFlow.Init( Core._LibraryAccess );
				break;
			default:
				ERRFwk();
				break;
			}

			fdr::ioflow_driver___<>::Init( ThreadSafety );
		}
	};

	class universal_client_ioflow___
	: public flw::ioflow__
	{
	private:
		universal_client_ioflow_driver___ _Driver;
		flw::datum__ _Cache[CSDUCL_CACHE_SIZE];
	public:
		void reset( bso::bool__ P = true )
		{
			ioflow__::reset( P );
			_Driver.reset();
		}
		universal_client_ioflow___( void )
		{
			reset( false );
		}
		~universal_client_ioflow___( void )
		{
			reset();
		}
		void Init(
			universal_client_core &Core,
			flw::size__ AmountMax = FLW_AMOUNT_MAX )
		{
			reset();

			_Driver.Init( Core, fdr::tsDisabled );
			ioflow__::Init( _Driver, _Cache, sizeof( _Cache ), AmountMax );
		}
	};
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
