/*
	Copyright (C) 2017 by Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of XDHq.

    XDHq is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    XDHq is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with XDHq.  If not, see <http://www.gnu.org/licenses/>.
*/

// XDH CoMmoN.
// Merge upstream and downstream part.

#ifndef XDH_CMN_INC_
# define XDH_CMN_INC_

# include "proxy.h"

# include "csdscb.h"
# include "sclnjs.h"

namespace xdh_cmn {

	qENUM( Status ) {
		sNew,		// New connexion.
		sAction,	// Action to be handled.
		sPending,	// A response of an action has to be handled.
		s_amount,
		s_Undefined
	};

	struct rData_
	{
	private:
		mtx::rHandler Lock_;
	public:
		proxy::rReturn Return;
		str::wString	// For the launching of an action.
			Id,
			Action;
		proxy::eRequest Request;
		proxy::rArguments Arguments;
		str::wString Language;
		void reset( bso::sBool P = true )
		{
			if ( P ) {
				if ( Lock_ != mtx::UndefinedHandler )
					mtx::Delete( Lock_ );
			}

			tol::reset( P, Return, Id, Action, Arguments, Language );
			Request = proxy::r_Undefined;
			Lock_ = NULL;
		}
		qCDTOR( rData_ );
		void Init( void )
		{
			reset();

			tol::Init( Return, Id, Action, Arguments, Language );
			Request = proxy::r_Undefined;
			Lock_ = mtx::Create();
		}
		eStatus GetAndResetStatus( void )
		{
			if ( Request != prxy_cmn::r_Undefined ) {
				Request = prxy_cmn::r_Undefined;
				return sPending;
			} else if ( Action.Amount() != 0 )
				return sAction;
			else
				return sNew;
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

	class rSharing_
	{
	private:
		mtx::rHandler Server_, JS_;	// Server upstream, JS, server downstream.
		rData_ *Data_;
	public:
		void reset( bso::sBool P = true )
		{
			if ( P ) {
				if ( Server_ != mtx::UndefinedHandler )
					mtx::Delete( Server_ );

				if ( JS_ != mtx::UndefinedHandler )
					mtx::Delete( JS_ );
			}

			Server_ = JS_ = NULL;
			Data_ = NULL;
		}
		qCDTOR( rSharing_ );
		void Init( void )
		{
			reset();

			Server_ = mtx::Create();
			JS_ = mtx::Create();

			Data_ = NULL;

			mtx::Lock( JS_ );
		}
		// Called by server.
		void Upstream( rData_ *Data )
		{
			mtx::Lock( Server_ );

			if ( Data_ != NULL )
				qRGnr();

			if ( Data == NULL )
				qRGnr();

			Data_ = Data;

			mtx::Unlock( JS_ );
		}
		rData_ *ReadJS( void )
		{
			mtx::Lock( JS_ );

			rData_ *Data = Data_;

			mtx::Unlock( Server_ );

			if ( Data_ == NULL )
				qRFwk();

			Data_ = NULL;

			return Data;
		}
	};

	template <typename data> class rSharing
	: public rSharing_
	{
	public:
		void Upstream( data *Data )
		{
			rSharing_::Upstream( Data );
		}
		data *ReadJS( void )
		{
			return (data *)rSharing_::ReadJS();
		}
	};

	class rProcessing_
	: public csdscb::cProcessing
	{
	private:
		qRMV( rSharing_, S_, Sharing_ );
	protected:
		virtual void *CSDSCBPreProcess(
			fdr::rRWDriver *IODriver,
			const ntvstr::char__ *Origin ) override;
		virtual csdscb::eAction CSDSCBProcess(
			fdr::rRWDriver *IODriver,
			void *UP ) override;
		virtual bso::sBool CSDSCBPostProcess( void *UP ) override;
		virtual rData_ *PRXYNew( void ) = 0;
		virtual void PRXYDelete( rData_ *Data ) = 0;
	public:
		void reset( bso::sBool P = true )
		{}
		qCVDTOR( rProcessing_ );
		void Init( rSharing_ &Sharing )
		{
			Sharing_ = &Sharing;
		}
	};

	template <typename data> class rProcessing
	: public rProcessing_
	{
	protected:
		virtual rData_ *PRXYNew( void ) override
		{
			return new data;
		}
		virtual void PRXYDelete( rData_ *Data ) override
		{
			delete (data *)Data;
		}
	};

	struct rData
	: public rData_
	{
	public:
		sclnjs::rCallback Callback;
		sclnjs::rObject XDH;	// User overloaded 'XDH' JS class.
		void reset( bso::sBool P = true )
		{
			rData_::reset( P );
			tol::reset( P, Callback, XDH );
		}
		qCDTOR( rData );
		void Init( void )
		{
			rData_::reset();

			tol::Init( Callback, XDH );
		}
	};
}

#endif
