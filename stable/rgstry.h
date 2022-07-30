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

#ifndef RGSTRY_INC_
#define RGSTRY_INC_

#define RGSTRY_NAME		"RGSTRY"

#if defined( E_DEBUG ) && !defined( RGSTRY_NODBG )
#define RGSTRY_DBG
#endif

// ReGiSTRY

# include "err.h"
# include "flw.h"
# include "str.h"
# include "lstbch.h"
# include "lstctn.h"
# include "xtf.h"
# include "cpe.h"
# include "xpp.h"
# include "fnm.h"
# include "tagsbs.h"


# define RGSTRY__TAG_MARKER_S	"$"
# define RGSTRY__TAG_MARKER_C	'$'

# define RGSTRY_TAGGING_ATTRIBUTE( attribute ) "[" attribute "=\"" RGSTRY__TAG_MARKER_S "\"]"

# define RGSTRY_TAGGED_ENTRY( tag, attribute )	tag RGSTRY_TAGGING_ATTRIBUTE( attribute )

/*************************/
/****** New version ******/
/*************************/

namespace rgstry {
	class entry___;
	struct tentry__;
	class tentry___;

	typedef tentry__ sTEntry;
	typedef tentry___ rTEntry;
	typedef entry___ rEntry;
}

/*************************/
/****** Old version ******/
/*************************/

// Pr�d�claration.
namespace lcl {
	class meaning_;
}

namespace rgstry {

	typedef str::wString	_term;
	typedef str::dString	_term_;

	typedef _term_			name_;
	typedef _term			name;

	typedef _term_			value_;
	typedef _term			value;

	class path_item_ {
	public:
		struct s {
			name_::s TagName, AttributeName;
			value_::s AttributeValue;
		};
		name_ TagName, AttributeName;
		value_ AttributeValue;
		path_item_( s &S )
		: TagName( S.TagName ),
		  AttributeName( S.AttributeName ),
		  AttributeValue( S.AttributeValue )
		{}
		void reset( bso::bool__ P = true )
		{
			TagName.reset( P );
			AttributeName.reset( P );
			AttributeValue.reset( P);
		}
		void plug( qASd *AS )
		{
			TagName.plug( AS );
			AttributeName.plug( AS );
			AttributeValue.plug( AS );
		}
		path_item_ &operator =( const path_item_ &PI )
		{
			TagName = PI.TagName;
			AttributeName = PI.AttributeName;
			AttributeValue = PI.AttributeValue;

			return *this;
		}
		void Init( void )
		{
			TagName.Init();
			AttributeName.Init();
			AttributeValue.Init();
		}
	};

	E_AUTO( path_item );

	typedef ctn::E_CONTAINER_( path_item_ ) _path_items_;
	class path_
	: public _path_items_
	{
	public:
		struct s
		: _path_items_::s
		{};
		path_( s &S )
		: _path_items_( S )
		{}
		void reset( bso::bool__ P = true )
		{
			_path_items_::reset( P );
		}
		void plug( qASd *AS )
		{
			_path_items_::plug( AS );
		}
		_path_items_ &operator =( const _path_items_ &PI )
		{
			_path_items_::operator =( PI );

			return *this;
		}
		void Init( void )
		{
			_path_items_::Init();
		}
	};


	E_AUTO( path )

	sdr::row__ BuildPath(
		const str::string_ &PathString,
		path_ &Path );

	typedef str::dStrings values_;
	E_AUTO( values )

	E_ROW( row__ );
	typedef bch::E_BUNCH_( row__ ) rows_;
	E_AUTO( rows );

	typedef values_	tags_;
	E_AUTO( tags );

	class entry___
	{
	private:
		void _GetParentPath(
			bso::bool__ NoTailingSlash,
			str::string_ &Path ) const
		{
			if ( _Parent != NULL ) {
				_Parent->GetPath( Path );

				if ( Path.Amount() != 0 ) {
					if ( Path( Path.Last() ) == '/' ) {
						if ( NoTailingSlash )
							Path.Remove( Path.Last() );
					} else if ( !NoTailingSlash )
						Path.Append( '/' );
				}
			}
		}
		bso::bool__ _IsPostInitialized( void ) const
		{
			return ( ( _Path.Amount() != 0 )
				     || ( _RawPath == NULL )
					 || ( *_RawPath == 0 ) );
		}
		void _PostInitialize( void ) const;
		const str::string_ &_GetPath(
			const tags_ &Tags,
			str::string_ &Path ) const;
	private:
		const entry___ *_Parent;
		const char *_RawPath;
		mutable str::string _Path;
	protected:
		virtual tagsbs::cShortTags *RGSTRYGetTagSubstitutionCallback( void ) const
		{
			return NULL;
		}
	public:
		void reset( bso::bool__ P = true )
		{
			_Parent = NULL;
			_RawPath = NULL;
			_Path.reset( P );
		}
		entry___(
			const char *Path = NULL,	// Non dupliqu� !
			const entry___ *Parent = NULL )
		{
			reset( false );
			Init( Path, Parent );
		}
		entry___(
			const char *Path,	// Non dupliqu� !
			const entry___ &Parent )
		{
			reset( false );
			Init( Path, Parent );
		}
		entry___( const entry___ &Entry )
		{
			reset( false );
			Init( Entry );
		}
		virtual ~entry___( void )
		{
			reset();
		}
		void Init(
			const char *Path = NULL,	// Non dupliqu� !
			const entry___ *Parent = NULL )
		{
			_Parent = Parent;
			_RawPath = Path;

			_Path.Init();
		}
		void Init(
			const char *Path,	// Non dupliqu� !
			const entry___ &Parent )
		{
			Init( Path, &Parent );
		}
		void Init( const entry___ &Entry )
		{
			_Parent = Entry._Parent;
			_RawPath = Entry._RawPath;

			_Path.Init();
		}
		const str::string_ &GetPath(
			const tags_ &Tags,
			str::string_ &Path ) const
		{
			if ( !_IsPostInitialized() )
				_PostInitialize();

			return _GetPath( Tags, Path );
		}
		const str::string_ &GetPath( str::string_ &Path ) const
		{
			return GetPath( tags(), Path );
		}
	};

	struct tentry__ {
	private:
		const entry___ *_Entry;
		const tags_ *_Tags;
	protected:
		tentry__( void )
		{
			reset( false );
		}
	public:
		void reset( bso::bool__ = true )
		{
			_Entry = NULL;
			_Tags = NULL;
		}
		tentry__(
			const entry___ &Entry,
			const tags_ *Tags = NULL )
		{
			reset( false );

			Init( Entry, Tags );
		}
		tentry__(
			const entry___ &Entry,
			const tags_ &Tags )
		{
			reset( false );

			Init( Entry, Tags );
		}
		~tentry__( void )
		{
			reset();
		}
		void Init( void )
		{
			_Entry = NULL;
			_Tags = NULL;
		}
		void Init(
			const entry___ &Entry,
			const tags_ *Tags = NULL )
		{
			_Entry = &Entry;
			_Tags = Tags;
		}
		void Init(
			const entry___ &Entry,
			const tags_ &Tags  )
		{
			Init( Entry, &Tags );
		}
		const str::string_ &GetPath( str::string_ &Path ) const
		{
			if ( _Tags != NULL )
				return _Entry->GetPath( *_Tags, Path );
			else
				return _Entry->GetPath( Path );
		}
	};

