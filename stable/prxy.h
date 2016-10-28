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

	typedef csdbnc::rIOFlow rFlow_;

	class rProxy_
	: public rFlow_
	{
	public:
		void reset( bso::bool__ P = true )
		{
			rFlow_::reset( P );
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
			if ( !rFlow_::Init(HostService, Timeout, qRP) ) {
				if ( qRPT )
					qRFwk();

				return sUnableToConnect;
			}

			csdcmn::SendProtocol( prxybase::ProtocolId, prxybase::ProtocolVersion, *this );

			prxybase::PutRequest( Request, *this );

			prxybase::PutType( Type, *this );

			prxybase::PutId( Identifier, *this );

			rFlow_::Commit();

			return sOK;
		}
		eState Init(
			const char *HostService,
			const char *Identifier,
			prxybase::eType Type,
			sck::duration__ Timeout,
			qRPD )
		{
			eState State = Init( HostService, Identifier, Type, prxybase::rPlug_1, Timeout, qRP );

			if ( State != sOK )
				return State;

			if ( rFlow_::EndOfFlow() ) {
				if ( qRPT )
					qRFwk();

				return sLostProxyConnexion;
			} else {
				if ( prxybase::GetAnswer( *this ) != prxybase::aOK )
					qRGnr();

				rFlow_::Dismiss();

				return sOK;
			}
		}
	};

	typedef fdr::ioflow_driver___<>	rFlowDriver_;



	typedef flw::standalone_ioflow__<> sFlow_;

	class rIODriver
	: public rFlowDriver_
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
		virtual void FDRCommit( bso::sBool Unlock ) override
		{
			Proxy_.Commit( Unlock );
		}
		virtual fdr::sTID FDROTake( fdr::sTID Owner ) override
		{
			return Proxy_.ODriver().OTake( Owner );
		}
		virtual fdr::size__ FDRRead(
			fdr::size__ Maximum,
			fdr::byte__ *Buffer ) override
		{
			return Proxy_.ReadUpTo( Maximum, Buffer );
		}
		virtual void FDRDismiss( bso::sBool Unlock ) override
		{
			Proxy_.Dismiss( Unlock );
		}
		virtual fdr::sTID FDRITake( fdr::sTID Owner ) override
		{
			return Proxy_.IDriver().ITake( Owner );
		}
	public:
		void reset( bso::bool__ P = true )
		{
			rFlowDriver_::reset( P );
			Proxy_.reset( P );
		}
		qCVDTOR( rIODriver );
		eState Init(
			const char *HostService,
			const char *Identifier,
			prxybase::eType Type,
			sck::duration__ Timeout,
			qRPD )
		{
			reset();

			rFlowDriver_::Init( fdr::ts_Default );

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
