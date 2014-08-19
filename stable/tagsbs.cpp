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

using namespace tagsbs;

void tagsbs::SubstituteShortTag(
	str::string_ &String,
	indice__ Indice,
	const str::string_ &Value,
	const char TagMarker )
{
ERRProlog
	str::strings Values;
	indice__ Counter = 1;
	bso::integer_buffer__ Buffer;
	str::string Tag;
ERRBegin
	if ( ( Indice == 0 ) || ( Indice > 9 ) )
		ERRPrm();

	Values.Init();

	while ( Counter != Indice ) {
		Tag.Init();
		Tag.Append( TagMarker );
		Tag.Append( bso::Convert( Counter, Buffer ) );

		Values.Append( Tag );

		Counter++;
	}

	Values.Append( Value );

	SubstituteShortTags( String, Values, TagMarker );
ERRErr
ERREnd
ERREpilog
}

void tagsbs::SubstituteLongTag(
	str::string_ &String,
	const str::string_ &Tag,
	const str::string_ &Value,
	char TagMarker )
{
ERRProlog
	str::strings Tags, Values;
ERRBegin
	Tags.Init();
	Tags.Append( Tag ) ;

	Values.Init();
	Values.Append( Value );

	SubstituteLongTags( String, Tags, Values, TagMarker );
ERRErr
ERREnd
ERREpilog
}


E_CDEF( indice__, Limit, 9 );

// Remplace le tag d'indice '0' par la liste des valeurs.
static void Replace0Tag_(
	str::string_ &Message,
	short_tags_callback__ &Callback,
	const char TagMarker )
{
ERRProlog
	str::string MergedValues;
	str::string Value;
	str::string Tag;
	indice__ Indice = 0;
	sdr::row__ Row = E_NIL;
ERRBegin

	MergedValues.Init( " (" );

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

	Tag.Init();
	Tag.Append( TagMarker );
	Tag.Append( '0' );
	
	Row = Message.Search( Tag );

	while ( Row != E_NIL ) {
		Message.Remove( Row, Tag.Amount() );

		Message.Insert( MergedValues, Row );

		Row = Message.Next( Row, MergedValues.Amount() );

		if ( Row != E_NIL )
			Row = Message.Search( Tag, Row );
	}

ERRErr
ERREnd
ERREpilog
}

indice__ tagsbs::SubstituteShortTags(
	str::string_ &String,
	short_tags_callback__ &Callback,
	char TagMarker )
{
	indice__ IndiceError = 0;
ERRProlog
	indice__ Indice = 1;
	sdr::row__ SearchRow = E_NIL;
	str::string Tag, Value;
	bso::integer_buffer__ Buffer;
	bso::bool__ ValueExists = false;
ERRBegin
	Replace0Tag_( String, Callback, TagMarker );

	while ( Indice <= Limit ) {
		Tag.Init();
		Tag.Append( TagMarker );
		Tag.Append( bso::Convert( Indice, Buffer ) );

		Value.Init();

		ValueExists = Callback.GetTagValue( Indice, Value );

		SearchRow = String.Search( Tag );

		while ( SearchRow != E_NIL ) {
			if ( !ValueExists ) {
				IndiceError = Indice;
				ERRReturn;
			}

			String.Remove( SearchRow, Tag.Amount() );

			String.Insert( Value, SearchRow );

			SearchRow = String.Search( Tag, SearchRow );
		}

		Indice++;
	}

	Tag.Init();
	Tag.Append( TagMarker );
	Tag.Append( TagMarker );

	SearchRow = String.Search( Tag );

	while ( SearchRow != E_NIL ) {
		String.Remove( SearchRow, Tag.Amount() );

		String.Insert( TagMarker, SearchRow );

		SearchRow = String.Search( Tag, SearchRow );
	}

ERRErr
ERREnd
ERREpilog
	return IndiceError;
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

indice__ tagsbs::SubstituteShortTags(
	str::string_ &String,
	const str::strings_ &Values,
	const char TagMarker )
{
	shorts_callback___ Callback;

	Callback.Init( Values );

	return SubstituteShortTags( String, Callback, TagMarker );
}

static bso::bool__ GetTag_(
	const str::string_ &Target,
	sdr::row__ Row,
	str::string_ &Tag,
	char Marker )
{
	if ( Row == E_NIL )
		ERRFwk();

	if ( Target( Row ) != Marker )
		ERRFwk();

	Row = Target.Next( Row );

	if ( Row == E_NIL )
		ERRFwk();

	if ( Target( Row ) == Marker ) {
		Row = Target.Next( Row );
		return false;
	}

	do {
		Tag.Append( Target( Row ) );

		Row = Target.Next( Row );
	} while ( ( Row != E_NIL ) && ( Target( Row ) != Marker ) );

	if ( Row == E_NIL )
		ERRFwk();

	return true;
}

tol::E_XROW tagsbs::SubstituteLongTags(
	str::string_ &String,
	long_tags_callback__ &Callback,
	char Marker )
{
	sdr::row__ Row = E_NIL;
ERRProlog
	str::string Tag, Value;
ERRBegin
	Row = String.First();

	while ( Row != E_NIL ) {
		if ( String( Row ) == '%' ) {
			Tag.Init();
			if ( GetTag_( String, Row, Tag, Marker ) ) {
				String.Remove( Row, Tag.Amount() + 2 );
				Value.Init();
				if ( !Callback.GetTagValue( Tag, Value ) )
					ERRReturn;

				String.Insert( Value, Row );

				Row = String.Next( Row, Value.Amount() );
			} else
				String.Remove( Row );
		} else
			Row = String.Next( Row );
	}
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

tol::E_XROW tagsbs::SubstituteLongTags(
	str::string_ &String,
	const str::strings_ &Tags,
	const str::strings_ &Values,
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

	Row = SubstituteLongTags( String, Callback, TagMarker );
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
