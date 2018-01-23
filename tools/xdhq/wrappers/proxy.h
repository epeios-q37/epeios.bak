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

	qENUM( Status ) {
		sNew,		// New connexion.
		sAction,	// Action to be handled.
		sPending,	// A response of an action has to be handled.
		s_amount,
		s_Undefined
	};

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
		mtx::rHandler Write_, Read_;
		rData *Data_;
	protected:
		rData *Read( void )
		{
			mtx::Lock( Read_ );

			rData *Data = Data_;

			mtx::Unlock( Write_ );

			if ( Data_ == NULL )
				qRFwk();

			Data_ = NULL;

			return Data;
		}
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
		qCDTOR( rSharing_ );
		void Init( void )
		{
			reset();

			Write_ = mtx::Create();
			Read_ = mtx::Create();

			Data_ = NULL;

			mtx::Lock( Read_ );
		}
		void Write( rData *Data )
		{
			mtx::Lock( Write_ );

			if ( Data_ != NULL )
				qRGnr();

			if ( Data == NULL )
				qRGnr();

			Data_ = Data;

			mtx::Unlock( Read_ );
		}
	};

	template <typename data> class rSharing
	: public rSharing_
	{
	public:
		data *Read( void )
		{
			return (data *)rSharing_::Read();
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
		virtual rData *PRXYNew( void ) = 0;
		virtual void PRXYDelete( rData *Data ) = 0;
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
		virtual rData *PRXYNew( void ) override
		{
			return new data;
		}
		virtual void PRXYDelete( rData *Data ) override
		{
			delete (data *)Data;
		}
	};

}

#endif