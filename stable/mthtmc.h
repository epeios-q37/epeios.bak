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

#ifndef MTHTMC__INC
# define MTHTMC__INC

# define MTHTMC_NAME		"MTHTMC"

//D MaTH TiMeCode 

# include "err.h"
# include "flw.h"
# include "tol.h"
# include "str.h"
# include "mthitg.h"
# include "mthrtn.h"

namespace mthtmc {
	typedef mthitg::integer_ frame_count_;
	typedef mthitg::integer frame_count;

	using mthrtn::dRational;
	using mthrtn::wRational;

	typedef dRational dPeriod;
	typedef wRational wPeriod;

	typedef dPeriod dTimecode;
	typedef wPeriod wTimecode;


	enum type__ {
		// 'Non drop-frame'.
		tNDF,
		// 'Drop frame'.
		tDF,
		// 'Non realtime'.
		tNRT,
		t_amount,
		t_Undefined
	};

	class dXFPS	// Extended fps.
	{
	public:
		struct s {
			frame_count_::s FPS;
			type__ Type;
		} &S_;
		frame_count_ FPS;
		dXFPS( s &S )
		: S_( S ),
		  FPS( S.FPS )
		{}
		void reset( bso::bool__ P = true )
		{
			FPS.reset( P );

			S_.Type = t_Undefined;
		}
		void plug( qASd *AS )
		{
			FPS.plug( AS );
		}
		dXFPS &operator =( const dXFPS &XFPS )
		{
			FPS = XFPS.FPS;

			S_.Type = XFPS.S_.Type;

			return *this;
		}
		void Init( void )
		{
			reset();

			FPS.Init();
		}
		void Init( const dXFPS &XFPS )
		{
			Init();

			this->operator =( XFPS );
		}
		void Init(
			const frame_count_ &FPS,
			type__ Type )
		{
			Init();

			this->FPS = FPS;
			S_.Type = Type;
		}
		void Init(
			bso::u32__ FPS,
			type__ Type )
		{
			Init( mthitg::integer( FPS ), Type );
		}
		bso::bool__ Init(
			const str::string_ &String,
			sdr::row__ &Position );	// 'Position' is set to the position of first unhandled character, or 'NONE' if all characters handled.
		bso::bool__ Init( const str::string_ &String )
		{
			sdr::row__ Position = String.First();

			return Init( String, Position );
		}
		E_RODISCLOSE_( type__ , Type );
		bso::bool__ IsValid( void ) const
		{
			return FPS.IsValid();
		}
	};

	qW( XFPS );

	inline void Convert(
		const dXFPS &XFPS,
		dPeriod &Period )
	{
		switch( XFPS.GetType() ) {
		case tNDF:
			Period.Init( mthitg::integer( 1 ), XFPS.FPS );
			break;
		case tDF:
			if ( ( XFPS.FPS != mthitg::integer( 30 ) ) && ( XFPS.FPS != mthitg::integer( 60 ) ) ) {
				qRFwk();
				break;
			}
		case tNRT:
			Period.Init( mthitg::integer( 1001 ), XFPS.FPS * mthitg::integer( 1000 ) );
			Normalize( Period );
			break;
		default:
			qRFwk();
			break;
		}
	}

	inline void Convert(
		const frame_count_ &FrameCount,
		const dPeriod &Period,
		dTimecode &Timecode )
	{
		Timecode.Init( FrameCount * Period.N, Period.D );

		mthrtn::Normalize( Timecode );
	}

	void Convert(
		const frame_count_ &FrameCount,
		const dXFPS &XFPS,
		dTimecode &Timecode );

	typedef bso::u32__ base__;

	void Convert(
		base__ D,
		base__ H,
		base__ M,
		base__ S,
		const frame_count_ &F,
		const dXFPS &XFPS,
		dTimecode &Timecode );

	inline void Convert(
		base__ D,
		base__ H,
		base__ M,
		base__ S,
		base__ F,
		const dXFPS &XFPS,
		dTimecode &Timecode )
	{
		Convert( D, H, M, S, frame_count( F ), XFPS, Timecode );
	}

	bso::bool__ Convert(
		const str::string_ &String,
		sdr::row__ &Position,
		const dXFPS &DefaultXFPS,
		dTimecode &Timecode );	// If the 'XFPS' is bot stored in 'String', then 'DefaultXFPS' is used.

	struct tokens__ {
		char
			DS,	// Decimal separator
			NDF,
			NRT,
			DF;
		void reset( bso::bool__ = true )
		{
			DS = NDF = NRT = DF = 0;
		}
		E_CDTOR( tokens__ );
		tokens__(
			char DS,
			char DF,
			char NRT = 0,
			char NDF = 0 )
		{
			reset( false );

			Init( DS, DF, NRT, NDF );
		}
		void Init(
			char DS,
			char DF,
			char NRT = 0,
			char NDF = 0 )
		{
			this->DS = DS;
			this->DF = DF;
			this->NRT = ( NRT == 0 ? this->DF : NRT );
			this->NDF = ( NDF == 0 ? this->NRT : NDF );
		}
		void Init( void )
		{
			DS = NDF = NRT = DF = 0;
		}
	};



	//f Return a string representation of 'TCI' following 'Standard' and 'Format'.
	bso::bool__ Display(
		const dTimecode &Timecode,
		const dXFPS &XFPS,
		const str::string_ &Format,
		const tokens__ &Tokens,
		txf::text_oflow__ &TFlow );

	 inline void Add(
		const dTimecode &Timecode1,
		const dTimecode &Timecode2,
		dTimecode &Result )
	 {
		 Result = Timecode1 + Timecode2;
	 }
}

/*$END$*/
#endif
