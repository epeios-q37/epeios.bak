/*
	Copyright (C) 2000-2015 Claude SIMON (http://q37.info/contact/).

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

#ifndef FRDBSE__INC
# define FRDBSE__INC

# define FRDBSE_NAME		"FRDBSE"

# if defined( E_DEBUG ) && !defined( FRDBSE_NODBG )
#  define FRDBSE_DBG
# endif

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

// FRonteNd BaSE

# include "err.h"
# include "flw.h"
# include "str.h"
# include "lcl.h"

namespace frdbse {
	enum backend_type__ {
		btNone,		// Use of no backend.
		btRemote,	// Remote, as a daemon.
		btEmbedded,	// Embedded, as a library.
		btPredefined,
		bt_amount,
		bt_Undefined
	};

	const char *GetLabel( backend_type__ BackendType );

	backend_type__ GetBackendType( const str::string_ &Pattern );

	template <typename t> class il_	// id, label.
	{
	public:
		struct s {
			t Id;
			str::string_::s Label;
		} &S_;
		str::string_ Label;
		il_( s &S )
		: S_( S ),
		  Label( S.Label )
		{}
		void reset( bso::bool__ P = true )
		{
			S_.Id = t(-1);
			Label.reset( P );
		}
		void plug( qAS_ &AS )
		{
			Label.plug( AS );
		}
		il_ &operator =(const il_ &IL)
		{
			S_.Id = IL.S_.Id;
			Label = IL.Label;

			return *this;
		}
		void Init(
			t Id,
			const str::string_ &Label )
		{
			S_.Id = Id;
			this->Label.Init( Label );
		}
		E_RODISCLOSE_( t, Id );
	};

	template <typename t> class ilw_	// id, label, wording.
	: public il_<t>
	{
	public:
		struct s
		: public il_<t>::s
		{
			str::string_::s Wording;
		};
		str::string_ Wording;
		ilw_( s &S )
		: il_<t>( S ),
		  Wording( S.Wording )
		{}
		void reset( bso::bool__ P = true )
		{
			il_<t>::reset( P );
			Wording.reset( P );
		}
		void plug( qAS_ &AS )
		{
			il_<t>::plug( AS );
			Wording.plug( AS );
		}
		ilw_ &operator =(const ilw_ &ILW)
		{
			il_<t>::operator = ( ILW );
			Wording = ILW.Wording;

			return *this;
		}
		void Init(
			t Id,
			const str::string_ &Label,
			const str::string_ &Wording )
		{
			il_<t>::Init( Id, Label );
			this->Wording.Init( Wording );
		}
	};

	E_ENUM( mode ) {
		mMonoUser,	// One use only. The content project (i.e. he can choose the backend) is defined by the user.
		mMultiUser,	// Several users. The project to use is (which defines the backend to use) is predefiend in the confifuration file.
		m_amount,
		m_Undefined
	};
}

				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
