/*
	Header for the 'stsfsm' library by Claude SIMON (csimon at zeusw dot org)
	Copyright (C) 20132004 Claude SIMON.

	This file is part of the Epeios (http://zeusw.org/epeios/) project.

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
	of the License, or (at your option) any later version.
 
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, go to http://www.fsf.org/
	or write to the:
  
         	         Free Software Foundation, Inc.,
           59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

//	$Id: stsfsm.h,v 1.1 2013/07/18 19:46:25 csimon Exp $

#ifndef STSFSM__INC
#define STSFSM__INC

#define STSFSM_NAME		"STSFSM"

#define	STSFSM_VERSION	"$Revision: 1.1 $"

#define STSFSM_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &STSFSMTutor;

#if defined( E_DEBUG ) && !defined( STSFSM_NODBG )
#define STSFSM_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.1 $
//C Claude SIMON (csimon at zeusw dot org)
//R $Date: 2013/07/18 19:46:25 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D State Transition System Finite State Machine 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

# include "err.h"
# include "flw.h"
# include "tol.h"
# include "sdr.h"
# include "bch.h"
# include "ctn.h"
# include "str.h"

namespace stsfsm {

	E_ROW( crow__ );	// 'Card Row'.

	typedef bch::E_BUNCH_( crow__ ) crows_;

	typedef bso::uint__ id__;
	E_CDEF( id__, UndefinedId, BSO_UINT_MAX );
	E_CDEF( id__, IdMax, UndefinedId - 1 );

	// Par défaut, cette valeur signifie qu'une 'card' n'est pas rattachée à une entrée. Dans ce cas 'Cards( 255 ) n'existe pas.
	// Si les 255 entrée de la table sont utilisée et pointe tous sur des 'card's inutilisée, alors 'Cards(255)' est un 'row' sur une 'card' comme un autre.
	E_CDEF( int, NoCard, 255 );

	class card_ {
	private:
		void _ResetTable( void )
		{
			memset( S_.Table, NoCard, sizeof( S_.Table ) );
		}
		bso::u8__ _T( bso::u8__ C ) const
		{
			return S_.Table[C ];
		}
		void _SetT(
			bso::u8__ C,
			sdr::row__ Row )
		{
			if ( *Row > BSO_U8_MAX )
				ERRFwk();

			S_.Table[C] = (bso::u8__)*Row;
		}
	public:
		struct s {
			id__ Id;
			bso::u8__ Table[256];
			crows_::s Cards;
		} &S_;
		crows_ Cards;
		card_ ( s &S )
		: S_( S ),
		  Cards( S.Cards )
		{}
		void reset( bso::bool__ P = true )
		{
			S_.Id = UndefinedId;
			_ResetTable();
			Cards.reset( P );
		}
		void plug( sdr::E_SDRIVER__ &SD )
		{
			Cards.plug( SD );
		}
		void plug( ags::E_ASTORAGE_ &AS )
		{
			Cards.plug( AS );
		}
		card_ &operator =( const card_ &C )
		{
			S_.Id = C.S_.Id;
			memcpy( S_.Table, C.S_.Table, sizeof( S_.Table ) );
			Cards = C.Cards;

			return *this;
		}
		void Init( void )
		{
			S_.Id = UndefinedId;
			_ResetTable();
			Cards.Init();
		}
		crow__ Get( bso::u8__ C ) const
		{
			if ( _T( C ) == NoCard )
				if ( !Cards.Exists( NoCard ) )
					return E_NIL;

			return Cards( _T( C ) );
		}
		void Set(
			bso::u8__ C,
			crow__ Row )
		{
			if ( Get( C ) != E_NIL )
				ERRFwk();

			_SetT( C, Cards.Append( Row ) );
		}
		E_RWDISCLOSE_( id__, Id );
	};

	typedef ctn::E_MCONTAINERt_( card_, crow__ ) automat_;
	E_AUTO( automat );

	id__ Add(
		const str::string_ &Tag,
		id__ Id,
		automat_ &Automat );

	inline id__ Add(
		const char *Tag,
		id__ Id,
		automat_ &Automat )
	{
		return Add( str::string( Tag ), Id, Automat );
	}

	inline id__ Add(
		const char *Tag,
		sdr::size__ Length,
		id__ Id,
		automat_ &Automat )
	{
		return Add( str::string( Tag, Length ), Id, Automat );
	}


	enum status__ {
		sPending,
		sMatch,
		sLost,
		s_amount,
		s_Undefined
	};

	class parser__ {
	private:
		const automat_ *_Automat;
		crow__ _Current;
		const automat_ &_A( void ) const
		{
			if ( _Automat == NULL )
				ERRFwk();

			return *_Automat;
		}
	public:
		void reset( bso::bool__ = true )
		{
			_Automat = NULL;
			_Current = E_NIL;
		}
		E_CVDTOR( parser__ );
		void Init( const automat_ &Automat )
		{
			_Automat = &Automat;
			_Current = E_NIL;
		}
		status__ Handle( bso::u8__ C );
		status__ Handle(
			const str::string_ &Pattern,
			sdr::row__ &LostPosition = *(sdr::row__ *)NULL );	// 'LostPosition' contient la position du caractère fautif lorsque la valeur retournée == 'sLost'.
		id__ GetId( void ) const
		{
			ctn::E_CMITEMt( card_, crow__ ) Card;
			
			if ( _Current == E_NIL )
				ERRFwk();

			Card.Init( _A() );

			return Card( _Current ).GetId();
		}
	};

	template <typename type> inline void FillAutomat(
		automat_ &Automat,
		type Amount,
		const char *(* GetLabel)( type ) )
	{
		int i = 0;

		while ( i < Amount ) {
			if ( Add( GetLabel( (type)i ), i, Automat ) != UndefinedId )
				ERRFwk();

			i++;
		}
	}

	template <typename type, typename version> inline void FillAutomat(
		version Version,
		automat_ &Automat,
		type Amount,
		const char *(* GetLabel)( version, type ) )
	{
		int i = 0;

		while ( i < Amount ) {
			if ( Add( GetLabel( Version, (type)i ), i, Automat ) != UndefinedId )
				ERRFwk();

			i++;
		}
	}

	id__ GetId_(
		const str::string_ &Pattern,
		const automat_ &Automat );

	template <typename type> inline type GetId(
		const str::string_ &Pattern,
		const automat_ &Automat,
		type UndefinedValue,
		bso::uint__ Amount )
	{
		id__ Id = GetId_( Pattern, Automat );

		if ( Id == UndefinedId )
			return UndefinedValue;
		else if ( Id >= (id__)Amount )
			ERRFwk();
		else
			return (type)Id;

		return UndefinedValue;	// Pour éviter un 'warning'.
	}
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
