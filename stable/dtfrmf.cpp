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

#define DTFRMF__COMPILATION

#include "dtfrmf.h"

using namespace dtfrmf;


#define M1	255
#define M2	65535
#define M3	16777215

void dtfrmf::PutSize(
	bso::ulong__ Size,
	flw::oflow___ &Flow )
{
	if ( Size >= M1 )
	{
		Flow.Put( (flw::datum__)M1 );
		Size -= M1;

		if ( Size >= M2 )
		{
			Flow.Put( (flw::datum__)M1 );
			Flow.Put( (flw::datum__)M1 );
			Size -= M2;

			if ( Size >= M3 )
			{
				Flow.Put( (flw::datum__)M1 );
				Flow.Put( (flw::datum__)M1 );
				Flow.Put( (flw::datum__)M1 );

				Size -= M3;

				Flow.Put( (flw::datum__)( ( Size & ( 255 << 24 ) ) >> 24 ) );
			}
			Flow.Put( (flw::datum__)( ( Size & ( 255 << 16 ) ) >> 16 ) );
		}
		Flow.Put( (flw::datum__)( ( Size & ( 255 << 8 ) ) >> 8 ) );
	}
	Flow.Put( (flw::datum__)( Size & 255 ) );
}

bso::ulong__ dtfrmf::GetSize( flw::iflow___ &IFlow )
{
	bso::ulong__ Size = (bso::ubyte__)IFlow.Get();

	if ( Size == M1 )
	{
		flw::datum__ Data[4];

		IFlow.Get( 2, Data );

		Size += ( Data[0] << 8 ) | Data[1];

		if ( Size == ( M1 + M2 ) )
		{
			IFlow.Get( 3, Data );

			Size += ( Data[0] << 16 ) | ( Data[1] << 8 ) | Data[2];

			if ( Size == ( M1 + M2 + M3 ) ) {

				IFlow.Get( 4, Data );

				Size += ( Data[0] << 24 ) | ( Data[1] << 16 ) | ( Data[2] << 8 ) | Data[3];
			}
		}
	}

	return Size;
}

void dtfrmf::Encapsulate(
	const flx::bunch_ &Data,
	flw::oflow___ &Flow )
{
ERRProlog
	flx::bunch_iflow___ IFlow;
	tym::size__ Size;
ERRBegin
	IFlow.Init( Data );
	
	Size = Data.Amount();
	
	PutSize( Size, Flow );

	while( Size-- )	// Should be optimized by using a buffer.
		Flow.Put( IFlow.Get() );
ERRErr
ERREnd
ERREpilog
}

void dtfrmf::Decapsulate(
	flw::iflow___ &Flow,
	flx::bunch_ &Data )
{
ERRProlog
	flx::bunch_oflow___ OFlow;
	tym::size__ Size;
ERRBegin
	OFlow.Init( Data );

	Size = GetSize( Flow );

	while( Size-- )	// Should be optimized by using a buffer.
		OFlow.Put( Flow.Get() );
ERRErr
ERREnd
ERREpilog
}
