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

// Client-Server Devices Universal CLient 

#ifndef CSDUCL__INC
# define CSDUCL__INC

# define CSDUCL_NAME		"CSDUCL"

# define	CSDUCL_VERSION	"$Revision: 1.37 $"

# define CSDUCL_OWNER		"Claude SIMON (http://zeusw.org/intl/contact.html)"

# if defined( E_DEBUG ) && !defined( CSDUCL_NODBG )
#  define CSDUCL_DBG
# endif

# include "err.h"
# include "flw.h"
# include "csdsnc.h"
# include "csdlec.h"

# define CSDUCL_CACHE_SIZE	1000

namespace csducl {
	enum type__ {
		tNone,	// Pas de backend.
		tDaemon,
		tLibrary,
		t_amount,
		t_Undefined
	};

	const char *GetLabel( type__ Type );

	type__ GetType( const str::string_ &Pattern );

	struct features___
	{
	public:
		str::string Location;
		csducl::type__ Type;
		bso::uint__ PingDelay;
		void reset( bso::bool__ P = true )
		{
			Location.reset( P );
			Type = csducl::t_Undefined;
			PingDelay = 0;
		}
		E_CDTOR( features___ )
		void Init( void )
		{
			Location.Init();
			Type = csducl::t_Undefined;
			PingDelay = 0;
		}
	};

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
			const features___ &Features,
			csdlec::library_data__ &LibraryData,
			csdsnc::log_callback__ *Log = NULL );
		bso::bool__ Init(
			const features___ &Features,
			csdlec::library_data__ &LibraryData,
			csdsnc::log_callback__ &Log )
		{
			return Init( Features, LibraryData, &Log );
		}
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
			case tNone:
				qRFwk();	// Should not be called.
				break;
			case tDaemon:
				return _DaemonFlow;
				break;
			case tLibrary:
				return _LibraryFlow;
				break;
			default:
				qRFwk();
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
			case tLibrary:
				_LibraryFlow.Init( Core._LibraryAccess );
				break;
			default:
				qRFwk();
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

#endif
