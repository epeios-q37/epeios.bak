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

#include <node_buffer.h>

#include "uvq.h"
#include "nodeq.h"

#include "common.h"

#include "sclmisc.h"

#include "cio.h"
#include "mtk.h"
#include "sclnjs.h"
#include "xtf.h"
#include "xpp.h"

namespace {
	namespace process_ {
		namespace {
			struct sData_ {
				fdr::rIDriver *IDriver;
				txf::sOFlow *OFlow;
				str::dString *Error;
				void reset( bso::sBool P = true )
				{
					tol::reset( P, IDriver, OFlow, Error );
				}
				qCDTOR( sData_ );
			};

			void Process_(
				fdr::rIDriver &IDriver,
				txf::sOFlow &OFlow,
				str::dString &Error )
			{
			qRH
				flw::sDressedIFlow<> IFlow;
				xtf::sIFlow XFlow;
				xpp::context___ Context;
				lcl::meaning Meaning;
			qRB
				IFlow.Init( IDriver );
				XFlow.Init( IFlow, utf::f_Guess );
				Context.Init();

				if ( xpp::Process(XFlow, xpp::criterions___(""), xml::oIndent, OFlow, Context) != xpp::sOK ) {
					Meaning.Init();
					xpp::GetMeaning( Context, Meaning );
					scllocale::GetTranslation( Meaning, "", Error );
				}
			qRR
			qRT
				OFlow.Commit();	
			qRE
			}

			void Routine_(
				void *UP,
				mtk::gBlocker &Blocker )
			{
			qRFH
				sData_ &Data = *(sData_ *)UP;
				fdr::rIDriver &IDriver = *Data.IDriver;
				txf::sOFlow &OFlow = *Data.OFlow;
				str::dString &Error = *Data.Error;
			qRFB
				Blocker.Release();

				Process_( IDriver, OFlow, Error );
			qRFR
			qRFT
			qRFE(sclnjs::ErrFinal() )
			}
		}

		void ASyncProcess(
			fdr::rIDriver &IDriver,
			txf::sOFlow &OFlow,
			str::dString &Error )
		{
		qRH
			sData_ Data;
		qRB
			Data.IDriver = &IDriver;
			Data.OFlow = &OFlow;
			Data.Error = &Error;

			mtk::Launch( Routine_, &Data );
		qRR
		qRT
		qRE
		}
	}

	class rContent_
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
		qCDTOR( rContent_ );
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

	// Flow driver to content.
	namespace f2c_ {
		namespace {
			struct sData_ {
				fdr::rIDriver *Driver;
				rContent_ *Content;
				void reset( bso::sBool P = true )
				{
					tol::reset( P, Driver, Content );
				}
				qCDTOR( sData_ );
			};

			namespace {
				void Process_(
					fdr::rIDriver &Driver,
					rContent_ &Content )
				{
				qRH
					fdr::sSize Amount = 0, Handled = 0;
					fdr::sByte Buffer[100];
					flw::sDressedIFlow<> Flow;
				qRB
					Flow.Init( Driver );

					while ( !Flow.EndOfFlow() ) {
						Handled = 0;
						Amount = Flow.ReadUpTo( sizeof( Buffer ), Buffer );

						while ( Amount != Handled ) {
							Handled += Content.P_Write( Buffer + Handled, Amount - Handled );
							tht::Defer();
						}
					}
				qRR
				qRT
					Content.P_Write( NULL, 0 );
				qRE
				}
			}

			void Routine_(
				void *UP,
				mtk::gBlocker &Blocker )
			{
			qRH
				sData_ &Data = *(sData_ *)UP;
			qRB
				fdr::rIDriver &Driver = *Data.Driver;
				rContent_ &Content = *Data.Content;

				Blocker.Release();

				Process_( Driver, Content );
			qRFR
			qRFT
			qRFE(sclnjs::ErrFinal() )
			}
		}

		void ASyncProcess(
			fdr::rIDriver &Driver,
			rContent_ &Content )
		{
			sData_ Data;

			Data.Driver = &Driver;
			Data.Content = &Content;

			mtk::Launch( f2c_::Routine_, &Data );
		}
	}

	namespace {
		typedef common::sRelay sRelay_;
	}

	class rStreamRack_	// By naming it simply 'rRack_', VC++ debugger confuse it with 'rRack_' in 'parser.cpp', althought both are defined in an anonymous namespace !
	{
	private:
		sRelay_ StreamRelay_, ProcessRelay_;
		txf::rOFlow ProcessFlow_;
		rContent_ Content_;
		v8::Persistent<v8::Object> Stream_;
		fdr::sByte Buffer_[100];
		fdr::sSize Amount_;	// Amount of dfata in the buffer.
	public:
		txf::rOFlow OFlow;
		tht::rBlocker Blocker;
		bso::sBool Wait;
		str::wString Error;
		void reset( bso::sBool P = true )
		{
			tol::reset( P, StreamRelay_, ProcessRelay_, OFlow, ProcessFlow_, Content_, Blocker, Wait, Error );
			Amount_ = 0;
			Stream_.Reset();
		}
		qCVDTOR( rStreamRack_ );
		void Init( nodeq::sRStream &Stream )
		{
			tol::Init( StreamRelay_, ProcessRelay_, Content_, Error );

			Blocker.Init( true );

			Wait = true;

			OFlow.Init( ProcessRelay_.Out );
			ProcessFlow_.Init( StreamRelay_.Out );

			process_::ASyncProcess( ProcessRelay_.In, ProcessFlow_, Error );
			f2c_::ASyncProcess( StreamRelay_.In, Content_ );

			Stream_.Reset( nodeq::GetIsolate(), Stream.Core() );

			Amount_ = 0;
		}
		void Retrieve( void )
		{
			Amount_ = 0;

			if ( Wait ) {
				Wait = false;
				Blocker.Wait();
			}

			while ( ( ( Amount_ = Content_.C_Read( sizeof( Buffer_ ) - 1, Buffer_ ) ) == 0 )
				    && !Content_.C_IsDrained() )
				tht::Defer();
		}
		bso::sBool Send( void )
		{
			bso::sBool Terminate = false;
		qRH
			nodeq::sRStream Stream;
		qRB
			Stream.Init( v8::Local<v8::Object>::New( v8q::GetIsolate(), Stream_ ) );
			
			if ( Amount_ != 0 ) {
				Wait = !Stream.Push( nodeq::sBuffer( (const char *)Buffer_, Amount_ ) );
			} else {
				Stream.End();
				Terminate = true;
			}
		qRR
		qRT
		qRE
			return Terminate;
		}
	};

