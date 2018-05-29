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

#define RGSTRY__COMPILATION

#include "rgstry.h"

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
		qRFwk();
		break;
	}

	return NULL;	// Pour viter un '<arbibg'.

}

void rgstry::GetMeaning(
	const context___ &Context,
	lcl::meaning_ &Meaning )
{
qRH
	lcl::meaning MeaningBuffer;
	TOL_CBUFFER___ Buffer;
qRB
	switch ( Context.Status ) {
	case sOK:
		qRFwk();
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
		if ( Context.PathErrorRow != qNIL )
			qRFwk();
	case sUnableToFindRootPath:
		Meaning.SetValue( GetLabel( Context.Status ) );
		break;
	default:
		qRFwk();
		break;
	}
qRR
qRT
qRE
}

row__ rgstry::registry_::_Search(
	const name_ &Name,
	const rows_ &NodeRows,
	sdr::row__ &Cursor ) const
{
	sdr::row__ &Row = Cursor;
	bNode Buffer;

	Buffer.Init( Nodes );
	
	if ( Row == qNIL )
		Row = NodeRows.First();
	else
		Row = NodeRows.Next( Row );

	while ( ( Row != qNIL ) && ( Buffer( NodeRows( Row ) ).Name != Name ) )
		Row = NodeRows.Next( Row );

	if ( Row != qNIL )
		return NodeRows( Row );
	else
		return qNIL;

}

row__ rgstry::registry_::_Search(
	nature__ Nature,
	const name_ &Name,
	const rows_ &NodeRows,
	cursor__ &Cursor ) const
{
	row__ Row = qNIL;
	bNode Buffer;

	Buffer.Init( Nodes );
	
	Row = _Search( Name, NodeRows, Cursor );

	while ( ( Row != qNIL ) && ( Buffer( Row ).Nature() != Nature ) )
		Row = _Search( Name, NodeRows, Cursor );

	return Row;
}

