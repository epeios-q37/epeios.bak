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

#ifndef CSDLEC__INC
# define CSDLEC__INC

# define CSDLEC_NAME		"CSDLEC"

# if defined( E_DEBUG ) && !defined( CSDLEC_NODBG )
#  define CSDLEC_DBG
# endif

// Client-Server Devices Library Embedded Client

# include "err.h"
# include "flw.h"
# include "bch.h"
# include "dir.h"

#include "dlbrry.h"

# include "csdleo.h"

# define CSDLEC_CACHE_SIZE	1000

namespace csdlec {

	typedef csdleo::data__ library_data__;

	typedef bch::E_BUNCH_( flw::byte__ ) data_;
	E_AUTO( data )

	class _passive_generic_driver___
	: public fdr::ioflow_driver___<>
	{
	private:
		data_ &_Read;
		data_ &_Write;
		sdr::row__ _Row;
	protected:
		virtual fdr::size__ FDRRead(
			fdr::size__ Maximum,
			fdr::byte__ *Buffer ) override
		{
			if ( _Row == qNIL )
				_Row = _Read.First();

			if ( _Row == qNIL )
				qRFwk();

			if ( ( _Read.Amount() - *_Row ) < (sdr::size__)Maximum )
				Maximum = _Read.Amount() - *_Row;

			_Read.Recall( _Row, Maximum, Buffer );

			_Row = _Read.Next( _Row, Maximum );

/* Concernant GESBIB, si l'on enlève le bloc ci-dessous, le logiciel est susceptible de se planter lorsque l'on manipule
une requte de manire trés intense (bombardage de 'push' 'join'). C'est comme si le 'Dismiss()' n'tait pas lancé correctement. */
// Dbut bloc.
			if ( _Row == qNIL )
				_Read.Init();
// Fin bloc.

			return Maximum;
		}
		virtual bso::sBool FDRDismiss(
            bso::sBool Unlock,
            qRPN ) override
		{
#ifdef CSDEBD_DBG
			if ( _Row != qNIL )
				ERRu();
#endif
			_Read.Init();

			return true;
		}
		virtual fdr::sTID FDRRTake( fdr::sTID Owner ) override
		{
			return fdr::UndefinedTID;
		}
		virtual fdr::size__ FDRWrite(
			const fdr::byte__ *Buffer,
			fdr::size__ Maximum ) override
		{
			_Write.Append( Buffer, Maximum );

			return Maximum;
		}
		virtual bso::sBool FDRCommit(
            bso::sBool Unlock,
            qRPN ) override
		{
            return true;
		}
		virtual fdr::sTID FDRWTake( fdr::sTID Owner ) override
		{
			return fdr::UndefinedTID;
		}
	public:
		void reset( bso::bool__ P = true )
		{
			fdr::ioflow_driver___<>::reset( P );

			_Row = qNIL;
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
		csdscb::callback__ *_Callback;
		void *UP_;
		fdr::rRWDriver *RWDriver_;
		bso::bool__ _DataAvailable;
		void _Create( void )
		{
		qRH
			fnm::name___ Path;
		qRB
			Path.Init();
			dir::GetSelfPath( Path );

			UP_ = _Callback->PreProcess( RWDriver_, Path.Internal() );
		qRR
		qRT
		qRE
		}
		void _Delete( void )
		{
			if( UP_ != NULL )
				if ( !_Callback->PostProcess( UP_ ) )
					qRFwk();
		}
	protected:
		virtual fdr::size__ FDRWrite(
			const fdr::byte__ *Buffer,
			fdr::size__ Maximum ) override
		{
			_DataAvailable = true;

			return _passive_generic_driver___::FDRWrite( Buffer, Maximum );
		}
		virtual bso::sBool FDRCommit(
            bso::sBool Unlock,
            qRPN ) override
		{
			if ( _DataAvailable )
				_Callback->Process( RWDriver_, UP_ );

			_DataAvailable = false;

			return true;
		}
	public:
		void reset( bso::bool__ P = true )
		{
			_passive_generic_driver___::reset( P );

			if ( P )
				_Delete();

			_Callback = NULL;
			UP_ = NULL;
			RWDriver_ = NULL;
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
			csdscb::callback__ &Callback,
			fdr::rRWDriver *RWDriver,
			fdr::thread_safety__ ThreadSafety )
		{
			reset();

			_Callback = &Callback;
			RWDriver_ = RWDriver;

			_passive_generic_driver___::Init( ThreadSafety );

			_Create();
		}
	};

	class rDriver
	: public _active_generic_driver___
	{
	private:
		data _Master, _Slave;
		struct backend {
			_passive_generic_driver___ Driver;
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
				Driver.Init( fdr::ts_Default );
			}
		} _Backend;
	public:
		rDriver( void )
		: _active_generic_driver___( _Master, _Slave ),
		  _Backend( _Slave ,_Master )
		{
			reset( false );
		}
		~rDriver( void )
		{
			reset();
		}
		void reset( bso::bool__ P = true )
		{
			_active_generic_driver___::reset( P );

			_Backend.reset( P );

			_Master.reset( P );
			_Slave.reset( P );
		}
		void Init( csdscb::callback__ &Callback )
		{
			reset();
/*
			ioflow__::Init();
			_Backend.Init();
*/
			_Master.Init();
			_Slave.Init();

			_Backend.Init();

			_active_generic_driver___::Init( Callback, &_Backend.Driver, fdr::ts_Default );
		}
	};

	class _client_server_ioflow___
	: public flw::ioflow__
	{
	private:
		rDriver Driver_;
		flw::byte__ _Cache[CSDLEC_CACHE_SIZE];
	public:
		void reset( bso::bool__ P = true )
		{
			Driver_.reset( P );
		}
		qCDTOR( _client_server_ioflow___ );
		void Init( csdscb::callback__ &Callback )
		{
			reset();
			Driver_.Init( Callback );
			ioflow__::Init( Driver_, _Cache, sizeof( _Cache ) );
		}
	};

	class library_embedded_client_core__
	{
	private:
		dlbrry::dynamic_library___ _Library;
		csdscb::callback__ *_Callback;
		bso::bool__ _RetrieveCallback( csdleo::shared_data__ *Data );
		bso::bool__ _ReleaseCallback( void );
	public:
		void reset( bso::bool__ P = true )
		{
			if ( P ) {
				if ( _Library.IsInitialized() ) {
					if ( _Callback != NULL )
						if ( !_ReleaseCallback() ) {
							_Library.reset();
							qRSys();
						}
				}
			}

			_Callback = NULL;
			_Library.reset( P );
		}
		library_embedded_client_core__( void )
		{
			reset( false );
		}
		~library_embedded_client_core__( void )
		{
			reset();
		}
		bso::bool__ Init(
			const char *LibraryName,
			library_data__ &Data,
			err::handling__ ERRHandling = err::h_Default );
		bso::bool__ IsInitialized( void ) const
		{
			return _Callback != NULL;
		}
		csdscb::callback__ &GetCallback( void ) const
		{
			if ( !IsInitialized() )
				qRFwk();

			return *_Callback;
		}
	};

	class library_embedded_client_ioflow___
	: public _client_server_ioflow___
	{
	public:
		void Init( library_embedded_client_core__ &Core )
		{
			_client_server_ioflow___::Init( Core.GetCallback() );
		}
	};

}

#endif
