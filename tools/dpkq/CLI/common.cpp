#include "common.h"
#include "common.h"
#include "common.h"
/*
	Copyright (C) 2010-2015 Claude SIMON (http://q37.info/contact/).

	This file is part of dpkq.

    dpkq is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

    dpkq is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with dpkq.  If not, see <http://www.gnu.org/licenses/>
*/

#include "common.h"

#include "registry.h"

#include "sclerror.h"
#include "sclmisc.h"

using namespace common;

namespace {
	void GetMeaning_(
		const xpp::preprocessing_iflow___ &IFlow,
		lcl::meaning_ &Meaning )
	{
	qRH
		xpp::context___ Context;
	qRB
		Context.Init();
		IFlow.GetContext( Context );

		if ( Context.Status != xpp::sOK )
			qRGnr();

		xpp::GetMeaning( Context.Coordinates, Meaning );
	qRR
	qRT
	qRE
	}
}

void common::ReportAndAbort( const xpp::preprocessing_iflow___ &IFlow )
{
qRH
	lcl::meaning Error, XPPError;
qRB
	Error.Init();
	Error.SetValue( "GenericError" );

	XPPError.Init();
	xpp::GetMeaning( IFlow, XPPError );

	Error.AddTag( XPPError );

	sclerror::SetMeaning( Error );

	qRAbort();
qRR
qRT
qRE
}

void common::ReportAndAbort( 
	const lcl::meaning_ &Meaning,
	const xpp::preprocessing_iflow___ &IFlow )

{
qRH
	lcl::meaning GenericMeaning;
	lcl::meaning MeaningBuffer;
qRB
	GenericMeaning.Init();
	GenericMeaning.SetValue( "GenericError" );

//	GenericMeaning.AddTag( Meaning );
	
	MeaningBuffer.Init();
	GetMeaning_( IFlow, MeaningBuffer );
	MeaningBuffer.AddTag( Meaning );

	GenericMeaning.AddTag( MeaningBuffer );

	sclmisc::ReportAndAbort( GenericMeaning );
qRR
qRT
qRE
}

void common::ReportAndAbort( 
	const char *Error,
	const xpp::preprocessing_iflow___ &IFlow )

{
qRH
	lcl::meaning Meaning;
qRB
	Meaning.Init();
	Meaning.SetValue( Error );

	ReportAndAbort( Meaning, IFlow );
qRR
qRT
qRE
}



