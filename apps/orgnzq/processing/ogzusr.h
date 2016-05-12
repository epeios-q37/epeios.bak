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

	class sUsers
	{
	private:
		qRMV( cUser, C_, Callback_ );
	public:
		void reset( bso::bool__ P = true )
		{
			Callback_ = NULL;
		}
		E_CDTOR( sUsers );
		void Init( cUser &Callback )
		{
			Callback_ = &Callback;
		}
		void Wipe( void ) const
		{
			C_().Wipe();
		}
		sRow New( sRow Row = qNIL ) const
		{
			return C_().New( Row );
		}
		void Delete( sRow Row ) const
		{
			return C_().Delete( Row );
		}
		sRRow Add(
			ogzrcd::sRow Record,
			sRow User )
		{
			return C_().Add( Record, User );
		}
		void Remove(
			sRow User,
			sRRow Record )
		{
			return C_().Remove( User, Record );
		}
		sAmount GetSet(
			sRow User,
			sIndice Indice,
			sAmount Amount,
			dSet &Set )
		{
			return C_().GetSet( User, Indice, Amount, Set );
		}
		ogzrcd::sRow GetRawRecordRow(
			sRow User,
			sRRow Record ) const
		{
			return C_().GetRawRecordRow( User, Record );
		}
	};

	class rRegularCallback
	: public cUser
	{
	private:
		lstctn::qLCONTAINERw( lstbch::qLBUNCHd( ogzrcd::sRow, sRRow ), sRow ) Core_;
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
			qRVct();
			return  0;
		}
		virtual ogzrcd::sRow OGZUSRGetRawRecordRow(
			sRow User,
			sRRow Record ) override
		{
			qRVct();
			return qNIL;
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

}


#endif
