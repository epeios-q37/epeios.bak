/*
	Copyright (C) 2015-2016 Claude SIMON (http://q37.info/contact/).

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

// OrGaniZer DaTaBase

#ifndef OGZDTB__INC
# define OGZDTB__INC

# ifdef XXX_DBG
#	define OGZDTB__DBG
# endif

# include "ogzbsc.h"
# include "ogzdta.h"
# include "ogztyp.h"
# include "ogzclm.h"
# include "ogzrcd.h"
# include "ogzusr.h"

namespace ogzdtb {
	class rDatabase
	{
	private:
		ogztyp::fTRow TextType_, RecordType_;	// The 2 types which have to exists.
		void FetchMandatoryTypes_( void );
	public:
		ogzdta::fData Data;
		ogztyp::iTypes Types;
		ogzclm::fColumns Columns;
		ogzfld::fFields Fields;
		ogzrcd::fRecords Records;
		ogzusr::fUsers Users;
		void reset( bso::bool__ P = true )
		{
			TextType_ = RecordType_ = qNIL;
			Data.reset( P );
			Types.reset( P );
			Columns.reset( P );
			Fields.reset( P );
			Records.reset( P );
			Users.reset( P );
		}
		qCDTOR( rDatabase );
		void Init(
			const ogztyp::vTypes &Types,
			ogzdta::fCallback &DTACallback,
			ogzclm::fCallback &CLMCallback,
			ogzfld::fCallback &FLDCallback,
			ogzrcd::fCallback &RCDCallback,
			ogzusr::fCallback &USRCallback )
		{
			this->Types.Init();
			this->Types = Types;

			Data.Init( DTACallback );
			Columns.Init( CLMCallback );
			Fields.Init( FLDCallback );
			Records.Init( RCDCallback );
			Users.Init( USRCallback );

			FetchMandatoryTypes_();
		}
	};
# ifdef M
#  define OGZDTB_M_	M
#  #undef M
# endif

# define M( module, name )\
	protected:\
		virtual module::fCallback &OGZDTBGet##name( void ) = 0;\
	public:\
		module::fCallback &Get##name( void )\
		{\
			return OGZDTBGet##name();\
		}

	class fCallback
	{
	protected:
		M( ogzdta, DTA );
		M( ogzclm, CLM );
		M( ogzfld, FLD );
		M( ogzrcd, RCD );
		M( ogzusr, USR );
	public:
		void reset( bso::fBool = true )
		{
			// Standardization.
		}
		qCVDTOR( fCallback );
		void Init( void )
		{
			// Standardization.
		}
	};

# ifdef OGZDTB_M_
#  define M OGZDTB_M_
#  undef OGZDTB_M_
# endif

}

#endif