	class tentry___
	: public tentry__
	{
	private:
		tags _Tags;
	public:
		void reset( bso::bool__ P = true )
		{
			tentry__::reset( P );

			_Tags.reset( P );
		}
		tentry___( void )
		{
			reset( false );
		}
		tentry___( const entry___ &Entry )
		{
			reset( false );

			Init( Entry );
		}
		tentry___(
			const entry___ &Entry,
			const str::string_ &Tag )
		{
			reset( false );

			Init( Entry, Tag );
		}
		tentry___(
			const entry___ &Entry,
			const char *Tag )
		{
			reset( false );

			Init( Entry, Tag );
		}
		tentry___(
			const entry___ &Entry,
			const char *Tag1,
			const str::string_ &Tag2 )
		{
			reset( false );

			Init( Entry, Tag1, Tag2 );
		}
		tentry___(
			const entry___ &Entry,
			const str::string_ &Tag1,
			const char *Tag2 )
		{
			reset( false );

			Init( Entry, Tag1, Tag2 );
		}
		tentry___(
			const entry___ &Entry,
			const str::string_ &Tag1,
			const str::string_ &Tag2 )
		{
			reset( false );

			Init( Entry, Tag1, Tag2 );
		}
		tentry___(
			const entry___ &Entry,
			const char *Tag1,
			const char *Tag2 )
		{
			reset( false );

			Init( Entry, Tag1, Tag2 );
		}
		~tentry___( void )
		{
			reset();
		}
		void Init( void )
		{
			_Tags.Init();

			tentry__::Init();
		}
		void Init( const entry___ &Entry )
		{
			_Tags.Init();

			tentry__::Init( Entry, &_Tags );
		}
		void Init(
			const entry___ &Entry,
			const str::string_ &Tag )
		{
			_Tags.Init();
			_Tags.Append( Tag );

			tentry__::Init( Entry, &_Tags );
		}
		void Init(
			const entry___ &Entry,
			const char *Tag )
		{
			Init( Entry, str::string( Tag ) );
		}
		void Init(
			const entry___ &Entry,
			const str::string_ &Tag1,
			const str::string_ &Tag2  )
		{
			_Tags.Init();
			_Tags.Append( Tag1 );
			_Tags.Append( Tag2 );

			tentry__::Init( Entry, &_Tags );
		}
		void Init(
			const entry___ &Entry,
			const char *Tag1,
			const str::string_ &Tag2  )
		{
			_Tags.Init();
			_Tags.Append( str::string( Tag1 ) );
			_Tags.Append( Tag2 );

			tentry__::Init( Entry, &_Tags );
		}
		void Init(
			const entry___ &Entry,
			const char *Tag1,
			const char *Tag2  )
		{
			_Tags.Init();
			_Tags.Append( str::string( Tag1 ) );
			_Tags.Append( str::string( Tag2 ) );

			tentry__::Init( Entry, &_Tags );
		}
		void Init(
			const entry___ &Entry,
			const str::string_ &Tag1,
			const char *Tag2  )
		{
			_Tags.Init();
			_Tags.Append( Tag1 );
			_Tags.Append( str::string( Tag2 ) );

			tentry__::Init( Entry, &_Tags );
		}
	};

	// Nature du n�ud.
	enum nature__
	{
		nTag,
		nAttribute,
		n_amount,
		n_Undefined
	};

	class node_ {
	public:
		struct s {
			nature__ Nature;
			row__ ParentRow;
			name_::s Name;
			value_::s Value;
			rows_::s Children;
		} &S_;
		name_ Name;
		value_ Value;
		rows_ Children;
		node_( s &S )
		: S_( S ),
		  Name( S.Name ),
		  Value( S.Value ),
		  Children( S.Children )
		{}
		void reset( bso::bool__ P = true )
		{
			S_.Nature = n_Undefined;
			S_.ParentRow = qNIL;

			Name.reset( P );
			Value.reset( P );
			Children.reset( P );
		}
		void plug( qASd *AS )
		{
			Name.plug( AS );
			Value.plug( AS );
			Children.plug( AS );
		}
		node_ &operator =( const node_ &N )
		{
			S_.Nature = N.S_.Nature;
			S_.ParentRow = N.S_.ParentRow;

			Name = N.Name;
			Value = N.Value;
			Children = N.Children;

			return *this;
		}
		void Init( void )
		{
			reset();

			Name.Init();
			Value.Init();
			Children.Init();
		}
		void Init(
			nature__ Nature,
			const name_ &Name,
			const value_ &Value,
			row__ ParentRow = qNIL )
		{
			Init();

			S_.ParentRow = ParentRow;
			S_.Nature = Nature;

			this->Name = Name;
			this->Value = Value;

			Children.Init();
		}
		void Init(
			nature__ Nature,
			const name_ &Name,
			row__ ParentRow = qNIL )
		{
			Init( Nature, Name, value(), ParentRow );
		}
		E_RWDISCLOSE_( row__, ParentRow )
		E_RODISCLOSE_( nature__, Nature );
	};

	typedef lstctn::E_LXCONTAINERt_( node_, row__ ) dNodes_;
	qW( Nodes_ );

	typedef sdr::row__ cursor__;

	typedef ctn::qCITEMs( node_, row__ )	bNode;

