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
		void UpdateFieldBufferEntry(
			sEntry Entry,	// if undefined, new entry is created.
			const str::dString &Content )
		{
			FieldBuffer_.UpdateEntry( *Entry, Content );
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

	class rUser
	{
	private:
		rUser_ Core_;
		eTarget Focus_;
		// Focused items.
		sRecord Record_;
		sField Field_;
		sEntry Entry_;
		sColumn Column_;
		sEntry DraggedEntry_;
		sField DraggedField_;
		void UnselectAllItems_( void )
		{
			Record_ = UndefinedRecord;
			Field_ = UndefinedField;
			Entry_ = UndefinedEntry;
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
			Entry_ = UndefinedEntry;
		}
		void FocusOn_( sEntry Entry )
		{
			if ( Focus_ != tField )
				qRGnr();

			Entry_ = Entry;
		}
		void FocusOn_( sColumn Column )
		{
			if ( ( Focus_ != tRecord )
				 && ( Focus_ != tField ) )
				qRGnr();

			Entry_ = UndefinedEntry;
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

				Entry_ = UndefinedEntry;
				break;
			case tRecord:
				if ( Record_ == UndefinedRecord )
					qRGnr();

				Column_ = UndefinedColumn;
				Entry_ = UndefinedEntry;
				Field_ = UndefinedField;
				break;
			case tRecords:
				Record_ = UndefinedRecord;
				Column_ = UndefinedColumn;
				Entry_ = UndefinedEntry;
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
			Core_.reset( P );

			UnselectAll_();
		}
		E_CVDTOR( rUser );
		void Init( frdfrntnd::rFrontend &Frontend )
		{
			if ( Frontend.IsConnected() )
				Core_.Init( Frontend );

			FocusOnRecords_();
		}
		bso::sBool Login(
			const str::dString &Username,
			const str::dString &Password )
		{
			return Core_.Login( Username, Password );
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
			Core_.UpdateColumnBuffer( Type, Number, Label, Comment );
			Core_.NewFieldBuffer( Core_.GetColumnBuffer() );
			FocusOn_( UndefinedField );
			FocusOn_( UndefinedEntry );
		}
		void DefineField( sField Field )
		{
			Core_.FillFieldBuffer( Field );
			FocusOn_( Field );
		}
		void DefineEntry( sEntry Entry )
		{
			FocusOn_( Entry );
		}
		void NewColumn( void )
		{
			Core_.NewColumnBuffer();
			FocusOn_( UndefinedColumn );
		}
		void GetColumn(
			sType &Type,
			sNumber &Number,
			str::dString &Label,
			str::dString &Comment ) const
		{
			Core_.GetColumnBuffer( Type, Number, Label, Comment );
		}
		void UpdateEntry( const str::dString &Content )
		{
			if ( Focus_ != tField )
				qRGnr();

			Core_.UpdateFieldBufferEntry( Entry_, Content );
		}
		void UpdateField( void )
		{
			bso::sBool RecordErased = false;

			if ( Record_ == UndefinedRecord )
				Record_ = Core_.CreateRecord();

			if ( Field_ == UndefinedField )
				Field_ = Core_.CreateField( Record_, Core_.GetColumnBuffer(), Core_.GetFieldBuffer() );

			if ( Core_.UpdateField( Field_, Core_.GetFieldBuffer(), RecordErased) ) {
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

			Core_.MoveEntry( DraggedEntry_, Entry );
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

			Core_.MoveField( Record_, DraggedField_, Field );
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
}

#endif
