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

// OrGaniZer Callback-Based Storage.

#ifndef OGZCBS_INC_
# define OGZCBS_INC_

# ifdef XXX_DBG
#	define OGZCBS__DBG
# endif

# include "ogzbsc.h"

# include "bso.h"
# include "tol.h"
# include "sdr.h"
# include "bch.h"
# include "lstctn.h"
# include "lstbch.h"

namespace ogzcbs {
	// Callback for static (fixed-sized) objects.
	template <typename item, typename row, typename srow> class cStatic
	{
	protected:
		virtual ogzbsc::sAmount OGZCBSAmount( void ) = 0;
		// If 'Amount' == 0, returns all from 'Indice'.
		virtual void OGZCBSGetSet(
			ogzbsc::sIndice Indice,
			ogzbsc::sAmount Amount,
			ogzbsc::dSet<row,srow> &Set ) = 0;
		// If 'Row' != 'qNIL', it must be used.
		virtual row OGZCBSNew( row Row ) = 0;
		// If 'Row' == 'qNIL', the content must be erased.
		virtual void OGZCBSDelete( row Row ) = 0;
		virtual void OGZCBSStore(
			const item &Item,
			row Row ) = 0;
		// Must return 'true' if the item exists, 'false' otherwise.
		virtual bso::sBool OGZCBSRecall(
			row Row,
			item &Item ) const = 0;
	public:
		qCALLBACK( Static );
		ogzbsc::sAmount Amount( void )
		{
			return OGZCBSAmount();
		}
		void GetSet(
			ogzbsc::sIndice Indice,
			ogzbsc::sAmount Amount,
			ogzbsc::dSet<row,srow> &Set )
		{
			return OGZCBSGetSet( Indice, Amount, Set );
		}
		void Wipe( void )
		{
			OGZCBSDelete( qNIL );
		}
		row New( row Row )
		{
			return OGZCBSNew( Row );
		}
		void Delete( row Row )
		{
			return OGZCBSDelete( Row );
		}
		void Store(
			const item &Item,
			row Row )
		{
			OGZCBSStore( Item, Row );
		}
		bso::bool__ Recall(
			row Row,
			item &Item )
		{
			return OGZCBSRecall( Row, Item );
		}
	};

