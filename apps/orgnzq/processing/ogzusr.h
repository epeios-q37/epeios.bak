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

// OrGaniZer USeR

#ifndef OGZUSR__INC
# define OGZUSR__INC

# ifdef XXX_DBG
#	define OGZUSR__DBG
# endif

# include "ogzbsc.h"
# include "ogzetr.h"
# include "ogzfld.h"
# include "ogzrcd.h"

# include "lstcrt.h"

namespace ogzusr {
	typedef bso::sUInt sIndice;	// Begins at '0'.

	typedef bso::sUInt sAmount;	// If == '0', return all available.
	qCDEF( sIndice, AmountMax, BSO_UINT_MAX );

	qROW( Row );

	typedef ogzbsc::dList<ogzbsc::sRRow> dRecords;
	qW( Records );

	typedef dRecords dSet;
	qW( Set );

	class cUser
	{
	protected:
		// If 'Row' != 'qNIL', it must be used.
		virtual sRow OGZUSRNew( sRow User ) = 0;
		// If 'Row' == 'qNIL', the content must be erased.
		virtual void OGZUSRDelete( sRow User ) = 0;
		// Columns related methods.
		virtual ogzbsc::sCRow OGZUSRAdd(
			ogzclm::sRow Column,
			sRow User ) = 0;
		virtual void OGZUSRRemove(
			ogzbsc::sCRow Column,
			sRow User ) = 0;
		virtual void OGZUSRGetRaws(
			const ogzfld::dColumns &Columns,
			sRow User,
			ogzclm::dRows &RawColumns ) = 0;
		// Meta related methods.
		virtual ogzbsc::sMRow OGZUSRAdd(
			ogzmta::sRow Meta,
			sRow User ) = 0;
		virtual void OGZUSRRemove(
			ogzbsc::sMRow Entry,
			sRow User ) = 0;
		virtual void OGZUSRGetRaws(
			const ogzclm::dMetas &Metas,
			sRow User,
			ogzmta::dRows &RawMetas ) = 0;
		// Storage related methods.
		virtual ogzbsc::sERow OGZUSRAdd(
			ogzetr::sRow Entry,
			sRow User ) = 0;
		virtual void OGZUSRRemove(
			ogzbsc::sERow Entry,
			sRow User ) = 0;
		virtual void OGZUSRGetRaws(
			const ogzfld::dEntries &Entries,
			sRow User,
			ogzetr::dRows &RawEntries ) = 0;
		// Fields related methods.
		virtual ogzbsc::sFRow OGZUSRAdd(
			ogzfld::sRow Field,
			sRow User ) = 0;
		virtual void OGZUSRRemove(
			ogzbsc::sFRow Field,
			sRow User ) = 0;
		virtual void OGZUSRGetRaws(
			const ogzrcd::dFields &Fields,
			sRow User,
			ogzfld::dRows &RawFields ) = 0;
		// Records related methods.
		virtual ogzbsc::sRRow OGZUSRAdd(
			ogzrcd::sRow Record,
			sRow User ) = 0;
		virtual void OGZUSRRemove(
			ogzbsc::sRRow Record,
			sRow User ) = 0;
		virtual void OGZUSRGetRaws(
			const dRecords &Records,
			sRow User,
			ogzrcd::dRows &RawRecords ) = 0;
		virtual sAmount OGZUSRGetRecordsSet(
			sRow User,
			sIndice Indice,
			sAmount Amount,
			dSet &Set ) = 0;
	public:
		qCALLBACK( User );
		void Wipe( void )
		{
			OGZUSRDelete( qNIL );
		}
		sRow New( sRow User )
		{
			return OGZUSRNew( User );
		}
		void Delete( sRow Row )
		{
			return OGZUSRDelete( Row );
		}
		template <typename regular, typename raw> regular Add(
			raw Raw,
			sRow User )
		{
			return OGZUSRAdd( Raw, User );
		}
		template <typename regular> void Remove(
			regular Regular,
			sRow User )
		{
			return OGZUSRRemove( Regular, User );
		}
		template <typename regulars, typename raws> void GetRaws(
			const regulars &Regulars,
			sRow User,
			raws &Raws )
		{
			return OGZUSRGetRaws( Regulars, User, Raws );
		}
		sAmount GetRecordsSet(
			sRow User,
			sIndice Indice,
			sAmount Amount,
			dSet &Set )
		{
			return OGZUSRGetRecordsSet( User, Indice, Amount, Set );
		}
	};

	typedef ogzbsc::rLock rLock_;

