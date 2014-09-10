/*
	'tagsbs.cpp' by Claude SIMON (http://zeusw.org/).

	'tagsbs' is part of the Epeios framework.

    The Epeios framework is free software: you can redistribute it and/or
	modify it under the terms of the GNU General Public License as published
	by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Epeios framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The Epeios framework.  If not, see <http://www.gnu.org/licenses/>.
*/

#define TAGSBS__COMPILATION

#include "tagsbs.h"

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

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
ERRProlog
	str::strings Values;
	indice__ Counter = 1;
	str::string Tag;
ERRBegin
	if ( ( Indice == 0 ) || ( Indice > 9 ) )
		ERRPrm();

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
ERRErr
ERREnd
ERREpilog
	return Success;
}

tol::E_XROW tagsbs::SubstituteShortTag(
	const str::string_ &String,
	indice__ Indice,
	const str::string_ &Value,
	str::string_ &Result,
	const char TagMarker )
{
	sdr::row__ Row = E_NIL;
ERRProlog
	flx::E_STRING_IFLOW__ IFlow;
	flx::E_STRING_OFLOW___ OFlow;
ERRBegin
	IFlow.Init( String );
	OFlow.Init( Result );

	if ( !SubstituteShortTag( IFlow, Indice, Value, OFlow, TagMarker ) )
		Row = IFlow.AmountRed();
ERRErr
ERREnd
ERREpilog
	return Row;
}

tol::E_XROW tagsbs::SubstituteShortTag(
	str::string_ &String,
	indice__ Indice,
	const str::string_ &Value,
	char TagMarker )
{
	sdr::row__ Row = E_NIL;
ERRProlog
	str::string Result;
ERRBegin
	Result.Init();
	
	Row = SubstituteShortTag( String, Indice, Value, Result, TagMarker );

	String = Result;
ERRErr
ERREnd
ERREpilog
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
ERRProlog
	str::strings Tags, Values;
ERRBegin
	Tags.Init();
	Tags.Append( Tag ) ;

	Values.Init();
	Values.Append( Value );

	Success = SubstituteLongTags( IFlow, Tags, Values, OFlow, TagMarker );
ERRErr
ERREnd
ERREpilog
	return Success;
}

tol::E_XROW tagsbs::SubstituteLongTag(
	const str::string_ &String,
	const str::string_ &Tag,
	const str::string_ &Value,
	str::string_ &Result,
	char TagMarker )
{
	sdr::row__ Row = E_NIL;
ERRProlog
	str::strings Tags, Values;
ERRBegin
	Tags.Init();
	Tags.Append( Tag ) ;

	Values.Init();
	Values.Append( Value );

	Row = SubstituteLongTags( String, Tags, Values, Result, TagMarker );
ERRErr
ERREnd
ERREpilog
	return Row;
}

tol::E_XROW tagsbs::SubstituteLongTag(
	str::string_ &String,
	const str::string_ &Tag,
	const str::string_ &Value,
	char TagMarker )
{
	sdr::row__ Row = E_NIL;
ERRProlog
	str::string Result;
ERRBegin
	Result.Init();
	
	Row = SubstituteLongTag( String, Tag, Value, Result, TagMarker );

	String = Result;
ERRErr
ERREnd
ERREpilog
	return Row;
}

static void MergeValues_(
	short_tags_callback__ &Callback,
	str::string_ &MergedValues )
{
ERRProlog
	str::string Value;
	indice__ Indice = 0;
ERRBegin
	MergedValues.Append( " (" );

	Value.Init();

	while ( ( Indice <= Limit ) && ( !Callback.GetTagValue( Indice, Value ) ) )
		Indice++;

	if ( Indice <= Limit ) {
		MergedValues.Append( '\'' );
		MergedValues.Append( Value );
		MergedValues.Append( '\'' );
		Indice++;
	}

	while ( Indice <= Limit ) {
		if ( Callback.GetTagValue(Indice, Value) ) {
			MergedValues.Append( " ,'" );
			MergedValues.Append( Value );
			MergedValues.Append( '\'' );
		}

		Indice++;
	}

	MergedValues.Append( ") " );
ERRErr
ERREnd
ERREpilog
}

bso::bool__ tagsbs::SubstituteShortTags(
	flw::iflow__ &IFlow,
	short_tags_callback__ &Callback,
	flw::oflow__ &OFlow,
	char TagMarker )
{
	bso::bool__ Success = false;
ERRProlog
	str::string Tag, Value, MergedValues;
	bso::char__ C = 0;
ERRBegin
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
						if ( !Callback.GetTagValue( C, Value ) )
							ERRReturn;
						Value.WriteToFlow( OFlow, false );
					}
				} else if ( C == TagMarker )  {
					OFlow.Put( TagMarker );
				} else  {
					ERRReturn;
				}
			} else {
				ERRReturn;
			}
		} else
			OFlow.Put( IFlow.Get() );
	}

	Success = true;
ERRErr
ERREnd
ERREpilog
	return Success;
}

tol::E_XROW tagsbs::SubstituteShortTags(
	const str::string_ &String,
	short_tags_callback__ &Callback,
	str::string_ &Result,
	char TagMarker )
{
	sdr::row__ Row = E_NIL;
ERRProlog
	flx::E_STRING_IFLOW__ IFlow;
	flx::E_STRING_OFLOW___ OFlow;
ERRBegin
	IFlow.Init( String );
	OFlow.Init( Result );

	if ( !SubstituteShortTags( IFlow, Callback, OFlow, TagMarker ) )
		Row = IFlow.AmountRed();
ERRErr
ERREnd
ERREpilog
	return Row;
}

