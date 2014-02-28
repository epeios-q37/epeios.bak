/*
	'rgstry.cpp' by Claude SIMON (http://zeusw.org/).

	'rgstry' is part of the Epeios framework.

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

#define RGSTRY__COMPILATION

#include "rgstry.h"

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

#include "flx.h"
#include "flf.h"
#include "fnm.h"
#include "lcl.h"
#include "crptgr.h"

using namespace rgstry;

#define CASE( m )\
	case s##m:\
	return RGSTRY_NAME "_" #m;\
	break

const char *rgstry::GetLabel( status__ Status )
{
	switch ( Status ) {
	CASE( UnableToOpenFile );
	CASE( ParseError );
	CASE( UnableToFindRootPath );
	CASE( RootPathError );
	default:
		ERRPrm();
		break;
	}

	return NULL;	// Pour éviter un '<arbibg'.

}

void rgstry::GetMeaning(
	const context___ &Context,
	lcl::meaning_ &Meaning )
{
ERRProlog
	lcl::meaning MeaningBuffer;
	TOL_CBUFFER___ Buffer;
ERRBegin
	switch ( Context.Status ) {
	case sOK:
		ERRFwk();
		break;
	case sUnableToOpenFile:
		Meaning.SetValue( GetLabel( Context.Status ) );

		Meaning.AddTag( Context.Coordinates.FileName.UTF8( Buffer ) );
		break;
	case sParseError:
		Meaning.SetValue( GetLabel( Context.Status ) );

		MeaningBuffer.Init();
		xpp::GetMeaning( Context, MeaningBuffer );

		Meaning.AddTag( MeaningBuffer );
		break;
	case sRootPathError:
		if ( Context.PathErrorRow != E_NIL )
			ERRFwk();
	case sUnableToFindRootPath:
		Meaning.SetValue( GetLabel( Context.Status ) );
		break;
	default:
		ERRFwk();
		break;
	}
ERRErr
ERREnd
ERREpilog
}

row__ rgstry::registry_::_Search(
	const name_ &Name,
	const rows_ &NodeRows,
	sdr::row__ &Cursor ) const
{
	buffer Buffer;

	sdr::row__ &Row = Cursor;
	
	if ( Row == E_NIL )
		Row = NodeRows.First();
	else
		Row = NodeRows.Next( Row );

	Buffer.Init( Nodes );

	while ( ( Row != E_NIL ) && ( Buffer( NodeRows( Row ) ).Name != Name ) )
		Row = NodeRows.Next( Row );

	if ( Row != E_NIL )
		return NodeRows( Row );
	else
		return E_NIL;

}

row__ rgstry::registry_::_Search(
	nature__ Nature,
	const name_ &Name,
	const rows_ &NodeRows,
	cursor__ &Cursor ) const
{
	buffer Buffer;

	row__ Row = E_NIL;
	
	Buffer.Init( Nodes );

	Row = _Search( Name, NodeRows, Cursor );

	while ( ( Row != E_NIL ) && ( Buffer( Row ).Nature() != Nature ) )
		Row = _Search( Name, NodeRows, Cursor );

	return Row;
}

row__ rgstry::registry_::_Search(
	const name_ &KeyName,
	const name_ &AttributeName,
	const value_ &AttributeValue,
	row__ Row,
	cursor__ &Cursor ) const
{
	row__ ResultRow = E_NIL;

	if ( KeyName.Amount() != 0 ) {
		// .../KeyName...

		ResultRow = _SearchKey( KeyName, Row, Cursor );

		if ( ( ResultRow != E_NIL ) && ( AttributeName.Amount() != 0 ) ) {
			// .../KeyName[@AttributeName='AttributeValue']/...

			while ( ( ResultRow != E_NIL ) && ( !_AttributeExists( AttributeName, AttributeValue, ResultRow ) ) ) {
				ResultRow = _SearchKey( KeyName, Row, Cursor );
			}
		}
	} else {
		if ( AttributeName.Amount() != 0 ) {
			// .../@AttributeName...
#ifdef RGSTRY_DBG
			if ( AttributeValue.Amount() != 0 )
				ERRFwk();
#endif
				ResultRow = _SearchAttribute( AttributeName, Row, Cursor );
		}
	}

	return ResultRow;
}

const str::string_ &rgstry::registry_::GetCompleteName(
	row__ Row,
	str::string_ &Name,
	const char *Separator ) const
{
	buffer Buffer;

	Buffer.Init( Nodes );

	if ( Row != E_NIL ) {
		Name.Append( Buffer( Row ).Name );
		Row = Buffer( Row ).ParentRow();
	}

	while ( Row != E_NIL ) {
		Name.Insert( Separator, 0 );
		Name.Insert( Buffer( Row ).Name, 0 );

		Row = Buffer( Row ).ParentRow();
	}

	return Name;
}

enum state__ {
	sKeyName,
	sAttributeName,
	sAttributeValue,
	s_amount,
	s_Undefined
};

sdr::row__ rgstry::BuildPath(
	const str::string_ &PathString,
	path_ &Path )
{
	sdr::row__ Row = E_NIL;
ERRProlog
	bso::bool__ Continue = true;
	state__ State = ::s_Undefined;
	bso::char__ C;
	path_item Item;
	bso::bool__ KeyNameAsAttribute = false;
	bso::bool__ AttributeMarkerFound = false;
ERRBegin
	Row = PathString.First();

	Item.Init();

	Continue = Row != E_NIL;

	State = sKeyName;

	while ( Continue ) {
		C = PathString( Row );
		switch ( State ) {
		case sKeyName:
			switch ( C ) {
			case '=':
			case '"':
				Continue = false;
				break;
			case '[':
				if ( KeyNameAsAttribute )
					Continue = false;
				else if ( Item.KeyName.Amount() == 0 )
					Continue = false;
				State = sAttributeName;
				Item.AttributeName.Init();
				AttributeMarkerFound = false;
				break;
			case '/':
				if ( KeyNameAsAttribute )
					Continue = false;
				else if ( Item.KeyName.Amount() == 0 )
					Continue = false;
				else
					Path.Append( Item );
				Item.Init();
				break;
			case '@':
				if ( Item.KeyName.Amount() != 0 )
					Continue = false;
				else
					KeyNameAsAttribute = true;
				break;
			default:
				Item.KeyName.Append( C );
				break;
			}
			break;
			case sAttributeName:
				switch ( C ) {
				case ']':
					Continue = false;
					break;
				case '"':
					Continue = false;
					break;
				case '@':
					if ( Item.AttributeName.Amount() != 0 )
						Continue = false;
					else if ( AttributeMarkerFound )
						Continue = false;
					else
						AttributeMarkerFound = true;
					break;
				case '=':
					if ( Item.AttributeName.Amount() == 0 )
						Continue = false;
					State = sAttributeValue;
					Item.AttributeValue.Init();
					if ( ( Row = PathString.Next( Row ) ) == E_NIL )
						Continue = false;
					else if ( PathString( Row ) != '"' )
						Continue = false;
					break;
				default:
					Item.AttributeName.Append( C );
					break;
				}
				break;
			case sAttributeValue:
				switch ( C ) {
/*				case '[':
				case ']':
				case '/':
					Continue = false;
					break;
*/				case '"':
					if ( ( Row = PathString.Next( Row ) ) == E_NIL )
						Continue = false;
					else if ( PathString( Row ) != ']' )
						Continue = false;
					else if ( ( Row = PathString.Next( Row ) ) == E_NIL )
						Continue = false;
					else if ( PathString( Row ) != '/' )
						Continue = false;
					Path.Append( Item );
					Item.Init();
					State = sKeyName;
					break;
				default:
					Item.AttributeValue.Append( C );
					break;
				}
				break;
			default:
				ERRFwk();
				break;
		}

		if ( Continue )
			Row = PathString.Next( Row );

		if ( Row == E_NIL )
			Continue = false;
	}

	if ( Item.KeyName.Amount() != 0 ) {
		if ( KeyNameAsAttribute ) {
			Item.AttributeName = Item.KeyName;
			Item.KeyName.Init();
		}

		Path.Append( Item );
	}
