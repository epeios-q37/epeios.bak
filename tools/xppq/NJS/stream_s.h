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

	extern const char *Id;

	typedef xpp::rIFlow rPreprocessor_;
	typedef txf::rOFlow rOFlow_;

	class rIn_
	: public rPreprocessor_ {
	private:
		qRMV( sclnjs::rRStream, S_, Stream_ );
		bso::sBool IsFirst_;
		bso::sBool TestBlocker_;
		char Buffer_[100];
		bso::sSize Amount_ = 0;
		tht::rBlocker Blocker_;
		flx::sIRelay Relay_;
		flw::sDressedIFlow<> Flow_;
		xtf::sIFlow XFlow_;
	protected:
		void Read( void )
		{
			if ( TestBlocker_ ) {
				Blocker_.Wait();
				TestBlocker_ = false;
			}

			if ( IsFirst_ ) {
				IsFirst_ = false;
				XFlow_.Init( Flow_, utf::f_Guess );
				rPreprocessor_::Init( XFlow_, xpp::rCriterions( "" ) );
			}

			if ( !EndOfFlow() )
				Amount_ = ReadUpTo( sizeof( Buffer_ ), Buffer_ );
		}
		sclnjs::eBehavior Send( void )
		{
			if ( Amount_ == 0 ) {
				S_().End();
				return sclnjs::bExitAndDelete;
			} else {
				if ( !S_().Push( Buffer_, Amount_ ) )
					TestBlocker_ = true;
				Amount_ = 0;
				return sclnjs::bRelaunch;
			}
		}
	public:
		void reset( bso::sBool P = true )
		{
			if ( P ) {
				if ( Stream_ != NULL )
					delete Stream_;
			}

			IsFirst_ = false;
			Amount_ = 0;
			TestBlocker_ = false;

			rPreprocessor_::reset( P );

			tol::reset( P, Stream_, Blocker_, Relay_, Flow_, XFlow_ );
		}
		qCDTOR( rIn_ );
		void Init(
			sclnjs::rRStream &Stream,
			flx::rRelay_ &Core )
		{
			Stream_ = &Stream;
			IsFirst_ = true;
			TestBlocker_ = true;
			Amount_ = 0;

			Blocker_.Init( true );

			Relay_.Init( Core, fdr::tsDisabled );	// Will be handled from different thread, ('SCLNJSWork(...)'), so the thread-safety is disabled.
			Flow_.Init( Relay_ );

			// The rest will be initialized asynchronously, as it is blocking.
		}
		void Unblock( void )
		{
			Blocker_.Unblock();
		}
	};

	class rOut_
	: public rOFlow_
	{
	private:
		flx::sORelay Relay_;
		flx::rDASync<> Async_;
	public:
		void reset( bso::sBool P = true )
		{
			rOFlow_::reset( P );

			tol::reset( P, Async_, Relay_ );
		}
		qCDTOR( rOut_ );
		void Init( flx::rRelay_ &Core )
		{
			Relay_.Init( Core );
			rOFlow_::Init( Async_.Init( Relay_ ) );
		}
	};


	typedef sclnjs::cAsync cAsync_;

	class rRack
	: public rIn_,
	  public rOut_,
	  public cAsync_
	{
	private:
		flx::rFRelay<> Relay_;
	protected:
		virtual void SCLNJSWork( void ) override
		{
			rIn_::Read();
		}
		// Note to 'v8' user : you CAN access any of the 'v8' data from this method.
		virtual sclnjs::eBehavior SCLNJSAfter( void ) override
		{
			return rIn_::Send();
		}
	public:
		void reset( bso::sBool P = true )
		{
			rIn_::reset( P );
			rOut_::reset( P );

			tol::reset( P, Relay_ );
		}
		qCDTOR( rRack );
		void Init( sclnjs::rRStream &Stream )
		{
			Relay_.Init();

			rIn_::Init( Stream, Relay_ );
			rOut_::Init( Relay_ );

		}
	};
}

#endif