	class registry_ {
	private:
		const node_ &GetNode_(
			row__ Row,
			bNode &Buffer ) const
		{
			Buffer.Init( Nodes );

			return Buffer( Row );
		}
		nature__ _GetNature( row__ Row ) const
		{
			bNode Buffer;

			return GetNode_( Row, Buffer ).Nature();
		}
		const name_ &GetName_(
			row__ Row,
			bNode &Buffer ) const
		{
			return GetNode_( Row, Buffer ).Name;
		}
		const value_ &GetValue_(
			row__ Row,
			bNode &Buffer ) const
		{
			return GetNode_( Row, Buffer ).Value;
		}
		row__ _Search(
			const name_ &Name,
			const rows_ &Rows,
			cursor__ &Cursor ) const;
		row__ _Search(
			const name_ &Name,
			row__ Row,
			cursor__ &Cursor ) const
		{
			bNode Buffer;

			return _Search( Name, GetNode_( Row, Buffer ).Children, Cursor );
		}
		row__ _Search(
			nature__ Nature,
			const name_ &Name,
			const rows_ &Rows,
			cursor__ &Cursor ) const;
		row__ _Search(
			nature__ Nature,
			const name_ &Name,
			row__ Row,
			cursor__ &Cursor ) const
		{
			bNode Buffer;

			return _Search( Nature, Name, GetNode_( Row, Buffer ).Children, Cursor );
		}
		row__ _SearchTag(
			const name_ &Name,
			row__ Row,
			cursor__ &Cursor ) const
		{
			return _Search( nTag, Name, Row, Cursor );
		}
		row__ _SearchTag(
			const name_ &Name,
			row__ Row ) const
		{
			cursor__ Cursor = qNIL;

			return _Search( nTag, Name, Row, Cursor );
		}
		row__ _SearchAttribute(
			const name_ &Name,
			row__ Row,
			cursor__ &Cursor ) const
		{
			return _Search( nAttribute, Name, Row, Cursor );
		}
		row__ _SearchAttribute(
			const name_ &Name,
			row__ Row ) const
		{
			cursor__ Cursor = qNIL;

			return _Search( nAttribute, Name, Row, Cursor );
		}
		bso::bool__ _AttributeExists(
			const name_ &Name,
			row__ Row ) const
		{
			return _SearchAttribute( Name, Row ) != qNIL;
		}
		row__ _SearchAttribute(
			const name_ &Name,
			const value_ &Value,
			row__ Row ) const
		{
			Row = _SearchAttribute( Name, Row );

			if ( Row != qNIL ) {
				bNode Buffer;

				if ( GetValue_( Row, Buffer ) != Value )
					Row = qNIL;
			}

			return Row;
		}
		bso::bool__ _AttributeExists(
			const name_ &Name,
			const value_ &Value,
			row__ Row ) const
		{
			return _SearchAttribute( Name, Value, Row ) != qNIL;
		}
		row__ _Search(
			const name_ &TagName,
			const name_ &AttributeName,
			const value_ &AttributeValue,
			row__ Row,
			cursor__ &Cursor ) const;
		row__ _Search(
			const path_item_ &Item,
			row__ Row,
			cursor__ &Cursor ) const
		{
			return _Search( Item.TagName, Item.AttributeName, Item.AttributeValue, Row, Cursor );
		}
		row__ _Search(
			const path_item_ &Item,
			row__ Row ) const
		{
			cursor__ Cursor = qNIL;

			return _Search( Item, Row, Cursor );
		}
		row__ _Search(
			const path_ &Path,
			sdr::row__ PathRow,
			row__ Row,
			rows_ *ResultRows ) const;
		row__ _Search(
			const path_ &Path,
			row__ Row ) const;
		row__ _Search(
			const str::string_ &PathString,
			row__ Row,
			sdr::row__ *PathErrorRow = NULL ) const;
		row__ _CreateWithoutFlush(
			nature__ Nature,
			const name_ &Name,
			const value_ &Value,
			row__ Row = qNIL )
		{
			row__ NewRow = Nodes.New();

			Nodes( NewRow ).Init( Nature, Name, Value, Row );

			return NewRow;
		}
		row__ _CreateWithFlush(
			nature__ Nature,
			const name_ &Name,
			const value_ &Value,
			row__ Row = qNIL )
		{
			Row = _CreateWithoutFlush( Nature, Name, Value, Row );

			Nodes.Flush();

			return Row;
		}
		row__ _CreateTag(
			const name_ &Name,
			row__ Row = qNIL )
		{
			return _CreateWithFlush( nTag, Name, value(), Row );
		}
		row__ _Add(
			nature__ Nature,
			const name_ &Name,
			const value_ &Value,
			row__ Row )
		{
			row__ NewRow = _CreateWithoutFlush( Nature, Name, Value, Row );

			Nodes( Row ).Children.Append( NewRow );

			Nodes.Flush();

			return NewRow;
		}
		row__ _AddTag(
			const name_ &Name,
			row__ Row )
		{
			return _Add( nTag, Name, value(), Row );
		}
		row__ _AddAttribute(
			const name_ &Name,
			const value_ &Value,
			row__ Row )
		{
			return _Add( nAttribute, Name, Value, Row );
		}
		row__ _Create(
			const path_item_ &Item,
			row__ Row )
		{
			if ( Item.TagName.Amount() != 0 ) {
				Row = _AddTag( Item.TagName, Row );

				if ( Item.AttributeName.Amount() != 0 )
					_AddAttribute( Item.AttributeName, Item.AttributeValue, Row );
				else
					if ( Item.AttributeValue.Amount() != 0 )
						qRFwk();

			} else if ( Item.AttributeName.Amount() != 0 )
				Row = _AddAttribute( Item.AttributeName, Item.AttributeValue, Row );
			else
				qRFwk();

			return Row;
		}
		void _Delete( row__ Row );
		void _Delete( const rows_ &Rows );
		void _DumpAttributes(
			row__ Row,
			xml::rWriter &Writer ) const;
		void _DumpNode(
			row__ Row,
			xml::rWriter &Writer ) const
		{
			bNode Buffer;

			Buffer.Init( Nodes );

			Writer.PushTag( Buffer( Row ).Name );	// 'PopTag' correspondant fait par m�thode appelante.
			_DumpAttributes( Row, Writer );

			const value_ &Value = Buffer( Row ).Value;

			if ( Value.Amount() != 0 )
				Writer.PutValue( Value );
		}
		sdr::size__ _Dump(
			row__ Root,
			bso::bool__ RootToo,
			xml::rWriter &Writer ) const;	// Retourne le nombre d'enfants.
	public:
		struct s {
			dNodes_::s Nodes;
		};
		dNodes_ Nodes;
		registry_( s &S )
		: Nodes( S.Nodes )
		{}
		void reset( bso::bool__ P = true )
		{
			Nodes.reset( P );
		}
		void plug( qASd *AS )
		{
			Nodes.plug( AS );
		}
		registry_ &operator =( const registry_ &R )
		{
			Nodes = R.Nodes;

			return *this;
		}
		void Init( void )
		{
			Nodes.Init();
		}
		bso::sBool HasChildren( row__ Row ) const
		{
			return Nodes( Row ).Children.Amount() != 0;
		}
		void Add(
			row__ Row,
			row__ ParentRow )
		{
			Nodes( ParentRow ).Children.Append( Row );

#ifdef RGSTRY_DBG
			if ( Nodes( Row ).ParentRow() != qNIL )
				qRFwk();
#endif
			Nodes( Row ).ParentRow() = ParentRow;

			Nodes.Flush();
		}
		row__ AddTag(
			const name_ &Name,
			row__ Row )
		{
			return _AddTag( Name, Row );
		}
		row__ AddAttribute(
			const name_ &Name,
			const value_ &Value,
			row__ Row )
		{
			return _AddAttribute( Name, Value, Row );
		}
		void SetValue(
			const value_ &Value,
			row__ Row,
			bso::bool__ Append )
		{
			if ( Append )
				Nodes( Row ).Value.Append( Value );
			else
				Nodes( Row ).Value.Init( Value );

			Nodes.Flush();
		}
		row__ Search(
			const path_ &Path,
			row__ Row ) const
		{
			return _Search( Path, Row );
		}
		row__ Search(
			const str::string_ &PathString,
			row__ Row,
			sdr::row__ *PathErrorRow = NULL ) const
		{
			return _Search( PathString, Row, PathErrorRow );
		}
		row__ Create(
			const path_ &Path,
			row__ Row,
			bso::bool__ Reuse );	// If at 'true', if the entry exists, reuse it, otherwise ad a  new entry.
		row__ Create(
			const str::string_ &PathString,
			row__ Row,
			bso::bool__ Reuse,	// If at 'true', if the entry exists, reuse it, otherwise ad a  new entry.
			sdr::row__ *PathErrorRow = NULL );
		nature__ GetNature( row__ Row ) const
		{
			return _GetNature( Row );
		}
		const value_ &GetValue(
			const path_ &Path,
			row__ Row,
			value_ &Value,
			bso::bool__ *Missing ) const;	// Nota : ne met 'Missing' � 'true' que lorsque 'Path' n'existe pas. Si 'Missing' est � 'true', aucune action n'est r�alis�e.
		bso::bool__ GetValue(
			const path_ &Path,
			row__ Row,
			value_ &Value ) const
		{
			bso::bool__ Missing = false;

			GetValue( Path, Row, Value, &Missing );

			return !Missing;
		}
		const value_ &GetValue(
			const str::string_ &PathString,
			row__ Row,
			value_ &Value,
			bso::bool__ *Missing,
			sdr::row__ *PathErrorRow = NULL ) const;	// Nota : ne met 'Missing' � 'true' que lorsque 'Path' n'existe pas. Si 'Missing' est � 'true', aucune action n'est r�alis�e.
		bso::bool__ GetValue(
			const str::string_ &PathString,
			row__ Row,
			value_ &Value ) const
		{
			bso::bool__ Missing = false;

			GetValue( PathString, Row, Value, &Missing );

			return !Missing;
		}
		const value_ &GetValue(
			const tentry__ &Entry,
			row__ Row,
			value_ &Value,
			bso::bool__ *Missing,
			sdr::row__ *PathErrorRow = NULL ) const;	// Nota : ne met 'Missing' � 'true' que lorsque 'Path' n'existe pas. Si 'Missing' est � 'true', aucune action n'est r�alis�e.
		bso::bool__ GetValue(
			const tentry__ &Entry,
			row__ Row,
			value_ &Value ) const
		{
			bso::bool__ Missing = false;

			GetValue( Entry, Row, Value, &Missing );

			return !Missing;
		}
#if 0
		const value_ &GetValue(
			const entry___ &Entry,
			const tags_ &Tags,
			row__ Row,
			bso::bool__ *Missing,
			buffer &Buffer,
			sdr::row__ *PathErrorRow = NULL ) const;	// Nota : ne met 'Missing' � 'true' que lorsque 'Path' n'existe pas. Si 'Missing' est � 'true', aucune action n'est r�alis�e.
#endif
		const value_ &GetValue(
			const tentry__ &Entry,
			row__ Row,
			bso::bool__ *Missing,
			str::string_ &Value,
			sdr::row__ *PathErrorRow = NULL ) const	// Nota : ne met 'Missing' � 'true' que lorsque 'Path' n'existe pas. Si 'Missing' est � 'true', aucune action n'est r�alis�e.
		{
			return GetValue( Entry, Row, Value, Missing, PathErrorRow );
		}
		bso::bool__ GetValues(
			const path_ &Path,
			row__ Row,
			values_ &Values ) const;
		bso::bool__ GetValues(
			const str::string_ &PathString,
			row__ Row,
			values_ &Values,
			sdr::row__ *PathErrorRow = NULL ) const;
		bso::bool__ GetValues(
			const tentry__ &Entry,
			row__ Row,
			values_ &Values,
			sdr::row__ *PathErrorRow = NULL ) const;
#if 0
		bso::bool__ GetValues(
			const entry___ &Entry,
			const tags_ &Tags,
			row__ Row,
			values_ &Values,
			sdr::row__ *PathErrorRow = NULL ) const;
#endif
		row__ SetValue(
			const path_ &Path,
			const value_ &Value,
			row__ Row )
		{
			Row = Create( Path, Row, true );

			SetValue( Value, Row, false );

			return Row;
		}
		row__ AddValue(
			const path_ &Path,
			const value_ &Value,
			row__ Row )
		{
			Row = Create( Path, Row, false );

			SetValue( Value, Row, false );

			return Row;
		}
		row__ SetValue(
			const str::string_ &PathString,
			const value_ &Value,
			row__ Row,
			sdr::row__ *PathErrorRow = NULL );
		row__ AddValue(
			const str::string_ &PathString,
			const value_ &Value,
			row__ Row,
			sdr::row__ *PathErrorRow = NULL );
		row__ GetParentRow( row__ Row ) const
		{
			bNode Buffer;

			return GetNode_( Row, Buffer ).ParentRow();
		}
		const name_ &GetName(
			row__ Row,
			bNode &Buffer ) const
		{
			return GetName_( Row, Buffer );
		}
		const value_ &GetValue(
			row__ Row,
			bNode &Buffer ) const
		{
			return GetValue_( Row, Buffer );
		}
		const str::string_ &GetCompleteName(
			row__ Row,
			str::string_ &Name,
			const char *Separator = ":" ) const;
		row__ SearchTag(
			const name_ &Name,
			row__ Row ) const
		{
			return _SearchTag( Name, Row );
		}
		row__ SearchAttribute(
			const name_ &Name,
			row__ Row ) const
		{
			return _SearchAttribute( Name, Row );
		}
		bso::bool__ AttributeExists(
			const name_ &Name,
			row__ Row ) const
		{
			return _AttributeExists( Name, Row );
		}
		const value_ &GetAttributeValue(
			const name_ &Name,
			row__ Row,
			bNode &Buffer ) const
		{
			row__ ResultRow = _SearchAttribute( Name, Row );

#ifdef RGSTRY_DBG
			if ( Row == qNIL )
				qRFwk();
#endif
			return GetValue_( ResultRow, Buffer );
		}
		void Delete( row__ Row )
		{
			_Delete( Row );
		}
		bso::bool__ Delete(
			const path_ &Path,
			row__ Row )
		{
			row__ ResultRow = _Search( Path, Row );

			if ( ResultRow != qNIL ) {
				_Delete( ResultRow );
				return true;
			} else
				return false;
		}
		bso::bool__ Delete(
			const str::string_ &PathString,
			row__ Row,
			sdr::row__ *PathErrorRow = NULL );
		row__ CreateRegistry( const name_ &Name )
		{
			return _CreateTag( Name );
		}
		bso::bool__ Exists(
			const path_ &Path,
			row__ Row ) const
		{
			return _Search( Path, Row ) != qNIL;
		}
		bso::bool__ Exists(
			const str::string_ &PathString,
			row__ Row,
			sdr::row__ *PathErrorRow = NULL ) const;
		sdr::size__ Dump(
			row__ Root,
			bso::bool__ RootToo,
			xml::rWriter &Writer ) const;	// Retourne le nombre d'enfants.
		sdr::size__ Dump(
			row__ Root,
			bso::bool__ RootToo,
			xml::outfit__ Outfit,
			xml::encoding__ Encoding,
			txf::text_oflow__ &Flow ) const;	// Retourne le nombre d'enfants.
	};

