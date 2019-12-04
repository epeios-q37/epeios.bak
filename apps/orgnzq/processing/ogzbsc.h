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

// OrGaniZer BaSiCs

#ifndef OGZBSC_INC_
# define OGZBSC_INC_

# include "bso.h"
# include "err.h"
# include "sdr.h"
# include "str.h"
# include "lstbch.h"
# include "crt.h"

namespace ogzbsc {
	typedef str::dString dDatum;
	qW( Datum );

	typedef str::dStrings dData;
	qW( Data );

	using crt::sHooks;

	qROW( URow );	// User row.
	qROW( MRow );	// User meta row.
	qROW( CRow );	// User column row.
	qROW( ERow );	// User entry row.
	qROW( FRow );	// User field row.
	qROW( RRow );	// User record row.

	template <typename row> qTCLONEd( bch::qBUNCHdl( row ), dRows );

	template <typename row> qTCLONEd( bch::qBUNCHdl( row ), dList );
	qW1(List );

	template <typename item, typename row> class cCommon
	{
	protected:
		// If 'Row' != 'qNIL', it must be used.
		virtual row OGZBSCNew( row Row ) = 0;
		// If 'Row' == 'qNIL', the content must be erased.
		virtual void OGZBSCErase( row Row ) = 0;
		virtual void OGZBSCStore(
			const item &Item,
			row Row ) = 0;
		// Must return 'true' if the item exists, 'false' otherwise.
		virtual bso::sBool OGZBSCRecall(
			row Row,
			item &Item ) const = 0;
	public:
		qCALLBACK( Common );
		row New( row Row )
		{
			return OGZBSCNew( Row );
		}
		void Erase( row Row )
		{
			return OGZBSCErase( Row );
		}
		void Store(
			const item &Item,
			row Row )
		{
			OGZBSCStore( Item, Row );
		}
		bso::bool__ Recall(
			row Row,
			item &Item )
		{
			return OGZBSCRecall( Row, Item );
		}
	};

	class rLock
	{
	private:
		mutable mtx::rHandler Mutex_;
	protected:
		void Lock_( void ) const
		{
			mtx::Lock( Mutex_ );
		}
		void Unlock_( void ) const
		{
			mtx::Unlock( Mutex_ );
		}
	public:
		void reset( bso::sBool P = true )
		{
			if ( P ) {
				if ( Mutex_ != mtx::UndefinedHandler )
					mtx::Delete( Mutex_ );
			}

			Mutex_ = mtx::UndefinedHandler;
		}
		void Init( void )
		{
			if ( Mutex_ != mtx::UndefinedHandler )
				mtx::Delete( Mutex_ );

			Mutex_ = mtx::Create();
		}
	};

	template <typename item, typename row> class mItems
	: public rLock
	{
	private:
		typedef cCommon<item,row> cCommon_;
		qRMV( cCommon_, C_, Callback_ );
		row New_( row Row ) const
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
		void Erase_( row Row ) const
		{
		qRH
		qRB
			Lock_();
			C_().Erase( Row );
		qRR
		qRT
			Unlock_();
		qRE
		}
		void Store_(
			const item &Item,
			row Row ) const
		{
		qRH
		qRB
			Lock_();
			C_().Store( Item, Row );
		qRR
		qRT
			Unlock_();
		qRE
		}
		row Append_( const item &Item ) const
		{
			row Row = qNIL;
		qRH
		qRB
			Lock_();
			Row = C_().New();
			C_().Store( Item, Row );
		qRR
		qRT
			Unlock_();
		qRE
			return Row;
		}
		bso::bool__ Recall_(
			row Row,
			item &Item ) const
		{
			bso::sBool Exists = false;
		qRH
		qRB
			Lock_();
			Exists = C_().Recall( Row, Item );
		qRR
		qRT
			Unlock_();
		qRE
			return Exists;
		}
	public:
		void reset( bso::bool__ P = true )
		{
			Callback_ = NULL;
			rLock::reset( P );
		}
		E_CDTOR( mItems );
		void Init( cCommon_ &Callback )
		{
			Callback_ = &Callback;
			rLock::Init();
		}
		void Wipe( void ) const
		{
			return Erase_( qNIL );
		}
		row New( row Row = qNIL ) const
		{
			return New_( Row );
		}
		void Erase( row Row ) const
		{
			return Erase_( Row );
		}
		void Store(
			const item &Item,
			row Row ) const
		{
			return Store_( Item, Row );
		}
		row Append( const item &Item ) const
		{
			return Append_( Item );
		}
		bso::bool__ Recall(	// Returns false if inexistent.
			row Row,
			item &Item,
			qRPD ) const
		{
			if ( Recall_( Row, Item ) )
				return true;
			else { 
				if ( qRPT )
					qRGnr();

				return false;
			}
		}
	};

	template <typename crate, typename item, typename row> class rRegularCallback
	: public cCommon<item,row>
	{
	private:
		crate Items_;
	protected:
		virtual row OGZBSCNew( row Row ) override
		{
			return Items_.New();
		}
		virtual void OGZBSCErase( row Row ) override
		{
			if ( Row == qNIL )
				Items_.Init();
			else
				Items_.Remove( Row );
		}
		virtual void OGZBSCStore(
			const item &Item,
			row Row ) override
		{
			Items_.Store( Item, Row );
		}
		virtual bso::sBool OGZBSCRecall(
			row Row,
			item &Item ) const override
		{
			if ( Items_.Exists( Row ) ) {
				Items_.Recall( Row, Item );
				return true;
			} else
				return false;
		}
	public:
		void reset( bso::sBool P = true )
		{
			Items_.reset( P );
		}
		qCVDTOR( rRegularCallback );
		void Init( void )
		{
			Items_.Init();
		}
	};
}

#endif
