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
		orgnzq::rOGZColumn Column_;
		orgnzq::rOGZField Field_;
		orgnzq::rOGZMyObject Object_;
	public:
		void reset( bso::bool__ P = true )
		{	
			Frontend_ = NULL;

			Column_.reset( P );
			Field_.reset( P );
			Object_.reset( P );
		}
		E_CVDTOR( rUser_ );
		void Init( frdfrntnd::rFrontend &Frontend )
		{
			Frontend_ = &Frontend;

			Column_.Init( Frontend.Column );
			Field_.Init( Frontend.Field );
			Object_.Init( Frontend.MyObject );
		}
		fbltyp::sObject GetColumnObjectId( void ) const
		{
			return Column_.ID();
		}
		fbltyp::sObject GetFieldObjectId( void ) const
		{
			return Field_.ID();
		}
		bso::sBool Login(
			const str::dString &Username,
			const str::dString &Password )
		{
			bso::sBool Success = false;

			S_().OGZLogin( Username, Password, Success );

			return Success;
		}
		sRecord CreateRecord( void )	// if == 'UndefinedId', we create empty record.
		{
			sRecord Record = UndefinedRecord;

			S_().OGZCreateRecord( *Record );

			return Record;
		}
		void DefineColumn( sColumn Column ) const
		{
			Column_.Define( *Column	);
		}
		void UpdateColumn(
			sType Type,
			sNumber Number,
			const str::dString &Label,
			const str::dString &Comment ) const
		{
			Column_.Update( *Type, *Number, Label, Comment );
		}
		void GetColumn(
			sType &Type,
			sNumber &Number,
			str::dString &Label,
			str::dString &Comment ) const
		{
			Column_.Get( *Type, *Number, Label, Comment );
		}
		sField CreateField(
			sRecord Record,
			fbltyp::sObject Column )
		{
			fbltyp::sId Id = fbltyp::UndefinedId;

			S_().OGZCreateField( *Record, Column, Id );

			return Id;
		}
		void UpdateEntry(
			sEntry Entry,	// if undefined, new entry is created.
			const str::dString &Content )
		{
			Field_.UpdateEntry( *Entry, Content );
		}
		// 'false' : mono, 'true' : multi.
		void DefineField( sField Field )
		{
			Field_.Define( *Field );
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
		void GetCurrentField(
			sType &Type,
			sNumber &Number,
			dEntriesI1S &Entries ) const
		{
			Field_.Get( *Type, *Number, Entries.Ids, Entries.Strings1 );
		}
		const frdmisc::wXTypes &Types( void ) const
		{
			return F_().Types();
		}
		void UpdateField(
			sField Field,
			fbltyp::sObject FieldBuffer )
		{
			S_().OGZUpdateField( *Field, FieldBuffer );
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
		void UnselectAllItems_( void )
		{
			Record_ = UndefinedRecord;
			Field_ = UndefinedField;
			Entry_ = UndefinedEntry;
			Column_ = UndefinedColumn;
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
			if ( ( Focus_ != tRecord )
				 && ( Focus_ != tColumn ) )
				qRGnr();

			Field_ = Field;
			Focus_ = tField;
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
		void CreateRecord( void )
		{
			FocusOn_( Core_.CreateRecord() );
		}
		void DefineRecord( sRecord Record )
		{
			FocusOn_( Record );
		}
		void DefineField( sField Field )
		{
			if ( Field == UndefinedField ) {
				Core_.DefineColumn( UndefinedColumn );
				FocusOn_( UndefinedColumn );
			} else {
				Core_.DefineField( Field );
				FocusOn_( Field );
			}
		}
		void DefineNewField( void )
		{
			DefineField( UndefinedField );
		}
		void GetColumn(
			sType &Type,
			sNumber &Number,
			str::dString &Label,
			str::dString &Comment ) const
		{
			Core_.GetColumn( Type, Number, Label, Comment );
		}
		void DefineField(
			sType Type,
			sNumber Number,
			const str::dString &Label,
			const str::dString &Comment )
		{
			Core_.UpdateColumn( Type, Number, Label, Comment );
			FocusOn_( Core_.CreateField( Record_, Core_.GetColumnObjectId() ) );
			Core_.DefineField( Field_ );
			FocusOn_( UndefinedEntry );
		}
		void UpdateEntry( const str::dString &Content )
		{
			if ( Focus_ != tField )
				qRGnr();

			Core_.UpdateEntry( Entry_, Content );

			FocusOn_( UndefinedEntry );
		}
		void UpdateField( void )
		{
			if ( Focus_ != tField )
				qRGnr();

			Core_.UpdateField( Field_, Core_.GetFieldObjectId() );

			FocusOn_( tRecord );
		}
		void DumpCurrentRecordColumns( xml::dWriter &Writer ) const;
		void DumpCurrentRecordFields( xml::dWriter &Writer ) const;
		void DumpColumnBuffer( xml::dWriter &Writer ) const;
		void DumpFieldBuffer( xml::dWriter &Writer ) const;
		void DumpFieldBufferCurrentField( xml::dWriter &Writer ) const;
		qRODISCLOSEr( eTarget, Focus );
	};
}

#endif
