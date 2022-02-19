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

// The preprocessing stream.

#ifndef STREAM_INC_
# define STREAM_INC_

# include "sclnjs.h"

# include "err.h"
# include "txf.h"

namespace stream {
  namespace shared {
    typedef txf::rWFlow rOFlow_;

    class rIn_ {
    private:
      qRMV( sclnjs::rObject, S_, Stream_ );
      bso::sBool IsFirst_;
      str::wString Buffer_;
      tht::rBlocker Blocker_;
      flx::sIRelay Relay_;
      flw::rDressedRFlow<> Flow_;
      xtf::sRFlow XFlow_;
      xpp::rRFlow Preprocessor_;
    protected:
      void Read( void )
      {
      qRH
        flx::rStringOFlow SFlow;
      qRB
        if ( IsFirst_ ) {
          IsFirst_ = false;
          XFlow_.Init( Flow_, utf::f_Guess );
          Preprocessor_.Init( XFlow_, xpp::rCriterions( "" ) );
        }

        Blocker_.Wait();	// Waits until '_read()' is called.

        SFlow.Init( Buffer_ );

        flw::Copy( Preprocessor_, SFlow );
      qRR
      qRT
      qRE
      }
      sclnjs::eBehavior Send( void )
      {
        qRVct();

        /* The use of a fixed size buffer, with several time calling the above function to fill it
        and then 'push()' the content, will crash the preprocessor. But, even in this case,
        when the preprocessor is handled by the parser, it will NOT crash (???). */
//        S_().Push( Buffer_ );
//        S_().End();

        return sclnjs::bExitAndDelete;
      }
    public:
      void reset( bso::sBool P = true )
      {
        if ( P ) {
          if ( Stream_ != NULL )
            delete Stream_;
        }

        IsFirst_ = false;

        Preprocessor_.reset( P );

        tol::reset( P, Stream_, Blocker_, Relay_, Flow_, XFlow_, Buffer_ );
      }
      qCDTOR( rIn_ );
      void Init(
        sclnjs::rObject &Stream,
        flx::rRelay_ &Core )
      {
        Stream_ = &Stream;
        IsFirst_ = true;

        tol::Init( Blocker_, Buffer_ );

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
      void Init( sclnjs::rObject &Stream )
      {
        Relay_.Init();

        rIn_::Init( Stream, Relay_ );
        rOut_::Init( Relay_ );

      }
    };
  }

  namespace upstream {
    // Both are called in the JS 'onReadable' function.
    void OnData( sclnjs::sCaller &Caller );	// Called when some data is available.
    void OnEnd( sclnjs::sCaller &Caller );	// Called when no more data is available.
  }

  namespace downstream {
    void Read( sclnjs::sCaller &Caller );	// Called when asked for data, in the JS '_read()') function.
  }

	SCLNJS_F( _Set );
}

#endif