ERRErr
ERREnd
ERREpilog
	return Row;
}

// Pour simplifier la mise en oeuvre de 'PathErrorRow' en tant que paramètre optionnel (alors == 'NULL').
static inline bso::bool__ BuildPath_(
	const str::string_ &PathString,
	path_ &Path,
	sdr::row__ *PathErrorRow )
{
	sdr::row__ PathErrorRowBuffer = BuildPath( PathString, Path );

	if ( PathErrorRowBuffer != E_NIL ) {
		if ( PathErrorRow == NULL )
			ERRDta();
		else {
			*PathErrorRow = PathErrorRowBuffer;
			return false;
		}
	}

	return true;

}

row__ rgstry::registry_::_Search(
	const path_ &Path,
	sdr::row__ PathRow,
	row__ Row,
	rows_ &ResultRows ) const
{
	cursor__ CandidateRow = E_NIL;
	cursor__ Cursor = E_NIL;
	ctn::E_CITEM( path_item_ ) Item;
	row__ ResultRow, ChildRow = E_NIL;
	bso::bool__ All = &ResultRows != NULL;
	buffer Buffer;

	Item.Init( Path );
	Buffer.Init( Nodes );

	ResultRow = ChildRow = _Search( Item( PathRow ), Row, Cursor );

	if ( PathRow != Path.Last() ) {

		while ( ChildRow != E_NIL )  {
			ResultRow = _Search( Path, Path.Next( PathRow ), ChildRow, ResultRows );

			if ( ResultRow != E_NIL ) {
				if ( All ) {
					ChildRow = _Search( Item( PathRow ), Row, Cursor );
				} else
					ChildRow = E_NIL;
			} else
				ChildRow = _Search( Item( PathRow ), Row, Cursor );
		}

	} else if ( All ) {

		while ( ChildRow != E_NIL ) {
			ResultRows.Append( ChildRow );

			ChildRow = _Search( Item( PathRow ), Row, Cursor );
		}

	}

	return ResultRow;
}

row__ rgstry::registry_::_Search(
	const path_ &Path,
	row__ Row ) const
{
	sdr::row__ PathRow = E_NIL, Cursor = E_NIL;

	if ( Path.Amount() != 0 )
		return _Search( Path, Path.First(), Row, *(rows_ *)NULL );
	else
		return E_NIL;
}

row__ rgstry::registry_::_Search(
	const str::string_ &PathString,
	row__ Row,
	sdr::row__ *PathErrorRow ) const
{
ERRProlog
	path Path;
ERRBegin
	Path.Init();

	if ( !BuildPath_( PathString, Path, PathErrorRow ) )
		Row = E_NIL;
	else
		Row = _Search( Path, Row );

ERRErr
ERREnd
ERREpilog
	return Row;
}