	E_AUTO( registry )

# if 0
	struct error_details_
	{
	public:
		struct s {
			xml::coord__ Coord;
			sdr::row__ PathErrorRow;
			xpp::status__ XPPStatus;
			str::string_::s FileName;
		} &S_;
		str::string_ FileName;
		error_details_( s &S )
		: S_( S ),
		  FileName( S.FileName )
		{}
		void reset( bso::bool__ P = true )
		{
			S_.Coord.reset( P );
			S_.PathErrorRow = qNIL;
			S_.XPPStatus = xpp::s_Undefined;

			S_.Coord.reset( P );
			FileName.reset( P );
		}
		void plug( sdr::E_MEMORY_DRIVER__ &MD )
		{
			FileName.plug( MD );
		}
		void plug( mmm::E_MULTIMEMORY_ &MM )
		{
			FileName.plug( MM );
		}
		error_details_ &operator =( const error_details_ &ED )
		{
			S_.Coord = ED.S_.Coord;
			S_.PathErrorRow = ED.S_.PathErrorRow;
			S_.XPPStatus = ED.S_.XPPStatus;

			FileName = ED.FileName;

			return *this;
		}
		void Init( void )
		{
			reset();

			S_.Coord.Init();
			FileName.Init();
		}
		E_RODISCLOSE_( xml::coord__, Coord );
		E_RODISCLOSE_( sdr::row__, PathErrorRow );
		E_RODISCLOSE_( xpp::status__, XPPStatus );
	};

	E_AUTO( error_details );
#endif

	row__ Fill(
		xtf::extended_text_iflow__ &XFlow,
		const xpp::criterions___ &Criterions,
		registry_ &Registry,
		xpp::context___ &Context );

	inline row__ Fill(
		xtf::extended_text_iflow__ &XFlow,
		const xpp::criterions___ &Criterions,
		registry_ &Registry	) // Peut �tre = 'qNIL', auquel cas une nouvelle 'registry' est cr�e dont la racine est stock�e dans ce param�tre.
	{
		row__ Row = qNIL;
	qRH
		xpp::context___ Context;
	qRB
		Context.Init();

		Row = Fill( XFlow, Criterions, Registry, Context );
	qRR
	qRT
	qRE
		return Row;
	}

	qENUM( RootTagHandling ) {
		rthKeep,		// The root tag is put in the registry.
		rthIgnore,	// The root tag is not put in the registry.
		rth_amount,
		rth_Undefined,
	};

	bso::sBool Insert(
		xtf::extended_text_iflow__ &XFlow,
		const xpp::criterions___ &Criterions,
		rgstry::row__ Root,
		eRootTagHandling RootTagHandling,
		registry_ &Registry,
		xpp::context___ &Context );

	inline bso::sBool Insert(
		xtf::extended_text_iflow__ &XFlow,
		const xpp::criterions___ &Criterions,
		rgstry::row__ Root,
		eRootTagHandling RootTagHandling,
		registry_ &Registry	) // Peut �tre = 'qNIL', auquel cas une nouvelle 'registry' est cr�e dont la racine est stock�e dans ce param�tre.
	{
		bso::sBool Success = false;
	qRH
		xpp::context___ Context;
	qRB
		Context.Init();

		Success = Insert( XFlow, Criterions, Root, RootTagHandling, Registry, Context );
	qRR
	qRT
	qRE
		return Success;
	}

	enum status__ {
		sOK,
		sUnableToOpenFile,
		sParseError,	// Pas d'entr�e dans le fichier de traduction : ce sont les traductions du 'parser__' qui sont utilis�s.
		sUnableToFindRootPath,
		sRootPathError,
		s_amount,
		s_Undefined
	};

	const char *GetLabel( status__ Status );

	typedef xpp::context___ _context___;

	struct context___
	: public _context___
	{
		status__ Status;
		sdr::row__ PathErrorRow;
		void reset( bso::bool__ P = true )
		{
			_context___::reset( P );

			Status = s_Undefined;
			PathErrorRow = qNIL;
		}
		context___( void )
		{
			reset( false );

			Init();
		}
		~context___( void )
		{
			reset();
		}
		void Init( void )
		{
			_context___::Init();

			Status = s_Undefined;
			PathErrorRow = qNIL;
		}
	};


	void GetMeaning(
		const context___ &Context,
		lcl::meaning_ &Meaning );

	rgstry::row__ Fill(
		xtf::extended_text_iflow__ &XFlow,
		const xpp::criterions___ &Criterions,
		const char *RootPath,
		rgstry::registry_ &Registry,
		context___ &Context );

	rgstry::row__ Fill(
		const fnm::name___ &FileName,
		const xpp::criterions___ &Criterions,
		const char *RootPath,
		rgstry::registry_ &Registry,
		context___ &Context );

