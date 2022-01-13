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

#define TAGSBS_COMPILATION_

#include "tagsbs.h"

#include "stsfsm.h"
#include "uys.h"
#include "flx.h"

using namespace tagsbs;

namespace {
  E_CDEF( sIndice, Limit_, 9 );
}

bso::bool__ tagsbs::SubstituteShortTag(
	xtf::sRFlow &Input,
	indice__ Indice,
	const str::string_ &Value,
	flw::oflow__ &Output,
	char TagMarker )
{
	bso::bool__ Success = false;
qRH
	str::wStrings Values;
	indice__ Counter = 1;
	str::string Tag;
qRB
	if ( ( Indice == 0 ) || ( Indice > 9 ) )
		qRFwk();

	Values.Init();

	Tag.Init();
	Tag.Append(TagMarker);
	Tag.Append('0' );

	while ( Counter <= Limit_ ) {
		if ( Counter != Indice ) {
			Tag.Store( Counter + '0', Tag.Last() );
			Values.Append( Tag );
		} else
			Values.Append( Value );

		Counter++;
	}

	Success = SubstituteShortTags(Input, Values, Output, TagMarker);
qRR
qRT
qRE
	return Success;
}

tol::E_XROW tagsbs::SubstituteShortTag(
	const str::string_ &String,
	indice__ Indice,
	const str::string_ &Value,
	str::string_ &Result,
	const char TagMarker )
{
	sdr::row__ Row = qNIL;
qRH
	flx::E_STRING_IFLOW__ IFlow;
	xtf::sRFlow XFlow;
	flx::E_STRING_OFLOW___ OFlow;
qRB
	IFlow.Init( String );
	XFlow.Init(IFlow, utf::f_Default);
	OFlow.Init( Result );

	if ( !SubstituteShortTag( XFlow, Indice, Value, OFlow, TagMarker ) )
		Row = IFlow.AmountRed();
qRR
qRT
qRE
	return Row;
}

tol::E_XROW tagsbs::SubstituteShortTag(
	str::string_ &String,
	indice__ Indice,
	const str::string_ &Value,
	char TagMarker )
{
	sdr::row__ Row = qNIL;
qRH
	str::string Result;
qRB
	Result.Init();

	Row = SubstituteShortTag( String, Indice, Value, Result, TagMarker );

	String = Result;
qRR
qRT
qRE
	return Row;
}

bso::bool__ tagsbs::SubstituteLongTag(
	xtf::sRFlow &Input,
	const str::string_ &Tag,
	const str::string_ &Value,
	flw::oflow__ &Output,
	char TagMarker )
{
	bso::bool__ Success = false;
qRH
	str::wStrings Tags, Values;
qRB
	Tags.Init();
	Tags.Append( Tag ) ;

	Values.Init();
	Values.Append( Value );

	Success = SubstituteLongTags(Input, Tags, Values, Output, TagMarker);
qRR
qRT
qRE
	return Success;
}

tol::E_XROW tagsbs::SubstituteLongTag(
	const str::string_ &String,
	const str::string_ &Tag,
	const str::string_ &Value,
	str::string_ &Result,
	char TagMarker )
{
	sdr::row__ Row = qNIL;
qRH
	str::wStrings Tags, Values;
qRB
	Tags.Init();
	Tags.Append( Tag ) ;

	Values.Init();
	Values.Append( Value );

	Row = SubstituteLongTags( String, Tags, Values, Result, TagMarker );
qRR
qRT
qRE
	return Row;
}

tol::E_XROW tagsbs::SubstituteLongTag(
	str::string_ &String,
	const str::string_ &Tag,
	const str::string_ &Value,
	char TagMarker )
{
	sdr::row__ Row = qNIL;
qRH
	str::string Result;
qRB
	Result.Init();

	Row = SubstituteLongTag( String, Tag, Value, Result, TagMarker );

	String = Result;
qRR
qRT
qRE
	return Row;
}