row__ rgstry::registry_::Create(
	const path_ &Path,
	row__ Row )
{
	bso::bool__ Exists = true;
	ctn::E_CITEM( path_item_ ) Item;
	sdr::row__ PathRow = Path.First();
	row__ CandidateRow = E_NIL;

	Item.Init( Path );

	while ( ( PathRow != E_NIL ) && Exists ) {
		CandidateRow = _Search( Item( PathRow ), Row );

		if ( CandidateRow == E_NIL ) {
			Exists = false;

			Row = _Create( Item( PathRow ), Row );
		} else
			Row = CandidateRow;

		PathRow = Path.Next( PathRow );
	}

	while ( PathRow != E_NIL ) {
		Row = _Create( Item( PathRow ), Row );

		PathRow = Path.Next( PathRow );
	}

	return Row;
}

row__ rgstry::registry_::Create(
	const str::string_ &PathString,
	row__ Row,
	sdr::row__ *PathErrorRow )
{
ERRProlog
	path Path;
ERRBegin
	Path.Init();

	if ( !BuildPath_( PathString, Path, PathErrorRow ) )
		Row = E_NIL;
	else
		Row = Create( Path, Row );

ERRErr
ERREnd
ERREpilog
	return Row;
}


row__ rgstry::registry_::SetValue(
	const str::string_ &PathString,
	const value_ &Value,
	row__ Row,
	sdr::row__ *PathErrorRow )
{
ERRProlog
	path Path;
ERRBegin
	Path.Init();

	if ( BuildPath_( PathString, Path, PathErrorRow ) )
		Row = SetValue( Path, Value, Row );
	else
		Row = E_NIL;

ERRErr
ERREnd
ERREpilog
	return Row;
}

#ifdef RGSTRY_PREPROCESSOR_NAMESPACE
#	define DEFAULT_NAMESPACE RGSTRY_PREPROCESSOR_NAMESPACE
#else
#	define DEFAULT_NAMESPACE XPP__PREPROCESSOR_DEFAULT_NAMESPACE
#endif

namespace {
	class callback___
	: public xml::callback__
	{
	private:
		registry_ &_Registry;
		row__ _Target, _Current;
	protected:
		virtual bso::bool__ XMLProcessingInstruction( const xml::dump_ & )
		{
			return true;
		}
		virtual bso::bool__ XMLStartTag(
			const str::string_ &TagName,
			const xml::dump_ &Dump )
		{

			if ( _Current == E_NIL )
				_Current = _Registry.CreateRegistry( str::string( "_root" ) );
		
			_Current = _Registry.AddKey( TagName, _Current );

			if ( _Target == E_NIL )
				_Target = _Current;

			return true;
		}
		virtual bso::bool__ XMLAttribute(
			const str::string_ &TagName,
			const str::string_ &Name,
			const str::string_ &Value,
			const xml::dump_ &Dump )
		{
			_Registry.AddAttribute( Name, Value, _Current );

			return true;
		}
		virtual bso::bool__ XMLStartTagClosed(
			const str::string_ &TagName,
			const xml::dump_ &Dump )
		{
			return true;
		}
		virtual bso::bool__ XMLValue(
			const str::string_ &TagName,
			const str::string_ &Value,
			const xml::dump_ &Dump )
		{
			_Registry.SetValue( Value, _Current, true );

			return true;
		}
		virtual bso::bool__ XMLEndTag(
			const str::string_ &TagName,
			const xml::dump_ &Dump )
		{

			_Current = _Registry.GetParentRow( _Current );

			if ( _Current == E_NIL )
				return false;

			return true;
		}
		virtual bso::bool__ XMLComment(
			const str::string_ &Value,
			const xml::dump_ &Dump )
		{
			return true;
		}
	public:
		callback___( registry_ &Registry )
		: _Registry( Registry )
		{
			_Target = _Current = E_NIL;
		}
		void Init( row__ Root )
		{
			_Target = E_NIL;
			_Current = Root;
		}
		row__ GetTarget( void ) const
		{
			return _Target;
		}
		row__ GetRoot( void ) const
		{
			return _Current;
		}
	};
}

	void rgstry::registry_::_Delete( row__ Row )
	{
		const node_ &Node = Nodes( Row );

		_Delete( Node.Children );

		Nodes.Delete( Row );
	}

	bso::bool__ rgstry::registry_::Exists(
		const str::string_ &PathString,
		row__ ParentRow,
		sdr::row__ *PathErrorRow ) const
	{
		bso::bool__ Result = false;
	ERRProlog
		path Path;
	ERRBegin
		Path.Init();

		if ( BuildPath_( PathString, Path, PathErrorRow ) )
			Result = Exists( Path, ParentRow );
	ERRErr
	ERREnd
	ERREpilog
		return Result;
	}

void rgstry::registry_::_DumpAttributes(
	row__ Row,
	xml::writer_ &Writer ) const
{
ERRProlog
	buffer Buffer;
	rows Rows;
	cursor__ Cursor = E_NIL;
ERRBegin
	Buffer.Init( Nodes );

	Rows.Init();

	Rows = Buffer( Row ).Children;

	Cursor = Rows.First();

	while ( Cursor != E_NIL ){
		if ( Buffer( Rows( Cursor ) ).Nature() == nAttribute )
			Writer.PutAttribute( Buffer().Name, Buffer().Value );

		Cursor = Rows.Next( Cursor );
	}
ERRErr
ERREnd
ERREpilog
}

