/*
	Copyright (C) 1999 Claude SIMON (http://q37.info/contact/).

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

#define XDHUJS_COMPILATION_

#include "xdhujs.h"

#include "xdhujr.h"

#include "sclmisc.h"

#include "stsfsm.h"

using namespace xdhujs;

namespace {
    namespace {
        namespace {
            rgstry::rEntry Scripts_("Scripts", sclrgstry::Definitions);
        }
        rgstry::rEntry LooseScript_("Script", Scripts_);
    }

    rgstry::rEntry TaggedScript_(RGSTRY_TAGGING_ATTRIBUTE("name"), LooseScript_ );
    rgstry::rEntry TaggedScriptArguments_("@Arguments", TaggedScript_ );
}


const str::string_ &xdhujs::GetTaggedScript(
	const char *Name,
	str::string_ &Buffer )
{
    sclmisc::MGetValue(rgstry::rTEntry(TaggedScript_, Name), Buffer);

    return Buffer;
}

namespace {
    namespace {
        void AppendRawTag_(
            const str::dString &Name,
            const str::dString &Value,
            str::strings_ &Names,
            str::strings_ &Values )
        {
            Names.Append( Name );
            Values.Append( Value );
        }

        void AppendEscapedTag_(
            const str::dString &Name,
            const str::dString &Value,
            str::strings_ &Names,
            str::strings_ &Values )
        {
        qRH
            str::string EscapedValue;
        qRB
            Names.Append( str::string( Name ) );
            EscapedValue.Init();
            xdhcmn::Escape( Value, EscapedValue, '"' );
            Values.Append( EscapedValue );
        qRR
        qRT
        qRE
        }
    }

	void AppendTag_(
        const str::dString &Name,
        const str::dString &Value,
		str::strings_ &Names,
		str::strings_ &Values )
	{
        if ( !Name.Amount() )
            qRFwk();

        if ( Name(Name.First()) == '#')
            AppendRawTag_(Name, Value, Names, Values );
        else
            AppendEscapedTag_(Name, Value, Names, Values );
	}

	namespace {
        namespace {
            void Split_(
                flw::rRFlow &Flow,
                str::dStrings &Splitted )
            {
            qRH
                str::wString Item;
                bso::sChar C = 0;
            qRB
                Item.Init();

                while ( !Flow.EndOfFlow() ) {
                    C = Flow.Get();

                    if ( C == ',') {
                        if ( Item.Amount()) {
                            Splitted.Append(Item);
                            Item.Init();
                        }
                    } else
                        Item.Append(C);
                }

                if ( Item.Amount() )
                    Splitted.Append(Item);
            qRR
            qRE
            qRT
            }
        }
        void Split_(
            const str::dString &Merged,
            str::dStrings &Splitted )
        {
            flx::sStringRFlow Flow;

            Flow.Init(Merged);

            Split_(Flow, Splitted);
        }
	}


	void SubstituteArguments_(
		const str::string_ &Tagged,	// Script with tags. When returning, tags are substitued.
		const str::dStrings &Arguments,
		const str::dStrings &Values,
		str::dString &Substituted )
	{
	qRH
		str::strings Tags, TagValues;
		sdr::sRow Row = qNIL;
	qRB
        if ( Arguments.Amount() != Values.Amount())
            qRFwk();

		Tags.Init();
		TagValues.Init();

		Row = Arguments.First();

		while ( Row != qNIL ) {
			AppendTag_(Arguments(Row), Values(Row), Tags, TagValues);

			Row = Arguments.Next(Row);
		}

		tagsbs::SubstituteLongTags( Tagged, Tags, TagValues, Substituted );
	qRR
	qRT
	qRE
	}

	void GetScriptArguments_(
        const char *Name,
        str::dStrings &Splitted)
	{
	qRH
        str::wString Merged;
	qRB
        Merged.Init();

        sclmisc::MGetValue(rgstry::rTEntry(TaggedScriptArguments_, Name), Merged);

        Split_(Merged, Splitted);
	qRR
	qRE
	qRT
	}
}

void xdhujs::GetScript(
	const char *Name,
	const str::dStrings &Values,
	str::string_ &Script)
{
qRH
	str::string TaggedScript;
	str::wStrings Arguments;
qRB
	TaggedScript.Init();
	GetTaggedScript( Name, TaggedScript );

	Arguments.Init();
	GetScriptArguments_(Name, Arguments);

	SubstituteArguments_(TaggedScript, Arguments, Values, Script);
qRR
qRT
qRE
}

/*
const str::string_ &xdhujs::GetScript(
	const str::dString &Name,
	const str::dStrings &Values,
	str::string_ *Buffer )
{
qRH
	va_list List;
qRB
	va_start( List, Buffer );

	GetScript( ScriptName, *Buffer, List );
qRR
qRT
	va_end( List );
qRE
	return *Buffer;
}
*/
