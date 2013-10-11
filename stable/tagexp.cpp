/*
	'tagexp' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Requires the 'tagexp' header file ('tagexp.h').
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



//	$Id: tagexp.cpp,v 1.16 2010/07/15 18:26:44 csimon Exp $

#define TAGEXP__COMPILATION

#include "tagexp.h"

class tagexptutor
: public ttr_tutor
{
public:
	tagexptutor( void )
	: ttr_tutor( TAGEXP_NAME )
	{
#ifdef TAGEXP_DBG
		Version = TAGEXP_VERSION "\b\bD $";
#else
		Version = TAGEXP_VERSION;
#endif
		Owner = TAGEXP_OWNER;
		Date = "$Date: 2010/07/15 18:26:44 $";
	}
	virtual ~tagexptutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

#include "flf.h"

using namespace tagexp;

// Must correspond to 'tagexp::tag'.
static const char *Labels_[tagexp::t_amount] =
{
	"_PRINT_",
	"_SKIP_",
	"_RAW_",
	"_END_"
};

const char *tagexp::TagLabel( tagexp::tag_id Id )
{
	return Labels_[Id];
}



void tagexp::tag_expander_::Init( void )
{
	References.Init();
	Detector.Init();

	for( bso::ubyte__ i = 0; i < tagexp::t_amount; i++ )
		if ( Add( str::string( "" ), tagexp::nText, str::string( TagLabel( (tagexp::tag_id)i ) ) ) != i )
			ERRc();
}

static fil::status__ WriteFile_(
	const str::string_ &FileName,
	txf::text_oflow__ &OFlow,
	err::handle ErrHandle )
{
	fil::status__ Return = fil::s_Undefined;
ERRProlog
	STR_BUFFER___ Name;
	flf::file_iflow___ File;
	xtf::extended_text_iflow__ IFlow;
ERRBegin
	FileName.Convert( Name );

	if ( ( Return = File.Init( Name, ErrHandle ) ) == fil::sSuccess ){
		File.EOFD( XTF_EOXT );

		IFlow.Init( File );

		while( !IFlow.EOX() )
			OFlow.Put( IFlow.Get() );
	}
ERRErr
ERREnd
ERREpilog

	return Return;
}


static status HandleReference_(
	const value_ &Value,
	nature Nature,
	txf::text_oflow__ &OFlow,
	err::handle ErrHandle )
{
	tagexp::status Status = tagexp::sOK;

	switch ( Nature ) {
	case tagexp::nText:
		OFlow << Value;
		break;
	case tagexp::nFile:
		if ( WriteFile_( Value, OFlow, ErrHandle ) != fil::sSuccess )
			Status = tagexp::sBadFile;
		break;
	case tagexp::nUserDefined:
		Status = sUserDefined;
		break;
	default:
		ERRc();
		break;
	}

	return Status;
}

status tagexp::tag_expander_::Expand(
	xtf::extended_text_iflow__ &IFlow,
	txf::text_oflow__ &OFlow,
	bso::char__ Delimiter,
	str::string_ &File,
	tagexp::action Action,
	err::handle ErrHandle ) const
{
	bso::bool__ Loop = true;
	id__ Id = TAGDTC_UNKNOW;
	ctn::E_CMITEM( value_ ) Value;
	tagexp::status Status = tagexp::sOK;

	Value.Init( References.Values );

	while( Loop && ( Status == tagexp::sOK ) ) {
		Id = Detector.Parse( IFlow, OFlow, Delimiter, Action, ErrHandle );

		if ( *Id < tagexp::t_amount ) {
			switch( *Id ) {
			case tagexp::tPrint:
				Action = tagexp::aPrint;
				break;
			case tagexp::tSkip:
				Action = tagexp::aSkip;
				break;
			case tagexp::tRaw:
				Loop = false;
				while( !IFlow.EOX() )
					OFlow.Put( IFlow.Get() );
				break;
			case tagexp::tEnd:
				Loop = false;
				break;
			default:
				ERRc();
				break;
			}
			if ( Action == tagexp::aPrint )
				Status = HandleReference_( Value( Id ), References.Natures( Id ), OFlow, ErrHandle );
		}
		else if ( *Id > TAGDTC_MAX_ID ) {
			switch ( *Id ) {
			case TAGDTC_EOF:
				Loop = false;
				break;
			case TAGDTC_UNKNOW:
				Status = tagexp::sUnknowTag;
				break;
			default:
				ERRc();
				break;
			}
		}
		else if ( Action == tagexp::aPrint )
			Status = HandleReference_( Value( Id ), References.Natures( Id ), OFlow, ErrHandle );

	}

	if ( Status == tagexp::sUserDefined )
		Status = (status)*Id;
	else if ( Status == tagexp::sBadFile )
	 	File = Value();

	return Status;
}


/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class tagexppersonnalization
: public tagexptutor
{
public:
	tagexppersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~tagexppersonnalization( void )
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

static tagexppersonnalization Tutor;

ttr_tutor &TAGEXPTutor = Tutor;
