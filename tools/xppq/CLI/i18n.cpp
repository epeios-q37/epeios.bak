/*
	Copyright (C) 2007-2017 Claude SIMON (http://q37.info/contact/).

	This file is part of XPPq.

	XPPq is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

	XPPq is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with XPPq. If not, see <http://www.gnu.org/licenses/>.
*/

#include "i18n.h"

#include "scll.h"

#include <stdarg.h>

#include "flf.h"
#include "xtf.h"
#include "xml.h"
#include "fnm.h"
#include "dir.h"

#include "sclr.h"
#include "sclt.h"

using namespace i18n;

#define CASE( l )\
	case t##l:\
	return #l;\
	break

const char *i18n::Label( text__ Text )
{
#if	LOCALE__TEXT_AMOUNT != 2
#	error "Amount of 'message__' entries changed ! Update !"
#endif

	switch( Text ) {
	CASE( ProcessingError );
	CASE( EncryptionError );
	default:
		qRGnr();
		break;
	}

	return NULL;	// To avoir a 'warning'.
}

const lcl::meaning_ &i18n::GetMeaning_(
	text__ Text,
	lcl::meaning_ *Meaning,
	... )
{
#if	LOCALE__TEXT_AMOUNT != 2
#	error "Amount of 'message__' entries changed ! Update !"
#endif
qRH
	va_list Args;
	lcl::meaning MeaningBuffer;
qRB
	va_start( Args, Meaning );

	Meaning->SetValue( Label( Text) );

	switch ( Text ) {
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
		qRGnr();
		break;
	}
qRR
qRT
qRE
	return *Meaning;
}

Q37_GCTOR( i18n )
{
	if ( LOCALE__TEXT_AMOUNT != t_amount )
		qRChk();
}