sdr::size__ rgstry::registry_::_Dump(
	row__ Root,
	bso::bool__ RootToo,
	xml::writer_ &Writer,
	buffer &Buffer ) const
{
	sdr::size__ ChildAmount = 0;
ERRProlog
	rows Children;
	sdr::row__ Row = E_NIL;
ERRBegin
	Children.Init();
	Children = Buffer( Root ).Children;

	if ( RootToo )
		_DumpNode( Root, Writer, Buffer );
	
	Row = Children.First();

	while ( Row != E_NIL ) {
		if ( Buffer( Children( Row ) ).GetNature() != nAttribute )
			_Dump( Children( Row ), true, Writer, Buffer );

		Row = Children.Next( Row );
	}

	if ( RootToo )
		Writer.PopTag();

	ChildAmount = Children.Amount();
ERRErr
ERREnd
ERREpilog
	return ChildAmount;
}


sdr::size__ rgstry::registry_::Dump(
	row__ Root,
	bso::bool__ RootToo,
	xml::writer_ &Writer ) const
{
	sdr::size__ ChildAmount = 0;
ERRProlog
	buffer Buffer;
ERRBegin
	Buffer.Init( Nodes );

	ChildAmount = _Dump( Root, RootToo, Writer, Buffer );
ERRErr
ERREnd
ERREpilog
	return ChildAmount;
}

sdr::size__ rgstry::registry_::Dump(
	row__ Root,
	bso::bool__ RootToo,
	xml::outfit__ Outfit,
	xml::encoding__ Encoding,
	txf::text_oflow__ &Flow ) const
{
	sdr::size__ ChildAmount = 0;
ERRProlog
	xml::writer Writer;
	buffer Buffer;
ERRBegin
	Buffer.Init( Nodes );

	Writer.Init( Flow, Outfit, Encoding );

	ChildAmount = Dump( Root, RootToo, Writer );
ERRErr
ERREnd
ERREpilog
	return ChildAmount;
}


void rgstry::registry_::_Delete( const rows_ &Rows )
{
	sdr::row__ Row = Rows.First();

	while ( Row != E_NIL ) {
		_Delete( Rows( Row ) );

		Row = Rows.Next( Row );
	}
}

const value_ &rgstry::registry_::GetValue(
	const path_ &Path,
	row__ Row,
	bso::bool__ *Missing,
	buffer &Buffer ) const	// Nota : ne met 'Missing' à 'true' que lorque 'Path' n'existe pas. Si 'Missing' est à 'true', aucune action n'est réalisée.
{
	static value Empty;
	const value_ *Result = &Empty;
	row__ ResultRow = E_NIL;

	if ( *Missing )
		return *Result;

	Empty.Init();
	Buffer.Init( Nodes );

	if ( ( ResultRow = _Search( Path, Row ) ) != E_NIL )
		Result = &Buffer( ResultRow ).Value;
	else
		*Missing = true;

	return *Result;
}

const value_ &rgstry::registry_::GetValue(
	const str::string_ &PathString,
	row__ Row,
	bso::bool__ *Missing,
	buffer &Buffer,
	sdr::row__ *PathErrorRow ) const	// Nota : ne met 'Missing' à 'true' que lorque 'Path' n'existe pas. Si 'Missing' est à 'true', aucune action n'est réalisée.
{
	static value Empty;
	const value_ *Result = &Empty;
ERRProlog
	path Path;
ERRBegin
	Path.Init();
	Empty.Init();

	if ( *Missing )
		ERRReturn;

	if ( BuildPath_( PathString, Path, PathErrorRow ) )
		Result = &GetValue( Path, Row, Missing, Buffer );
	else
		*Missing = true;
ERRErr
ERREnd
ERREpilog
	return *Result;
}

const value_ &rgstry::registry_::GetValue(
	const tentry__ &Entry,
	row__ Row,
	bso::bool__ *Missing,
	buffer &Buffer,
	sdr::row__ *PathErrorRow ) const
{
	const value_ *Value = NULL;
ERRProlog
	str::string Path;
ERRBegin
	Path.Init();

	Value = &GetValue( Entry.GetPath( Path ), Row, Missing, Buffer, PathErrorRow );
ERRErr
ERREnd
ERREpilog
	return *Value;
}


#if 0
const value_ &rgstry::registry_::GetValue(
	const entry___ &Entry,
	const tags_ &Tags,
	row__ Row,
	bso::bool__ *Missing,
	buffer &Buffer,
	sdr::row__ *PathErrorRow ) const
{
	const value_ *Value = NULL;
ERRProlog
	str::string Path;
ERRBegin
	Path.Init();

	Entry.GetPath( Tags, Path );

	Value = &GetValue( Path, Row, Missing, Buffer, PathErrorRow );
ERRErr
ERREnd
ERREpilog
	return *Value;
}
#endif

