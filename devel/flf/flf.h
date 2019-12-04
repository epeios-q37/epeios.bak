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

#ifndef FLF_INC_
# define FLF_INC_

# define FLF_NAME		"FLF"

# if defined( E_DEBUG ) && !defined( FLF_NODBG )
#  define FLF_DBG
# endif

// FiLe Flow

# include "cpe.h"
# include "err.h"
# include "bso.h"
# include "fil.h"
# include "iof.h"
# include "fnm.h"

/***************/
/***** OLD *****/
/***************/

namespace flf {

	class file_iflow_driver___
	: public iof::io_iflow_driver___
	{
	private:
		iop::descriptor__ _D;
	public:
		void reset( bso::bool__ P = true )
		{
			io_iflow_driver___::reset( P );

			if ( P ) {
				if ( _D != IOP_UNDEFINED_DESCRIPTOR )
					fil::Close( _D );
			}

			_D = IOP_UNDEFINED_DESCRIPTOR;
		}
		file_iflow_driver___( void )
		{
			reset( false );
		}
		~file_iflow_driver___( void )
		{
			reset();
		}
		tol::report__ Init(
			const fnm::name___ &FileName,
			fdr::thread_safety__ ThreadSafety = fdr::ts_Default,
			fil::mode__ Mode = fil::mReadOnly,
			err::handling__ ErrorHandling = err::h_Default )
		{
			reset();

			tol::report__ Report = tol::r_Undefined;
			_D = fil::Open( FileName, Mode );

			if ( _D == IOP_UNDEFINED_DESCRIPTOR ) {
				Report = tol::rFailure;
				switch ( ErrorHandling ) {
				case err::hUserDefined:
					break;
				case err::hThrowException:
					qRLbr();
					break;
				default:
					qRFwk();
					break;
				}
			} else {
				Report = tol::rSuccess;
				io_iflow_driver___::Init( _D, ThreadSafety );
			}

			return Report;
		}
		tol::report__ Init(
			const fnm::name___ &FileName,
			fdr::thread_safety__ ThreadSafety,
			err::handling__ ErrorHandling,
			fil::mode__ Mode = fil::mReadOnly )
		{
			return Init( FileName, ThreadSafety, Mode, ErrorHandling );
		}
		tol::report__ Init(
			const fnm::name___ &FileName,
			err::handling__ ErrorHandling,
			fdr::thread_safety__ ThreadSafety = fdr::ts_Default,
			fil::mode__ Mode = fil::mReadOnly )
		{
			return Init( FileName, ThreadSafety, Mode, ErrorHandling );
		}
	};

	typedef iof::io_iflow___		_io_iflow___;

	//c A file as standard input flow.
	class file_iflow___
	: public _io_iflow___
	{
	private:
		iop::descriptor__ D_;
	public:
		void reset( bool P = true )
		{
			if ( P ) {
				if ( D_ != IOP_UNDEFINED_DESCRIPTOR )
					fil::Close( D_ );
			}

			_io_iflow___::reset( P );

			D_ = IOP_UNDEFINED_DESCRIPTOR;
		}
		file_iflow___( void )
		{
			reset( false );
		}
		virtual ~file_iflow___( void )
		{
			reset( true );
		}
		tol::report__ Init(
			const fnm::name___ &FileName,
			fil::mode__ Mode = fil::mReadOnly,
			err::handling__ ErrorHandling = err::h_Default )
		{
			reset();

			D_ = fil::Open( FileName, Mode );

			if ( D_ == IOP_UNDEFINED_DESCRIPTOR ) {
				switch ( ErrorHandling ) {
				case err::hUserDefined:
					return tol::rFailure;
					break;
				case err::hThrowException:
					qRLbr();
					break;
				default:
					qRFwk();
					break;
				}
			}

			_io_iflow___::Init( D_ );

			return tol::rSuccess;
		}
		tol::report__ Init(
			const fnm::name___ &FileName,
			err::handling__ ErrorHandling,
			fil::mode__ Mode = fil::mReadOnly )
		{
			return Init( FileName, Mode, ErrorHandling );
		}
	};

