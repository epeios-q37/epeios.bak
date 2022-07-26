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

// MuSiC MiDi Device

#ifndef MSCMDD_INC_
# define MSCMDD_INC_

# if defined( E_DEBUG ) && !defined( MSCMDD_NODBG )
#  define MSCMDD_DBG
# endif

# include "err.h"
# include "flw.h"
# include "cpe.h"
# include "str.h"
# include "crt.h"
# include "lcl.h"

# ifdef CPE_S_WIN
#  define MSCMDD__WINDOWS
# elif defined( CPE_S_LINUX )
#  define MSCMDD__ALSA
# else
#  error
# endif

#ifdef MSCMDD__WINDOWS
#  include <Windows.h>
#  include <MMSystem.h>
#  include "mtx.h"
# elif defined( MSCMDD__ALSA )
#  include <alsa/asoundlib.h>
# endif

#ifdef MSCMDD_INPUT_CACHE_SIZE
#	define MSCMDD__INPUT_CACHE_SIZE	MSCMDD_INPUT_CACHE_SIZE
#else
#	define MSCMDD__INPUT_CACHE_SIZE FLW__INPUT_CACHE_SIZE
#endif

#ifdef MSCMDD_OUTPUT_CACHE_SIZE
#	define MSCMDD__OUTPUT_CACHE_SIZE	MSCMDD_OUTPUT_CACHE_SIZE
#else
#	define MSCMDD__OUTPUT_CACHE_SIZE FLW__OUTPUT_CACHE_SIZE
#endif


namespace mscmdd {
#ifdef MSCMDD__WINDOWS
	typedef UINT_PTR	sHandler;
	typedef char sName[MAXPNAMELEN];

	class rOut
	{
	private:
		HMIDIOUT Handle_;
	public:
		void reset( bso::bool__ P = true )
		{
			if ( P )
				if ( Handle_ != NULL )
					if ( midiOutClose( Handle_ ) != MMSYSERR_NOERROR )
						qRFwk();

			Handle_ = NULL;
		}
		qCDTOR( rOut );
		bso::sBool Init(
			const str::dString &Id,
			err::handling__ ErrorHandling )
		{
			reset();

			UINT Device = 0;

			Id.ToNumber(Device);

			if ( midiOutOpen( &Handle_, Device, 0, 0, CALLBACK_NULL) != MMSYSERR_NOERROR ) {
				if ( ErrorHandling != err::hUserDefined )
					qRFwk();
				else
					return false;
			}

			return true;
		}
		fdr::size__ Write(
			const fdr::sByte *Buffer,
			fdr::size__ Maximum )
		{
			MIDIHDR     midiHdr;
			int Err;

			midiHdr.lpData = (LPSTR)Buffer;

			midiHdr.dwBufferLength = Maximum;

			midiHdr.dwFlags = 0;

#ifdef MSCMDD_DBG
			if ( Handle_ == NULL )
				qRFwk();
#endif
			/* Prepare the buffer and MIDIHDR */
			if ( midiOutPrepareHeader( Handle_,  &midiHdr, sizeof(MIDIHDR)) != MMSYSERR_NOERROR )
				qRFwk();

			if ( midiOutLongMsg( Handle_, &midiHdr, sizeof(MIDIHDR ) ) != MMSYSERR_NOERROR )
				qRFwk();

			while ( ( Err = midiOutUnprepareHeader( Handle_, &midiHdr, sizeof(MIDIHDR ) ) ) == MIDIERR_STILLPLAYING )
			{}

			if ( Err != MMSYSERR_NOERROR )
				qRFwk();

			return Maximum;
		}
	};
#	elif defined( MSCMDD__ALSA )
	bso::bool__ GetMIDIOutDeviceName(
		int Device,
		str::string_ &Name );

