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

//	$Id: mscmdd.h,v 1.16 2012/09/07 16:18:21 csimon Exp $

#ifndef MSCMDD__INC
#define MSCMDD__INC

#define MSCMDD_NAME		"MSCMDD"

#define	MSCMDD_VERSION	"$Revision: 1.16 $"

#define MSCMDD_OWNER		"Claude SIMON (http://zeusw.org/intl/contact.html)"


#if defined( E_DEBUG ) && !defined( MSCMDD_NODBG )
#define MSCMDD_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.16 $
//C Claude SIMON (http://zeusw.org/intl/contact.html)
//R $Date: 2012/09/07 16:18:21 $

/* End of automatic documentation generation part. */

/* Addendum to the automatic documentation generation part. */
//D MuSiC MiDi Device 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

# include "err.h"
# include "flw.h"
# include "cpe.h"
# include "str.h"
# include "ctn.h"
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
	typedef UINT_PTR	handler__;
	typedef char name__[MAXPNAMELEN];

	class midi_out___
	{
	private:
		HMIDIOUT    _Handle;
	public:
		void reset( bso::bool__ P = true )
		{
			if ( P )
				if ( _Handle != NULL )
					if ( midiOutClose( _Handle ) != MMSYSERR_NOERROR )
						qRFwk();

			_Handle = NULL;
		}
		midi_out___( void )
		{
			reset( false );
		}
		~midi_out___( void )
		{
			reset();
		}
		bso::bool__ Init(
			int Device,
			err::handling__ ErrorHandling )
		{
			reset();

			if ( midiOutOpen( &_Handle, Device, 0, 0, CALLBACK_NULL) != MMSYSERR_NOERROR ) {
				if ( ErrorHandling != err::hUserDefined )
					qRFwk();
				else
					return false;
			}

			return true;
		}
		fdr::size__ Write(
			const fdr::datum__ *Buffer,
			fdr::size__ Maximum )
		{
			MIDIHDR     midiHdr;
			int Err;

			midiHdr.lpData = (LPSTR)Buffer;

			midiHdr.dwBufferLength = Maximum;

			midiHdr.dwFlags = 0;

#ifdef MSCMDD_DBG
			if ( _Handle == NULL )
				qRFwk();
#endif
			/* Prepare the buffer and MIDIHDR */
			if ( midiOutPrepareHeader( _Handle,  &midiHdr, sizeof(MIDIHDR)) != MMSYSERR_NOERROR )
				qRFwk();

			if ( midiOutLongMsg( _Handle, &midiHdr, sizeof(MIDIHDR ) ) != MMSYSERR_NOERROR )
				qRFwk();

			while ( ( Err = midiOutUnprepareHeader( _Handle, &midiHdr, sizeof(MIDIHDR ) ) ) == MIDIERR_STILLPLAYING )
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
	
	class midi_out___
	{
	private:
		snd_rawmidi_t    *_Handle;
	public:
		void reset( bso::bool__ P = true )
		{
			if ( P )
				if ( _Handle != NULL ) {
					if ( snd_rawmidi_drain( _Handle ) < 0 )
						ERRf();

					if ( snd_rawmidi_close( _Handle ) < 0 )
						ERRf();
				}

			_Handle = NULL;
		}
		midi_out___( void )
		{
			reset( false );
		}
		~midi_out___( void )
		{
			reset();
		}
		bso::bool__ Init(
			int Device,
			err::handling__ ErrorHandling )
		{
			bso::bool__ Success = false;
		qRH
			str::string Name;
			STR_BUFFER___ SBuffer;
		qRB
			Name.Init( );
			
			if ( !GetMIDIOutDeviceName( Device, Name ) )
				qRReturn;
			
			if ( snd_rawmidi_open( NULL, &_Handle, Name.Convert( SBuffer ), 0 ) < 0 ) {
				if ( ErrorHandling != err::hUserDefined )
					ERRf();
				else
					qRReturn;
			}
			
			Success = true;
		qRR
		qRT
		qRE
			return Success;
		}
		fdr::size__ Write(
			const fdr::datum__ *Buffer,
			fdr::size__ Maximum )
		{
			return snd_rawmidi_write( _Handle, Buffer, Maximum );
		}
		void Commit( void )
		{
			if ( _Handle != NULL )
				if ( snd_rawmidi_drain( _Handle ) < 0 )
					ERRf();
		}
	};
#	endif

	class midi_oflow_driver___
	: public fdr::oflow_driver___<>
	{
	private:
		midi_out___ _Out;
	protected:
		virtual fdr::size__ FDRWrite(
			const fdr::datum__ *Buffer,
			fdr::size__ Maximum )
		{
			return _Out.Write( Buffer, Maximum );
		}
		virtual void FDRCommit( void )
		{}
	public:
		void reset( bso::bool__ P = true )
		{
			fdr::oflow_driver___<>::reset( P );
			_Out.reset();
		}
		midi_oflow_driver___( void )
		{
			reset( false );
		}
		~midi_oflow_driver___( void )
		{
			reset();
		}
		bso::bool__ Init(
			int Device,
			err::handling__ ErrHandling = err::h_Default,
			fdr::thread_safety__ ThreadSafety = fdr::ts_Default )
		{
			fdr::oflow_driver___<>::Init( ThreadSafety );

			return _Out.Init( Device, ErrHandling );
		}
	};

	typedef flw::standalone_oflow__<MSCMDD__OUTPUT_CACHE_SIZE> _oflow__;

	class midi_oflow___
	: public _oflow__
	{
	private:
		midi_oflow_driver___ _Driver;
	public:
		void reset( bool P = true )
		{
			_oflow__::reset( P );
			_Driver.reset( P );
		}
		midi_oflow___( void )
		{
			reset( false );
		}
		virtual ~midi_oflow___( void )
		{
			reset();
		}
		//f Initialization with socket 'Socket' and 'TimeOut' as timeout.
		bso::bool__ Init(
			int DeviceId,
			flw::size__ AmountMax = FLW_AMOUNT_MAX,
			err::handling__ ErrHandle = err::h_Default )
		{
			_oflow__::Init( _Driver, AmountMax );

			return _Driver.Init( DeviceId, ErrHandle );
		}
		bso::bool__ Init(
			int DeviceId,
			err::handling__ ErrHandle )
		{
			return Init( DeviceId, FLW_AMOUNT_MAX, ErrHandle );
		}
	};

#	ifdef MSCMDD__WINDOWS
	struct _data___
	{
	public:
		mtx::handler___ Access;	// Pour protger l'accs aus donnes de cet structure.
		mtx::handler___ Full;		// Pour faire attendre le producteur si 'Buffer' est plein.
		mtx::handler___ Empty;		// Pout faire attendre le consommateur si 'Buffer' est vide.
		fdr::datum__ *Buffer;
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

	inline bso::bool__ _IsFull( const _data___ &Data )
	{
#ifdef MSCMDD_DBG
		if ( !mtx::IsLocked( Data.Access ) )
			qRFwk();
#endif
		return ( ( Data.Available + Data.Position ) == Data.Size );
	}

	inline bso::bool__ _IsEmpty( const _data___ &Data )
	{
#ifdef MSCMDD_DBG
		if ( !mtx::IsLocked( Data.Access ) )
			qRFwk();
#endif
		return ( Data.Available == 0 );
	}

	inline bso::size__ _Emptyness( const _data___ &Data )
	{
#ifdef MSCMDD_DBG
		if ( !mtx::IsLocked( Data.Access ) )
			qRFwk();
#endif
		return ( Data.Size - ( Data.Available + Data.Position ) );
	}


	class midi_in___
	{
	private:
		bso::bool__ _Started;
		HMIDIIN _Handle;
		MIDIHDR _Header[3];
		fdr::datum__ _Cache[2000];
		char _HeaderBuffer[512][3];
		_data___ _Data;
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
		midi_in___( void )
		{
			reset( false );
		}
		~midi_in___( void )
		{
			reset();
		}
		bso::bool__ Init(
			int Device,
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
			fdr::datum__ *Buffer );
	};
#	elif defined( MSCMDD__ALSA )
	bso::bool__ GetMIDIInDeviceName(
		int Device,
		str::string_ &Name );
	
	class midi_in___
	{
	private:
		snd_rawmidi_t    *_Handle;
	public:
		void reset( bso::bool__ P = true )
		{
			if ( P )
				if ( _Handle != NULL ) {
					if ( snd_rawmidi_drain( _Handle ) < 0 )
						ERRf();
					if ( snd_rawmidi_close( _Handle ) < 0 )
						ERRf();
				}

			_Handle = NULL;
		}
		midi_in___( void )
		{
			reset( false );
		}
		~midi_in___( void )
		{
			reset();
		}
		bso::bool__ Init(
			int Device,
			err::handling__ ErrorHandling )
		{
			bso::bool__ Success = false;
		qRH
			str::string Name;
			STR_BUFFER___ SBuffer;
		qRB
			Name.Init( );
			
			if ( !GetMIDIInDeviceName( Device, Name ) )
				qRReturn;
			
			if ( snd_rawmidi_open( &_Handle, NULL, Name.Convert( SBuffer ), 0 ) < 0 ) {
				if ( ErrorHandling != err::hUserDefined )
					ERRf();
				else
					qRReturn;
			}
			
			Success = true;
		qRR
		qRT
		qRE
			return Success;
		}
		fdr::size__ Read(
			fdr::size__ Maximum,
			fdr::datum__ *Buffer )
		{
			return snd_rawmidi_read( _Handle, Buffer, Maximum );
		}
		void Dismiss( void )
		{
			if ( _Handle != NULL )
				if ( snd_rawmidi_drain( _Handle ) < 0 )
					ERRf();
		}
		// 'Start', 'Stop': pour singer la version 'Windows'.
		void Stop( void )
		{}
		void Start( void )
		{}
	};
#	endif

	class midi_iflow_driver___
	: public fdr::iflow_driver___<MSCMDD__INPUT_CACHE_SIZE>
	{
	private:
		midi_in___ _In;
	protected:
		virtual fdr::size__ FDRRead(
			fdr::size__ Maximum,
			fdr::datum__ *Buffer )
		{
			return _In.Read( Maximum, Buffer );
		}
		virtual void FDRDismiss( void )
		{
			_In.Stop();
		}
	public:
		void reset( bso::bool__ P = true )
		{
			fdr::iflow_driver___<>::reset( P );
			_In.reset( P );
		}
		midi_iflow_driver___( void )
		{
			reset( false );
		}
		~midi_iflow_driver___( void )
		{
			reset();
		}
		bso::bool__ Init(
			int Device,
			err::handling__ ErrHandling = err::h_Default,
			fdr::thread_safety__ ThreadSafety = fdr::ts_Default )
		{
			fdr::iflow_driver___<MSCMDD__INPUT_CACHE_SIZE>::Init( ThreadSafety );
			return _In.Init( Device, ErrHandling );
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

	class midi_iflow___
	: public _iflow__
	{
	private:
		midi_iflow_driver___ _Driver;
	public:
		void reset( bool P = true )
		{
			_iflow__::reset( P );
			_Driver.reset( P );
		}
		midi_iflow___( void )
		{
			reset( false );
		}
		virtual ~midi_iflow___( void )
		{
			reset();
		}
		bso::bool__ Init(
			int Device,
			flw::size__ AmountMax = FLW_AMOUNT_MAX,
			err::handling__ ErrHandle = err::h_Default )
		{
			_iflow__::Init( _Driver, AmountMax );

			return _Driver.Init( Device, ErrHandle );
		}
		bso::bool__ Init(
			int Device,
			err::handling__ ErrHandle )
		{
			return Init( Device, FLW_AMOUNT_MAX, ErrHandle );
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

	class midi_ioflow_driver___
	: public _ioflow_driver___
	{
	private:
		midi_in___ _In;
		midi_out___ _Out;
	protected:
		virtual fdr::size__ FDRRead(
			fdr::size__ Maximum,
			fdr::datum__ *Buffer )
		{
			return _In.Read( Maximum, Buffer );
		}
		virtual void FDRDismiss( void )
		{
			_In.Stop();
		}
		virtual fdr::size__ FDRWrite(
			const fdr::datum__ *Buffer,
			fdr::size__ Maximum )
		{
			_In.Start();
			return _Out.Write( Buffer, Maximum );
		}
		virtual void FDRCommit( void )
		{}
	public:
		void reset( bso::bool__ P = true )
		{
			_ioflow_driver___::reset( P );
			_In.reset( P );
			_Out.reset( P );
		}
		midi_ioflow_driver___( void )
		{
			reset( false );
		}
		~midi_ioflow_driver___( void )
		{
			reset();
		}
		status__ Init(
			int DeviceIn,
			int DeviceOut,
			err::handling__ ErrorHandling = err::h_Default,
			fdr::thread_safety__ ThreadSafety = fdr::ts_Default )
		{
			if ( !_In.Init( DeviceIn, ErrorHandling ) )
				return sUnableToOpenMIDIInDevice;

			if ( !_Out.Init( DeviceOut, ErrorHandling ) )
				return sUnableToOpenMIDIOutDevice;

			_ioflow_driver___::Init( ThreadSafety );

			return sOK;
		}
	};

	typedef flw::standalone_ioflow__<MSCMDD__OUTPUT_CACHE_SIZE> _ioflow___;

	class midi_ioflow___
	: public _ioflow___
	{
	private:
		midi_ioflow_driver___ _Driver;
	public:
		void reset( bso::bool__ P = true )
		{
			_ioflow___::reset( P );
			_Driver.reset( P );
		}
		midi_ioflow___( void )
		{
			reset( false );
		}
		~midi_ioflow___( void )
		{
			reset();
		}
		status__ Init(
			int DeviceIn,
			int DeviceOut,
			err::handling__ ErrorHandling = err::h_Default,
			bso::size__ AmountMax = FLW_AMOUNT_MAX  )
		{
			status__ Status = _Driver.Init( DeviceIn, DeviceOut, ErrorHandling );

			if ( Status != sOK )
				return Status;

			_ioflow___::Init( _Driver, AmountMax );

			return Status;
		}
	};


	typedef str::string_	name_;
	typedef str::string		name;

	typedef ctn::E_MCONTAINER_( name_ ) names_;
	E_AUTO( names )

	bso::uint__ GetMidiInDevicesNames( names_ &Names );
	bso::uint__ GetMidiOutDevicesNames( names_ &Names );

	enum way__ {
		wIn,
		wOut,
		w_amount,
		w_Undefined
	};

	inline bso::uint__ GetMidiDevicesNames(
		way__ Way,
		names_ &Names )
	{
		switch ( Way ) {
		case wIn:
			return GetMidiInDevicesNames( Names );
			break;
		case wOut:
			return GetMidiOutDevicesNames( Names );
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