	template <typename source> inline rgstry::row__ Fill(
		source &Source,
		const xpp::criterions___ &Criterions,
		const char *RootPath,
		rgstry::registry_ &Registry )
	{
		rgstry::row__ Root = qNIL;
	qRH
		context___ Context;
	qRB
		Context.Init();

		Root = Fill( Source, Criterions, RootPath, Registry, Context );
	qRR
	qRT
	qRE
		return Root;
	}

# if 1	// D�pr�ci�, destin� � dispara�tre. Utiliser 'multi_leyer_registry_'.
	class overloaded_registry___
	{
	public:
		struct global {
			const registry_ *Registry;
			row__ Root;
		} Global;
		struct local {
			registry_ *Registry;
			row__ Root;
		} Local;
		void reset( bso::bool__ P = true )
		{
			Global.Registry = NULL;
			Global.Root = qNIL;

			Local.Registry = NULL;
			Local.Root = qNIL;
		}
		overloaded_registry___( void )
		{
			reset( false );
		}
		~overloaded_registry___( void )
		{
			reset();
		}
		void Init(
			const registry_ &Global,
			row__ Root )
		{
			this->Global.Registry = &Global;
			this->Global.Root = Root;

			Local.Registry = NULL;
			Local.Root = qNIL;
		}
		row__ Init(
			const registry_ &Global,
			row__ Root,
			registry_ &Local,	// 'Global' et 'Local' peuvent �tre identiques.
			row__ LocalRoot )	// Si �gal � qNIL, est cr�e et retourn�.
		{
			bNode Buffer;

			Init( Global, Root );

			this->Local.Registry = &Local;

			if ( LocalRoot == qNIL )
				LocalRoot = Local.CreateRegistry( this->Global.Registry->GetName( Root, Buffer ) );

			return this->Local.Root = LocalRoot;
		}
		row__ SetLocal(
			registry_ &Registry,	// Si == 'NULL', on prend le 'Global'.
			row__ Root )	// Si == 'qNIL' est cr�e et retourn�.
		{
			if ( ( Global.Registry == NULL ) || ( Global.Root == qNIL ) )
				qRFwk();

			Local.Registry = &Registry;

			if ( Root == qNIL ) {
				bNode Buffer;

				Root = Registry.CreateRegistry( this->Global.Registry->GetName( Global.Root, Buffer ) );
			}

			return Local.Root = Root;
		}
		const value_ &GetValue(
			const str::string_ &PathString,
			value_ &Value,
			bso::bool__ *Missing,
			sdr::row__ *PathErrorRow = NULL  ) const;	// Nota : ne met 'Missing' � 'true' que lorsque 'Path' n'existe pas. Si 'Missing' est � 'true', aucune action n'est r�alis�e.
		bso::bool__ GetValue(
			const str::string_ &PathString,
			value_ &Value ) const
		{
			bso::bool__ Missing = false;

			GetValue( PathString, Value, &Missing );

			return !Missing;
		}
		bso::bool__ GetValues(
			const str::string_ &PathString,
			values_ &Values,
			sdr::row__ *PathErrorRow = NULL ) const;
		void SetValue(
			const str::string_ &PathString,
			const value_ &Value,
			sdr::row__ *PathErrorRow = NULL )
		{
			Local.Registry->SetValue( PathString, Value, Local.Root, PathErrorRow );
		}
		void AddValue(
			const str::string_ &PathString,
			const value_ &Value,
			sdr::row__ *PathErrorRow = NULL )
		{
			Local.Registry->AddValue( PathString, Value, Local.Root, PathErrorRow );
		}
		void Delete( row__ Row )
		{
			Local.Registry->Delete( Row );
		}
		bso::bool__ Exists(
			const str::string_ &Path,
			sdr::row__ *PathErrorRow = NULL ) const;
		void Search(
			const path_ &Path,
			row__ &GlobalRow,
			row__ &LocalRow ) const
		{
			GlobalRow = Global.Registry->Search( Path, Global.Root );
			LocalRow = Local.Registry->Search( Path, Local.Root );
		}
		void Search(
			const str::string_ &Path,
			row__ &GlobalRow,
			row__ &LocalRow,
			sdr::row__ *PathErrorRow = NULL ) const;
 	};

	class overloaded_unique_registry___	// La base de registre de base et locale sont la m�me.
	: public overloaded_registry___
	{
	private:
		row__ _LocalRoot;
	public:
		void reset( bso::bool__ P = true )
		{
			if ( P ) {
				if ( _LocalRoot != qNIL )
					overloaded_registry___::Delete( _LocalRoot );
			}

			overloaded_registry___::reset( P );
			_LocalRoot = qNIL;
		}
		overloaded_unique_registry___( void )
		{
			reset( false );
		}
		virtual ~overloaded_unique_registry___( void )
		{
			reset();
		}
		row__ Init(
			registry_ &Global,
			row__ Root )	// Si == 'qNIL', est cr�e et retourn�.
		{
			reset();

			if ( Root == qNIL )
				Root = Global.CreateRegistry( name() );

			overloaded_registry___::Init( Global, Root );
			_LocalRoot = qNIL;

			return Root;
		}
		row__ Init(
			registry_ &Global,
			row__ Root,
			row__ LocalRoot ) // Si �gal � qNIL, est cr�e et retourn�.
		{
			reset();

			if ( LocalRoot == qNIL )
				LocalRoot = _LocalRoot = overloaded_registry___::Init( Global, Root, Global, LocalRoot );
			else
				overloaded_registry___::Init( Global, Root, Global, LocalRoot );

			return LocalRoot;
		}
		row__ SetLocal(
			registry_ &Registry,
			row__ Root )	// Si 'Root' == 'qNIL'
		{
			if ( Root == qNIL )
				Root = _LocalRoot = overloaded_registry___::SetLocal( Registry, Root );
			else
				overloaded_registry___::SetLocal( Registry, Root );

			return Root;
		}
	};
# endif
	E_ROW( layer__ );
#	define RGSTRY_UNDEFINED_LAYER	qNIL
	E_CDEF( layer__, UndefinedLayer, qNIL );

	class cLocker
	{
	protected:
		virtual void RGSTRYLock( void ) = 0;
		virtual void RGSTRYUnlock( void ) = 0;
	public:
		qCALLBACK( Locker );
		void Lock( void )
		{
			return RGSTRYLock();
		}
		void Unlock( void )
		{
			return RGSTRYUnlock();
		}
	};

	class hLock
	{
	private:
		cLocker *Locker_;
	public:
		void reset( bso::sBool P = true )
		{
			if ( P ) {
				if ( Locker_ != NULL )
					Locker_->Unlock();
			}

			Locker_ = NULL;
		}
		qCDTOR( hLock );
		void Set( cLocker *Locker )
		{
			Locker_ = Locker;
		}
	};

	struct entry__
	{
		row__ Root;
		const registry_ *Registry;
		cLocker *Locker;
		void reset( bso::bool__ = true )
		{
			Root = qNIL;
			Registry = NULL;
		}
		void Init(
			cLocker &Locker,
			row__ Root )
		{
			this->Root = Root;
			this->Registry = NULL;
			this->Locker = &Locker;
		}
		void Init(
			row__ Root = qNIL,
			const registry_ *Registry = NULL,
			cLocker *Locker = NULL )
		{
			this->Root = Root;
			this->Registry = Registry;
			this->Locker = Locker;
		}
		void Init( const entry__ &Entry )
		{
			*this = Entry;
		}
		entry__(
			row__ Root = qNIL,
			const registry_ *Registry = NULL,
			cLocker *Locker = NULL )
		{
			Init( Root, Registry, Locker );
		}
		entry__(
			row__ Root,
			const registry_ &Registry )
		{
			Init( Root, &Registry, NULL );
		}
		entry__(
			row__ Root,
			const registry_ &Registry,
			cLocker &Locker )
		{
			Init( Root, &Registry, &Locker );
		}
		entry__(
			cLocker &Locker,
			row__ Root )
		{
			Init( Locker, Root );
		}
		bso::bool__ IsInitialized( void ) const
		{
			return ( Root != qNIL );
		}
		void Lock( void ) const
		{
			if ( Locker != NULL )
				Locker->Lock();
		}
		void Unlock( void ) const
		{
			if ( Locker != NULL )
				Locker->Unlock();
		}
	};

	void Dump(
		const entry__ &Entry,
		bso::bool__ RootToo,
		xml::rWriter &Writer );

	typedef stkbch::qBSTACKd( entry__, layer__ ) _entries_;
	typedef stkbch::qBSTACKd( time_t, layer__ ) _timestamps_;

