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

// STacK ConTaiNer

#ifndef STKCTN__INC
# define STKCTN__INC

# define STKCTN_NAME		"STKCTN"

# if defined( E_DEBUG ) && !defined( STKCTN_NODBG )
#  define STKCTN_DBG
# endif

# include "stkbse.h"
# include "ctn.h"

namespace stkctn {
	using stkbse::vStack;

	class fCore
	{
	protected:
		virtual ctn::fCore &STKGetContainer( void ) = 0;
	public:
		qCALLBACK_DEF( Core );
		ctn::fCore &GetContainer( void )
		{
			return STKGetContainer();
		}
	};

	template <typename structure, typename item, typename row> class vContainerStack
	: public fCore,
	  public vStack< structure, item, row >
	{
	protected:
		virtual ctn::fCore &STKGetContainer( void ) override
		{
			return *this;
		}
	public:
		struct s
		: public vStack< structure, item, row >::s
		{};
		vContainerStack( s &S )
		: vStack< structure, item, row >( S )
		{}
		void reset( bso::fBool P = true )
		{
			fCore::reset( P );
			vStack< structure, item, row >::reset( P );
		}
		void Init( void )
		{
			fCore::Init();
			vStack< structure, item, row >::Init();
		}
	};

	using ctn::fHook;

	inline bso::fBool Plug(
		fCore &Core,
		fHook &Hook )
	{
		return ctn::Plug( Core.GetContainer(), Hook );
	}

	using ctn::rHF;
	typedef ctn::rFH rFH_;

	class rFH
	: public rFH_
	{
	public:
		uys::eState Init(
			const rHF &Filenames,
			fCore &Core,
			uys::mode__ Mode,
			uys::behavior__ Behavior,
			flsq::id__ ID )
		{
			return rFH_::Init(Filenames, Core.GetContainer(), Mode, Behavior, ID );
		}
	};
	
	qW3( ContainerStack );

	qROW( Row );

# define qCSTACKv( item, row )	vContainerStack< ctn::E_CONTAINERt_( item, row ), item, row >
# define qCSTACKi( item, row )	iContainerStack< ctn::E_CONTAINERt_( item, row ), item, row >

# define qCSTACKvl( item )		qCSTACKv( item, stkctn::fRow )
# define qCSTACKil( item )		qCSTACKi( item, stkctn::fRow )

# define qMCSTACKv( item, row )	vContainerStack< ctn::E_MCONTAINERt_( item, row ), item, row >
# define qMCSTACKi( item, row )	iContainerStack< ctn::E_MCONTAINERt_( item, row ), item, row >

# define qMCSTACKvl( item )		qMCSTACKv( item, stkctn::fRow )
# define qMCSTACKil( item )		qMCSTACKi( item, stkctn::fRow )


}

#endif