	class rOut
	{
	private:
		snd_rawmidi_t    *_Handle;
	public:
		void reset( bso::bool__ P = true )
		{
			if ( P )
				if ( _Handle != NULL ) {
					if ( snd_rawmidi_drain( _Handle ) < 0 )
						qRLbr();

					if ( snd_rawmidi_close( _Handle ) < 0 )
						qRLbr();
				}

			_Handle = NULL;
		}
		rOut( void )
		{
			reset( false );
		}
		~rOut( void )
		{
			reset();
		}
		bso::bool__ Init(
			const str::dString &Id,
			err::handling__ ErrorHandling )
		{
			bso::bool__ Success = false;
		qRH
			qCBUFFERh SBuffer;
		qRB
			if ( snd_rawmidi_open( NULL, &_Handle, Id.Convert(SBuffer), 0 ) < 0 ) {
				if ( ErrorHandling != err::hUserDefined )
					qRLbr();
			} else
				Success = true;
		qRR
		qRT
		qRE
			return Success;
		}
		fdr::size__ Write(
			const fdr::sByte *Buffer,
			fdr::size__ Maximum )
		{
			return snd_rawmidi_write( _Handle, Buffer, Maximum );
		}
		void Commit( void )
		{
			if ( _Handle != NULL )
				if ( snd_rawmidi_drain( _Handle ) < 0 )
					qRLbr();
		}
	};
#	endif

	class rWDriver
	: public fdr::rWDressedDriver
	{
	private:
		rOut Out_;
	protected:
		virtual fdr::sSize FDRWrite(
			const fdr::sByte *Buffer,
			fdr::size__ Maximum ) override
		{
			return Out_.Write( Buffer, Maximum );
		}
		virtual bso::sBool FDRCommit(
            bso::sBool,
            qRPN ) override
		{
		    return true;
		}
		virtual tht::sTID FDRWTake( tht::sTID TID ) override
		{
			return TID;
		}
	public:
		void reset( bso::bool__ P = true )
		{
			fdr::oflow_driver___<>::reset( P );
			Out_.reset();
		}
		qCVDTOR( rWDriver)
		bso::sBool Init(
			const str::dString &Id,
			err::handling__ ErrHandling = err::h_Default,
			fdr::thread_safety__ ThreadSafety = fdr::ts_Default )
		{
			fdr::oflow_driver___<>::Init( ThreadSafety );

			return Out_.Init(Id, ErrHandling);
		}
	};

	typedef flw::rDressedWFlow<MSCMDD__OUTPUT_CACHE_SIZE> WFlow_;

	class rWFlow
	: public WFlow_
	{
	private:
		rWDriver Driver_;
	public:
		void reset( bool P = true )
		{
			WFlow_::reset( P );
			Driver_.reset( P );
		}
		qCVDTOR( rWFlow );
		//f Initialization with socket 'Socket' and 'Timeout' as timeout.
		bso::bool__ Init(
			const str::dString &Id,
			err::handling__ ErrHandle = err::h_Default )
		{
			WFlow_::Init( Driver_ );

			return Driver_.Init(Id, ErrHandle);
		}
	};

#	ifdef MSCMDD__WINDOWS
	struct rData_
	{
	public:
		mtx::handler___ Access;	// Pour protger l'accs aus donnes de cet structure.
		mtx::handler___ Full;		// Pour faire attendre le producteur si 'Buffer' est plein.
		mtx::handler___ Empty;		// Pout faire attendre le consommateur si 'Buffer' est vide.
		fdr::sByte *Buffer;
		fdr::size__ Size, Available, Position;
		bso::bool__ Purge;	// Lorsque  'true', purge l'ensemble des donnes MIDI.
		void _data__( void )
		{
			Access = Full = Empty = MTX_INVALID_HANDLER;
			Buffer = NULL;
			Size = Available = Position = 0;
			Purge = false;
		}
	};

	inline bso::bool__ IsFull_( const rData_ &Data )
	{
#ifdef MSCMDD_DBG
		if ( !mtx::IsLocked( Data.Access ) )
			qRFwk();
#endif
		return ( ( Data.Available + Data.Position ) == Data.Size );
	}

	inline bso::bool__ IsEmpty_( const rData_ &Data )
	{
#ifdef MSCMDD_DBG
		if ( !mtx::IsLocked( Data.Access ) )
			qRFwk();
#endif
		return ( Data.Available == 0 );
	}

	inline bso::size__ Emptyness_( const rData_ &Data )
	{
#ifdef MSCMDD_DBG
		if ( !mtx::IsLocked( Data.Access ) )
			qRFwk();
#endif
		return ( Data.Size - ( Data.Available + Data.Position ) );
	}


