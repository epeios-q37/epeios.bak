/*
	Copyright (C) 2015-2016 Claude SIMON (http://q37.info/contact/).

	This file is part of 'orgnzq' software.

    'orgnzq' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'orgnzq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'orgnzq'.  If not, see <http://www.gnu.org/licenses/>.
*/

// FRontenD INSTanCes

#ifndef FRDINSTC__INC
# define FRDINSTC__INC

# include "frdfrntnd.h"

namespace frdinstc {

	using namespace frdfrntnd;

	class rUser_
	{
	private:
		qRMV( frdfrntnd::rFrontend, F_,  Frontend_ );
		orgnzq::fStatics &S_( void ) const
		{
			return F_().Statics;
		}
		orgnzq::rOGZColumn ColumnBuffer_;
		orgnzq::rOGZField FieldBuffer_;
	public:
		void reset( bso::bool__ P = true )
		{	
			Frontend_ = NULL;

			ColumnBuffer_.reset( P );
			FieldBuffer_.reset( P );
		}
		E_CVDTOR( rUser_ );
		void Init( frdfrntnd::rFrontend &Frontend )
		{
			Frontend_ = &Frontend;

			ColumnBuffer_.Init( Frontend.Column );
			FieldBuffer_.Init( Frontend.Field );
		}
		fbltyp::sObject GetColumnBuffer( void ) const
		{
			return ColumnBuffer_.ID();
		}
		fbltyp::sObject GetFieldBuffer( void ) const
		{
			return FieldBuffer_.ID();
		}
		bso::sBool Login(
			const str::dString &Username,
			const str::dString &Password )
		{
			bso::sBool Success = false;

			S_().OGZLogin( Username, Password, Success );

			return Success;
		}
		void NewColumnBuffer( void ) const
		{
			ColumnBuffer_.New();
		}
		void FillColumnBuffer( sColumn Column ) const
		{
			ColumnBuffer_.Fill( *Column	);
		}
		void UpdateColumnBuffer(
			sType Type,
			sNumber Number,
			const str::dString &Label,
			const str::dString &Comment ) const
		{
			ColumnBuffer_.Update( *Type, *Number, Label, Comment );
		}
		void GetColumnBuffer(
			sType &Type,
			sNumber &Number,
			str::dString &Label,
			str::dString &Comment ) const
		{
			ColumnBuffer_.Get( *Type, *Number, Label, Comment );
		}
		bso::sBool UpdateFieldBufferEntry(
			sEntry Entry,	// if undefined, new entry is created.
			const str::dString &Content )
		{
			bso::sBool Removed = false;

			FieldBuffer_.UpdateEntry( *Entry, Content, Removed );

			return Removed;
		}
		void NewFieldBuffer( fbltyp::sObject ColumnBuffer )
		{
			FieldBuffer_.New( ColumnBuffer );
		}
		void FillFieldBuffer( sField Field )	// 'Field' must be != 'UndefiendField'. For an new empty field, use 'NewFieldBuffer'.
		{
			FieldBuffer_.Fill( *Field );
		}
		void GetRecordColumns(
			sRecord Record,
			fbltyp::dIds &Columns,
			fbltyp::dIds &Types,
			fbltyp::dId8s &Numbers,
			str::dStrings &Labels,
			str::dStrings &Comments ) const
		{
			S_().OGZGetRecordColumns( *Record, Columns, Types, Numbers, Labels, Comments );
		}
		void GetRecordFields(
			sRecord Record,
			fbltyp::dIds &Fields,
			fbltyp::dIds &Columns,
			fbltyp::dStringsSet &EntriesSet,
			fbltyp::dIds &Types ) const
		{
			S_().OGZGetRecordFields( *Record, Fields, Columns, Types, EntriesSet );
		}
		void GetFieldBuffer(
			sType &Type,
			sNumber &Number,
			dEntriesI1S &Entries ) const
		{
			FieldBuffer_.Get( *Type, *Number, Entries.Ids, Entries.Strings1 );
		}
		const frdmisc::wXTypes &Types( void ) const
		{
			return F_().Types();
		}
		sRecord CreateRecord( void ) const
		{
			sRecord Record = UndefinedRecord;

			S_().OGZCreateRecord( *Record );

			return Record;
		}
		sField CreateField(
			sRecord Record,
			fbltyp::sObject ColumnBuffer,
			fbltyp::sObject FieldBuffer ) const
		{
			sField Field = UndefinedField;

			S_().OGZCreateField( *Record, ColumnBuffer, FieldBuffer, *Field );

			return Field;
		}
		bso::sBool UpdateField(
			sField Field,
			fbltyp::sObject FieldBuffer,
			bso::sBool &RecordErased ) const
		{
			bso::sBool FieldErased = false;

			S_().OGZUpdateField( *Field, FieldBuffer, FieldErased, RecordErased );

			return FieldErased;
		}
		void GetRecords( dDigestsI1S &Digests ) const
		{
			S_().OGZGetRecords( Digests.Ids, Digests.Strings1 );
		}
		void MoveEntry(
			sEntry Source,
			sEntry Target ) const
		{
			FieldBuffer_.MoveEntry( *Source, *Target );
		}
		void MoveField(
			sRecord Record,
			sField Source,
			sField Target ) const
		{
			S_().OGZMoveField( *Record, *Source, *Target );
		}
	};

