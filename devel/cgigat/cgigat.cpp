/*
	Copyright (C) 1999-2016 Claude SIMON (http://q37.info/contact/).

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

#define CGIGAT__COMPILATION

#include "cgigat.h"

#include "flx.h"

using namespace cgigat;

void cgigat::cgi_gate__::_Process(
	const cgiarg::arguments_ &Arguments,
	str::string_ &XML,
	str::string_ &XSLFileName )
{
qRH
	flx::E_STRING_OFLOW___ SFlow;
	txf::text_oflow__ TFlow;
	xml::writer Writer;
qRB
	SFlow.Init( XML );
	TFlow.Init( SFlow );
	Writer.Init( TFlow, xml::oCompact, xml::e_Default );

	_Process( Arguments, Writer, XSLFileName );
qRR
qRT
qRE
}



csdscb::action__ cgigat::cgi_gate__::CSDSCBProcess(
	flw::ioflow__ &Flow,
	void *UP )
{
qRH
	cgiarg::arguments Arguments;
	str::string XML;
	str::string XSLFileName;
	txf::text_oflow__ TFlow;
qRB
	Arguments.Init();
	Arguments.Parse( Flow );

	XML.Init();
	XSLFileName.Init();

	_Process( Arguments, XML, XSLFileName );

	TFlow.Init( Flow );

	TFlow << XSLFileName << txf::nl << XML << txf::commit;
qRR
qRT
qRE
	return csdscb::aStop;	// Pour fermer la connection.
}
