/*
	Copyright (C) 1999 Claude SIMON (http://q37.info/contact/).

	This file is part of the Epeios framework.

	The Epeios framework is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

	The Epeios framework is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with the Epeios framework.  If not, see <http://www.gnu.org/licenses/>
*/

#ifndef XDHUPS_INC_
# define XDHUPS_INC_

# define XDHUPS_NAME		"XDHUPS"

# if defined( E_DEBUG ) && !defined( XDHUPS_NODBG )
#  define XDHUPS_DBG
# endif

// X(SL)/DH(TML) UPStream

# include "xdhcdc.h"

# include "err.h"
# include "dlbrry.h"

namespace xdhups {
	typedef ntvstr::char__ nchar__;
	typedef ntvstr::string___ nstring___;

	typedef xdhcdc::cSingle cDownstream_;

  xdhcmn::sScriptsVersion GetScriptsVersion(void);

	class cEngine
	{
	protected:
		// The value returned by the script has to be stored
		// in 'ReturnedValue', unless it is equal to 'NULL'.
		// Returns 'false' when error (mainly lost connection to client).
		virtual bso::sBool XDHUPSProcess(
			const str::dString &Script,
			tht::rBlocker *Blocker,	// If != NULL, has to be unblocked once the script sent.
			bso::sBool *Success, // If != NULL, mirrors the return value ; needed when launched in a dedicated thread.
			str::dString *ReturnedValue ) = 0;	// If == NULL, Blocker should also be NULL.
	public:
		qCALLBACK( Engine );
		bso::sBool Process(
			const str::dString &Script,
			tht::rBlocker *Blocker = NULL,	// If != 'NULL', has to be unblocked once the script sent.
			bso::sBool *Success = NULL,
			str::dString *ReturnValue = NULL)
		{
			return XDHUPSProcess(Script, Blocker, Success, ReturnValue);
		}
	};

	class rPrimitiveSession
	{
	private:
	  tht::rLocker Locker_; // Used to avoid the destruction of below 'Callback_' while being used.
		Q37_MRMDF( cEngine, C_, Callback_ );
	public:
		void reset( bso::bool__ P = true )
		{
		  if ( P ) {
        if ( Callback_ != NULL ) {
          Locker_.Lock(); // Locked downstream while 'Callback_' being used.
          Locker_.Unlock();
        }
		  }

		  Locker_.reset(P);
			Callback_ = NULL;
		}
		E_CVDTOR( rPrimitiveSession );
		void Init( cEngine &Callback )
		{
			reset();

			Locker_.Init();
			Callback_ = &Callback;
		}
		bso::sBool Execute(
      const str::dString &Primitive,
      const str::dStrings &TagValues,
      str::dString *ReturnedValue);
  };

	class sEventSession
	{
	private:
		Q37_MRMDF( cDownstream_, C_, Callback_ );
	public:
		void reset( bso::bool__ = true )
		{
			Callback_ = NULL;
		}
		E_CVDTOR( sEventSession );
		void Init(cDownstream_ &Callback)
		{
			reset();

			Callback_ = &Callback;
		}
		bso::sBool Initialize(
			xdhcuc::cSingle &Callback,
			const char *Language,
			const str::dString &Token)	// If empty, SlfH session, else token used for the FaaS session.
		{
			return C_().Initialize(Callback, Language, Token);
		}
		bso::bool__ Handle(
      const char *EventDigest,
      const str::dString &UserID);
  };

	class rSession
	: public sEventSession,
	  public rPrimitiveSession
	{
	public:
		void reset( bso::bool__ P = true )
		{
      sEventSession::reset(P);
      rPrimitiveSession::reset(P);
		}
		E_CVDTOR( rSession );
		void Init(
      cDownstream_ &DownstreamCallback,
      cEngine &EngineCallback )
		{
			reset();

			sEventSession::Init(DownstreamCallback);
			rPrimitiveSession::Init(EngineCallback);
		}
  };

  void GetBroadcastPrimitive(
    const str::dString &Action,
    const str::dString &Id,
    str::dString &Primitive,
    str::dStrings &TagValues);

	class rAgent
	{
	private:
		dlbrry::dynamic_library___ Library_;
		Q37_MRMDF( xdhcdc::cGlobal, C_, Callback_ );
		TOL_CBUFFER___ Buffer_;
	public:
		void reset( bso::bool__ P = true )
		{
			Library_.reset( P );
			Callback_ = NULL;
		}
		E_CDTOR( rAgent );
		bso::bool__ Init(
			xdhcuc::cGlobal &Upstream,
			xdhcdc::eMode Mode,
			const str::string_ &ModuleFileName,
			dlbrry::eNormalization Normalization,	// Usually 'dlbrry::n_Default', except when used for 'Node.js' (set to 'dlbrry::nExtOnly').
			const char *Identification);
		cDownstream_ *FetchCallback()
		{
			return C_().FetchCallback();
		}
		const char *BaseLanguage( TOL_CBUFFER___ &Buffer ) const
		{
			return C_().BaseLanguage( Buffer );
		}
		void DismissCallback( cDownstream_ *Callback )
		{
			return C_().DismissCallback(Callback);
		}
		const str::dString &Info( str::dString &Info )
		{
			const scli::sInfo &I = C_().GetInfo();

			Info.Append( "T: " );
			Info.Append( I.Target() );
			Info.Append( "; P: " );
			Info.Append( I.Product() );
			Info.Append( "; O: " );
			Info.Append( I.Organization() );

			return Info;
		}
		bso::sBool GetHead(
			const str::dString &Token,
			str::dString &Head)
		{
			return C_().GetHead(Token, Head);
		}
		// Deprecated.
		bso::sBool IsValid(const str::dString &Token);
	};

}

#endif
