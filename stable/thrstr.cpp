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

#define THRSTR_COMPILATION_

#include "thrstr.h"

using namespace thrstr;

thread_store_handler__ thrstr::Create( mtk__routine Routine )
{
	thread_store_handler__ Handler = THRSTR_INVALID;
qRH
	mtx::mutex___ Mutex;
qRB
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

qRR
	if ( Handler != THRSTR_INVALID ) {
		if ( Handler->Data != MTX_INVALID )
			mtx::Delete( Handler->Data );

		if ( Handler->Thread != MTX_INVALID )
			mtx::Delete( Handler->Thread );

		if ( Handler->Store != MTX_INVALID )
			mtx::Delete( Handler->Store );
			
		delete Handler;
	}

qRT
qRE
	return Handler;
}

void thrstr::Close( thread_store_handler__ Handler )
{
qRH
	mtx::mutex___ Data, Thread, Store;
qRB
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
qRR
qRT
qRE
}

namespace {
	void Launcher_( void *UP )
	{
	qRH
		thread_store_handler__ &Handler = *(thread_store_handler__ *)&UP;
		mtx::mutex___ Data, Thread, Store;
		void *RUP;
	qRB
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
		
	qRR
	qRT
	qRE
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
		
