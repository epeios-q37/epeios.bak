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

//D DaTE 

#ifndef DTE_INC_
# define DTE_INC_

# define DTE_NAME		"DTE"

# define	DTE_VERSION	"$Revision: 1.30 $"

# define DTE_OWNER		"Claude SIMON (http://zeusw.org/intl/contact.html)"

# if defined( E_DEBUG ) && !defined( DTE_NODBG )
#  define DTE_DBG
# endif

# include "err.h"
# include "flw.h"
# include "bso.h"
# include "txf.h"

//d An invalid date.
# define DTE_INVALID_DATE	0

/*d Decennia under which we consider we are in the XXI century,
and over which we consider we are in the XX century. */
# define DTE_DEFAULT_DECENNIA_LIMIT	90

# define DTE_CORE_SHIFT	5

# define DTE_SIGN_BIT_POSITION	31

# define DTE_SIGN_MASK	( 1 << DTE_SIGN_BIT_POSITION )

# define DTE_CORE_MASK	( ( (raw_date__)~0 << DTE_CORE_SHIFT ) & ( ~DTE_SIGN_MASK ) )


namespace dte {
	//t Type of a raw date.
	typedef bso::u32__ raw_date__;

	//t Type for the year.
	typedef bso::u16__	year__;

	//t Type for the month.
	typedef bso::u8__	month__;

	//t Type of the day.
	typedef bso::u8__ day__;

	typedef char buffer__[11];

	enum format__ {
		fDDMMYYYY,	// ('25/11/2003').
		fMMDDYYYY,	// ('11/25/2003').
		fYYYYMMDD,	// ('2003/25/11').
		f_amount,
		f_Undefined
	};

	inline const char *GetLabel( format__ Format )
	{
		switch ( Format ) {
		case fDDMMYYYY:
			return "DDMMYYYY";
			break;
		case fMMDDYYYY:
			return "MMDDYYYY";
			break;
		case fYYYYMMDD:
			return "YYYYMMDD";
			break;
		default:
			qRFwk();
			return NULL;	// Pour viter un warning.
			break;
		}
	};

	inline bso::bool__ _Compare(
		format__ Format,
		const char *Label )
	{
		return strcmp( GetLabel( Format ), Label ) == 0;
	}

	inline format__ GetFormat( const char *Label )
	{
		if ( _Compare( fDDMMYYYY, Label ) )
			return fDDMMYYYY;
		else if ( _Compare( fMMDDYYYY, Label ) )
			return fDDMMYYYY;
		else if ( _Compare( fYYYYMMDD, Label ) )
			return fYYYYMMDD;
		else
			return f_Undefined;
	}

