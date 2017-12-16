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

// XDH UPStream.
// Stuff related to the server part.

#ifndef XDH_UPS_INC_
# define XDH_UPS_INC_

# include "csdscb.h"
# include "sclnjs.h"

namespace xdh_ups {
	namespace js {
		struct rJS
		{
		public:
			sclnjs::rCallback *Callback;
			sclnjs::wArguments Arguments;
			void reset( bso::sBool P = true )
			{
				Callback = NULL;
				Arguments.reset( P );
			}
			qCDTOR( rJS );
			void Init( void )
			{
				Callback = NULL;
				Arguments.Init();
			}
		};
	}

	namespace server {
		qENUM( Request )
		{
			rSetLayout,
				r_amount,
				r_Undefined
		};

		struct rArguments
		{
		public:
			str::wString Id, XML, XSL, Language;
			void reset( bso::sBool P = true )
			{
				tol::reset( Id, XML, XSL, Language );
			}
			qCDTOR( rArguments );
			void Init( void )
			{
				tol::Init( Id, XML, XSL, Language );
			}
		};

		struct rServer
		{
		public:
			eRequest Id;
			rArguments Arguments;
			void reset( bso::sBool P = true )
			{
				Id = r_Undefined;
				Arguments.reset( P );
			}
			qCDTOR( rServer );
			void Init( void )
			{
				Id = r_Undefined;
				Arguments.Init();
			}
		};
	}

	struct rData
	{
	public:
		js::rJS JS;
		server::rServer Server;
		void reset( bso::sBool P = true )
		{
			tol::reset( P, JS, Server );
		}
		qCDTOR( rData );
		void Init( void )
		{
			tol::Init( JS, Server );
		}
	};

	class rSharing
	{
	private:
		mtx::rHandler Set_, Get_, Completion_;
		rData *Data_;
	public:
		void reset( bso::sBool P = true )
		{
			if ( P ) {
				if ( Set_ != mtx::UndefinedHandler )
					mtx::Delete( Set_ );

				if ( Get_ != mtx::UndefinedHandler )
					mtx::Delete( Get_ );

				if ( Completion_ != mtx::UndefinedHandler )
					mtx::Delete( Completion_ );
			}

			Set_ = Get_ = Completion_ = NULL;
			Data_ = NULL;
		}
		qCDTOR( rSharing );
		void Init( void )
		{
			reset();

			Set_ = mtx::Create();
			Get_ = mtx::Create();
			Completion_ = mtx::Create();

			Data_ = NULL;

			mtx::Lock( Get_ );
		}
		void SetData( rData *Data )
		{
			mtx::Lock( Set_ );

			if ( Data_ != NULL )
				qRGnr();

			if ( Data == NULL )
				qRGnr();

			Data_ = Data;

			mtx::Unlock( Get_ );
			mtx::Lock( Set_ );
			mtx::Lock( Completion_ );
		}
		rData &GetData( void )
		{
			mtx::Lock( Get_ );

			rData *Data = Data_;

			if ( Data == NULL )
				qRGnr();

			mtx::Unlock( Set_ );

			mtx::Lock( Get_ );

			return *Data;
		}
		void ReportCompletion( void )
		{
			mtx::Unlock( Completion_ );
		}
		void WaitForCompletion( void )
		{
			mtx::Lock( Completion_ );
		}
	};

	class rProcessing
	: public csdscb::cProcessing
	{
	private:
		qRMV( rSharing, S_, Sharing_ );
	protected:
		virtual void *CSDSCBPreProcess(
			fdr::rIODriver *IODriver,
			const ntvstr::char__ *Origin ) override;
		virtual csdscb::eAction CSDSCBProcess(
			fdr::rIODriver *IODriver,
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