bso::bool__ rgstry::registry_::GetValues(
	const path_ &Path,
	row__ Row,
	values_ &Values ) const
{
	bso::bool__ Exists = false;
ERRProlog
	cursor__ Cursor = E_NIL;
	buffer Buffer;
	rows ResultRows;
ERRBegin
	Buffer.Init( Nodes );
	ResultRows.Init();

	_Search( Path, Path.First(), Row, ResultRows );

	if ( ResultRows.Amount() != 0 ) {
		Exists = true;
		Cursor = ResultRows.First();

		while ( Cursor != E_NIL ) {
			Values.Append( Buffer( ResultRows( Cursor ) ).Value );

			Cursor = ResultRows.Next( Cursor );
		}
	}
ERRErr
ERREnd
ERREpilog
	return Exists;
}

bso::bool__ rgstry::registry_::GetValues(
	const str::string_ &PathString,
	row__ ParentRow,
	values_ &Values,
	sdr::row__ *PathErrorRow ) const
{
	bso::bool__ Exists = false;
ERRProlog
	path Path;
ERRBegin
	Path.Init();

	if ( BuildPath_( PathString, Path, PathErrorRow ) )
		Exists = GetValues( Path, ParentRow, Values );
ERRErr
ERREnd
ERREpilog
	return Exists;
}

bso::bool__ rgstry::registry_::GetValues(
	const tentry__ &Entry,
	row__ Row,
	values_ &Values,
	sdr::row__ *PathErrorRow ) const
{
	bso::bool__ Result = false;
ERRProlog
	str::string Path;
ERRBegin
	Path.Init();

	Result = GetValues( Entry.GetPath( Path ), Row, Values, PathErrorRow );
ERRErr
ERREnd
ERREpilog
	return Result;
}

#if 0
bso::bool__ rgstry::registry_::GetValues(
	const entry___ &Entry,
	const tags_ &Tags,
	row__ Row,
	values_ &Values,
	sdr::row__ *PathErrorRow ) const
{
	bso::bool__ Result = false;
ERRProlog
	str::string Path;
ERRBegin
	Path.Init();

	Entry.GetPath( Tags, Path );

	Result = GetValues( Path, Row, Values, PathErrorRow );
ERRErr
ERREnd
ERREpilog
	return Result;
}
#endif

bso::bool__ rgstry::registry_::Delete(
	const str::string_ &PathString,
	row__ Row,
	sdr::row__ *PathErrorRow )
{
	bso::bool__ Exists = false;
ERRProlog
	path Path;
ERRBegin
	Path.Init();

	if ( BuildPath_( PathString, Path, PathErrorRow ) )
		Exists = Delete( Path, Row );
ERRErr
ERREnd
ERREpilog
	return Exists;
}

row__ rgstry::Parse(
	xtf::extended_text_iflow__ &XFlow,
	const xpp::criterions___ &Criterions,
	registry_ &Registry,
	row__ &Root,
	xpp::context___ &Context )
{
	row__ Target = E_NIL;
ERRProlog
	callback___ Callback( Registry );
	xpp::preprocessing_iflow___ PFlow;
	xtf::extended_text_iflow__ PXFlow;
	xml::status__ Status = xml::s_Undefined;
ERRBegin
	Callback.Init( Root );

	PFlow.Init( XFlow, xpp::criterions___( Criterions.Directory, Criterions.CypherKey, Criterions.IsNamespaceDefined() ? Criterions.Namespace : str::string( DEFAULT_NAMESPACE ) ) );
	PXFlow.Init( PFlow, XFlow.Format() );

	switch ( Status = xml::Parse( PXFlow, xml::ehReplace, Callback ) ) {
	case xml::sOK:
		Target = Callback.GetTarget();
		Root = Callback.GetRoot();
		break;
	case xml::sUnexpectedEOF:
//		Root = E_NIL;	// 'Root' peut avoir été in,itilisée par l'utilisateur.
		PFlow.GetContext( Context );
		break;
	default:
		if ( xml::IsEncodingRelatedError( Status ) )
			PFlow.GetContext( Context );
		else
			// Puisque l'on passe par le préprocesseur, si une erreur est rencontrée, xml::Parse(...)' ne peut normalement retourner que 'xml::sUndexpectedEOF'.
			ERRFwk();
		break;
	}

	if ( PXFlow.Format() != utf::f_Guess )
		XFlow.SetFormat( PXFlow.Format() );
ERRErr
ERREnd
ERREpilog
	return Target;
}

#if 1
const value_ &rgstry::overloaded_registry___::GetValue(
	const str::string_ &PathString,
	bso::bool__ *Missing,
	buffer &Buffer,
	sdr::row__ *PathErrorRow )const // Nota : ne met 'Missing' à 'true' que lorque 'Path' n'existe pas. Si 'Missing' est à 'true', aucune action n'est réalisée.
{
	static value Empty;
	const value_ *Result = &Empty;
ERRProlog
	row__ Row = E_NIL;
	path Path;
ERRBegin
	if ( *Missing )
		ERRReturn;

	Path.Init();
	Empty.Init();

	if ( !BuildPath_( PathString, Path, PathErrorRow ) ) {
		*Missing = true;
		ERRReturn;
	}

	if ( Local.Registry != NULL )
		Result = &Local.Registry->GetValue( Path, Local.Root, Missing, Buffer );
	else
		*Missing = true;

	if ( *Missing ) {
		*Missing = false;

		if ( Global.Registry != NULL )
			Result = &Global.Registry->GetValue( Path, Global.Root, Missing, Buffer );
	}
ERRErr
ERREnd
ERREpilog
	return *Result;
}

