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
		ogzrcd::sFRow NewField_(
			ogzrcd::sRow Record,
			const ogzclm::rColumnBuffer &Column );
		void GetEntries_(
			const ogzfld::dField &Field,
			ogzbsc::dData &Entries ) const;
		void GetEntries_(
			ogzfld::sRow Field,
			ogzbsc::dData &Entries	) const;
		void GetEntries_(
			ogzrcd::sRow Record,
			ogzrcd::sFRow Field,
			ogzbsc::dData &Entries	) const
		{
			return GetEntries_( Records.GetRawFieldRow( Record, Field ), Entries );
		}
	public:
		ogzdta::sData Data;
		ogzclm::sColumns Columns;
		ogzfld::sFields Fields;
		ogzrcd::sRecords Records;
		ogzusr::sUsers Users;
		void reset( bso::bool__ P = true )
		{
			Data.reset( P );
			Columns.reset( P );
			Fields.reset( P );
			Records.reset( P );
			Users.reset( P );
		}
		qCDTOR( rDatabase );
		void Init(
			const ogztyp::dTypes &Types,
			ogzdta::cData &DTACallback,
			ogzclm::cColumn &CLMCallback,
			ogzfld::cField &FLDCallback,
			ogzrcd::cRecord &RCDCallback,
			ogzusr::cUser &USRCallback )
		{
			Data.Init( DTACallback );
			Columns.Init( CLMCallback );
			Fields.Init( FLDCallback );
			Records.Init( RCDCallback );
			Users.Init( USRCallback );
		}
		ogzusr::sRRow NewRecord( ogzusr::sRow User )
		{
			return Users.Add( Records.New(), User );
		}
		ogzrcd::sFRow NewField(
			ogzusr::sRow User,
			ogzusr::sRRow Record,
			const ogzclm::rColumnBuffer &Column )
		{
			return NewField_( Users.GetRawRecordRow( User, Record ), Column );
		}
		void GetEntries(
			ogzusr::sRow User,
			ogzusr::sRRow Record,
			ogzrcd::sFRow Field,
			ogzbsc::dData &Data	) const
		{
			return GetEntries_( Users.GetRawRecordRow( User, Record ), Field, Data );
		}
	};
# ifdef M
#  define OGZDTB_M_	M
#  #undef M
# endif

# define M( module, name )\
	protected:\
		virtual module::c##name &OGZDTBGet##name##Callback( void ) = 0;\
	public:\
		module::c##name &Get##name##Callback( void )\
		{\
			return OGZDTBGet##name##Callback();\
		}

	class cDatabase
	{
	protected:
		M( ogzdta, Data );
		M( ogzclm, Column );
		M( ogzfld, Field );
		M( ogzrcd, Record );
		M( ogzusr, User );
	public:
		qCALLBACK( Database );
	};

# ifdef OGZDTB_M_
#  define M OGZDTB_M_
#  undef OGZDTB_M_
# else
#  undef M
# endif

}

#endif
