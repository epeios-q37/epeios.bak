/*
	'strmrg.h' by Claude SIMON (http://zeusw.org/).

	'strmrg' is part of the Epeios framework.

    The Epeios framework is free software: you can redistribute it and/or
	modify it under the terms of the GNU General Public License as published
	by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Epeios framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The Epeios framework.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef STRMRG__INC
# define STRMRG__INC

# define STRMRG_NAME		"STRMRG"

# if defined( E_DEBUG ) && !defined( STRMRG_NODBG )
#  define STRMRG_DBG
# endif

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

// STRings MeRGer

# include "err.h"
# include "flw.h"

# include "err.h"
# include "flw.h"

# include "ctn.h"
# include "str.h"
# include "xtf.h"


namespace strmrg {
# if 0	// Old
	typedef ctn::E_CONTAINER_( str::strings_ ) table_;

	E_AUTO( table );

	void Merge(
		const table_ &Table,
		flw::oflow__ &Flow,
		char EntrySeparator,
		char FieldSeparator,
		char EscapeChar );

	void Merge(
		const table_ &Table,
		str::string_ &String,
		char EntrySeparator,
		char FieldSeparator,
		char EscapeChar );

	bso::bool__ Split(
		xtf::extended_text_iflow__ &Flow,
		table_ &Table,
		char EntrySeparator,
		char FieldSeparator,
		char EscapeChar );

	bso::bool__ Split(
		const str::string_ &String,
		table_ &Table,
		char EntrySeparator,
		char FieldSeparator,
		char EscapeChar );

	// Dans tous les cas, sauf pour 'aStrings', on peut appeler 'GetString()' et 'GetStrings()'.
	enum availability__ {
		aNone,		// Pas d'autres paramètres disponible.
		aEmpty,		// Le prochain paramètre est vide.
		aString,	// Le prochain paramètre est un 'string'.
		aStrings,	// Le prochain paramètre est un 'strings' (ne peut ête récupèré que par 'GetStrings()'.
		a_amount,
		a_Undefined
	};

	class retriever__ {
	private:
		const table_ *_Table;
		sdr::row__ _Row;
		const table_ &_T( void ) const
		{
			if ( _Table == NULL )
				ERRFwk();

			return *_Table;
		}
	public:
		void reset( bso::bool__ = true )
		{
			_Table = NULL;
			_Row = E_NIL;
		}
		E_CDTOR( retriever__ );
		void Init( const table_ &Table )
		{
			_Table = &Table;
			_Row = Table.First();
		}
		void GetString( str::string_ &Target )
		{
			ctn::E_CITEM( str::strings_ ) Strings;
			ctn::E_CMITEM( str::string_ ) String;

			if ( _Row != E_NIL ) {
				Strings.Init(_T() );

				switch ( Strings( _Row ).Amount() ) {
				case 0:
					break;
				case 1:
					String.Init(Strings() );
					Target = String(Strings().First() );
					break;
				default:
					ERRFwk();
					break;
				}

				_Row = _T().Next( _Row );
			}
		}
		void GetStrings( str::strings_ &Target )
		{
			ctn::E_CITEM( str::strings_ ) Strings;

			if ( _Row != E_NIL ) {
				Strings.Init(_T() );

				Target = Strings( _Row );

				_Row = _T().Next( _Row );
			}
		}
		availability__ Availability( void ) const
		{
			availability__ Availability = a_Undefined;

			if ( _Row == E_NIL )
				Availability = aNone;
			else {
				ctn::E_CITEM( str::strings_ ) Strings;
				ctn::E_CMITEM( str::string_ ) String;

				Strings.Init( _T() );

				switch ( Strings( _Row ).Amount() ) {
				case 0:
					Availability = aEmpty;
					break;
				case 1:
					String.Init(Strings() );

					if ( String( Strings().First() ).Amount() == 0 )
						Availability = aEmpty;
					else
						Availability = aString;
					break;
				default:
					Availability = aStrings;
					break;
				}
			}

			return Availability;
		}
	};
# else // New
	E_ROW( row__ );

	E_ROW( _irow__ );	// Item row.

# if 0	// Sinon VC++ perd les pédales lors de l'édition de liens...
	namespace {
# endif
		typedef bch::E_BUNCHt_( _irow__, row__ ) _irows_;
		E_AUTO( _irows );

		typedef _irows_ _table_;
		E_AUTO( _table );

		class _item_
		{
		public:
			struct s {
				str::string_::s String;
				_irows_::s Items;
			};
			str::string_ String;
			_irows_ Items;
			_item_( s &S )
			: String( S.String ),
			  Items( S.Items )	// Si vide, l'item conteient une chaîne vide.
			{}
			void reset( bso::bool__ P = true )
			{
				String.reset( P );
				Items.reset( P );
			}
			void plug( ags::E_ASTORAGE_ &AS )
			{
				String.plug( AS );
				Items.plug( AS );
			}
			_item_ &operator =( const _item_ &I )
			{
				String = I.String;
				Items = I.Items;

				return *this;
			}
			void Init( void )
			{
				String.Init();
				Items.Init();
			}
			void Init( const str::string_ &String )
			{
				if ( ( this->String.Amount() != 0 ) || ( Items.Amount() != 0 ) )
					ERRFwk();

				this->String.Init( String );
				Items.Init();
			}
			void Append( _irow__ Item )
			{
				if ( ( String.Amount() != 0 ) )
					ERRFwk();

				Items.Append( Item );

			}
			bso::bool__ ContainsItems( void ) const
			{
				if ( Items.Amount() != 0 ) {
					if ( String.Amount() != 0 )
						ERRFwk();

					return true;
				} else
					return false;

			}
			bso::bool__ ContainsString( void ) const
			{
				return !ContainsItems();
			}
		};

		E_AUTO( _item );

		typedef ctn::E_CONTAINERt_( _item_, _irow__ ) _items_;
		E_AUTO( _items );
# if 0
	}
# endif
	class table_
	{
	public:
		struct s {
			_items_::s Items;
			_table_::s Main;
		};
		_items_ Items;
		_table_ Main;
		table_( s &S )
		: Items( S.Items ),
		  Main( S.Main )
		{}
		void reset( bso::bool__ P = true )
		{
			Items.reset( P );
			Main.reset( P );
		}
		void plug( ags::E_ASTORAGE_ &AS )
		{
			Items.plug( AS );
			Main.plug( AS );
		}
		table_ &operator =(const table_ &T)
		{
			Items = T.Items;
			Main = T.Main;

			return *this; 
		}
		void Init( void )
		{
			Items.Init();
			Main.Init();
		}
		row__ Append( const str::string_ &String );
		row__ Append( const table_ &Table );
		row__ Append( const str::strings_ &Strings );	// ATTENTION : Crée une seule entrée avec tous les strings, et pas autant d'entrée que de string.
		void GetTable(
			row__ Row, 
			table_ &Table ) const;
		E_NAVt( Main., row__ );
	};

	void GetTable(
		const _item_ &Item,
		const _items_ &Items,
		table_ &Table );

	E_CDEF(char, DefaultSeparatorToken, '|');
	E_CDEF(char, DefaultBeginToken, '(');
	E_CDEF(char, DefaultEndToken, ')');
	E_CDEF(char, DefaultEscapeToken, '\\');

	struct tokens__ {
		char
			Separator,	// Caractère séparant les différentes entrées.
			Begin,		// Caractère marquantr le début d'un gruope.
			End,		// Caractère marquant la fin d'un groupe.
			Escape;		// Caratère d'échappement.
		void reset( bso::bool__ = true )
		{
			Separator = DefaultSeparatorToken;
			Begin = DefaultBeginToken;
			End = DefaultEndToken,
			Escape = DefaultEscapeToken;
		}
		E_CDTOR( tokens__ );
		tokens__( char Entry )
		{
			reset( false );

			Init( Entry );
		}
		tokens__(
			char Entry,
			char Begin,
			char End,
			char Escape = DefaultEscapeToken )
		{
			Init( Entry, Begin, End, Escape );
		}
		void Init(
			char Entry,
			char Begin,
			char End,
			char Escape = DefaultEscapeToken )
		{
			this->Separator = Separator;
			this->Begin = Begin;
			this->End = End;
			this->Escape = Escape;
		}
		void Init( char Entry )
		{
			Init( Entry, DefaultBeginToken, DefaultEndToken );
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
		aNone,		// Pas d'autres paramètres disponible.
		aEmpty,		// Le prochain paramètre est vide.
		aString,	// Le prochain paramètre est un 'string'.
		aTable,		// Le prochain paramètre est une table.
		a_amount,
		a_Undefined
	};

	class retriever__ {
	private:
		const _items_ *_Items;
		const _irows_ *_Rows;
		row__ _Row;
		const _items_ &_I( void ) const
		{
			if ( _Items == NULL )
				ERRFwk();

			return *_Items;
		}
		const _irows_ &_R( void ) const
		{
			if ( _Rows == NULL )
				ERRFwk();

			return *_Rows;
		}
	public:
		void reset( bso::bool__ = true )
		{
			_Items = NULL;
			_Rows = NULL;
			_Row = E_NIL;
		}
		E_CDTOR( retriever__ );
		void Init(
			const _items_ &Items,
			const _irows_ &Rows )
		{
			_Items = &Items;
			_Rows = &Rows;

			_Row = Rows.First();
		}
		void Init( const table_ &Table )
		{
			Init( Table.Items, Table.Main );
		}
		void GetString( str::string_ &Target )
		{
			ctn::E_CITEMt( _item_, _irow__ ) Item;
			Item.Init( _I() );

			if ( !Item( _R()( _Row ) ).ContainsString() )
				ERRFwk();

			Target.Append(Item().String );

			_Row = _R().Next( _Row );
		}
		void GetTable( table_ &Table )
		{
			ctn::E_CITEMt( _item_, _irow__ ) Item;
			Item.Init( _I() );

			strmrg::GetTable( Item( _R()( _Row ) ), _I(), Table );

			_Row = _R().Next( _Row );
		}
		// Ne retourne que le contenu d'une seule entrée. L'entrée en cours doit être un 'string', ou une table ne contenant que des 'string's, sinon erreur.
		void GetStrings( str::strings_ &Strings );
		availability__ Availability( void ) const
		{
			availability__ Availability = a_Undefined;

			if ( _Row == E_NIL )
				Availability = aNone;
			else {
					ctn::E_CITEMt( _item_, _irow__ ) Item;
					Item.Init( _I() );

					if ( Item(_R()( _Row ) ).ContainsString() )
						if ( Item().String.Amount() == 0 )
							Availability = aEmpty;
						else
							Availability = aString;
					else
						Availability = aTable;
			}

			return Availability;
		}
	};

# endif
}

				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
