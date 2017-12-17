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

# include "xdh_ups.h"
# include "xdh_dws.h"

namespace xdh_cmn {
	struct rData
	{
	private:
		mtx::rHandler Lock_;
	public:
		xdh_dws::rJS JS;
		xdh_ups::rServer Server;
		sclnjs::rObject XDH;	// User overloaded 'XDH' JS class.
		str::wString Language;
		void reset( bso::sBool P = true )
		{
			if ( P ) {
				if ( Lock_ != mtx::UndefinedHandler )
					mtx::Delete( Lock_ );
			}

			tol::reset( P, JS, Server, XDH, Language );
			Lock_ = NULL;
		}
		qCDTOR( rData );
		void Init( void )
		{
			reset();

			tol::Init( JS, Server, XDH, Language );
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

	class rSharing
	{
	private:
		mtx::rHandler Server_, JS_;	// Server upstream, JS, server downstream.
		rData *Data_;
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
		qCDTOR( rSharing );
		void Init( void )
		{
			reset();

			Server_ = mtx::Create();
			JS_ = mtx::Create();

			Data_ = NULL;

			mtx::Lock( JS_ );
		}
		// Called by server.
		void Upstream( rData *Data )
		{
			mtx::Lock( Server_ );

			if ( Data_ != NULL )
				qRGnr();

			if ( Data == NULL )
				qRGnr();

			Data_ = Data;

			mtx::Unlock( JS_ );
		}
		rData &ReadJS( void )
		{
			mtx::Lock( JS_ );

			rData &Data = *Data_;

			mtx::Unlock( Server_ );

			if ( Data_ == NULL )
				qRFwk();

			Data_ = NULL;

			return Data;
		}
	};

	class rProcessing
	: public csdscb::cProcessing
	{
	private:
		qRMV( rSharing, S_, Sharing_ );
	protected:
		virtual void *CSDSCBPreProcess(
			fdr::rRWDriver *IODriver,
			const ntvstr::char__ *Origin ) override;
		virtual csdscb::eAction CSDSCBProcess(
			fdr::rRWDriver *IODriver,
			void *UP ) override;
		virtual bso::sBool CSDSCBPostProcess( void *UP ) override;
	public:
		void reset( bso::sBool P = true )
		{}
		qCVDTOR( rProcessing );
		void Init( rSharing &Sharing )
		{
			Sharing_ = &Sharing;
		}
	};

}

#endif
