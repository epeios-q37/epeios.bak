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

#include "uvq.h"
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
				void reset( bso::sBool P = true )
				{
					tol::reset( P, IDriver, OFlow );
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

			void Routine_(
				void *UP,
				mtk::gBlocker &Blocker )
			{
			qRFH
				sData_ &Data = *(sData_ *)UP;
				fdr::rIDriver &IDriver = *Data.IDriver;
				txf::sOFlow &OFlow = *Data.OFlow;
			qRFB
				Blocker.Release();

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
		qRB
			Data.IDriver = &IDriver;
			Data.OFlow = &OFlow;

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
			qRFE(scln::ErrFinal() )
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
		typedef common::sFRelay sFRelay_;
	}

	class rRack_
	{
	private:
		sFRelay_ StreamRelay_, ProcessRelay_;
		txf::rOFlow ProcessFlow_;
		rContent_ Content_;
		v8::Persistent<v8::Object> Stream_;
		fdr::sByte Buffer_[100];
	public:
		txf::rOFlow OFlow;
		void reset( bso::sBool P = true )
		{
			tol::reset( P, StreamRelay_, ProcessRelay_, OFlow, ProcessFlow_, Content_ );
			Stream_.Reset();
		}
		qCVDTOR( rRack_ );
		void Init( v8qnjs::sRStream &Stream )
		{
			tol::Init( StreamRelay_, ProcessRelay_, Content_ );

			OFlow.Init( ProcessRelay_.Out );
			ProcessFlow_.Init( StreamRelay_.Out );

			process_::ASyncProcess( ProcessRelay_.In, ProcessFlow_ );
			f2c_::ASyncProcess( StreamRelay_.In, Content_ );

			Stream_.Reset( v8qnjs::GetIsolate(), Stream.Core() );
		}
		void Retrieve( void )
		{
			fdr::sSize Amount = 0;

			while ( ( ( Amount = Content_.C_Read( sizeof( Buffer_ ) - 1, Buffer_ ) ) == 0 )
				    && !Content_.C_IsDrained() )
				tht::Defer();

			Buffer_[Amount] = 0;
		}
		bso::sBool Send( void )
		{
			bso::sBool Terminate = false;
		qRH
			v8qnjs::sRStream Stream;
			v8qnjs::sString String;
		qRB
			Stream.Init( v8::Local<v8::Object>::New( v8q::GetIsolate(), Stream_ ) );
			
			if ( *Buffer_ ) {
				String.Init( (const char *)Buffer_ );
				Stream.Push( v8q::ToLocal( node::Buffer::New( v8q::GetIsolate(), String.Core() ) ) );
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

	class rRackASyncCallback_
	: public rRack_,
		public cASync_
	{
	protected:
		virtual void UVQWork( void ) override
		{
			rRack_::Retrieve();
		}
		virtual uvq::eBehavior UVQAfter( void ) override
		{
			if ( rRack_::Send() ) {
				return uvq::bExitAndDelete;
			} else
				return uvq::bRelaunch;
		}
	public:
		void reset( bso::sBool P = true )
		{
			rRack_::reset( P );
		}
		qCVDTOR( rRackASyncCallback_ );
		void Init( v8qnjs::sRStream &Stream )
		{
			rRack_::Init( Stream );
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

		Rack.OFlow << Chunk;
	qRFR
	qRFT
	qRFE( scln::ErrFinal() )
	}

	void OnEnd_( const v8q::sFunctionInfos &Infos )
	{
	qRFH
		v8qnjs::sRStream This;
	qRFB
		This.Init( Infos.This() );
		rRack_ &Rack = *v8qnjs::sExternal<rRack_>( This.Get( "_rack0" ) ).Value();

		Rack.OFlow.Commit();
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
	rRackASyncCallback_ *Rack = NULL;
qRFB
	Rack = new rRackASyncCallback_;

	if ( Rack == NULL )
		qRAlc();

	tol::Init( Source, This );
	Arguments.Get( Source, This );

	Rack->Init( This );

	Source.Set( "_rack0", v8qnjs::sExternal<rRack_>( Rack ) );

	Source.OnData( OnData_ );
	Source.OnEnd( OnEnd_ );
	This.OnRead( OnRead_ );

	uvq::Launch( *Rack );

	Rack = NULL;
qRFR
qRFT
	if ( Rack != NULL )
		delete Rack;
qRFE( scln::ErrFinal() )
}
