/*
	Copyright (C) 2021 Claude SIMON (http://zeusw.org/epeios/contact.html).

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
		void BuilTree_(
			core::rSession &Session,
			xml::rWriter &Writer )
			{
				Writer.PushTag("Children");
				Writer.PutValue("Child1", "Child");
				Writer.PutValue("Child2", "Child");
				Writer.PutValue("Child3", "Child");
				Writer.PopTag();
			}
		void Get(
			core::rSession &Session,
			xml::rWriter &Writer )
		{}
	}
}

void main::SetLayout( core::rSession &Session )
{
	Session.Inner( "", XSLAffix_, layout_::Get );
}

namespace {
	void BuildTree_(
		const char *Id,
		core::rSession &Session)
	{
		Session.Inner(Id, "Tree", layout_::BuilTree_);
	}
}

void main::Display( core::rSession &Session )
{
	SetLayout( Session );

	Session.SetValue("Input", str::wString("***coucou***"));
	Session.Execute("var essai = edit('Input')");
	Session.Focus("Input");

	BuildTree_("Tree", Session);

	Session.SwitchTo( base::pMain );
}

#define A( name ) BASE_ADef( main, name )

namespace {
	void Submit_( core::rSession &Session )
	{
	qRH;
		str::wString Content;
	qRB;
		Content.Init();
		Session.Execute( "essai.value()", Content );

		Session.Execute("display('%Content%', 'Output')", "Content", NULL, Content);
	qRR;
	qRT;
	qRE;
	}
}

A( Submit )
{
	Submit_(Session);
}

A( Toggle )
{
qRH;
		qCBUFFERh Buffer;
qRB;
	const char *TargetId = Session.LastChild(Session.Parent(Id, Buffer), Buffer);

	if ( Session.BGetValue(Id) )
		BuildTree_(TargetId, Session);
	else
		Session.SetValue(TargetId, "");
qRR;
qRT;
qRE;
}
