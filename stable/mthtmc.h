/*
	Copyright (C) 2000-2015 Claude SIMON (http://q37.info/contact/).

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

//	$Id: mthtmc.h,v 1.1 2010/07/15 18:26:44 csimon Exp $

#ifndef MTHTMC__INC
#define MTHTMC__INC

#define MTHTMC_NAME		"MTHTMC"

#define	MTHTMC_VERSION	"$Revision: 1.1 $"

#define MTHTMC_OWNER		"Claude SIMON"

#if defined( E_DEBUG ) && !defined( MTHTMC_NODBG )
#define MTHTMC_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.1 $
//C Claude SIMON (csimon at zeusw dot org)
//R $Date: 2010/07/15 18:26:44 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D MaTH TiMeCode 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"
#include "tol.h"
#include "str.h"
#include "mthitg.h"
#include "mthfrc.h"

namespace mthtmc {
	typedef mthitg::integer_ frame_count_;
	typedef mthitg::integer frame_count;

	using mthfrc::fraction_;
	using mthfrc::fraction;

	typedef fraction_ period_;
	typedef fraction period;

	typedef period_ timecode_;
	typedef period timecode;


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

	class xfps_	// Extended fps.
	{
	public:
		struct s {
			frame_count_::s FPS;
			type__ Type;
		} &S_;
		frame_count_ FPS;
		xfps_( s &S )
		: S_( S ),
		  FPS( S.FPS )
		{}
		void reset( bso::bool__ P = true )
		{
			FPS.reset( P );

			S_.Type = t_Undefined;
		}
		void plug( qAS_ &AS )
		{
			FPS.plug( AS );
		}
		xfps_ &operator =( const xfps_ &XFPS )
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
		void Init( const xfps_ &XFPS )
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

	E_AUTO( xfps );

	inline void Convert(
		const xfps_ &XFPS,
		period_ &Period )
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
			Simplify( Period );
			break;
		default:
			qRFwk();
			break;
		}
	}

	inline void Convert(
		const frame_count_ &FrameCount,
		const period_ &Period,
		timecode_ &Timecode )
	{
		Timecode.Init( FrameCount * Period.N, Period.D );

		mthfrc::Simplify( Timecode );
	}

	void Convert(
		const frame_count_ &FrameCount,
		const xfps_ &XFPS,
		timecode_ &Timecode );

	typedef bso::u32__ base__;

	void Convert(
		base__ D,
		base__ H,
		base__ M,
		base__ S,
		const frame_count_ &F,
		const xfps_ &XFPS,
		timecode_ &Timecode );

	inline void Convert(
		base__ D,
		base__ H,
		base__ M,
		base__ S,
		base__ F,
		const xfps_ &XFPS,
		timecode_ &Timecode )
	{
		Convert( D, H, M, S, frame_count( F ), XFPS, Timecode );
	}

	bso::bool__ Convert(
		const str::string_ &String,
		sdr::row__ &Position,
		const xfps_ &DefaultXFPS,
		timecode_ &Timecode );	// If the 'XFPS' is bot stored in 'String', then 'DefaultXFPS' is used.

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
		const timecode_ &Timecode,
		const xfps_ &XFPS,
		const str::string_ &Format,
		const tokens__ &Tokens,
		txf::text_oflow__ &TFlow );

	 inline void Add(
		const timecode_ &Timecode1,
		const timecode_ &Timecode2,
		timecode_ &Result )
	 {
		 Result = Timecode1 + Timecode2;
	 }
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