tol::E_XROW  tagsbs::SubstituteShortTags(
	str::string_ &String,
	short_tags_callback__ &Callback,
	char TagMarker )
{
	sdr::row__ Row = E_NIL;
ERRProlog
	str::string Result;
ERRBegin
	Result.Init();
	
	Row = SubstituteShortTags( String, Callback, Result, TagMarker );

	String = Result;
ERRErr
ERREnd
ERREpilog
	return Row;
}


/// 'namespace {...}' parce que sinon VC++ 12 se mélange les pinceaux...
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
	sdr::row__ Row = E_NIL;
ERRProlog
	str::string Result;
ERRBegin
	Result.Init();
	
	Row = SubstituteShortTags( String, Values, Result, TagMarker );

	String = Result;
ERRErr
ERREnd
ERREpilog
	return Row;
}


static bso::bool__ GetTag_(
	flw::iflow__ &Flow,
	str::string_ &Tag,
	char Marker )
{
	if ( Flow.EndOfFlow() )
		ERRFwk();

	if ( Flow.Get() != Marker )
		ERRFwk();

	if ( Flow.EndOfFlow() )
		ERRFwk();

	if ( Flow.View() == Marker ) {
		Flow.Skip();
		return false;
	}

	do {
		Tag.Append( Flow.Get() );
	} while ( !Flow.EndOfFlow() && ( Flow.View() != Marker ) );

	if ( Flow.EndOfFlow() )
		ERRFwk();

	Flow.Skip();

	return true;
}

bso::bool__ tagsbs::SubstituteLongTags(
	flw::iflow__ &IFlow,
	long_tags_callback__ &Callback,
	flw::oflow__ &OFlow,
	char Marker )
{
	bso::bool__ Success = false;
ERRProlog
	str::string Tag, Value;
ERRBegin
	while ( !IFlow.EndOfFlow() ) {
		if ( IFlow.View() == Marker ) {
			Tag.Init();
			if ( GetTag_( IFlow, Tag, Marker ) ) {

				Value.Init();
				if ( !Callback.GetTagValue( Tag, Value ) )
					ERRReturn;

				Value.WriteToFlow( OFlow, false );
			} else
				OFlow.Put( Marker );
		} else
			OFlow.Put( IFlow.Get() );
	}

	Success = true;
ERRErr
ERREnd
ERREpilog
	return Success;
}

tol::E_XROW tagsbs::SubstituteLongTags(
	const str::string_ &String,
	long_tags_callback__ &Callback,
	str::string_ &Result,
	char TagMarker )
{
	sdr::row__ Row = E_NIL;
ERRProlog
	flx::E_STRING_IFLOW__ IFlow;
	flx::E_STRING_OFLOW___ OFlow;
ERRBegin
	IFlow.Init( String );
	OFlow.Init( Result );

	if ( !SubstituteLongTags( IFlow, Callback, OFlow, TagMarker ) )
		Row = IFlow.AmountRed();
ERRErr
ERREnd
ERREpilog
	return Row;
}

tol::E_XROW tagsbs::SubstituteLongTags(
	str::string_ &String,
	long_tags_callback__ &Callback,
	char TagMarker )
{
	sdr::row__ Row = E_NIL;
ERRProlog
	str::string Result;
ERRBegin
	Result.Init();
	
	Row = SubstituteLongTags( String, Callback, Result, TagMarker );

	String = Result;
ERRErr
ERREnd
ERREpilog
	return Row;
}

static void FillAutomat_(
	const str::strings_ &Tags,
	stsfsm::automat_ &Automat )
{
	ctn::E_CMITEM( str::string_ ) Tag;
	sdr::row__ Row = Tags.First();

	Tag.Init( Tags );

	while ( Row != E_NIL ) {
		stsfsm::Add( Tag( Row ), *Row, Automat );

		Row = Tags.Next( Row );
	}
}

/// 'namespace {...}' parce que sinon VC++ 12 se mélange les pinceaux...
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
			sdr::row__ Row = E_NIL;

			Parser.Init( Automat );

			if ( Parser.Handle( Tag ) != stsfsm::sMatch ) {
				BadTag = Tag;
				return false;
			}

			Row = Parser.GetId();

			if ( !Values->Exists( Row ) )
				ERRFwk();

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
ERRProlog
	longs_callback___ Callback;
ERRBegin
	if ( Tags.Amount() != Values.Amount() )
		ERRFwk();

	Callback.Init( Values );

	FillAutomat_( Tags, Callback.Automat );

	Success = SubstituteLongTags( IFlow, Callback, OFlow, TagMarker );
ERRErr
ERREnd
ERREpilog
	return Success;
}


tol::E_XROW tagsbs::SubstituteLongTags(
	const str::string_ &String,
	const str::strings_ &Tags,
	const str::strings_ &Values,
	str::string_ &Result,
	char TagMarker )
{
	sdr::row__ Row = E_NIL;
ERRProlog
	longs_callback___ Callback;
ERRBegin
	if ( Tags.Amount() != Values.Amount() )
		ERRFwk();

	Callback.Init( Values );

	FillAutomat_( Tags, Callback.Automat );

	Row = SubstituteLongTags( String, Callback, Result, TagMarker );
ERRErr
ERREnd
ERREpilog
	return Row;
}

tol::E_XROW tagsbs::SubstituteLongTags(
	str::string_ &String,
	const str::strings_ &Tags,
	const str::strings_ &Values,
	char TagMarker )
{
	sdr::row__ Row = E_NIL;
ERRProlog
	str::string Result;
ERRBegin
	Result.Init();
	
	Row = SubstituteLongTags( String, Tags, Values, Result, TagMarker );

	String = Result;
ERRErr
ERREnd
ERREpilog
	return Row;
}




/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class tagsbspersonnalization
{
public:
	tagsbspersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~tagsbspersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the ending of the application  */
	}
};


				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

static tagsbspersonnalization Tutor;
