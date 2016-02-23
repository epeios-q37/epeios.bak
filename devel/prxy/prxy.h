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

# include "prxybase.h"

# include "csdbnc.h"

# include "err.h"
# include "flw.h"
# include "lcl.h"

namespace prxy {

	class rProxy
	{
	private:
		qRVM( flw::ioflow__, F_, Flow_ );
	public:
		void reset( bso::bool__ P = true )
		{
			Flow_ = NULL;
		}
		qCDTOR( rProxy );
		bso::bool__ Init(
			flw::ioflow__ &Flow,
			const char *Identifier,
			prxybase::eType Type )
		{
			Flow_ = &Flow;

			prxybase::PutType( Type, Flow );

			Flow.Write( Identifier, strlen( Identifier ) + 1 );	// '+1' to put the final 0.

			Flow.Commit();

			if ( !Flow.EndOfFlow() ) {
				if ( Flow.Get() != 0 )
					qRGnr();

				Flow.Dismiss();

				return true;
			} else
				return false;
		}
		fdr::size__ WriteUpTo(
			const fdr::byte__ *Buffer,
			fdr::size__ Maximum )
		{
			return F_().WriteUpTo( Buffer, Maximum );
		}
		void Commit( void )
		{
			F_().Commit();
		}
		fdr::size__ ReadUpTo(
			fdr::size__ Maximum,
			fdr::byte__ *Buffer )
		{
			return F_().ReadUpTo( Maximum, Buffer );
		}
		void Dismiss( void )
		{
			F_().Dismiss();
		}
	};

	typedef fdr::ioflow_driver___<>	rFlowDriver_;

	typedef flw::standalone_ioflow__<> sFlow_;

	qENUM( State ) {
		sOK,
		sUnableToConnect,
		sLostProxyConnexion,
		s_amount,
		s_Undefined
	};

	const char *GetLabel( eState State );

	lcl::meaning_ &GetMeaning(
		eState State,
		const char *HostService,
		lcl::meaning_ &Meaning );

	class rFlow
	: private rFlowDriver_,
	  public sFlow_
	{
	private:
		csdbnc::flow___ Flow_;
		prxy::rProxy Proxy_;
	protected:
		virtual fdr::size__ FDRWrite(
			const fdr::byte__ *Buffer,
			fdr::size__ Maximum) override
		{
			return Proxy_.WriteUpTo( Buffer, Maximum );
		}
		virtual void FDRCommit( void ) override
		{
			Proxy_.Commit();
		}
		virtual fdr::size__ FDRRead(
			fdr::size__ Maximum,
			fdr::byte__ *Buffer ) override
		{
			return Proxy_.ReadUpTo( Maximum, Buffer );
		}
		virtual void FDRDismiss( void ) override
		{
			Proxy_.Dismiss();
		}
	public:
		void reset( bso::bool__ P = true )
		{
			sFlow_::reset( P );
			rFlowDriver_::reset( P );
			Proxy_.reset( P );
			Flow_.reset( P );
		}
		qCVDTOR( rFlow );
		eState Init(
			const char *HostService,
			const char *Identifier,
			prxybase::eType Type,
			err::handling__ ErrorHandling = err::h_Default )
		{
			reset();

			if ( !Flow_.Init( HostService, ErrorHandling ) )
				return sUnableToConnect;

			rFlowDriver_::Init( fdr::ts_Default );

			sFlow_::Init( *this );

			if ( Proxy_.Init( Flow_, Identifier, Type ) )
				return sOK;
			else if ( ErrorHandling == err::hThrowException )
				qRFwk();
			else
				return sLostProxyConnexion;

			return s_Undefined;	// To avoid a warning.
		}
		bso::fBool Init(
			const char *HostService,
			const char *Identifier,
			prxybase::eType Type,
			lcl::meaning_ &Meaning )	// If returned value == 'false', then 'Meaning' contents the error message.
		{
			eState State = Init( HostService, Identifier, Type, err::hUserDefined );

			if ( State != sOK ) {
				GetMeaning( State, HostService, Meaning );
				return false;
			} else
				return true;
		}
		time_t EpochTimeStamp( void ) const
		{
			return Flow_.EpochTimeStamp();
		}
	};

}

#endif