	class mUsers
	: public rLock_
	{
	private:
		qRMV( cUser, C_, Callback_ );
		sRow New_( sRow Row ) const
		{
		qRH
		qRB
			Lock_();
			Row = C_().New( Row );
		qRR
		qRT
			Unlock_();
		qRE
			return Row;
		}
		void Delete_( sRow Row ) const
		{
		qRH
		qRB
			Lock_();
			C_().Delete( Row );
		qRR
		qRT
			Unlock_();
		qRE
		}
		template <typename regular, typename raw> regular Add_(
			raw Raw,
			sRow User ) const
		{
			regular Row = qNIL;
		qRH
		qRB
			Lock_();
			Row = C_().Add<regular,raw>( Raw, User );
		qRR
		qRT
			Unlock_();
		qRE
			return Row;
		}
		template <typename regular> void Remove_(
			regular Regular,
			sRow User ) const
		{
		qRH
		qRB
			Lock_();
			C_().Remove( Regular, User );
		qRR
		qRT
			Unlock_();
		qRE
		}
		template <typename regulars, typename raws> void GetRaws_(
			const regulars &Regulars,
			sRow User,
			raws &Raws ) const
		{
		qRH
		qRB
			Lock_();
			C_().GetRaws( Regulars, User, Raws );
		qRR
		qRT
			Unlock_();
		qRE
		}
		template <typename regular, typename raw> raw GetRaw_(
			regular Regular,
			sRow User ) const
		{
			raw Raw = qNIL;
		qRH
			ogzbsc::wList<regular> Regulars;
			bch::qBUNCHwl( raw ) Raws;
		qRB
			Regulars.Init();
			Regulars.Append( Regular );

			Raws.Init();

			GetRaws( Regulars, User, Raws );

			Raw = Raws( Raws.First() );
		qRR
		qRT
		qRE
			return Raw;
		}
		sAmount GetRecordsSet_(
			sRow User,
			sIndice Indice,
			sAmount Amount,
			dSet &Set ) const
		{
		qRH
		qRB
			Lock_();
			Amount = C_().GetRecordsSet( User, Indice, Amount, Set );
		qRR
		qRT
			Unlock_();
		qRE
			return Amount;
		}
	public:
		void reset( bso::bool__ P = true )
		{
			Callback_ = NULL;
			rLock_::reset( P );
		}
		E_CDTOR( mUsers );
		void Init( cUser &Callback )
		{
			Callback_ = &Callback;
			rLock_::Init();
			New();
		}
		void Wipe( void ) const
		{
			return Delete_( qNIL );
		}
		sRow New( sRow Row = qNIL ) const
		{
			return New_( Row );
		}
		void Delete( sRow Row ) const
		{
			return Delete_( Row );
		}
		template <typename regular> void Remove(
			regular Regular,
			sRow User ) const
		{
			return Remove_( Regular, User );
		}
		template <typename regulars, typename raws> void GetRaws(
			regulars &Regulars,
			sRow User,
			raws &Raws ) const
		{
			return GetRaws_( Regulars, User, Raws );
		}
		// No template members below, because some template can not be deduced.
		ogzbsc::sMRow Add(
			ogzmta::sRow Meta,
			sRow User ) const
		{
			return Add_<ogzbsc::sMRow,ogzmta::sRow>( Meta, User );
		}
		ogzmta::sRow GetRaw(
			ogzbsc::sMRow Meta,
			sRow User ) const
		{
			return GetRaw_<ogzbsc::sMRow,ogzmta::sRow>( Meta, User );
		}
		ogzbsc::sCRow Add(
			ogzclm::sRow Column,
			sRow User ) const
		{
			return Add_<ogzbsc::sCRow,ogzclm::sRow>( Column, User );
		}
		ogzclm::sRow GetRaw(
			ogzbsc::sCRow Column,
			sRow User ) const
		{
			return GetRaw_<ogzbsc::sCRow,ogzclm::sRow>( Column, User );
		}
		ogzbsc::sERow Add(
			ogzetr::sRow Entry,
			sRow User ) const
		{
			return Add_<ogzbsc::sERow,ogzetr::sRow>( Entry, User );
		}
		ogzetr::sRow GetRaw(
			ogzbsc::sERow Entry,
			sRow User ) const
		{
			return GetRaw_<ogzbsc::sERow,ogzetr::sRow>( Entry, User );
		}
		ogzbsc::sFRow Add(
			ogzfld::sRow Field,
			sRow User ) const
		{
			return Add_<ogzbsc::sFRow,ogzfld::sRow>( Field, User );
		}
		ogzfld::sRow GetRaw(
			ogzbsc::sFRow Field,
			sRow User ) const
		{
			return GetRaw_<ogzbsc::sFRow,ogzfld::sRow>( Field, User );
		}
		ogzbsc::sRRow Add(
			ogzrcd::sRow Record,
			sRow User ) const
		{
			return Add_<ogzbsc::sRRow,ogzrcd::sRow>( Record, User );
		}
		ogzrcd::sRow GetRaw(
			ogzbsc::sRRow Record,
			sRow User ) const
		{
			return GetRaw_<ogzbsc::sRRow,ogzrcd::sRow>( Record, User );
		}
		sAmount GetRecordsSet(
			sRow User,
			sIndice Indice,
			sAmount Amount,
			dSet &Set ) const
		{
			return GetRecordsSet_( User, Indice, Amount, Set );
		}
	};