	class rIn
	{
	private:
		bso::bool__ _Started;
		HMIDIIN _Handle;
		MIDIHDR _Header[3];
		fdr::sByte _Cache[2000];
		char _HeaderBuffer[512][3];
		rData_ _Data;
		void _Purge( void )
		{
			_Data.Purge = true;

			if ( midiInReset( _Handle ) != MMSYSERR_NOERROR )
				qRFwk();
		}
	public:
		void reset( bso::bool__ P = true )
		{
			if ( P ) {
				if ( _Data.Buffer != NULL ) {
					if ( _Started )
						if ( midiInStop( _Handle ) != MMSYSERR_NOERROR )
						qRFwk();

					_Purge();

					mtx::Delete( _Data.Access );
					mtx::Delete( _Data.Full );
					mtx::Delete( _Data.Empty, true );	// Comme toute les donnes ont t lues, ce mutex est verrouill, on autorise donc sa destruction mme si verrouill.


					midiInClose( _Handle );
				}
			}

			_Started = false;
			_Data.Available = _Data.Position = _Data.Size = 0;
			_Data.Access = _Data.Full = _Data.Empty = MTX_INVALID_HANDLER;
			_Data.Buffer = NULL;
		}
		qCDTOR( rIn );
		bso::bool__ Init(
			const str::dString &Id,
			err::handling__ ErrHandling );
		void Start( void )
		{
			if ( !_Started ) {
				if ( midiInStart( _Handle ) != MMSYSERR_NOERROR )
					qRFwk();
				_Started = true;
			}
		}
		void Stop( void )
		{
			if ( _Started ) {
				if ( midiInStop( _Handle ) != MMSYSERR_NOERROR )
					qRFwk();
				_Started = false;
			}

		}
		fdr::size__ Read(
			fdr::size__ Maximum,
			fdr::sByte *Buffer );
	};
#	elif defined( MSCMDD__ALSA )
	bso::bool__ GetMIDIInDeviceName(
		int Device,
		str::string_ &Name );

	class rIn
	{
	private:
		snd_rawmidi_t    *_Handle;
	public:
		void reset( bso::bool__ P = true )
		{
			if ( P )
				if ( _Handle != NULL ) {
					if ( snd_rawmidi_drain( _Handle ) < 0 )
						qRLbr();
					if ( snd_rawmidi_close( _Handle ) < 0 )
						qRLbr();
				}

			_Handle = NULL;
		}
		rIn( void )
		{
			reset( false );
		}
		~rIn( void )
		{
			reset();
		}
		bso::bool__ Init(
			const str::dString &Id,
			err::handling__ ErrorHandling )
		{
			bso::bool__ Success = false;
		qRH
			qCBUFFERh SBuffer;
		qRB
			if ( snd_rawmidi_open( &_Handle, NULL, Id.Convert(SBuffer), 0 ) < 0 ) {
				if ( ErrorHandling != err::hUserDefined )
					qRLbr();
			} else
				Success = true;
		qRR
		qRT
		qRE
			return Success;
		}
		fdr::size__ Read(
			fdr::size__ Maximum,
			fdr::sByte *Buffer )
		{
			return snd_rawmidi_read( _Handle, Buffer, Maximum );
		}
		void Dismiss( void )
		{
			if ( _Handle != NULL )
				if ( snd_rawmidi_drain( _Handle ) < 0 )
					qRLbr();
		}
		// 'Start', 'Stop': pour singer la version 'Windows'.
		void Stop( void )
		{}
		void Start( void )
		{}
	};
#	endif

	class rRDriver
	: public fdr::iflow_driver___<MSCMDD__INPUT_CACHE_SIZE>
	{
	private:
		rIn _In;
	protected:
		virtual fdr::size__ FDRRead(
			fdr::size__ Maximum,
			fdr::sByte *Buffer ) override
		{
			return _In.Read( Maximum, Buffer );
		}
		virtual bso::sBool FDRDismiss(
            bso::sBool,
            qRPN ) override
		{
			_In.Stop();

			return true;
		}
		virtual tht::sTID FDRRTake( tht::sTID TID ) override
		{
			return TID;
		}
	public:
		void reset( bso::bool__ P = true )
		{
			fdr::iflow_driver___<>::reset( P );
			_In.reset( P );
		}
		qCVDTOR( rRDriver );
		bso::bool__ Init(
			const str::dString &Id,
			err::handling__ ErrHandling = err::h_Default,
			fdr::thread_safety__ ThreadSafety = fdr::ts_Default )
		{
			fdr::iflow_driver___<MSCMDD__INPUT_CACHE_SIZE>::Init( ThreadSafety );
			return _In.Init(Id, ErrHandling);
		}
		void Start( void )
		{
			_In.Start();
		}
		void Stop( void )
		{
			_In.Stop();
		}
	};

