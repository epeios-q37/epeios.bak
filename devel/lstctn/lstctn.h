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

//D LiST ConTaiNer 

#ifndef LSTCTN__INC
# define LSTCTN__INC

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

# define qLMCONTAINERv( type, row ) E_LMCONTAINERt_( type, row )
# define qLMCONTAINERi( type, row ) E_LMCONTAINERt( type, row )

# define qLMCONTAINERvl( type ) qLMCONTAINERv( type, sdr::fRow )
# define qLMCONTAINERil( type ) qLMCONTAINERi( type, sdr::fRow )

/*************************/
/****** Old version ******/
/*************************/


namespace lstctn {
	using lst::list_;

	template <typename container, typename row, typename row_t> class list_container_
	: public list_<row, row_t>,
	  public container
	{
	protected:
		virtual void LSTAllocate(
			sdr::size__ Size,
			aem::mode__ Mode )
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
		void plug( qAS_ &AS )
		{
			list_<row, row_t>::plug( AS );
			container::plug( AS );
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
		const container &GetContainer( void ) const
		{
			return *this;
		}
		container &GetContainer( void )
		{
			return *this;
		}
		list_<row, row_t> &GetList( void )
		{
			return *this;
		}
		const list_<row, row_t> &GetList( void ) const
		{
			return *this;
		}
	};

	E_AUTO3( list_container );

	class fHook
	{
	protected:
		virtual ctn::fHook &LSTCTNGetContainerHook( void ) = 0;
		virtual lst::fHook &LSTCTNGetListHook( void ) = 0;
	public:
		qCALLBACK_DEF( Hook );
		ctn::fHook &GetContainerHook( void )
		{
			return LSTCTNGetContainerHook();
		}
		lst::fHook &GetListHook( void )
		{
			return LSTCTNGetListHook();
		}
	};

	template <typename host> inline bso::fBool Plug(
		host &Host,
		fHook &Hook )
	{
		bso::fBool Exists = ctn::Plug( Host..GetContainer(), Hook.GetContainerHook() );

		return lst::Plug( Host.GetList(), Hook.GetListHook() ) || Exists;
	}

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

	template <typename host> class rFH
	: public fHook
	{
	private:
		ctn::rFH<host> _Container;
		lst::rFH<host> _List;
		time_t _ContainerTimeStamp( void ) const
		{
			return _Container.TimeStamp();
		}
		time_t _ListTimeStamp( void ) const
		{
			return _List.TimeStamp();
		}
	protected:
		virtual ctn::fHook &LSTCTNGetContainerHook( void ) override
		{
			return _Container;
		}
		virtual lst::fHook &LSTCTNGetListHook( void ) override
		{
			return _List;
		}
	public:
		void reset( bso::bool__ P = true )
		{
			if ( P ) {
				_List.AdjustSize( _Container.ModificationTimestamp() );
			}

			_Container.reset( P );
			_List.reset( P );
			fHook::reset( P );
		}
		qCVDTOR( rFH );
		void Init(
			const rHF &Filenames,
			const host &Host,
			uys::mode__ Mode,
			uys::behavior__ Behavior,
			flsq::id__ ID )
		{
			reset();

			_Container.Init( Filenames.Container, Host, Mode, Behavior, ID );
			_List.Init( Filenames.List, Host, Mode, Behavior, ID, _Container.ModificationTimestamp() );
			fHook::Init();
		}
		/*
		uys::mode__ Mode( uys::mode__ Mode )
		{
			uys::mode__ ModeBuffer = _Container.Mode( Mode );

			if ( ModeBuffer != _List.Mode( Mode ) )
				qRFwk();

			return ModeBuffer;
		}
		uys::mode__ Mode( void ) const
		{
			uys::mode__ Mode = _Container.Mode();

			if ( Mode != _List.Mode() )
				qRFwk();

			return Mode;
		}
		uys::state__ State( void ) const
		{
			uys::state__ State = _Container.State();

			if ( !State.IsError() )
				if ( State != _List.State() )
					State = uys::sInconsistent;

			return State;
		}
		uys::state__ Settle( void )
		{
			uys::state__ State = _Container.Settle();

			if ( State.IsError() )
				return State;

			if ( _Container.IsPersistent() && _Container.Exists() )
				if ( _List.Settle( _Container.TimeStamp() ) != State )
						State = uys::sInconsistent;

			return State;
		}
		void Drop( void )
		{
			 _Container.Drop();
			_List.Drop();
		}
		bso::bool__ CreateFiles( err::handling__ ErrorHandling = err::h_Default )
		{
			bso::bool__ Success = _Container.CreateFiles( ErrorHandling );

			if ( !Success )
				return false;

			if ( Settle().IsError() ) {
				_Container.Drop();
				Success = false;
			}

			return Success;
		}
		void ReleaseFiles( void )
		{
			_Container.ReleaseFiles();
			_List.ReleaseFiles();
		}
		time_t TimeStamp( void ) const
		{
			time_t ContainerTimeStamp = _ContainerTimeStamp();
			time_t ListTimeStamp = _ListTimeStamp();

			if ( ContainerTimeStamp > ListTimeStamp )
				return ContainerTimeStamp;
			else
				return ListTimeStamp;
		}
		bso::bool__ IsPersistent( void ) const
		{
			bso::bool__ Is = _Container.IsPersistent();

			if ( Is != _List.IsPersistent() )
				qRFwk();

			return Is;
		}
		ctn::fh___ &ContainerFilesHook( void )
		{
			return _Container;
		}
		lst::fh___ &ListFilesHook( void )
		{
			return _List;
		}
		*/
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