	//c A date.
	class date__ {
		// The raw date. See '.cpp' for structure.
		raw_date__ Raw_;
		/* Convert 'Date' (in 'yyyymmdd' format) into internal format.
		Return 'STXDTE_INVALID_DATE' if date not valid, or the converted date */
		raw_date__ _Convert(
			day__ Day,
			month__ Month,
			year__ Year ) const;
		/* Covert Date (in "dd.mm.yyyy" ) format into internal format.
		Return 'STXDTE_INVALID_DATE' if date not valid, or the converted date */
		raw_date__ _Convert(
			const char *&Date,
			format__ Format );
		// Return the core date.
		bso::u32__ _Core( raw_date__ Date ) const
		{
			return ( Raw_ & DTE_CORE_MASK) >> DTE_CORE_SHIFT;
		}
		// Return the year of 'RawDate'.
		year__ _Year( raw_date__ RawDate ) const
		{
			return (year__)( _Core( RawDate ) >> 9 );
		}
		// Return the month of the date.
		month__ _Month( raw_date__ RawDate ) const
		{
			return (month__)( ( _Core( RawDate ) >> 5 ) & 0xf );
		}
		// Return the day of 'RawDate'.
		day__ _Day( raw_date__ RawDate ) const
		{
			return (day__)( _Core( RawDate ) & 0x1f );
		}
		// Return true if 'Date' is a simplified raw date (yyyymmdd).
		bso::bool__ _IsSimplifiedRawDate( bso::u32__ Date ) const
		{
			return ( Date != DTE_INVALID_DATE ) && ( ( Date & DTE_SIGN_MASK ) == 0 );
		}
		// Convert a simplified raw date (yyyymmdd) to raw date.
		raw_date__ _Convert( bso::u32__ Date ) const
		{
			if ( _IsSimplifiedRawDate( Date ) )
				return _Convert( (day__)( Date % 100 ), (month__)( ( Date % 10000 ) / 100 ), (year__)( Date / 10000 ) );
			else
				return Date;
		}
	 public:
		void reset( bso::bool__ = true )
		{
			Raw_ = DTE_INVALID_DATE;
		}
		date__( bso::u32__ Date = DTE_INVALID_DATE )
		{
			reset( false );

			Raw_ = _Convert( Date );
		}
		date__(
			const char *Date,
			format__ Format,
			const char **End = NULL )	// N'est ventuellement modifi que si '*End== NULL'.
		{
			reset( false );

			Raw_ = _Convert( Date, Format );

			if ( ( End != NULL ) && ( *End == NULL ) )
				*End = Date;
		}
		date__(
			day__ Day,
			month__ Month,
			year__ Year )
		{
			reset( false );

			Raw_ = _Convert( Day, Month, Year );
		}
		//f Initialization with date 'Date'.
		void Init( bso::u32__ Date = DTE_INVALID_DATE )
		{
			reset();

			Raw_ = _Convert( Date );
		}
		//f Initialization with date 'Date'.
		bso::bool__ Init(
			const char *Date,
			format__ Format,
			const char **End = NULL )	// N'est ventuellement modifi que si '*End == NULL'.
		{
			reset();

			Raw_ = _Convert( Date, Format );

			if ( ( End != NULL ) && ( *End == NULL ) )
				*End = Date;

			return IsValid();
		}
		//f Initialization with 'Day', 'Month' and 'Year'.
		void Init( 
			day__ Day,
			month__ Month,
			year__ Year )
		{
			reset();

			Raw_ = _Convert( Day, Month, Year );
		}
		//f Return the date in raw format.
		operator raw_date__( void ) const
		{
			return Raw_;
		}
		//f Return simplified raw date 'yyyymmdd'.
		unsigned long GetSimplifiedRawDate( void ) const
		{
			return _Year( Raw_ ) * 10000 + _Month( Raw_ ) * 100 + _Day( Raw_ );
		}
		year__ Year( raw_date__ Date = DTE_INVALID_DATE ) const
		{
			return _Year( Date == DTE_INVALID_DATE ? Raw_ : Date );
		}
		month__ Month( raw_date__ Date = DTE_INVALID_DATE ) const
		{
			return _Month( Date == DTE_INVALID_DATE ? Raw_ : Date );
		}
		day__ Day( raw_date__ Date = DTE_INVALID_DATE ) const
		{
			return _Day( Date == DTE_INVALID_DATE ? Raw_ : Date );
		}
		//f Return the date in ASCII ('dd/mm/yyyy') and put in 'Result' if != 'NULL'.
		const char *ASCII(
			format__ Format,
			buffer__ &Buffer ) const;
#ifndef CPE_F_MT
		const char *ASCII( format__ Format ) const
		{
			static buffer__ Buffer;

			return ASCII( Format, Buffer );
		}
#endif
		void Date(
			day__ Day,
			month__ Month,
			year__ Year )
		{
			Raw_ = _Convert( Day, Month, Year );
		}
		//f Add 'Amount' Month.
		void AddMonth( bso::u32__ Amount = 1 )
		{
			bso::u16__ DeltaYear = (bso::u16__)( Amount / 12 );
			bso::u8__ DeltaMonth = (bso::u8__)( Amount % 12 );

			if ( ( DeltaMonth + Month() ) > 12 ) {
				DeltaYear++;
				DeltaMonth -= 12;
			}

			Raw_ = _Convert( this->Day(), (month__)( Month() + DeltaMonth ), (year__)( Year() + DeltaYear ) );
		}
		void SubMonth( bso::u32__ Amount = 1 )
		{
			bso::u16__ DeltaYear = (bso::u16__)( Amount / 12 );
			bso::u8__ DeltaMonth = (bso::u8__)( Amount % 12 );

			if ( DeltaMonth >= Month() ) {
				DeltaYear++;
				DeltaMonth -= 12;
			}

			Raw_ = _Convert( this->Day(), (month__)( Month() - DeltaMonth ), (year__)( Year() - DeltaYear ) );
		}
		//f Return true if the date is valid, false otherwise.
		bso::bool__ IsValid( void ) const
		{
			return Raw_ != DTE_INVALID_DATE;
		}
		//f Return true if date is in a leap year, false otherwise.
		bso::bool__ IsLeapYear( year__ Year = 0 ) const
		{
			if ( Year == 0 )
				Year = this->Year();
		    return ( ( ( Year % 4 ) == 0 )
					   && ( ( Year % 100 ) != 0 ) )
					 || ( ( Year % 400 ) == 0 );
		}
		day__ GetAmountOfDaysInMonth(
			month__ Month = 0,
			year__ Year = 0 ) const
		{
			if ( Month == 0 )
				Month = this->Month();

			if ( Year == 0 )
				Year = this->Year();

			switch ( Month ) {
			case 2: // Februrary
				if ( IsLeapYear( Year ) )
					return 29;
				else
					return 28;
				break;
			case 4: // April Falls Through
			case 6: // June
			case 9: // September
			case 11: // November
				return 30;
				break;
			default:
				return 31;
			}
	    } 
		void AddDay( void )
		{
			if ( Day() == GetAmountOfDaysInMonth() ) {
				Date( 1, Month(), Year() );
				AddMonth();
			} else
				Date( Day() + 1, Month(), Year() );
		}
		// Sunday == 0.
		day__ GetDayInWeek(
			day__ Day = 0,
			month__ Month = 0,
			year__ Year = 0 )
		{
			if ( Day == 0 )
				Day = this->Day();

			if ( Month == 0 )
				Month = this->Month();

			if ( Year == 0 )
				Year = this->Year();

			if ( Month < 3 ) {
				Month += 12;
				Year -= 1;
			}
			return ( ( Day + 1 + ( Month * 2 ) + (int )( ( Month + 1 ) * 3 / 5 )
				       + Year + (int)( Year / 4 ) - (int)( Year / 100)
					   + (int) ( Year / 400 ) ) % 7 );
		}
	};

	//f Return sign of 'Date1' - 'Date2'.
	inline bso::sign__ Compare(
		const date__ &Date1,
		const date__ &Date2 )
	{
		return bso::Compare( Date1, Date2 );
	}

	inline txf::text_oflow__ &operator <<(
		txf::text_oflow__ &Flow,
		date__ Date )
	{
		buffer__ Buffer;

		return Flow << Date.ASCII( fDDMMYYYY, Buffer );
	}

	date__ Now( void );
}

/*$END$*/
#endif
