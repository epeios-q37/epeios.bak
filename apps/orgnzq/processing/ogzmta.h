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
# include "lstcrt.h"

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
		virtual sRow OGZMTASearch(
			const str::dString &Pattern,
			eTarget Target,
			sRow First ) = 0;	// Excluded. If == 'qNil', searcg from the first one.
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
		sRow Search(
			const str::dString &Pattern,
			eTarget Target,
			sRow First = qNIL )
		{
			return OGZMTASearch( Pattern, Target, First );
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
		sRow Search_(
			const str::dString &Pattern,
			eTarget Target,
			sRow First ) const
		{
			sRow &Row = First;
		qRH
		qRB
			Lock_();
			Row = C_().Search( Pattern, Target, First );
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
		sRow Search(
			const str::dString &Pattern,
			eTarget Target ) const
		{
			return Search_( Pattern, Target, qNIL );
		}
		template <typename ... t> sdr::sRow Search(
			const str::dString &Pattern,
			eTarget First,
			t... Others ) const
		{
			sdr::sRow Row = Search( Pattern, First );

			if ( sRow == qNIL )
				Row = Search( Pattern, Others );

			return Row;
		}
	};

	typedef str::dString dBytes;
	qW( Bytes );

	class rRegularCallback
	: public cMeta
	{
	private:
		lstcrt::qLMCRATEw( dBytes, sRow ) Data_;
		lstbch::qLBUNCHw( eTarget, sRow ) Targets_;
	protected:
		virtual sRow OGZMTANew(	sRow Row ) override 
		{
			Row = Data_.New( Row );

			if ( Targets_.New() != Row )
				qRGnr();

			Data_(Row).Init();
			Data_.Flush();

			return Row;
		}
		virtual void OGZMTADelete( sRow Row ) override 
		{
			if ( Row == qNIL ) {
				tol::reset( Data_, Targets_ );
			} else {
				Data_.Remove( Row );
				Targets_.Remove( Row );
			}
		}
		virtual void OGZMTAStore(
			const dDatum &Datum,
			eTarget Target,
			sRow Row ) override 
		{
			Data_.Store( Datum, Row );
			Targets_.Store( Target, Row );
		}
		virtual void OGZMTARecall(
			sRow Row,
			dDatum &Datum ) override 
		{
			Data_.Recall( Row, Datum );
		}
		virtual sRow OGZMTASearch(
			const str::dString &Pattern,
			eTarget Target,
			sRow First ) override
		{
			sRow &Row = First;

			if ( Row == qNIL )
				Row = Data_.First();
			else
				Row = Data_.Next( Row );

			while ( ( Row != qNIL )
				    && ( ( Targets_( Row ) != Target )
					     || ( Data_( Row ) != Pattern ) ) )
					Row = Data_.Next( Row );

			return Row;

		}
	public:
		void reset( bso::bool__ P = true )
		{
			tol::reset( Data_, Targets_ );
		}
		qCVDTOR( rRegularCallback );
		void Init( void )
		{
			tol::Init( Data_, Targets_ );
		}
	};

	using bch::cHook;
}


#endif
