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

// LiST ConTaiNer

#ifndef LSTCTN_INC_
# define LSTCTN_INC_

# define LSTCTN_NAME		"LSTCTN"

# define	LSTCTN_VERSION	"$Revision: 1.60 $"

# define LSTCTN_OWNER		"Claude SIMON (http://zeusw.org/intl/contact.html)"

# if defined( E_DEBUG ) && !defined( LSTCTN_NODBG )
#  define LSTCTN_DBG
# endif

# include "err.h"
# include "flw.h"
# include "lst.h"
# include "ctn.h"

/*************************/
/****** New version ******/
/*************************/

# define qLMCONTAINERd( type, row ) E_LMCONTAINERt_( type, row )
# define qLMCONTAINERw( type, row ) E_LMCONTAINERt( type, row )

# define qLMCONTAINERdl( type ) qLMCONTAINERd( type, sdr::sRow )
# define qLMCONTAINERwl( type ) qLMCONTAINERw( type, sdr::sRow )

# define qLCONTAINERd( type, row ) E_LCONTAINERt_( type, row )
# define qLCONTAINERw( type, row ) E_LCONTAINERt( type, row )

# define qLCONTAINERdl( type ) qLCONTAINERd( type, sdr::sRow )
# define qLCONTAINERwl( type ) qLCONTAINERw( type, sdr::sRow )

# define qLXMCONTAINERd( type, row ) E_LXMCONTAINERt_( type, row )
# define qLXMCONTAINERw( type, row ) E_LXMCONTAINERt( type, row )

# define qLXMCONTAINERdl( type ) qLXMCONTAINERd( type, sdr::sRow )
# define qLXMCONTAINERwl( type ) qLXMCONTAINERw( type, sdr::sRow )

# define qLXCONTAINERd( type, row ) E_LXCONTAINERt_( type, row )
# define qLXCONTAINERw( type, row ) E_LXCONTAINERt( type, row )

# define qLXCONTAINERdl( type ) qLXCONTAINERd( type, sdr::sRow )
# define qLXCONTAINERwl( type ) qLXCONTAINERw( type, sdr::sRow )

/*************************/
/****** Old version ******/
/*************************/


namespace lstctn {
	using lst::list_;

	qHOOKS2( ctn::sHooks, Container, lst::sHook, List );

	template <typename container, typename row, typename row_t> class list_container_
	: public list_<row, row_t>,
	  public container
	{
		virtual void LSTAllocate(
			sdr::size__ Size,
			aem::mode__ Mode ) override
		{
			container::Allocate( Size, Mode );
		}
	public:
		struct s
		: public list_<row, row_t>::s,
		  public container::s
		{};
		list_container_( s &S )
		: list_<row, row_t>( S ),
		  container( S )
		{}
		void reset( bso::bool__ P = true )
		{
			list_<row, row_t>::reset( P );
			container::reset( P );
		}
		void plug( sHooks &Hooks )
		{
			container::plug( Hooks.Container_ );
			list_<row, row_t>::plug( Hooks.List_, container::Dynamics.Amount() );
		}
		void plug( qASd *AS )
		{
			container::plug( AS );
			list_<row, row_t>::plug( AS );
		}
		list_container_ &operator =( const list_container_ &LC )
		{
			list_<row, row_t>::operator =( LC );
			container::operator =( LC );

			return *this;
		}
		E_NAVt( list_<E_COVER2(row,row_t)>::, row )
		E_NAVXt( list_<E_COVER2(row,row_t)>::, row )
		//f Initialization.
		void Init( void )
		{
			list_<row, row_t>::Init();
			container::Init();
		}
		//f RemoveDelete object at 'Row'.
		void Remove( row Row )
		{
			container::operator()( Row ).reset();
			container::Flush();
			list_<row, row_t>::Delete( Row );
		}
		//f Create new entry.
		row New( void )
		{
			return list_<row, row_t>::New();
		}
		//f Create new entry with row 'Row'.
		row New( row Row )
		{
			return list_<row, row_t>::New( Row );
		}
		// To avoid the use of herited 'Append' methods.
		void Append( void ) const
		{
			qRFbd();
		}
		// To avoid the use of herited 'Add' methods.	// Must use 'New' and 'Store'.
		row Add( void )
		{
			qRFbd();
		}
		// To avoid the use of herited 'Delete' methods.
		void Delete( void ) const
		{
			qRFbd();
		}
	};

	E_AUTO3( list_container );

	template <typename container, typename list> struct rH_
	: public sHooks
	{
	protected:
		container Container_;
		list List_;
	public:
		rH_( void )
		: sHooks( Container_, List_ )
		{}
	};

	class rRH
	: public rH_<ctn::rRH, lst::rRH>
	{
	public:
		void reset( bso::sBool P = true )
		{
			Container_.reset( P );
			List_.reset( P );
		}
		qCDTOR( rRH );
		void Init( void )
		{
			Container_.Init();
			List_.Init();
		}
	};

	struct rHF
	{
	public:
		ctn::rHF Container;
		lst::rHF List;
		void reset( bso::bool__ P = true )
		{
			Container.reset( P );
			List.reset( P );
		}
		E_CDTOR( rHF );
		void Init(
			const fnm::name___ &Path,
			const fnm::name___ &Basename );
	};

