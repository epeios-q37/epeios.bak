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

#ifndef FRDFRNTND_INC_
# define FRDFRNTND_INC_

# include "ogzinf.h"
# include "frdapi.h"

# include "frdrgstry.h"
# include "frdmisc.h"

# include "sclfrntnd.h"

# include "xml.h"

namespace frdfrntnd {
	typedef sclfrntnd::rFrontend rFrontend_;
	typedef fblfrd::cFrontend cFrontend_;

	using frdmisc::sType;
	using frdmisc::dTypes;
	using frdmisc::UndefinedType;

	SCLF_I1S( number, Number, Id8 );
	SCLF_I( column, Column, Id );
	SCLF_I( field, Field, Id );
	SCLF_I( record, Record, Id );
	SCLF_I1S( digest, Digest, Id );


	SCLF_I1S( entry, Entry, Id );
	typedef dEntrys dEntries;
	qW( Entries );
	typedef dEntrysI1S dEntriesI1S;
	qW( EntriesI1S );

	class rFrontend
	: public rFrontend_,
	  public cFrontend_
	{
	private:
		frdmisc::wXTypes Types_;
		wNumbersI1S Numbers_;
		void GetTypes_( void );
		void DumpTypes_( xml::writer_ &Writer );
		void GetNumbers_( void );
		void DumpNumbers_( xml::writer_ &Writer );
	protected:
		virtual void FBLFRDOnConnection( void ) override
		{
			Statics.Init( *this );
			Column.Init( *this );
			Field.Init( *this );
			MyObject.Init( *this );

			GetTypes_();
			GetNumbers_();
		}
		virtual void FBLFRDOnDisconnection( void ) override
		{
			Statics.reset();
			Column.reset();
			Field.reset();
			MyObject.reset();
		}
	public:
		orgnzq::fStatics Statics;
		orgnzq::fOGZColumnCommon Column;
		orgnzq::fOGZFieldCommon Field;
		orgnzq::fOGZMyObjectCommon MyObject;
		void reset( bso::bool__ P = true )
		{
			rFrontend_::reset( P );
			Types_.reset( P );
			Numbers_.reset( P );

			Statics.reset( P );
			Column.reset( P );
			Field.reset();
			MyObject.reset( P );
		}
		qCVDTOR( rFrontend );
		void Init(
			sclfrntnd::rKernel &Kernel,
			const char *Language,
			fblfrd::reporting_callback__ &ReportingCallback )
		{
			rFrontend_::Init( Kernel, Language, *this, ReportingCallback );
			Types_.Init();
			Numbers_.Init();
		}
		bso::sBool Login(
			const str::dString &Username,
			const str::dString &Password )
		{
			bso::sBool Success = false;

			Statics.OGZLogin_1( Username, Password, Success );

			return Success;
		}
		void GetRecordColumns(
			sRecord Record,
			fbltyp::dIds &Columns,
			fbltyp::dIds &Types,
			fbltyp::dId8s &Numbers,
			str::dStrings &Labels,
			str::dStrings &Comments ) const
		{
			Statics.OGZGetRecordColumns_1( *Record, Columns, Types, Numbers, Labels, Comments );
		}
		void GetRecordFields(
			sRecord Record,
			fbltyp::dIds &Fields,
			fbltyp::dIds &Columns,
			fbltyp::dStringsSet &EntriesSet,
			fbltyp::dIds &Types ) const
		{
			Statics.OGZGetRecordFields_1( *Record, Fields, Columns, Types, EntriesSet );
		}
		void GetEntries(
			const fbltyp::dIds &Fields,
			const fbltyp::dIds &EntryIds,
			fbltyp::dStrings &Entries ) const
		{
			Statics.OGZGetEntries_1( Fields, EntryIds, Entries );
		}
		sRecord CreateRecord( void ) const
		{
			sRecord Record = UndefinedRecord;

			Statics.OGZCreateRecord_1( *Record );

			return Record;
		}
		sField CreateField(
			sRecord Record,
			fbltyp::sObject ColumnBuffer,
			fbltyp::sObject FieldBuffer ) const
		{
			sField Field = UndefinedField;

			Statics.OGZCreateField_1( *Record, ColumnBuffer, FieldBuffer, *Field );

			return Field;
		}
		bso::sBool UpdateField(
			sField Field,
			fbltyp::sObject FieldBuffer,
			bso::sBool &RecordErased ) const
		{
			bso::sBool FieldErased = false;

			Statics.OGZUpdateField_1( *Field, FieldBuffer, FieldErased, RecordErased );

			return FieldErased;
		}
		void GetRecords( dDigestsI1S &Digests ) const
		{
			Statics.OGZGetRecords_1( Digests.Ids, Digests.Strings1 );
		}
		void MoveField(
			sRecord Record,
			sField Source,
			sField Target ) const
		{
			Statics.OGZMoveField_1( *Record, *Source, *Target );
		}
		void DumpCorpus( xml::writer_ &Writer )
		{
			DumpTypes_( Writer );
			DumpNumbers_( Writer );
		}
		const frdmisc::wXTypes &Types( void ) const
		{
			if ( Types_.Amount() == 0 )
				qRGnr();

			return Types_;
		}
	};
}

#endif