row__ rgstry::registry_::_Search(
	const name_ &TagName,
	const name_ &AttributeName,
	const value_ &AttributeValue,
	row__ Row,
	cursor__ &Cursor ) const
{
	row__ ResultRow = qNIL;

	if ( TagName.Amount() != 0 ) {
		// .../TagName...

		ResultRow = _SearchTag( TagName, Row, Cursor );

		if ( ( ResultRow != qNIL ) && ( AttributeName.Amount() != 0 ) ) {
			// .../TagName[@AttributeName='AttributeValue']/...

			while ( ( ResultRow != qNIL ) && ( !_AttributeExists( AttributeName, AttributeValue, ResultRow ) ) ) {
				ResultRow = _SearchTag( TagName, Row, Cursor );
			}
		}
	} else {
		if ( AttributeName.Amount() != 0 ) {
			// .../@AttributeName...
#ifdef RGSTRY_DBG
			if ( AttributeValue.Amount() != 0 )
				qRFwk();
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
	bNode Buffer;

	Buffer.Init( Nodes );

	if ( Row != qNIL ) {
		Name.Append( Buffer( Row ).Name );
		Row = Buffer( Row ).ParentRow();
	}

	while ( Row != qNIL ) {
		Name.InsertAt( Separator );
		Name.InsertAt( Buffer( Row ).Name );

		Row = Buffer( Row ).ParentRow();
	}

	return Name;
}

enum state__ {
	sTagName,
	sAttributeName,
	sAttributeValue,
	s_amount,
	s_Undefined
};

sdr::row__ rgstry::BuildPath(
	const str::string_ &PathString,
	path_ &Path )
{
	sdr::row__ Row = qNIL;
qRH
	bso::bool__ Continue = true;
	state__ State = ::s_Undefined;
	bso::char__ C;
	path_item Item;
	bso::bool__ TagNameAsAttribute = false;
	bso::bool__ AttributeMarkerFound = false;
qRB
	Row = PathString.First();

	Item.Init();

	Continue = Row != qNIL;

	State = sTagName;

	while ( Continue ) {
		C = PathString( Row );
		switch ( State ) {
		case sTagName:
			switch ( C ) {
			case '=':
			case '"':
				Continue = false;
				break;
			case '[':
				if ( TagNameAsAttribute )
					Continue = false;
				else if ( Item.TagName.Amount() == 0 )
					Continue = false;
				State = sAttributeName;
				Item.AttributeName.Init();
				AttributeMarkerFound = false;
				break;
			case '/':
				if ( TagNameAsAttribute )
					Continue = false;
				else if ( Item.TagName.Amount() == 0 )
					Continue = false;
				else
					Path.Append( Item );
				Item.Init();
				break;
			case '@':
				if ( Item.TagName.Amount() != 0 )
					Continue = false;
				else
					TagNameAsAttribute = true;
				break;
			default:
				Item.TagName.Append( C );
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
					if ( ( Row = PathString.Next( Row ) ) == qNIL )
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
					if ( ( Row = PathString.Next( Row ) ) == qNIL )
						Continue = false;
					else if ( PathString( Row ) != ']' )
						Continue = false;
					else if ( ( Row = PathString.Next( Row ) ) == qNIL )
						Continue = false;
					else if ( PathString( Row ) != '/' )
						Continue = false;
					Path.Append( Item );
					Item.Init();
					State = sTagName;
					break;
				default:
					Item.AttributeValue.Append( C );
					break;
				}
				break;
			default:
				qRFwk();
				break;
		}

		if ( Continue )
			Row = PathString.Next( Row );

		if ( Row == qNIL )
			Continue = false;
	}

	if ( Item.TagName.Amount() != 0 ) {
		if ( TagNameAsAttribute ) {
			Item.AttributeName = Item.TagName;
			Item.TagName.Init();
		}

		Path.Append( Item );
	}
qRR
qRT
qRE
	return Row;
}

// Pour simplifier la mise en oeuvre de 'PathErrorRow' en tant que paramtre optionnel (alors == 'NULL').
static inline bso::bool__ BuildPath_(
	const str::string_ &PathString,
	path_ &Path,
	sdr::row__ *PathErrorRow )
{
	sdr::row__ PathErrorRowBuffer = BuildPath( PathString, Path );

	if ( PathErrorRowBuffer != qNIL ) {
		if ( PathErrorRow == NULL )
			qRFwk();
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
	rows_ *ResultRows ) const
{
	cursor__ Cursor = qNIL;
	ctn::qCITEMsl( path_item_ ) Item;
	row__ ResultRow, ChildRow = qNIL;
	bso::bool__ All = ResultRows != NULL;

	Item.Init( Path );	// An item is used, due to recusive behavior of this method.

	ResultRow = ChildRow = _Search( Item( PathRow ), Row, Cursor );

	if ( PathRow != Path.Last() ) {

		while ( ChildRow != qNIL )  {
			ResultRow = _Search( Path, Path.Next( PathRow ), ChildRow, ResultRows );

			if ( ResultRow != qNIL ) {
				if ( All ) {
					ChildRow = _Search( Item( PathRow ), Row, Cursor );
				} else
					ChildRow = qNIL;
			} else
				ChildRow = _Search( Item( PathRow ), Row, Cursor );
		}

	} else if ( All ) {

		while ( ChildRow != qNIL ) {
			ResultRows->Append( ChildRow );

			ChildRow = _Search( Item( PathRow ), Row, Cursor );
		}

	}

	return ResultRow;
}

row__ rgstry::registry_::_Search(
	const path_ &Path,
	row__ Row ) const
{
	if ( Path.Amount() != 0 )
		return _Search( Path, Path.First(), Row, NULL );
	else
		return qNIL;
}

row__ rgstry::registry_::_Search(
	const str::string_ &PathString,
	row__ Row,
	sdr::row__ *PathErrorRow ) const
{
qRH
	path Path;
qRB
	Path.Init();

	if ( !BuildPath_( PathString, Path, PathErrorRow ) )
		Row = qNIL;
	else
		Row = _Search( Path, Row );

qRR
qRT
qRE
	return Row;
}

row__ rgstry::registry_::Create(
	const path_ &Path,
	row__ Row, 
	bso::bool__ Reuse )
{
	bso::bool__ Continue = true;
	sdr::row__ PathRow = Path.First();
	row__ CandidateRow = qNIL;

	while ( ( PathRow != qNIL ) && Continue ) {
		CandidateRow = _Search( Path( PathRow ), Row );

		if ( CandidateRow == qNIL ) {
			Continue = false;

			Row = _Create( Path( PathRow ), Row );
		} else
			Row = CandidateRow;

		PathRow = Path.Next( PathRow );

		if ( !Reuse && ( PathRow == Path.Last() ) )
			Continue = false;
	}

	while ( PathRow != qNIL ) {
		Row = _Create( Path( PathRow ), Row );

		PathRow = Path.Next( PathRow );
	}

	return Row;
}

row__ rgstry::registry_::Create(
	const str::string_ &PathString,
	row__ Row,
	bso::bool__ Reuse,
	sdr::row__ *PathErrorRow )
{
qRH
	path Path;
qRB
	Path.Init();

	if ( !BuildPath_( PathString, Path, PathErrorRow ) )
		Row = qNIL;
	else
		Row = Create( Path, Row, Reuse );

qRR
qRT
qRE
	return Row;
}

row__ rgstry::registry_::SetValue(
	const str::string_ &PathString,
	const value_ &Value,
	row__ Row,
	sdr::row__ *PathErrorRow )
{
qRH
	path Path;
qRB
	Path.Init();

	if ( BuildPath_( PathString, Path, PathErrorRow ) )
		Row = SetValue( Path, Value, Row );
	else
		Row = qNIL;

qRR
qRT
qRE
	return Row;
}

row__ rgstry::registry_::AddValue(
	const str::string_ &PathString,
	const value_ &Value,
	row__ Row,
	sdr::row__ *PathErrorRow )
{
qRH
	path Path;
qRB
	Path.Init();

	if ( BuildPath_( PathString, Path, PathErrorRow ) )
		Row = AddValue( Path, Value, Row );
	else
		Row = qNIL;

qRR
qRT
qRE
	return Row;
}

#ifdef RGSTRY_PREPROCESSOR_NAMESPACE
#	define DEFAULT_NAMESPACE RGSTRY_PREPROCESSOR_NAMESPACE
#else
#	define DEFAULT_NAMESPACE XPP__PREPROCESSOR_DEFAULT_NAMESPACE
#endif

void rgstry::registry_::_Delete( row__ Row )
{
	const node_ &Node = Nodes( Row );

	_Delete( Node.Children );

	Nodes.Remove( Row );
}

bso::bool__ rgstry::registry_::Exists(
	const str::string_ &PathString,
	row__ ParentRow,
	sdr::row__ *PathErrorRow ) const
{
	bso::bool__ Result = false;
qRH
	path Path;
qRB
	Path.Init();

	if ( BuildPath_( PathString, Path, PathErrorRow ) )
		Result = Exists( Path, ParentRow );
qRR
qRT
qRE
	return Result;
}

void rgstry::registry_::_DumpAttributes(
	row__ Row,
	xml::writer_ &Writer ) const
{
qRH
	rows Rows;
	cursor__ Cursor = qNIL;
	bNode Buffer;
qRB
	Rows.Init();
	Buffer.Init( Nodes );

	Rows = Buffer( Row ).Children;

	Cursor = Rows.First();

	while ( Cursor != qNIL ){
		if ( Buffer( Rows( Cursor ) ).Nature() == nAttribute )
			Writer.PutAttribute( Buffer().Name, Buffer().Value );

		Cursor = Rows.Next( Cursor );
	}
qRR
qRT
qRE
}

sdr::size__ rgstry::registry_::_Dump(
	row__ Root,
	bso::bool__ RootToo,
	xml::writer_ &Writer ) const
{
	sdr::size__ ChildAmount = 0;
qRH
	rows Children;
	sdr::row__ Row = qNIL;
	bNode Buffer;
qRB
	Buffer.Init( Nodes );

	Children.Init();
	Children = Buffer( Root ).Children;

	if ( RootToo )
		_DumpNode( Root, Writer );
	
	Row = Children.First();

	while ( Row != qNIL ) {
		if ( Buffer( Children( Row ) ).GetNature() != nAttribute )
			_Dump( Children( Row ), true, Writer );

		Row = Children.Next( Row );
	}

	if ( RootToo )
		Writer.PopTag();

	ChildAmount = Children.Amount();
qRR
qRT
qRE
	return ChildAmount;
}

sdr::size__ rgstry::registry_::Dump(
	row__ Root,
	bso::bool__ RootToo,
	xml::writer_ &Writer ) const
{
	return _Dump( Root, RootToo, Writer );
}

sdr::size__ rgstry::registry_::Dump(
	row__ Root,
	bso::bool__ RootToo,
	xml::outfit__ Outfit,
	xml::encoding__ Encoding,
	txf::text_oflow__ &Flow ) const
{
	sdr::size__ ChildAmount = 0;
qRH
	xml::writer Writer;
qRB
	Writer.Init( Flow, Outfit, Encoding );

	ChildAmount = Dump( Root, RootToo, Writer );
qRR
qRT
qRE
	return ChildAmount;
}


void rgstry::registry_::_Delete( const rows_ &Rows )
{
	sdr::row__ Row = Rows.First();

	while ( Row != qNIL ) {
		_Delete( Rows( Row ) );

		Row = Rows.Next( Row );
	}
}

const value_ &rgstry::registry_::GetValue(
	const path_ &Path,
	row__ Row,
	value_ &Value,
	bso::bool__ *Missing ) const	// Nota : ne met 'Missing'  'true' que lorque 'Path' n'existe pas. Si 'Missing' est  'true', aucune action n'est ralise.
{
	row__ ResultRow = qNIL;
	bNode Buffer;

	Buffer.Init( Nodes );

	if ( *Missing )
		return Value;

	if ( ( ResultRow = _Search( Path, Row ) ) != qNIL )
		Value.Append( Buffer( ResultRow ).Value );
	else
		*Missing = true;

	return Value;
}

const value_ &rgstry::registry_::GetValue(
	const str::string_ &PathString,
	row__ Row,
	value_ &Value,
	bso::bool__ *Missing,
	sdr::row__ *PathErrorRow ) const	// Nota : ne met 'Missing'  'true' que lorque 'Path' n'existe pas. Si 'Missing' est  'true', aucune action n'est ralise.
{
qRH
	path Path;
qRB
	Path.Init();

	if ( *Missing )
		qRReturn;

	if ( BuildPath_( PathString, Path, PathErrorRow ) )
		GetValue( Path, Row, Value, Missing );
	else
		*Missing = true;
qRR
qRT
qRE
	return Value;
}

const value_ &rgstry::registry_::GetValue(
	const tentry__ &Entry,
	row__ Row,
	value_ &Value,
	bso::bool__ *Missing,
	sdr::row__ *PathErrorRow ) const
{
qRH
	str::string Path;
qRB
	Path.Init();

	GetValue( Entry.GetPath( Path ), Row, Value, Missing, PathErrorRow );
qRR
qRT
qRE
	return Value;
}

bso::bool__ rgstry::registry_::GetValues(
	const path_ &Path,
	row__ Row,
	values_ &Values ) const
{
	bso::bool__ Exists = false;
qRH
	cursor__ Cursor = qNIL;
	rows ResultRows;
	bNode Buffer;
qRB
	ResultRows.Init();
	Buffer.Init( Nodes );

	_Search( Path, Path.First(), Row, &ResultRows );

	if ( ResultRows.Amount() != 0 ) {
		Exists = true;
		Cursor = ResultRows.First();

		while ( Cursor != qNIL ) {
			Values.Append( Buffer( ResultRows( Cursor ) ).Value );

			Cursor = ResultRows.Next( Cursor );
		}
	}
qRR
qRT
qRE
	return Exists;
}

bso::bool__ rgstry::registry_::GetValues(
	const str::string_ &PathString,
	row__ ParentRow,
	values_ &Values,
	sdr::row__ *PathErrorRow ) const
{
	bso::bool__ Exists = false;
qRH
	path Path;
qRB
	Path.Init();

	if ( BuildPath_( PathString, Path, PathErrorRow ) )
		Exists = GetValues( Path, ParentRow, Values );
qRR
qRT
qRE
	return Exists;
}

bso::bool__ rgstry::registry_::GetValues(
	const tentry__ &Entry,
	row__ Row,
	values_ &Values,
	sdr::row__ *PathErrorRow ) const
{
	bso::bool__ Result = false;
qRH
	str::string Path;
qRB
	Path.Init();

	Result = GetValues( Entry.GetPath( Path ), Row, Values, PathErrorRow );
qRR
qRT
qRE
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
qRH
	str::string Path;
qRB
	Path.Init();

	Entry.GetPath( Tags, Path );

	Result = GetValues( Path, Row, Values, PathErrorRow );
qRR
qRT
qRE
	return Result;
}
#endif

bso::bool__ rgstry::registry_::Delete(
	const str::string_ &PathString,
	row__ Row,
	sdr::row__ *PathErrorRow )
{
	bso::bool__ Exists = false;
qRH
	path Path;
qRB
	Path.Init();

	if ( BuildPath_( PathString, Path, PathErrorRow ) )
		Exists = Delete( Path, Row );
qRR
qRT
qRE
	return Exists;
}

namespace {
	bso::sBool Parse_(
		xtf::extended_text_iflow__ &XFlow,
		const xpp::criterions___ &Criterions,
		xml::callback__ &Callback,
		xpp::context___ &Context )
	{
		bso::sBool Success = false;
	qRH
		xpp::preprocessing_iflow___ PFlow;
		xtf::extended_text_iflow__ PXFlow;
		xml::status__ Status = xml::s_Undefined;
	qRB
		PFlow.Init( XFlow, xpp::criterions___( Criterions.Directory, Criterions.CypherKey, Criterions.IsNamespaceDefined() ? Criterions.Namespace : str::string( DEFAULT_NAMESPACE ) ) );
		PXFlow.Init( PFlow, XFlow.Format() );

		switch ( Status = xml::Parse( PXFlow, xml::ehReplace, Callback, qRPU ) ) {
		case xml::sOK:
			Success = true;
			break;
		case xml::sUnexpectedEOF:
			PFlow.GetContext( Context );
			break;
		default:
			if ( xml::IsEncodingRelatedError( Status ) )
				PFlow.GetContext( Context );
			else
				// Puisque l'on passe par le prprocesseur, si une erreur est rencontre, xml::Parse(...)' ne peut normalement retourner que 'xml::sUndexpectedEOF'.
				qRFwk();
			break;
		}

		if ( PXFlow.Format() != utf::f_Guess )
			XFlow.SetFormat( PXFlow.Format() );
	qRR
	qRT
	qRE
		return Success;
	}
}

namespace {
	class callback___
	: public xml::callback__
	{
	private:
		registry_ &_Registry;
		row__ _Current, Root_;
		eRootTagHandling RootTagHandling_;
		bso::sBool Ignore_;	// At true when the first call of 'XMLStartTag()' has to be ignored.
	protected:
		virtual bso::bool__ XMLProcessingInstruction( const xml::dump_ & )
		{
			return true;
		}
		virtual bso::bool__ XMLStartTag(
			const str::string_ &TagName,
			const xml::dump_ &Dump )
		{
			if ( Ignore_ )
				Ignore_ = false;
			else {
				if ( _Current == qNIL )
					Root_ = _Current = _Registry.CreateRegistry( TagName );
				else
					_Current = _Registry.AddTag( TagName, _Current );
			}

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
			if ( _Current == qNIL )
				qRFwk();

			_Current = _Registry.GetParentRow( _Current );

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
			Root_ = _Current = qNIL;
			RootTagHandling_ = rth_Undefined;
			Ignore_ = false;
		}
		void Init(
			row__ Root,
			eRootTagHandling RootTagHandling )
		{
			if ( RootTagHandling == rthIgnore ) {
				if ( Root == qNIL )
					qRFwk();
				else
					Ignore_ = true;
			} else
				Ignore_ = false;

			Root_ = _Current = Root;

			RootTagHandling_ = RootTagHandling;
		}
		row__ GetRoot( void ) const
		{
			return Root_;
		}
	};
}


row__ rgstry::Fill(
	xtf::extended_text_iflow__ &XFlow,
	const xpp::criterions___ &Criterions,
	registry_ &Registry,
	xpp::context___ &Context )
{
	row__ Root = qNIL;
qRH
	callback___ Callback( Registry );
qRB
	Callback.Init( qNIL, rthKeep );

	if ( Parse_( XFlow, Criterions, Callback, Context ) )
		Root = Callback.GetRoot();
qRR
qRT
qRE
	return Root;
}

bso::sBool rgstry::Insert(
	xtf::extended_text_iflow__ &XFlow,
	const xpp::criterions___ &Criterions,
	rgstry::row__ Root,
	eRootTagHandling RootTagHandling,
	registry_ &Registry,
	xpp::context___ &Context )
{
	bso::sBool Success = false;
qRH
	callback___ Callback( Registry );
qRB
	Callback.Init( Root, RootTagHandling );

	Success =  Parse_( XFlow, Criterions, Callback, Context );
qRR
qRT
qRE
	return Success;
}


#if 1
const value_ &rgstry::overloaded_registry___::GetValue(
	const str::string_ &PathString,
	value_ &Value,
	bso::bool__ *Missing,
	sdr::row__ *PathErrorRow )const // Nota : ne met 'Missing'  'true' que lorque 'Path' n'existe pas. Si 'Missing' est  'true', aucune action n'est ralise.
{
qRH
	path Path;
qRB
	if ( *Missing )
		qRReturn;

	Path.Init();

	if ( !BuildPath_( PathString, Path, PathErrorRow ) ) {
		*Missing = true;
		qRReturn;
	}

	if ( Local.Registry != NULL )
		Local.Registry->GetValue( Path, Local.Root, Value, Missing );
	else
		*Missing = true;

	if ( *Missing ) {
		*Missing = false;

		if ( Global.Registry != NULL )
			Global.Registry->GetValue( Path, Global.Root, Value, Missing );
	}
qRR
qRT
qRE
	return Value;
}

bso::bool__ rgstry::overloaded_registry___::GetValues(
	const str::string_ &PathString,
	values_ &Values,
	sdr::row__ *PathErrorRow ) const
{
	bso::bool__ Exists = false;
qRH
	path Path;
qRB
	Path.Init();

	if ( !BuildPath_( PathString, Path, PathErrorRow ) )
		qRReturn;

	if ( Local.Registry != NULL )
		Exists = Local.Registry->GetValues( Path, Local.Root, Values );

	Exists = Global.Registry->GetValues( Path, Global.Root, Values ) || Exists;
qRR
qRT
qRE
	return Exists;
}

bso::bool__ rgstry::overloaded_registry___::Exists(
	const str::string_ &PathString,
	sdr::row__ *PathErrorRow ) const
{
	bso::bool__ Exists = false;
qRH
	path Path;
qRB
	Path.Init();

	if ( BuildPath_( PathString, Path, PathErrorRow ) )
		if ( Local.Registry != NULL )
			Exists = Local.Registry->Exists( Path, Local.Root );
	
	Exists = Exists || Global.Registry->Exists( Path, Global.Root );
qRR
qRT
qRE
	return Exists;
}

void rgstry::overloaded_registry___::Search(
	const str::string_ &RawPath,
	row__ &GlobalRow,
	row__ &LocalRow,
	sdr::row__ *PathErrorRow ) const
{
qRH
	path Path;
qRB
	Path.Init();

	if ( BuildPath_( RawPath, Path, PathErrorRow ) )
		Search( Path, GlobalRow, LocalRow );
qRR
qRT
qRE
}
#endif

void rgstry::Dump(
	const entry__ &Entry,
	bso::bool__ RootToo,
	xml::writer_ &Writer )
{
qRH
	hLock Lock;
qRB
	if ( Entry.Registry == NULL )
		qRFwk();

	if ( Entry.Root == qNIL )
		qRFwk();

	Entry.Lock();
	Lock.Set( Entry.Locker );
	Entry.Registry->Dump( Entry.Root, RootToo, Writer );
qRR
qRT
qRE
}

const value_ &rgstry::multi_level_registry_::GetValue(
	const str::string_ &PathString,
	value_ &Value,
	bso::bool__ *Missing,
	sdr::row__ *PathErrorRow ) const	// Nota : ne met 'Missing'  'true' que lorque 'Path' n'existe pas. Si 'Missing' est  'true', aucune action n'est ralise.
{
qRH
	level__ Level = qNIL;
	path Path;
qRB
	if ( *Missing )
		qRReturn;

	Path.Init();

	if ( !BuildPath_( PathString, Path, PathErrorRow ) )
		qRReturn;

	Level = Entries.Last();

	while ( Level != qNIL ) {
		*Missing = false;

		GetValue( Level, Path, Value, Missing );

		if ( *Missing  )
			Level = Entries.Previous( Level );
		else
			Level = qNIL;
	}
qRR
qRT
qRE
	return Value;
}

bso::bool__ rgstry::multi_level_registry_::GetValue(
	const str::string_ &PathString,
	value_ &Value,
	sdr::row__ *PathErrorRow ) const
{
	bso::bool__ Found = false;
qRH
	level__ Level = qNIL;
	path Path;
qRB
	Path.Init();

	if ( !BuildPath_( PathString, Path, PathErrorRow ) )
		qRReturn;

	Level = Entries.Last();

	while ( ( Level != qNIL ) && ( !Found ) ) {
		Found = GetValue( Level, Path, Value );

		Level = Entries.Previous( Level );
	}
qRR
qRT
qRE
	return Found;
}

bso::bool__ rgstry::multi_level_registry_::GetValue(
	const tentry__ &Entry,
	str::string_ &Value ) const
{
	bso::bool__ Found = false;
qRH
	str::string Path;
	sdr::row__ PathErrorRow = qNIL;
qRB
	Path.Init();

	Found = GetValue( Entry.GetPath( Path ), Value, &PathErrorRow );

	if ( PathErrorRow != qNIL )
		qRFwk();
qRR
qRT
qRE
	return Found;
}

bso::bool__ rgstry::multi_level_registry_::GetValue(
	level__ Level,
	const tentry__ &Entry,
	str::string_ &Value ) const
{
	bso::bool__ Found = false;
qRH
	str::string Path;
	sdr::row__ PathErrorRow = qNIL;
qRB
	Path.Init();

	Found = GetValue( Level, Entry.GetPath( Path ), Value, &PathErrorRow );

	if ( PathErrorRow != qNIL )
		qRFwk();
qRR
qRT
qRE
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
qRH
	str::string Path;
qRB
	Path.Init();

	Entry.GetPath( Tags, Path );

	Found = GetValue( Path, Value, PathErrorRow );
qRR
qRT
qRE
	return Found;
}

bso::bool__ rgstry::multi_level_registry_::GetValue(
	const entry___ &Entry,
	str::string_ &Value,
	sdr::row__ *PathErrorRow ) const
{
	bso::bool__ Found = false;
qRH
	tags Tags;
qRB
	Tags.Init();

	Found = GetValue( Entry, Tags, Value, PathErrorRow );
qRR
qRT
qRE
	return Found;
}
#endif

bso::bool__ rgstry::multi_level_registry_::GetValues(
	const str::string_ &PathString,
	values_ &Values,
	sdr::row__ *PathErrorRow ) const
{
	bso::bool__ Found = false;
qRH
	level__ Level = qNIL;
	path Path;
qRB
	Path.Init();

	if ( !BuildPath_( PathString, Path, PathErrorRow ) )
		qRReturn;

	Level = Entries.Last();

	while ( Level != qNIL ) {
		Found |= GetValues( Level, Path, Values );

		Level = Entries.Previous( Level );
	}
qRR
qRT
qRE
	return Found;
}

bso::bool__ rgstry::multi_level_registry_::GetValues(
	const tentry__ &Entry,
	values_ &Values ) const
{
	bso::bool__ Found = false;
qRH
	str::string Path;
	sdr::row__ PathErrorRow = qNIL;
qRB
	Path.Init();
	Entry.GetPath( Path );

	Found = GetValues( Path, Values, &PathErrorRow );

	if ( PathErrorRow != qNIL )
		qRFwk();
qRR
qRT
qRE
	return Found;
}

bso::bool__ rgstry::multi_level_registry_::GetValues(
	level__ Level,
	const tentry__ &Entry,
	values_ &Values ) const
{
	bso::bool__ Found = false;
qRH
	str::string Path;
	sdr::row__ PathErrorRow = qNIL;
qRB
	Path.Init();

	Found = GetValues( Level, Entry.GetPath( Path ), Values, &PathErrorRow );

	if ( PathErrorRow != qNIL )
		qRFwk();
qRR
qRT
qRE
	return Found;
}

bso::bool__ rgstry::multi_level_registry_::SetValue(
	const str::string_ &PathString,
	const value_ &Value,
	sdr::row__ *PathErrorRow )
{
	bso::bool__ Set = false;
qRH
	value CurrentValue;
	sdr::row__ LocalPathErrorRow = qNIL;
qRB
	CurrentValue.Init();

	if ( !GetValue( PathString, CurrentValue, &LocalPathErrorRow ) ) {
		if ( LocalPathErrorRow != qNIL ) {
			*PathErrorRow = LocalPathErrorRow;
			qRReturn;
		}
	} else 
		if ( CurrentValue == Value )
			qRReturn;

	SetValue( Entries.Last(), PathString, Value, PathErrorRow );

	Set = true;
qRR
qRT
qRE
	return Set;
}

bso::bool__ rgstry::multi_level_registry_::SetValue(
	const tentry__ &Entry,
	const value_ &Value,
	sdr::row__ *PathErrorRow )
{
	bso::bool__ Set = false;
qRH
	str::string Path;
qRB
	Path.Init();

	Set = SetValue( Entry.GetPath( Path ), Value, PathErrorRow );
qRR
qRT
qRE
	return Set;
}

bso::bool__ rgstry::multi_level_registry_::AddValue(
	const str::string_ &PathString,
	const value_ &Value,
	sdr::row__ *PathErrorRow )
{
	bso::bool__ Set = false;
qRH
	value CurrentValue;
	sdr::row__ LocalPathErrorRow = qNIL;
qRB
	CurrentValue.Init();

	if ( !GetValue( PathString, CurrentValue, &LocalPathErrorRow ) ) {
		if ( LocalPathErrorRow != qNIL ) {
			*PathErrorRow = LocalPathErrorRow;
			qRReturn;
		}
	}

	AddValue( Entries.Last(), PathString, Value, PathErrorRow );

	Set = true;
qRR
qRT
qRE
	return Set;
}

bso::bool__ rgstry::multi_level_registry_::AddValue(
	const tentry__ &Entry,
	const value_ &Value,
	sdr::row__ *PathErrorRow )
{
	bso::bool__ Set = false;
qRH
	str::string Path;
qRB
	Path.Init();

	Set = AddValue( Entry.GetPath( Path ), Value, PathErrorRow );
qRR
qRT
qRE
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
qRH
	str::string Path;
qRB
	Path.Init();

	Entry.GetPath( Tags, Path );

	Set = SetValue( Path, Value, PathErrorRow );
qRR
qRT
qRE
	return Set;
}

bso::bool__ rgstry::multi_level_registry_::SetValue(
	const entry___ &Entry,
	const value_ &Value,
	sdr::row__ *PathErrorRow )
{
	bso::bool__ Set = false;
qRH
	str::string Path;
qRB
	Path.Init();

	Entry.GetPath( Path );

	Set = SetValue( Path, Value, PathErrorRow );
qRR
qRT
qRE
	return Set;
}
#endif



bso::bool__ rgstry::multi_level_registry_::Delete(
	const str::string_ &PathString,
	sdr::row__ *PathErrorRow )
{
	bso::bool__ Deleted = false;
qRH
	level__ Level = qNIL;
	path Path;
qRB
	Path.Init();

	if ( !BuildPath_( PathString, Path, PathErrorRow ) )
		qRReturn;

	Level = Entries.Last();

	while ( Level != qNIL ) {
		Deleted |= Delete( Path, Level );

		Level = Entries.Previous( Level );
	}
qRR
qRT
qRE
	return Deleted;
}


bso::bool__ rgstry::multi_level_registry_::MoveTo(
	const str::string_ &Path,
	level__ Level )
{
	bso::bool__ Moved = false;
qRH
	str::string Value;
	str::string LevelValue;
qRB
	Value.Init();

	if ( GetValue( Path, Value ) ) {
		LevelValue.Init();

		if ( GetValue( Level, Path, LevelValue ) || ( Value != LevelValue ) ) {
			Delete( Path );
			SetValue( Level, Path, Value );

			Moved = true;
		}
	}
qRR
qRT
qRE
	return Moved;
}

row__ rgstry::multi_level_registry_::Search(
	level__ Level,
	const tentry__ &Entry ) const
{
	row__ Row = qNIL;
qRH
	str::string Path;
qRB
	Path.Init();
	Entry.GetPath( Path );

	Row = Search( Level, Path );
qRR
qRT
qRE
	return Row;
}


row__ rgstry::multi_level_registry_::Search(
	const str::string_ &PathString,
	level__ &Level,
	sdr::row__ *PathErrorRow ) const
{
	row__ Row = qNIL;
qRH
	path Path;
qRB
	Level = qNIL;
	Path.Init();

	if ( !BuildPath_( PathString, Path, PathErrorRow ) )
		qRReturn;

	Level = Entries.Last();

	while ( ( Level != qNIL ) && ( Row == qNIL ) ) {
		Row = Search( Level, Path );

		if ( Row == qNIL )
			Level = Entries.Previous( Level );
	}
qRR
qRT
qRE
	return Row;
}

row__ rgstry::multi_level_registry_::Search(
	const tentry__ &Entry,
	level__ &Level ) const
{
	row__ Row = qNIL;
qRH
	str::string Path;
	sdr::row__ PathErrorRow = qNIL;
qRB
	Path.Init();
	Entry.GetPath( Path );

	Row = Search( Path, Level, &PathErrorRow );

	if ( PathErrorRow != qNIL )
		qRFwk();
qRR
qRT
qRE
	return Row;
}


rgstry::row__ rgstry::Fill(
	xtf::extended_text_iflow__ &XFlow,
	const xpp::criterions___ &Criterions,
	const char *RootPath,
	rgstry::registry_ &Registry,
	context___ &Context )
{
	sdr::row__ PathErrorRow = qNIL;
	rgstry::row__ Root = qNIL, NewRoot = qNIL;

	if ( ( Root = rgstry::Fill(XFlow, Criterions, Registry, Context ) ) != qNIL ) {
		if ( ( RootPath != NULL ) && ( *RootPath ) ) {
			if ( ( NewRoot = Registry.Search( str::string( RootPath ), Root, &PathErrorRow ) ) == qNIL ) {
				if ( PathErrorRow != qNIL ) {
					Context.PathErrorRow = PathErrorRow;
					Context.Status = sRootPathError;
					Registry.Delete( Root );
					Root = qNIL;
				} else  {
					Context.Status = sUnableToFindRootPath;
					Registry.Delete( Root );
					Root = qNIL;
				}
			} else if ( Registry.GetNature(Root) == nAttribute ) {
				Context.Status = sRootPathError;
				Registry.Delete( Root );
				Root = qNIL;
			} else
				Root = NewRoot;
		}
	} else
		Context.Status = sParseError;

	return Root;
}

rgstry::row__ rgstry::Fill(
	const fnm::name___ &FileName,
	const xpp::criterions___ &Criterions,
	const char *RootPath,
	rgstry::registry_ &Registry,
	context___ &Context )
{
	rgstry::row__ Root = qNIL;
qRH
	flf::file_iflow___ FFlow;
	xtf::extended_text_iflow__ XFlow;
	fnm::name___ Location;
	str::string Buffer;
qRB
	if ( FFlow.Init( FileName, err::hUserDefined ) != tol::rSuccess ) {
		Context.Status = sUnableToOpenFile;
		Context.Coordinates.FileName = FileName;
		qRReturn;
	}

	XFlow.Init( FFlow, utf::f_Default );

	if ( !Criterions.Directory.IsEmpty() )
		qRFwk();

	Location.Init();
	Buffer.Init();
	Root = Fill( XFlow, xpp::criterions___( fnm::GetLocation( FileName, Location ).UTF8( Buffer ), Criterions.CypherKey, Criterions.Namespace ), RootPath, Registry, Context );

	if ( Root == qNIL )
		if ( Context.Status == sParseError )
			if ( Context.Coordinates.FileName.IsEmpty() )
				Context.Coordinates.FileName = FileName;
qRR
qRT
qRE
	return Root;
}

#define LIMIT 9

static bso::u8__ GetTagAmount_( const str::string_ &String )
{
	bso::u8__ Amount = 0;

	sdr::row__ Row = String.First();

	while ( ( Row != qNIL ) && ( ( Row = String.Search( RGSTRY__TAG_MARKER_C, Row ) ) != qNIL ) )
	{
		Row = String.Next( Row );

		if ( Row == qNIL )
			qRFwk();

		if ( String( Row ) != RGSTRY__TAG_MARKER_C ) {
			if ( Amount == LIMIT )
				qRLmt();
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
					qRLmt();

				Target.Append( '0' + Indice++ );
			} else if ( *Source == 0 )
				qRFwk();

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
qRH
	str::string Buffer;
	tagsbs::short_tags_callback__ *TagSubstitutionCallback = NULL;
qRB
	Buffer.Init( _Path );

	if ( Tags.Amount() == 0 )
		TagSubstitutionCallback = RGSTRYGetTagSubstitutionCallback();

	if ( TagSubstitutionCallback != NULL )
		tagsbs::SubstituteShortTags( Buffer, *TagSubstitutionCallback, RGSTRY__TAG_MARKER_C );
	else if ( Tags.Amount() != 0  )
		tagsbs::SubstituteShortTags( Buffer, Tags, RGSTRY__TAG_MARKER_C );

	Path.Append( Buffer );
qRR
qRT
qRE
	return Path;
}