static void MergeValues_(
	cShortTags &Callback,
	str::string_ &MergedValues )
{
qRH
	str::wString Value;
	indice__ Indice = 0;
	flx::rStringWFlow Flow;
qRB
	Value.Init();
	Flow.Init(Value);

	while ( ( Indice <= Limit_ ) && ( !Callback.HandleTag(Indice, Flow) ) )
		Indice++;

  Flow.Commit();

	if ( Indice <= Limit_ ) {
		MergedValues.Append( "(" );
		MergedValues.Append( '\'' );
		MergedValues.Append( Value );
		MergedValues.Append( '\'' );
		Indice++;

		while ( Indice <= Limit_ ) {
      Value.Init();
			if ( Callback.HandleTag(Indice, Flow) ) {
			  Flow.Commit();
				MergedValues.Append( " ,'" );
				MergedValues.Append( Value );
				MergedValues.Append( '\'' );
			}

			Indice++;
		}

		MergedValues.Append( ")" );
	}

qRR
qRT
qRE
}

bso::bool__ tagsbs::SubstituteShortTags(
	xtf::sRFlow &Input,
	cShortTags &Callback,
	flw::oflow__ &Output,
	char TagMarker )
{
	bso::bool__ Success = true;
qRH
	str::string Tag, MergedValues;
	bso::char__ C = 0;
  xtf::sUTF UTF;
qRB
	MergedValues.Init();
	MergeValues_( Callback, MergedValues );

	while ( !Input.EndOfFlow() ) {
		if ( Input.View() == TagMarker ) {
			Input.Get();

			if ( !Input.EndOfFlow() ) {
				if ( isdigit( C = Input.Get() ) ) {
					C -= '0';

					if ( C == 0 )
						MergedValues.WriteToFlow(Output, false);
					else {
						if ( !Callback.HandleTag(C, Output) ) {
							Success = false;
							break;
						}
					}
				} else if ( C == TagMarker )  {
					Output.Put( TagMarker );
				} else  {
					Success = false;
					break;
				}
			} else {
				Success = false;
				break;
			}
		} else {
      UTF.Init();
      Input.Get(UTF);
			Output.Write(UTF.Data, UTF.Size);
		}
	}
qRR
qRT
qRE
	return Success;
}

tol::E_XROW tagsbs::SubstituteShortTags(
	const str::string_ &String,
	cShortTags &Callback,
	str::string_ &Result,
	char TagMarker )
{
	sdr::row__ Row = qNIL;
qRH
	flx::E_STRING_IFLOW__ IFlow;
	xtf::sRFlow XFlow;
	flx::E_STRING_OFLOW___ OFlow;
qRB
	IFlow.Init( String );
	XFlow.Init(IFlow, utf::f_Default);
	OFlow.Init( Result );

	if ( !SubstituteShortTags( XFlow, Callback, OFlow, TagMarker ) )
		Row = IFlow.AmountRed();
qRR
qRT
qRE
	return Row;
}

tol::E_XROW  tagsbs::SubstituteShortTags(
	str::string_ &String,
	cShortTags &Callback,
	char TagMarker )
{
	sdr::row__ Row = qNIL;
qRH
	str::string Result;
qRB
	Result.Init();

	Row = SubstituteShortTags( String, Callback, Result, TagMarker );

	String = Result;
qRR
qRT
qRE
	return Row;
}


/// 'namespace {...}' parce que sinon VC++ 12 se mlange les pinceaux...
namespace {
	class sShortTagsCallback_
	: public cShortTags
	{
  private:
    qCRMV(str::dStrings, V_, Values_)
	protected:
		virtual bso::bool__ TAGSBSHandleTag(
			indice__ Indice,
			flw::rWFlow &Output) override
		{
			sdr::row__ Row = Indice - 1;

			if ( !V_().Exists( Row ) )
				return false;

      V_()(Row).WriteToFlow(Output, false);

			return true;
		}
	public:
		void reset( bso::bool__ P = true )
		{
			Values_ = NULL;
		}
		E_CVDTOR( sShortTagsCallback_ );
		void Init(const str::dStrings &Values)
		{
			Values_ = &Values;
		}
	};
}

