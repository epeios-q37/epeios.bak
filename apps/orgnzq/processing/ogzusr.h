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
# include "ogzfld.h"
# include "ogzrcd.h"

# include "lstctn.h"

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
		virtual ogzbsc::sDRow OGZUSRAdd(
			ogzdta::sRow Entry,
			ogzbsc::sFRow Field,
			sRow User ) = 0;
		virtual void OGZUSRRemove(
			ogzbsc::sDRow Entry,
			sRow User ) = 0;
		virtual void OGZUSRGetRaws(
			const ogzrcd::dFields &Fields,
			sRow User,
			ogzfld::dRows &RawFields ) = 0;
		virtual ogzbsc::sFRow OGZUSRAdd(
			ogzfld::sRow Field,
			ogzbsc::sRRow Record,
			sRow User ) = 0;
		virtual void OGZUSRRemove(
			ogzbsc::sFRow Entry,
			sRow User ) = 0;
		virtual void OGZUSRGetRaws(
			const dRecords &Records,
			sRow User,
			ogzrcd::dRows &RawRecords ) = 0;
		virtual ogzbsc::sRRow OGZUSRAdd(
			ogzrcd::sRow Record,
			sRow User ) = 0;
		// Dummy method which simplifies the use of template methods.
		ogzbsc::sRRow OGZUSRAdd(
			ogzrcd::sRow Record,
			sdr::sRow,	// Dummy argument.
			sRow User )
		{
			return OGZUSRAdd( Record, User );
		}
		virtual void OGZUSRRemove(
			ogzbsc::sRRow Record,
			sRow User ) = 0;
		virtual void OGZUSRGetRaws(
			const ogzfld::dData &Data,
			sRow User,
			ogzdta::dRows &RawData ) = 0;
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
		template <typename regular, typename raw, typename owner> regular Add(
			raw Raw,
			owner Owner,
			sRow User )
		{
			return OGZUSRAdd( Raw, Owner, User );
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
		template <typename regular, typename raw, typename owner> regular Add_(
			raw Raw,
			owner Owner,
			sRow User ) const
		{
			regular Row = qNIL;
		qRH
		qRB
			Lock_();
			Row = C_().Add<regular,raw,owner>( Raw, Owner, User );
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
			regular &Regular,
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
			regular Record,
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
		ogzbsc::sDRow Add(
			ogzdta::sRow Datum,
			ogzbsc::sFRow Field,
			sRow User ) const
		{
			return Add_<ogzbsc::sDRow,ogzdta::sRow,ogzbsc::sFRow>( Datum, Field, User );
		}
		ogzdta::sRow GetRaw(
			ogzbsc::sDRow Datum,
			sRow User ) const
		{
			return GetRaw_<ogzbsc::sDRow,ogzdta::sRow>( Datum, User );
		}
		ogzbsc::sFRow Add(
			ogzfld::sRow Field,
			ogzbsc::sRRow Record,
			sRow User ) const
		{
			return Add_<ogzbsc::sFRow,ogzfld::sRow,ogzbsc::sRRow>( Field, Record, User );
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
			return Add_<ogzbsc::sRRow,ogzrcd::sRow,sdr::sRow>( Record, qNIL, User );
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

	typedef lstbch::qBUNCHd( ogzrcd::sRow, ogzbsc::sRRow ) dRecords_;

	class rRegularCallback
	: public cUser
	{
	private:
		lstctn::qLCONTAINERw( dRecords_, sRow ) Core_;
		sAmount GetRecordsSet_(
			const dRecords_ &Records,
			sIndice Indice,
			sAmount Amount,
			dSet &Set ) const;
	protected:
		// If 'Row' != 'qNIL', it must be used.
		sRow OGZUSRNew( sRow User ) override
		{
			return Core_.New();
		}
		// If 'Row' == 'qNIL', the content must be erased.
		virtual void OGZUSRDelete( sRow User ) override
		{
			if ( User == qNIL )
				Core_.Init();
			else
				Core_.Remove( User );
		}
		virtual ogzbsc::sRRow OGZUSRAdd(
			ogzrcd::sRow Record,
			sRow User ) override
		{
			return Core_( User ).Add( Record );
		}
		virtual void OGZUSRRemove(
			ogzbsc::sRRow Record,
			sRow User ) override
		{
			Core_( User ).Remove( Record );
		}
		virtual sAmount OGZUSRGetRecordsSet(
			sRow User,
			sIndice Indice,
			sAmount Amount,
			dSet &Set ) override
		{
			return GetRecordsSet_( Core_( User ), Indice, Amount, Set );
		}
	public:
		void reset( bso::sBool P = true )
		{
			Core_.reset( P );
		}
		qCVDTOR( rRegularCallback );
		void Init( void )
		{
			Core_.Init();
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
