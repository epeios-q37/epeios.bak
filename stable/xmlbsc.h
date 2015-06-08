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

//	$Id: xmlbsc.h,v 1.6 2013/07/25 15:59:15 csimon Exp $

#ifndef XMLBSC__INC
#define XMLBSC__INC

#define XMLBSC_NAME		"XMLBSC"

#define	XMLBSC_VERSION	"$Revision: 1.6 $"

#define XMLBSC_OWNER		"Claude SIMON"

#if defined( E_DEBUG ) && !defined( XMLBSC_NODBG )
#define XMLBSC_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.6 $
//C Claude SIMON (csimon at zeusw dot org)
//R $Date: 2013/07/25 15:59:15 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D XML BaSiC 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

/* Addendum to the automatic documentation generation part. */
//D Hierarchically Organized Structures BaSiCs
/* End addendum to automatic documentation generation part. */

#include "err.h"
#include "flw.h"
#include "dtr.h"
#include "lst.h"
#include "ctn.h"
#include "fls.h"
#include "str.h"
#include "xml.h"

namespace xmlbsc {

	using dtr::dynamic_tree_;
	using lst::list_;
	using bch::bunch_;
	using ctn::mono_container_;

	/*c Contains the file memory driver tu use together with the 'hdbbsc' class.
	Designed for using with the merged memory ('MMG' library). */
	class file_memory_drivers {
	public:
		struct tree_drivers {
			fls::file_storage_driver___ Tree, Queue;
		} DTree;
		fls::file_storage_driver___ List;
		fls::file_storage_driver___ Bunch;
		struct container_drivers {
			fls::file_storage_driver___ Statics;
			struct descriptors_drivers {
				fls::file_storage_driver___ Descriptors;
				fls::file_storage_driver___ Multimemory;
			} Dynamics;
		} Container;
		//f Initialize with generic name 'Name', directory 'Directory' and  suffix 'Suffix'.
		void Init(
			const char *Name,
			fls::id__ ID,
			const char *Directory = NULL,
			const char *Suffix = "hdb" );
		//f Make persistent.
		void MakePersistent( void )
		{
			DTree.Tree.Persistent();
			DTree.Queue.Persistent();
			List.Persistent();
			Bunch.Persistent();
			Container.Statics.Persistent();
			Container.Dynamics.Descriptors.Persistent();
			Container.Dynamics.Multimemory.Persistent();
		}
		//f Manual mode.
		void Manual( void )
		{
			DTree.Tree.Manual();
			DTree.Queue.Manual();
			List.Manual();
			Bunch.Manual();
			Container.Statics.Manual();
			Container.Dynamics.Descriptors.Manual();
			Container.Dynamics.Multimemory.Manual();
		}
	};

	typedef str::string_	item_;
	typedef str::string		item;