bso::bool__ tagsbs::SubstituteShortTags(
	xtf::sRFlow &Input,
	const str::dStrings &Values,
	flw::oflow__ &Output,
	const char TagMarker )
{
	sShortTagsCallback_ Callback;

	Callback.Init( Values );

	return SubstituteShortTags(Input, Callback, Output, TagMarker);
}

tol::E_XROW tagsbs::SubstituteShortTags(
	const str::string_ &String,
	const str::dStrings &Values,
	str::string_ &Result,
	const char TagMarker )
{
	sShortTagsCallback_ Callback;

	Callback.Init( Values );

	return SubstituteShortTags( String, Callback, Result, TagMarker );
}

tol::E_XROW  tagsbs::SubstituteShortTags(
	str::string_ &String,
	const str::dStrings &Values,
	char TagMarker )
{
	sdr::row__ Row = qNIL;
qRH
	str::string Result;
qRB
	Result.Init();

	Row = SubstituteShortTags( String, Values, Result, TagMarker );

	String = Result;
qRR
qRT
qRE
	return Row;
}

namespace {
    class rGetTagsCallback_
    : public cLongTagsString
    {
    protected:
  		virtual bso::bool__ TAGSBSHandleTag(
        const str::dString &Tag,
        flw::rWFlow &) override
			{
        if ( str::Search(Tag,Tags) == qNIL)
            Tags.Append(Tag);

        return true;
			}
    public:
        str::wStrings Tags;
        void reset( bso::sBool P = true )
        {
            Tags.reset(P);
        }
        qCVDTOR( rGetTagsCallback_);
        void Init(void)
        {
            Tags.Init();
        }
    };
}

void tagsbs::GetLongTags(
    xtf::sRFlow &Flow,
    str::dStrings &Tags,
    bso::sChar TagMarker )
{
qRH
    rGetTagsCallback_ Callback;
    sdr::sRow Row = qNIL;
qRB
    Callback.Init();

    SubstituteLongTags(Flow, Callback, flx::VoidWFlow, TagMarker);

    Row = Callback.Tags.First();

    while ( Row != qNIL ) {
        Tags.Append(Callback.Tags(Row));

        Row = Callback.Tags.Next(Row);
    }
qRR
qRE
qRT
}

void tagsbs::GetLongTags(
    const str::dString &String,
    str::dStrings &Tags,
    bso::sChar TagMarker )
{
qRH
    flx::sStringRFlow Flow;
    xtf::sRFlow XFlow;
qRB
    Flow.Init( String);
    XFlow.Init(Flow, utf::f_Default);

    GetLongTags(XFlow,Tags,TagMarker);
qRR
qRE
qRT
}

static bso::bool__ GetTag_(
	xtf::sRFlow &Flow,
	str::string_ &Tag,
	char Marker,
	bso::sBool &Error )
{
  bso::sChar C = 0;

	if ( Flow.EndOfFlow() )
		qRFwk();

	if ( Flow.Get() != Marker )
		qRFwk();

  if ( Flow.EndOfFlow() ) {
    Error = true;
    return false;
  }

	if ( Flow.View() == Marker ) {
		Flow.Get();
		return false;
	} else {
    do {
      C = Flow.Get();
      if ( isalnum(C) || ( C == '_' ) )
        Tag.Append(C);
      else {
        Error = true;
        return false;
      }
    } while ( !Flow.EndOfFlow() && ( Flow.View() != Marker ) );

    if ( Flow.EndOfFlow() ) {
      Error = true;
      return false;
    }

    Flow.Get();
	}

  return true;
}

