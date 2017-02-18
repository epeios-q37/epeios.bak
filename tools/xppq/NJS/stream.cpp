/*
	Copyright (C) 2007-2017 Claude SIMON (http://q37.info/contact/).

	This file is part of xppq.

	xppq is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

	xppq is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with xppq. If not, see <http://www.gnu.org/licenses/>.
*/

#include <uv.h>

#include "stream.h"

#include "v8qnjs.h"

#include "common.h"

#include "cio.h"
#include "mtk.h"
#include "scln.h"
#include "xtf.h"
#include "xpp.h"

namespace {
	namespace process_ {
		namespace {
			typedef common::cASync cASync_;
		}

		class sASyncCallback
		: public cASync_
		{
		private:
			fdr::rIDriver *IDriver_;
			txf::sOFlow *OFlow_;
		protected:
			virtual void COMMONProcess( void ) override
			{
			qRH
				flw::sDressedIFlow<> IFlow;
				xtf::sIFlow XFlow;
			qRB
				IFlow.Init( *IDriver_ );
				XFlow.Init( IFlow, utf::f_Guess );

				if ( xpp::Process( XFlow, xpp::criterions___( "" ), xml::oIndent, *OFlow_ ) != xpp::sOK )
					qRGnr();
			qRR
			qRT
				XFlow.Dismiss();	// Avoid lock owner problem when destroying rack.
				OFlow_->Commit();	
			qRE
			}
		public:
			void reset( bso::sBool P = true )
			{
				tol::reset( P, IDriver_, OFlow_ );
			}
			qCVDTOR( sASyncCallback );
			void Init(
				fdr::rIDriver &IDriver,
				txf::sOFlow &OFlow )
			{
				IDriver_ = &IDriver;
				OFlow_ = &OFlow;
			}
		};
	}

	// 'P_...' : producer ; 'C_...' : consumer.
	namespace read_ {
		class rContent
		{
		private:
			tht::rLocker Locker_;
			fdr::sByte Buffer_[100];
			fdr::sSize Amount_;
			bso::sBool Drained_;
			fdr::sSize Size_( void )
			{
				return sizeof( Buffer_ );
			}
		public:
			void reset( bso::sBool P = true )
			{
				tol::reset( P, Locker_, Drained_ );

				Amount_ = 0;
			}
			qCDTOR( rContent );
			void Init( void )
			{
				Locker_.Init();
				Amount_ = 0;
				Drained_ = false;
			}
			fdr::sSize C_Read(
				fdr::sSize Amount,
				fdr::sByte *Buffer )
			{
			qRH
				tht::rLockerHandler Locker;
			qRB
				Locker.Init( Locker_ );

				if ( Amount_ != 0 ) {
					if ( Amount > Amount_ )
						Amount = Amount_;

					memcpy( Buffer, Buffer_, Amount );

					Amount_ -= Amount;

					if ( Amount_ != 0 )
						memcpy( Buffer_, Buffer_ + Amount, Amount_ );
				} else
					Amount = 0;
			qRR
			qRT
			qRE
				return Amount;
			}
			fdr::sSize P_Write(
				fdr::sByte *Buffer,
				fdr::sSize Amount )
			{
			qRH
				tht::rLockerHandler Locker;
			qRB
				Locker.Init( Locker_ );

				if ( Drained_ )
					qRGnr();

				if ( ( Amount == 0) || (Buffer == NULL) ) {
					Drained_ = true;
					Amount = 0;
				} else {
					if ( Amount > ( Size_() - Amount_ ) )
						Amount = Size_() - Amount_;

					if ( Amount != 0 ) {
						memcpy( Buffer_ + Amount_, Buffer, Amount );

						Amount_ += Amount;
					}
				}
			qRR
			qRT
			qRE
				return Amount;
			}
			bso::sBool C_IsDrained( void )
			{
				return Drained_;
			}
		};
		
		namespace {
			typedef common::cASync cASync_;
		}

		class sASyncCallback
		: public cASync_
		{
		private:
			flw::sIFlow *Flow_;
			rContent *Content_;
		protected:
			virtual void COMMONProcess( void ) override
			{
			qRH
				fdr::sSize Amount = 0, Handled = 0;
				fdr::sByte Buffer[100];
				txf::rOFlow COut;
			qRB
				COut.Init( cio::GetOutDriver() );

				while ( !Flow_->EndOfFlow() ) {
					Handled = 0;
					Amount = Flow_->ReadUpTo( sizeof( Buffer ), Buffer );

					while ( Amount != Handled ) {
						Handled += Content_->P_Write( Buffer + Handled, Amount - Handled );
						tht::Defer();
					}
				}
			qRR
			qRT
				Flow_->Dismiss();	// Avoid lock owner problem when destroying rack.
				Content_->P_Write( NULL, 0 );
			qRE
			}
		public:
			void reset( bso::sBool P = true )
			{
				tol::reset( P, Flow_, Content_ );
			}
			qCVDTOR( sASyncCallback );
			void Init(
				flw::sIFlow &Flow,
				rContent &Content )
			{
				Flow_ = &Flow;
				Content_ = &Content;
			}
		};
	}

	typedef common::sFRelay sFRelay_;

