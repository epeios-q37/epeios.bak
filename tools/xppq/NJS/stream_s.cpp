/*
	Copyright (C) 2007-2017 Claude SIMON (http://q37.info/contact/).

	This file is part of XPPq.

	XPPq is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

	XPPq is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with XPPq. If not, see <http://www.gnu.org/licenses/>.
*/

#include "stream_s.h"

#include "mtk.h"

using namespace stream_s;

fdr::sSize stream_s::rContent_::C_Read(
	fdr::sSize Amount,
	fdr::sByte *Buffer )
{
qRH
	tht::rLockerHandler Locker;
qRB
	Locker.Init( Locker_ );

	if ( Amount_ == 0 ) {
		Locker.Unlock();
		Blocker_.Wait();
		Locker.Lock();
	}

	if ( Amount_ != 0 ) {
		if ( Amount > Amount_ )
			Amount = Amount_;

		memcpy( Buffer, Buffer_, Amount );

		Amount_ -= Amount;

		if ( Amount_ != 0 )
			memcpy( Buffer_, Buffer_ + Amount, Amount_ );
	} else
		qRGnr();
qRR
qRT
qRE
	return Amount;
}

fdr::sSize stream_s::rContent_::P_Write(
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

			Blocker_.Unblock();
		}
	}
qRR
qRT
qRE
	return Amount;
}

void stream_s::rRack::reset( bso::sBool P )
{
	if ( P ) {
		if ( Stream_ != NULL )
			delete Stream_;
	}

	tol::reset( P, StreamRelay_, ProcessRelay_, OFlow, ProcessFlow_, Content_, Blocker, Wait, Error, Stream_ );
	Amount_ = 0;
}

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



void stream_s::rRack::Init( sclnjs::rRStream &Stream )
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

void stream_s::rRack::Retrieve( void )
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

bso::sBool stream_s::rRack::Send( void )
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
