/*
	'mmm' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Requires the 'mmm' header file ('mmm.h').
	Copyright (C) 2000-2001, 2004 Claude SIMON (http://zeusw.org/intl/contact.html).

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



//	$Id: mmm.cpp,v 1.49 2013/03/14 11:09:19 csimon Exp $

#define MMM__COMPILATION

#include "mmm.h"

class mmmtutor
: public ttr_tutor
{
public:
	mmmtutor( void )
	: ttr_tutor( MMM_NAME )
	{
#ifdef MMM_DBG
		Version = MMM_VERSION "\b\bD $";
#else
		Version = MMM_VERSION;
#endif
		Owner = MMM_OWNER;
		Date = "$Date: 2013/03/14 11:09:19 $";
	}
	virtual ~mmmtutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

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

	Flow << "Size : " << _Size() << txf::tab;
	Flow << "FreeFragment : ";
	Display_( S_.FreeFragment, Flow );
	Flow << txf::tab;

	Flow << "Tailing free fragment position : ";
	Display_( S_.TailingFreeFragmentPosition, Flow );
	
	Flow << txf::nl;

	Flow << "Pos." << txf::tab << "  State" << txf::tab << "Size" << txf::tab << "Next" << txf::tab << "DataS" << txf::tab << txf::tab << "PF" << txf::tab << "NF/L" << txf::nl;

	if ( _Size() != 0 ) {
		row__ Position = 0;
		mdr::datum__ Header[MMM_HEADER_MAX_LENGTH];

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

			if ( Position == _Size() )
				Position = NONE;
			else if ( *Position > _Size() )
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

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */
class mmmpersonnalization
: public mmmtutor
{
public:
	mmmpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~mmmpersonnalization( void )
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

static mmmpersonnalization Tutor;

ttr_tutor &MMMTutor = Tutor;
