/*
	Copyright (C) 1999-2016 Claude SIMON (http://q37.info/contact/).

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

// Directory WaTcher BaSiCs

#ifndef DWTBSC__INC
# define DWTBSC__INC

# define DWTBSC_NAME		"DWTBSC"

# if defined( E_DEBUG ) && !defined( DWTBSC_NODBG )
#  define DWTBSC_DBG
# endif

# include "bso.h"
# include "err.h"
# include "str.h"
# include "stsfsm.h"
# include "ltf.h"

# ifdef M
#  define DWTBSC_M_ M
#  undef M
# endif

namespace dwtbsc {
	typedef bso::uint__ depth__;	// Type de la profondeur d'un répertoire.
	E_CDEF( depth__, DepthMax, bso::UIntMax );
	E_CDEF( depth__, UndefinedDepth, 0 );

	typedef bso::uint__ tamount__;	/// Thread amount.
	E_CDEF( tamount__, ThreadAmountMax, bso::UIntMax );

	typedef str::string_ name_;
	E_AUTO( name );

	typedef str::string_ oddity_;
	E_AUTO( oddity );

	E_ROW( nrow__ );	// Name row.
	E_ROW( orow__ );	// Oddity row.

	typedef ctn::E_MCONTAINERt_( name_, nrow__ ) names_;
	E_AUTO( names );

	typedef ctn::E_MCONTAINERt_( oddity_, orow__ ) oddities_;
	E_AUTO( oddities );

	enum exclusion__ {
		xNo,	// Not excluded.
		xMatchList,	// Excluded because it matches the list.,
		xGhost,	// Exclut parce que c'est un répertoire/fichier spécial (répertoire de renommage).
		xFileTooBig,	// Exclut parce que la taille di fichier dépasse la limite autorisée.
		xNameTooLong,	// Exclut parce que le nom du fichier/Répertoire est trop long.
		x_amount,
		x_Undefined,
	};

	enum exclusions_handling__ {
		ehNone,	// No exclusion at all (except ghost files).
		ehRegular,	// Comportemnt normal (conserve ou non les fichiers/dossiers exclus en fonction de la raisonde leur exclusion).
		ehSkip,	// Ignore les fichiers/dossiers exclus.
		ehKeep,	// Conserve fichiers/dossiers exclus (mais sont marqués comme tel).
		ehKeepGhostLike,	// Garde les émlements ressemblant à un 'ghost'. Pour la gestion des 'ghosts'.
		eh_amount,
		eh_Undefined,
		eh_Default = ehRegular
	};

	enum sort_type__ {
		stNone,
		stRegular,
		stReverse,
		st_amount,
		st_Default = stRegular,
		st_Undefined
	};

	const char *GetLabel( sort_type__ SortType );

	sort_type__ GetSortType( const str::string_ &Pattern );

	class common__
	{
	public:
		nrow__ Name;
		void reset( bso::bool__ = true )
		{
			Name = qNIL;
		}
		void Init( void )
		{
			Name = qNIL;
		}
	};

	class goof__	// Entrée dont in ne peut définir le type ('répertoire ou fichier').
	: public common__
	{
	public:
		orow__ Oddity;
		void reset( bso::bool__ P = true )
		{
			common__::reset( P );
			Oddity = qNIL;
		}
		E_CDTOR( goof__ );
		void Init( void )
		{
			common__::Init();
			Oddity = qNIL;
		}
	};

	E_ROW( grow__);

	typedef bch::E_BUNCHt_( goof__, grow__ ) goofs_;
	E_AUTO( goofs );

	typedef bch::E_BUNCH_( grow__) grows_;
	E_AUTO( grows );


	class regular__	// Base pour un fichier ou un répertoire.
	: public common__
	{
	public:
		time_t Timestamp;	// Last modification.
		exclusion__ Exclusion;
		void reset( bso::bool__ P = true )
		{
			common__::reset( P );
			Timestamp = 0;
			Exclusion = x_Undefined;
		}
		E_CDTOR( regular__ );
		void Init( void )
		{
			common__::Init();
			Timestamp = 0;
			Exclusion = x_Undefined;
		}
	};

	typedef bch::E_BUNCH_( common__ ) commons_;
	E_AUTO( commons );

	class file__
	: public regular__
	{
	public:
		fil::size__ Size;
		void reset( bso::bool__ P = true )
		{
			regular__::reset( P );

			Size = 0;
		}
		void Init( void )
		{
			regular__::Init();

			Size = 0;
		}
	};

	E_ROW( frow__ );	// 'file row'.

	typedef bch::E_BUNCH_( frow__ ) frows_;
	E_AUTO( frows );

	E_ROW( drow__);	// 'dir row'.

	typedef bch::E_BUNCH_( drow__) drows_;
	E_AUTO( drows );

	typedef tol::object_<regular__> regular_;
	E_AUTO( regular );

	class directory_
	: public regular_
	{
	public:
		struct s
		: public regular::s
		{
			grows_::s Goofs;
			frows_::s Files;
			drows_::s Dirs;
		} &S_;
		grows_ Goofs;
		frows_ Files;
		drows_ Dirs;
		directory_(s &S)
		: S_( S ),
		  regular_( S ),
		  Goofs( S.Goofs ),
		  Files( S.Files ),
		  Dirs( S.Dirs )
		{}
		void reset( bso::bool__ P = true )
		{
			regular_::reset( P );
			Goofs.reset( P );
			Files.reset( P );
			Dirs.reset(P);
		}
		void plug( qAS_ &AS )
		{
			regular_::plug( AS );
			Goofs.plug( AS );
			Files.plug( AS );
			Dirs.plug( AS );
		}
		directory_ &operator =( const directory_ &D )
		{
			regular_::operator =( D );
			Goofs = D.Goofs;
			Files = D.Files;
			Dirs = D.Dirs;

			return *this;
		}
		void Init( void )
		{
			regular_::operator()().Init();
			Goofs.Init();
			Files.Init();
			Dirs.Init();
		}
	};

	E_AUTO( directory );

	typedef bch::E_BUNCHt_( file__, frow__ ) files_;
	E_AUTO( files );

	typedef ctn::E_CONTAINERt_( directory_, drow__ ) directories_;
	E_AUTO( directories );

	class vKernel
	{
	public:
		struct s
		{
			goofs_::s Goofs;
			files_::s Files;
			directories_::s Directories;
			names_::s Names;
			oddities_::s Oddities;
		};
		goofs_ Goofs;
		files_ Files;
		directories_ Directories;
		names_ Names;
		oddities_ Oddities;
		vKernel( s &S )
		: Goofs( S.Goofs ),
		  Files( S.Files ),
		  Directories( S.Directories ),
		  Names( S.Names ),
		  Oddities( S.Oddities )
		{}
		void reset( bso::bool__ P = true )
		{
			Goofs.reset( P );
			Files.reset( P );
			Directories.reset( P );
			Names.reset( P );
			Oddities.reset( P );
		}
		void plug( qAS_ &AS )
		{
			Goofs.plug( AS );
			Files.plug( AS );
			Directories.plug( AS );
			Names.plug( AS );
			Oddities.plug( AS );
		}
		vKernel &operator =( const vKernel &K )
		{
			Goofs = K.Goofs;
			Files = K.Files;
			Directories = K.Directories;
			Names = K.Names;
			Oddities = K.Oddities;

			return *this;
		}
		void Init( void )
		{
			Goofs.Init();
			Files.Init();
			Directories.Init();
			Names.Init();
			Oddities.Init();
		}
	};

	qW( Kernel );

	template <typename hook> inline void CreateFilesIfMissing_( hook &Hook )
	{
		if ( !Hook.Exists() )
			Hook.CreateFiles();
	}

# define M( lib, name )\
	protected:\
		virtual lib::fHook &DWTBSCGet##name##Hook( void ) = 0;\
	public:\
		lib::fHook &Get##name##Hook( void ) { return DWTBSCGet##name##Hook(); }

	class fHook
	{
	public:
		qCALLBACK_DEF( Hook );
		M( bch, Goofs );
		M( bch, Files);
		M( ctn, Directories );
		M( ctn, Names );
		M( ctn, Oddities );
	};

# undef M

	inline bso::fBool Plug(
		vKernel &Kernel,
		fHook  &Hook )
	{
		bso::fBool Exists = false;

# define M( lib, name ) Exists |= lib::Plug( Kernel.name, Hook.Get##name##Hook() )
		M( bch, Goofs );
		M( bch, Files);
		M( ctn, Directories );
		M( ctn, Names );
		M( ctn, Oddities );
#undef M

		return Exists;
	}

	struct rHF
	{
	public:
		bch::rHF Goofs;
		bch::rHF Files;
		ctn::rHF Directories;
		ctn::rHF Names;
		ctn::rHF Oddities;
		void reset( bso::bool__ P = true )
		{
			Goofs.reset( P );
			Files.reset( P );
			Directories.reset( P );
			Names.reset( P );
			Oddities.reset( P );
		}
		E_CDTOR( rHF );
		void Init(
			const fnm::name___ &Path,
			const fnm::name___ &Basename );
	};

	class rFH
	: public fHook
	{
	protected:
# define M( lib, name )	virtual lib::fHook &DWTBSCGet##name##Hook( void ) override { return name##_; }
		M( bch, Goofs );
		M( bch, Files);
		M( ctn, Directories );
		M( ctn, Names );
		M( ctn, Oddities );
#undef M
	private:
		bch::rFH<goofs_> Goofs_;
		bch::rFH<files_> Files_;
		ctn::rFH<directories_> Directories_;
		ctn::rFH<names_> Names_;
		ctn::rFH<oddities_> Oddities_;
	public:
		void reset( bso::bool__ P = true )
		{
			Goofs_.reset( P );
			Files_.reset( P );
			Directories_.reset( P );
			Names_.reset( P );
			Oddities_.reset( P );
		}
		E_CDTOR( rFH );
		uys::eState Init(
			rHF &Filenames,
			const vKernel &Kernel,
			uys::mode__ Mode,
			uys::behavior__ Behavior,
			flsq::id__ ID )
		{
			uys::eState State = Goofs_.Init( Filenames.Goofs, Kernel.Goofs, Mode, Behavior, ID );

# define M( lib, name ) \
			if ( !State.IsError() ) {\
				if ( name##_.Init( Filenames.name, Kernel.name, Mode, Behavior, ID ) != State )\
					State = uys::sInconsistent;\
			}

			M( bch, Files);
			M( ctn, Directories );
			M( ctn, Names );
			M( ctn, Oddities );
# undef M
			return State;
		}
	};

	class ghosts_oddities_	// Elements pour gèrer le renommage de dossiers.
	{
	public:
		struct s {
			str::string_::s
				Prefix,
				Suffix;
		};
		str::string_
			Prefix,
			Suffix;
		ghosts_oddities_( s &S )
		: Prefix( S.Prefix ),
		  Suffix( S.Suffix )
		{}
		void reset( bso::bool__ P = true )
		{
			Prefix.reset( P );
			Suffix.reset( P );
		}
		void plug( qAS_ &AS )
		{
			Prefix.plug( AS );
			Suffix.plug( AS );
		}
		ghosts_oddities_ &operator =(const ghosts_oddities_ &GO)
		{
			Prefix = GO.Prefix;
			Suffix = GO.Suffix;

			return *this;
		}
		void Init( void )
		{
			Prefix.Init();
			Suffix.Init();
		}
	};

	E_AUTO( ghosts_oddities );

	const fnm::name___ &GetGhostLocalizedName(
		sdr::row_t__ Row,
		const str::string_ &Root,
		const str::string_ &Path,
		const ghosts_oddities_ &GO,
		fnm::name___ &LocalizedName );

	const fnm::name___ &GetGhostsDataDirName(
		const str::string_ &Root,
		const ghosts_oddities_ &GO,
		fnm::name___ &Name );

	struct limitations__
	{
	public:
		bso::size__ NameLengthMax;
		fdr::size__ FileSizeMax;
		void reset( bso::bool__ = true )
		{
			NameLengthMax = 0;
			FileSizeMax = 0;
		}
		E_CDTOR( limitations__ );
		void Init( void )
		{
			NameLengthMax = 0;
			FileSizeMax = 0;
		}
	};

	class observer__
	: public tol::timer__
	{
	protected:
		virtual void DWTBSCReport(
			bso::uint__ Handled,
			bso::uint__ Total ) = 0;
	public:
		void Report(
			bso::uint__ Handled,
			bso::uint__ Total )
		{
			if ( IsEnabled() ) {
				DWTBSCReport( Handled, Total );
				Launch();
			}
		}
	};

	class basic_observer___
	{
	protected:
		void Report_(
			bso::uint__ Handled,
			bso::uint__ Total );
	private:
		str::string _Message;
		ltf::line_text_oflow___<> _Flow;
	public:
		void reset( bso::bool__ P = true )
		{
			_Message.Init();
			_Flow.reset( P );
		}
		E_CVDTOR( basic_observer___ );
		void Init(
			const str::string_ &Message,
			txf::text_oflow__ &TFlow )
		{
			_Message.Init( Message );
			_Flow.Init( TFlow );
		}
	};

	enum root_fate__ {
		rfKeep,	// Le répertoire racine est effacé
		rfDelete,	// Le répertoire racine est conservé.
		rf_amount,
		rf_Undefined,
		rf_Default = rfDelete, 
	};

	bso::bool__ Delete(
		const str::string_ &Path,
		tamount__ ThreadAmountMax,
		root_fate__ RootFate = rf_Default );

	bso::bool__ Delete(
		const str::string_ &Root,
		const str::string_ &Path,
		tamount__ ThreadAmountMax );

	bso::fBool Delete(
		const str::vString &Root,
		const str::vString &Path,
		const str::vString &Filename );

	bso::bool__ Delete(
		const str::string_ &Path,
		const str::strings_ &FileNames );
}

# ifdef DWTBSC_M_
#  define M DWTBSC_M_
# endif


#endif