	namespace {
		typedef uvq::cASync cASync_;
	}

	class rStreamRackASyncCallback_	// By naming it simply 'rRackASyncCallback_', VC++ debugger confuse it with the one in 'parser.cpp', althought both are defined in an anonymous namespace !
	: public rStreamRack_,
		public cASync_
	{
	protected:
		virtual void UVQWork( void ) override
		{
			rStreamRack_::Retrieve();
		}
		virtual uvq::eBehavior UVQAfter( void ) override
		{
			if ( rStreamRack_::Send() ) {
				return uvq::bExitAndDelete;
			} else
				return uvq::bRelaunch;
		}
	public:
		void reset( bso::sBool P = true )
		{
			rStreamRack_::reset( P );
		}
		qCVDTOR( rStreamRackASyncCallback_ );
		void Init( nodeq::sRStream &Stream )
		{
			rStreamRack_::Init( Stream );
		}
	};
#if 1
	void OnReadable_( const v8q::sFunctionInfos &Infos )
	{
	qRFH
		nodeq::sRStream This;
		nodeq::sBuffer Chunk;
	qRFB
		This.Init(Infos.This() );

		rStreamRack_ &Rack = *nodeq::sExternal<rStreamRack_>( This.Get( "_rack0" ) ).Value();

		Chunk.Init();

		if ( This.Read( Chunk ) )
			Rack.OFlow << Chunk;
		else
			Rack.OFlow.Commit();

		if ( sclerror::IsErrorPending() )
			qRAbort();
	qRFR
	qRFT
	qRFE( sclnjs::ErrFinal() )
	}
#else
	void OnData_( const v8q::sFunctionInfos &Infos )
	{
	qRFH
		nodeq::sRStream This;
		nodeq::sBuffer Chunk;
	qRFB
		This.Init(Infos.This() );

		Chunk.Init();
		v8q::Get( Infos, Chunk );
		
		rStreamRack_ &Rack = *nodeq::sExternal<rStreamRack_>( This.Get( "_rack0" ) ).Value();

		Rack.OFlow << Chunk;
	qRFR
	qRFT
	qRFE( sclnjs::ErrFinal() )
	}

	void OnEnd_( const v8q::sFunctionInfos &Infos )
	{
	qRFH
		nodeq::sRStream This;
	qRFB
		This.Init( Infos.This() );
		rStreamRack_ &Rack = *nodeq::sExternal<rStreamRack_>( This.Get( "_rack0" ) ).Value();

		Rack.OFlow.Commit();
	qRFR
	qRFT
	qRFE( sclnjs::ErrFinal() )
	}
#endif

	namespace {
		void Error( const v8::FunctionCallbackInfo<v8::Value>& info )
		{
		//	v8q::sObject(info[0]).Launch("emit", "error", "Ohlala" );
			v8q::sObject(info[0]).EmitError( info[1] );
		}
	}

	void OnRead_( const v8q::sFunctionInfos &Infos )
	{
		nodeq::sRStream This;
		This.Init( Infos.This() );
		v8q::sFunction Function;

		Function.Init( Error );

		rStreamRack_ &Rack = *nodeq::sExternal<rStreamRack_>( This.Get( "_rack0" ) ).Value();

		if ( Rack.Error.Amount() != 0 ) {
			v8q::process::NextTick( Error, This, Rack.Error );
			Rack.Error.Init();
		}

		Rack.Blocker.Unblock();
	}
}

void stream::Set( const sclnjs::sArguments &Arguments )
{
qRH
	nodeq::sRStream Source, This;
	rStreamRackASyncCallback_ *Rack = NULL;
qRB
	Rack = new rStreamRackASyncCallback_;

	if ( Rack == NULL )
		qRAlc();

	tol::Init( Source, This );
	Arguments.Get( Source, This );

	Rack->Init( This );

	Source.Set( "_rack0", nodeq::sExternal<rStreamRack_>( Rack ) );
	This.Set( "_rack0", nodeq::sExternal<rStreamRack_>( Rack ) );

#if 1
	Source.OnReadable( OnReadable_ );
# else // Doesn't always work. Sometimes, 'onend' event is not launched...
	Source.OnData( OnData_ );
	Source.OnEnd( OnEnd_ );
#endif

	This.OnRead( OnRead_ );

	uvq::Launch( *Rack );

	Rack = NULL;
qRR
qRT
	if ( Rack != NULL )
		delete Rack;
qRE
}
