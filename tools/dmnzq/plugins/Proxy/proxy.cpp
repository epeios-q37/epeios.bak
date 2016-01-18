/*
	Copyright (C) 2016 Claude SIMON (http://q37.info/contact/).

	This file is part of 'dmnzq'.

    'dmnzq' is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

    'dmnzq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'dmnzq'.  If not, see <http://www.gnu.org/licenses/>
*/

// 'proxy' slot plugin.

#include "misc.h"

#include "registry.h"

#include "csdbnc.h"
#include "csdbns.h"

#include "sclplugin.h"
#include "sclmisc.h"

#include "mtk.h"

#define PLUGIN_NAME	"proxy"

typedef misc::callback__ _plugin__;

using misc::module__;

namespace {
	typedef fdr::ioflow_driver___<> _flow_driver___;

	typedef flw::standalone_ioflow__<> _flow__;

	class flow___
	: public _flow_driver___,
	  public _flow__
	{
	private:
		csdbnc::flow___ Flow_;
		flx::size_embedded_oflow___ OFlow_;
		flw::iflow__ &IFlow_;
	protected:
		virtual fdr::size__ FDRWrite(
			const fdr::byte__ *Buffer,
			fdr::size__ Maximum) override
		{
			return OFlow_.WriteUpTo( Buffer, Maximum );
		}
		virtual void FDRCommit( void ) override
		{
			OFlow_.Commit();
		}
		virtual fdr::size__ FDRRead(
			fdr::size__ Maximum,
			fdr::byte__ *Buffer ) override
		{
			return IFlow_.ReadUpTo( Maximum, Buffer );
		}
		virtual void FDRDismiss( void ) override
		{
			IFlow_.Dismiss();
		}
public:
		void reset( bso::bool__ P = true )
		{
			_flow__::reset( P );
			_flow_driver___::reset( P );
			OFlow_.reset( P );
			Flow_.reset( P );
		}
		E_VDTOR( flow___ );
		flow___( void )
		: IFlow_( Flow_ )
		{
			reset( false );
		}
		bso::bool__ Init(
			const char *HostService,
			const char *Identifier )
		{
			reset();

			_flow_driver___::Init( fdr::ts_Default );
			_flow__::Init( *this );

			Flow_.Init( HostService );

			OFlow_.Init( Flow_ );

			OFlow_.Write(Identifier, strlen( Identifier ) + 1 );	// '+1' to put the final 0.

			if ( !IFlow_.EndOfFlow() ) {
				if ( IFlow_.Get() != 0 )
					qRGnr();

				return true;
			} else
				return false;
		}
	};

	struct data__ {
		flow___ *Flow;
		module__ *Module;
		mtx::handler___ Mutex;
		const char *Origin;
		void reset( bso::bool__ P = true )
		{
			if ( P )
				if ( Mutex != mtx::UndefinedHandler )
					mtx::Delete( Mutex );

			Flow = NULL;
			Module = NULL;
			Origin = NULL;

			Mutex = mtx::UndefinedHandler;
		}
		void Init(
			flow___ *Flow,
			module__ &Module,
			const char *Origin )
		{
			reset();

			Mutex = mtx::Create();

			this->Flow = Flow;
			this->Module = &Module;
			this->Origin = Origin;
		}
		E_CDTOR( data__ );
	};

	void Process_( void *UP )
	{
	qRFH
		data__ &Data = *(data__ *)UP;
		flow___ *Flow = Data.Flow;
		module__ &Module = *Data.Module;
		ntvstr::string___ Origin;
		void *MUP = NULL;
	qRFB
		Origin.Init( Data.Origin );

		mtx::Unlock( Data.Mutex );

		MUP = Module.PreProcess( Origin );

		while ( Module.Process( *Flow, MUP ) == csdscb::aContinue );
	qRFR
	qRFT
		Module.PostProcess( MUP );
	
		delete Flow;
	qRFE( sclmisc::ErrFinal() )
	}

	class plugin___
	: public _plugin__
	{
	private:
		TOL_CBUFFER___ HostService_, Identifier_;
	protected:
		virtual void MISCProcess( module__ &Module ) override
		{
		qRH
			flow___ *Flow = NULL;
			data__ Data;
		qRB
			Flow = new flow___;

			if ( Flow == NULL )
				qRAlc();

			while ( Flow->Init( HostService_, Identifier_ ) ) {
				Data.Init( Flow, Module, HostService_ );

				mtx::Lock( Data.Mutex );

				mtk::Launch( Process_, &Data );

				mtx::Lock( Data.Mutex );
				mtx::Unlock( Data.Mutex );

				Flow = NULL;

				Flow = new flow___;

				if ( Flow == NULL )
					qRAlc();
			}
		qRR
		qRT
			if ( Flow != NULL )
				delete Flow;
		qRE
		}
	public:
		void reset( bso::bool__ P = true )
		{
			_plugin__::reset( P );
		}
		E_CVDTOR( plugin___ );
		void Init( void )
		{
			_plugin__::Init();
		}
		void SCLPLUGINInitialize( void )
		{
		qRH
			str::string HostService, Identifier;
		qRB
			HostService.Init();
			sclmisc::MGetValue( registry::HostService, HostService );

			Identifier.Init();
			sclmisc::MGetValue( registry::Identifier, Identifier );

			HostService.Convert( HostService_ );
			Identifier.Convert( Identifier_ );
		qRR
		qRT
		qRE
		}
	};
}

SCLPLUGIN_DEF( plugin___ );

const char *sclmisc::SCLMISCTargetName = PLUGIN_NAME;