	typedef lstbch::qBUNCHd( ogzmta::sRow, ogzbsc::sMRow ) dMeta_;
	typedef lstcrt::qLMCRATEd( dMeta_, sRow ) dMetasCrate_;
	qW( MetasCrate_ );

	typedef lstbch::qBUNCHd( ogzclm::sRow, ogzbsc::sCRow ) dColumns_;
	typedef lstcrt::qLMCRATEd( dColumns_, sRow ) dColumnsCrate_;
	qW( ColumnsCrate_ );

	typedef lstbch::qBUNCHd( ogzetr::sRow, ogzbsc::sERow ) dEntries_;
	typedef lstcrt::qLMCRATEd( dEntries_, sRow ) dEntriesCrate_;
	qW( EntriesCrate_ );

	typedef lstbch::qBUNCHd( ogzfld::sRow, ogzbsc::sFRow ) dFields_;
	typedef lstcrt::qLMCRATEd( dFields_, sRow ) dFieldsCrate_;
	qW( FieldsCrate_ );

	typedef lstbch::qBUNCHd( ogzrcd::sRow, ogzbsc::sRRow ) dRecords_;
	typedef lstcrt::qLMCRATEd( dRecords_, sRow ) dRecordsCrate_;
	qW( RecordsCrate_ );