	class file_oflow_driver___
	: public iof::io_oflow_driver___
	{
	private:
		iop::descriptor__ _D;
	public:
		void reset( bso::bool__ P = true )
		{
			io_oflow_driver___::reset( P );

			if ( P ) {
				if ( _D != IOP_UNDEFINED_DESCRIPTOR )
					fil::Close( _D );
			}

			_D = IOP_UNDEFINED_DESCRIPTOR;
		}
		file_oflow_driver___( void )
		{
			reset( false );
		}
		~file_oflow_driver___( void )
		{
			reset();
		}
		tol::report__ Init(
			const fnm::name___ &FileName,
			fdr::thread_safety__ ThreadSafety = fdr::ts_Default,
			fil::mode__ Mode = fil::mRemove,
			err::handling__ ErrorHandling = err::h_Default )
		{
			reset();

			tol::report__ Report = tol::r_Undefined;
			_D = fil::Open( FileName, Mode );

			if ( _D == IOP_UNDEFINED_DESCRIPTOR ) {
				Report = tol::rFailure;
				switch ( ErrorHandling ) {
				case err::hUserDefined:
					break;
				case err::hThrowException:
					qRLbr();
					break;
				default:
					qRFwk();
					break;
				}
			} else {
				Report = tol::rSuccess;
				io_oflow_driver___::Init( _D, ThreadSafety );
			}

			return Report;
		}
		tol::report__ Init(
			const fnm::name___ &FileName,
			fdr::thread_safety__ ThreadSafety,
			err::handling__ ErrorHandling,
			fil::mode__ Mode = fil::mRemove )
		{
			return Init( FileName, ThreadSafety, Mode, ErrorHandling );
		}
		tol::report__ Init(
			const fnm::name___ &FileName,
			err::handling__ ErrorHandling,
			fdr::thread_safety__ ThreadSafety = fdr::ts_Default,
			fil::mode__ Mode = fil::mRemove )
		{
			return Init( FileName, ThreadSafety, Mode, ErrorHandling );
		}
	};

	typedef iof::io_oflow___		_io_oflow___;

	//c A file as standard input flow.
	class file_oflow___
	: public _io_oflow___
	{
	private:
		iop::descriptor__ D_;
	public:
		void reset( bool P = true )
		{
			_io_oflow___::reset( P );

			if ( P ) {
				if ( D_ != IOP_UNDEFINED_DESCRIPTOR ) {
					fil::Close( D_ );
				}
			}

			D_ = IOP_UNDEFINED_DESCRIPTOR;
		}
		file_oflow___( void ) 
		{
			reset( false );
		}
		virtual ~file_oflow___( void )
		{
			reset( true );
		}
		tol::report__ Init(
			const fnm::name___ &FileName,
			fil::mode__ Mode = fil::mRemove,
			err::handling__ ErrorHandling = err::h_Default )
		{
			reset();

			D_ = fil::Open( FileName, Mode );

			if ( D_ == IOP_UNDEFINED_DESCRIPTOR ) {
				switch ( ErrorHandling ) {
				case err::hUserDefined:
					return tol::rFailure;
					break;
				case err::hThrowException:
					qRLbr();
					break;
				default:
					qRFwk();
					break;
				}
			}

			io_oflow___::Init( D_ );

			return tol::rSuccess;
		}
		tol::report__ Init(
			const fnm::name___ &FileName,
			err::handling__ ErrorHandling,
			fil::mode__ Mode = fil::mRemove )
		{
			return Init( FileName, Mode, ErrorHandling );
		}
	};
}

/***************/
/***** NEW *****/
/***************/

namespace flf {
	typedef file_oflow___ rOFlow;
	typedef file_iflow___ rIFlow;

	typedef file_iflow_driver___ rIDriver;
	typedef file_oflow_driver___ rODriver;

	typedef rOFlow rWFlow;
	typedef rIFlow rRFlow;

	typedef rIDriver rRDriver;
	typedef rODriver rWDriver;
}


#endif