	// Registre multi-niveau
	class multi_layer_registry_
	{
	private:
		void _Touch( layer__ Layer )
		{
			TimeStamps.Store( time( NULL ), Layer );
		}
		layer__ _RawCreateLayer( void )
		{
			layer__ Layer = TimeStamps.Push( 0 );

			if ( Entries.Push( entry__() ) != Layer )
				qRFwk();

			_Touch( Layer );

			return Layer;
		}
		layer__ _RawPushLayer( const entry__ &Entry )
		{
			layer__ Layer = RGSTRY_UNDEFINED_LAYER;

			Entries.Store( Entry, Layer = _RawCreateLayer() );

			return Layer;
		}
		const entry__ _GetEntry( layer__ Layer ) const
		{
			return Entries( Layer );
		}
		bso::bool__ IsInitialized_( layer__ Layer ) const
		{
			return _GetEntry( Layer ).IsInitialized();
		}
		const registry_ &_GetRegistry(
			layer__ Layer,
			hLock &Lock ) const
		{
			entry__ Entry = _GetEntry( Layer );

			Lock.Set( Entry.Locker );
			Entry.Lock();

			if ( Entry.Registry == NULL ) {
				return EmbeddedRegistry;
			} else {
				return *Entry.Registry;
			}
		}
		registry_ &_GetRegistry(
			layer__ Layer,
			hLock &Lock )
		{
			entry__ Entry = _GetEntry( Layer );

			if ( Entry.Registry != NULL )
				qRFwk();

			Lock.Set( Entry.Locker );
			Entry.Lock();

			return EmbeddedRegistry;
		}
		row__ _GetRoot( layer__ Layer ) const
		{
			return _GetEntry( Layer ).Root;
		}
		cLocker *_GetLocker( layer__ Layer ) const
		{
			return _GetEntry( Layer ).Locker;
		}
	public:
		struct s {
			registry_::s EmbeddedRegistry;
			_entries_::s Entries;
			_timestamps_::s TimeStamps;
		};
		registry_ EmbeddedRegistry;
		_entries_ Entries;
		_timestamps_ TimeStamps;
		multi_layer_registry_( s &S )
		: EmbeddedRegistry( S.EmbeddedRegistry ),
		  Entries( S.Entries ),
		  TimeStamps( S.TimeStamps )
		{}
		void reset( bso::bool__ P = true )
		{
			EmbeddedRegistry.reset( P );
			Entries.reset( P );
			TimeStamps.reset( P );
		}
		void plug( qASd *AS )
		{
			EmbeddedRegistry.plug( AS );
			Entries.plug( AS );
			TimeStamps.plug( AS );
		}
		multi_layer_registry_ &operator =( const multi_layer_registry_ &MLR )
		{
			EmbeddedRegistry = MLR.EmbeddedRegistry;
			Entries = MLR.Entries;
			TimeStamps = MLR.TimeStamps;

			return *this;
		}
		void Init( void )
		{
			EmbeddedRegistry.Init();
			Entries.Init();
			TimeStamps.Init();
		}
		E_NAVt( Entries., layer__ );
		const registry_ &GetRegistry(
			layer__ Layer,
			hLock &Lock ) const
		{
			if ( !IsInitialized_( Layer ) )
				qRFwk();

			return _GetRegistry( Layer, Lock );
		}
		registry_ &GetRegistry(
			layer__ Layer,
			hLock &Lock )
		{
			if ( !IsInitialized_( Layer ) )
				qRFwk();

			return _GetRegistry( Layer, Lock );
		}
		bso::bool__ IsEmpty( layer__ Layer ) const
		{
			bso::sBool Result = false;
		qRH
			hLock Lock;
		qRB
			if ( !IsInitialized_( Layer ) )
				qRFwk();

			Result = !GetRegistry( Layer, Lock ).HasChildren( GetRoot( Layer ) );
		qRR
		qRT
		qRE
			return Result;
		}
		row__ GetRoot( layer__ Layer ) const
		{
			return _GetRoot( Layer );
		}
		cLocker *GetLocker( layer__ Layer ) const
		{
			return _GetLocker( Layer );
		}
		layer__ Create( const entry__ &Entry = entry__() )
		{
			return _RawPushLayer( Entry );
		}
		void SetRoot(
			layer__ Layer,
			row__ Root )
		{
			entry__ Entry = Entries( Layer );

			Entry.Root = Root;

			Entries.Store( Entry, Layer );
		}
		void Set(
			layer__ Layer,
			const entry__ Entry )
		{
			if ( IsInitialized_( Layer ) )
				qRFwk();

			Entries.Store( Entry, Layer );
		}
		void Set( layer__ Layer )
		{
			Set( Layer, entry__( EmbeddedRegistry.CreateRegistry( name() ) ) );
		}
		layer__ Push( const entry__ &Entry )
		{
			return _RawPushLayer( Entry );
		}
		layer__ CreateEmbedded( const name_ &Name = name() )
		{
			return _RawPushLayer( entry__( EmbeddedRegistry.CreateRegistry( Name ) ) );
		}
		layer__ CreateEmbedded(
			cLocker &Locker,
			const name_ &Name = name() )
		{
			return _RawPushLayer( entry__( Locker, EmbeddedRegistry.CreateRegistry( Name ) ) );
		}
		void Push(
			const multi_layer_registry_ &Registry,
			layer__ Layer )
		{
		qRH
			hLock Lock;
		qRB
			Push( entry__( Registry.GetRoot( Layer ), Registry.GetRegistry( Layer, Lock ) ) );
		qRR
		qRT
		qRE
		}
		void Push( const multi_layer_registry_ &Registry )
		{
			layer__ Layer = Registry.First();

			while ( Layer != UndefinedLayer ) {
				if ( Registry.IsInitialized_( Layer ) )
					Push( Registry, Layer );

				Layer = Registry.Next( Layer );
			}
		}
		void Erase( layer__ Layer )
		{
			entry__ Entry = Entries( Layer );

			if ( Entry.IsInitialized() ) {

				if ( Entry.Registry == NULL )
					EmbeddedRegistry.Delete( Entry.Root );

				Entry.Init();

				Entries.Store( Entry, Layer );
			}
		}
		layer__ Pop( void )
		{
			layer__ Layer = Entries.Last();

			if ( Entries.Top().Registry == NULL )
				EmbeddedRegistry.Delete( Entries.Top().Root );

			Entries.Pop();
			TimeStamps.Pop();

			return Layer;
		}
		layer__ TopLayer( void ) const
		{
			return Entries.Last();
		}
		void Create(
			layer__ Layer,
			const str::string_ &Path,
			bso::bool__ Reuse )
		{
		qRH
			hLock Lock;
		qRB
			_GetRegistry( Layer, Lock ).Create( Path, _GetRoot( Layer ), Reuse );
		qRR
		qRT
		qRE
		}
		const value_ &GetValue(
			layer__ Layer,
			const path_ &Path,
			value_ &Value,
			bso::bool__ *Missing ) const	// Nota : ne met 'Missing' � 'true' que lorsque 'Path' n'existe pas. Si 'Missing' est � 'true', aucune action n'est r�alis�e.
		{
		qRH
			hLock Lock;
		qRB
			if ( !IsInitialized_( Layer ) )
				*Missing = true;
			else
				_GetRegistry( Layer, Lock ).GetValue( Path, _GetRoot( Layer ), Value, Missing );
		qRR
		qRT
		qRE
			return Value;
		}
		const value_ &GetValue(
			layer__ Layer,
			const str::string_ &PathString,
			value_ &Value,
			bso::bool__ *Missing,
			sdr::row__ *PathErrorRow = NULL  ) const	// Nota : ne met 'Missing' � 'true' que lorsque 'Path' n'existe pas. Si 'Missing' est � 'true', aucune action n'est r�alis�e.
		{
		qRH
			hLock Lock;
		qRB
			if ( !IsInitialized_( Layer ) )
				*Missing = true;
			else
				_GetRegistry( Layer, Lock ).GetValue( PathString, _GetRoot( Layer ), Value, Missing, PathErrorRow );
		qRR
		qRT
		qRE
			return Value;
		}
		const value_ &GetValue(
			const str::string_ &PathString,
			value_ &Value,
			bso::bool__ *Missing,
			sdr::row__ *PathErrorRow = NULL  ) const;	// Nota : ne met 'Missing' � 'true' que lorsque 'Path' n'existe pas. Si 'Missing' est � 'true', aucune action n'est r�alis�e.
		bso::bool__ GetValue(
			layer__ Layer,
			const path_ &Path,
			value_ &Value ) const
		{
			bso::sBool Result = false;
		qRH
			hLock Lock;
		qRB
			if ( IsInitialized_( Layer ) )
				Result = _GetRegistry( Layer, Lock ).GetValue( Path, _GetRoot( Layer ), Value );
		qRR
		qRT
		qRE
			return Result;
		}
		bso::bool__ GetValue(
			layer__ Layer,
			const str::string_ &PathString,
			value_ &Value,
			sdr::row__ *PathErrorRow = NULL ) const
		{
			bso::bool__ Missing = false;

			GetValue( Layer, PathString, Value, &Missing, PathErrorRow );

			return !Missing;
		}
		bso::bool__ GetValue(
			const str::string_ &PathString,
			value_ &Value,
			sdr::row__ *PathErrorRow = NULL ) const;
		bso::bool__ GetValue(
			const char *PathString,
			value_ &Value,
			sdr::row__ *PathErrorRow = NULL ) const
		{
			return GetValue( str::string( PathString ), Value, PathErrorRow );
		}
		bso::bool__ GetValue(
			const tentry__ &Entry,
			str::string_ &Value ) const;
		bso::bool__ GetValue(
			layer__ Layer,
			const tentry__ &Entry,
			str::string_ &Value ) const;
		bso::bool__ GetValues(
			layer__ Layer,
			const path_ &Path,
			values_ &Values ) const
		{
			bso::sBool Result = false;
		qRH
			hLock Lock;
		qRB
			if ( IsInitialized_( Layer ) )
				Result = _GetRegistry( Layer, Lock ).GetValues( Path, _GetRoot( Layer ), Values );
		qRR
		qRT
		qRE
			return Result;
		}
		bso::bool__ GetValues(
			layer__ Layer,
			const str::string_ &PathString,
			values_ &Values,
			sdr::row__ *PathErrorRow = NULL ) const
		{
			bso::sBool Result = false;
		qRH
			hLock Lock;
		qRB
			if ( IsInitialized_( Layer ) )
				Result = _GetRegistry( Layer, Lock ).GetValues( PathString, _GetRoot( Layer ), Values, PathErrorRow );
		qRR
		qRT
		qRE
			return Result;
		}
		bso::bool__ GetValues(
			const str::string_ &PathString,
			values_ &Values,
			sdr::row__ *PathErrorRow = NULL ) const;
		bso::bool__ GetValues(
			const char *PathString,
			values_ &Values,
			sdr::row__ *PathErrorRow = NULL ) const
		{
			return GetValues( str::string( PathString ), Values, PathErrorRow );
		}
		bso::bool__ GetValues(
			const tentry__ &Entry,
			values_ &Values ) const;
		bso::bool__ GetValues(
			layer__ Layer,
			const tentry__ &Entry,
			values_ &Values ) const;
		void SetValue(
			layer__ Layer,
			const str::string_ &PathString,
			const value_ &Value,
			sdr::row__ *PathErrorRow = NULL )
		{
		qRH
			hLock Lock;
		qRB
			if ( !IsInitialized_( Layer ) )
				qRFwk();

			_GetRegistry( Layer, Lock ).SetValue( PathString, Value, _GetRoot( Layer ), PathErrorRow );

			_Touch( Layer );
		qRR
		qRT
		qRE
		}
		void AddValue(
			layer__ Layer,
			const str::string_ &PathString,
			const value_ &Value,
			sdr::row__ *PathErrorRow = NULL )
		{
		qRH
			hLock Lock;
		qRB
			if ( !IsInitialized_( Layer ) )
				qRFwk();

			_GetRegistry( Layer, Lock ).AddValue( PathString, Value, _GetRoot( Layer ), PathErrorRow );

			_Touch( Layer );
		qRR
		qRT
		qRE
		}
		bso::bool__ SetValue(
			const str::string_ &PathString,
			const value_ &Value,
			sdr::row__ *PathErrorRow = NULL );	// Retourne 'false' si 'PathString' a d�j� la valeur 'Value', 'true' sinon.
		bso::bool__ SetValue(
			const tentry__ &Entry,
			const value_ &Value,
			sdr::row__ *PathErrorRow = NULL );	// Retourne 'false' si 'PathString' a d�j� la valeur 'Value', 'true' sinon.
		bso::bool__ AddValue(
			const str::string_ &PathString,
			const value_ &Value,
			sdr::row__ *PathErrorRow = NULL );	// Retourne 'false' si 'PathString' a d�j� la valeur 'Value', 'true' sinon.
		bso::bool__ AddValue(
			const tentry__ &Entry,
			const value_ &Value,
			sdr::row__ *PathErrorRow = NULL );	// Retourne 'false' si 'PathString' a d�j� la valeur 'Value', 'true' sinon.
#if 0
		bso::bool__ SetValue(
			const entry___ &Entry,
			const tags_ &Tags,
			const value_ &Value,
			sdr::row__ *PathErrorRow = NULL );	// Retourne 'false' si 'PathString' a d�j� la valeur 'Value', 'true' sinon.
		bso::bool__ SetValue(
			const entry___ &Entry,
			const value_ &Value,
			sdr::row__ *PathErrorRow = NULL );	// Retourne 'false' si 'PathString' a d�j� la valeur 'Value', 'true' sinon.
#endif
		bso::bool__ Delete(
			const path_ &Path,
			layer__ Layer )
		{
			bso::sBool Result = false;
		qRH
			hLock Lock;
		qRB
			if ( IsInitialized_( Layer ) && _GetRegistry( Layer, Lock ).Delete( Path, _GetRoot( Layer ) ) ) {
				_Touch( Layer );
				Result = true;
			}
		qRR
		qRT
		qRE
			return Result;
		}
		bso::bool__ Delete(
			const str::string_ &PathString,
			layer__ Layer,
			sdr::row__ *PathErrorRow = NULL )	// Retourne 'false' si 'PathString' a d�j� la valeur 'Value', 'true' sinon.
		{
			bso::sBool Result = false;
		qRH
			hLock Lock;
		qRB
			if ( IsInitialized_( Layer ) && _GetRegistry( Layer, Lock ).Delete( PathString, _GetRoot( Layer ), PathErrorRow ) ) {
				_Touch( Layer );
				Result = true;
			}
		qRR
		qRT
		qRE
			return Result;
		}
		bso::bool__ Delete(
			const char *PathString,
			layer__ Layer,
			sdr::row__ *PathErrorRow = NULL )	// Retourne 'false' si 'PathString' a d�j� la valeur 'Value', 'true' sinon.
		{
			return Delete( str::string( PathString ), Layer, PathErrorRow );
		}
		bso::bool__ Delete(
			const str::string_ &PathString,
			sdr::row__ *PathErrorRow = NULL );	// Retourne 'false' si 'PathString' a d�j� la valeur 'Value', 'true' sinon.
		bso::bool__ Delete(
			const char *PathString,
			sdr::row__ *PathErrorRow = NULL )	// Retourne 'false' si 'PathString' a d�j� la valeur 'Value', 'true' sinon.
		{
			return Delete( str::string( PathString ), PathErrorRow );
		}
		bso::bool__ MoveTo(
			const str::string_ &Path,
			layer__ Layer );
		bso::bool__ MoveTo(
			const char *Path,
			layer__ Layer )
		{
			return MoveTo( str::string( Path ), Layer );
		}
		row__ Search(
			layer__ Layer,
			const path_ &Path ) const
		{
			row__ Result = qNIL;
		qRH
			hLock Lock;
		qRB
			if ( IsInitialized_( Layer ) )
				Result = _GetRegistry( Layer, Lock ).Search( Path, _GetRoot( Layer ) );
		qRR
		qRT
		qRE
			return Result;
		}
		row__ Search(
			layer__ Layer,
			const str::string_ &PathString,
			sdr::row__ *PathErrorRow = NULL ) const
		{
			row__ Result = qNIL;
		qRH
			hLock Lock;
		qRB
			if ( IsInitialized_( Layer ) )
				Result = _GetRegistry( Layer, Lock ).Search( PathString, _GetRoot( Layer ), PathErrorRow );
		qRR
		qRT
		qRE
			return Result;
		}
		row__ Search(
			layer__ Layer,
			const tentry__ &Entry ) const;
		row__ Search(
			const str::string_ &PathString,
			layer__ &Layer,	// Valeur retourn�e != 'qNIL', contient le 'layer' de la registry contenant l'entr�e.
			sdr::row__ *PathErrorRow = NULL ) const;
		row__ Search(
			const tentry__ &Entry,
			layer__ &Layer ) const;	// Valeur retourn�e != 'qNIL', contient le 'layer' de la registry contenant l'entr�e.
		bso::bool__ Exists(
			layer__ Layer,
			const path_ &Path ) const
		{
			return Search( Layer, Path ) != qNIL;
		}
		bso::bool__ Exists(
			layer__ Layer,
			const str::string_ &PathString,
			sdr::row__ *PathErrorRow = NULL ) const
		{
			return Search( Layer, PathString, PathErrorRow ) != qNIL;
		}
		bso::bool__ Exists(
			const str::string_ &PathString,
			sdr::row__ *PathErrorRow = NULL ) const
		{
			layer__ Dummy = qNIL;
			return Search( PathString, Dummy, PathErrorRow ) != qNIL;
		}
		bso::bool__ Exists(	const tentry__ &Entry ) const
		{
			layer__ Dummy = qNIL;
			return Search( Entry, Dummy ) != qNIL;
		}
		template <typename source> bso::sBool Fill(
			layer__ Layer,
			source &Source,
			const xpp::criterions___ &Criterions,
			const char *RootPath,
			context___ &Context )
		{
			bso::sBool Result = false;
		qRH
			hLock Lock;
			row__ Root = qNIL;
		qRB
			if ( _GetRoot( Layer ) != qNIL )
				qRFwk();

			if ( ( Root = rgstry::Fill( Source, Criterions, RootPath, _GetRegistry( Layer, Lock ), Context ) ) != qNIL ) {
				Entries.Store( entry__( Root ), Layer );

				_Touch( Layer );

				Result = true;
			}
		qRR
		qRT
		qRE
			return Result;
		}
		template <typename source> bso::sBool Fill(
			layer__ Layer,
			source &Source,
			const xpp::criterions___ &Criterions,
			const char *RootPath )
		{
			bso::sBool Result = false;
		qRH
			hLock Lock;
			row__ Root = qNIL;
		qRB
			if ( _GetRoot( Layer ) != qNIL )
				qRFwk();

			if ( ( Root = rgstry::Fill( Source, Criterions, RootPath, _GetRegistry( Layer, Lock ) ) ) != qNIL ) {
				Entries.Store( entry__( Root ), Layer );

				_Touch( Layer );

				Result = true;
			}
		qRR
		qRT
		qRE
			return Result;
		}
		template <typename source> bso::sBool Insert(
			layer__ Layer,
			source &Source,
			const xpp::criterions___ &Criterions,
			eRootTagHandling RootTagHandling,
			context___ &Context )
		{
			bso::sBool Result = false;
		qRH
			hLock Lock;
		qRB
			if ( rgstry::Insert( Source, Criterions, _GetRoot( Layer ), RootTagHandling, _GetRegistry( Layer, Lock ), Context ) ) {
				_Touch( Layer );

				Result = true;
			} else {
				Context.Status = sParseError;
			}
		qRR
		qRT
		qRE
			return Result;
		}
		template <typename source> bso::sBool Insert(
			layer__ Layer,
			source &Source,
			const xpp::criterions___ &Criterions,
			eRootTagHandling RootTagHandling )
		{
			bso::sBool Result = false;
		qRH
			hLock Lock;
		qRB
			if ( rgstry::Insert( Source, Criterions, _GetRoot( Layer ), RootTagHandling, _GetRegistry( Layer, Lock ) ) ) {
				_Touch( Layer );

				Result = true;
			}
		qRR
		qRT
		qRE
			return Result;
		}
		bso::bool__ Convert(
			const tentry__ TaggedEntry,
			entry__ &Entry,
			err::handling__ ErrHandling = err::h_Default ) const
		{
			bso::sBool Result = false;
		qRH
			hLock Lock;
		qRB
			layer__ Layer = rgstry::UndefinedLayer;

			Entry.Root = Search( TaggedEntry, Layer );

			if ( Entry.Root == qNIL ) {
				if ( ErrHandling == err::hUserDefined )
					return false;
				else
					qRFwk();
			}

			Entry.Registry = &GetRegistry( Layer, Lock );
			Entry.Locker = _GetLocker( Layer );

			Result = true;
		qRR
		qRT
		qRE
			return Result;
		}
		sdr::size__ Dump(
			layer__ Layer,
			row__ Node,	// Si == 'qNIL', on part de la racine.
			bso::bool__ NodeToo,
			xml::rWriter &Writer ) const
		{
			sdr::size__ Result = 0;
		qRH
			hLock Lock;
		qRB
			if ( IsInitialized_( Layer ) )
				Result = _GetRegistry( Layer, Lock ).Dump( Node == qNIL ? _GetRoot( Layer ) : Node, NodeToo, Writer );
		qRR
		qRT
		qRE
			return Result;
		}
		sdr::size__ Dump(
			layer__ Layer,
			row__ Node,	// Si == 'qNIL', on part de la racine.
			bso::bool__ NodeToo,
			xml::outfit__ Outfit,
			xml::encoding__ Encoding,
			txf::text_oflow__ &TFlow ) const
		{
			sdr::size__ Result = 0;
		qRH
			hLock Lock;
		qRB
			if ( IsInitialized_( Layer ) )
				Result = _GetRegistry( Layer, Lock ).Dump( Node == qNIL ? _GetRoot( Layer ) : Node, NodeToo, Outfit, Encoding, TFlow );
		qRR
		qRT
		qRE
			return Result;
		}
		time_t TimeStamp( layer__ Layer ) const
		{
			return TimeStamps( Layer );
		}
	};

