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
	typedef sclbacknd::rBackend rBackend;	// // ('eXtended Backend').

	class rStuff	// Contains data peculiar to a backend, each (virtual) connection having its own backend.
	{
	private:
		rBackend *Backend_;
		ogzusr::fURow User_;
	public:
		void reset( bso::bool__ = true )
		{
			Backend_ = NULL;
			User_ = qNIL;
		}
		E_CVDTOR( rStuff );
		void Init( rBackend &Backend )
		{
			Backend_ = &Backend;
		}
		rBackend &Backend( void ) const
		{
			if ( Backend_ == NULL )
				qRGnr();

			return *Backend_;
		}
		void SetUser( ogzusr::fURow User )
		{
			User_ = User;
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
		void Init( const char *Identifier );
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
		void Init(
			const ogztyp::vTypes &Types,
			const char *Identifier );
	};

	class rAuthentication
	{
	private:
		plgn::rRetriever<ogzplg::fAuthenticationPlugin> Retriever_;
		ogzplg::fAuthenticationPlugin &P_( void )
		{
			return Retriever_.Plugin();
		}
	public:
		void reset( bso::fBool P = true )
		{
			Retriever_.reset( P );
		}
		qCDTOR( rAuthentication );
		void Init( const char *Identifier );
		ogzusr::fURow Login(
			const str::vString &Username,
			const str::vString &Password )
		{
			return P_().Authenticate( Username, Password );
		}
	};

	typedef ogzrcd::rRecordBuffer rRecord;

	class naked_rack___ {
	public:
		rTypes Types;
		rDatabase Database;
		rAuthentication Authentication;
		rRecord Record;
	};

	typedef lck::control___<naked_rack___> rack___;

	typedef lck::read_write_access___<common::naked_rack___> rw_rack___;
	typedef lck::read_only_access___<common::naked_rack___> ro_rack___;

	extern rack___ Rack;

	void Initialize( void );

	bso::bool__ IsInitialized( void );
}



#endif
