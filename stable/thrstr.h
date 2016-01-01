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

//	$Id: thrstr.h,v 1.4 2012/11/14 16:06:38 csimon Exp $

#ifndef THRSTR__INC
#define THRSTR__INC

#define THRSTR_NAME		"THRSTR"

#define	THRSTR_VERSION	"$Revision: 1.4 $"	

#define THRSTR_OWNER		"the Epeios project (http://zeusw.org/epeios/)"

#if defined( E_DEBUG ) && !defined( THRSTR_NODBG )
#define THRSTR_DBG 
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.4 $
//C Claude SIMON (http://zeusw.org/intl/contact.html)
//R $Date: 2012/11/14 16:06:38 $

/* End of automatic documentation generation part. */
/*$BEGIN$*/

/* Addendum to the automatic documentation generation part. */
//D PLEASE COMPLETE
/* End addendum to automatic documentation generation part. */

#error "Obsolete library. Integrated in the 'MTK' library."

#include "err.h"
#include "flw.h"
#include "mtx.h"
#include "mtk.h"

//d The value of an invalid handler.
#define THRSTR_INVALID	NULL

namespace thrstr {
	struct data {
			mtx::mutex_handler__ Data;
			mtx::mutex_handler__ Thread;
			mtx::mutex_handler__ Store;
			bso__bool Continue;
			void *UP;
			bso__ulong Amount;
			mtx::mutex_handler__ AmountMutex;
			mtk__routine Routine;
	};
	
	typedef data* thread_store_handler__;
	
	//f Create a thread_store_handler which deals with 'Routine'.
	thread_store_handler__ Create( mtk__routine Routine );

	//f Delete 'Handler'.	
	void Close( thread_store_handler__ Handler );

	
	//c A thread store.
	class thread_store___ {
	private:
		thread_store_handler__ Handler_;
		mtx::mutex___ Data_;
		mtx::mutex___ Thread_;
		mtx::mutex___ Store_;
	public:
		void reset( bso__bool P = true )
		{
			Data_.reset( P );
			Thread_.reset( P );
			Store_.reset( P );
			Handler_ = THRSTR_INVALID;
		}
		thread_store___( void )
		{
			reset( false );
		}
		~thread_store___( void )
		{
			reset( true );
		}
		//f Initialinf with thread store handler 'Handler'.	
		void Init( thread_store_handler__ Handler )
		{
			Handler_ = Handler;
			
			Data_.Init( Handler->Data );
			Thread_.Init( Handler->Thread );
			Store_.Init( Handler->Store );
		}
		//f Launch a routine with 'UP' as user pointer.
		void Launch( void *UP );
	};		
}

/*$END$*/
#endif
