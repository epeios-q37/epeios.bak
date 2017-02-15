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
		struct rData {
			tht::rBlocker Blocker;
			fdr::rIDriver *IDriver;
			txf::sOFlow *OFlow;
			void reset( bso::sBool P = true )
			{
				tol::reset( P , Blocker, IDriver, OFlow );
			}
			qCDTOR( rData );
			void Init( void )
			{
				reset();
			}
		};

		void Routine( void *UP )
		{
		qRFH
			flw::sDressedIFlow<> IFlow;
			xtf::sIFlow XFlow;
			rData &Data = *(rData *)UP;
			fdr::rIDriver &IDriver = *Data.IDriver;
			txf::sOFlow &OFlow = *Data.OFlow;
		qRFB
			Data.Blocker.Unblock();

			IFlow.Init( IDriver );
			XFlow.Init( IFlow, utf::f_Guess );

			if ( xpp::Process( XFlow, xpp::criterions___( "" ), xml::oIndent, OFlow ) != xpp::sOK )
				qRGnr();

		qRFR
		qRFT
			XFlow.Dismiss();	// Avoid lock owner problem when destroying rack.
			OFlow.Commit();	
		qRFE( scln::ErrFinal() )
		}
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
		
		struct rData {
			tht::rBlocker Blocker;
			flw::sIFlow *Flow;
			rContent *Content;
			void reset( bso::sBool P = true )
			{
				tol::reset( P , Blocker, Flow, Content );
			}
			qCDTOR( rData );
			void Init( void )
			{
				reset();
			}
		};

		void Routine( void *UP )
		{
		qRFH
			fdr::sSize Amount = 0, Handled = 0;
			fdr::sByte Buffer[100];
			txf::rOFlow COut;
			rData &Data = *( rData *)UP;
			flw::sIFlow &Flow = *Data.Flow;
			rContent &Content = *Data.Content;
		qRFB
			COut.Init( cio::GetOutDriver() );

			Data.Blocker.Unblock();

			while ( !Flow.EndOfFlow() ) {
				Handled = 0;
				Amount = Flow.ReadUpTo( sizeof( Buffer ), Buffer );

				while ( Amount != Handled ) {
					Handled += Content.P_Write( Buffer + Handled, Amount - Handled );
					tht::Defer();
				}
			}

			COut << __LOC__ << txf::nl << txf::commit;
		qRFR
		qRFT
			Flow.Dismiss();	// Avoid lock owner problem when destroying rack.
			Content.P_Write( NULL, 0 );
		qRFE( scln::ErrFinal() )
		}
	}

	typedef common::rASync rASync_;

	class sUpstreamRack_
	{
	private:
		rASync_ ASync_;
	public:
		txf::rOFlow OFlow;
		void reset( bso::sBool P = true )
		{
			tol::reset( P, ASync_, OFlow );
		}
		qCDTOR( sUpstreamRack_ );
		void Init( void )
		{
			tol::Init( ASync_ );
			OFlow.Init( ASync_.Out );
		}
		fdr::rIDriver &IDriver( void )
		{
			return ASync_.In;
		}
	};

	class sDownstreamRack_
	{
	private:
		rASync_ ASync_;
		flx::sMarkers Markers_;
		txf::rOFlow FlowI_, FlowO_;
		flx::rIOMonitor Monitor_;
	public:
		flw::sDressedIFlow<> IFlow;
		read_::rContent Content;
		txf::rOFlow OFlow;
		void reset( bso::sBool P = true )
		{
			tol::reset( P, ASync_, OFlow, Markers_,  FlowI_, FlowO_, Monitor_, IFlow, Content );
		}
		qCDTOR( sDownstreamRack_ );
		void Init( void )
		{
			tol::Init( ASync_ );
			OFlow.Init( ASync_.Out );

			Markers_.Async = true;
			Markers_.In.Before = "\n\t>>>>>>>>>>>>>>>>>I\n";
			Markers_.In.After = "\n\t<<<<<<<<<<<<<<<<<I\n";
			Markers_.Out.Before = "\n\t>>>>>>>>>>>>>>>>>O\n";
			Markers_.Out.After = "\n\t<<<<<<<<<<<<<<<<<O\n";

			FlowI_.Init( cio::GetOutDriver() );
			FlowO_.Init( cio::GetOutDriver() );

			Monitor_.Init( ASync_.In, ASync_.Out, Markers_, FlowI_, FlowO_ );
	#if 0
			IFlow.Init( Monitor_ );
			OFlow.Init( Monitor_ );
	#else
			IFlow.Init( ASync_.In );
			OFlow.Init( ASync_.Out );
	#endif
			Content.Init();
		}
	};

	class sRack_ {
	public:
		sUpstreamRack_ Upstream;
		sDownstreamRack_ Downstream;
		bso::sBool Started;
		void reset( bso::sBool P = true )
		{
			tol::reset( P, Upstream, Downstream, Started );
		}
		qCDTOR( sRack_ );
		void Init( void )
		{
		qRH
			process_::rData Data;
		qRB
			Upstream.Init();
			Downstream.Init();
			Started = false;

//			int i = 0; while ( i == 0 );

			Data.Init();

			Data.Blocker.Init();
			Data.IDriver = &Upstream.IDriver();
			Data.OFlow = &Downstream.OFlow;

			mtk::Launch( process_::Routine, &Data );
			Data.Blocker.Wait();
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

		if ( Block )
		cio::COut << __LOC__ << tht::GetTID() << txf::nl << txf::commit;

		do {
			while ( ( Amount = Content.C_Read( sizeof( Buffer ) - 1, Buffer ) ) != 0 ) {
				Buffer[Amount] = 0;
				Chunk.Init( (const char *)Buffer, Function );
				Stream.Push( Chunk );
			}
			tht::Defer();
		} while ( !Content.C_IsDrained() && Block );

		if ( Block )
		cio::COut << __LOC__ << tht::GetTID() << txf::nl << txf::commit;
	qRR
		if ( Block )
		cio::COut << __LOC__ << tht::GetTID() << txf::nl << txf::commit;
	qRT
		if ( Block )
		cio::COut << __LOC__ << tht::GetTID() << txf::nl << txf::commit;
	qRE
		if ( Block )
		cio::COut << __LOC__ << tht::GetTID() << txf::nl << txf::commit;
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
		
		sRack_ &Rack = *v8qnjs::sExternal<sRack_>( This.Get( "_rack0" ) ).Value();

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
		sRack_ &Rack = *v8qnjs::sExternal<sRack_>( This.Get( "_rack0" ) ).Value();

		Dest.Init( This.Get("_dest" ) );

		Rack.Upstream.OFlow.Commit();

		cio::COut << __LOC__ << tht::GetTID() << txf::nl << txf::commit;

		Fill_( Rack.Downstream.Content, Dest, This.Get( "_func" ), true );

		cio::COut << __LOC__ << tht::GetTID() << txf::nl << txf::commit;
		
		delete v8qnjs::sExternal<sRack_>( This.Get( "_rack0" ) ).Value();
	qRFR
	qRFT
		cio::COut << __LOC__ << tht::GetTID() << txf::nl << txf::commit;
	qRFE( scln::ErrFinal() )
		cio::COut << __LOC__ << tht::GetTID() << txf::nl << txf::commit;

	}

	void OnRead_( const v8q::sFunctionInfos &Infos )
	{
	qRFH
		v8qnjs::sRStream This;
		read_::rData Data;
	qRFB
		This.Init( Infos.This() );
		sRack_ &Rack = *v8qnjs::sExternal<sRack_>( This.Get( "_rack0" ) ).Value();

		if ( !Rack.Started ) {
			Data.Init();
			Data.Blocker.Init();
			Data.Flow = &Rack.Downstream.IFlow;
			Data.Content = &Rack.Downstream.Content;

			mtk::Launch( read_::Routine, &Data );

			Data.Blocker.Wait();
			Rack.Started = true;
		}

	qRFR
	qRFT
	qRFE( scln::ErrFinal() )
	}
}

void stream::Set( const v8q::sArguments &Arguments )
{
qRFH
	v8qnjs::sRStream Source, This;
	v8qnjs::sHelper Helper;
	sRack_ *Rack = NULL;
qRFB
	Rack = new sRack_;

	if ( Rack == NULL )
		qRGnr();

	tol::Init( Source, This, Helper );
	Arguments.Get( Source, This, Helper );

	Rack->Init();

	Source.Set( "_rack0", v8qnjs::sExternal<sRack_>( Rack ) );
	Source.Set( "_dest", This );
	Source.Set("_func", v8q::sFunction( Helper.Get( "from" ) ).Core() ),
	This.Set( "_rack0", v8qnjs::sExternal<sRack_>( Rack ) );

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
