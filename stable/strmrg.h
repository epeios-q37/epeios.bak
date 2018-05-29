/*
	Copyright (C) 1999 Claude SIMON (http://q37.info/contact/).

	This file is part of the Epeios framework.

	The Epeios framework is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

	The Epeios framework is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with the Epeios framework.  If not, see <http://www.gnu.org/licenses/>
*/

#ifndef STRMRG__INC
# define STRMRG__INC

# define STRMRG_NAME		"STRMRG"

# if defined( E_DEBUG ) && !defined( STRMRG_NODBG )
#  define STRMRG_DBG
# endif

// STRings MeRGer

# include "err.h"
# include "flw.h"

# include "err.h"
# include "flw.h"

# include "ctn.h"
# include "str.h"
# include "xtf.h"


namespace strmrg { 
	E_ROW( row__ );

	E_ROW( _irow__ );	// Item row.

# if 0	// Sinon VC++ perd les pdales lors de l'dition de liens...
	namespace {
# endif
		E_ROW( _srow__);
		typedef str::string_ _string_;
		typedef ctn::E_MCONTAINERt_( _string_, _srow__ ) _strings_;
		E_AUTO( _strings );

		typedef bch::E_BUNCHt_( _irow__, row__ ) _irows_;
		E_AUTO( _irows );

		typedef _irows_ _table_;
		E_AUTO( _table );

		class _item_
		{
		public:
			struct s {
				_srow__ String;
				_irows_::s Items;
			} &S_;
			_irows_ Items;
			_item_( s &S )
			: S_( S ),
			  Items( S.Items )
			{}
			void reset( bso::bool__ P = true )
			{
				S_.String = qNIL;
				Items.reset( P );
			}
			void plug( bch::sHook &Hook )
			{
				return Items.plug( Hook );
			}
			void plug( qASd *AS )
			{
				return Items.plug( AS );
			}
			_item_ &operator =( const _item_ &I )
			{
				S_.String = I.S_.String;
				Items = I.Items;

				return *this;
			}
			void Init( _srow__ String = qNIL )
			{
				if ( ( S_.String != qNIL ) || ( Items.Amount() != 0 ) )
					qRFwk();

				S_.String = String;
				Items.Init();
			}
			E_RODISCLOSE_( _srow__, String );
			void Append( _irow__ Item )
			{
				if ( S_.String != qNIL )
					qRFwk();

				Items.Append( Item );
			}
			bso::bool__ ContainsItems( void ) const
			{
				return Items.Amount() != 0;
			}
			bso::bool__ ContainsString( void ) const
			{
				return S_.String != qNIL;
			}
		};

		E_AUTO( _item );

		typedef ctn::E_MCONTAINERt_( _item_, _irow__ ) _items_;
		E_AUTO( _items );
# if 0
	}
# endif
	class table_
	{
	public:
		struct s {
			_strings_::s Strings;
			_items_::s Items;
			_table_::s Main;
		};
		_strings_ Strings;
		_items_ Items;
		_table_ Main;
		table_( s &S )
		: Strings( S.Strings ),
		  Items( S.Items ),
		  Main( S.Main )
		{}
		void reset( bso::bool__ P = true )
		{
			Strings.reset( P );
			Items.reset( P );
			Main.reset( P );
		}
		void plug( qASd *AS )
		{
			Strings.plug( AS );
			Items.plug( AS );
			Main.plug( AS );
		}
		table_ &operator =(const table_ &T)
		{
			Strings = T.Strings;
			Items = T.Items;
			Main = T.Main;

			return *this; 
		}
		void Init( void )
		{
			Strings.Init();
			Items.Init();
			Main.Init();
		}
		row__ Append( const str::string_ &String );
		row__ AppendMono( const table_ &Table );	// 'Table' is stored in one entry.
		void AppendMulti( const table_ &Table );	// An entry is created for each (first-level) entry of 'Table' 
		row__ AppendMono( const str::strings_ &Strings );	// ATTENTION : Cre une seule entre avec tous les strings, et pas autant d'entre que de string.
		void AppendMulti( const str::strings_ &Strings );	// ATTENTION : Ajoute autant d'entre que de 'Strings'.
		void GetTable(
			row__ Row, 
			table_ &Table ) const;
		E_NAVt( Main., row__ );
	};

	void GetTable(
		const _item_ &Item,
		const _items_ &Items,
		const _strings_ &Strings,
		table_ &Table );

