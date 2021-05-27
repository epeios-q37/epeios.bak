/*
	Copyright (C) 2018 Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of 'TaskQ' software.

	'TaskQ' is free software: you can redistribute it and/or modify it
	under the terms of the GNU Affero General Public License as published
	by the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	'TaskQ' is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with 'TaskQ'.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "main.h"

#include "core.h"
#include "registry.h"

namespace{
	qCDEF( char *, XSLAffix_, "Main" );

	namespace layout_ {
		void Get(
			core::rSession &Session,
			xml::rWriter &Writer )
		{}
	}
}

void main::SetLayout( core::rSession &Session )
{
	Session.SetDocumentLayout( XSLAffix_, layout_::Get );
}

void main::Display( core::rSession &Session )
{
	SetLayout( Session );

	Session.SetContent("Input", str::wString("***coucou***"));
	Session.Execute("edit('Input');");
	Session.Focus("Input");

	Session.SwitchTo( base::pMain );
}

#define A( name ) BASE_ADef( main, name )

namespace {
	void Submit_( core::rSession &Session )
	{
	qRH;
		str::wString Content, Script;
	qRB;
		Content.Init();
//		Session.GetContent( "Input", Content );
		Session.Execute( "get()", Content );

		cio::COut << Content << txf::nl << txf::commit;

		Script.Init();
		Script.Append("display('");
		Script.Append(Content);
		Script.Append("','Output');");

		Session.Execute(Script);
	qRR;
	qRT;
	qRE;
	}
}

A( Submit )
{
	Submit_(Session);
}