	E_AUTO( multi_layer_registry )

	inline str::uint__ _GetUnsigned(
		const str::string_ &RawValue,
		str::uint__ Default,
		bso::bool__ *Error,
		str::uint__ Min,
		str::uint__ Max )
	{
		str::uint__ Value = Default;
		sdr::row__ LocalError = qNIL;

		Value = RawValue.ToUInt(str::sPos(0), &LocalError, str::sBase(), str::sULimit<str::sUInt>(Max) );

		if ( ( LocalError != qNIL ) || ( Value < Min ) ) {

			Value = Default;

			if ( Error != NULL )
				*Error = true;
			else
				qRFwk();
		}

		return Value;
	}

	template <typename registry, typename path> inline str::uint__ _GetUnsigned(
		const registry &Registry,
		const path &Path,
		str::uint__ Default,
		bso::bool__ *Error,
		str::uint__ Min,
		str::uint__ Max )
	{
		str::uint__ Value = Default;
	qRH
		str::string RawValue;
		bso::bool__ ConversionError = false;
	qRB
		RawValue.Init();

		if ( Registry.GetValue( Path, RawValue ) )
			Value = _GetUnsigned( RawValue, Default, &ConversionError, Min, Max );

		if ( ConversionError ) {

			Value = Default;

			if ( Error != NULL )
				*Error = true;
			else
				qRFwk();
		}
	qRR
	qRT
	qRE
		return Value;
	}