	class rFH
	: public rH_<ctn::rFH, lst::rFH>
	{
	public:
		void reset( bso::sBool P = true )
		{
			Container_.reset( P );
			List_.reset( P );
		}
		qCDTOR( rFH );
		uys::eState Init(
			const rHF &Filenames,
			uys::mode__ Mode,
			uys::eBehavior Behavior,
			flsq::rId Id)
		{
			uys::eState State = Container_.Init(Filenames.Container, Mode, Behavior, Id);

			if ( !State.IsError() ) {
				if ( List_.Init(Filenames.List, Mode, Behavior, Id) != State )
					State = uys::sInconsistent;
			}

			return State;
		}
	};

	template <typename container, typename object, typename row, typename row_t> class list_xcontainer_
	: public list_container_<container, row, row_t>
	{
	private:
		row NewWithoutFlush_( void )
		{
			row Row = list_container_<container, row, row_t>::New();

			list_container_<container, row, row_t>::operator()( Row ).Init();

			return Row;
		}
		row NewWithoutFlush_( row Row )
		{
			Row = list_container_<container, row, row_t>::New( Row );

			list_container_<container, row, row_t>::operator()( Row ).Init();

			return Row;
		}
	public:
		struct s
		: public list_container_<container, row, row_t>::s
		{};
		list_xcontainer_( s &S )
		: list_container_<container, row, row_t>( S )
		{}
		row New( void )
		{
			row Row = NewWithoutFlush_();

			list_container_<container, row, row_t>::Flush();

			return Row;
		}
		row New( row Row )
		{
			Row = NewWithoutFlush_( Row );

			list_container_<container, row, row_t>::Flush();

			return Row;
		}
		row Add( const object &Object )
		{
			row Row = NewWithoutFlush_();

			list_container_<container, row, row_t>::operator()( Row ).operator =( Object );
			list_container_<container, row, row_t>::Flush();

			return Row;
		}
		// To avoid the use of herited 'Append' methods.
		void Append( void )
		{
			qRFbd();
		}
	};

	E_AUTO4( list_xcontainer )
}

# define E_LMCONTAINERtx_( type, row, row_t )	list_container_< ctn::E_MCONTAINERt_( type, row ), row, row_t >
# define E_LMCONTAINERtx( type, row, row_t )		list_container< ctn::E_MCONTAINERt_( type, row ), row, row_t >

# define E_LMCONTAINERt_( type, row )	E_LMCONTAINERtx_( type, row, sdr::row_t__ )
# define E_LMCONTAINERt( type, row )		E_LMCONTAINERtx( type, row, sdr::row_t__ )

# define E_LMCONTAINER_( type )			E_LMCONTAINERt_( type, sdr::row__ )
# define E_LMCONTAINER( type )			E_LMCONTAINERt( type, sdr::row__ )

# define E_LCONTAINERtx_( type, row, row_t )		list_container_< ctn::E_CONTAINERt_( type, row ), row, row_t >
# define E_LCONTAINERtx( type, row, row_t )		list_container< ctn::E_CONTAINERt_( type, row ), row, row_t >

# define E_LCONTAINERt_( type, row )		E_LCONTAINERtx_( type, row, sdr::row_t__ )
# define E_LCONTAINERt( type, row )		E_LCONTAINERtx( type, row, sdr::row_t__ )

# define E_LCONTAINER_( type )			E_LCONTAINERt_( type, sdr::row__ )
# define E_LCONTAINER( type )			E_LCONTAINERt( type, sdr::row__ )

# define E_LXMCONTAINERtx_( type, row, row_t )	list_xcontainer_< ctn::E_MCONTAINERt_( type, row ), type, row, row_t >
# define E_LXMCONTAINERtx( type, row, row_t )	list_xcontainer< ctn::E_MCONTAINERt_( type, row ), type, row, row_t >

# define E_LXMCONTAINERt_( type, row )	E_LXMCONTAINERtx_( type, row, sdr::row_t__ )
# define E_LXMCONTAINERt( type, row )	E_LXMCONTAINERtx( type, row, sdr::row_t__ )

# define E_LXMCONTAINER_( type )			E_LXMCONTAINERt_( type, sdr::row__ )
# define E_LXMCONTAINER( type )			E_LXMCONTAINERt( type, sdr::row__ )

# define E_LXCONTAINERtx_( type, row, row_t )	list_xcontainer_< ctn::E_CONTAINERt_( type, row ), type, row, row_t >
# define E_LXCONTAINERtx( type, row, row_t )		list_xcontainer< ctn::E_CONTAINERt_( type, row ), type, row, row_t >

# define E_LXCONTAINERt_( type, row )	E_LXCONTAINERtx_( type, row, sdr::row_t__ )
# define E_LXCONTAINERt( type, row )		E_LXCONTAINERtx( type, row, sdr::row_t__ )

# define E_LXCONTAINER_( type )			E_LXCONTAINERt_( type, sdr::row__ )
# define E_LXCONTAINER( type )			E_LXCONTAINERt( type, sdr::row__ )

/*$END$*/
#endif
