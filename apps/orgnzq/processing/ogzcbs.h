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

#ifndef OGZCBS__INC
# define OGZCBS__INC

# ifdef XXX_DBG
#	define OGZCBS__DBG
# endif

# include "bso.h"
# include "tol.h"
# include "sdr.h"
# include "bch.h"
# include "lstctn.h"

// 'Base Template Typenames'. 'i_item' : instanciable item.
#define OGZCBS_BTT	typename item, typename i_item, typename row, typename subitem, typename subrow
// 'Base Template Parameters'.
#define OGZCBS_BTP	item, i_item, row, subitem, subrow

// 'Regular Callback Template Typenames'.
#define OGZCBS_RCTT	typename container, OGZCBS_BTT
// 'Regular Callback Template PArameterss'.
#define OGZCBS_RCTP	container, OGZCBS_BTP

namespace ogzcbs {

	// Callback for static (fixed-sized) objects.
	template <typename item, typename row> class fSCallback
	{
	protected:
		// If 'Row' != 'qNIL', it must be used.
		virtual row OGZCBSNew( row Row ) = 0;
		// If 'Row' == 'qNIL', the content must be erased.
		virtual void OGZCBSDelete( row Row ) = 0;
		virtual void OGZCBSStore(
			const item &Item,
			row Row ) = 0;
		// Must 'true' if the item exists, 'false' otherwise.
		virtual bso::bool__ OGZCBSRecall(
			row Row,
			item &Item ) const = 0;
	public:
		void reset( bso::bool__ = true )
		{
			//Standadisation.
		}
		E_CVDTOR( fSCallback );
		void Init( void )
		{
			//Standardisation.
		}
	};

