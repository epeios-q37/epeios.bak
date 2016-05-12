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

#ifndef OGZBSC__INC
# define OGZBSC__INC

# include "bso.h"
# include "err.h"
# include "sdr.h"
# include "str.h"
# include "lstbch.h"

namespace ogzbsc {
	typedef str::dString dDatum;
	qW( Datum );

	typedef str::dStrings dData;
	qW( Data );

	template <typename lrow, typename srow> qTCLONEd( lstbch::qLBUNCHd( lrow, srow ), dList );
	qW2( List );

	template <typename item, typename row> class cCommon
	{
	protected:
		// If 'Row' != 'qNIL', it must be used.
		virtual row OGZBSCNew( row Row ) = 0;
		// If 'Row' == 'qNIL', the content must be erased.
		virtual void OGZBSCDelete( row Row ) = 0;
		virtual void OGZBSCStore(
			const item &Item,
			row Row ) = 0;
		// Must return 'true' if the item exists, 'false' otherwise.
		virtual bso::sBool OGZBSCRecall(
			row Row,
			item &Item ) const = 0;
	public:
		qCALLBACK( Common );
		void Wipe( void )
		{
			OGZBSCDelete( qNIL );
		}
		row New( row Row )
		{
			return OGZBSCNew( Row );
		}
		void Delete( row Row )
		{
			return OGZBSCDelete( Row );
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

	template <typename item, typename row> class sItems
	{
	private:
		typedef cCommon<item,row> cCommon_;
		qRMV( cCommon_, C_, Callback_ );
	public:
		void reset( bso::bool__ P = true )
		{
			Callback_ = NULL;
		}
		E_CDTOR( sItems );
		void Init( cCommon_ &Callback )
		{
			Callback_ = &Callback;
		}
		void Wipe( void ) const
		{
			C_().Wipe();
		}
		row New( row Row = qNIL ) const
		{
			return C_().New( Row );
		}
		void Delete( row Row ) const
		{
			return C_().Delete( Row );
		}
		void Store(
			const item &Item,
			row Row ) const
		{
			return C_().Store( Item, Row );
		}
		row Append( const item &Item ) const
		{
			row Row = New();

			Store( Item, Row );

			return Row;
		}
		bso::bool__ Recall(
			row Row,
			item &Item ) const
		{
			return C_().Recall( Row, Item );
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
		virtual void OGZBSCDelete( row Row ) override
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
