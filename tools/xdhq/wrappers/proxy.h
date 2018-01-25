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

	struct rData
	{
	private:
		mtx::rHandler Lock_;
	public:
		rReturn Return;
		str::wString	// For the launching of an action.
			Id,
			Action;
		prxy_cmn::eRequest Request;
		rArguments Arguments;
		str::wString Language;
		void reset( bso::sBool P = true )
		{
			if ( P ) {
				if ( Lock_ != mtx::UndefinedHandler )
					mtx::Delete( Lock_ );
			}

			tol::reset( P, Return, Id, Action, Arguments, Language );
			Request = prxy_cmn::r_Undefined;
			Lock_ = NULL;
		}
		qCDTOR( rData );
		void Init( void )
		{
			reset();

			tol::Init( Return, Id, Action, Arguments, Language );
			Request = prxy_cmn::r_Undefined;
			Lock_ = mtx::Create();
		}
		void Lock( void )
		{
			mtx::Lock( Lock_ );
		}
		void Unlock( void )
		{
			mtx::Unlock( Lock_ );
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

			Flow.Init( *IODriver );
			Handshake_( Flow, Data->Language );

			Data->Lock();

			Data->Lock();
			Data->Unlock();

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

			Data.Return.Init();

			if ( prxy_recv::Recv( Data.Request, Flow, Data.Return ) ) {
				Data.Request = prxy_cmn::r_Undefined;
				Data.Lock();
				PRXYOnPending( &Data );
			}  else {
				GetAction_( Flow, Data.Id, Data.Action );
				Data.Lock();
				PRXYOnAction( &Data );
			}

			Data.Lock();
			Data.Unlock();

			if ( !prxy_send::Send( Data.Request, Flow, Data.Arguments ) )
				prtcl::PutAnswer( prtcl::aOK_1, Flow );
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
		{}
		qCVDTOR( rProcessing );
		void Init( void )
		{}
	};

	template <typename data> class rSharing
	{
	private:
		mtx::rHandler Write_, Read_;
		data *Data_;
	public:
		void reset( bso::sBool P = true )
		{
			if ( P ) {
				if ( Write_ != mtx::UndefinedHandler )
					mtx::Delete( Write_ );

				if ( Read_ != mtx::UndefinedHandler )
					mtx::Delete( Read_ );
			}

			Write_ = Read_ = NULL;
			Data_ = NULL;
		}
		qCDTOR( rSharing );
		void Init( void )
		{
			reset();

			Write_ = mtx::Create();
			Read_ = mtx::Create();

			Data_ = NULL;

			mtx::Lock( Read_ );
		}
		void Write( data *Data )
		{
			mtx::Lock( Write_ );

			if ( Data_ != NULL )
				qRGnr();

			if ( Data == NULL )
				qRGnr();

			Data_ = Data;

			mtx::Unlock( Read_ );
		}
		data *Read( void )
		{
			mtx::Lock( Read_ );

			data *Data = Data_;

			mtx::Unlock( Write_ );

			if ( Data_ == NULL )
				qRFwk();

			Data_ = NULL;

			return Data;
		}
	};
}

#endif