	template <typename registry, typename path> inline str::sint__ _GetSigned(
		const registry &Registry,
		const path &Path,
		str::sint__ Default,
		bso::bool__ *Error,
		str::sint__ Min,
		str::sint__ Max )
	{
		str::sint__ Value = Default;
	qRH
		str::string RawValue;
		sdr::row__ GenericError = qNIL;
	qRB
		RawValue.Init();

		if ( Registry.GetValue( Path, RawValue, &GenericError ) )
			Value = str::SConversion(RawValue, str::sPos(0), &GenericError, str::sBase(), str::sSLimits<str::sSInt>(Min, Max));

		if ( ( GenericError != qNIL ) ) {

			Value = Default;

			if ( Error != NULL )
				*Error = true;
			else
				qRFwk();
		}

	qRR
	qRT
	qRE
		return Value;
	}


#ifdef _M
#	define RGSRTY__M_BACKUP	_M
#endif

#define _M( name, type, min, max )\
	template <typename registry, typename path> inline type Get##name(\
		const registry &Registry,\
		const path &Path,\
		type Default,\
		bso::bool__ *Error = NULL,\
		type Min = min,\
		type Max = max )\
	{\
		return (type)_GetUnsigned( Registry, Path, Default, Error, Min, Max );\
	}

	_M( UInt, bso::uint__, BSO_UINT_MIN, BSO_UINT_MAX )
#ifdef BSO__64BITS_ENABLED
	_M( U64, bso::u64__, BSO_U64_MIN, BSO_U64_MAX )
#endif
	_M( U32, bso::u32__, BSO_U32_MIN, BSO_U32_MAX )
	_M( U16, bso::u16__, BSO_U16_MIN, BSO_U16_MAX )
	_M( U8, bso::u8__, BSO_U8_MIN, BSO_U8_MAX )

# undef _M

# ifdef RGSRTY__M_BACKUP
#  define _M RGSRTY__M_BACKUP
# endif


#ifdef _M
#	define RGSRTY__M_BACKUP	_M
#endif

#define _M( name, type, min, max )\
	template <typename registry, typename path> inline type Get##name(\
		const registry &Registry,\
		const path &Path,\
		type Default,\
		bso::bool__ *Error = NULL,\
		type Min = min,\
		type Max = max )\
	{\
		return (type)_GetSigned( Registry, Path, Default, Error, Min, Max );\
	}

	_M( Int, bso::sint__, BSO_SINT_MIN, BSO_SINT_MAX )
#ifdef BSO__64BITS_ENABLED
	_M( S64, bso::s64__, BSO_S64_MIN, BSO_S64_MAX )
#endif
	_M( S32, bso::s32__, BSO_S32_MIN, BSO_S32_MAX )
	_M( S16, bso::s16__, BSO_S16_MIN, BSO_S16_MAX )
	_M( SB, bso::s8__, BSO_S8_MIN, BSO_S8_MAX )


# undef _M

# ifdef RGSRTY__M_BACKUP
# define _M RGSRTY__M_BACKUP
# endif

}
/*******/
/* NEW */
/*******/

namespace rgstry {
	typedef tags_ dTags;
	qW(Tags);
}

#endif
