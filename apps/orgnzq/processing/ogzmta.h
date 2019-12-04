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

#ifndef OGZMTA_INC_
# define OGZMTA_INC_

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

	using crt::sHooks;

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
		virtual void OGZMTAErase( sRow Row ) = 0;
		virtual void OGZMTAStore(
			ogzbsc::sURow User,
			const dDatum &Datum,
			eTarget Target,
			sRow Row )= 0;
		virtual void OGZMTARecall(
			sRow Row,
			dDatum &Datum ) = 0;
		virtual sRow OGZMTASearch(
			ogzbsc::sURow User,
			eTarget Target,
			const str::dString &Pattern,
			sRow First ) = 0;	// Excluded. If == 'qNil', search from the first one.
	public:
		qCALLBACK( Meta );
		void Wipe( void )
		{
			OGZMTAErase( qNIL );
		}
		sRow New( sRow Row = qNIL )
		{
			return OGZMTANew( Row );
		}
		void Delete( sRow Row )
		{
			OGZMTAErase( Row );
		}
		void Store(
			ogzbsc::sURow User,
			sRow Row,
			eTarget Target,
			const dDatum &Datum )
		{
			OGZMTAStore( User, Datum, Target, Row );
		}
		void Recall(
			sRow Row,
			dDatum &Datum )
		{
			return OGZMTARecall( Row, Datum );
		}
		sRow Search(
			ogzbsc::sURow User,
			eTarget Target,
			const str::dString &Pattern,
			sRow First = qNIL )
		{
			return OGZMTASearch( User, Target, Pattern, First );
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
			ogzbsc::sURow USer,
			const dDatum &Datum,
			eTarget Target,
			sRow Row ) const
		{
		qRH
		qRB
			Lock_();
			C_().Store( USer, Row, Target, Datum );
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
			ogzbsc::sURow User,
			eTarget Target,
			const str::dString &Pattern,
			sRow First ) const
		{
			sRow &Row = First;
		qRH
		qRB
			Lock_();
			Row = C_().Search( User, Target, Pattern, First );
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
		sRow New( sRow Row = qNIL ) const
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
			ogzbsc::sURow User,
			const dDatum &Datum,
			eTarget Target,
			sRow Row ) const
		{
			return Store_( User, Datum, Target, Row );
		}
		void Recall(
			sRow Row,
			dDatum &Datum ) const
		{
			return Recall_( Row, Datum );
		}
		sRow Search(
			ogzbsc::sURow User,
			eTarget Target,
			const str::dString &Pattern ) const
		{
			return Search_( User, Target, Pattern, qNIL );
		}
		template <typename ... t> sdr::sRow Search(
			ogzbsc::sURow User,
			const str::dString &Pattern,
			eTarget First,
			t... Others ) const
		{
			sdr::sRow Row = Search( User, Pattern, First );

			if ( Row == qNIL )
				Row = Search( User, Pattern, Others... );

			return Row;
		}
	};

	typedef str::dString dBytes;
	qW( Bytes );

	class rRegularCallback
	: public cMeta
	{
	private:
		lstbch::qLBUNCHw( ogzbsc::sURow, sRow ) Users_;
		lstcrt::qLMCRATEw( dBytes, sRow ) Data_;
		lstbch::qLBUNCHw( eTarget, sRow ) Targets_;
	protected:
		virtual sRow OGZMTANew(	sRow Row ) override 
		{
			Row = Users_.New();

			if ( Data_.New( Row ) != Row )
				qRGnr();

			if ( Targets_.New() != Row )
				qRGnr();

			Data_(Row).Init();
			Data_.Flush();

			return Row;
		}
		virtual void OGZMTAErase( sRow Row ) override 
		{
			if ( Row == qNIL ) {
				tol::reset( Users_, Data_, Targets_ );
			} else {
				Users_.Remove( Row );
				Data_.Remove( Row );
				Targets_.Remove( Row );
			}
		}
		virtual void OGZMTAStore(
			ogzbsc::sURow User,
			const dDatum &Datum,
			eTarget Target,
			sRow Row ) override 
		{
			Users_.Store( User, Row );
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
			ogzbsc::sURow User,
			eTarget Target,
			const str::dString &Pattern,
			sRow First ) override
		{
			sRow &Row = First;

			if ( Row == qNIL )
				Row = Data_.First();
			else
				Row = Data_.Next( Row );

			while ( ( Row != qNIL )
				    && ( ( Users_( Row ) != User )
					     || ( Targets_( Row ) != Target )
					     || ( Data_( Row ) != Pattern ) ) )
					Row = Data_.Next( Row );

			return Row;
		}
	public:
		void reset( bso::bool__ P = true )
		{
			tol::reset( Users_, Data_, Targets_ );
		}
		qCVDTOR( rRegularCallback );
		void Init( void )
		{
			tol::Init( Users_, Data_, Targets_ );
		}
	};

	using bch::sHook;
}


#endif
