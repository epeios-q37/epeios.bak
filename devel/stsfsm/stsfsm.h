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

// State Transition System Finite State Machine .

#ifndef STSFSM_INC_
#define STSFSM_INC_

#define STSFSM_NAME		"STSFSM"

#define	STSFSM_VERSION	"$Revision: 1.1 $"

#define STSFSM_OWNER		"Claude SIMON"

#if defined( E_DEBUG ) && !defined( STSFSM_NODBG )
#define STSFSM_DBG
#endif

# include "err.h"
# include "flw.h"
# include "tol.h"
# include "sdr.h"
# include "bch.h"
# include "ctn.h"
# include "str.h"

/***************/
/***** NEW *****/
/***************/

namespace stsfsm {
	typedef bso::uint__ fId;

	qROW( CRow );	// 'Card Row'.

	typedef bch::qBUNCHdl( sCRow ) dCRows;

	// By defalt, cette valeur signifie qu'une 'card' n'est pas rattache  une entre. Dans ce cas 'Cards( 255 ) n'existe pas.
	// Si les 255 entre de la table sont utilise et pointe tous sur des 'card's inutilise, alors 'Cards(255)' est un 'row' sur une 'card' comme un autre.

	// By default, this value means that a card is not linked to another. In this case 'Cards( 255 )' doesn't exist.
	// If the 255 entries of a table are used and are all linked to unused cards, then 'Cards( 255 )' is a row pointing to a card like the others .
	qCDEF( int, NoCard, 255 );

	qCDEF( fId, UndefinedId, BSO_UINT_MAX );

	using bch::sHook;

	class dCard {
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
				qRFwk();

			S_.Table[C] = (bso::u8__)*Row;
		}
	public:
		struct s {
			fId Id;
			bso::u8__ Table[256];
			dCRows::s Cards;
		} &S_;
		dCRows Cards;
		dCard ( s &S )
		: S_( S ),
		  Cards( S.Cards )
		{}
		void reset( bso::bool__ P = true )
		{
			S_.Id = UndefinedId;
			_ResetTable();
			Cards.reset( P );
		}
		void plug( sHook &Hook )
		{
			Cards.plug( Hook );
		}
		void plug( qASd *AS )
		{
			Cards.plug( AS );
		}
		dCard &operator =( const dCard &C )
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
		sCRow Get( bso::u8__ C ) const
		{
			if ( _T( C ) == NoCard )
				if ( !Cards.Exists( NoCard ) )
					return qNIL;

			return Cards( _T( C ) );
		}
		void Set(
			bso::u8__ C,
			sCRow Row )
		{
			if ( Get( C ) != qNIL )
				qRFwk();

			_SetT( C, Cards.Append( Row ) );
		}
		E_RWDISCLOSE_( fId, Id );
	};

	typedef ctn::qMCONTAINERd( dCard, sCRow ) dAutomat;
	typedef crt::qCMITEMs( dCard, sCRow ) sCard_;

	qW( Automat );

	using bch::rRH;

	using bch::rHF;
	using bch::rFH;
}

/***************/
/***** OLD *****/
/***************/

namespace stsfsm {

	typedef sCRow crow__;

	typedef dCRows crows_;

	typedef fId id__;

	typedef dCard card_;
	E_AUTO(  card  );

	typedef dAutomat automat_;
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
		crow__ Current_;
		qCRMV( automat_, A_, Automat_ );
		status__ _Handle(
			const str::string_ &Pattern,
			sdr::row__ *LostPosition );
	public:
		void reset( bso::bool__ = true )
		{
			Automat_ = NULL;
			Current_ = qNIL;
		}
		E_CVDTOR( parser__ );
		void Init( const automat_ &Automat )
		{
			Automat_ = &Automat;
			Current_ = qNIL;
		}
		status__ Handle( bso::u8__ C );
		status__ Handle( const str::string_ &Pattern )
		{
			return _Handle( Pattern, NULL );
		}
		status__ Handle(
			const str::string_ &Pattern,
			sdr::row__ &LostPosition )	// 'LostPosition' contient la position du caractre fautif lorsque la valeur retourne == 'sLost'.
		{
			return _Handle( Pattern, &LostPosition);
		}
		id__ GetId( void ) const
		{
			sCard_ Card;

			Card.Init( A_() );

			return Card( Current_ ).GetId();
		}
	};

	template <typename type> inline void Fill(
		automat_ &Automat,
		type Amount,
		const str::dString &(* GetLabel)( type, str::dString & ) )
	{
	qRH
		int i = 0;
		str::wString Label;
	qRB
		while ( i < Amount ) {
			Label.Init();

			if ( Add( GetLabel( (type)i, Label ), i, Automat ) != UndefinedId )
				qRFwk();

			i++;
		}
	qRR
	qRT
	qRE
	}

	template <typename type> inline void Fill(
		automat_ &Automat,
		type Amount,
		const char *(* GetLabel)( type ) )
	{
		int i = 0;

		while ( i < Amount ) {
			if ( Add( GetLabel( (type)i ), i, Automat ) != UndefinedId )
				qRFwk();

			i++;
		}
	}

	template <typename type, typename version> inline void Fill(
		version Version,
		automat_ &Automat,
		type Amount,
		const char *(* GetLabel)( type, version ) )
	{
		int i = 0;

		while ( i < Amount ) {
			if ( Add( GetLabel( (type)i, Version ), i, Automat ) != UndefinedId )
				qRFwk();

			i++;
		}
	}

	// What to do with remaining data when no matching found.
	qENUM( UnmatchingBehavior ) {
		ubPurge,	// Purge remainig data (until EOF or '\0').
		ubKeep,		// Keep remaining data.
		ub_amount,
		ub_Undefined,
		ub_Default = ubKeep,
	};

	id__ GetId(
		flw::iflow__ &Flow,
		const automat_ &Automat,
		eUnmatchingBehavior UnmatchingBehavior );

	template <typename type> inline type Normalize_(
		id__ Id,
		bso::uint__ Amount,
		type UndefinedValue,
		qRPN )
	{
		if ( Id == UndefinedId ) {
			if ( qRPT )
				qRFwk();
			else
				return UndefinedValue;
		} else if ( Id >= (id__)Amount )
			qRFwk();
		else
			return (type)Id;

		return UndefinedValue;	// Pour viter un 'warning'.
	}

	template <typename type> inline type GetId(
		flw::iflow__ &Flow,
		const automat_ &Automat,
		type UndefinedValue,
		bso::uint__ Amount,
		eUnmatchingBehavior UnmatchingBehavior = ub_Default,
		qRPD )
	{
		return Normalize_<type>( GetId( Flow, Automat, UnmatchingBehavior ), Amount, UndefinedValue, qRP );
	}

	id__ GetId(
		const str::string_ &Pattern,
		const automat_ &Automat );

	template <typename type> inline type GetId(
		const str::string_ &Pattern,
		const automat_ &Automat,
		type UndefinedValue,
		bso::uint__ Amount,
		qRPD )
	{
		return Normalize_<type>( GetId( Pattern, Automat ), Amount, UndefinedValue, qRP );
	}
}

#endif
