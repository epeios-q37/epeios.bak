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

// LOGging

#ifndef LOGQ_INC_
# define LOGQ_INC_

# define LOGQ_NAME		"LOGQ"

# if defined( E_DEBUG ) && !defined( LOGQ_NODBG )
#  define LOGQ_DBG
# endif

# include "err.h"
# include "fdr.h"
# include "ltf.h"

# define LOGQ_DEFAULT_SIZE	1000

namespace logq {

	typedef fdr::oflow_driver_base___ rFDRiver_;

	template <int size = LOGQ_DEFAULT_SIZE> class rFDriver
	: public rFDRiver_
	{
	private:
		ltf::line_text_oflow___<> LTFlow_;
		tol::bDate Date_;
		tol::bDate Time_;
		bso::sByte Message_[size+1];
		bso::sBool Changed_;
		bso::sSize Pos_, PreviousPos_;
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
		virtual bso::sBool FDRCommit(
			bso::sBool Unlock,
			qRPN ) override
		{
			tol::bDate Date;
			tol::bTime Time;

			if ( Pos_ != 0 ) {

				LTFlow_.CR( true );

				if ( Pos_ != PreviousPos_ )
					Changed_ = true;

				if ( Changed_ ) {
					LTFlow_ << txf::nl;
					if ( !LTFlow_.Commit( Unlock, ErrHandling ) )
						return false;
				}

				LTFlow_ << '(';

				tol::Date( Date );

# if 0	// For testing datre change
				if ( ( tol::EpochTime( false ) / 10 ) % 2 )
					Date[2] = '-';
# endif

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

				PreviousPos_ = Pos_;

				Pos_ = 0;

				return LTFlow_.Commit( Unlock, ErrHandling );
			} else
				return true;
		}
		virtual fdr::sTID FDRWTake( fdr::sTID Owner ) override
		{
			return LTFlow_.Flow().WDriver().WTake( Owner );
		}
	public:
		void reset( bso::sBool P = true )
		{
			rFDRiver_::reset( P );
			tol::reset( P, LTFlow_, Changed_ );
			Message_[0] = 0;
			Pos_ = PreviousPos_ = 0;
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

	typedef txf::text_oflow__ rTFlow_;

	class rLog
	: public rTFlow_
	{
	private:
		rFDriver<> Driver_;
		flw::rDressedWFlow<> OFlow_;
	public:
		void reset( bso::sBool P = true )
		{
			tol::reset( P, OFlow_, Driver_ );
			rTFlow_::reset( P );
		}
		qCDTOR( rLog );
		void Init( txf::text_oflow__ &Flow )
		{
			Driver_.Init( Flow );
			OFlow_.Init( Driver_ );
			rTFlow_::Init( OFlow_ );
		}
	};

	template <int size = LOGQ_DEFAULT_SIZE> class rLogRack
	: public rTFlow_
	{
	private:
		flw::rDressedWFlow<> OFlow_;
	public:
		void reset( bso::sBool P = true )
		{
			rTFlow_::reset( P );
			tol::reset( P, OFlow_ );
		}
		qCDTOR( rLogRack );
		void Init( rFDriver<size> &Driver  )
		{
			OFlow_.Init( Driver );
			rTFlow_::Init( OFlow_ );
		}
	};
}

#endif
