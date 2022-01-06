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
	flw::iflow__ &IFlow,
	indice__ Indice,
	const str::string_ &Value,
	flw::oflow__ &OFlow,
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

	Success = SubstituteShortTags( IFlow, Values, OFlow, TagMarker );
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
	flx::E_STRING_OFLOW___ OFlow;
qRB
	IFlow.Init( String );
	OFlow.Init( Result );

	if ( !SubstituteShortTag( IFlow, Indice, Value, OFlow, TagMarker ) )
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
	flw::iflow__ &IFlow,
	const str::string_ &Tag,
	const str::string_ &Value,
	flw::oflow__ &OFlow,
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

	Success = SubstituteLongTags( IFlow, Tags, Values, OFlow, TagMarker );
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
	flw::iflow__ &IFlow,
	cShortTags &Callback,
	flw::oflow__ &OFlow,
	char TagMarker )
{
	bso::bool__ Success = true;
qRH
	str::string Tag, MergedValues;
	bso::char__ C = 0;
qRB
	MergedValues.Init();
	MergeValues_( Callback, MergedValues );

	while ( !IFlow.EndOfFlow() ) {
		if ( IFlow.View() == TagMarker ) {
			IFlow.Skip();

			if ( !IFlow.EndOfFlow() ) {
				if ( isdigit( C = IFlow.Get() ) ) {
					C -= '0';

					if ( C == 0 )
						MergedValues.WriteToFlow(OFlow, false);
					else {
						if ( !Callback.HandleTag(C, OFlow) ) {
							Success = false;
							break;
						}
					}
				} else if ( C == TagMarker )  {
					OFlow.Put( TagMarker );
				} else  {
					Success = false;
					break;
				}
			} else {
				Success = false;
				break;
			}
		} else
			OFlow.Put(IFlow.Get());
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
	flx::E_STRING_OFLOW___ OFlow;
qRB
	IFlow.Init( String );
	OFlow.Init( Result );

	if ( !SubstituteShortTags( IFlow, Callback, OFlow, TagMarker ) )
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
	flw::iflow__ &IFlow,
	const str::dStrings &Values,
	flw::oflow__ &OFlow,
	const char TagMarker )
{
	sShortTagsCallback_ Callback;

	Callback.Init( Values );

	return SubstituteShortTags( IFlow, Callback, OFlow, TagMarker );
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
    : public cLongTags
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
    flw::rRFlow &Flow,
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
qRB
    Flow.Init( String);

    GetLongTags(Flow,Tags,TagMarker);
qRR
qRE
qRT
}

static bso::bool__ GetTag_(
	flw::iflow__ &Flow,
	str::string_ &Tag,
	char Marker )
{
	if ( Flow.EndOfFlow() )
		qRFwk();

	if ( Flow.Get() != Marker )
		qRFwk();

	if ( Flow.EndOfFlow() )
		qRFwk();

	if ( Flow.View() == Marker ) {
		Flow.Skip();
		return false;
	}

	do {
		Tag.Append( Flow.Get() );
	} while ( !Flow.EndOfFlow() && ( Flow.View() != Marker ) );

	if ( Flow.EndOfFlow() )
		qRFwk();

	Flow.Skip();

	return true;
}

bso::bool__ tagsbs::SubstituteLongTags(
	flw::iflow__ &IFlow,
	cLongTags &Callback,
	flw::oflow__ &OFlow,
	char Marker )
{
	bso::bool__ Success = true;
qRH
	str::wString Tag;
qRB
	while ( !IFlow.EndOfFlow() ) {
		if ( IFlow.View() == Marker ) {
			Tag.Init();
			if ( GetTag_(IFlow, Tag, Marker) ) {
				if ( !Callback.HandleTag(Tag, OFlow) ) {
					Success = false;
					break;
				}
			} else
				OFlow.Put(Marker);
		} else
			OFlow.Put(IFlow.Get());
	}
qRR
qRT
qRE
	return Success;
}

tol::E_XROW tagsbs::SubstituteLongTags(
	const str::string_ &String,
	cLongTags &Callback,
	str::string_ &Result,
	char TagMarker )
{
	sdr::row__ Row = qNIL;
qRH
	flx::E_STRING_IFLOW__ IFlow;
	flx::E_STRING_OFLOW___ OFlow;
qRB
	IFlow.Init( String );
	OFlow.Init( Result );

	if ( !SubstituteLongTags( IFlow, Callback, OFlow, TagMarker ) )
		Row = IFlow.AmountRed();
qRR
qRT
qRE
	return Row;
}

tol::E_XROW tagsbs::SubstituteLongTags(
	str::string_ &String,
	cLongTags &Callback,
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
	class rLongTagsCallback_
	: public cLongTags
	{
  private:
    qCRMV(str::dStrings, V_, Values_);
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

			if ( !V_().Exists( Row ) )
				qRFwk();

			V_()(Row).WriteToFlow(Output, false);

			return true;
		}
	public:
		stsfsm::automat Automat;
		str::string BadTag;
		void reset( bso::bool__ P = true )
		{
			Automat.reset( P );
			Values_ = NULL;
			BadTag.reset( P );
		}
		E_CVDTOR( rLongTagsCallback_ );
		void Init( const str::dStrings &Values )
		{
			Automat.Init();
			Values_ = &Values;
			BadTag.Init();
		}
	};
}

bso::bool__ tagsbs::SubstituteLongTags(
	flw::iflow__ &IFlow,
	const str::dStrings &Tags,
	const str::dStrings &Values,
	flw::oflow__ &OFlow,
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

		FillAutomat_( Tags, Callback.Automat );

		Success = SubstituteLongTags( IFlow, Callback, OFlow, TagMarker );
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

	FillAutomat_( Tags, Callback.Automat );

	Row = SubstituteLongTags( String, Callback, Result, TagMarker );
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
