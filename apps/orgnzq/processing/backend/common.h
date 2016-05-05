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
		ogzusr::sRow User_;
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
		void SetUser( ogzusr::sRow User )
		{
			User_ = User;
		}
	};

	class rTypes
	{
	private:
		plgn::rRetrievers<ogzplg::fTypePlugin> Retrievers_;
		ogztyp::wTypes Core_;
		ogztyp::sRow
			MandatoryRecordType_,
			MandatoryTextType_;
		void FetchMandatoryTypes_( void );
	public:
		void reset( bso::sBool P = true )
		{
			Core_.reset( P );
			Retrievers_.reset( P );
			MandatoryRecordType_, MandatoryTextType_ = qNIL;
		}
		qCDTOR( rTypes );
		void Init( const char *Identifier );
		qRODISCLOSEs( ogztyp::dTypes, Core );
		qRODISCLOSEr( ogztyp::sRow, MandatoryRecordType );
		qRODISCLOSEr( ogztyp::sRow, MandatoryTextType );
	};

	typedef ogzdtb::rDatabase rDatabase_;

	class rDatabase
	: public rDatabase_
	{
	private:
		plgn::rRetriever<ogzplg::fDatabasePlugin> Retriever_;
		ogzplg::fDatabasePlugin &P_( void )
		{
			return Retriever_.Plugin();
		}
	public:
		void reset( bso::sBool P = true )
		{
			rDatabase_::reset( P );
			Retriever_.reset( P );
		}
		E_CDTOR( rDatabase );
		void Init(
			const ogztyp::dTypes &Types,
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
		void reset( bso::sBool P = true )
		{
			Retriever_.reset( P );
		}
		qCDTOR( rAuthentication );
		void Init( const char *Identifier );
		ogzusr::sRow Login(
			const str::dString &Username,
			const str::dString &Password )
		{
			return P_().Authenticate( Username, Password );
		}
	};

	typedef ogzrcd::rRecordBuffer rRecord;

	class rNakedRack {
	public:
		rDatabase Database;
		rAuthentication Authentication;
	};

	typedef lck::rControl<rNakedRack> rRack;

	typedef lck::rReadWriteAccess<common::rNakedRack> rRWLock;
	typedef lck::rReadOnlyAccess<common::rNakedRack> rROLock;

	extern rRack Rack;

	void Initialize( void );

	bso::bool__ IsInitialized( void );

	const ogztyp::dTypes &GetTypes( void );

	ogztyp::sRow GetMandatoryRecordType( void  );
	ogztyp::sRow GetMandatoryTextType( void  );
}

# define RWL common::rRWLock Lock

# define RWR\
	Lock.Init( common::Rack );\
	common::rNakedRack &Rack = Lock()

# define ROL common::rROLock Lock

# define ROR\
	Lock.Init( common::Rack );\
	const common::rNakedRack &Rack = Lock()

#endif
