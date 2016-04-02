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

#ifndef DWTFTR__INC
# define DWTFTR__INC

# define DWTFTR_NAME		"DWTFTR"

# if defined( E_DEBUG ) && !defined( DWTFTR_NODBG )
#  define DWTFTR_DBG
# endif

// Directory WaTcher File TRee

# include "dwtbsc.h"
# include "dwtdct.h"

# include "err.h"
# include "str.h"
# include "dtr.h"
# include "xml.h"

/***************/
/***** OLD *****/
/***************/

namespace dwtftr {
	using namespace dwtbsc;

	// XML flow content version.
	enum version__ {
		v0_1,
		v0_2,
		v0_3,
		v_amount,
		v_Current = v_amount - 1,
		v_Undefined
	};

	const char *GetLabel( version__ Version );

	version__ GetVersion( const str::string_ &Pattern );

	typedef dtr::E_DTREEt_( dwtbsc::drow__ ) dtree_;
	E_AUTO( dtree );

	struct file_tree_hf___
	{
	public:
		dtr::rHF Tree;
		dwtbsc::rHF Kernel;
		void reset( bso::bool__ P = true )
		{
			Tree.reset( P );
			Kernel.reset( P );
		}
		E_CDTOR( file_tree_hf___ );
		void Init(
			const fnm::name___ &Path,
			const fnm::name___ &Basename );
	};

	struct file_tree_fh___
	{
	public:
		dtr::rFH Tree;
		dwtbsc::rFH Kernel;
		void reset( bso::bool__ P = true )
		{
			Tree.reset( P );
			Kernel.reset( P );
		}
		E_CDTOR( file_tree_fh___ );
		uys::eState Init(
			file_tree_hf___ &Filenames,
			uys::mode__ Mode,
			uys::behavior__ Behavior,
			flsq::id__ ID )
		{
			uys::eState State = Tree.Init( Filenames.Tree, Mode, Behavior, ID );

			if ( !State.IsError() ) {
				if ( Kernel.Init( Filenames.Kernel, Mode, Behavior, ID ) != State )
					State = uys::sInconsistent;
			}

			return State;
		}
	};


	class file_tree_
	: public dKernel,
	  public dtree_
	{
	public:
		struct s
		: public dKernel::s,
		  public dtree_::s
		{};
		file_tree_( s &S )
		: dKernel( S ),
		  dtree_( S )
		{}
		void reset( bso::bool__ P = true )
		{
			dKernel::reset( P );
			dtree_::reset( P );
		}
		void plug( file_tree_fh___ &Hook )
		{
			dKernel::plug( Hook.Kernel );
			dtree_::plug( Hook.Tree );
		}
		void plug( qASd *AS )
		{
			dKernel::plug( AS );
			dtree_::plug( AS );
		}
		file_tree_ &operator =( const file_tree_ &FT )
		{
			dKernel::operator =( FT );
			dtree_::operator =( FT );

			return *this;
		}
		void Init( void )
		{
			dKernel::Init();
			dtree_::Init();
		}
		const str::string_ &GetPath(
			drow__ Row,
			str::string_ &Path ) const;
		 void Dump( 
			 drow__ Row,
			 xml::writer_ &Writer ) const;
	};

	E_AUTO( file_tree );

	struct file_tree_rack___ {
		file_tree_fh___ Hook;
		file_tree Tree;
		flsq::rId Id;
		void reset( bso::bool__ P = true )
		{
			Hook.reset( P );
			Tree.reset( P );

			if ( P ) {
				if ( Id != flsq::Undefined )
					flsq::ReleaseId( Id );
			}

			Id = flsq::Undefined;
		}
		E_CDTOR( file_tree_rack___ );
		void Init( void )
		{
			Hook.reset();
			Tree.reset();

			if ( Id != flsq::Undefined )
				flsq::ReleaseId( Id );

			Id = flsq::GetId();
			// L'initialisation proprement dite sera réalisée par des fonctions dédiées.
		}
	};

	file_tree_ &GetRWFileTree(
		const str::string_ &Root,
		const dwtbsc::ghosts_oddities_ &GO,
		file_tree_rack___ &Rack );

	const file_tree_ &GetROFileTree(
		const str::string_ &Root,
		const dwtbsc::ghosts_oddities_ &GO,
		file_tree_rack___ &Rack );

	typedef dwtbsc::observer__ processing_observer__;
	
	drow__ Process(
		const dwtdct::content_ &Content,
		file_tree_ &Tree,
		processing_observer__ &Observer );

	typedef bch::E_BUNCHt_( grow__, drow__ ) d2g_;
	E_AUTO( d2g );

	void Fill(
		const dwtdct::i2d_ &I2D,
		const dwtdct::content_ &Content,
		d2g_ &D2G );

	typedef dwtbsc::observer__ load_observer__;

	drow__ Load(
		xml::parser___ &Parser,
		version__ Version,
		dKernel &Kernel,
		dtree_ &Tree,
		load_observer__ &Observer );

	using dwtbsc::basic_observer___;

	class basic_processing_observer___
	: public processing_observer__,
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
			processing_observer__::reset( P );
		}
		E_CVDTOR( basic_processing_observer___ );
		void Init(
			tol::delay__ Delay,
			const str::string_ &Message,
			txf::text_oflow__ &Flow )
		{
			processing_observer__::Init( Delay );
			basic_observer___::Init( Message, Flow );
		}
	};

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
			load_observer__::reset( P );
			basic_observer___::reset( P );
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

	void Sort(
		dKernel &Tree,
		dwtbsc::sort_type__ SortType );

	void Sort(
		drow__ Root,
		file_tree_ &Tree,
		dwtbsc::sort_type__ SortType );
}

/***************/
/***** NEW *****/
/***************/

namespace dwtftr{
	typedef file_tree_ dFileTree;
	qW( FileTree );

	typedef drow__ sDRow;
}



#endif
