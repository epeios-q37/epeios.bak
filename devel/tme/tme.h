/*
	Copyright (C) 1999-2017 Claude SIMON (http://q37.info/contact/).

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

#ifndef TME__INC
# define TME__INC

# define TME_NAME		"TME"

# if defined( E_DEBUG ) && !defined( TME_NODBG )
#  define TME_DBG
# endif

// TiME

# include "err.h"
# include "flw.h"
# include "tol.h"

#define TME_CORE_SHIFT	4

#define TME_CORE_MASK	( ~0 << TME_CORE_SHIFT )


namespace tme {
	typedef bso::u32__ raw_time__;

	E_CDEF( raw_time__, Undefined, bso::U32Max );

	typedef bso::u8__ hours__;
	typedef bso::u8__ minutes__;
	typedef bso::u8__ seconds__;
	typedef bso::u16__ ticks__;

	typedef char buffer__[13] ;

	enum format__ {
		fH,
		fHM,
		fHMS,
		fHMSd, // diximes de secondes (deciseconds)
		fHMSc, // centimes de secondes (centiseconds)
		fHMSm,  // millimes de secondes (milliseconds)
		f_amount,
		f_Undefined,
		f_Default = fHMS
	};

	inline raw_time__ Convert(
		hours__ Hours,
		minutes__ Minutes,
		seconds__ Seconds,
		ticks__ Ticks )
	{
		if ( ( Hours < 24 )
			&& ( Minutes < 60 )
			&& ( Seconds < 60 )
			&& ( Ticks < 1000 ) )
			return ( ( Hours * ( 60 * 60 * 1000 ) )
						+ ( Minutes * ( 60 * 1000 ) )
						+ ( Seconds * 1000 )
						+ Ticks )
						<< TME_CORE_SHIFT;
		else
			return Undefined;
	}

	raw_time__ Convert(
		const char *Time,
		const char **End = NULL );

	class time__ 
	{
	private:
		raw_time__ _Raw;
		raw_time__ _Core( void ) const
		{
			return ( _Raw & TME_CORE_MASK ) >> TME_CORE_SHIFT; 
		}
		hours__ _H( void ) const
		{
			return (hours__)( _Core() / ( 1000 * 60 * 60 ) );
		}
		minutes__ _M( void ) const
		{
			return ( _Core() / ( 1000 * 60 ) ) % 60;
		}
		seconds__ S_( void ) const
		{
			return ( _Core() / 1000 ) % 60;
		}
		ticks__ _T( void ) const // Ticks, millime de secondes.
		{
			return _Core() % 1000;
		}
		void _ASCII( buffer__ &Buffer ) const
		{
			sprintf( Buffer, "%02i:%02i:%02i:%03i", _H(), _M(), S_(), _T() );
		}
	public:
		void reset( bso::bool__ = true )
		{
			_Raw = Undefined;
		}
		E_CDTOR( time__ );
		void Init( raw_time__ Time = Undefined )
		{
			_Raw = Time;
		}
		bso::bool__ Init(
			const char *Time,
			const char **End = NULL )	// N'est ventuellement modifi que si '*End == NULL'.
		{
			_Raw = tme::Convert( Time, End );

			return IsSet();
		}
		bso::bool__ Init(
			hours__ Hours,
			minutes__ Minutes = 0,
			seconds__ Seconds = 0,
			ticks__ Ticks = 0 )
		{
			_Raw = tme::Convert( Hours, Minutes, Seconds, Ticks );

			return IsSet();
		}
		operator raw_time__( void ) const
		{
			return _Raw;
		}
		bso::bool__ IsSet( void )
		{
			return _Raw != Undefined;
		}
		const char *ASCII(
			buffer__ &Buffer, 
			format__ Format = f_Default ) const;
	};

}

#endif