	// Storage for static (fixed-sized) objects.
	template <typename item, typename row> class fSItems
	{
	private:
		typedef fSCallback<item, row> fCallback;
		Q37_MRMDF( fCallback, C_, Callback_ );
	public:
		void reset( bso::bool__ P = true )
		{
			Callback_ = NULL;
		}
		E_CDTOR( fSItems );
		void Init( fCallback &Callback )
		{
			Callback_ = &Callback;
		}
		void Wipe( void )
		{
			C_().Wipe();
		}
		row New( row Row = qNIL )
		{
			return C_().New( Row );
		}
		void Delete( row Row )
		{
			return C_().Delete( Row );
		}
		void Store(
			const item &Item,
			row Row )
		{
			return C_().Store( Item, Row );
		}
		row Append( const item &Item )
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

	// Callback for dynamic (not fixed size) objetcs.
	template <OGZCBS_BTT> class fDCallback
	{
	protected:
		// If 'Row' != 'qNIL', it must be used.
		virtual row OGZCBSNew( row Row ) = 0;
		// If 'Row' == 'qNIL', the content must be erased.
		virtual void OGZCBSDelete( row Row ) = 0;
		virtual void OGZCBSStore(
			const item &Item,
			row Row ) = 0;
		// Must 'true' if the item exists, 'false' otherwise.
		virtual bso::bool__ OGZCBSRecall(
			row Row,
			item &Item ) const = 0;
		virtual void OGZCBSStore(
			const subitem &SubItem,
			row Row,
			subrow SubRow )
		{
		qRH
			i_item Item;
		qRB
			Item.Init();

			Recall( Row, Item );

			Item.Store( SubItem, SubRow );

			Store( Item, Row );
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
			i_item Item;
		qRB
			Item.Init();

			Recall( Row, Item );

			Item.Recall( SubRow, SubItem );

			Store( Item, Row );
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
			i_item Item;
		qRB
			Item.Init();

			Recall( Row, Item );

			SubRow = Item.Add( SubItem );

			Store( Item, Row );
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
			i_item Item;
		qRB
			Item.Init();

			Recall( Row, Item );

			Item.Remove( SubRow );

			Store( Item, Row );
		qRR
		qRT
		qRE
		}
	public:
		void reset( bso::bool__ = true )
		{
			// Standardisation.
		}
		E_CVDTOR( fDCallback);
		void Init( void )
		{
			// Standardisation.
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
			item &Item ) const
		{
			return OGZCBSRecall( Row, Item );
		}
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

	// Callbakc-based storage for dynamic objects.
	template <OGZCBS_BTT> class fDItems
	{
	private:
		typedef fDCallback<OGZCBS_BTP> fCallback;
		Q37_MRMDF( fCallback, C_, Callback_ );
	public:
		void reset( bso::bool__ P = true )
		{
			Callback_ = NULL;
		}
		E_CDTOR( fDItems );
		void Init( fCallback &Callback )
		{
			Callback_ = &Callback;
		}
		void Wipe( void )
		{
			C_().Wipe();
		}
		row New( row Row = qNIL )
		{
			return C_().New( Row );
		}
		void Delete( row Row )
		{
			return C_().Delete( Row );
		}
		void Store(
			const item &Item,
			row Row )
		{
			return C_().Store( Item, Row );
		}
		bso::bool__ Recall(
			row Row,
			item &Item ) const
		{
			return C_().Recall( Row, Item );
		}
		void Store(
			const subitem &SubItem,
			row Row,
			subrow SubRow )
		{
			return C_().Store( SubItem, Row, SubRow );
		}
		void Recall(
			row Row,
			subrow SubRow,
			subitem &SubItem )
		{
			return C_().Recall( Row, SubRow, SubItem );
		}
		subrow Append(
			row Row,
			subitem &SubItem )
		{
			return C_().Append( Row, SubItem );
		}
		void Remove(
			row Row,
			subrow SubRow )
		{
			return C_().Remove( Row, SubRow );
		}
	};

	/*
	E_ROW( _rcrow__ ); // Callback row.
	template <OGZCBS_RCTT> class _regular_callback___
	: public callback__ <OGZCBS_BTP>
	{
	private:
		bch::E_BUNCHt( _rcrow__, row ) _Links;
		container _Items;
	protected:
		virtual row OGZCBSNew( row Row ) override
		{
			if ( Row != qNIL )
				if ( _Links.Exists( Row ) ) {
					if ( _Links( Row ) != qNIL )
						qRGnr();
				} else
					_Links.Allocate( *Row + 1, qNIL );
			else
				Row = _Links.Append( qNIL );

			return Row;
		}
		virtual void OGZCBSDelete( row Row, int test ) override
		{
			if ( Row == qNIL ) {
				_Items.Init();
				_Links.Init();
			} else {
				_rcrow__ CRow = _Links( Row );

				if ( CRow != qNIL ) {
					_Items.Remove( CRow );
					_Links.Store( qNIL, Row );
				}
			}
		}
		virtual void OGZCBSStore(
			const item &Item,
			row Row ) override
		{
			_rcrow__ RCRow = _Links( Row );

			if ( RCRow == qNIL ) {
				RCRow = _Items.New();

				_Links.Store( RCRow, Row );

				_Items( RCRow ).Init();
			}

			_Items.Store( Item, _Links( Row ) );
		}
		virtual bso::bool__ OGZCBSRecall(
			row Row,
			item &Item ) const override
		{
			_rcrow__ RCRow = _Links( Row );

			if ( RCRow != qNIL ) {
				_Items.Recall( _Links( Row ), Item );
				return true;
			} else
				return false;
		}
	public:
		void reset( bso::bool__ P = true )
		{
			_Links.reset( P );
			_Items.reset( P );
		}
		E_CVDTOR( _regular_callback___ );
		void Init( void )
		{
			_Links.Init();
			_Items.Init();
		}
		bch::E_BUNCHt( _rcrow__, row ) &Links( void )
		{
			return _Links;
		}
		const bch::E_BUNCHt( _rcrow__, row ) &Links( void ) const
		{
			return _Links;
		}
		container &Items( void )
		{
			return _Items;
		}
		const container &Items( void ) const
		{
			return _Items;
		}
	};

	template <typename item, typename i_item, typename row, typename subitem, typename subrow>
	E_TTCLONE__( _regular_callback___<E_COVER2( lstctn::E_LMCONTAINERt( item, _rcrow__ ), OGZCBS_BTP )>, mregular_callback___ );

	template <typename item, typename i_item, typename row, typename subitem, typename subrow>
	E_TTCLONE__( _regular_callback___<E_COVER2( lstctn::E_LCONTAINERt( item, _rcrow__ ), OGZCBS_BTP> ), regular_callback___ );
	*/
}


#endif
