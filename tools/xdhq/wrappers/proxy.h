/*
	Copyright (C) 2017 Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of 'XDHq' software.

    'XDHq' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'XDHq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'XDHq'.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef PROXY_INC_
# define PROXY_INC_

# include "prtcl.h"

# include "prxy_send.h"
# include "prxy_recv.h"

# include "csdscb.h"
# include "flw.h"
# include "str.h"

namespace proxy {
	using prxy_send::rArguments;
	using prxy_recv::rReturn;

	typedef tht::rReadWrite rControl_;

	// Data received from proxy.
	class rRecv
	: public rControl_
	{
	private:
	public:
		rReturn Return;
		str::wString	// For the launching of an action.
			Id,
			Action;
		void reset( bso::sBool P = true )
		{
			rControl_::reset( P );
			tol::reset( Return, Id, Action );
		}
		qCDTOR( rRecv );
		void Init( void )
		{
			rControl_::Init();
			tol::Init( Return, Id, Action );
		}
	};

	// Data sent to proxy.
	class rSent
	: public rControl_
	{
	public:
		rArguments Arguments;
		void reset( bso::sBool P = true )
		{
			rControl_::reset( P );
			tol::reset( P, Arguments );
		}
		qCDTOR( rSent );
		void Init( void )
		{
			rControl_::Init();
			tol::Init( Arguments );
		}
	};


	struct rData
	{
	public:
		rRecv Recv;
		rSent Sent;
		prxy_cmn::eRequest Request;
		str::wString Language;
		void reset( bso::sBool P = true )
		{
			tol::reset( P, Recv, Sent, Language );
			Request = prxy_cmn::r_Undefined;
		}
		qCDTOR( rData );
		void Init( void )
		{
			reset();

			tol::Init( Recv, Sent, Language );
			Request = prxy_cmn::r_Undefined;
		}
		bso::sBool IsTherePendingRequest( void ) const
		{
			return Request != prxy_cmn::r_Undefined;
		}
	};

	void Handshake_(
		flw::sRFlow &Flow,
		str::dString &Language );

	void GetAction_(
		flw::sRWFlow &Flow,
		str::dString &Id,
		str::dString &Action );

	template <typename data> class rProcessing
	: public csdscb::cProcessing
	{
	private:
		// Action to launch on a new session.
		str::wString NewSessionAction_;
	protected:
		virtual void *CSDSCBPreProcess(
			fdr::rRWDriver *IODriver,
			const ntvstr::char__ *Origin ) override
		{
			data *Data = NULL;
		qRH;
			flw::sDressedRWFlow<> Flow;
		qRB;
			Data = PRXYNew();

			if ( Data == NULL )
				qRAlc();

			Data->Recv.WriteDismiss();

			Flow.Init( *IODriver );
			Handshake_( Flow, Data->Language );

			prtcl::PutAnswer( prtcl::aOK_1, Flow );
			Flow.Commit();
		qRR;
			if ( Data != NULL )
				delete Data;
		qRT;
		qRE;
			return Data;
		}
		virtual csdscb::eAction CSDSCBProcess(
			fdr::rRWDriver *IODriver,
			void *UP ) override
		{
		qRH;
			flw::sDressedRWFlow<> Flow;
			data &Data = *(data *)UP;
		qRB;
			if ( UP == NULL )
				qRGnr();

			Flow.Init( *IODriver );

			if ( Data.Request != prxy_cmn::r_Undefined ) {
				Data.Recv.WriteBegin();
				Data.Recv.Return.Init();
				prxy_recv::Recv( Data.Request, Flow, Data.Recv.Return );
				Data.Request = prxy_cmn::r_Undefined;
				Data.Recv.WriteEnd();
				PRXYOnPending( &Data );
			} else {
				Data.Recv.WriteBegin();
				tol::Init( Data.Recv.Id, Data.Recv.Action );
				GetAction_( Flow, Data.Recv.Id, Data.Recv.Action );
				if ( Data.Recv.Action.Amount() == 0 ) {
					if ( Data.Recv.Id.Amount() == 0 )
						Data.Recv.Action = NewSessionAction_;
					else
						qRGnr();
				}
				Data.Recv.WriteEnd();
				PRXYOnAction( &Data );
			}

			Data.Sent.ReadBegin();

			// 'Data.Request' is set by the 'PRXYOn...' method above.
			if ( Data.Request != prxy_cmn::r_Undefined )
				prxy_send::Send( Data.Request, Flow, Data.Sent.Arguments );
			else
				prtcl::PutAnswer( prtcl::aOK_1, Flow );

			Data.Sent.ReadEnd();
		qRR;
		qRT;
		qRE;
			return csdscb::aContinue;
		}
		virtual bso::sBool CSDSCBPostProcess( void *UP ) override
		{
			data *Data = (data *)UP;

			if ( Data == NULL )
				qRGnr();

			delete Data;

			return false;
		}
		virtual data *PRXYNew( void ) = 0;
		virtual void PRXYOnAction( data *Data ) = 0;
		virtual void PRXYOnPending( data *Data ) = 0;
	public:
		void reset( bso::sBool P = true )
		{
			tol::reset( P, NewSessionAction_ );
		}
		qCVDTOR( rProcessing );
		void Init( const str::dString &NewSessionAction )
		{
			NewSessionAction_.Init( NewSessionAction );
		}
	};

	template <typename data> class rSharing
	{
	private:
		rControl_ Control_;
		data *Data_;
	public:
		void reset( bso::sBool P = true )
		{
			tol::reset( P, Control_, Data_ );
		}
		qCDTOR( rSharing );
		void Init( void )
		{
			Control_.Init();
			Data_ = NULL;
		}
		void Write( data *Data )
		{
			Control_.WriteBegin();

			if ( Data_ != NULL )
				qRGnr();

			if ( Data == NULL )
				qRGnr();

			Data_ = Data;

			Control_.WriteEnd();

		}
		data *Read( void )
		{
			Control_.ReadBegin();

			data *Data = Data_;

			if ( Data_ == NULL )
				qRFwk();

			Data_ = NULL;

			Control_.ReadEnd();

			return Data;
		}
	};
}

#endif