bso::bool__ rgstry::overloaded_registry___::GetValues(
	const str::string_ &PathString,
	values_ &Values,
	sdr::row__ *PathErrorRow ) const
{
	bso::bool__ Exists = false;
ERRProlog
	path Path;
ERRBegin
	Path.Init();

	if ( !BuildPath_( PathString, Path, PathErrorRow ) )
		ERRReturn;

	if ( Local.Registry != NULL )
		Exists = Local.Registry->GetValues( Path, Local.Root, Values );

	Exists = Global.Registry->GetValues( Path, Global.Root, Values ) || Exists;
ERRErr
ERREnd
ERREpilog
	return Exists;
}

bso::bool__ rgstry::overloaded_registry___::Exists(
	const str::string_ &PathString,
	sdr::row__ *PathErrorRow ) const
{
	bso::bool__ Exists = false;
ERRProlog
	path Path;
ERRBegin
	Path.Init();

	if ( BuildPath_( PathString, Path, PathErrorRow ) )
		if ( Local.Registry != NULL )
			Exists = Local.Registry->Exists( Path, Local.Root );
	
	Exists = Exists || Global.Registry->Exists( Path, Global.Root );
ERRErr
ERREnd
ERREpilog
	return Exists;
}

void rgstry::overloaded_registry___::Search(
	const str::string_ &RawPath,
	row__ &GlobalRow,
	row__ &LocalRow,
	sdr::row__ *PathErrorRow ) const
{
ERRProlog
	path Path;
ERRBegin
	Path.Init();

	if ( BuildPath_( RawPath, Path, PathErrorRow ) )
		Search( Path, GlobalRow, LocalRow );
ERRErr
ERREnd
ERREpilog
}
#endif

void rgstry::multi_level_registry_::Erase( level__ Level )
{
ERRProlog
	name Name;
	_entry__ Entry;
	buffer Buffer;
ERRBegin
	Entry.Init();
	Entries.Recall( Level, Entry );

	if ( Entry.Registry != NULL )
		ERRFwk();

	Name.Init( EmbeddedRegistry.GetValue( Entry.Root, Buffer ) );

	EmbeddedRegistry.Delete( Entry.Root );

	Entry.Root = EmbeddedRegistry.CreateRegistry( Name );

	Entries.Store( Entry, Level );
ERRErr
ERREnd
ERREpilog
}


const value_ &rgstry::multi_level_registry_::GetValue(
	const str::string_ &PathString,
	bso::bool__ *Missing,
	buffer &Buffer,
	sdr::row__ *PathErrorRow ) const	// Nota : ne met 'Missing' à 'true' que lorque 'Path' n'existe pas. Si 'Missing' est à 'true', aucune action n'est réalisée.
{
	static value Empty;
	const value_ *Result = &Empty;
ERRProlog
	level__ Level = E_NIL;
	path Path;
ERRBegin
	if ( *Missing )
		ERRReturn;

	Path.Init();

	if ( !BuildPath_( PathString, Path, PathErrorRow ) )
		ERRReturn;

	Level = Entries.Last();

	while ( Level != E_NIL ) {
		*Missing = false;

		Result = &GetValue( Level, Path, Missing, Buffer );

		if ( *Missing  )
			Level = Entries.Previous( Level );
		else
			Level = E_NIL;
	}
ERRErr
ERREnd
ERREpilog
	return *Result;
}

bso::bool__ rgstry::multi_level_registry_::GetValue(
	const str::string_ &PathString,
	value_ &Value,
	sdr::row__ *PathErrorRow ) const	// Nota : ne met 'Missing' à 'true' que lorque 'Path' n'existe pas. Si 'Missing' est à 'true', aucune action n'est réalisée.
{
	bso::bool__ Found = false;
ERRProlog
	level__ Level = E_NIL;
	path Path;
ERRBegin
	Path.Init();

	if ( !BuildPath_( PathString, Path, PathErrorRow ) )
		ERRReturn;

	Level = Entries.Last();

	while ( ( Level != E_NIL ) && ( !Found ) ) {
		Found = GetValue( Level, Path, Value );

		Level = Entries.Previous( Level );
	}
ERRErr
ERREnd
ERREpilog
	return Found;
}

bso::bool__ rgstry::multi_level_registry_::GetValue(
	const tentry__ &Entry,
	str::string_ &Value,
	sdr::row__ *PathErrorRow ) const
{
	bso::bool__ Found = false;
ERRProlog
	str::string Path;
ERRBegin
	Path.Init();

	Found = GetValue( Entry.GetPath( Path ), Value, PathErrorRow );
ERRErr
ERREnd
ERREpilog
	return Found;
}

bso::bool__ rgstry::multi_level_registry_::GetValue(
	level__ Level,
	const tentry__ &Entry,
	str::string_ &Value,
	sdr::row__ *PathErrorRow ) const
{
	bso::bool__ Found = false;
ERRProlog
	str::string Path;
ERRBegin
	Path.Init();

	Found = GetValue( Level, Entry.GetPath( Path ), Value, PathErrorRow );
ERRErr
ERREnd
ERREpilog
	return Found;
}

#if 0
bso::bool__ rgstry::multi_level_registry_::GetValue(
	const entry___ &Entry,
	const tags_ &Tags,
	str::string_ &Value,
	sdr::row__ *PathErrorRow ) const
{
	bso::bool__ Found = false;
ERRProlog
	str::string Path;
ERRBegin
	Path.Init();

	Entry.GetPath( Tags, Path );

	Found = GetValue( Path, Value, PathErrorRow );
ERRErr
ERREnd
ERREpilog
	return Found;
}

