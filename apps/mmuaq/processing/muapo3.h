/*
	Copyright (C) 2016 Claude SIMON (http://zeusw.org/epeios/contact.html).

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

// MUA Post Office protocole 3 (POP3).

#ifndef MUAXMP__INC
# define MUAXMP__INC

# ifdef XXX_DBG
#	define MUAXMP__DBG
# endif

# include "muabsc.h"

# include "htp.h"

namespace muapo3 {

	typedef fdr::rIDressedDriver rIDressedDriver_;

	// For a given state, the next state can only be 's_Regular', or the immediatly following one (except for the last one).
	qENUM( State ) {
		sRegular,	// We are not in a potential termination sequence.
		sICR,	// Initial CR.
		sILF,	// Initial LF.
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

	class rSequenceDelimitedIDriver
	: public rIDressedDriver_
	{
	private:
		eState State_;
		flw::sIFlow Flow_;
		bso::sBool MultiLine_;
		// If returned value == 'true', we are in a potential termination sequen ce.
		fdr::sByte StateSize_( void ) const
		{
			switch ( State_ ) {
			case sRegular:
				return 0;
				break;
			case sICR:
				return 1;
				break;
			case sILF:
				return 2;
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
			eState Next )
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
					Termination = HandleState_( C, LF_, sILF );
				else {
					State_ = sCompleted;
					Termination = true;
				}
				break;
			case sILF:
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
		virtual void FDRDismiss( bso::sBool Unlock ) override
		{
			Flow_.Dismiss( Unlock );
		}
		virtual fdr::sTID FDRITake( fdr::sTID Owner ) override
		{
			return Flow_.IDriver().ITake( Owner );
		}
	public:
		void reset( bso::sBool P = true )
		{
			rIDressedDriver_::reset( P );
			tol::reset( P, Flow_ );
			State_ = s_Undefined;
			MultiLine_ = false;
		}
		qCVDTOR( rSequenceDelimitedIDriver );
		void Init(
			fdr::rIDriver &Driver,
			bso::sBool MultiLine,
			fdr::thread_safety__ ThreadSafety )
		{
			rIDressedDriver_::Init( ThreadSafety );
			Flow_.Init( Driver );
			MultiLine_ = MultiLine;
			State_ = sRegular;
		}
	};



	qENUM( Command ) {
		cUser,
		cPass,
		cDele,
		cList,
		cRetr,
		cStat,
		cTop,
		cApop,
		cNoop,
		cQuit,
		cRset,
		cUidl,
		cCapa,
		c_amount,
		c_Undefined
	};

	const char *GetLabel( eCommand Command );

	class hBody
	{
	private:
		rSequenceDelimitedIDriver Driver_;
		flw::sIFlow Flow_;
	public:
		void reset( bso::sBool P = true )
		{
			tol::reset( P, Flow_, Driver_, Message );
		}
		qCDTOR( hBody );
		void Init(
			fdr::rIDriver &Driver,
			bso::sBool Multiline )
		{
			Driver_.Init( Driver, Multiline, fdr::ts_Default );
			Flow_.Init( Driver_ );
			tol::Init( Message );
		}
		str::wString Message;
		flw::sIFlow &GetFlow( void )
		{
			return Flow_;
		}
	};


	// To launch before any of the operation below. Eats the message sent on connection.
	bso::sBool Authenticate(
		const str::dString &Username,
		const str::dString &Password,
		fdr::rIODriver &Server,
		hBody & Body );

	bso::sBool List(
		fdr::rIODriver &Server,
		hBody & Body );

	bso::sBool Retrieve(
		bso::sUInt Index,
		fdr::rIODriver &Server,
		hBody & Body );

	bso::sBool Top(
		bso::sUInt Index,
		bso::sUInt AmoutOfLine,
		fdr::rIODriver &Server,
		hBody & Body );

	bso::sBool UIDL(
		bso::sUInt Index,	// Si = à '04, retourne l'ensemble des messages.
		fdr::rIODriver &Server,
		hBody & Body );

	bso::sBool Quit(
		fdr::rIODriver &Server,
		hBody & Body );
}


#endif
