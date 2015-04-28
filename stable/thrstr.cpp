/*
  'thrstr' library by Claude SIMON (http://zeusw.org/intl/contact.html)
  Requires the 'thrstr' header file ('thrstr.h').
  Copyright (C) 2000 Claude SIMON (http://zeusw.org/intl/contact.html).

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
  along with this program; if not, go to http://www.fsf.org or write to the:
  
                        Free Software Foundation, Inc.,
           59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#define THRSTR__COMPILATION

#include "thrstr.h"

using namespace thrstr;

thread_store_handler__ thrstr::Create( mtk__routine Routine )
{
	thread_store_handler__ Handler = THRSTR_INVALID;
ERRProlog
	mtx::mutex___ Mutex;
ERRBegin
	if ( ( Handler = new data ) == NULL )
		ERRa();
		
	Handler->Thread = Handler->Data = Handler->Store = MTX_INVALID;

	Handler->Continue = true;
	Handler->UP = NULL;
	Handler->Amount = 0;
	Handler->Routine = Routine;
	Handler->Data = mtx::Create();
	Handler->Thread = mtx::Create();
	Handler->Store = mtx::Create();
	
	Mutex.Init( Handler->Thread );
	Mutex.Lock();

	Mutex.Init( Handler->Store );
	Mutex.Lock();

ERRErr
	if ( Handler != THRSTR_INVALID ) {
		if ( Handler->Data != MTX_INVALID )
			mtx::Delete( Handler->Data );

		if ( Handler->Thread != MTX_INVALID )
			mtx::Delete( Handler->Thread );

		if ( Handler->Store != MTX_INVALID )
			mtx::Delete( Handler->Store );
			
		delete Handler;
	}

ERREnd
ERREpilog
	return Handler;
}

void thrstr::Close( thread_store_handler__ Handler )
{
ERRProlog
	mtx::mutex___ Data, Thread, Store;
ERRBegin
	Handler->Continue = false;
	
	Data.Init( Handler->Data );
	Thread.Init( Handler->Thread );
	Store.Init( Handler->Store );
	
	Data.Lock();
	
	while ( Handler->Amount != 0 ){
	
		Data.Unlock();
		
		Thread.Unlock();
		
		Store.Lock();
		
		Data.Lock();		
	}
	
	Data.reset();
		
	mtx::Delete( Handler->Data );
	mtx::Delete( Handler->Thread );
	mtx::Delete( Handler->Store );
	
	delete Handler;
ERRErr
ERREnd
ERREpilog
}

namespace {
	void Launcher_( void *UP )
	{
	ERRProlog
		thread_store_handler__ &Handler = *(thread_store_handler__ *)&UP;
		mtx::mutex___ Data, Thread, Store;
		void *RUP;
	ERRBegin
		Data.Init( Handler->Data );
		Thread.Init( Handler->Thread );
		Store.Init( Handler->Store );
		
		Data.Lock();
	
		do {
			RUP = Handler->UP;
			Data.Unlock();
			
			Store.Unlock();
			
			Handler->Routine( RUP );
			
			Data.Lock();
			
			if ( Handler->Amount == BSO_USHORT_MAX )
				ERRl();
				
			Handler->Amount++;
			
			Data.Unlock();
			
			Thread.Lock();
			
			Data.Lock();
			
			Handler->Amount--;
			
		} while ( Handler->Continue );
		
		Data.Unlock();
		
		Store.Unlock();
		
	ERRErr
	ERREnd
	ERREpilog
	}
}		
		
	
void thrstr::thread_store___::Launch( void *UP )
{
	Data_.Lock();
	
	Handler_->UP = UP;
	
	if ( Handler_->Amount == 0 )
		MTKLaunch( Launcher_, Handler_ ); 
	else
		Thread_.Unlock();
		
	Data_.Unlock();
	
	Store_.Lock();
		
}
		
