/*
	'ags' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'ags' header file ('ags.h').
	Copyright (C) 20132004 Claude SIMON.

	This file is part of the Epeios (http://zeusw.org/epeios/) project.

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
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



//	$Id: ags.cpp,v 1.12 2013/04/26 14:43:40 csimon Exp $

#define AGS__COMPILATION

#include "ags.h"

class agstutor
: public ttr_tutor
{
public:
	agstutor( void )
	: ttr_tutor( AGS_NAME )
	{
#ifdef AGS_DBG
		Version = AGS_VERSION "\b\bD $";
#else
		Version = AGS_VERSION;
#endif
		Owner = AGS_OWNER;
		Date = "$Date: 2013/04/26 14:43:40 $";
	}
	virtual ~agstutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace ags;

void aggregated_storage_driver__::_Free( void )
{
	if ( _Descriptor != AGS_UNDEFINED_DESCRIPTOR )
		_AStorage->Free( _Descriptor );
}

void aggregated_storage_driver__::SDRAllocate( sdr::size__ Size )
{
	_Descriptor = _AStorage->Reallocate( _Descriptor, Size );
}


sdr::size__ aggregated_storage_driver__::_UnderlyingSize( void ) const
{
	if ( _Descriptor != AGS_UNDEFINED_DESCRIPTOR )
		return _AStorage->Size( _Descriptor );
	else
		return 0;
}

void aggregated_storage_driver__::SDRRecall(
	sdr::row_t__ Position,
	sdr::size__ Amount,
	sdr::datum__ *Buffer )
{
	_AStorage->Read( _Descriptor, Position, Amount, Buffer );
}
// lit à partir de 'Position' et place dans 'Tampon' 'Nombre' octets;
void aggregated_storage_driver__::SDRStore(
	const sdr::datum__ *Buffer,
	sdr::size__ Amount,
	sdr::row_t__ Position )
{
	_AStorage->Write( Buffer, Amount, _Descriptor, Position );
}


void ags::aggregated_storage_::DisplayStructure( txf::text_oflow__ &Flow ) const
{
	sdr::row_t__ Row = 0;

	while ( Row < _UnderlyingSize() ) {
		_Display( Row, Flow );

		Row += _GetFragmentSize( Row );

		Flow << txf::tab << txf::commit;
	}

	Flow << txf::nl;
}


/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class agspersonnalization
: public agstutor
{
public:
	agspersonnalization( void )
	{
		if ( sizeof( header__ ) != AGS__HEADER_SIZE )
			ERRChk();
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~agspersonnalization( void )
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

static agspersonnalization Tutor;

ttr_tutor &AGSTutor = Tutor;
