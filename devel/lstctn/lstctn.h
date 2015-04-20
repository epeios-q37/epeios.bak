/*
	Header for the 'lstctn' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Copyright (C) 2004 Claude SIMON (http://zeusw.org/intl/contact.html).

	This file is part of the Epeios (http://zeusw.org/epeios/) project.

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 3
	of the License, or (at your option) any later version.
 
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, go to http://www.fsf.org/
	or write to the:
  
         	         Free Software Foundation, Inc.,
           59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

//	$Id: lstctn.h,v 1.60 2013/04/11 19:36:49 csimon Exp $

#ifndef LSTCTN__INC
#define LSTCTN__INC

#define LSTCTN_NAME		"LSTCTN"

#define	LSTCTN_VERSION	"$Revision: 1.60 $"

#define LSTCTN_OWNER		"Claude SIMON (http://zeusw.org/intl/contact.html)"

#include "ttr.h"

extern class ttr_tutor &LSTCTNTutor;

#if defined( E_DEBUG ) && !defined( LSTCTN_NODBG )
#define LSTCTN_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.60 $
//C Claude SIMON (http://zeusw.org/intl/contact.html)
//R $Date: 2013/04/11 19:36:49 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D LiST ConTaiNer 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"
#include "lst.h"
#include "ctn.h"

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
		void plug( ags::E_ASTORAGE_ &AS )
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
			ERRFbd();
		}
		// To avoid the use of herited 'Delete' methods.
		void Delete( void ) const
		{
			ERRFbd();
		}
		// Pour viter certains 'castage'.
		const container &Container( void ) const
		{
			return *this;
		}
		// Pour viter certains 'castage'.
		container &Container( void )
		{
			return *this;
		}
	};

	E_AUTO3( list_container );

	struct hook_filenames___
	{
	public:
		ctn::hook_filenames___ Container;
		lst::hook_filenames___ List;
		void reset( bso::bool__ P = true )
		{
			Container.reset( P );
			List.reset( P );
		}
		E_CDTOR( hook_filenames___ );
		void Init(
			const fnm::name___ &Path,
			const fnm::name___ &Basename );
	};

	class files_hook___
	{
	private:
		ctn::files_hook___ _Container;
		lst::files_hook___ _List;
		time_t _ContainerTimeStamp( void ) const
		{
			return _Container.TimeStamp();
		}
		time_t _ListTimeStamp( void ) const
		{
			return _List.TimeStamp();
		}
	public:
		void reset( bso::bool__ P = true )
		{
			_Container.ReleaseFiles();	// Sinon le 'Settle()'  ci'dessous ne fonctionne pas correctement.

			if ( P ) {
				Settle();	// Lanc explicitement, car le 'reset(...)' de '_List' ne peut lancer son propre 'Settle(...)'.
			}

			_Container.reset( P );
			_List.reset( P );
		}
		files_hook___( void )
		{
			reset( false );
		}
		~files_hook___( void )
		{
			reset();
		}
		void Init(
			const hook_filenames___ &Filenames,
			uys::mode__ Mode,
			uys::behavior__ Behavior,
			flsq::id__ ID )
		{
			reset();

			_Container.Init( Filenames.Container, Mode, Behavior, ID );
			_List.Init( Filenames.List, Mode, Behavior );
		}
		uys::mode__ Mode( uys::mode__ Mode )
		{
			uys::mode__ ModeBuffer = _Container.Mode( Mode );

			if ( ModeBuffer != _List.Mode( Mode ) )
				ERRFwk();

			return ModeBuffer;
		}
		uys::mode__ Mode( void ) const
		{
			uys::mode__ Mode = _Container.Mode();

			if ( Mode != _List.Mode() )
				ERRFwk();

			return Mode;
		}
		uys::state__ State( void ) const
		{
			uys::state__ State = _Container.State();

			if ( !uys::IsError( State ) )
				if ( State != _List.State() )
					State = uys::sInconsistent;

			return State;
		}
		uys::state__ Bind( void )
		{
			uys::state__ State = _Container.Bind();

			if ( uys::IsError( State ) )
				return State;

			if ( _List.Bind( _Container.TimeStamp() ) != State )
				State = uys::sInconsistent;

			return State;
		}
		uys::state__ Settle( void )
		{
			uys::state__ State = _Container.Settle();

			if ( uys::IsError( State ) )
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

			if ( uys::IsError( Settle() ) ) {
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
				ERRFwk();

			return Is;
		}
		ctn::files_hook___ &ContainerFilesHook( void )
		{
			return _Container;
		}
		lst::files_hook___ &ListFilesHook( void )
		{
			return _List;
		}
	};

	template <typename list_container> inline uys::state__ Plug(
		list_container &ListContainer,
		files_hook___ &Hook )
	{
		uys::state__ State = ctn::Plug( ListContainer.Container(), Hook.ContainerFilesHook() );

		if ( uys::IsError( State ) ) {
			Hook.reset();
		} else {
			fil::size__ Size = Hook.ContainerFilesHook().StaticsFilesHook().FileSize() / ListContainer.GetStaticsItemSize();

			if ( Size > SDR_SIZE_MAX )
				ERRDta();

			if ( lst::Plug( ListContainer, Hook.ListFilesHook(), ( sdr::size__ )Size, Hook.ContainerFilesHook().TimeStamp() ) != State ) {
				Hook.reset();
				State = uys::sInconsistent;
			}
		}

		return State;
	}


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
			ERRFbd();
		}
	};

	E_AUTO4( list_xcontainer )

	#define E_LMCONTAINERtx_( type, row, row_t )	list_container_< ctn::E_MCONTAINERt_( type, row ), row, row_t >
	#define E_LMCONTAINERtx( type, row, row_t )		list_container< ctn::E_MCONTAINERt_( type, row ), row, row_t >

	#define E_LMCONTAINERt_( type, row )	E_LMCONTAINERtx_( type, row, sdr::row_t__ )
	#define E_LMCONTAINERt( type, row )		E_LMCONTAINERtx( type, row, sdr::row_t__ )

	#define E_LMCONTAINER_( type )			E_LMCONTAINERt_( type, sdr::row__ )
	#define E_LMCONTAINER( type )			E_LMCONTAINERt( type, sdr::row__ )

	#define E_LCONTAINERtx_( type, row, row_t )		list_container_< ctn::E_CONTAINERt_( type, row ), row, row_t >
	#define E_LCONTAINERtx( type, row, row_t )		list_container< ctn::E_CONTAINERt_( type, row ), row, row_t >

	#define E_LCONTAINERt_( type, row )		E_LCONTAINERtx_( type, row, sdr::row_t__ )
	#define E_LCONTAINERt( type, row )		E_LCONTAINERtx( type, row, sdr::row_t__ )

	#define E_LCONTAINER_( type )			E_LCONTAINERt_( type, sdr::row__ )
	#define E_LCONTAINER( type )			E_LCONTAINERt( type, sdr::row__ )

	#define E_LXMCONTAINERtx_( type, row, row_t )	list_xcontainer_< ctn::E_MCONTAINERt_( type, row ), type, row, row_t >
	#define E_LXMCONTAINERtx( type, row, row_t )	list_xcontainer< ctn::E_MCONTAINERt_( type, row ), type, row, row_t >

	#define E_LXMCONTAINERt_( type, row )	E_LXMCONTAINERtx_( type, row, sdr::row_t__ )
	#define E_LXMCONTAINERt( type, row )	E_LXMCONTAINERtx( type, row, sdr::row_t__ )

	#define E_LXMCONTAINER_( type )			E_LXMCONTAINERt_( type, sdr::row__ )
	#define E_LXMCONTAINER( type )			E_LXMCONTAINERt( type, sdr::row__ )

	#define E_LXCONTAINERtx_( type, row, row_t )	list_xcontainer_< ctn::E_CONTAINERt_( type, row ), type, row, row_t >
	#define E_LXCONTAINERtx( type, row, row_t )		list_xcontainer< ctn::E_CONTAINERt_( type, row ), type, row, row_t >

	#define E_LXCONTAINERt_( type, row )	E_LXCONTAINERtx_( type, row, sdr::row_t__ )
	#define E_LXCONTAINERt( type, row )		E_LXCONTAINERtx( type, row, sdr::row_t__ )

	#define E_LXCONTAINER_( type )			E_LXCONTAINERt_( type, sdr::row__ )
	#define E_LXCONTAINER( type )			E_LXCONTAINERt( type, sdr::row__ )

}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
