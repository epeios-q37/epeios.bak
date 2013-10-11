/*
	'mmm2' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Requires the 'mmm2' header file ('mmm2.h').
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



//	$Id: mmm2.cpp,v 1.40 2012/11/14 16:06:31 csimon Exp $

#define MMM2__COMPILATION

#include "mmm2.h"

class mmm2tutor
: public ttr_tutor
{
public:
	mmm2tutor( void )
	: ttr_tutor( MMM2_NAME )
	{
#ifdef MMM2_DBG
		Version = MMM2_VERSION "\b\bD $";
#else
		Version = MMM2_VERSION;
#endif
		Owner = MMM2_OWNER;
		Date = "$Date: 2012/11/14 16:06:31 $";
	}
	virtual ~mmm2tutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

#include "flf.h"

using namespace mmm;

static void Display_(
	row__ Position,
	txf::text_oflow__ &Flow )
{
	if ( Position == NONE )
		Flow << "NONE";
	else
		Flow << *Position;
}

void mmm::multimemory_::DisplayStructure( txf::text_oflow__ &Flow ) const
{
	bso::ulong__ UsedFragmentTotalSize = 0, UsedFragmentDataSize = 0, FreeFragmentSize = 0;

	Flow << "Extent : " << S_.Extent << txf::tab;
	Flow << "FreeFragment : ";
	Display_( S_.FreeFragment, Flow );
	Flow << txf::tab;

	Flow << "Tailing free fragment position : ";
	Display_( S_.TailingFreeFragmentPosition, Flow );
	
	Flow << txf::nl;

	Flow << "Pos." << txf::tab << "  State" << txf::tab << "Size" << txf::tab << "Next" << txf::tab << "DataS" << txf::tab << txf::tab << "PF" << txf::tab << "NF/L" << txf::nl;

	if ( S_.Extent != 0 ) {
		row__ Position = 0;
		mdr::datum__ Header[MMM2_HEADER_MAX_LENGTH];

		while ( Position != NONE ) {
			Flow << *Position << txf::tab << ": ";

			_GetHeader( Position, Header );

			if ( _IsFragmentUsed( Header ) ) {
				Flow << "USED" << txf::tab << _GetUsedFragmentTotalSize( Header ) << txf::tab << ( *Position + _GetUsedFragmentTotalSize( Header ) ) << txf::tab << _GetUsedFragmentDataSize( Header ) << txf::tab << txf::tab;

				UsedFragmentTotalSize += _GetUsedFragmentTotalSize( Header );
				UsedFragmentDataSize += _GetUsedFragmentDataSize( Header );

				if ( _IsUsedFragmentFreeFlagSet( Header ) )
					Flow << *_GetFreeFragmentPosition( Position );
				else
					Flow << "NONE";

				Flow << txf::tab;

				if ( _IsUsedFragmentLinkFlagSet( Header ) )
					Flow << *_GetUsedFragmentLink( Position, Header );
				else
					Flow << "NONE";

				Position = _GetUsedFragmentNextFragmentPosition( Position, Header );
			} else if ( _IsFragmentFree( Header ) ) {
				if ( _IsFreeFragmentOrphan( Header ) ) {
					Flow << "Orph." << txf::tab << _GetFreeFragmentSize( Header ) << txf::tab << ( *Position + _GetFreeFragmentSize( Header ) ) << txf::tab;
				} else {
					Flow << "Free" << txf::tab << _GetFreeFragmentSize( Header ) << txf::tab << ( *Position + _GetFreeFragmentSize( Header ) ) << txf::tab << txf::tab << txf::tab;
					Display_( _GetFreeFragmentPreviousFreeFragmentPosition( Header ), Flow );
					Flow << txf::tab;
					Display_( _GetFreeFragmentNextFreeFragmentPosition( Header ), Flow );
				}

				FreeFragmentSize += _GetFreeFragmentSize( Header );

				Position = _GetFreeFragmentNextFragmentPosition( Position, Header );
			} else
				ERRc();

			Flow << txf::nl;

			if ( Position == S_.Extent )
				Position = NONE;
			else if ( *Position > S_.Extent )
				ERRc();

		}

		Flow << "DS : " << UsedFragmentDataSize << txf::tab << "OH : " << ( 100 * ( UsedFragmentTotalSize - UsedFragmentDataSize ) ) / UsedFragmentDataSize << '%' << txf::tab << "FS : " << FreeFragmentSize << txf::tab << "R : " << ( 100 * FreeFragmentSize ) / UsedFragmentDataSize << '%' << txf::nl; 
	}
}

void mmm::multimemory_file_manager___::_WriteFreeFragmentPositions( void )
{
ERRProlog
	flf::file_oflow___ OFlow;
ERRBegin
	OFlow.Init( _FreeFragmentPositionFileName );

	flw::Put( _Multimemory->S_.FreeFragment, OFlow );
	flw::Put( _Multimemory->S_.TailingFreeFragmentPosition, OFlow );
ERRErr
ERREnd
ERREpilog
}

bso::bool__ mmm::Connect(
	multimemory_ &Multimemory,
	multimemory_file_manager___ &FileManager )
{
	bso::bool__ Exists = false;
ERRProlog
	flf::file_iflow___ IFlow;
ERRBegin
	Exists = uym::Connect( Multimemory.Memory, FileManager );

	if ( Exists ) {
		Multimemory.S_.Extent = FileManager.FileSize();
		IFlow.Init( FileManager._FreeFragmentPositionFileName );

		flw::Get( IFlow, Multimemory.S_.FreeFragment );
		flw::Get( IFlow, Multimemory.S_.TailingFreeFragmentPosition );
	}
ERRErr
ERREnd
ERREpilog
	return Exists;
}




/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class mmm2personnalization
: public mmm2tutor
{
public:
	mmm2personnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~mmm2personnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the ending of the application  */
	}
};


/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

// 'static' by GNU C++.

static mmm2personnalization Tutor;

ttr_tutor &MMM2Tutor = Tutor;