	class rRegularCallback
	: public cUser
	{
	private:
		wMetasCrate_ Metas_;
		wColumnsCrate_ Columns_;
		wEntriesCrate_ Entries_;
		wFieldsCrate_ Fields_;
		wRecordsCrate_ Records_;
		template <typename crate, typename regular, typename raw> regular Add_(
			crate &Crate,
			raw Raw,
			sRow User )
		{
			return Crate( User ).Add( Raw );
		}
		template <typename crate, typename regular> void Remove_(
			crate &Crate,
			regular Regular,
			sRow User )
		{
			if ( Regular == qNIL )
				Crate( User ).Init();
			else
				Crate( User).Remove( Regular );
		}
		template <typename crate, typename regular, typename raws> void GetRaws_(
			const ogzbsc::dList<regular> &Regulars,
			const crate &Crate,
			sRow User,
			raws &Raws )
		{
			sdr::sRow Row = Regulars.First();

			while ( Row != qNIL ) {
				Raws.Append( Crate( User )( Regulars( Row ) ) );

				Row = Regulars.Next( Row );
			}
		}
		sAmount GetRecordsSet_(
			const dRecords_ &Records,
			sIndice Indice,
			sAmount Amount,
			dSet &Set ) const;
	protected:
		// If 'Row' != 'qNIL', it must be used.
		sRow OGZUSRNew( sRow User ) override
		{
			sRow Row = Metas_.New();

			if ( Columns_.New() != Row )
				qRGnr();

			if ( Entries_.New() != Row )
				qRGnr();

			if ( Fields_.New() != Row )
				qRGnr();

			if ( Records_.New() != Row )
				qRGnr();

			Metas_( Row ).Init();
			Columns_( Row ).Init();
			Fields_( Row ).Init();
			Entries_( Row ).Init();
			Records_( Row ).Init();

			return Row;
		}
		// If 'Row' == 'qNIL', the content must be erased.
		virtual void OGZUSRDelete( sRow User ) override
		{
			if ( User == qNIL ) {
				Metas_.Init();
				Columns_.Init();
				Entries_.Init();
				Fields_.Init();
				Records_.Init();
			} else {
				Metas_.Remove( User );
				Columns_.Remove( User );
				Entries_.Remove( User );
				Fields_.Remove( User );
				Records_.Remove( User );
			}
		}
		virtual ogzbsc::sMRow OGZUSRAdd(
			ogzmta::sRow Meta,
			sRow User ) override
		{
			return Add_<dMetasCrate_, ogzbsc::sMRow, ogzmta::sRow> ( Metas_, Meta, User );
		}
		virtual void OGZUSRRemove(
			ogzbsc::sMRow Meta,
			sRow User ) override
		{
			return Remove_( Metas_, Meta, User );
		}
		virtual void OGZUSRGetRaws(
			const ogzclm::dMetas &Metas,
			sRow User,
			ogzmta::dRows &RawMetas ) override
		{
			return GetRaws_( Metas, Metas_, User, RawMetas );
		}
		virtual ogzbsc::sCRow OGZUSRAdd(
			ogzclm::sRow Column,
			sRow User ) override
		{
			return Add_<dColumnsCrate_, ogzbsc::sCRow, ogzclm::sRow> ( Columns_, Column, User );
		}
		virtual void OGZUSRRemove(
			ogzbsc::sCRow Column,
			sRow User ) override
		{
			return Remove_( Columns_, Column, User );
		}
		virtual void OGZUSRGetRaws(
			const ogzfld::dColumns &Columns,
			sRow User,
			ogzclm::dRows &RawColumns ) override
		{
			return GetRaws_( Columns, Columns_, User, RawColumns );
		}
		virtual ogzbsc::sERow OGZUSRAdd(
			ogzetr::sRow Entry,
			sRow User ) override
		{
			return Add_<dEntriesCrate_, ogzbsc::sERow, ogzetr::sRow> ( Entries_, Entry, User );
		}
		virtual void OGZUSRRemove(
			ogzbsc::sERow Entry,
			sRow User ) override
		{
			return Remove_( Entries_, Entry, User );
		}
		virtual void OGZUSRGetRaws(
			const ogzfld::dEntries &Entries,
			sRow User,
			ogzetr::dRows &RawEntries ) override
		{
			return GetRaws_( Entries, Entries_, User, RawEntries );
		}
		virtual ogzbsc::sFRow OGZUSRAdd(
			ogzfld::sRow Field,
			sRow User ) override
		{
			return Add_<dFieldsCrate_, ogzbsc::sFRow, ogzfld::sRow> ( Fields_, Field, User );
		}
		virtual void OGZUSRRemove(
			ogzbsc::sFRow Field,
			sRow User ) override
		{
			return Remove_( Fields_, Field, User );
		}
		virtual void OGZUSRGetRaws(
			const ogzrcd::dFields &Fields,
			sRow User,
			ogzfld::dRows &RawFields ) override
		{
			return GetRaws_( Fields, Fields_, User, RawFields );
		}
		virtual ogzbsc::sRRow OGZUSRAdd(
			ogzrcd::sRow Record,
			sRow User ) override
		{
			return Records_( User ).Add( Record );
		}
		virtual void OGZUSRRemove(
			ogzbsc::sRRow Record,
			sRow User ) override
		{
			Records_( User ).Remove( Record );
		}
		virtual void OGZUSRGetRaws(
			const dRecords &Records,
			sRow User,
			ogzrcd::dRows &RawRecords ) override
		{
			return GetRaws_( Records, Records_, User, RawRecords );
		}
		virtual sAmount OGZUSRGetRecordsSet(
			sRow User,
			sIndice Indice,
			sAmount Amount,
			dSet &Set ) override
		{
			return GetRecordsSet_( Records_( User ), Indice, Amount, Set );
		}
	public:
		void reset( bso::sBool P = true )
		{
			Metas_.reset( P );
			Columns_.reset( P );
			Entries_.reset( P );
			Fields_.reset( P );
			Records_.reset( P );
		}
		qCVDTOR( rRegularCallback );
		void Init( void )
		{
			Metas_.Init();
			Columns_.Init();
			Entries_.Init();
			Fields_.Init();
			Records_.Init();
		}
	};

	class cAuthentication
	{
	protected:
		virtual ogzusr::sRow OGZUSRAuthenticate(
			const str::dString &Username,
			const str::dString &Password ) = 0;
	public:
		qCALLBACK( Authentication );
		ogzusr::sRow Authenticate(
			const str::dString &Username,
			const str::dString &Password )
		{
			return OGZUSRAuthenticate( Username, Password );
		}
	};

	class mAuthentication
	: public rLock_
	{
	private:
		qRMV( cAuthentication, C_, Callback_ );
		ogzusr::sRow Authenticate_(
			const str::dString &Username,
			const str::dString &Password ) const
		{
			ogzusr::sRow Row = qNIL;
		qRH
		qRB
			Lock_();
			Row = C_().Authenticate( Username, Password );
		qRR
		qRT
			Unlock_();
		qRE
			return Row;
		}
	public:
		void reset( bso::sBool P = true )
		{
			Callback_ = NULL;
			rLock_::reset( P );
		}
		qCDTOR( mAuthentication );
		void Init( cAuthentication &Callback )
		{
			Callback_ = &Callback;
			rLock_::Init();
		}
		ogzusr::sRow Authenticate(
			const str::dString &Username,
			const str::dString &Password ) const
		{
			return Authenticate_( Username, Password );
		}
	};
}

#endif