bso::bool__ rgstry::multi_level_registry_::GetValue(
	const entry___ &Entry,
	str::string_ &Value,
	sdr::row__ *PathErrorRow ) const
{
	bso::bool__ Found = false;
ERRProlog
	tags Tags;
ERRBegin
	Tags.Init();

	Found = GetValue( Entry, Tags, Value, PathErrorRow );
ERRErr
ERREnd
ERREpilog
	return Found;
}
#endif

bso::bool__ rgstry::multi_level_registry_::GetValues(
	const str::string_ &PathString,
	values_ &Values,
	sdr::row__ *PathErrorRow ) const
{
	bso::bool__ Found = false;
ERRProlog
	level__ Level = E_NIL;
	path Path;
ERRBegin
	Path.Init();

	if ( !BuildPath_( PathString, Path, PathErrorRow ) )
		ERRReturn;

	Level = Entries.Last();

	while ( Level != E_NIL ) {
		Found |= GetValues( Level, Path, Values );

		Level = Entries.Previous( Level );
	}
ERRErr
ERREnd
ERREpilog
	return Found;
}

bso::bool__ rgstry::multi_level_registry_::GetValues(
	const tentry__ &Entry,
	values_ &Values,
	sdr::row__ *PathErrorRow ) const
{
	bso::bool__ Found = false;
ERRProlog
	str::string Path;
ERRBegin
	Path.Init();
	Entry.GetPath( Path );

	Found = GetValues( Path, Values, PathErrorRow );
ERRErr
ERREnd
ERREpilog
	return Found;
}



bso::bool__ rgstry::multi_level_registry_::SetValue(
	const str::string_ &PathString,
	const value_ &Value,
	sdr::row__ *PathErrorRow )
{
	bso::bool__ Set = false;
ERRProlog
	value CurrentValue;
	sdr::row__ LocalPathErrorRow = E_NIL;
ERRBegin
	CurrentValue.Init();

	if ( !GetValue( PathString, CurrentValue, &LocalPathErrorRow ) ) {
		if ( LocalPathErrorRow != E_NIL ) {
			*PathErrorRow = LocalPathErrorRow;
			ERRReturn;
		}
	} else 
		if ( CurrentValue == Value )
			ERRReturn;

	SetValue( Entries.Last(), PathString, Value, PathErrorRow );

	Set = true;
ERRErr
ERREnd
ERREpilog
	return Set;
}

bso::bool__ rgstry::multi_level_registry_::SetValue(
	const tentry__ &Entry,
	const value_ &Value,
	sdr::row__ *PathErrorRow )
{
	bso::bool__ Set = false;
ERRProlog
	str::string Path;
ERRBegin
	Path.Init();

	Set = SetValue( Entry.GetPath( Path ), Value, PathErrorRow );
ERRErr
ERREnd
ERREpilog
	return Set;
}

#if 0
bso::bool__ rgstry::multi_level_registry_::SetValue(
	const entry___ &Entry,
	const tags_ &Tags,
	const value_ &Value,
	sdr::row__ *PathErrorRow )
{
	bso::bool__ Set = false;
ERRProlog
	str::string Path;
ERRBegin
	Path.Init();

	Entry.GetPath( Tags, Path );

	Set = SetValue( Path, Value, PathErrorRow );
ERRErr
ERREnd
ERREpilog
	return Set;
}

bso::bool__ rgstry::multi_level_registry_::SetValue(
	const entry___ &Entry,
	const value_ &Value,
	sdr::row__ *PathErrorRow )
{
	bso::bool__ Set = false;
ERRProlog
	str::string Path;
ERRBegin
	Path.Init();

	Entry.GetPath( Path );

	Set = SetValue( Path, Value, PathErrorRow );
ERRErr
ERREnd
ERREpilog
	return Set;
}
#endif



bso::bool__ rgstry::multi_level_registry_::Delete(
	const str::string_ &PathString,
	sdr::row__ *PathErrorRow )
{
	bso::bool__ Deleted = false;
ERRProlog
	level__ Level = E_NIL;
	path Path;
ERRBegin
	Path.Init();

	if ( !BuildPath_( PathString, Path, PathErrorRow ) )
		ERRReturn;

	Level = Entries.Last();

	while ( Level != E_NIL ) {
		Deleted |= Delete( Path, Level );

		Level = Entries.Previous( Level );
	}
ERRErr
ERREnd
ERREpilog
	return Deleted;
}


bso::bool__ rgstry::multi_level_registry_::MoveTo(
	const str::string_ &Path,
	level__ Level )
{
	bso::bool__ Moved = false;
ERRProlog
	str::string Value;
	str::string LevelValue;
ERRBegin
	Value.Init();

	if ( GetValue( Path, Value ) ) {
		LevelValue.Init();

		if ( GetValue( Level, Path, LevelValue ) || ( Value != LevelValue ) ) {
			Delete( Path );
			SetValue( Level, Path, Value );

			Moved = true;
		}
	}
ERRErr
ERREnd
ERREpilog
	return Moved;
}

