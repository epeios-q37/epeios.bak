/*
	Copyright (C) 2016-2017 Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of 'MMUAq' software.

    'MUAq' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'MUAq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'MUAq'.  If not, see <http://www.gnu.org/licenses/>.
*/

// MUA Post Office Protocol 3 (POP3 - RFC 1939).

#ifndef MUAPO3_INC_
# define MUAPO3_INC_

# ifdef XXX_DBG
#  define MUAXMP__DBG
# endif

# include "muabsc.h"

# include "crt.h"
# include "lstcrt.h"

namespace muapo3 {
	typedef fdr::rRDressedDriver rRDressedDriver_;

	// For a given state, the next state can only be 's_Regular', or the immediatly following one (except for the last one).
	qENUM( State_ ) {
		sRegular,	// We are not in a potential termination sequence.
		sICR,	// Initial CR.
		sRILF,	// Regular Initial LF.
		sBILF,	// LF right before the begin of the message.
		sDot,
		sFCR,	// Final CR.
		sFLF,	// Final LF.
		sCompleted = sFLF,
		s_amount,
		s_Undefined
	};

	qCDEF( bso::sU8, LF_, 10 );
	qCDEF( bso::sU8, CR_, 13 );
	qCDEF( bso::sU8, Dot_, 46 );

	class rSequenceDelimitedRDriver_
	: public rRDressedDriver_
	{
	private:
		eState_ State_;
		flw::rDressedRFlow<> Flow_;
		bso::sBool MultiLine_;
		fdr::sByte StateSize_( void ) const
		{
			switch ( State_ ) {
			case sRegular:
				return 0;
				break;
			case sICR:
				return 1;
				break;
			case sRILF:
				return 2;
				break;
			case sBILF:	// LF (and CR) from the answer ; should not be displayed.
				return 0;
				break;
			case sDot:
				return 2;	// Not 3, because if the '.' will not be dumped.
				break;
			case sFCR:
				return 3;
				break;
			case sFLF:
				qRGnr();	// Should never occur.
				break;
			default:
				qRGnr();
				break;
			}

			return 0;	// To avoid a warning.
		}
		bso::sBool HandleState_(
			fdr::sByte C,
			fdr::sByte Awaited,
			eState_ Next )
		{
			if ( C == Awaited ) {
				State_ = Next;
				return true;
			} else {
				return false;
			}
		}
		/// If returned value == 'true', we are in a potential termination sequance.
		bso::sBool HandleState_( fdr::sByte C )
		{
			bso::sBool Termination = true;

			switch ( State_ ) {
			case sRegular:
				Termination = HandleState_( C, CR_, sICR );
				break;
			case sICR:
				if ( MultiLine_ )
					Termination = HandleState_( C, LF_, sRILF );
				else {
					State_ = sCompleted;
					Termination = true;
				}
				break;
			case sRILF:
			case sBILF:
				Termination = HandleState_( C, Dot_, sDot );
				break;
			case sDot:
				Termination = HandleState_( C, CR_, sFCR );
				break;
			case sFCR:
				Termination = HandleState_( C, LF_, sFLF );
				break;
			case sFLF:
				qRGnr();	// Should never occur.
				break;
			default:
				qRGnr();
				break;
			}

			return Termination;
		}
		bso::sByte DumpState_( fdr::sByte *Buffer )
		{
			// If 'State_' at least 'CFLF.' but not 'CRLF.CRLF', the '.' must be removed.
			const fdr::sByte Sequence[] = { CR_, LF_, CR_ };
			bso::sByte Index = 0;

			if ( State_ == sRegular )
				qRGnr();

			Index = StateSize_();

			memcpy( Buffer, Sequence, Index );

			return Index;
		}
	protected:
		virtual fdr::size__ FDRRead(
			fdr::size__ Maximum,
			fdr::byte__ *Buffer ) override
		{
			fdr::sSize Amount = 0;
			fdr::sByte C = 0;
			bso::sBool HandleAgain = false;	// To indicate that the current char must be handled again, and not the next one.

			if ( State_ == sCompleted )
				return 0;

			if ( Flow_.EndOfFlow() )
				qRGnr();

			if ( Maximum < 5 )
				qRGnr();

			while ( ( ( Amount + StateSize_() ) < Maximum ) && ( ( Amount == 0 ) ||  !Flow_.IsCacheEmpty() ) ) {
				if ( HandleAgain )
					HandleAgain = false;
				else
					C = Flow_.Get();

				if ( !HandleState_( C ) ) {	// No (more) potential termination sequence detected.
					if ( State_ == sRegular ) {
						Buffer[Amount++] = C;
					} else { // A potential termination sequence was detected.
						Amount += DumpState_( Buffer + Amount );

						if ( State_ != sDot )	// If we are in the dot part of the termination sequence, the dot is skipped,
							HandleAgain = true;

						State_ = sRegular;
					}
				} else if ( State_ == sCompleted ) {
					// It doesn't matter if 'Amount' == 0 : 'EOF' is only sent now instead on the next call.
					break;
				}
			}

			return Amount;
		}
		virtual bso::sBool FDRDismiss(
			bso::sBool Unlock,
			qRPN) override
		{
			return Flow_.Dismiss(Unlock, qRP);
		}
		virtual fdr::sTID FDRRTake( fdr::sTID Owner ) override
		{
			return Flow_.RDriver().RTake( Owner );
		}
	public:
		void reset( bso::sBool P = true )
		{
			rRDressedDriver_::reset( P );
			tol::reset( P, Flow_ );
			State_ = s_Undefined;
			MultiLine_ = false;
		}
		qCVDTOR( rSequenceDelimitedRDriver_ );
		void Init(
			bso::sBool EOL,
			fdr::rRDriver &Driver,
			bso::sBool MultiLine,
			fdr::thread_safety__ ThreadSafety )
		{
			rRDressedDriver_::Init( ThreadSafety );
			Flow_.Init( Driver );
			MultiLine_ = MultiLine;
			if ( EOL  )
				State_ = sBILF;
			else
				State_ = sRegular;
		}
	};

