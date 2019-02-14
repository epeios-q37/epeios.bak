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

//	$Id: crptgr.h,v 1.13 2013/04/19 19:28:00 csimon Exp $

#ifndef CRPTGR__INC
#define CRPTGR__INC

#define CRPTGR_NAME		"CRPTGR"

#define	CRPTGR_VERSION	"$Revision: 1.13 $"

#define CRPTGR_OWNER		"Claude SIMON"

#if defined( E_DEBUG ) && !defined( CRPTGR_NODBG )
#define CRPTGR_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.13 $
//C Claude SIMON (csimon at zeusw dot org)
//R $Date: 2013/04/19 19:28:00 $

/* End of automatic documentation generation part. */

/* Addendum to the automatic documentation generation part. */
//D CRyPToGRaphy 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"
#include "fdr.h"
#include "str.h"

namespace crptgr {

	typedef fdr::oflow_driver___<>	_oflow_driver___;
	
	class encrypt_oflow_driver___
	: public _oflow_driver___
	{
	private:
		flw::oflow__ *_Flow;
		str::string _Key;
		bso::size__ _Position;
		fdr::byte__ _Encrypt( fdr::byte__ Datum )
		{
			if ( _Position == _Key.Amount() )
				_Position = 0;

			return Datum + _Key(_Position++ );
		}
	protected:
		virtual fdr::size__ FDRWrite(
			const fdr::byte__ *Buffer,
			fdr::size__ Maximum ) override
		{
			fdr::size__ Amount = 0;

			// A optimiser.
			while ( Amount < Maximum )
				_Flow->Put( _Encrypt( Buffer[Amount++] ) );

			return Amount;
		}
		virtual bso::sBool FDRCommit(
			bso::sBool Unlock,
			qRPN ) override
		{
			return _Flow->Commit( Unlock, ErrHandling );
		}
		virtual fdr::sTID FDRWTake( fdr::sTID Owner ) override
		{
			 return _Flow->ODriver().WTake( Owner );
		}
	public:
		void reset( bso::bool__ P = true )
		{
			_oflow_driver___::reset( P );

			_Flow = NULL;
			_Key.reset( P );
			_Position = 0;
		}
		encrypt_oflow_driver___( void )
		{
			reset( false );
		}
		~encrypt_oflow_driver___( void )
		{
			reset( false );
		}
		void Init(
			flw::oflow__ &Flow,
			const str::string_ &Key,
			fdr::thread_safety__ ThreadSafety )
		{
			_oflow_driver___::Init( ThreadSafety );

			_Flow = &Flow;

			_Key.Init( Key );

			if ( !_Key.Amount() )
				_Key.Append( '\0' );	// Pas de cryptage si 'Key' vide.

			_Position = 0;
		}
	};

	typedef flw::standalone_oflow__<> _oflow__;

	class encrypt_oflow___
	: public _oflow__
	{
	private:
		encrypt_oflow_driver___ _Driver;
	public:
		void reset( bso::bool__ P = true )
		{
			_oflow__::reset( P );
			_Driver.reset( P );
		}
		encrypt_oflow___( void )
		{
			reset( false );
		}
		~encrypt_oflow___( void )
		{
			reset();
		}
		void Init(
			flw::oflow__ &Flow,
			const str::string_ &Key )
		{
			_Driver.Init( Flow, Key, fdr::tsDisabled );
			_oflow__::Init( _Driver );
		}
	};

	const str::string_ &Encrypt(
		const str::string_ &Plain,
		const str::string_ &Key,
		str::string_ &Crypted );

	typedef fdr::iflow_driver___<>	_iflow_driver___;
	
	class decrypt_iflow_driver___
	: public _iflow_driver___
	{
	private:
		flw::iflow__ *_Flow;
		str::string _Key;
		bso::size__ _Position;
		fdr::byte__ _Decrypt( fdr::byte__ Datum )
		{
			if ( _Position == _Key.Amount() )
				_Position = 0;

			return Datum - _Key(_Position++ );
		}
	protected:
		virtual fdr::size__ FDRRead(
			fdr::size__ Maximum,
			fdr::byte__ *Buffer ) override
		{
			fdr::size__ Amount = 0;

			Maximum = _Flow->ReadUpTo( Maximum, Buffer );

			// A optimiser.
			while ( Amount < Maximum ) {
				Buffer[Amount] = _Decrypt( Buffer[Amount] );
				Amount++;
			}

			return Maximum;
		}
		virtual bso::sBool FDRDismiss(
			bso::sBool Unlock,
			qRPN ) override
		{
			return _Flow->Dismiss( Unlock, ErrHandling );
		}
		virtual fdr::sTID FDRRTake( fdr::sTID Owner ) override
		{
			return _Flow->IDriver().RTake( Owner );
		}
	public:
		void reset( bso::bool__ P = true )
		{
			_iflow_driver___::reset( P );

			_Flow = NULL;
			_Key.reset( P );
			_Position = 0;
		}
		decrypt_iflow_driver___( void )
		{
			reset( false );
		}
		~decrypt_iflow_driver___( void )
		{
			reset( false );
		}
		void Init(
			flw::iflow__ &Flow,
			const str::string_ &Key,
			fdr::thread_safety__ ThreadSafety )
		{
			_iflow_driver___::Init( ThreadSafety );

			_Flow = &Flow;

			_Key.Init( Key );

			if ( !_Key.Amount() )
				_Key.Append( '\0' );	// Pas de dcryptage si 'Key' vide.

			_Position = 0;
		}
	};

	typedef flw::standalone_iflow__<> _iflow__;

	class decrypt_iflow___
	: public _iflow__
	{
	private:
		decrypt_iflow_driver___ _Driver;
	public:
		void reset( bso::bool__ P = true )
		{
			_iflow__::reset( P );
			_Driver.reset( P );
		}
		decrypt_iflow___( void )
		{
			reset( false );
		}
		~decrypt_iflow___( void )
		{
			reset();
		}
		void Init(
			flw::iflow__ &Flow,
			const str::string_ &Key )	// 'Key' is NOT duplicated.
		{
			_Driver.Init( Flow, Key, fdr::tsDisabled );
			_iflow__::Init( _Driver );
		}
	};

	const str::string_ &Decrypt(
		const str::string_ &Crypted,
		const str::string_ &Key,
		str::string_ &Plain );

}

/*$END$*/
#endif
