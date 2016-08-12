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

# include "csdcmn.h"
# include "csdbnc.h"

# include "err.h"
# include "flw.h"
# include "lcl.h"

namespace prxy {

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

	class rProxy_
	{
	private:
		csdbnc::flow___ Flow_;
	public:
		void reset( bso::bool__ P = true )
		{
			tol::reset( P, Flow_ );
		}
		qCDTOR( rProxy_ );
		eState Init(
			const char *HostService,
			const char *Identifier,
			prxybase::eType Type,
			prxybase::eRequest Request,
			sck::duration__ Timeout,
			qRPD )
		{
			if ( !Flow_.Init(HostService, Timeout, qRP) ) {
				if ( qRPT )
					qRFwk();

				return sUnableToConnect;
			}

			csdcmn::SendProtocol( prxybase::ProtocolId, prxybase::ProtocolVersion, Flow_ );

			prxybase::PutRequest( Request, Flow_ );

			prxybase::PutType( Type, Flow_ );

			prxybase::PutId( Identifier, Flow_ );

			Flow_.Commit();

			return sOK;
		}
		eState Init(
			const char *HostService,
			const char *Identifier,
			prxybase::eType Type,
			sck::duration__ Timeout,
			qRPD )
		{
			eState State = Init( HostService, Identifier, Type, prxybase::rPlug, Timeout, qRP );

			if ( State != sOK )
				return State;

			if ( Flow_.EndOfFlow() ) {
				if ( qRPT )
					qRFwk();

				return sLostProxyConnexion;
			} else {
				if ( prxybase::GetAnswer( Flow_ ) != prxybase::aPlugged )
					qRGnr();

				Flow_.Dismiss();

				return sOK;
			}
		}
		fdr::size__ WriteUpTo(
			const fdr::byte__ *Buffer,
			fdr::size__ Maximum )
		{
			return Flow_.WriteUpTo( Buffer, Maximum );
		}
		void Commit( void )
		{
			Flow_.Commit();
		}
		fdr::size__ ReadUpTo(
			fdr::size__ Maximum,
			fdr::byte__ *Buffer )
		{
			return Flow_.ReadUpTo( Maximum, Buffer );
		}
		void Dismiss( void )
		{
			Flow_.Dismiss();
		}
		time_t EpochTimeStamp( void ) const
		{
			return Flow_.EpochTimeStamp();
		}
	};

	typedef fdr::ioflow_driver___<>	rFlowDriver_;



	typedef flw::standalone_ioflow__<> sFlow_;

	class rFlow
	: private rFlowDriver_,
	  public sFlow_
	{
	private:
		prxy::rProxy_ Proxy_;
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
		}
		qCVDTOR( rFlow );
		eState Init(
			const char *HostService,
			const char *Identifier,
			prxybase::eType Type,
			sck::duration__ Timeout,
			qRPD )
		{
			reset();

			rFlowDriver_::Init( fdr::ts_Default );

			sFlow_::Init( *this );

			return Proxy_.Init( HostService, Identifier, Type, Timeout, qRP );
		}
		bso::sBool Init(
			const char *HostService,
			const char *Identifier,
			prxybase::eType Type,
			sck::duration__ Timeout,
			lcl::meaning_ &Meaning )	// If returned value == 'false', then 'Meaning' contents the error message.
		{
			eState State = Init( HostService, Identifier, Type, Timeout, qRPU );

			if ( State != sOK ) {
				GetMeaning( State, HostService, Meaning );
				return false;
			} else
				return true;
		}
		time_t EpochTimeStamp( void ) const
		{
			return Proxy_.EpochTimeStamp();
		}
	};

	// Request the proxy to close all server connections related to 'Identifier'.
	eState RequestDismiss(
		const char *HostService,
		const char *Identifier,
		qRPD );
}

#endif