	qENUM( Target )
	{
		tColumn,
		tField,
		tRecord,
		tRecords,
		t_amount,
		t_Undefined
	};

	const char *GetLabel( eTarget Target );

	class sEntryManager
	{
	private:
		sEntry Removed_;
		sEntry Current_;
	public:
		void reset( bso::sBool = true )
		{
			Removed_ = Current_ = UndefinedEntry;
		}
		qCDTOR( sEntryManager );
		void Init( void )
		{
			Removed_ = Current_ = UndefinedEntry;
		}
		sEntry Get( void ) const
		{
			if ( Removed_ != UndefinedEntry )
				qRGnr();

			return Current_;
		}
		void Set( sEntry Entry )
		{
			if ( Removed_ != UndefinedEntry ) {
				if ( ( Entry != UndefinedEntry ) && ( **Entry >= **Removed_ ) )
					(**Entry )--;

				Removed_ = UndefinedEntry;
			}

			Current_ = Entry;
		}
		void MarkAsRemoved( void )
		{
			Removed_ = Current_;

			Current_ = UndefinedEntry;
		}
	};

	class rPanel
	{
	private:
		qRMV( rUser_, C_, Core_ );
		eTarget Focus_;
		// Focused items.
		sRecord Record_;
		sField Field_;
		sEntryManager Entry_;
		sColumn Column_;
		sEntry DraggedEntry_;
		sField DraggedField_;
		void UnselectAllItems_( void )
		{
			Record_ = UndefinedRecord;
			Field_ = UndefinedField;
			Entry_.reset();
			Column_ = UndefinedColumn;
			DraggedEntry_ = UndefinedEntry;
			DraggedField_ = UndefinedField;
		}
		void UnselectAll_( void )
		{
			Focus_ = t_Undefined;

			UnselectAllItems_();
		}
		void FocusOnRecords_( void )
		{
			UnselectAllItems_();

			Focus_ = tRecords;
		}
		void FocusOn_( sRecord Record )
		{
			UnselectAllItems_();

			Record_ = Record;
			Focus_ = tRecord;
		}
		void FocusOn_( sField Field )
		{
			Field_ = Field;
			Focus_ = tField;
			Entry_.Set( UndefinedEntry );
		}
		void FocusOn_( sEntry Entry )
		{
			if ( Focus_ != tField )
				qRGnr();

			Entry_.Set( Entry );
		}
		void FocusOn_( sColumn Column )
		{
			if ( ( Focus_ != tRecords )
			     && ( Focus_ != tRecord )
				 && ( Focus_ != tField ) )
				qRGnr();

			Entry_.Set( UndefinedEntry );
			Field_ = UndefinedField;
			Column_ = Column;
			Focus_ = tColumn;
		}
		void FocusOn_( eTarget Target )
		{
			switch ( Target ) {
			case tField:
				if ( Field_ == UndefinedField )
					qRGnr();

				Entry_.Set( UndefinedEntry );
				break;
			case tRecord:
				if ( Record_ == UndefinedRecord )
					qRGnr();

				Column_ = UndefinedColumn;
				Entry_.Set( UndefinedEntry );
				Field_ = UndefinedField;
				break;
			case tRecords:
				Record_ = UndefinedRecord;
				Column_ = UndefinedColumn;
				Entry_.Set( UndefinedEntry );
				Field_ = UndefinedField;
				break;
			default:
				qRGnr();
				break;
			}

			Focus_ = Target;
		}
	public:
		void reset( bso::bool__ P = true )
		{	
			Core_ = NULL;
			UnselectAll_();
		}
		E_CVDTOR( rPanel );
		void Init( rUser_ &Core )
		{
			Core_ = &Core;

			UnselectAll_();

			FocusOnRecords_();
		}
		void NewRecord( void )
		{
			FocusOn_( UndefinedRecord );
		}
		void DefineRecord( sRecord Record )
		{
			FocusOn_( Record );
		}
		void NewField(
			sType Type,
			sNumber Number,
			const str::dString &Label,
			const str::dString &Comment )
		{
			C_().UpdateColumnBuffer( Type, Number, Label, Comment );
			C_().NewFieldBuffer( C_().GetColumnBuffer() );
			FocusOn_( UndefinedField );
			FocusOn_( UndefinedEntry );
		}
		void DefineField( sField Field )
		{
			C_().FillFieldBuffer( Field );
			FocusOn_( Field );
		}
		void DefineEntry( sEntry Entry )
		{
			FocusOn_( Entry );
		}
		void NewColumn( void )
		{
			C_().NewColumnBuffer();
			FocusOn_( UndefinedColumn );
		}
		void GetColumn(
			sType &Type,
			sNumber &Number,
			str::dString &Label,
			str::dString &Comment ) const
		{
			C_().GetColumnBuffer( Type, Number, Label, Comment );
		}
		void UpdateEntry( const str::dString &Content )
		{
			if ( Focus_ != tField )
				qRGnr();

			if ( C_().UpdateFieldBufferEntry( Entry_.Get(), Content ) )
				Entry_.MarkAsRemoved();
		}
		void UpdateField( void )
		{
			bso::sBool RecordErased = false;

			if ( Record_ == UndefinedRecord )
				Record_ = C_().CreateRecord();

			if ( Field_ == UndefinedField )
				Field_ = C_().CreateField( Record_, C_().GetColumnBuffer(), C_().GetFieldBuffer() );

			if ( C_().UpdateField( Field_, C_().GetFieldBuffer(), RecordErased) ) {
				Field_ = UndefinedField;

				if ( RecordErased )
					FocusOn_( tRecords );
				else
					FocusOn_( tRecord );
			} else
				FocusOn_( tField );
		}
		void BackToList( void )
		{
			FocusOn_( tRecords );
		}
		void DragEntry( sEntry Entry )
		{
			if ( DraggedEntry_ != UndefinedEntry )
				qRGnr();

			DraggedEntry_ = Entry;
		}
		void DropEntry( sEntry Entry )
		{
			if ( DraggedEntry_ == UndefinedEntry )
				qRGnr();

			C_().MoveEntry( DraggedEntry_, Entry );
		}
		void EndEntryDragging( void )
		{
			DraggedEntry_ = UndefinedEntry;
		}
		bso::sBool IsEntryDraggingInProgress( void ) const
		{
			return DraggedEntry_ != UndefinedEntry;
		}
		void DragField( sField Field )
		{
			if ( DraggedField_ != UndefinedField )
				qRGnr();

			DraggedField_ = Field;

			FocusOn_( tRecord );
		}
		void DropField( sField Field )
		{
			if ( Record_ == UndefinedRecord )
				qRGnr();

			if ( DraggedField_ == UndefinedField )
				qRGnr();

			C_().MoveField( Record_, DraggedField_, Field );
		}
		void EndFieldDragging( void )
		{
			DraggedField_ = UndefinedField;
		}
		bso::sBool IsFieldDraggingInProgress( void ) const
		{
			return DraggedField_ != UndefinedField;
		}
		bso::sBool IsDraggingInProgress( void ) const
		{
			return IsFieldDraggingInProgress() || IsEntryDraggingInProgress();
		}
		void DumpCurrentRecordColumns( xml::dWriter &Writer ) const;
		void DumpCurrentRecordFields( xml::dWriter &Writer ) const;
		void DumpColumnBuffer( xml::dWriter &Writer ) const;
		void DumpFieldBuffer( xml::dWriter &Writer ) const;
		void DumpRecords( xml::dWriter &Writer ) const;
		qRODISCLOSEr( eTarget, Focus );
	};

