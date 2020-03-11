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

// Client-Server Devices Universal CLient

#ifndef CSDUCL_INC_
# define CSDUCL_INC_

# define CSDUCL_NAME		"CSDUCL"

# define	CSDUCL_VERSION	"$Revision: 1.37 $"

# define CSDUCL_OWNER		"Claude SIMON (http://zeusw.org/intl/contact.html)"

# if defined( E_DEBUG ) && !defined( CSDUCL_NODBG )
#  define CSDUCL_DBG
# endif

# include "err.h"
# include "flw.h"
# include "csdlec.h"
# include "csdrcu.h"

# define CSDUCL_CACHE_SIZE	1000

/***************/
/***** OLD *****/
/***************/

namespace csducl {
	enum type__ {
		tNone,		// No server.
		tLibrary,	// Library embedded  server,
		tRemote,	// Remote server (accessed throught a plugin).
		t_amount,
		t_Undefined
	};

	const char *GetLabel( type__ Type );

	type__ GetType( const str::string_ &Pattern );

	class universal_client_core___
	{
	private:
		type__ _Type;
		str::string _Location;
		csdlec::library_embedded_client_core__ _LibraryAccess;
		csdrcu::core___ _RemoteAccess;
	public:
		void reset( bso::bool__ P = true )
		{
			_LibraryAccess.reset( P );
			_RemoteAccess.reset( P );
			_Type = t_Undefined;
			_Location.reset( P );
		}
		qCDTOR( universal_client_core___ );
		bso::bool__ InitNone( void )
		{
			_Type = tNone;
			_Location.Init();

			return true;
		}
		bso::bool__ InitLibrary(
			const str::string_ &LibraryPath,
			csdlec::library_data__ &LibraryData );
		sdr::sRow InitRemote(
			const str::string_ &PluginPath,
			const char *PluginIdentifier,
			const str::string_ &Parameters,
			const plgn::dAbstracts &Abstracts );
		type__ GetType( void ) const
		{
			return _Type;
		}
		const char *RemoteIdentifier( void )
		{
			return _RemoteAccess.Identifier();
		}
		const char *RemoteDetails( void )
		{
			return _RemoteAccess.Details();
		}
		E_RODISCLOSE__( str::string_, Location );
		friend class universal_client_ioflow_driver___;
	};

	class universal_client_ioflow_driver___
	: public fdr::ioflow_driver___<>
	{
	private:
		csdlec::library_embedded_client_ioflow___ _LibraryFlow;
		csdrcu::client__ _RemoteFlow;
		universal_client_core___ *_Core;
		flw::ioflow__ &_Get( void )
		{
			switch ( _Core->_Type ) {
			case tNone:
				qRFwk();	// Should not be called.
				break;
			case tLibrary:
				return _LibraryFlow;
				break;
			case tRemote:
				return _RemoteFlow;
				break;
			default:
				qRFwk();
				break;
			}

			qRFbd();	// To be sure that we never execute line below.
			return _RemoteFlow;	// It doesn't matter that _'RemoteFlow' is not set, because this code is never executed; it's only here to avoid a warning.
		}
	protected:
		virtual fdr::size__ FDRRead(
			fdr::size__ Maximum,
			fdr::byte__ *Buffer ) override
		{
			return _Get().ReadUpTo( Maximum, Buffer );
		}
		virtual bso::sBool FDRDismiss(
            bso::sBool Unlock,
            qRPN ) override
		{
			return _Get().Dismiss( Unlock, ErrHandling );
		}
		virtual fdr::sTID FDRRTake( fdr::sTID Owner ) override
		{
			return _Get().RDriver().RTake( Owner );
		}
		virtual fdr::size__ FDRWrite(
			const fdr::byte__ *Buffer,
			fdr::size__ Maximum ) override
		{
			return _Get().WriteUpTo( Buffer, Maximum );
		}
		virtual bso::sBool FDRCommit(
            bso::sBool Unlock,
            qRPN ) override
		{
			return _Get().Commit( Unlock, ErrHandling );
		}
		virtual fdr::sTID FDRWTake( fdr::sTID Owner ) override
		{
			return _Get().WDriver().WTake( Owner );
		}
	public:
		void reset( bso::bool__ P = true )
		{
			fdr::ioflow_driver___<>::reset( P );
			_LibraryFlow.reset( P );
			_RemoteFlow.reset( P );

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
			universal_client_core___ &Core,
			fdr::thread_safety__ ThreadSafety )
		{
			reset();

			_Core = &Core;

			switch ( Core._Type ) {
			case tNone:
				break;
			case tLibrary:
				_LibraryFlow.Init( Core._LibraryAccess );
				break;
			case tRemote:
				_RemoteFlow.Init( Core._RemoteAccess );
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
		flw::byte__ _Cache[CSDUCL_CACHE_SIZE];
	public:
		void reset( bso::bool__ P = true )
		{
			ioflow__::reset( P );
			_Driver.reset( P );
		}
		universal_client_ioflow___( void )
		{
			reset( false );
		}
		~universal_client_ioflow___( void )
		{
			reset();
		}
		void Init( universal_client_core___ &Core )
		{
			reset();

			_Driver.Init( Core, fdr::tsDisabled );
			ioflow__::Init( _Driver, _Cache, sizeof( _Cache ) );
		}
	};
}

/***************/
/***** NEW *****/
/***************/

namespace csducl {
	typedef universal_client_core___ rCore;
	typedef universal_client_ioflow___ rIOFlow;
	typedef universal_client_ioflow_driver___ rIOFlowDriver;
}

#endif
