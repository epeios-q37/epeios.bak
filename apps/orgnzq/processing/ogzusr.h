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
# include "ogzrcd.h"

# include "lstctn.h"

namespace ogzusr {
	typedef bso::sUInt sIndice;	// Begins at '0'.

	typedef bso::sUInt sAmount;	// If == '0', return all available.
	qCDEF( sIndice, AmountMax, BSO_UINT_MAX );

	qROW( Row );
	qROW( RRow );
	qROW( SRow );

	typedef bch::qBUNCHd( sRRow, sSRow ) dSet;
	qW( Set );

	class cUser
	{
	protected:
		// If 'Row' != 'qNIL', it must be used.
		virtual sRow OGZUSRNew( sRow User ) = 0;
		// If 'Row' == 'qNIL', the content must be erased.
		virtual void OGZUSRDelete( sRow User ) = 0;
		virtual sRRow OGZUSRAdd(
			ogzrcd::sRow Record,
			sRow User ) = 0;
		virtual void OGZUSRRemove(
			sRow User,
			sRRow Record ) = 0;
		virtual sAmount OGZUSRGetSet(
			sRow User,
			sIndice Indice,
			sAmount Amount,
			dSet &Set ) = 0;
		virtual ogzrcd::sRow OGZUSRGetRawRecordRow(
			sRow User,
			sRRow Record ) = 0;
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
		sRRow Add(
			ogzrcd::sRow Record,
			sRow User )
		{
			return OGZUSRAdd( Record, User );
		}
		void Remove(
			sRow User,
			sRRow Record )
		{
			return OGZUSRRemove( User, Record );
		}
		sAmount GetSet(
			sRow User,
			sIndice Indice,
			sAmount Amount,
			dSet &Set )
		{
			return OGZUSRGetSet( User, Indice, Amount, Set );
		}
		ogzrcd::sRow GetRawRecordRow(
			sRow User,
			sRRow Record )
		{
			return OGZUSRGetRawRecordRow( User, Record );
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
		sRRow Add_(
			ogzrcd::sRow Record,
			sRow User )
		{
			sRRow Row = qNIL;
		qRH
		qRB
			Lock_();
			Row = C_().Add( Record, User );
		qRR
		qRT
			Unlock_();
		qRE
			return Row;
		}
		void Remove_(
			sRow User,
			sRRow Record )
		{
		qRH
		qRB
			Lock_();
			C_().Remove( User, Record );
		qRR
		qRT
			Unlock_();
		qRE
		}
		sAmount GetSet_(
			sRow User,
			sIndice Indice,
			sAmount Amount,
			dSet &Set )
		{
		qRH
		qRB
			Lock_();
			Amount = C_().GetSet( User, Indice, Amount, Set );
		qRR
		qRT
			Unlock_();
		qRE
			return Amount;
		}
		ogzrcd::sRow GetRawRecordRow_(
			sRow User,
			sRRow Record ) const
		{
			ogzrcd::sRow Row = qNIL;
		qRH
		qRB
			Lock_();
			Row = C_().GetRawRecordRow( User, Record );
		qRR
		qRT
			Unlock_();
		qRE
			return Row;
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
		sRRow Add(
			ogzrcd::sRow Record,
			sRow User )
		{
			return Add_( Record, User );
		}
		void Remove(
			sRow User,
			sRRow Record )
		{
			return Remove_( User, Record );
		}
		sAmount GetSet(
			sRow User,
			sIndice Indice,
			sAmount Amount,
			dSet &Set )
		{
			return GetSet_( User, Indice, Amount, Set );
		}
		ogzrcd::sRow GetRawRecordRow(
			sRow User,
			sRRow Record ) const
		{
			return GetRawRecordRow_( User, Record );
		}
	};

	typedef ogzbsc::dList<ogzrcd::sRow, sRRow> dRecords;

	class rRegularCallback
	: public cUser
	{
	private:
		lstctn::qLCONTAINERw( dRecords, sRow ) Core_;
		sAmount GetSet_(
			const dRecords &Records,
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
		virtual sRRow OGZUSRAdd(
			ogzrcd::sRow Record,
			sRow User ) override
		{
			return Core_( User ).Add( Record );
		}
		virtual void OGZUSRRemove(
			sRow User,
			sRRow Record ) override
		{
			Core_( User ).Remove( Record );
		}
		virtual sAmount OGZUSRGetSet(
			sRow User,
			sIndice Indice,
			sAmount Amount,
			dSet &Set ) override
		{
			return GetSet_( Core_( User ), Indice, Amount, Set );
		}
		virtual ogzrcd::sRow OGZUSRGetRawRecordRow(
			sRow User,
			sRRow Record ) override
		{
			return Core_(User).Get( Record );
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
	};
}

#endif
