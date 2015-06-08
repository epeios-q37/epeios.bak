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

#define TAGEXP__COMPILATION

#include "tagexp.h"


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
qRH
	STR_BUFFER___ Name;
	flf::file_iflow___ File;
	xtf::extended_text_iflow__ IFlow;
qRB
	FileName.Convert( Name );

	if ( ( Return = File.Init( Name, ErrHandle ) ) == fil::sSuccess ){
		File.EOFD( XTF_EOXT );

		IFlow.Init( File );

		while( !IFlow.EOX() )
			OFlow.Put( IFlow.Get() );
	}
qRR
qRT
qRE

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
