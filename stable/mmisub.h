/*
	Header for the 'mmisub' library by Claude SIMON (http://zeusw.org/intl/contact.html)
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

//	$Id: mmisub.h,v 1.5 2012/11/14 16:06:31 csimon Exp $

#ifndef MMISUB__INC
#define MMISUB__INC

#define MMISUB_NAME		"MMISUB"

#define	MMISUB_VERSION	"$Revision: 1.5 $"

#define MMISUB_OWNER		"Claude SIMON (http://zeusw.org/intl/contact.html)"

#include "ttr.h"

extern class ttr_tutor &MMISUBTutor;

#if defined( E_DEBUG ) && !defined( MMISUB_NODBG )
#define MMISUB_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.5 $
//C Claude SIMON (http://zeusw.org/intl/contact.html)
//R $Date: 2012/11/14 16:06:31 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D MultiMemory Index SUBpart 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#error Do not use. Use 'MMI' library instead.

#include "err.h"
#include "flw.h"
#include "mdr.h"
#include "epeios.h"

// Ce module est en ralit un sous-module de la bibliothque 'MMI', d'o le 'namespace' ci-dessous.
// Facilite l'imbrication avec la bibliothque 'BCH'.
// Toutes les mthodes signales comme 'dportes' sont dfini dans le module 'MMM'.

namespace mmi {
	//t The type of an index in the indexed multimemory.
	E_ROW( index__ );

	// Prdclaration.
	class indexed_multimemory_;

	class _base_indexed_multimemory_driver__
	: public mdr::E_MEMORY_DRIVER__
	{
	private:
		index__ _Index;
		const indexed_multimemory_ *&Multimemoire_;
	protected:
		// Dporte.
		virtual void MDRRecall(
			mdr::row_t__ Position,
			mdr::size__ Amount,
			mdr::datum__ *Buffer );
		virtual void MDRStore(
			const mdr::datum__ *Buffer,
			mdr::size__ Amount,
			mdr::row_t__ Position )
		{
			ERRu();
		}
		virtual void MDRAllocate( mdr::size__ Capacity )
		{
			ERRu();
		}
		virtual void MDRFlush( void )
		{
			ERRu();
		}
	public:
		_base_indexed_multimemory_driver__(
			const indexed_multimemory_ *&Multimemoire,
			mdr::size__ &Extent )
		: Multimemoire_( Multimemoire ),
		  E_MEMORY_DRIVER__( Extent )
		{}
		_base_indexed_multimemory_driver__(
			indexed_multimemory_ *&Multimemoire,
			mdr::size__ &Extent )
		: Multimemoire_( *(const indexed_multimemory_ **)& Multimemoire ),
		  E_MEMORY_DRIVER__( Extent )
		{}
		void reset( bool P = true )
		{
			E_MEMORY_DRIVER__::reset( P );

			_Index = NONE;
			Multimemoire_ = NULL;
		}
		//f Initialize with 'Multimemory' multimemory.
		void Init( void )
		{
			E_MEMORY_DRIVER__::Init();

			_Index = NONE;
		}
		//f The 'Index' memory becomes the memory handled by this memory driver.
		void Index( index__ Index )
		{
			_Index = Index;
		}
		//f Return the index of the current memory.
		index__ Index( void ) const
		{
			return _Index;
		}
		// Dporte.
		epeios::size__ Size( void ) const;
		friend class indexed_multimemory_driver__;
	};

	//c This class is the standard memory driver for the indexed multimemory.
	class indexed_multimemory_driver__
	: public _base_indexed_multimemory_driver__
	{
	private:
		indexed_multimemory_ *Multimemoire_;
		// memoire  laquelle il a t affect
	protected:
		// Dporte.
		virtual void MDRStore(
			const epeios::datum__ *Buffer,
			mdr::size__ Amount,
			mdr::row_t__ Position );
		// Dporte.
		virtual void MDRAllocate( mdr::size__ Capacity );
		// Dporte.
		virtual void MDRFlush( void );
	public:
		indexed_multimemory_driver__( mdr::size__ &Extent	)
		: _base_indexed_multimemory_driver__( Multimemoire_, Extent ) {}
		void reset( bool P = true )
		{
			_base_indexed_multimemory_driver__::reset( P );
		}
		//f Initialize with the 'Multimemory' multimemory.
		void Init( indexed_multimemory_ &Multimemory )
		{
			_base_indexed_multimemory_driver__::Init();
			Multimemoire_ = &Multimemory;
		}
		//f The 'Index' memory becomes the memory handled by this driver.
	};

	class standalone_indexed_multimemory_driver__
	: public indexed_multimemory_driver__
	{
	private:
		epeios::size__ _Extent;
	public:
		standalone_indexed_multimemory_driver__( void )
		: indexed_multimemory_driver__( _Extent )
		{
			_Extent = 0;
		}
	};

	//c Same as 'mmmi_indexed_multimemory_driver_', but for read-only memory.
	class const_indexed_multimemory_driver__
	: public _base_indexed_multimemory_driver__
	{
	private:
		const indexed_multimemory_ *Multimemoire_;
		// memoire  laquelle il a t affect
	public:
		const_indexed_multimemory_driver__( mdr::size__ &Extent	)
		: _base_indexed_multimemory_driver__( Multimemoire_, Extent ) {}
		void reset( bool P = true )
		{
			_base_indexed_multimemory_driver__::reset( P );
		}
		//f Initialize with the 'Multimemory' multimemory.
		void Init( const indexed_multimemory_ &Multimemory )
		{
			_base_indexed_multimemory_driver__::Init();
			Multimemoire_ = &Multimemory;
		}
		//f The 'Index' memory becomes the memory handled by this driver.
	};

	class standalone_const_indexed_multimemory_driver__
	: public const_indexed_multimemory_driver__
	{
	private:
		epeios::size__ _Extent;
	public:
		standalone_const_indexed_multimemory_driver__( void )
		: const_indexed_multimemory_driver__( _Extent )
		{
			_Extent = 0;
		}
	};

}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