	class rDownstreamRack_
	{
	private:
		sFRelay_ Relay_;
		flx::sMarkers Markers_;
		txf::rOFlow FlowI_, FlowO_;
		flx::rIOMonitor Monitor_;
	public:
		flw::sDressedIFlow<> IFlow;
		read_::rContent Content;
		txf::rOFlow OFlow;
		void reset( bso::sBool P = true )
		{
			tol::reset( P, Relay_, OFlow, Markers_,  FlowI_, FlowO_, Monitor_, IFlow, Content );
		}
		qCDTOR( rDownstreamRack_ );
		void Init( void )
		{
			tol::Init( Relay_ );
			OFlow.Init( Relay_.Out );

			Markers_.Async = true;
			Markers_.In.Before = "\n\t>>>>>>>>>>>>>>>>>I\n";
			Markers_.In.After = "\n\t<<<<<<<<<<<<<<<<<I\n";
			Markers_.Out.Before = "\n\t>>>>>>>>>>>>>>>>>O\n";
			Markers_.Out.After = "\n\t<<<<<<<<<<<<<<<<<O\n";

			FlowI_.Init( cio::GetOutDriver() );
			FlowO_.Init( cio::GetOutDriver() );

			Monitor_.Init( Relay_.In, Relay_.Out, Markers_, FlowI_, FlowO_ );
	#if 0
			IFlow.Init( Monitor_ );
			OFlow.Init( Monitor_ );
	#else
			IFlow.Init( Relay_.In );
			OFlow.Init( Relay_.Out );
	#endif
			Content.Init();
		}
	};

	class rRack_ {
	private:
		read_::sASyncCallback AReadCallback_;
		process_::sASyncCallback AProcessCallback_;
	public:
		common::sUpstreamRack Upstream;
		rDownstreamRack_ Downstream;
		bso::sBool Started;
		void reset( bso::sBool P = true )
		{
			tol::reset( P, AReadCallback_, AProcessCallback_, Upstream, Downstream, Started );
		}
		qCDTOR( rRack_ );
		void Init( void )
		{
		qRH
		qRB
			Upstream.Init();
			Downstream.Init();
			Started = false;

			AProcessCallback_.Init( Upstream.IDriver(), Downstream.OFlow );
			common::HandleASync( AProcessCallback_ );

			AReadCallback_.Init( Downstream.IFlow, Downstream.Content );
			common::HandleASync( AReadCallback_ );
		qRR
		qRT
		qRE
		}
	};

	void Fill_(
		read_::rContent &Content,
		v8qnjs::sRStream &Stream,
		v8::Local<v8::Value> RawFunction,
		bso::sBool Block )
	{
	qRH
		fdr::sByte Buffer[100];
		fdr::sSize Amount = 0;
		v8qnjs::sBuffer Chunk;
		v8qnjs::sFunction Function;
	qRB
		Function.Init( RawFunction );

		do {
			while ( ( Amount = Content.C_Read( sizeof( Buffer ) - 1, Buffer ) ) != 0 ) {
				Buffer[Amount] = 0;
				Chunk.Init( (const char *)Buffer, Function );
				Stream.Push( Chunk );
			}
			tht::Defer();
		} while ( !Content.C_IsDrained() && Block );

	qRR
	qRT
	qRE
	}

	void OnData_( const v8q::sFunctionInfos &Infos )
	{
	qRFH
		v8qnjs::sRStream This, Dest;
		v8qnjs::sBuffer Chunk;
		fdr::sSize Amount = 0;
	qRFB
		This.Init(Infos.This() );

		Chunk.Init();
		v8q::Get( Infos, Chunk );

		Dest.Init( This.Get("_dest" ) );
		
		rRack_ &Rack = *v8qnjs::sExternal<rRack_>( This.Get( "_rack0" ) ).Value();

		Rack.Upstream.OFlow << Chunk;

		Fill_( Rack.Downstream.Content, Dest, This.Get( "_func" ), false );
	qRFR
	qRFT
	qRFE( scln::ErrFinal() )
	}

	void OnEnd_( const v8q::sFunctionInfos &Infos )
	{
	qRFH
		v8qnjs::sRStream This, Dest;
	qRFB
		This.Init(Infos.This() );
		rRack_ &Rack = *v8qnjs::sExternal<rRack_>( This.Get( "_rack0" ) ).Value();

		Dest.Init( This.Get("_dest" ) );

		Rack.Upstream.OFlow.Commit();

		Fill_( Rack.Downstream.Content, Dest, This.Get( "_func" ), true );

		delete v8qnjs::sExternal<rRack_>( This.Get( "_rack0" ) ).Value();
	qRFR
	qRFT
	qRFE( scln::ErrFinal() )
	}

	void OnRead_( const v8q::sFunctionInfos &Infos )
	{
		// Nothing to do !
	}
}

void stream::Set( const v8q::sArguments &Arguments )
{
qRFH
	v8qnjs::sRStream Source, This;
	v8qnjs::sHelper Helper;
	rRack_ *Rack = NULL;
qRFB
	Rack = new rRack_;

	if ( Rack == NULL )
		qRGnr();

	tol::Init( Source, This, Helper );
	Arguments.Get( Source, This, Helper );

	Rack->Init();

	Source.Set( "_rack0", v8qnjs::sExternal<rRack_>( Rack ) );
	Source.Set( "_dest", This );
	Source.Set("_func", v8q::sFunction( Helper.Get( "from" ) ).Core() ),
	This.Set( "_rack0", v8qnjs::sExternal<rRack_>( Rack ) );

	Source.OnData( OnData_ );
	Source.OnEnd( OnEnd_ );
	This.OnRead( OnRead_ );

	Rack = NULL;
qRFR
qRFT
	if ( Rack != NULL )
		delete Rack;
qRFE( scln::ErrFinal() )
}