bso::bool__ tagsbs::SubstituteLongTags(
	xtf::sRFlow &Input,
	cLongTagsString &Callback,
	flw::oflow__ &Output,
	char Marker )
{
	bso::sBool Error = false;
qRH
	str::wString Tag;
	xtf::sUTF UTF;
qRB
	while ( !Input.EndOfFlow() && !Error ) {
		if ( Input.View() == Marker ) {
			Tag.Init();
			if ( GetTag_(Input, Tag, Marker, Error) ) {
				if ( !Callback.HandleTag(Tag, Output) ) {
					Error = true;
				}
			} else if ( !Error )
				Output.Put(Marker);
		} else {
		  UTF.Init();
		  Input.Get(UTF);
			Output.Write(UTF.Data, UTF.Size);
		}
	}
qRR
qRT
qRE
	return !Error;
}

tol::E_XROW tagsbs::SubstituteLongTags(
	const str::string_ &String,
	cLongTagsString &Callback,
	str::string_ &Result,
	char TagMarker )
{
	sdr::row__ Row = qNIL;
qRH
	flx::E_STRING_IFLOW__ IFlow;
	xtf::sRFlow XFlow;
	flx::E_STRING_OFLOW___ OFlow;
qRB
	IFlow.Init( String );
	XFlow.Init(IFlow, utf::f_Default);
	OFlow.Init( Result );

	if ( !SubstituteLongTags( XFlow, Callback, OFlow, TagMarker ) )
		Row = IFlow.AmountRed();
qRR
qRT
qRE
	return Row;
}

tol::E_XROW tagsbs::SubstituteLongTags(
	str::string_ &String,
	cLongTagsString &Callback,
	char TagMarker )
{
	sdr::row__ Row = qNIL;
qRH
	str::string Result;
qRB
	Result.Init();

	Row = SubstituteLongTags( String, Callback, Result, TagMarker );

	String = Result;
qRR
qRT
qRE
	return Row;
}

static void FillAutomat_(
	const str::dStrings &Tags,
	stsfsm::automat_ &Automat )
{
	sdr::row__ Row = Tags.First();

	while ( Row != qNIL ) {
		stsfsm::Add( Tags( Row ), *Row, Automat );

		Row = Tags.Next( Row );
	}
}

/// 'namespace {...}' parce que sinon VC++ 12 se mlange les pinceaux...
namespace {
	class rLongTagsString2Row_
	: public cLongTagsString
	{
  private:
    qRMV(cLongTagsRow, C_, Callback_);
	protected:
		virtual bso::bool__ TAGSBSHandleTag(
			const str::string_ &Tag,
			flw::rWFlow &Output) override
		{
			stsfsm::parser__ Parser;
			sdr::row__ Row = qNIL;

			Parser.Init( Automat );

			if ( Parser.Handle( Tag ) != stsfsm::sMatch ) {
				BadTag = Tag;
				return false;
			}

			Row = Parser.GetId();

			return C_().HandleTag(Parser.GetId(), Output);
		}
	public:
		stsfsm::automat Automat;
		str::string BadTag;
		void reset( bso::bool__ P = true )
		{
			Automat.reset( P );
			Callback_ = NULL;
			BadTag.reset( P );
		}
		E_CVDTOR( rLongTagsString2Row_ );
		void Init(cLongTagsRow &Callback)
		{
			Automat.Init();
			Callback_ = &Callback;
			BadTag.Init();
		}
	};
}

bso::bool__ tagsbs::SubstituteLongTags(
  xtf::sRFlow &Input,
  const str::dStrings &Tags,
  cLongTagsRow &Callback,
  flw::oflow__ &Output,
  char Delimiter)// Si la valeur retourne != 'qNIL', elle indique la position problmatique dans la chane.
{
	bso::bool__ Success = true;
qRH
  rLongTagsString2Row_ String2RowCallback;
qRB
  String2RowCallback.Init(Callback);
  FillAutomat_(Tags, String2RowCallback.Automat);

  Success = SubstituteLongTags(Input, String2RowCallback, Output, Delimiter);
qRR
qRT
qRE
	return Success;
}

