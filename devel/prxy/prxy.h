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

// PRoXY

#ifndef PRXY__INC
# define PRXY__INC

# define PRXY_NAME		"PRXY"

# if defined( E_DEBUG ) && !defined( PRXY_NODBG )
#  define PRXY_DBG
# endif

# include "err.h"
# include "flw.h"
# include "csdbnc.h"
# include "flx.h"

namespace prxy {
	typedef fdr::ioflow_driver___<>	rFlowDriver_;

	typedef flw::standalone_ioflow__<> sFlow_;

	class rFlow
	: private rFlowDriver_,
	  public sFlow_
	{
	private:
		csdbnc::flow___ Flow_;
		flx::size_embedded_oflow___ OFlow_;
		flw::iflow__ &IFlow_;
	protected:
		virtual fdr::size__ FDRWrite(
			const fdr::byte__ *Buffer,
			fdr::size__ Maximum) override
		{
			return OFlow_.WriteUpTo( Buffer, Maximum );
		}
		virtual void FDRCommit( void ) override
		{
			OFlow_.Commit();
		}
		virtual fdr::size__ FDRRead(
			fdr::size__ Maximum,
			fdr::byte__ *Buffer ) override
		{
			return IFlow_.ReadUpTo( Maximum, Buffer );
		}
		virtual void FDRDismiss( void ) override
		{
			IFlow_.Dismiss();
		}
public:
		void reset( bso::bool__ P = true )
		{
			sFlow_::reset( P );
			rFlowDriver_::reset( P );
			OFlow_.reset( P );
			Flow_.reset( P );
		}
		qVDTOR( rFlow );
		rFlow( void )
		: IFlow_( Flow_ )
		{
			reset( false );
		}
		bso::bool__ Init(
			const char *HostService,
			const char *Identifier )
		{
			reset();

			rFlowDriver_::Init( fdr::ts_Default );
			sFlow_::Init( *this );

			if ( !Flow_.Init( HostService, err::hUserDefined ) )
				return false;

			OFlow_.Init( Flow_, flx::chPropagate );

			OFlow_.Write(Identifier, strlen( Identifier ) + 1 );	// '+1' to put the final 0.

			OFlow_.Commit();

			if ( !IFlow_.EndOfFlow() ) {
				if ( IFlow_.Get() != 0 )
					qRGnr();

				return true;
			} else
				return false;
		}
		void Commit( void )
		{
			return sFlow_::Commit();
		}
		void Dismiss( void )
		{
			return sFlow_::Dismiss();
		}
	};


}

#endif
