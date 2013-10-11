/*
	Header for the 'mmm0' library by Claude SIMON (http://zeusw.org/intl/contact.html)
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

//	$Id: mmm0.h,v 1.8 2012/11/14 16:06:31 csimon Exp $

#ifndef MMM0__INC
#define MMM0__INC

#define MMM0_NAME		"MMM0"

#define	MMM0_VERSION	"$Revision: 1.8 $"

#define MMM0_OWNER		"Claude SIMON (http://zeusw.org/intl/contact.html)"

#include "ttr.h"

extern class ttr_tutor &MMM0Tutor;

#if defined( E_DEBUG ) && !defined( MMM0_NODBG )
#define MMM0_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.8 $
//C Claude SIMON (http://zeusw.org/intl/contact.html)
//R $Date: 2012/11/14 16:06:31 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D MultiMeMory base 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#error Do not use. Use 'MMM' instead.

// Ce module contient les définitions communes pour tous les 'mmmX' (X > 0), d'où son namesapce 'mmm'.

#include "err.h"
#include "flw.h"
#include "mdr.h"

#ifdef MMM_USE_V1
#	define MMM__USE_V1
#elif defined( MMM_USE_V2 )
#	define MMM__USE_V2
#else
#	define MMM__USE_V2
#endif

#ifdef MMM__USE_V1
#	define MMM_UNDEFINED_DESCRIPTOR	0
#elif defined( MMM__USE_V2 )
#	define MMM_UNDEFINED_DESCRIPTOR NONE
#else
#	error "No 'MMM' version défined !".
#endif

namespace mmm {
	class multimemory_;	// Prédéclaration.

	//t Type d'un descripteur de sous-méméoire dans le multimémoire.
	typedef mdr::row_t__	descriptor__;

	//c The standard memory driver for the multimemory.
	class multimemory_driver__
	: public mdr::E_MEMORY_DRIVER__
	{
	private:
		descriptor__ &_Descriptor;
		// memoire à laquelle il a été affecté
		class multimemory_ *Multimemoire_;
		bso::ubyte__ &_Addendum;
		void Liberer_();
	protected:
		// fonction déportée
		// lit à partir de 'Position' et place dans 'Tampon' 'Nombre' octets;
		virtual void MDRRecall(
			mdr::row_t__ Position,
			mdr::size__ Amount,
			mdr::datum__ *Buffer );
		// fonction déportée
		// écrit 'Nombre' octets à la position 'Position'
		virtual void MDRStore(
			const mdr::datum__ *Buffer,
			mdr::size__ Amount,
			mdr::row_t__ Position );
		// fonction déportée
		// alloue 'Capacite' octets
		virtual void MDRAllocate( mdr::size__ Size );
		virtual void MDRFlush( void );
	public:
		multimemory_driver__(
			descriptor__ &Descriptor,
			bso::ubyte__ &Addendum,
			mdr::size__ &Extent )
		: _Descriptor( Descriptor ),
		  _Addendum( Addendum ),
		  E_MEMORY_DRIVER__( Extent )
		{}
		void reset( bool P = true )
		{
			if ( P )
				Liberer_();
			else
				Multimemoire_ = NULL;

			_Descriptor = MMM_UNDEFINED_DESCRIPTOR;
			
			E_MEMORY_DRIVER__::reset( P );
		}
		//f Initialization with the 'Multimemory' multimemory.
		void Init( multimemory_ &Multimemory )
		{
			Liberer_();

			Multimemoire_ = &Multimemory;
			_Descriptor = MMM_UNDEFINED_DESCRIPTOR;

			E_MEMORY_DRIVER__::Init();
		}
		//f Return the current descriptor.
		descriptor__ Descriptor( void ) const
		{
			return _Descriptor;
		}
		//f Return the used multimemory.
		multimemory_ *Multimemory( void ) const
		{
			return Multimemoire_;
		}
	};

	class standalone_multimemory_driver__
	: public multimemory_driver__
	{
	private:
		mdr::size__ _Extent;
		descriptor__ _Descriptor;
		bso::ubyte__ _Addendum;
	public:
		standalone_multimemory_driver__( void )
		: multimemory_driver__( _Descriptor, _Addendum, _Extent )
		{}
	};

	#define E_MULTIMEMORY_DRIVER__	standalone_multimemory_driver__

}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