	qROW( PRow );	// Panel row.

	typedef lstbch::qLBUNCHd( rPanel*, sPRow ) dPanels;
	qW( Panels );

	class rUser
	{
	private:
		rUser_ Core_;
		wPanels Panels_;
		sPRow CurrentPanel_;
		void DeletePanels_( void );
		sPRow NewPanel_( void )
		{
			sPRow Row = qNIL;
		qRH
			rPanel *Panel = NULL;
		qRB
			rPanel *Panel = new rPanel;

			if ( Panel == NULL )
				qRAlc();

			Panel->Init( Core_ );

			Row = Panels_.Add( Panel );
		qRR
			if ( Panel != NULL )
				delete Panel;
		qRT
		qRE
			return Row;
		}
	public:
		void reset( bso::bool__ P = true )
		{	
			Core_.reset( P );

			if ( P )
				DeletePanels_();

			Panels_.reset( P );
			CurrentPanel_ = qNIL;
		}
		E_CVDTOR( rUser );
		void Init( frdfrntnd::rFrontend &Frontend )
		{
			if ( Frontend.IsConnected() )
				Core_.Init( Frontend );

			DeletePanels_();

			CurrentPanel_ =  qNIL;
		}
		bso::sBool Login(
			const str::dString &Username,
			const str::dString &Password )
		{
			if ( Core_.Login( Username, Password ) ) {
				Panels_.Init();
				CurrentPanel_ = NewPanel_();
				CurrentPanel_ = NewPanel_();
				CurrentPanel_ = NewPanel_();
				CurrentPanel_ = NewPanel_();
				CurrentPanel_ = NewPanel_();
				CurrentPanel_ = NewPanel_();
				return true;
			} else 
				return false;

		}
		void DumpPanels( xml::dWriter &Writer );
		void SelectPanel( sPRow Row )
		{
			if ( !Panels_.Exists( Row ) )
				qRFwk();

			CurrentPanel_ = Row;
		}
		bso::sBool HasPanel( void ) const
		{
			return CurrentPanel_ != qNIL;
		}
		rPanel &Panel( void )
		{
			if ( CurrentPanel_ == qNIL )
				qRGnr();

			return *Panels_( CurrentPanel_ );
		}
		const rPanel &Panel( void ) const
		{
			if ( CurrentPanel_ == qNIL )
				qRGnr();

			return *Panels_( CurrentPanel_ );
		}
	};


}

#endif
