/*
	Header for the 'crptgr' library by Claude SIMON (csimon at zeusw dot org)
	Copyright (C) 20112004 Claude SIMON.

	This file is part of the Epeios (http://zeusw.org/epeios/) project.

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
	of the License, or (at your option) any later version.
 
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, go to http://www.fsf.org/
	or write to the:
  
         	         Free Software Foundation, Inc.,
           59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
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

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

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
		fdr::datum__ _Encrypt( fdr::datum__ Datum )
		{
			if ( _Position == _Key.Amount() )
				_Position = 0;

			return Datum + _Key(_Position++ );
		}
	protected:
		virtual fdr::size__ FDRWrite(
			const fdr::datum__ *Buffer,
			fdr::size__ Maximum )
		{
			fdr::size__ Amount = 0;

			// A optimiser.
			while ( Amount < Maximum )
				_Flow->Put( _Encrypt( Buffer[Amount++] ) );

			return Amount;
		}
		virtual void FDRCommit( void )
		{
			_Flow->Commit();
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
			const str::string_ &Key,
			flw::size__ AmountMax = FLW_AMOUNT_MAX )
		{
			_Driver.Init( Flow, Key, fdr::tsDisabled );
			_oflow__::Init( _Driver, AmountMax );
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
		fdr::datum__ _Decrypt( fdr::datum__ Datum )
		{
			if ( _Position == _Key.Amount() )
				_Position = 0;

			return Datum - _Key(_Position++ );
		}
	protected:
		virtual fdr::size__ FDRRead(
			fdr::size__ Maximum,
			fdr::datum__ *Buffer )
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
		virtual void FDRDismiss( void )
		{
			_Flow->Dismiss();
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
			const str::string_ &Key,
			flw::size__ AmountMax = FLW_AMOUNT_MAX )	// 'Key' n'est PAS dupliqu.
		{
			_Driver.Init( Flow, Key, fdr::tsDisabled );
			_iflow__::Init( _Driver, AmountMax );
		}
	};

	const str::string_ &Decrypt(
		const str::string_ &Crypted,
		const str::string_ &Key,
		str::string_ &Plain );

}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