	typedef flw::standalone_iflow__<> _iflow__;	// En fait, le 'template' est, et doit tre,  'O' (voir 'flw::standalone_iflow__').

	class rRFlow
	: public _iflow__
	{
	private:
		rRDriver _Driver;
	public:
		void reset( bool P = true )
		{
			_iflow__::reset( P );
			_Driver.reset( P );
		}
		qCVDTOR( rRFlow );
		bso::bool__ Init(
			const str::dString &Id,
			err::handling__ ErrHandle = err::h_Default )
		{
			_iflow__::Init( _Driver );

			return _Driver.Init(Id, ErrHandle);
		}
		void Start( void )
		{
			_Driver.Start();
		}
		void Stop( void )
		{
			_Driver.Stop();
		}
	};

	enum status__
	{
		sOK,
		sUnableToOpenMIDIInDevice,
		sUnableToOpenMIDIOutDevice,
		s_amount,
		s_Undefined
	};

	const char *Label( status__ Status );

	typedef fdr::ioflow_driver___<MSCMDD__INPUT_CACHE_SIZE> _ioflow_driver___;

	class rRWDriver
	: public _ioflow_driver___
	{
	private:
		rIn _In;
		rOut _Out;
	protected:
		virtual fdr::size__ FDRRead(
			fdr::size__ Maximum,
			fdr::sByte *Buffer ) override
		{
			return _In.Read( Maximum, Buffer );
		}
		virtual bso::sBool FDRDismiss(
            bso::sBool,
            qRPN ) override
		{
			_In.Stop();
			return true;
		}
		virtual tht::sTID FDRRTake( tht::sTID TID ) override
		{
			return TID;
		}
		virtual fdr::size__ FDRWrite(
			const fdr::sByte *Buffer,
			fdr::size__ Maximum ) override
		{
			_In.Start();
			return _Out.Write( Buffer, Maximum );
		}
		virtual bso::sBool FDRCommit(
            bso::sBool,
            qRPN ) override
		{
		    return true;
		}
		virtual tht::sTID FDRWTake( tht::sTID TID ) override
		{
			return TID;
		}
	public:
		void reset( bso::bool__ P = true )
		{
			_ioflow_driver___::reset( P );
			_In.reset( P );
			_Out.reset( P );
		}
		qCVDTOR( rRWDriver );
		status__ Init(
			const str::dString &InId,
			const str::dString &OutId,
			err::handling__ ErrorHandling = err::h_Default,
			fdr::thread_safety__ ThreadSafety = fdr::ts_Default )
		{
			if ( !_In.Init(InId, ErrorHandling ) )
				return sUnableToOpenMIDIInDevice;

			if ( !_Out.Init(OutId, ErrorHandling ) )
				return sUnableToOpenMIDIOutDevice;

			_ioflow_driver___::Init( ThreadSafety );

			return sOK;
		}
	};

	typedef flw::standalone_ioflow__<MSCMDD__OUTPUT_CACHE_SIZE> _ioflow___;

	class rRWFlow
	: public _ioflow___
	{
	private:
		rRWDriver _Driver;
	public:
		void reset( bso::bool__ P = true )
		{
			_ioflow___::reset( P );
			_Driver.reset( P );
		}
		qCVDTOR( rRWFlow );
		status__ Init(
			const str::dString &InId,
			const str::dString &OutId,
			err::handling__ ErrorHandling = err::h_Default  )
		{
			status__ Status = _Driver.Init(InId, OutId, ErrorHandling );

			if ( Status != sOK )
				return Status;

			_ioflow___::Init( _Driver );

			return Status;
		}
	};

  bso::sSize GetMidiInDeviceNames(
    str::dStrings &Ids,
    str::dStrings &Names);

	bso::sSize GetMidiOutDeviceNames(
    str::dStrings &Ids,
    str::dStrings &Names);

	qENUM( Way ) {
		wIn,
		wOut,
		w_amount,
		w_Undefined
	};

	inline bso::sUInt GetMidiDeviceNames(
		eWay Way,
		str::dStrings &Ids,
		str::dStrings &Names)
	{
		switch ( Way ) {
		case wIn:
			return GetMidiInDeviceNames(Ids, Names);
			break;
		case wOut:
			return GetMidiOutDeviceNames(Ids, Names);
			break;
		default:
			qRFwk();
			break;
		}

		return 0;	// Pour viter un 'warning'.
	}
}

/*$END$*/
#endif
