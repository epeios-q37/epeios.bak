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

// OrGaniZer MeTAdata

#ifndef OGZMTA__INC
# define OGZMTA__INC

# ifdef XXX_DBG
#	define OGZMTA__DBG
# endif

# include "ogzbsc.h"
# include "ogztyp.h"

# include "bch.h"
# include "lstctn.h"

namespace ogzmta {
	using ogzbsc::dDatum;
	using ogzbsc::wDatum;

	using crt::cHooks;

	qROW( Row );
	qROWS( Row );

	qENUM( Target ) {
		tColumnLabel,
		tColumnComment,
		t_amount,
		t_Undefined
	};

	qFLAG( ColumnLabel, tColumnLabel );

	class cMeta {
	protected:
		// If 'Row' != 'qNIL', it must be used.
		virtual sRow OGZMTANew(	sRow Row ) = 0;
		// if 'Row' == 'qNIL', the entire content must be erased.
		virtual void OGZMTADelete( sRow Row ) = 0;
		virtual void OGZMTAStore(
			const dDatum &Datum,
			eTarget Target,
			sRow Row )= 0;
		virtual void OGZMTARecall(
			sRow Row,
			dDatum &Datum ) = 0;
	public:
		qCALLBACK( Meta );
		void Wipe( void )
		{
			OGZMTADelete( qNIL );
		}
		sRow New( sRow Row = qNIL )
		{
			return OGZMTANew( Row );
		}
		void Delete( sRow Row )
		{
			OGZMTADelete( Row );
		}
		void Store(
			sRow Row,
			eTarget Target,
			const dDatum &Datum )
		{
			OGZMTAStore( Datum, Target, Row );
		}
		void Recall(
			sRow Row,
			dDatum &Datum )
		{
			return OGZMTARecall( Row, Datum );
		}
	};

	typedef ogzbsc::rLock rLock_;

	class mMetas
	: public rLock_
	{
	private:
		qRMV( cMeta, C_, Callback_ );
		sRow New_( sRow Row = qNIL ) const
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
		void Store_(
			const dDatum &Datum,
			eTarget Target,
			sRow Row ) const
		{
		qRH
		qRB
			Lock_();
			C_().Store( Row, Target, Datum );
		qRR
		qRT
			Unlock_();
		qRE
		}
		void Recall_(
			sRow Row,
			dDatum &Datum ) const
		{
			if ( Row != qNIL ) {
			qRH
			qRB
				Lock_();
				C_().Recall( Row, Datum );
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
		void Init( cMeta &Callback )
		{
			Callback_ = &Callback;
			rLock_::Init();
		}
		void Wipe( void ) const
		{
			return Delete_( qNIL );
		}
		sRow New( sRow Row = qNIL )
		{
			return New_( Row );
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
			eTarget Target,
			sRow Row ) const
		{
			return Store_( Datum, Target, Row );
		}
		void Recall(
			sRow Row,
			dDatum &Datum ) const
		{
			return Recall_( Row, Datum );
		}
	};

	typedef str::dString dBytes;
	qW( Bytes );

	class rRegularCallback
	: public cMeta
	{
	private:
		lstctn::qLMCONTAINERw( dBytes, sRow ) Container_;
	protected:
		virtual sRow OGZMTANew(	sRow Row ) override 
		{
			Row = Container_.New( Row );

			Container_(Row).Init();
			Container_.Flush();

			return Row;
		}
		virtual void OGZMTADelete( sRow Row ) override 
		{
			if ( Row == qNIL )
				Container_.reset();
			else
				Container_.Remove( Row );
		}
		virtual void OGZMTAStore(
			const dDatum &Datum,
			eTarget Target,
			sRow Row ) override 
		{
			Container_.Store( Datum, Row );
		}
		virtual void OGZMTARecall(
			sRow Row,
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

	using bch::cHook;
}


#endif
