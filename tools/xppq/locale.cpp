/*
	Copyright (C) 2007, 2008, 2010-2015 Claude SIMON (http://q37.info/contact/).

	This file is part of the xppq tool.

	The xppq tool is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

	The xppq tool is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with the Epeios framework.  If not, see <http://www.gnu.org/licenses/>
*/

#include "locale.h"

#include "scllocale.h"

#include <stdarg.h>

#include "flf.h"
#include "xtf.h"
#include "xml.h"
#include "fnm.h"
#include "dir.h"

#include "sclrgstry.h"
#include "scltool.h"

using namespace locale;

#define CASE( l )\
	case t##l:\
	return #l;\
	break

const char *locale::Label( text__ Text )
{
#if	LOCALE__TEXT_AMOUNT != 8
#	error "Amount of 'message__' entries changed ! Update !"
#endif

	switch( Text ) {
	CASE( ProcessCommandDescription );
	CASE( EncryptCommandDescription );
	CASE( NamespaceOptionDescription );
	CASE( NoIndentOptionDescription );
	CASE( SourceFileArgumentDescription );
	CASE( DestFileArgumentDescription );
	CASE( ProcessingError );
	CASE( EncryptionError );
	default:
		ERRFwk();
		break;
	}

	return NULL;	// To avoir a 'warning'.
}

const lcl::meaning_ &locale::GetMeaning_(
	text__ Text,
	lcl::meaning_ *Meaning,
	... )
{
#if	LOCALE__TEXT_AMOUNT != 8
#	error "Amount of 'message__' entries changed ! Update !"
#endif
ERRProlog
	va_list Args;
	lcl::meaning MeaningBuffer;
ERRBegin
	va_start( Args, Meaning );

	Meaning->SetValue( Label( Text) );

	switch ( Text ) {
	case tProcessCommandDescription:
	case tEncryptCommandDescription:
	case tNamespaceOptionDescription:
	case tNoIndentOptionDescription:
		Meaning->AddTag( va_arg( Args, const char *) );
		break;
	case tSourceFileArgumentDescription:
	case tDestFileArgumentDescription:
		break;
	case tProcessingError:
		MeaningBuffer.Init();
		xpp::GetMeaning( *va_arg( Args, const xpp::context___ *), MeaningBuffer );

		Meaning->AddTag( MeaningBuffer );
		break;
	case tEncryptionError:
		MeaningBuffer.Init();
		xpp::GetMeaning( *va_arg( Args, const xpp::context___ *), MeaningBuffer );

		Meaning->AddTag( MeaningBuffer );
		break;
	default:
		ERRFwk();
		break;
	}
ERRErr
ERREnd
ERREpilog
	return *Meaning;
}

static struct locale_cdtor {
	locale_cdtor( void )
	{
		if ( LOCALE__TEXT_AMOUNT != t_amount )
			ERRChk();
	}
	~locale_cdtor( void )
	{
	}
} GLOBALCDTor_;