	//c The basic manager of item type 'i' and referred by 'referrence__'
	template <typename o, typename r> class basic_
	: public E_LISTt_( r ),
	  public E_DTREEt_( r ),
	  public E_BUNCHt_( o, r ),
	  public E_MCONTAINERt_( item_, r )
	{
	private:
		r Create_(
			const item_ &Item,
			o Oddity )
		{
			r P = E_LISTt_( r )::New();

			E_BUNCHt_( o, r )::Store( Oddity, P );
			E_MCONTAINERt_( item_, r )::Store( Item, P );

			return P;
		}
	protected:
		void LSTAllocate(
			sdr::size__ Size,
			aem::mode__ Mode )
		{
			E_DTREEt_( r )::Allocate( Size, Mode );

			E_BUNCHt_( o, r )::Allocate( Size, Mode );
			E_MCONTAINERt_( item_, r )::Allocate( Size, Mode );
		}
	public:
		struct s
		: public E_LISTt_( r )::s,
		  public E_DTREEt_( r )::s,
		  public E_BUNCHt_( o, r )::s,
		  public E_MCONTAINERt_( item_, r )::s
		{};
		basic_( s &S )
		: E_LISTt_( r )( S ),
		  E_DTREEt_( r )( S ),
		  E_BUNCHt_( o, r )( S ),
		  E_MCONTAINERt_( item_, r )( S )
		{}
		void reset( bool P = true )
		{
			E_LISTt_( r )::reset( P );
			E_DTREEt_( r )::reset( P );
			E_BUNCHt_( o, r )::reset( P );
			E_MCONTAINERt_( item_, r )::reset( P );
		}
		void plug( qAS_ &AS )
		{
			E_LISTt_( r )::plug( AS );
			E_DTREEt_( r )::plug( AS );
			E_BUNCHt_( o, r )::plug( AS );
			E_MCONTAINERt_( item_, r )::plug( AS );
		}
		basic_ &operator =( const basic_ &BM )
		{
			E_LISTt_( r )::operator =( BM );
			E_DTREEt_( r )::operator =( BM );
			E_BUNCHt_( o, r )::operator =( BM );
			E_MCONTAINERt_( item_, r )::operator =( BM );

			return *this;
		}
		//f Plug the file meomry drivers 'Drivers'. Call it BEFORE 'Init()'.
		void Plug( file_memory_drivers &Drivers )
		{
			E_DTREEt_( r )::Tree.plug( Drivers.DTree.Tree );
			E_DTREEt_( r )::Queue.plug( Drivers.DTree.Queue );
			E_LISTt_( r )::plug( Drivers.List );
			E_BUNCHt_( o, r )::plug( Drivers.Bunch );
			E_MCONTAINERt_( item_, r )::Statics.plug( Drivers.Container.Statics );
			E_MCONTAINERt_( item_, r )::Dynamics.Descripteurs.plug( Drivers.Container.Dynamics.Descriptors );
			E_MCONTAINERt_( item_, r )::Dynamics.Multimemoire.plug( Drivers.Container.Dynamics.Multimemory );
		}
		void Init( void )
		{
			E_LISTt_( r )::Init();
			E_DTREEt_( r )::Init();
			E_BUNCHt_( o, r )::Init();
			E_MCONTAINERt_( item_, r )::Init();
		}
		void BecomeFirstChild(
			r First,
			r Node )
		{
			E_DTREEt_( r )::BecomeFirstChild( First, Node );
		}
		void BecomeLastChild(
			r Last,
			r Node )
		{
			E_DTREEt_( r )::BecomeLastChild( Last, Node );
		}
		void BecomePreviousSibling(
			r Previous,
			r Node )
		{
			E_DTREEt_( r )::BecomePreviousSibling( Previous, Node );
		}
		void BecomeNextSibling(
			r Next,
			r Node )
		{
			E_DTREEt_( r )::BecomeNext( Next, Node );
		}
		//f Return the position where new item 'Item' is put.
		r Create(
			const item_ &Item,
			o Oddity )
		{
			return Create_( Item, Oddity );
		}
		//f Return the psotion of a new empty item which would be initialized later.
		r Create( void )
		{
			return E_LISTt_( r )::New();
		}
		/*f The new item 'Item' becomes first child of node 'Node'.
		Return position where 'Item' is put. */
		r BecomeFirstChild(
			const item_ &Item,
			o Oddity,
			r Node )
		{
			r P = Create( Item, Oddity );

			E_DTREEt_( r )::BecomeFirstChild( P, Node );

			return P;
		}
		/*f The new name 'Name' becomes last child of node 'Node'.
		Return position where 'Name' is put. */
		r BecomeLastChild(
			const item_ &Item,
			o Oddity,
			r Node )
		{
			r P = Create( Item, Oddity );

			E_DTREEt_( r )::BecomeLastChild( P, Node );

			return P;
		}
		/*f The new name 'Name' becomes node next to node 'Node'.
		Return position where 'Name' is put. */
		r BecomeNextSibling(
			const item_ &Item,
			o Oddity,
			r Node )
		{
			r P = Create( Item, Oddity );

			E_DTREEt_( r )::BecomeNextSibling( P, Node );

			return P;
		}
		r BecomePreviousSibling(
			const item_ &Item,
			o Oddity,
			r Node )
		{
			r P = Create( Item, Oddity );

			E_DTREEt_( r )::BecomePreviousSibling( P, Node );

			return P;
		}
		//f Returns true if 'Node' exists, or false if no.
		bso::bool__ Exists( r Node ) const
		{
			return E_DTREEt_( r )::Parent( Node ) != qNIL;
		}
		r First( r Node ) const
		{
			return E_DTREEt_( r )::First( Node );
		}
		r Next( r Node ) const
		{
			return E_DTREEt_( r )::Next( Node );
		}
		r Previous( r Node ) const
		{
			return E_DTREEt_( r )::Previous( Node );
		}
		r Last( r Node ) const
		{
			return E_DTREEt_( r )::Last( Node );
		}
		sdr::size__ Amount( void ) const
		{
			return E_DTREEt_( r )::Amount();
		}
		o GetOddity( r Row ) const 
		{
			return E_BUNCHt_( o, r )::Get( Row );
		}
		void Store(
			const item_ &Item,
			r Row )
		{
			E_MCONTAINERt_( item_, r )::Store( Item, Row );
			E_MCONTAINERt_( item_, r )::Flush();
		}
	};

	E_AUTO2( basic )

	
	using xml::TransformUsingEntities;
	using xml::WriteXMLHeader;
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
