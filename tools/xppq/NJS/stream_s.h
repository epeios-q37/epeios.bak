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

// STREAM Shared.

#ifndef STREAM_S_INC_
# define STREAM_S_INC_

# include "common.h"

# include "sclnjs.h"
# include "tol.h"
# include "xtf.h"

namespace stream_s {
	class rContent_
	{
	private:
		tht::rLocker Locker_;
		tht::rBlocker Blocker_;
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
			tol::reset( P, Locker_, Blocker_, Drained_ );

			Amount_ = 0;
		}
		qCDTOR( rContent_ );
		void Init( void )
		{
			Locker_.Init();
			Blocker_.Init( true );
			Amount_ = 0;
			Drained_ = false;
		}
		fdr::sSize C_Read(
			fdr::sSize Amount,
			fdr::sByte *Buffer );
		fdr::sSize P_Write(
			fdr::sByte *Buffer,
			fdr::sSize Amount );
		bso::sBool C_IsDrained( void )
		{
			return Drained_;
		}
	};

	typedef common::sRelay sRelay_;

	class rRack
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
		void reset( bso::sBool P = true );
		qCVDTOR( rRack );
		void Init( sclnjs::rRStream &Stream );
		void Retrieve( void );
		bso::sBool Send( void );
	};

	typedef xpp::rIFlow rPreprocessor_;
	typedef txf::rOFlow rOFlow_;

	class rNewRack
	: public rPreprocessor_,
	  public rOFlow_
	{
	private:
		flx::rFRelay<> Relay_;
		flx::sIRelay IRelay_;
		flx::sORelay ORelay_;
		flw::sDressedIFlow<> IFlow_;
		xtf::sIFlow XFlow_;
	public:
		void reset( bso::sBool P = true )
		{
			rPreprocessor_::reset( P );
			rOFlow_::reset( P );

			tol::reset( P, XFlow_, IFlow_, ORelay_, IRelay_ );
		}
		qCDTOR( rNewRack );
		void Init( void )
		{
			Relay_.Init();

			IRelay_.Init( Relay_ );
			IFlow_.Init( IRelay_ );

			ORelay_.Init( Relay_ );
			rOFlow_::Init( ORelay_ );
		}
		void DelayedInit( void )
		{
			XFlow_.Init( IFlow_, utf::f_Guess );
			rPreprocessor_::Init( XFlow_, xpp::rCriterions( "" ) );
		}
	};
}

#endif