tol::E_XROW tagsbs::SubstituteLongTags(
	const str::string_ &String,
  const str::dStrings &Tags,
  cLongTagsRow &Callback,
	str::string_ &Result,
	char TagMarker )
{
	sdr::row__ Row = qNIL;
qRH
	flx::E_STRING_IFLOW__ IFlow;
	xtf::sRFlow XFlow;
	flx::E_STRING_OFLOW___ OFlow;
qRB
	IFlow.Init( String );
	XFlow.Init(IFlow, utf::f_Default);
	OFlow.Init( Result );

	if ( !SubstituteLongTags( XFlow, Tags, Callback, OFlow, TagMarker ) )
		Row = IFlow.AmountRed();
qRR
qRT
qRE
	return Row;
}

tol::E_XROW tagsbs::SubstituteLongTags(
	str::string_ &String,
  const str::dStrings &Tags,
  cLongTagsRow &Callback,
	char TagMarker )
{
	sdr::row__ Row = qNIL;
qRH
	str::string Result;
qRB
	Result.Init();

	Row = SubstituteLongTags( String, Tags, Callback, Result, TagMarker );

	String = Result;
qRR
qRT
qRE
	return Row;
}



/// 'namespace {...}' parce que sinon VC++ 12 se mlange les pinceaux...
namespace {
	class rLongTagsCallback_
	: public cLongTagsRow
	{
  private:
    qCRMV(str::dStrings, V_, Values_);
	protected:
		virtual bso::bool__ TAGSBSHandleTag(
			sdr::sRow TagRow,
			flw::rWFlow &Output) override
		{
			if ( !V_().Exists(TagRow) )
				qRFwk();

			V_()(TagRow).WriteToFlow(Output, false);

			return true;
		}
	public:
		str::string BadTag;
		void reset( bso::bool__ P = true )
		{
			Values_ = NULL;
			BadTag.reset( P );
		}
		E_CVDTOR( rLongTagsCallback_ );
		void Init( const str::dStrings &Values )
		{
			Values_ = &Values;
			BadTag.Init();
		}
	};
}

bso::bool__ tagsbs::SubstituteLongTags(
	xtf::sRFlow &Input,
	const str::dStrings &Tags,
	const str::dStrings &Values,
	flw::oflow__ &Output,
	char TagMarker )
{
	bso::bool__ Success = false;
qRH
	rLongTagsCallback_ Callback;
qRB
	if ( Tags.Amount() != Values.Amount() )
		qRFwk();

	if ( Tags.Amount() != 0 ) {
		Callback.Init( Values );

		Success = SubstituteLongTags(Input, Tags, Callback, Output, TagMarker);
	} else
		Success = true;
qRR
qRT
qRE
	return Success;
}


tol::E_XROW tagsbs::SubstituteLongTags(
	const str::string_ &String,
	const str::dStrings &Tags,
	const str::dStrings &Values,
	str::string_ &Result,
	char TagMarker )
{
	sdr::row__ Row = qNIL;
qRH
  rLongTagsCallback_ Callback;
qRB
	if ( Tags.Amount() != Values.Amount() )
		qRFwk();

	Callback.Init( Values );

	Row = SubstituteLongTags( String, Tags, Callback, Result, TagMarker );
qRR
qRT
qRE
	return Row;
}

tol::E_XROW tagsbs::SubstituteLongTags(
	str::string_ &String,
	const str::dStrings &Tags,
	const str::dStrings &Values,
	char TagMarker )
{
	sdr::row__ Row = qNIL;
qRH
	str::string Result;
qRB
	Result.Init();

	Row = SubstituteLongTags( String, Tags, Values, Result, TagMarker );

	String = Result;
qRR
qRT
qRE
	return Row;
}
