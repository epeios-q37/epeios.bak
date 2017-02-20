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
#include <nan.h>

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
			struct sData_ {
				fdr::rIDriver *IDriver;
				txf::sOFlow *OFlow;
				tht::rBlocker *Blocker;
				void reset( bso::sBool P = true )
				{
					tol::reset( P, IDriver, OFlow, Blocker );
				}
				qCDTOR( sData_ );
			};

			void Process_(
				fdr::rIDriver &IDriver,
				txf::sOFlow &OFlow )
			{
			qRH
				flw::sDressedIFlow<> IFlow;
				xtf::sIFlow XFlow;
			qRB
				IFlow.Init( IDriver );
				XFlow.Init( IFlow, utf::f_Guess );

				if ( xpp::Process( XFlow, xpp::criterions___( "" ), xml::oIndent, OFlow ) != xpp::sOK )
					qRGnr();
			qRR
			qRT
				OFlow.Commit();	
			qRE
			}

			void Routine_( void *UP )
			{
			qRFH
				sData_ &Data = *(sData_ *)UP;
				fdr::rIDriver &IDriver = *Data.IDriver;
				txf::sOFlow &OFlow = *Data.OFlow;
			qRFB
				Data.Blocker->Unblock();

				Process_( IDriver, OFlow );
			qRFR
			qRFT
			qRFE(scln::ErrFinal() )
			}
		}

		void ASyncProcess(
			fdr::rIDriver &IDriver,
			txf::sOFlow &OFlow )
		{
		qRH
			sData_ Data;
			tht::rBlocker Blocker;
		qRB
			Blocker.Init();

			Data.IDriver = &IDriver;
			Data.OFlow = &OFlow;
			Data.Blocker = &Blocker;

			mtk::Launch( Routine_, &Data );

			Blocker.Wait();
		qRR
		qRT
		qRE
		}
	}

	// Flow to stream. Read the content of a flow (epeios) and put it in a stream (node.js).
	namespace f2s_ {
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

		class rAIDriverCallback
		: public cASync_
		{
		private:
			fdr::rIDriver *Driver_;
			rContent *Content_;
		protected:
			virtual bso::sBool COMMONProcess( void ) override
			{
			qRH
				fdr::sSize Amount = 0, Handled = 0;
				fdr::sByte Buffer[100];
				flw::sDressedIFlow<> Flow;
			qRB
				Flow.Init( *Driver_ );

				while ( !Flow.EndOfFlow() ) {
					Handled = 0;
					Amount = Flow.ReadUpTo( sizeof( Buffer ), Buffer );

					while ( Amount != Handled ) {
						Handled += Content_->P_Write( Buffer + Handled, Amount - Handled );
						tht::Defer();
					}
				}
			qRR
			qRT
				Content_->P_Write( NULL, 0 );
			qRE
				return true;
			}
			void COMMONDisclose(void) override {}
		public:
			void reset( bso::sBool P = true )
			{
				tol::reset( P, Driver_, Content_ );
			}
			qCVDTOR( rAIDriverCallback );
			void Init(
				fdr::rIDriver &Driver,
				rContent &Content )
			{
				Driver_ = &Driver;
				Content_ = &Content;
			}
		};

		class rAStreamCallback
		: public cASync_
		{
		private:
		rContent *Content_;
		v8::Persistent<v8::Object> Stream_;
		tht::rBlocker *Blocker_;
		fdr::sByte Buffer_[100];
		protected:
			virtual bso::sBool COMMONProcess( void ) override
			{
				fdr::sSize Amount = 0;

				while ( ((Amount = Content_->C_Read(sizeof(Buffer_) - 1, Buffer_)) == 0) && !Content_->C_IsDrained() )
					tht::Defer();
				Buffer_[Amount] = 0;

				return Content_->C_IsDrained();
			}
			virtual void COMMONDisclose( void ) override
			{
			qRH
				v8qnjs::sRStream Stream;
				v8qnjs::sString String;
			qRB
/*				v8::Local<v8::Context> context = v8::Context::New(v8q::GetIsolate());
				v8::Context::Scope context_scope(context);
				context->Enter();
				*/

				if ( *Buffer_ ) {
					String.Init( (const char *)Buffer_ );
					Stream.Init( v8::Local<v8::Object>::New( v8q::GetIsolate(), Stream_ ) );
					Stream.Push( v8q::ToLocal( node::Buffer::New( v8q::GetIsolate(), String.Core() ) ) );
				} else
					Stream.End();
			qRR
			qRT
//				Blocker_->Unblock();
			qRE
			}
		public:
			void reset( bso::sBool P = true )
			{
				tol::reset( P, Content_, Blocker_ );
				Stream_.Reset();
			}
			qCVDTOR( rAStreamCallback );
			void Init(
				rContent &Content,
				v8qnjs::sRStream &Stream,
				tht::rBlocker &Blocker )
			{
				Content_ = &Content;
				Stream_.Reset( v8qnjs::GetIsolate(), Stream.Core() );
				Blocker_ = &Blocker;
			}
		};

	}

	typedef common::sFRelay sFRelay_;

	class rDownstreamRack_
	{
	private:
		sFRelay_ StreamRelay_, ProcessRelay_;
		txf::rOFlow ProcessFlow_;
		f2s_::rContent Content_;
		f2s_::rAIDriverCallback DriverCallback_;
		f2s_::rAStreamCallback StreamCallback_;
	public:
		txf::rOFlow OFlow;
		void reset( bso::sBool P = true )
		{
			tol::reset( P, StreamRelay_, ProcessRelay_, ProcessFlow_, Content_, DriverCallback_, StreamCallback_, OFlow );
		}
		qCDTOR( rDownstreamRack_ );
		void Init(
			v8qnjs::sRStream &Stream,
			tht::rBlocker &Blocker )
		{
			tol::Init( StreamRelay_, ProcessRelay_, Content_ );

			OFlow.Init( ProcessRelay_.Out );
			ProcessFlow_.Init( StreamRelay_.Out );

			process_::ASyncProcess( ProcessRelay_.In, ProcessFlow_ );

			DriverCallback_.Init( StreamRelay_.In, Content_ );
			common::HandleASync( DriverCallback_, false );

			StreamCallback_.Init( Content_, Stream, Blocker );
			common::HandleASync( StreamCallback_, false );
		}
	};

	class rRack_ {
	public:
		tht::rBlocker Blocker_;
//		common::sUpstreamRack Upstream;
		rDownstreamRack_ Downstream;
		void reset( bso::sBool P = true )
		{
			if ( P && false)
				Blocker_.Wait();
			tol::reset( P, Blocker_, /*Upstream, */ Downstream );
		}
		qCDTOR( rRack_ );
		void Init( v8qnjs::sRStream &Stream )
		{
			Blocker_.Init();
//			Upstream.Init();
			Downstream.Init( Stream, Blocker_  );
		}
	};

	void OnData_( const v8q::sFunctionInfos &Infos )
	{
	qRFH
		v8qnjs::sRStream This;
		v8qnjs::sBuffer Chunk;
		fdr::sSize Amount = 0;
	qRFB
		This.Init(Infos.This() );

		Chunk.Init();
		v8q::Get( Infos, Chunk );
		
		rRack_ &Rack = *v8qnjs::sExternal<rRack_>( This.Get( "_rack0" ) ).Value();

		Rack.Downstream.OFlow << Chunk;
	qRFR
	qRFT
	qRFE( scln::ErrFinal() )
	}

	void OnEnd_( const v8q::sFunctionInfos &Infos )
	{
	qRFH
		v8qnjs::sRStream This;
	qRFB
		This.Init(Infos.This() );
		rRack_ &Rack = *v8qnjs::sExternal<rRack_>( This.Get( "_rack0" ) ).Value();

		Rack.Downstream.OFlow.Commit();

		// delete v8qnjs::sExternal<rRack_>( This.Get( "_rack0" ) ).Value();
	qRFR
	qRFT
	qRFE( scln::ErrFinal() )
	}

	void OnRead_( const v8q::sFunctionInfos &Infos )
	{
		// Nothing to do !

		static int i = 0;

		if ( i == 0 )
			i++;
	}
}

void stream::Set( const v8q::sArguments &Arguments )
{
qRFH
	v8qnjs::sRStream Source, This;
	rRack_ *Rack = NULL;
qRFB
	Rack = new rRack_;

	if ( Rack == NULL )
		qRGnr();

	tol::Init( Source, This );
	Arguments.Get( Source, This );

	Rack->Init( This );

	Source.Set( "_rack0", v8qnjs::sExternal<rRack_>( Rack ) );
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
