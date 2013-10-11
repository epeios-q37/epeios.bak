/*
	Header for the 'iof' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Copyright (C) 2004 Claude SIMON (http://zeusw.org/intl/contact.html).

	This file is part of the Epeios (http://zeusw.org/epeios/) project.

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 3
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

//	$Id: iof.h,v 1.29 2013/04/05 16:33:40 csimon Exp $

#ifndef IOF__INC
#define IOF__INC

#define IOF_NAME		"IOF"

#define	IOF_VERSION	"$Revision: 1.29 $"

#define IOF_OWNER		"Claude SIMON (http://zeusw.org/intl/contact.html)"

#include "ttr.h"

extern class ttr_tutor &IOFTutor;

#if defined( E_DEBUG ) && !defined( IOF_NODBG )
#define IOF_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.29 $
//C Claude SIMON (http://zeusw.org/intl/contact.html)
//R $Date: 2013/04/05 16:33:40 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D Input/Output Flows 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "cpe.h"
#include "fdr.h"
#include "flw.h"
#include "iop.h"

namespace iof {
	typedef iop::input__		_input__;
	typedef iop::output__		_output__;
	typedef iop::io__			_io__;

	using bso::size__;

	class _output_driver__
	: public _output__
	{
	protected:
		fdr::size__ _Write(
			const fdr::datum__ *Buffer,
			fdr::size__ Maximum )
		{
#ifdef IOF_DBG
			if ( Buffer == NULL )
				ERRPrm();
#endif
			return _output__::Write( Buffer, Maximum );
		}
		void _Commit( void )
		{
			_output__::Flush();
		}
	};

	typedef fdr::oflow_driver___<> _oflow_driver___;

	class io_oflow_driver___
	: public _output_driver__,
	  public _oflow_driver___
	{
	protected:
		virtual fdr::size__ FDRWrite(
			const fdr::datum__ *Tampon,
			fdr::size__ Maximum )
		{
			return _output_driver__::Write( Tampon, Maximum );
		}
		virtual void FDRCommit( void )
		{
			return _output_driver__::_Commit();
		}
	public:
		void reset( bso::bool__ P = true )
		{
			_output_driver__::reset( P );
			_oflow_driver___::reset( P );
		}
		io_oflow_driver___( void )
		{
			reset( false );
		}
		virtual ~io_oflow_driver___( void )
		{
			reset();
		}
		void Init(
			iop::descriptor__ D,
			fdr::thread_safety__ ThreadSafety )
		{
			_output_driver__::Init( D );
			_oflow_driver___::Init( ThreadSafety );
		}
	};

	class io_oflow___
	: public flw::oflow__
	{
	private:
		io_oflow_driver___ _Driver;
		flw::datum__ _Cache[IOP__BUFFER_SIZE];
	public:
		void reset( bso::bool__ P = true )
		{
			oflow__::reset( P );
			_Driver.reset( P );
		}
		io_oflow___( void )
		{
			reset( false );
		}
		virtual ~io_oflow___( void )
		{
			reset();
		}
		void Init(
			iop::descriptor__ D,
			size__ AmountMax )
		{
			_Driver.Init( D, fdr::tsDisabled );
			oflow__::Init( _Driver, _Cache, sizeof( _Cache ), AmountMax );
		}
	};

	class _input_driver__
	: public _input__
	{
	protected:
		virtual fdr::size__ _Read(
			fdr::size__ Maximum,
			fdr::datum__ *Buffer )
		{
	#ifdef IOF_DBG
			if( Buffer == NULL )
				ERRPrm();
	#endif
			if ( !OnEOF() )
				return _input__::Read( Maximum, Buffer );
			else
				return 0;
		}
		virtual void _Dismiss( void )
		{}
	};

	typedef fdr::iflow_driver___<> _iflow_driver___;

	class io_iflow_driver___
	: public _input_driver__,
	  public _iflow_driver___
	{
	protected:
		virtual fdr::size__ FDRRead(
			fdr::size__ Maximum,
			fdr::datum__ *Buffer )
		{
			return _input_driver__::_Read( Maximum, Buffer );
		}
		virtual void FDRDismiss( void )
		{
			_input_driver__::_Dismiss();
		}
	public:
		void reset( bso::bool__ P = true )
		{
			_input_driver__::reset( P );
			_iflow_driver___::reset( P );
		}
		io_iflow_driver___( void )
		{
			reset( false );
		}
		virtual ~io_iflow_driver___( void )
		{
			reset();
		}
		void Init(
			iop::descriptor__ D,
			fdr::thread_safety__ ThreadSafety )
		{
			_input_driver__::Init( D );
			_iflow_driver___::Init( ThreadSafety );
		}
	};

	class io_iflow___
	: public flw::iflow__
	{
	private:
		io_iflow_driver___ _Driver;
	public:
		void reset( bso::bool__ P = true )
		{
			_Driver.reset( P );
			iflow__::reset( P );
		}
		io_iflow___( void )
		{
			reset( false );
		}
		virtual ~io_iflow___( void )
		{
			reset();
		}
		void Init(
			iop::descriptor__ D,
			size__ AmountMax )
		{
			_Driver.Init( D, fdr::tsDisabled );
			iflow__::Init( _Driver, AmountMax );
		}
	};

	typedef fdr::ioflow_driver___<> _ioflow_driver___;

	class io_flow_driver___
	: public _output_driver__,
	  public _input_driver__,
	  public _ioflow_driver___
	{
	protected:
		virtual fdr::size__ FDRWrite(
			const fdr::datum__ *Buffer,
			fdr::size__ Maximum )
		{
			return _output_driver__::_Write( Buffer, Maximum );
		}
		virtual void FDRCommit( void )
		{
			_output_driver__::_Commit();
		}
		virtual fdr::size__ FDRRead(
			fdr::size__ Maximum,
			fdr::datum__ *Buffer )
		{
			return _input_driver__::_Read( Maximum, Buffer );
		}
		virtual void FDRDismiss( void )
		{
			return _input_driver__::_Dismiss();
		}
	public:
		void Init(
			iop::descriptor__ D,
			fdr::thread_safety__ ThreadSafety )
		{
			_input_driver__::Init( D );
			_output_driver__::Init( D );
			_ioflow_driver___::Init( ThreadSafety );
		}

	};

	class io_flow___
	: public _io__,
	  public flw::ioflow__
	{
	private:
		io_flow_driver___ _Driver;
		flw::datum__ _Cache[2 * IOP__BUFFER_SIZE];
	public:
		void reset( bso::bool__ P = true )
		{
			_io__::reset( P );
			ioflow__::reset( P );
		}
		io_flow___( void )
		{
			reset( false );
		}
		virtual ~io_flow___( void )
		{
			reset();
		}
		void Init(
			iop::descriptor__ D,
			size__ AmountMax,
			fdr::thread_safety__ ThreadSafety )
		{
			_io__::Init( D );
			_Driver.Init( D, ThreadSafety );
			ioflow__::Init( _Driver, _Cache, sizeof( _Cache ), AmountMax );
		}
	};
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
