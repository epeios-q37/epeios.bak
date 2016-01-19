/*
	'common.h' by Claude SIMON (http://q37.info/contact/).

	 This file is part of 'orgnzq' software.

    'orgnzq' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'orgnzq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'orgnzq'.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef COMMON__INC
# define COMMON__INC

# include "ogzbsc.h"
# include "ogzinf.h"
# include "ogzdtb.h"
# include "ogzplg.h"


# include "fblbkd.h"

# include "sclbacknd.h"

# include "lck.h"

namespace common {
	typedef sclbacknd::backend___ xbackend___;	// // 'xbackend' ('eXtended' 'backend').

	class stuff___	// Contient les donnes propres  chaque 'backend', chaque connection (virtuelle) ayant son propre backend.
	{
	private:
		xbackend___ *_XBackend;
	public:
		void reset( bso::bool__ = true )
		{
			_XBackend = NULL;
		}
		E_CVDTOR( stuff___ );
		void Init( xbackend___ &XBackend )
		{
			_XBackend = &XBackend;
		}
		xbackend___ &XBackend( void ) const
		{
			if ( _XBackend == NULL )
				qRGnr();

			return *_XBackend;
		}
	};

	class rTypes
	{
	private:
		plgn::rRetrievers<ogzplg::fTypePlugin> Retrievers_;
	public:
		ogztyp::iTypes Core;
		void reset( bso::fBool P = true )
		{
			Core.reset( P );
			Retrievers_.reset( P );
		}
		qCDTOR( rTypes );
		void Init( void );
	};

	class rDatabase
	{
	private:
		plgn::rRetriever<ogzplg::fDatabasePlugin> Retriever_;
		ogzplg::fDatabasePlugin &P_( void )
		{
			return Retriever_.Plugin();
		}
	public:
		ogzdtb::rDatabase Core;
		void reset( bso::fBool P = true )
		{
			Core.reset( P );
			Retriever_.reset( P );
		}
		E_CDTOR( rDatabase );
		void Init( const ogztyp::vTypes &Types );
	};

	class naked_rack___ {
	public:
		rTypes Types;
		rDatabase Database;
	};

	typedef lck::control___<naked_rack___> rack___;

	typedef lck::read_write_access___<common::naked_rack___> rw_rack___;
	typedef lck::read_only_access___<common::naked_rack___> ro_rack___;

	extern rack___ Rack;

	void Initialize( void );

	bso::bool__ IsInitialized( void );
}



#endif
