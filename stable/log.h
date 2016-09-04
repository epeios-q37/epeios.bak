/*
	Copyright (C) 1999-2016 Claude SIMON (http://q37.info/contact/).

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

// LOGging

#ifndef LOG__INC
# define LOG__INC

# define LOG_NAME		"LOG"

# if defined( E_DEBUG ) && !defined( LOG_NODBG )
#  define LOG_DBG
# endif

# include "err.h"

# include "ltf.h"

namespace log {

	typedef fdr::oflow_driver_base___ rFDRiver_;
	
	template <typename int size = 1000> class rFDriver
	: public rFDRiver_
	{
	private:
		ltf::line_text_oflow___<> LTFlow_;
		tol::bDate Date_;
		tol::bDate Time_;
		bso::sByte Message_[size+1];
		bso::sBool Changed_;
		bso::sSize Pos_ = 0;
		bso::sSize DuplicateTimeCount_;
	protected:
		virtual bso::sSize FDRWrite(
			const bso::sByte *Buffer,
			bso::sSize Maximum ) override
		{
			if ( ( Maximum + Pos_ ) > size )
				qRFwk();

			if ( Changed_ || memcmp( Message_ + Pos_, Buffer, Maximum ) ) {
				Changed_ = true;
				memcpy( Message_ + Pos_, Buffer, Maximum );
			}

			Pos_ += Maximum;

			return Maximum;
		}
		virtual void FDRCommit( void ) override
		{
			tol::bDate Date;
			tol::bTime Time;

			if ( Pos_ != 0 ) {

				LTFlow_.CR( true );

				if ( Changed_ ) {
					LTFlow_ << txf::nl;
					LTFlow_.Commit();
				}

				LTFlow_ << '(';

				tol::Date( Date );

				if ( ( tol::EpochTime( false ) / 10 ) % 2 )
					Date[2] = '-';

				if ( strcmp( Date, Date_ ) ) {
					strcpy( Date_, Date );
					LTFlow_ << Date << ' ';
					Changed_ = true;
				} else
					Changed_ = false;

				tol::Time( Time );

# if 0
				if ( strcmp( Time, Time_ ) ) {
					strcpy( Time_, Time );
					DuplicateTimeCount_++;
					LTFlow_ << DuplicateTimeCount_ << 'x';
				} else
					DuplicateTimeCount_ = 0;
# endif

				LTFlow_ << Time << ") ";

				LTFlow_.Freeze();

				Message_[Pos_] = 0;

				LTFlow_ << (const char *)Message_;

				Pos_ = 0;

				LTFlow_.Commit();
			}
		}
	public:
		void reset( bso::sBool P = true )
		{
			rFDRiver_::reset( P );
			tol::reset( P, LTFlow_, Changed_ );
			Message_[0] = 0;
			Pos_ = 0;
			DuplicateTimeCount_ = 0;
		}
		qCVDTOR( rFDriver );
		void Init( txf::text_oflow__ &Flow )
		{
			reset();

			LTFlow_.Init( Flow );
			rFDRiver_::Init( fdr::ts_Default );
		}
	};

	typedef txf::text_oflow__ rTOFlow_;

	class rLog
	: public rTOFlow_
	{
	private:
		rFDriver<> Driver_;
		flw::sDressedOFlow<> OFlow_;
	public:
		void reset( bso::sBool P = true )
		{
			tol::reset( P, OFlow_, Driver_ );
			rTOFlow_::reset( P );
		}
		qCDTOR( rLog );
		void Init( txf::text_oflow__ &Flow )
		{
			Driver_.Init( Flow );
			OFlow_.Init( Driver_ );
			rTOFlow_::Init( OFlow_ );
		}
	};


}

#endif
