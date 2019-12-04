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

// OrGaniZer EnTRies

#ifndef OGZETR_INC_
# define OGZETR_INC_

# ifdef XXX_DBG
#	define OGZETR__DBG
# endif

# include "ogzbsc.h"
# include "ogztyp.h"

# include "bch.h"
# include "lstctn.h"

namespace ogzetr {
	using ogzbsc::dDatum;
	using ogzbsc::wDatum;

	using crt::sHooks;

	qROW( Row );
	qROWS( Row );

	class cEntry {
	protected:
		// If 'Row' != 'qNIL', it must be used.
		virtual sRow OGZETRNew(
			ogztyp::sRow Type,	// If == 'qNIL', it's the mandatory text type.
			sRow Row ) = 0;
		// if 'Row' == 'qNIL', the entire content must be erased.
		virtual void OGZETRDelete( sRow Row ) = 0;
		virtual void OGZETRStore(
			const dDatum &Datum,
			ogztyp::sRow Type,	// If == 'qNIL', it's the mnadatory text type.
			sRow Row )= 0;
		virtual void OGZETRRecall(
			sRow Row,
			ogztyp::sRow Type,	// If == 'qNIL', it's the mnadatory text type.
			dDatum &Datum ) = 0;
	public:
		qCALLBACK( Entry );
		void Wipe( void )
		{
			OGZETRDelete( qNIL );
		}
		sRow New(
			ogztyp::sRow Type,	// If == 'qNIL', it's the mandatory text type.
			sRow Row = qNIL )
		{
			return OGZETRNew( Type, Row );
		}
		void Delete( sRow Row )
		{
			OGZETRDelete( Row );
		}
		void Store(
			sRow Row,
			ogztyp::sRow Type,
			const dDatum &Datum )
		{
			OGZETRStore( Datum, Type, Row );
		}
		void Recall(
			sRow Row,
			ogztyp::sRow Type,
			dDatum &Datum )
		{
			return OGZETRRecall( Row, Type, Datum );
		}
	};

	typedef ogzbsc::rLock rLock_;

	class mEntries
	: public rLock_
	{
	private:
		qRMV( cEntry, C_, Callback_ );
		sRow New_(
			ogztyp::sRow Type,	// If == 'qNIL', it's the mandatory text type.
			sRow Row = qNIL ) const
		{
		qRH
		qRB
			Lock_();
			Row = C_().New( Type, Row );
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
		void Store_(
			const dDatum &Datum,
			ogztyp::sRow Type,	// If == 'qNIL', it's the mnadatory text type.
			sRow Row ) const
		{
		qRH
		qRB
			Lock_();
			C_().Store( Row, Type, Datum );
		qRR
		qRT
			Unlock_();
		qRE
		}
		void Recall_(
			sRow Row,
			ogztyp::sRow Type,
			dDatum &Datum ) const
		{
			if ( Row != qNIL ) {
			qRH
			qRB
				Lock_();
				C_().Recall( Row, Type, Datum );
			qRR
			qRT
				Unlock_();
			qRE
			}
		}
	public:
		void reset( bso::bool__ P = true )
		{
			Callback_ = NULL;
			rLock_::reset( P );
		}
		qCDTOR( mEntries );
		void Init( cEntry &Callback )
		{
			Callback_ = &Callback;
			rLock_::Init();
		}
		void Wipe( void ) const
		{
			return Delete_( qNIL );
		}
		sRow New(
			ogztyp::sRow Type,	// If == 'qNIL', it's the mandatory text type.
			sRow Row = qNIL ) const
		{
			return New_( Type, Row );
		}
		void Delete( sRow Row ) const
		{
			return Delete_( Row );
		}
		void Delete( const dRows &Rows )const
		{
			sdr::sRow Row = Rows.First();

			while ( Row != qNIL ) {
				Delete(Rows( Row ) );

				Row = Rows.Next( Row );
			}
		}
		void Store(
			const dDatum &Datum,
			ogztyp::sRow Type,	// If == 'qNIL', it's the mandatory text type.
			sRow Row ) const
		{
			return Store_( Datum, Type, Row );
		}
		void Recall(
			sRow Row,
			ogztyp::sRow Type,
			dDatum &Datum ) const
		{
			return Recall_( Row, Type, Datum );
		}
	};

	typedef str::dString dBytes;
	qW( Bytes );

	class rRegularCallback
	: public cEntry
	{
	private:
		lstctn::qLMCONTAINERw( dBytes, sRow ) Container_;
	protected:
		virtual sRow OGZETRNew(
			ogztyp::sRow Type,
			sRow Row ) override 
		{
			Row = Container_.New( Row );

			Container_(Row).Init();
			Container_.Flush();

			return Row;
		}
		virtual void OGZETRDelete( sRow Row ) override 
		{
			if ( Row == qNIL )
				Container_.reset();
			else
				Container_.Remove( Row );
		}
		virtual void OGZETRStore(
			const dDatum &Datum,
			ogztyp::sRow Type,
			sRow Row ) override 
		{
			Container_.Store( Datum, Row );
		}
		virtual void OGZETRRecall(
			sRow Row,
			ogztyp::sRow Type,
			dDatum &Datum ) override 
		{
			Container_.Recall( Row, Datum );
		}
	public:
		void reset( bso::bool__ P = true )
		{
			Container_.reset( P );
		}
		qCVDTOR( rRegularCallback );
		void Init( void )
		{
			Container_.Init();
		}
	};

	using bch::sHook;
}


#endif