	// Storage for static (fixed-sized) objects.
	template <typename item, typename row, typename srow> class sStaticItems
	{
	private:
		typedef cStatic<item,row,srow> cStatic_;
		qRMV( cStatic_, C_, Callback_ );
	public:
		void reset( bso::bool__ P = true )
		{
			Callback_ = NULL;
		}
		E_CDTOR( sStaticItems );
		void Init( cStatic_ &Callback )
		{
			Callback_ = &Callback;
		}
		ogzbsc::sAmount Amount( void )
		{
			return C_().Amount();
		}
		void GetSet(
			ogzbsc::sIndice Indice,
			ogzbsc::sAmount Amount,
			ogzbsc::dSet<row,srow> &Set ) const
		{
			C_().GetSet( Indice, Amount, Set );
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
}

// 'Base Template Typenames'.
#define OGZCBS_BTT	typename d_item, typename w_item, typename row, typename subitem, typename subrow, typename srow
// 'Base Template Parameters'.
#define OGZCBS_BTP	d_item, w_item, row, subitem, subrow, srow

namespace ogzcbs {
	// Callback for dynamic (not fixed size) objetcs.
	template <OGZCBS_BTT> class cDynamic
	: public cStatic<d_item,row,srow>
	{
	protected:
		virtual void OGZCBSStore(
			const subitem &SubItem,
			row Row,
			subrow SubRow )
		{
		qRH
			w_item Item;
		qRB
			Item.Init();

			cStatic<d_item,row,srow>::Recall( Row, Item );

			Item.Store( SubItem, SubRow );

			cStatic<d_item,row,srow>::Store( Item, Row );
		qRR
		qRT
		qRE
		}
		virtual void OGZCBSRecall(
			row Row,
			subrow SubRow,
			subitem &SubItem )
		{
		qRH
			w_item Item;
		qRB
			Item.Init();

			cStatic<d_item, row, srow>::Recall( Row, Item );

			Item.Recall( SubRow, SubItem );

			cStatic<d_item, row, srow>::Store( Item, Row );
		qRR
		qRT
		qRE
		}
		virtual subrow OGZCBSAppend(
			row Row,
			subitem &SubItem )
		{
			subrow SubRow = qNIL;
		qRH
			w_item Item;
		qRB
			Item.Init();

			cStatic<d_item, row, srow>::Recall( Row, Item );

			SubRow = Item.Add( SubItem );

			cStatic<d_item, row, srow>::Store( Item, Row );
		qRR
		qRT
		qRE
			return SubRow;
		}
		virtual void OGZCBSRemove(
			row Row,
			subrow SubRow )
		{
		qRH
			w_item Item;
		qRB
			Item.Init();

			cStatic<d_item, row, srow>::Recall( Row, Item );

			Item.Remove( SubRow );

			cStatic<d_item, row, srow>::Store( Item, Row );
		qRR
		qRT
		qRE
		}
	public:
		qCALLBACK( Dynamic );
		void Store(
			const subitem &SubItem,
			row Row,
			subrow SubRow )
		{
			return OGZCBSStore( SubItem, Row, SubRow );
		}
		bso::bool__ Recall(
			row Row,
			subrow SubRow,
			subitem &SubItem )
		{
			return OGZCBSRecall( Row, SubRow, SubItem );
		}
		subrow Append(
			row Row,
			subitem &SubItem )
		{
			return OGZCBSAppend( Row, SubItem );
		}
		void Remove(
			row Row,
			subrow SubRow )
		{
			return OGZCBSRemove( Row, SubRow );
		}
	};

	// Callback-based storage for dynamic objects.
	template <OGZCBS_BTT> class sDynamicItems
	{
	private:
		typedef cDynamic<OGZCBS_BTP> cDynamic_;
		qRMV( cDynamic_, C_, Callback_ );
		typedef cStatic<d_item,row,srow> cStatic_;
		cStatic_ &SC_( void ) const
		{
			return C_();
		}
	public:
		void reset( bso::bool__ P = true )
		{
			Callback_ = NULL;
		}
		E_CDTOR( sDynamicItems );
		void Init( cDynamic_ &Callback )
		{
			Callback_ = &Callback;
		}
		void Wipe( void ) const
		{
			C_().Wipe();
		}
		row New( row Row = qNIL ) const
		{
			return SC_().New( Row );
		}
		void Delete( row Row ) const
		{
			return SC_().Delete( Row );
		}
		void Store(
			const d_item &Item,
			row Row ) const
		{
			return SC_().Store( Item, Row );
		}
		bso::bool__ Recall(
			row Row,
			d_item &Item ) const
		{
			return SC_().Recall( Row, Item );
		}
		void Store(
			const subitem &SubItem,
			row Row,
			subrow SubRow ) const
		{
			return C_().Store( SubItem, Row, SubRow );
		}
		void Recall(
			row Row,
			subrow SubRow,
			subitem &SubItem ) const
		{
			return C_().Recall( Row, SubRow, SubItem );
		}
		subrow Append(
			row Row,
			subitem &SubItem ) const
		{
			return C_().Append( Row, SubItem );
		}
		void Remove(
			row Row,
			subrow SubRow ) const
		{
			return C_().Remove( Row, SubRow );
		}
		void GetSet(
			ogzbsc::sIndice Indice,
			ogzbsc::sAmount Amount,
			ogzbsc::dSet<row,srow> &Set ) const
		{
			SC_().GetList( Indice, Amount, Rows );
		}
	};

	template <typename item, typename row, typename srow> class rRegularStaticCallback
	: public cStatic<item,row,srow>
	{
	private:
		lstbch::qLBUNCHw( item, row ) Items_;
	protected:
		virtual ogzbsc::sAmount OGZCBSAmount( void ) override
		{
			return Items_.Amount();
		}
		virtual void OGZCBSGetList(
			ogzbsc::sIndice Indice,
			ogzbsc::sAmount Amount,
			ogzbsc::dSet<row,srow> &Set ) override
		{
			row Row = Items_.First( Indice );

			if ( Amount == 0 )
				Amount = AmountMax;

			while ( ( Row != qNIL ) && Amount-- ) {
				List.Append( Row );
				Row = Items_.Next( Row );
			}
		}
		virtual row OGZCBSNew( row Row ) override
		{
			return Items_.New();
		}
		virtual void OGZCBSDelete( row Row ) override
		{
			if ( Row == qNIL )
				Items_.Init();
			else
				Items_.Remove( Row );
		}
		virtual void OGZCBSStore(
			const item &Item,
			row Row ) override
		{
			Items_.Store( Item, Row );
		}
		virtual bso::sBool OGZCBSRecall(
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
		qCVDTOR( rRegularStaticCallback );
		void Init( void )
		{
			Items_.Init();
		}
	};
}

// 'Regular Callback Template Typenames'.
#define OGZCBS_RCTT	typename container, OGZCBS_BTT
// 'Regular Callback Template PArameterss'.
#define OGZCBS_RCTP	container, OGZCBS_BTP


namespace ogzcbs {
	template <OGZCBS_BTT> class rRegularDynamicCallback
	: public cDynamic<OGZCBS_BTP>
	{
	private:
		lstctn::qLMCONTAINERw( d_item, row ) Container_;
	protected:
		virtual ogzbsc::sAmount OGZCBSAmount( void ) override
		{
			return Container_.Amount();
		}
		virtual void OGZCBSGetSet(
			ogzbsc::sIndice Indice,
			ogzbsc::sAmount Amount,
			ogzbsc::dList<row,srow> &List ) override
		{
			row Row = Container_.First( Indice );

			if ( Amount == 0 )
				Amount = AmountMax;

			while ( (Row != qNIL) && Amount-- ) {
				List.Append( Row );
				Row = Container_.Next( Row );
			}
		}
		virtual row OGZCBSNew( row Row ) override
		{
			return Container_.New( Row );
		}
		virtual void OGZCBSDelete( row Row ) override
		{
			if ( Row == qNIL )
				Container_.Init();
			else
				Container_.Remove( Row );
		}
		virtual void OGZCBSStore(
			const d_item &Item,
			row Row ) override
		{
			Container_.Store( Item, Row );
		}
		virtual bso::sBool OGZCBSRecall(
			row Row,
			d_item &Item ) const override
		{
			if ( Container_.Exists(Row) ) {
				Container_.Recall( Row, Item );
				return true;
			}
			else {
				return false;
			}
		}
	public:
		void reset( bso::sBool P = true )
		{
			Container_.reset( P );
		}
		qCVDTOR( rRegularDynamicCallback );
		void Init( void )
		{
			Container_.Init();
		}
	};

}


#endif