row__ rgstry::multi_level_registry_::Search(
	const str::string_ &PathString,
	sdr::row__ *PathErrorRow ) const
{
	row__ Row = E_NIL;
ERRProlog
	level__ Level = E_NIL;
	path Path;
ERRBegin
	Path.Init();

	if ( !BuildPath_( PathString, Path, PathErrorRow ) )
		ERRReturn;

	Level = Entries.Last();

	while ( ( Level != E_NIL ) && ( Row == E_NIL ) ) {
		Row = Search( Level, Path );

		Level = Entries.Previous( Level );
	}
ERRErr
ERREnd
ERREpilog
	return Row;
}

row__ rgstry::multi_level_registry_::Search(
	const tentry__ &Entry,
	sdr::row__ *PathErrorRow ) const
{
	row__ Row = E_NIL;
ERRProlog
	str::string Path;
ERRBegin
	Path.Init();
	Entry.GetPath( Path );

	Row = Search( Path, PathErrorRow );
ERRErr
ERREnd
ERREpilog
	return Row;
}


status__ rgstry::FillRegistry(
	xtf::extended_text_iflow__ &XFlow,
	const xpp::criterions___ &Criterions,
	const char *RootPath,
	rgstry::registry_ &Registry,
	rgstry::row__ &RegistryRoot,
	context___ &Context )
{
	sdr::row__ PathErrorRow = E_NIL;
	rgstry::row__ NewRoot = E_NIL;

	if ( rgstry::Parse( XFlow, Criterions, Registry, RegistryRoot, Context ) == E_NIL )
		return Context.Status = sParseError;

	if ( ( RootPath != NULL ) && ( *RootPath ) ) {
		if ( ( NewRoot = Registry.Search( str::string( RootPath ), RegistryRoot, &PathErrorRow ) ) == E_NIL ) {
			if ( PathErrorRow != E_NIL ) {
				Context.PathErrorRow = PathErrorRow;
				return Context.Status = sRootPathError;
			} else
				return Context.Status = sUnableToFindRootPath;
		} else if ( Registry.GetNature( RegistryRoot ) == nAttribute )
				return Context.Status = sRootPathError;
			else
				RegistryRoot = NewRoot;
	}


	return sOK;
}

status__ rgstry::FillRegistry(
	const fnm::name___ &FileName,
	const xpp::criterions___ &Criterions,
	const char *RootPath,
	rgstry::registry_ &Registry,
	rgstry::row__ &RegistryRoot,
	context___ &Context )
{
	status__ Status = s_Undefined;
ERRProlog
	flf::file_iflow___ FFlow;
	xtf::extended_text_iflow__ XFlow;
	fnm::name___ Location;
	str::string Buffer;
ERRBegin
	if ( FFlow.Init( FileName, err::hUserDefined ) != tol::rSuccess ) {
		Status = Context.Status = sUnableToOpenFile;
		Context.Coordinates.FileName = FileName;
		ERRReturn;
	}

	XFlow.Init( FFlow, utf::f_Default );

	if ( Criterions.Directory.Amount() != 0 )
		ERRPrm();

	Location.Init();
	Buffer.Init();
	Status = FillRegistry( XFlow, xpp::criterions___( fnm::GetLocation( FileName, Location ).UTF8( Buffer ), Criterions.CypherKey, Criterions.Namespace ), RootPath, Registry, RegistryRoot, Context );

	if ( Status == sParseError )
		if ( Context.Coordinates.FileName.Size() == 0 )
			Context.Coordinates.FileName = FileName;
ERRErr
ERREnd
ERREpilog
	return Status;
}

#define LIMIT 9

static bso::u8__ GetTagAmount_( const str::string_ &String )
{
	bso::u8__ Amount = 0;

	sdr::row__ Row = String.First();

	while ( ( Row != E_NIL ) && ( ( Row = String.Search( RGSTRY__TAG_MARKER_C, Row ) ) != E_NIL ) )
	{
		Row = String.Next( Row );

		if ( Row == E_NIL )
			ERRFwk();

		if ( String( Row ) != RGSTRY__TAG_MARKER_C ) {
			if ( Amount == LIMIT )
				ERRLmt();
			else
				Amount++;
		}

		Row = String.Next( Row );
	}

	return Amount;
}

static void HandleTag_(
	bso::u8__ Indice,
	const char *Source,
	str::string_ &Target )
{
	while ( *Source ) {
		if ( *Source == RGSTRY__TAG_MARKER_C ) {
			Target.Append( RGSTRY__TAG_MARKER_C );

			Source++;

			if ( *Source != RGSTRY__TAG_MARKER_C ) {
				if ( Indice > LIMIT )
					ERRLmt();

				Target.Append( '0' + Indice++ );
			} else if ( *Source == 0 )
				ERRDta();

		} 

		Target.Append( *Source++ );
	}
}

void rgstry::entry___::_PostInitialize( void ) const
{
	_Path.Init();

	_GetParentPath( ( _RawPath != NULL ) && ( *_RawPath == '[' ), _Path );

	HandleTag_( GetTagAmount_( _Path ) + 1, _RawPath, _Path );
}

const str::string_ &rgstry::entry___::_GetPath(
	const tags_ &Tags,
	str::string_ &Path ) const
{
ERRProlog
	str::string Buffer;
ERRBegin
	Buffer.Init( _Path );

	str::ReplaceShortTags( Buffer, Tags, RGSTRY__TAG_MARKER_C );

	Path.Append( Buffer );
ERRErr
ERREnd
ERREpilog
	return Path;
}

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class rgstrypersonnalization
{
public:
	rgstrypersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~rgstrypersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the ending of the application  */
	}
};


				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

static rgstrypersonnalization Tutor;