	class hBody
	{
	private:
		rSequenceDelimitedRDriver_ Driver_;
	public:
		void reset( bso::sBool P = true )
		{
			tol::reset( P, Driver_ );
		}
		qCDTOR( hBody );
		void Init(
			bso::sBool EOL,
			fdr::rRDriver &Driver,
			bso::sBool Multiline )
		{
			Driver_.Init( EOL, Driver, Multiline, fdr::ts_Default );
		}
		fdr::rRDriver &GetDriver( void )
		{
			return Driver_;
		}
	};

	typedef bso::sUInt sNumber;	// POP3 message-number.

	namespace base {
		qENUM( Indicator_ ) {
			iOK,
			i_True = iOK,
			iError,
			i_False = iError,
			i_Error,
			iErroneous = i_Error,	// Server returned a not 'POP3' compliant answer.
			i_amount,
			i_Undefined
		};

		qXENUM( Indicator, i );

		// To launch before any of the operation below. Eats the message sent on connection.
		eIndicator Authenticate(
			const str::dString &Username,
			const str::dString &Password,
			fdr::rRWDriver &Server,
			hBody & Body );

		eIndicator List(
			sNumber Number,
			fdr::rRWDriver &Server,
			bso::sBool SkipAnswer,
			hBody & Body );

		eIndicator Retrieve(
			sNumber Number,
			fdr::rRWDriver &Server,
			bso::sBool SkipAnswer,
			hBody & Body );

		eIndicator Top(
			sNumber Number,
			bso::sUInt AmoutOfLine,
			fdr::rRWDriver &Server,
			bso::sBool SkipAnswer,
			hBody & Body );

		eIndicator UIDL(
			sNumber Number,	// If == 0, give all the messages.
			fdr::rRWDriver &Server,
			hBody & Body );

		eIndicator Quit(
			fdr::rRWDriver &Server,
			hBody & Body );
	}

	/*
	First call 'Authenticate', then you can call several
	of the following functions one after the other. Call
	'Quit()' as the last one.
	*/

	bso::sBool Authenticate(
		const str::dString &Username,
		const str::dString &Password,
		fdr::rRWDriver &Server,
		qRPD );

	bso::sBool Quit(
		fdr::rRWDriver &Server,
		qRPD );

	bso::sBool GetIndexes(
		fdr::rRWDriver &Server,
		muabsc::cIndex &Callback,
		qRPD );

	inline bso::sBool GetIndexes(
		fdr::rRWDriver &Server,
		muabsc::dIndexes &Indexes,
		qRPD )
	{
		muabsc::sDefaultIndexCallback Callback;

		Callback.Init( Indexes );

		return GetIndexes( Server, Callback, qRP );
	}

	bso::sBool GetHeader(
		sNumber Number,
		fdr::rRWDriver &Server,
		hBody &Body,
		qRPD );


	bso::sBool GetMessage(
		sNumber Number,
		fdr::rRWDriver &Server,
		hBody &Body,
		qRPD );

	bso::sBool GetMessage(
		sNumber Number,
		fdr::rRWDriver &Server,
		str::dString &Body,
		qRPD );

	qSTRING( UIDL );
	qSTRINGSl( UIDLs );

	class cUIDL {
	protected:
		virtual void MUAPO3OnUIDL(
			sNumber Number,
			const dUIDL &UIDL ) = 0;
	public:
		qCALLBACK( UIDL );
		void OnUIDL(
			sNumber Number,
			const dUIDL &UIDL )
		{
			return MUAPO3OnUIDL( Number, UIDL );
		}
	};

	bso::sBool GetUIDLs(
		fdr::rRWDriver &Server,
		cUIDL &Callback,
		qRPD );


	typedef bch::qBUNCHdl( sNumber ) dNumbers;
	qW( Numbers );

	class sDefaultUIDLCallback
	: public cUIDL
	{
	private:
		qRMV( dNumbers, N_, Numbers_ );
		qRMV( dUIDLs, U_, UIDLs_ );
	protected:
		virtual void MUAPO3OnUIDL(
			sNumber Number,
			const dUIDL &UIDL ) override
		{
			if ( N_().Add( Number) != U_().Add( UIDL ) )
				qRGnr();
		}
	public:
		void reset( bso::sBool P = true )
		{
			tol::reset( P, Numbers_, UIDLs_ );
		}
		qCVDTOR( sDefaultUIDLCallback );
		void Init(
			dNumbers &Numbers,
			dUIDLs &UIDLs )
		{
			Numbers_ = &Numbers;
			UIDLs_ = &UIDLs;
		}
	};

	inline bso::sBool GetUIDLs(
		fdr::rRWDriver &Server,
		dNumbers &Numbers,
		dUIDLs &UIDLs,
		qRPD )
	{
		sDefaultUIDLCallback Callback;

		Callback.Init( Numbers, UIDLs );

		return GetUIDLs( Server, Callback, qRP );
	}

	// If 0 is retuned, no corresponding message found.
	sNumber GetNumberForUIDL(
		const dUIDL &UIDL,
		fdr::rRWDriver &Server );
}


#endif
