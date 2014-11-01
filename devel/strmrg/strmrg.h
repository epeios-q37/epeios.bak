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
}

				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
