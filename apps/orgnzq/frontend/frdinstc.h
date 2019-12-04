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

#ifndef FRDINSTC_INC_
# define FRDINSTC_INC_

# include "frdfrntnd.h"

namespace frdinstc {
	using namespace frdfrntnd;

	class rPanel_
	{
	private:
		orgnzq::rOGZColumn ColumnBuffer_;
		orgnzq::rOGZField FieldBuffer_;
	public:
		void reset( bso::bool__ P = true )
		{	
			ColumnBuffer_.reset( P );
			FieldBuffer_.reset( P );
		}
		E_CVDTOR( rPanel_ );
		void Init( frdfrntnd::rFrontend &Frontend )
		{
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
		void NewColumnBuffer( void ) const
		{
			ColumnBuffer_.New_1();
		}
		void FillColumnBuffer( sColumn Column ) const
		{
			ColumnBuffer_.Fill_1( *Column	);
		}
		void UpdateColumnBuffer(
			sType Type,
			sNumber Number,
			const str::dString &Label,
			const str::dString &Comment ) const
		{
			ColumnBuffer_.Update_1( *Type, *Number, Label, Comment );
		}
		void GetColumnBuffer(
			sType &Type,
			sNumber &Number,
			str::dString &Label,
			str::dString &Comment ) const
		{
			ColumnBuffer_.Get_1( *Type, *Number, Label, Comment );
		}
		bso::sBool UpdateFieldBufferEntry(
			sEntry Entry,	// if undefined, new entry is created.
			const str::dString &Content )
		{
			bso::sBool Removed = false;

			FieldBuffer_.UpdateEntry_1( *Entry, Content, Removed );

			return Removed;
		}
		void NewFieldBuffer( fbltyp::sObject ColumnBuffer )
		{
			FieldBuffer_.New_1( ColumnBuffer );
		}
		void FillFieldBuffer( sField Field )	// 'Field' must be != 'UndefiendField'. For an new empty field, use 'NewFieldBuffer'.
		{
			FieldBuffer_.Fill_1( *Field );
		}
		void GetFieldBuffer(
			sType &Type,
			sNumber &Number,
			dEntriesI1S &Entries ) const
		{
			FieldBuffer_.Get_1( *Type, *Number, Entries.Ids, Entries.Strings1 );
		}
		void MoveEntry(
			sEntry Source,
			sEntry Target ) const
		{
			FieldBuffer_.MoveEntry_1( *Source, *Target );
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
		rPanel_ Core_;
		qRMV( rFrontend, F_, Frontend_ );
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
			Core_.reset( P );
			Frontend_ = NULL;
			UnselectAll_();
		}
		E_CVDTOR( rPanel );
		void Init( rFrontend &Frontend )
		{
			Frontend_ = &Frontend;

			Core_.Init( Frontend );

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

			if ( Core_.UpdateFieldBufferEntry( Entry_.Get(), Content ) )
				Entry_.MarkAsRemoved();
		}
		void UpdateField( void )
		{
			bso::sBool RecordErased = false;

			if ( Record_ == UndefinedRecord )
				Record_ = F_().CreateRecord();

			if ( Field_ == UndefinedField )
				Field_ = F_().CreateField( Record_, Core_.GetColumnBuffer(), Core_.GetFieldBuffer() );

			if ( F_().UpdateField( Field_, Core_.GetFieldBuffer(), RecordErased) ) {
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

			F_().MoveField( Record_, DraggedField_, Field );
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
	qROW( PPos );	// Panel position.

	class rUser
	{
	private:
		qRMV( rFrontend, F_, Frontend_ );
		lstbch::qLBUNCHw( rPanel*, sPRow ) Panels_;
		bch::qBUNCHw( sPRow, sPPos ) PanelPositions_;
		sPPos CurrentPanelPosition_;
		sPPos DraggedPanel_;
		void DeletePanels_( void );
		sPPos NewPanel_( void )
		{
			sPPos Pos = qNIL;
		qRH
			rPanel *Panel = NULL;
		qRB
			Panel = new rPanel;

			if ( Panel == NULL )
				qRAlc();

			Panel->Init( F_() );

			Pos = PanelPositions_.Append( Panels_.Add( Panel ) );
		qRR
			if ( Panel != NULL )
				delete Panel;
		qRT
		qRE
			return Pos;
		}
	public:
		void reset( bso::bool__ P = true )
		{	
			if ( P )
				DeletePanels_();

			tol::reset( P, Panels_, PanelPositions_, Frontend_ );
			CurrentPanelPosition_ = qNIL;
			DraggedPanel_ = qNIL;
		}
		E_CVDTOR( rUser );
		void Init( frdfrntnd::rFrontend &Frontend )
		{
			Frontend_ = &Frontend;

			DeletePanels_();

			Panels_.Init();
			PanelPositions_.Init();

			CurrentPanelPosition_ =  qNIL;
			DraggedPanel_ = qNIL;
		}
		bso::sBool Login(
			const str::dString &Username,
			const str::dString &Password )
		{
			if ( F_().Login( Username, Password ) ) {
				CurrentPanelPosition_ = NewPanel_();
				return true;
			} else 
				return false;

		}
		void DumpPanels( xml::dWriter &Writer );
		void NewPanel( void )
		{
			if ( PanelPositions_.Amount() >= sclmisc::OGetU8( frdrgstry::parameter::MaxAllowedAmountOfTabs, 10, 25 ) )
				sclmisc::ReportAndAbort( "TooMuchTabs" );

			CurrentPanelPosition_ = NewPanel_();
		}
		void SelectPanel( sPPos Pos )
		{
			if ( !PanelPositions_.Exists( Pos ) )
				qRFwk();

			CurrentPanelPosition_ = Pos;
		}
		bso::sBool HasPanel( void ) const
		{
			return CurrentPanelPosition_ != qNIL;
		}
		void DragPanel( sPPos Pos )
		{
			if ( DraggedPanel_ != qNIL )
				qRGnr();

			DraggedPanel_ = Pos;
		}
		void DropPanel( sPPos Pos )
		{
			if ( DraggedPanel_ == qNIL )
				qRGnr();

			sPRow Row = PanelPositions_( DraggedPanel_ );
			sPRow Current = PanelPositions_( CurrentPanelPosition_ );

			if ( Pos != qNIL ) {
				if ( !PanelPositions_.Exists( Pos ) )
					qRGnr();

				PanelPositions_.InsertAt( Row, Pos );

				PanelPositions_.Remove( (sPPos)( *DraggedPanel_ + ( *DraggedPanel_ > *Pos ? 1 : 0 ) ) );
			}
			else {
				PanelPositions_.Append( Row );

				PanelPositions_.Remove( DraggedPanel_ );

				if ( CurrentPanelPosition_ == DraggedPanel_ )
					CurrentPanelPosition_ = PanelPositions_.Last();
			}

			CurrentPanelPosition_ = PanelPositions_.Search( Current );

		}
		void EndPanelDragging( void )
		{
			DraggedPanel_ = qNIL;
		}
		bso::sBool IsPanelDraggingInProgress( void ) const
		{
			return DraggedPanel_ != qNIL;
		}
		rPanel &Panel( void )
		{
			if ( CurrentPanelPosition_ == qNIL )
				qRGnr();

			return *Panels_( PanelPositions_( CurrentPanelPosition_ ) );
		}
		const rPanel &Panel( void ) const
		{
			if ( CurrentPanelPosition_ == qNIL )
				qRGnr();

			return *Panels_( PanelPositions_( CurrentPanelPosition_ ) );
		}
	};


}

#endif
