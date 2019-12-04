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

#ifndef COMMON_INC_
# define COMMON_INC_

# include "ogzbsc.h"
# include "ogzinf.h"
# include "ogzdtb.h"
# include "ogzplg.h"


# include "fblbkd.h"

# include "sclbacknd.h"

# include "lck.h"

namespace common {
	qENUM( Message ) {
		mBadEntryValue,
		mFieldNameAlreadyUsed,
		mNoSuchEntry,
		mNoSuchField,
		mNoSuchRecord,
		mFieldLabelCanNotBeEmpty,
		mOnlyOneEntryAllowed,
		mTestMessage,
		mUnknownEntry,
		mUnknownFieldNumber,
		mUnknowFieldType,
		mInconsistentArguments,
		m_amount,
		m_Undefined
	};

	const char *GetLabel( eMessage Message );

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
		ogzusr::sRow User( void ) const
		{
			if ( User_ == qNIL )
				qRGnr();

			return User_;
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
			MandatoryRecordType_ = MandatoryTextType_ = qNIL;
		}
		qCDTOR( rTypes );
		void Init( const char *Identifier );
		qRODISCLOSEs( ogztyp::dTypes, Core );
		qRODISCLOSEr( ogztyp::sRow, MandatoryRecordType );
		qRODISCLOSEr( ogztyp::sRow, MandatoryTextType );
	};

	typedef ogzdtb::mDatabase mDatabase_;

	class rDatabase
	: public mDatabase_
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
			mDatabase_::reset( P );
			Retriever_.reset( P );
		}
		E_CDTOR( rDatabase );
		void Init(
			const ogztyp::dTypes &Types,
			const char *Identifier );
	};

	typedef ogzusr::mAuthentication mAuthentication_;

	class rAuthentication
	: public mAuthentication_
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
			mAuthentication_::reset(P );
			Retriever_.reset( P );
		}
		qCDTOR( rAuthentication );
		void Init(
			const char *Identifier,
			ogzusr::mUsers &Users );
	};

	ogzdtb::mDatabase &Database( void );
	ogzusr::mAuthentication &Authentication( void );

	void Initialize( void );

	bso::bool__ IsInitialized( void );

	const ogztyp::dTypes &GetTypes( void );

	ogztyp::sRow GetMandatoryRecordType( void  );
	ogztyp::sRow GetMandatoryTextType( void  );
}

# define REPORT( message ) sclmisc::ReportAndAbort( common::GetLabel( common::m##message ) )

# define BACKEND_	((sclbacknd::rBackend *)BaseBackend.UP())
# define STUFF_		(((common::rStuff *)(BACKEND_->Stuff())))

# define BACKEND		sclbacknd::rBackend &Backend = *BACKEND_
# define STUFF			common::rStuff &Stuff = *STUFF_
# define USER			const ogzusr::sRow &User = STUFF_->User()
# define DATABASE		ogzdtb::mDatabase &Database = common::Database()
# define AUTHENTICATION	ogzusr::mAuthentication &Authentication = common::Authentication()

#endif
