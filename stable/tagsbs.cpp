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

#define TAGSBS__COMPILATION

#include "tagsbs.h"

#include "uys.h"
#include "flx.h"

using namespace tagsbs;

E_CDEF( indice__, Limit, 9 );

bso::bool__ tagsbs::SubstituteShortTag(
	flw::iflow__ &IFlow,
	indice__ Indice,
	const str::string_ &Value,
	flw::oflow__ &OFlow,
	char TagMarker )
{
	bso::bool__ Success = false;
qRH
	str::strings Values;
	indice__ Counter = 1;
	str::string Tag;
qRB
	if ( ( Indice == 0 ) || ( Indice > 9 ) )
		qRFwk();

	Values.Init();

	Tag.Init();
	Tag.Append( TagMarker);
	Tag.Append('0' );

	while ( Counter <= Limit ) {
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
	str::strings Tags, Values;
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
	str::strings Tags, Values;
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
	short_tags_callback__ &Callback,
	str::string_ &MergedValues )
{
qRH
	str::string Value;
	indice__ Indice = 0;
qRB

	Value.Init();

	while ( ( Indice <= Limit ) && ( !Callback.GetTagValue( Indice, Value ) ) )
		Indice++;

	if ( Indice <= Limit ) {
		MergedValues.Append( "(" );
		MergedValues.Append( '\'' );
		MergedValues.Append( Value );
		MergedValues.Append( '\'' );
		Indice++;



		while ( Indice <= Limit ) {
			if ( Callback.GetTagValue(Indice, Value) ) {
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
	short_tags_callback__ &Callback,
	flw::oflow__ &OFlow,
	char TagMarker )
{
	bso::bool__ Success = true;
qRH
	str::string Tag, Value, MergedValues;
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
						MergedValues.WriteToFlow( OFlow, false );
					else {
						Value.Init();
						if ( !Callback.GetTagValue( C, Value ) ) {
							Success = false;
							break;
						}

						Value.WriteToFlow( OFlow, false );
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
			OFlow.Put( IFlow.Get() );
	}
qRR
qRT
qRE
	return Success;
}

tol::E_XROW tagsbs::SubstituteShortTags(
	const str::string_ &String,
	short_tags_callback__ &Callback,
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
	short_tags_callback__ &Callback,
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
	class shorts_callback___
	: public short_tags_callback__
	{
	protected:
		virtual bso::bool__ TAGSBSGetTagValue(
			indice__ Indice,
			str::string_ &Value )
		{
			sdr::row__ Row = Indice - 1;

			if ( !Values->Exists( Row ) )
				return false;

			Values->Recall( Row, Value );

			return true;
		}
	public:
		const str::strings_ *Values;
		void reset( bso::bool__ P = true )
		{
			short_tags_callback__::reset( P );
			Values = NULL;
		}
		E_CVDTOR( shorts_callback___ );
		void Init( const str::strings_ &Values )
		{
			short_tags_callback__::Init();
			this->Values = &Values;
		}
	};
}

bso::bool__ tagsbs::SubstituteShortTags(
	flw::iflow__ &IFlow,
	const str::strings_ &Values,
	flw::oflow__ &OFlow,
	const char TagMarker )
{
	shorts_callback___ Callback;

	Callback.Init( Values );

	return SubstituteShortTags( IFlow, Callback, OFlow, TagMarker );
}

tol::E_XROW tagsbs::SubstituteShortTags(
	const str::string_ &String,
	const str::strings_ &Values,
	str::string_ &Result,
	const char TagMarker )
{
	shorts_callback___ Callback;

	Callback.Init( Values );

	return SubstituteShortTags( String, Callback, Result, TagMarker );
}

tol::E_XROW  tagsbs::SubstituteShortTags(
	str::string_ &String,
	const str::strings_ &Values,
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
	long_tags_callback__ &Callback,
	flw::oflow__ &OFlow,
	char Marker )
{
	bso::bool__ Success = true;
qRH
	str::string Tag, Value;
qRB
	while ( !IFlow.EndOfFlow() ) {
		if ( IFlow.View() == Marker ) {
			Tag.Init();
			if ( GetTag_( IFlow, Tag, Marker ) ) {

				Value.Init();
				if ( !Callback.GetTagValue( Tag, Value ) ) {
					Success = false;
					break;
				}

				Value.WriteToFlow( OFlow, false );
			} else
				OFlow.Put( Marker );
		} else
			OFlow.Put( IFlow.Get() );
	}
qRR
qRT
qRE
	return Success;
}

tol::E_XROW tagsbs::SubstituteLongTags(
	const str::string_ &String,
	long_tags_callback__ &Callback,
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
	long_tags_callback__ &Callback,
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
	const str::strings_ &Tags,
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
	class longs_callback___
	: public long_tags_callback__
	{
	protected:
		virtual bso::bool__ TAGSBSGetTagValue(
			const str::string_ &Tag,
			str::string_ &Value )
		{
			stsfsm::parser__ Parser;
			sdr::row__ Row = qNIL;

			Parser.Init( Automat );

			if ( Parser.Handle( Tag ) != stsfsm::sMatch ) {
				BadTag = Tag;
				return false;
			}

			Row = Parser.GetId();

			if ( !Values->Exists( Row ) )
				qRFwk();

			Values->Recall( Row, Value );

			return true;
		}
	public:
		stsfsm::automat Automat;
		const str::strings_ *Values;
		str::string BadTag;
		void reset( bso::bool__ P = true )
		{
			long_tags_callback__::reset( P );
			Automat.reset( P );
			Values = NULL;
			BadTag.reset( P );
		}
		E_CVDTOR( longs_callback___ );
		void Init( const str::strings_ &Values )
		{
			long_tags_callback__::Init();
			Automat.Init();
			this->Values = &Values;
			BadTag.Init();
		}
	};
}

bso::bool__ tagsbs::SubstituteLongTags(
	flw::iflow__ &IFlow,
	const str::strings_ &Tags,
	const str::strings_ &Values,
	flw::oflow__ &OFlow,
	char TagMarker )
{
	bso::bool__ Success = false;
qRH
	longs_callback___ Callback;
qRB
	if ( Tags.Amount() != Values.Amount() )
		qRFwk();

	if ( Tags.Amount() != Values.Amount() )
		qRFwk();

	if ( Tags.Amount() == 0 ) {
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
	const str::strings_ &Tags,
	const str::strings_ &Values,
	str::string_ &Result,
	char TagMarker )
{
	sdr::row__ Row = qNIL;
qRH
	longs_callback___ Callback;
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
	const str::strings_ &Tags,
	const str::strings_ &Values,
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
