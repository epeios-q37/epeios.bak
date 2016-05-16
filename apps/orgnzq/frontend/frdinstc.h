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
		orgnzq::fStatics &S_( void )
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
		bso::sBool Login(
			const str::dString &Username,
			const str::dString &Password )
		{
			bso::sBool Success = false;

			S_().OGZLogin( Username, Password, Success );

			return Success;
		}
		sRecord DefineRecord( sRecord Record )	// if == 'UndefinedId', we create empty record.
		{
			S_().OGZDefineRecord( *Record, *Record );

			return Record;
		}
		void DefineColumn( sColumn Column ) const
		{
			Column_.Define( *Column	);
		}
		sField CreateField(
			sRecord Record,
			fbltyp::sObject Column )
		{
			fbltyp::sId Id = fbltyp::UndefinedId;

			S_().OGZCreateField( *Record, Column, Id );

			return Id;
		}
		void DefineField(
			sRecord Record,
			sField Field )
		{
			Field_.Define( *Record, *Field );
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
		fbltyp::sObject GetColumnObjectId( void ) const
		{
			return Column_.ID();
		}
		void GetRecordColumns(
			fbltyp::sId Record,
			fbltyp::dIds &Columns,
			fbltyp::dIds &Types,
			fbltyp::dId8s &Numbers,
			str::dStrings &Labels,
			str::dStrings &Comments )
		{
			S_().OGZGetRecordColumns( Record, Columns, Types, Numbers, Labels, Comments );
		}
		void GetRecordFields(
			fbltyp::sId Record,
			fbltyp::dIds &Fields,
			fbltyp::dIds &Columns,
			fbltyp::dStringsSet &EntriesSet,
			fbltyp::dIds &Types ) const
		{
			Record_.GetFields( Fields, Columns, EntriesSet, Types );
		}
		const frdmisc::wXTypes &Types( void ) const
		{
			return F_().Types();
		}
	};

	qENUM( View )
	{
		vColumn,
		vField,
		vRecord,
		vRecords,
		v_amount,
		v_Undefined
	};

	const char *GetLabel( eView View );

	class rUser
	{
	private:
		rUser_ Core_;
		eView View_;
		sField Field_;	// Current field.
	public:
		void reset( bso::bool__ P = true )
		{	
			Core_.reset( P );
			View_ = v_Undefined;
			Field_ = UndefinedField;
		}
		E_CVDTOR( rUser );
		void Init( frdfrntnd::rFrontend &Frontend )
		{
			if ( Frontend.IsConnected() )
				Core_.Init( Frontend );

			View_ = vRecords;
			Field_ = UndefinedField;
		}
		bso::sBool Login(
			const str::dString &Username,
			const str::dString &Password )
		{
			return Core_.Login( Username, Password );
		}
		void DefineRecord( sRecord Record )
		{
			Core_.DefineRecord( Record );
			View_ = vRecord;
		}
		void DefineNewRecord( void )
		{
			return DefineRecord( UndefinedRecord );
		}
		void DefineNewField( void )
		{
			Core_.DefineColumn( UndefinedColumn );
			View_ = vColumn;
		}
		void DefineColumn( sColumn Column )
		{
			Core_.DefineColumn( Column );
			View_ = vColumn;
		}
		void GetColumn(
			sType &Type,
			sNumber &Number,
			str::dString &Label,
			str::dString &Comment ) const
		{
			Core_.GetColumn( Type, Number, Label, Comment );
		}
		void CreateField(
			sType Type,
			sNumber Number,
			const str::dString &Label,
			const str::dString &Comment )
		{
			Core_.UpdateColumn( Type, Number, Label, Comment );
			Field_ = Core_.CreateField(Core_.GetColumnObjectId() );
			Core_.
			View_ = vRecord;
		}
		void DumpColumnBuffer( xml::dWriter &Writer ) const;
		void DumpFieldBuffer( xml::dWriter &Writer ) const;
		void DumpRecordColumns( xml::dWriter &Writer ) const;
		void DumpRecordFields( xml::dWriter &Writer ) const;
		qRODISCLOSEr( eView, View );
	};
}

#endif
