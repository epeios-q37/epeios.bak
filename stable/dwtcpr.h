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

#ifndef DWTCPR__INC
# define DWTCPR__INC

# define DWTCPR_NAME		"DWTCPR"

# if defined( E_DEBUG ) && !defined( DWTCPR_NODBG )
#  define DWTCPR_DBG
# endif

// Directory WaTcher ComPaRison

# include "dwtbsc.h"
# include "dwtftr.h"

# include "err.h"
# include "str.h"
# include "dtr.h"

namespace dwtcpr {
	enum status__ {
		sSteady,	// Pas de modification.
		sCreated,
		sRemoved,
		sModified,
		s_amount,
		s_Undefined
	};

	// Version du contenu du fichiers XML.
	enum version__ {
		v0_1,
		v_amount,
		v_Current = v_amount - 1,
		v_Undefined
	};

	const char *GetLabel( version__ Version );

	version__ GetVersion( const str::string_ &Pattern );

	using dwtbsc::names_;
	E_AUTO( names );

	using dwtbsc::name_;
	E_AUTO( name );

	using dwtbsc::nrow__;


	class core__
	{
	public:
		nrow__ Name;
		status__ Status;
		void reset( bso::bool__ = true )
		{
			Name = qNIL;
			Status = s_Undefined;
		}
		void Init( void )
		{
			Name = qNIL;
			Status = s_Undefined;
		}
	};

	typedef bch::E_BUNCH_( core__ ) cores_;
	E_AUTO( cores );

	typedef core__ file__;

	qROW( frow__ );	// 'file row'.

	typedef bch::E_BUNCH_( frow__ ) frows_;
	E_AUTO( frows );

	qROW( drow__);	// 'dir row'.

	typedef bch::E_BUNCH_( drow__) drows_;
	E_AUTO( drows );

	typedef tol::object_<core__> core_;
	E_AUTO( core );

	class directory_
	: public core_
	{
	public:
		struct s
		: public core::s
		{
			frows_::s Files;
			drows_::s Dirs;
			nrow__ NewName;	// Nouveau nom et/ou nouveau chemin.
		} &S_;
		frows_ Files;
		drows_ Dirs;
		directory_(s &S)
		: S_( S ),
		  core_( S ),
		  Files( S.Files ),
		  Dirs( S.Dirs )
		{}
		void reset( bso::bool__ P = true )
		{
			core_::reset( P );
			Files.reset( P );
			Dirs.reset(P);

			S_.NewName = qNIL;
		}
		void plug( qAS_ &AS )
		{
			core_::plug( AS );
			Files.plug( AS );
			Dirs.plug( AS );
		}
		directory_ &operator =( const directory_ &D )
		{
			core_::operator =( D );
			Files = D.Files;
			Dirs = D.Dirs;

			S_.NewName = D.S_.NewName;

			return *this;
		}
		void Init( void )
		{
			core_::operator ()().Init();
			Files.Init();
			Dirs.Init();

			S_.NewName = qNIL;
		}
	};

	E_AUTO( directory );

	typedef bch::E_BUNCHt_( file__, frow__ ) files_;
	E_AUTO( files );

	typedef ctn::E_CONTAINERt_( directory_, drow__ ) directories_;
	E_AUTO( directories );

	class kernel_
	{
	public:
		struct s
		{
			files_::s Files;
			directories_::s Directories;
			names_::s Names;
		};
		files_ Files;
		directories_ Directories;
		names_ Names;
		kernel_( s &S )
		: Files( S.Files ),
		  Directories( S.Directories ),
		  Names( S.Names )
		{}
		void reset( bso::bool__ P = true )
		{
			Files.reset( P );
			Directories.reset( P );
			Names.reset( P );
		}
		void plug( qAS_ &AS )
		{
			Files.plug( AS );
			Directories.plug( AS );
			Names.plug( AS );
		}
		kernel_ &operator =( const kernel_ &K )
		{
			Files = K.Files;
			Directories = K.Directories;
			Names = K.Names;

			return *this;
		}
		void Init( void )
		{
			Files.Init();
			Directories.Init();
			Names.Init();
		}
	};

	E_AUTO( kernel );

	typedef dtr::E_DTREEt_( drow__ ) dtree_;
	E_AUTO( dtree );

	typedef dwtbsc::observer__ comparison_observer__;
	using dwtbsc::basic_observer___;

	class comparison_basic_observer___
	: public comparison_observer__,
	  public basic_observer___
	{
	protected:
		virtual void DWTBSCReport(
			bso::uint__ Handled,
			bso::uint__ ToHandle ) override
		{
			basic_observer___::Report_( Handled, ToHandle );
		}
	public:
		void reset( bso::bool__ P = true )
		{
			basic_observer___::reset( P );
			comparison_observer__::reset( P );
		}
		E_CVDTOR( comparison_basic_observer___ );
		void Init(
			const str::string &Message,
			txf::text_oflow__ &Flow,
			tol::delay__ Delay )
		{
			comparison_observer__::Init( Delay );
			basic_observer___::Init( Message, Flow );
		}
	};

	class scene_
	: public kernel_,
	  public dtree_
	{
	public:
		struct s
		: public kernel_::s,
		  public dtree_::s
		{};
		scene_( s &S )
		: kernel_( S ),
		  dtree_( S )
		{}
		void reset( bso::bool__ P = true )
		{
			kernel_::reset( P );
			dtree_::reset( P );
		}
		void plug( qAS_  &AS )
		{
			kernel_::plug( AS );
			dtree_::plug( AS );
		}
		scene_ &operator =( const scene_ &S )
		{
			kernel_::operator =( S );
			dtree_::operator =( S );

			return *this;
		}
		void Init( void )
		{
			kernel_::Init();
			dtree_::Init();
		}
		const str::string_ &GetPath(
			drow__ Row,
			str::string_ &Path ) const;
	};

	E_AUTO( scene );

	drow__ Compare(
		const dwtftr::kernel_ &Source,
		const dwtftr::kernel_ &Target,
		scene_ &Scene,
		comparison_observer__ &Observer );

	void Dump( 
		drow__ Row,
		const scene_ &Scene,
		xml::writer_ &Writer,
		version__ Version = v_Current );

	drow__ Clean(
		const scene_ &Scene,
		drow__ Root,
		scene_ &NewScene );	// Enlève les entrées ne nécessitant pas d'action.

	drow__ Clean(
		scene_ &Scene,
		drow__ Root );	// Enlève les entrées ne nécessitant pas d'action.

	typedef dwtbsc::observer__ load_observer__;

	drow__ Load(
		xml::parser___ &Parser,
		version__ Version,
		scene_ &Scene,
		load_observer__ &Observer );

	class basic_load_observer___
	: public load_observer__,
	  public basic_observer___
	{
	protected:
		virtual void DWTBSCReport(
			bso::uint__ Handled,
			bso::uint__ ToHandle ) override
		{
			basic_observer___::Report_( Handled, ToHandle );
		}
	public:
		void reset( bso::bool__ P = true )
		{
			basic_observer___::reset( P );
			load_observer__::reset( P );
		}
		E_CVDTOR( basic_load_observer___ );
		void Init(
			const str::string_ &Message,
			txf::text_oflow__ &Flow,
			tol::delay__ Delay )
		{
			load_observer__::Init( Delay );
			basic_observer___::Init( Message, Flow );
		}
	};
}

#endif
