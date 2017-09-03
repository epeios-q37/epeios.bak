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

#include "common.h"

#include "sclmisc.h"
#include "sclnjs.h"

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

			namespace {

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
			qRFE( sclnjs::ERRFinal() )
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
			qRFE( sclnjs::ERRFinal() )
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
		sclnjs::rRStream *Stream_;
		fdr::sByte Buffer_[100];
		fdr::sSize Amount_;	// Amount of data in the buffer.
	public:
		txf::rOFlow OFlow;
		tht::rBlocker Blocker;
		bso::sBool Wait;
		str::wString Error;
		void reset( bso::sBool P = true )
		{
			if ( P ) {
				if ( Stream_ != NULL )
					delete Stream_;
			}

			tol::reset( P, StreamRelay_, ProcessRelay_, OFlow, ProcessFlow_, Content_, Blocker, Wait, Error, Stream_ );
			Amount_ = 0;
		}
		qCVDTOR( rStreamRack_ );
		void Init( sclnjs::rRStream &Stream )
		{
			tol::Init( StreamRelay_, ProcessRelay_, Content_, Error );

			Blocker.Init( true );

			Wait = true;

			OFlow.Init( ProcessRelay_.Out );
			ProcessFlow_.Init( StreamRelay_.Out );

			process_::ASyncProcess( ProcessRelay_.In, ProcessFlow_, Error );
			f2c_::ASyncProcess( StreamRelay_.In, Content_ );

			Stream_ = &Stream;

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
			
			if ( Amount_ != 0 ) {
				Wait = !Stream_->Push( Buffer_, Amount_ );
			} else {
				Stream_->End();
				Terminate = true;
			}

			return Terminate;
		}
	};

	namespace {
		typedef sclnjs::cAsync cAsync_;
	}

	class rStreamRackASyncCallback_	// By naming it simply 'rRackASyncCallback_', VC++ debugger confuse it with the one in 'parser.cpp', althought both are defined in an anonymous namespace !
	: public rStreamRack_,
		public cAsync_
	{
	protected:
		virtual void SCLNJSWork( void ) override
		{
			rStreamRack_::Retrieve();
		}
		virtual uvq::eBehavior SCLNJSAfter( void ) override
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
		void Init( sclnjs::rRStream &Stream )
		{
			rStreamRack_::Init( Stream );
		}
	};
}

#if 1
void stream::OnData( sclnjs::sCaller &Caller )
{
qRH
	sclnjs::rRStream This;
	sclnjs::rBuffer Chunk;
qRB
	tol::Init( This, Chunk );
	Caller.GetArgument( This, Chunk );

	rStreamRack_ &Rack = *(rStreamRack_ *)This.Get( "_rack0" );

	Rack.OFlow << Chunk;
qRR
qRT
qRE
}

void stream::OnEOD( sclnjs::sCaller &Caller )
{
qRH
	sclnjs::rRStream This;
qRB
	tol::Init( This );
	Caller.GetArgument( This );

	rStreamRack_ &Rack = *(rStreamRack_ *)This.Get( "_rack0" );

	Rack.OFlow.Commit();
qRR
qRT
qRE
}
/*
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
*/
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


void stream::OnRead( sclnjs::sCaller &Caller )
{
qRH
	sclnjs::rRStream This;
qRB
	This.Init();
	Caller.GetArgument( This );

	rStreamRack_ &Rack = *(rStreamRack_ *)( This.Get( "_rack0" ) );

	if ( Rack.Error.Amount() != 0 ) {
		This.EmitError( Rack.Error );
		Rack.Error.Init();
	}

	Rack.Blocker.Unblock();
qRR
qRE
qRT
}

void stream::Set( sclnjs::sCaller &Caller )
{
qRH
	sclnjs::rRStream Source, *This = NULL;
	rStreamRackASyncCallback_ *Rack = NULL;
qRB
	Rack = new rStreamRackASyncCallback_;

	if ( Rack == NULL )
		qRAlc();

	This = new sclnjs::rRStream;

	if ( This == NULL )
		qRAlc();

	tol::Init( Source, *This );

	Caller.GetArgument( Source, *This );

	Rack->Init( *This );

	Source.Set( "_rack0", Rack );
	This->Set( "_rack0", Rack );

#if 1
//	Source.OnReadable( OnReadable_ );
# else // Doesn't always work. Sometimes, 'onend' event is not launched...
	Source.OnData( OnData_ );
	Source.OnEnd( OnEnd_ );
#endif

//	This.OnRead( OnRead_ );

	sclnjs::Launch( *Rack );
qRR
	if ( Rack != NULL )
		delete Rack;

	if ( This != NULL )
		delete This;
qRT
qRE
}
