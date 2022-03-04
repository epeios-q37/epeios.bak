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

#include "stream.h"

#include "mtk.h"

using namespace stream;

namespace {
  qCDEF( char *, Id_, "_q37Preprocessor" );
}

namespace {
	class rRWDriver_
	: public fdr::rRWDressedDriver
	{
  private:
    flx::sStringRFlow Input_;
    flx::rStringWFlow Output_;
    tht::rBlocker UpstreamBlocker_;
    tht::rBlocker DownstreamBlocker_;
    bso::sBool
      Pending_,
      Working_,
      Finished_;
  protected:
		virtual fdr::sSize FDRRead(
			fdr::sSize Maximum,
			fdr::sByte *Buffer ) override
    {
      fdr::sSize &Amount = Maximum;

      if ( !Working_ ) {
        DownstreamBlocker_.Wait();
        Working_ = true;
      }

      if ( ( Amount = Input_.ReadUpTo(Maximum, Buffer) ) == 0 ) { // EOF; all the current chunk as been read.
        Pending_ = false;
        Working_ = false;
        UpstreamBlocker_.Unblock();
      }

      return Amount;
    }
		virtual bso::sBool FDRDismiss(
			bso::sBool Unlock,
			qRPN ) override
    {
      return true;
    }
		virtual fdr::sTID FDRRTake( fdr::sTID Owner ) override
		{
			return fdr::UndefinedTID;
		}
		virtual fdr::sSize FDRWrite(
			const fdr::sByte *Buffer,
			fdr::sSize Maximum ) override
    {
      if ( !Working_ ) {
        DownstreamBlocker_.Wait();
        Working_ = true;
      }

      Output_.Write(Buffer, Maximum);

      if ( !Pending_ ) {
        Working_ = false;
        UpstreamBlocker_.Unblock();
      }

      return Maximum;
    }
		virtual bso::sBool FDRCommit(
			bso::sBool Unlock,
			qRPN ) override
    {
      Finished_ = true;
      Output_.Commit();
      UpstreamBlocker_.Unblock();

      return true;
    }
		virtual fdr::sTID FDRWTake( fdr::sTID Owner ) override
		{
			return fdr::UndefinedTID;
		}
	public:
		void reset( bso::sBool P = true )
		{
			fdr::rRWDressedDriver::reset( P );
			tol::reset( P, Input_, Output_, DownstreamBlocker_, UpstreamBlocker_);
			Pending_ = Working_ = Finished_ = false;
		}
		qCVDTOR( rRWDriver_ );
		void Init( fdr::eThreadSafety ThreadSafety = fdr::ts_Default )
		{
			fdr::rRWDressedDriver::Init( ThreadSafety );
			// 'Input_' and 'Output' will be initialized later.
			Pending_ = Working_ = Finished_ = false;
			tol::Init(UpstreamBlocker_, DownstreamBlocker_);
			// 'Output' will be initialized later;
		}
		void Handle(
     const str::dString &Input,
      str::dString &Output)
    {
      Finished_ = false;

      Input_.Init(Input);
      Output_.Init(Output);

      Pending_ = true;

      DownstreamBlocker_.Unblock();
      UpstreamBlocker_.Wait();
    }
    void Finish(str::dString &Output)
    {
      if ( !Finished_ ) {
        Output_.Init(Output);

        Pending_ = true;
        DownstreamBlocker_.Unblock();
        UpstreamBlocker_.Wait();
      }
    }
	};

	typedef flw::rXDressedRWFlow<rRWDriver_> rRWFlow_;

	struct sData_ {
	  rRWFlow_ *Flow;
	  str::dString *Error;
	};

	void Routine(
    void *UP,
    mtk::gBlocker &Blocker)
  {
  qRH;
    xpp::eStatus Status = xpp::s_Undefined;
    xpp::rContext Context;
    lcl::wMeaning Meaning;
  qRB;
    const sData_ &Data = *(const sData_ *)UP;

    rRWFlow_ &Flow = *Data.Flow;
    str::dString &Error = *Data.Error;

    Blocker.Release();

    xtf::sRFlow XFlow;
    txf::sWFlow TFlow;

    XFlow.Init(Flow, utf::f_Default);
    TFlow.Init(Flow);

    Context.Init();

    if ( ( Status = xpp::Process(XFlow, xpp::rCriterions(""), xml::oIndent, TFlow, Context) ) != xpp::sOK ) {
      Meaning.Init();
      xpp::GetMeaning(Context, Meaning);
      sclm::GetBaseTranslation(Meaning, Error);
    }
  qRR;
  qRT;
  qRE;
  }

	class rPreprocessor_
	: public rRWFlow_
	{
  public:
    str::wString Error;
    void reset(bso::sBool P = true)
    {
      rRWFlow_::reset(P);
      Error.reset(P);
    }
    qCVDTOR(rPreprocessor_);
    void Init(void)
    {
      sData_ Data;

      subInit();
      Driver_.Init();
      Error.Init();

      Data.Flow = this;
      Data.Error = &Error;

      mtk::Launch(Routine, &Data);
    }
		void Handle(
      const str::dString &Input,
      str::dString &Output)
    {
      return Driver_.Handle(Input, Output);
    }
    void Finish(str::dString &Output)
    {
      return Driver_.Finish(Output);
    }
  };
}

SCLNJS_F(stream::Set)
{
qRH
	sclnjs::rObject This;
	rPreprocessor_ *Preprocessor = NULL;
qRB
  Preprocessor = qNEW(rPreprocessor_);

	tol::Init(This);
	Preprocessor->Init();

	Caller.GetArgument( This );

	This.Set(Id_, Preprocessor);
qRR
  qDELETE(Preprocessor);
qRT
qRE
}

SCLNJS_F(stream::Transform)
{
qRH
	sclnjs::rObject This;
	sclnjs::rBuffer Chunk;
	str::wString Input, Output;
qRB
	tol::Init(This, Chunk, Input, Output);
	Caller.GetArgument(This, Chunk);

	Chunk.ToString(Input);

	rPreprocessor_ &Preprocessor = This.Get<rPreprocessor_>(Id_);

	Preprocessor.Handle(Input, Output);

	if ( Preprocessor.Error.Amount() )
    Caller.SetReturnValueAsNull();
  else
    Caller.SetReturnValue(Output);
qRR
qRT
qRE
}

SCLNJS_F(stream::Flush)
{
qRH
	sclnjs::rObject This;
	str::wString Output;
	rPreprocessor_ *Preprocessor = NULL;
qRB
	tol::Init(This, Output);

	Caller.GetArgument( This );

	Preprocessor = This.GetP<rPreprocessor_>(Id_);

	Preprocessor->Finish(Output);

	if ( Preprocessor->Error.Amount() )
    Caller.SetReturnValueAsNull();
  else
    Caller.SetReturnValue(Output);
qRR
qRE
  qDELETE(Preprocessor);
qRT
}

SCLNJS_F(stream::GetError)
{
qRH
	sclnjs::rObject This;
qRB
	tol::Init(This);

	Caller.GetArgument( This );

	rPreprocessor_ &Preprocessor = This.Get<rPreprocessor_>(Id_);

	if ( Preprocessor.Error.Amount() == 0 )
    qRGnr();

  Caller.SetReturnValue(Preprocessor.Error);
qRR
qRE
qRT
}
