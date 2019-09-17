/*
	Copyright (C) 1999 Claude SIMON (http://q37.info/contact/).

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

#ifndef FRDBSE_INC_
# define FRDBSE_INC_

# define FRDBSE_NAME		"FRDBSE"

# if defined( E_DEBUG ) && !defined( FRDBSE_NODBG )
#  define FRDBSE_DBG
# endif

// FRonteNd BaSE

# error "Obsolete ! Content moved into 'SCLFRNTND' !"

# include "fbltyp.h"

# include "err.h"
# include "flw.h"
# include "str.h"
# include "lcl.h"

namespace frdbse {
	E_ENUM( mode ) {
		mMonoUser,	// One use only. The content project (i.e. he can choose the backend) is defined by the user.
		mMultiUser,	// Several users. The project to use is (which defines the backend to use) is predefiend in the configuration file.
		m_amount,
		m_Undefined
	};

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

# define FRDBSE_I_( name, Name, id, Id  )\
	typedef fbltyp::id##__	name##_t__;\
	typedef fbltyp::id##s_	name##s_t_;\
	E_AUTO( name##s_t );\
\
	E_TMIMIC__( name##_t__, name##__ );\
	E_CDEF( name##__, Undefined##Name, fbltyp::Undefined##Id );\

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
		void plug( qSD__ &SD )
		{
			Label.plug( SD );
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

# define FRDBSE_IL( name, Name, id, Id  )\
	FRDBSE_I_( name, Name, id, Id );\
\
	typedef frdbse::il_<name##__> name##_;\
	E_AUTO( name );\
\
	typedef ctn::E_MCONTAINER_( name##_ ) name##s_;\
	E_AUTO( name##s );


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


# define FRDBSE_ILW( name, Name, id, Id  )\
	FRDBSE_I_( name, Name, id, Id );\
\
	typedef frdbse::ilw_<name##__> name##_;\
	E_AUTO( name );\
\
	typedef ctn::E_CONTAINER_( name##_ ) name##s_;\
	E_AUTO( name##s );

	template <typename ids_t, typename item, typename items_> inline void Fill(
		const ids_t &Ids,
		const fbltyp::strings_ &Labels,
		items_ &Items )
	{
	qRH
		ctn::E_CMITEM( fbltyp::string_ ) Label;
		sdr::row__ Row = qNIL;
		item Item;
	qRB
		Row = Ids.First();

		if ( Ids.Amount() != Labels.Amount() )
			qRGnr();

		Label.Init( Labels );

		while ( Row != qNIL ) {
			Item.Init( Ids(Row), Label(Row) );
			Items.Append( Item );

			Row = Ids.Next( Row );
		}
	qRR
	qRT
	qRE
	}

	template <typename ids_t, typename item, typename items_> inline void Fill(
		const ids_t &Ids,
		const fbltyp::strings_ &Labels,
		const fbltyp::strings_ &Wordings,
		items_ &Items )
	{
	qRH
		ctn::E_CMITEM( fbltyp::string_ ) Label;
		ctn::E_CMITEM( fbltyp::string_ ) Wording;
		sdr::row__ Row = qNIL;
		item Item;
	qRB
		Row = Ids.First();

		if ( Ids.Amount() != Labels.Amount() )
			qRGnr();

		if ( Ids.Amount() != Wordings.Amount() )
			qRGnr();

		Label.Init( Labels );
		Wording.Init( Wordings );

		while ( Row != qNIL ) {
			Item.Init( Ids(Row), Label(Row), Wording( Row ) );
			Items.Append( Item );

			Row = Ids.Next( Row );
		}
	qRR
	qRT
	qRE
	}

	E_CDEF( char *, AmountAttribute, "Amount" );
	E_CDEF( char *, IdAttribute, "id" );
	E_CDEF( char *, LabelAttribute, "label" );
	E_CDEF( char *, WordingAttribute, "Wording" );

	template <typename id> inline void Dump_(
		const frdbse::ilw_<id> &Item,
		xml::writer_ &Writer )
	{
		xml::PutAttribute( frdbse::IdAttribute, **Item.Id(), Writer );
		Writer.PutValue( Item.Label, frdbse::LabelAttribute );
		Writer.PutValue( Item.Wording, frdbse::WordingAttribute );
	}

	template <typename id> static void Dump_(
		const frdbse::il_<id> &Item,
		xml::writer_ &Writer )
	{
		xml::PutAttribute( frdbse::IdAttribute, **Item.Id(), Writer );
		Writer.PutValue( Item.Label, frdbse::LabelAttribute );
	}

	template <typename id, typename citem, typename items> static void Dump_(
		const items &Items,
		const char *ItemsLabel,
		const char *ItemLabel,
		xml::writer_ &Writer )
	{
		citem Item;
		sdr::row__ Row = Items.First();

		Item.Init( Items );

		Writer.PushTag( ItemsLabel );
		xml::PutAttribute( frdbse::AmountAttribute, Items.Amount(), Writer );

		while ( Row != qNIL ) {
			Writer.PushTag( ItemLabel );

			Dump_<id>( Item( Row ), Writer );

			Writer.PopTag();

			Row = Items.Next( Row );
		}

		Writer.PopTag();
	}

	template <typename id> static void Dump(
		const ctn::E_MCONTAINER_( frdbse::il_<id> ) &Items,
		const char *ItemsLabel,
		const char *ItemLabel,
		xml::writer_ &Writer )
	{
		Dump_<id, ctn::E_CMITEM( frdbse::il_<id> ), ctn::E_MCONTAINER_( frdbse::il_<id> )>( Items, ItemsLabel, ItemLabel, Writer );
	}

	template <typename id> static void Dump(
		const ctn::E_CONTAINER_( frdbse::ilw_<id> ) &Items,
		const char *ItemsLabel,
		const char *ItemLabel,
		xml::writer_ &Writer )
	{
		Dump_<id, ctn::E_CITEM( frdbse::ilw_<id> ), ctn::E_CONTAINER_( frdbse::ilw_<id> )>( Items, ItemsLabel, ItemLabel, Writer );
	}
}

#endif
