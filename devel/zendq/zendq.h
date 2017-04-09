/*
	Copyright (C) 1999-2017 Claude SIMON (http://q37.info/contact/).

	This file is part of the Epeios framework.

	The Epeios framework is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

	The Epeios framework is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with the Epeios framework.  If not, see <http://www.gnu.org/licenses/>
*/

// (From Zend (PHP related framework)).

#ifndef ZENDQ_INC_
# define ZENDQ_INC_

# define ZENDQ_NAME		"ZENDQ"

# if defined( E_DEBUG ) && !defined( ZENDQ_NODBG )
#  define ZENDQ_DBG
# endif

# include "cpe.h"

# ifdef CPE_S_WIN
#  define ZEND_WIN32
#  define ZEND_WIN32_FORCE_INLINE
# endif

// Note to developer : include 'h:\hg\ZNDIncludeDirectories.props' in the '.vcxproj'.
# include "zend_API.h"

# include "bso.h"
# include "err.h"
# include "tol.h"

namespace zendq {
	inline void Get(
		zval *Val,
		long &Long )
	{
		Long = Z_LVAL_P( Val );
	}

	class sArray {
	private:
		HashTable *Table_;
	public:
		void reset( bso::sBool P = true )
		{
			tol::reset( P, Table_ );
		}
		qCDTOR( sArray );
		void Init( HashTable *Table = NULL )
		{
			Table_ = Table;
		}
		template <typename t> bso::sBool Get(
			int Index,
			t &Val,
			qRPD )
		{
			zval **ZVal = NULL;

			if ( Table_ == NULL )
				qRFwk();

			if ( zend_hash_index_find(Table_, Index, (void **)&ZVal) != SUCCESS ) {
				if ( qRPT )
					qRFwk();
				else
					return false;
			}

			zendq::Get( *ZVal, Val );

			return true;
		}
		template <typename t> bso::sBool Get(
			const char *Key,
			t &Val,
			qRPD )
		{
			zval **ZVal = NULL;

			if ( Table_ == NULL )
				qRFwk();

			if ( zend_hash_find( Table_, Key, strlen( Key ) + 1, (void **)&ZVal ) != SUCCESS ) {
				if ( qRPT )
					qRFwk();
				else
					return false;
			}

			zendq::Get( *ZVal, Val );

			return true;
		}
	};

	inline void Get(
		zval **Val,
		sArray &Array )
	{
		Array.Init( Z_ARRVAL_PP( Val ) );
	}

	template <typename t, int (* type_id)( void )> class sResource
	{
	private:
		t *Pointer_;
		void ***tsrm_ls_;
	protected:
		t *P_( void )
		{
			if ( Pointer_ == NULL )
				qRFwk();

			return Pointer_;
		}
		void ***T_( void )
		{
			if ( tsrm_ls_ == NULL )
				qRFwk();

			return tsrm_ls_;
		}
	public:
		static const char *TypeName_;
		int TypeId_;
		void reset( bso::sBool P = true )
		{
			tol::reset( P, Pointer_);
		}
		qCDTOR( sResource );
		void Init( void *Pointer = NULL )
		{
			TypeId_ = type_id();
			Pointer_ = (t *)Pointer;
		}
	};

	template <typename t, int (* type_id)( void )> inline void Get(
		zval **Val,
# ifdef ZTS
		void ***tsrm_ls,
# endif
		sResource<t, type_id> &Resource )
	{
# ifdef ZTS
		Resource.Init( zend_fetch_resource( Val, tsrm_ls, -1, Resource.TypeName_, NULL, 1, Resource.TypeId_ ) );
# else
		Resource.Init( zend_fetch_resource( Val, -1, Resource.TypeName_, NULL, 1, Resource.TypeId_ ) );
# endif
	}
}

#endif