	E_CDEF(char, DefaultSeparatorToken, '|');
	E_CDEF(char, DefaultBeginToken, '(');
	E_CDEF(char, DefaultEndToken, ')');
	E_CDEF(char, DefaultEscapeToken, '\\');

	struct tokens__ {
		char
			Separator,	// Caractre sparant les diffrentes entres.
			Begin,		// Caractre marquant le dbut d'un groupe.
			End,		// Caractre marquant la fin d'un groupe.
			Escape;		// Caratre d'chappement.
		void reset( bso::bool__ = true )
		{
			Separator = DefaultSeparatorToken;
			Begin = DefaultBeginToken;
			End = DefaultEndToken,
			Escape = DefaultEscapeToken;
		}
		E_CDTOR( tokens__ );
		tokens__( char Separator )
		{
			reset( false );

			Init( Separator );
		}
		tokens__(
			char Separator,
			char Begin, 
			char End,
			char Escape = DefaultEscapeToken )
		{
			reset( false );

			Init( Separator, Begin, End, Escape );
		}
		void Init(
			char Separator,
			char Begin,
			char End,
			char Escape = DefaultEscapeToken )
		{
			this->Separator = Separator;
			this->Begin = Begin;
			this->End = End;
			this->Escape = Escape;
		}
		void Init( char Separator )
		{
			Init( Separator, DefaultBeginToken, DefaultEndToken );
		}
	};

	E_AUTO( table );

	void Merge(
		const table_ &Table,
		flw::oflow__ &Flow,
		const tokens__ &Tokens = tokens__() );

	void Merge(
		const table_ &Table,
		str::string_ &String,
		const tokens__ &Tokens = tokens__() );

	bso::bool__ Split(
		xtf::extended_text_iflow__ &Flow,
		table_ &Table,
		const tokens__ &Tokens = tokens__() );

	bso::bool__ Split(
		const str::string_ &String,
		table_ &Table,
		const tokens__ &Tokens = tokens__() );

	enum availability__ {
		aNone,		// Pas d'autres paramtres disponible.
		aEmpty,		// Le prochain paramtre est vide.
		aString,	// Le prochain paramtre est un 'string'.
		aTable,		// Le prochain paramtre est une table.
		a_amount,
		a_Undefined
	};

	class retriever__ {
	private:
		const _strings_ *_Strings;
		const _items_ *_Items;
		const _irows_ *_Rows;
		row__ _Row;
		const _strings_ &s_( void ) const
		{
			if ( _Strings == NULL )
				qRFwk();

			return *_Strings;
		}
		const _items_ &_I( void ) const
		{
			if ( _Items == NULL )
				qRFwk();

			return *_Items;
		}
		const _irows_ &_R( void ) const
		{
			if ( _Rows == NULL )
				qRFwk();

			return *_Rows;
		}
	public:
		void reset( bso::bool__ = true )
		{
			_Strings = NULL;
			_Items = NULL;
			_Rows = NULL;
			_Row = qNIL;
		}
		E_CDTOR( retriever__ );
		void Init(
			const _strings_ &Strings,
			const _items_ &Items,
			const _irows_ &Rows )
		{
			_Strings = &Strings;
			_Items = &Items;
			_Rows = &Rows;

			_Row = Rows.First();
		}
		void Init( const table_ &Table )
		{
			Init( Table.Strings, Table.Items, Table.Main );
		}
		void GetString( str::string_ &Target );
		void GetTable( table_ &Table )
		{
			ctn::qCMITEMs( _item_, _irow__ ) Item;
			Item.Init( _I() );

			if ( _Row != qNIL ) {
				strmrg::GetTable( Item( _R()( _Row ) ), _I(), s_(), Table );

				_Row = _R().Next( _Row );
			}
		}
		void Skip( void )
		{
			_Row = _R().Next( _Row );
		}
		// Ne retourne que le contenu d'une seule entre. L'entre en cours doit tre un 'string', ou une table ne contenant que des 'string's, sinon erreur.
		void GetStrings( str::strings_ &Strings );
		availability__ Availability( void ) const
		{
			availability__ Availability = a_Undefined;

			if ( _Row == qNIL )
				Availability = aNone;
			else {
				if ( _I()( _R()( _Row ) ).ContainsString() )	// Positionne 'Item()'.
					Availability = aString;
				else if ( _I()().ContainsItems() )
					Availability = aTable;
				else
					Availability = aEmpty;
			}

			return Availability;
		}
	};
}

#endif
