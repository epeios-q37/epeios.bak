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

#define XDHDWS_COMPILATION_

#include "xdhdws.h"

#include "xdhutl.h"

#include "mtk.h"

using namespace xdhdws;

// Special scripts, for internal use,
// which are not directly launched by the user.
namespace ss_ {	// Special Script
	qCDEF(char *, SetEventHandlers, "@SetEventHandlers_1");
	qCDEF(char *, InstantiateWidgets, "@InstantiateWidgets_1" );
	qCDEF(char *, BroadcastAction, "@BroadcastAction_1");
}

// Name of scripts, called by the user, which are 'patched',
// i.e. some special additional actions are made.
namespace psn_ {	// Patched Script Name
	qCDEF(char *, HandleLayout, "HandleLayout_1");
}

namespace {
  rgstry::rEntry Scripts_("Scripts", sclr::Definitions);
}

namespace {
  rgstry::rEntry ScriptsVersion_("@Version", Scripts_);
}

sScriptsVersion xdhdws::GetScriptsVersion(void)
{
  return sclm::MGetU8(ScriptsVersion_);
}

namespace {
	namespace {
		namespace {
			namespace {
				rgstry::rEntry LooseScript_("Script", Scripts_);
			}

			namespace {
				namespace {
					rgstry::rEntry TaggedScript_(RGSTRY_TAGGING_ATTRIBUTE("name"), LooseScript_ );
				}

				template <typename type> const str::string_ &GetTaggedScript_(
					const type &Name,
					str::string_ &Buffer )
				{
					sclm::MGetValue(rgstry::rTEntry(TaggedScript_, Name), Buffer);

					return Buffer;
				}
			}

			namespace {
				namespace {
					rgstry::rEntry TaggedScriptArguments_("@Arguments", TaggedScript_ );
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

				template <typename type> void GetScriptTags_(
					const type &Name,
					str::dStrings &Tags)
				{
				qRH
					str::wString MergedTags;
				qRB
					MergedTags.Init();

					sclm::MGetValue(rgstry::rTEntry(TaggedScriptArguments_, Name), MergedTags);

					Split_(MergedTags, Tags);
				qRR
				qRE
				qRT
				}
			}

			namespace {
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
				}

				void SubstituteTags_(
					const str::string_ &TaggedScript,	// Script with tags. When returning, tags are substitued.
					const str::dStrings &TagList,
					const str::dStrings &RawTagValues,
					str::dString &Script )
				{
				qRH
					str::strings Tags, TagValues;
					sdr::sRow Row = qNIL;
				qRB
					if ( TagList.Amount() != RawTagValues.Amount())
						qRFwk();

					Tags.Init();
					TagValues.Init();

					Row = TagList.First();

					while ( Row != qNIL ) {
						AppendTag_(TagList(Row), RawTagValues(Row), Tags, TagValues);

						Row = TagList.Next(Row);
					}

					tagsbs::SubstituteLongTags(TaggedScript, Tags, TagValues, Script);
				qRR
				qRT
				qRE
				}
			}

			template <typename type> void GetScript_(
				const type &Name,
				const str::dStrings &TagValues,
				str::string_ &Script)
			{
			qRH
				str::string TaggedScript;
				str::wStrings TagList;
			qRB
				TaggedScript.Init();
				GetTaggedScript_( Name, TaggedScript );

				TagList.Init();
				GetScriptTags_(Name, TagList);

				SubstituteTags_(TaggedScript, TagList, TagValues, Script);
			qRR
			qRT
			qRE
			}
		}

		template <typename type> bso::sBool BaseProcess_(
			const type &ScriptName,
			const str::dStrings &TagValues,
			xdhcuc::cSingle &Callback,
			tht::rBlocker *Blocker = NULL,
			bso::sBool *SuccessPointer = NULL,
			str::dString *ReturnValue = NULL)
		{
			bso::sBool Success = false;
		qRH
			str::wString Script;
		qRB
			Script.Init();

			GetScript_(ScriptName, TagValues, Script);

			Success = Callback.Process(Script, Blocker, SuccessPointer, ReturnValue );
		qRR
		qRT
		qRE
			return Success;
		}
	}

	namespace {
		namespace {
			bso::sBool ExecuteAndGetDigest_(
				const char *ScriptName,
				const str::dStrings &TagValues,
				xdhcuc::cSingle &Callback,
				xdhcmn::digest_ &Digest,
				tht::rBlocker &Blocker,
				bso::sBool *SuccessPointer)
			{
				bso::sBool Success = false;
			qRH
				str::wString RawDigest;
			qRB
				RawDigest.Init();

				if ( ( Success = BaseProcess_(ScriptName, TagValues, Callback, &Blocker, SuccessPointer, &RawDigest) ) ) {
					Digest.Init();
					xdhcmn::Split( RawDigest, Digest );
				}
			qRR
			qRE
			qRT
				return Success;
			}
		}

		namespace {
			namespace {
				namespace {
					void Append_(
						const str::string_ &Item,
						str::string_ &Tag )
					{
						Tag.Append('"');
						Tag.Append( Item );
						Tag.Append('"');
					}
				}

				void HandleEvents_(
					const str::strings_ &Ids,
					const xdhutl::event_abstracts_ &Abstracts,
					str::string_ &IdsTag,
					str::string_ &EventsTag )
				{
					sdr::row__ Row = qNIL;

					if ( Ids.Amount() != Abstracts.Amount() )
						qRFwk();

					Row = Ids.First();

					IdsTag.Append( "[ " );
					EventsTag.Append( "[ ");

					while ( Row != qNIL ) {
						Append_( Ids( Row ), IdsTag );
						Append_( Abstracts( Row ).Event, EventsTag );

						Row = Ids.Next( Row );

						if ( Row != qNIL ) {
							IdsTag.Append( ", " );
							EventsTag.Append( ", " );
						}
					}

					IdsTag.Append( " ]" );
					EventsTag.Append( " ]");
				}
			}

			bso::sBool HandleEvents_(
				const xdhcmn::digest_ &Descriptions,
				xdhcuc::cSingle &Callback )
			{
				bso::sBool Success = true;
			qRH
				str::strings Ids;
				xdhutl::event_abstracts Abstracts;
				str::string IdsTag, EventsTag;
				str::wStrings Arguments;
			qRB
				Ids.Init();
				Abstracts.Init();
				xdhutl::FillEventAbstracts( Descriptions, Ids, Abstracts );

				if ( Ids.Amount() != 0 ) {
					IdsTag.Init();
					EventsTag.Init();
					HandleEvents_( Ids, Abstracts, IdsTag, EventsTag );

					Arguments.Init();
					Arguments.Append(IdsTag);
					Arguments.Append(EventsTag);

					Success = BaseProcess_(ss_::SetEventHandlers, Arguments, Callback);
				}
			qRR
			qRT
			qRE
				return Success;
			}
		}

		namespace {
			namespace {
				void HandleWidgets_(
					const str::strings_ &Ids,
					const str::strings_ &Types,
					const str::strings_ &ParametersSets,
					const str::dStrings &ContentRetrievingMethods,
					const str::dStrings &FocusingMethods,
					const str::dStrings &SelectionMethods,
					str::string_ &FlatIds,
					str::string_ &FlatTypes,
					str::string_ &FlatParametersSets,
					str::dString &FlatContentRetrievingMethods,
					str::dString &FlatFocusingMethods,
					str::dString &FlatSelectionMethods )
				{
					sdr::row__ Row = qNIL;

					if ( Ids.Amount() != Types.Amount() )
						qRFwk();

					if ( Ids.Amount() != ParametersSets.Amount() )
						qRFwk();

					if ( Ids.Amount() != ContentRetrievingMethods.Amount() )
						qRFwk();

					if ( Ids.Amount() != FocusingMethods.Amount() )
						qRFwk();

					if ( Ids.Amount() != SelectionMethods.Amount() )
						qRFwk();

					Row = Ids.First();

					FlatIds.Append("[ ");
					FlatTypes.Append("[ ");
					FlatParametersSets.Append("[ ");
					FlatContentRetrievingMethods.Append("[ ");
					FlatFocusingMethods.Append("[ ");
					FlatSelectionMethods.Append("[ ");

					while ( Row != qNIL ) {
						Append_( Ids( Row ), FlatIds );
						Append_( Types( Row ), FlatTypes );
						Append_( ParametersSets( Row ), FlatParametersSets );
						Append_( ContentRetrievingMethods( Row ), FlatContentRetrievingMethods );
						Append_( FocusingMethods( Row ), FlatFocusingMethods );
						Append_( SelectionMethods( Row ), FlatSelectionMethods );

						Row = Ids.Next( Row );

						if ( Row != qNIL ) {
							FlatIds.Append( ", " );
							FlatTypes.Append( ", " );
							FlatParametersSets.Append( ", " );
							FlatContentRetrievingMethods.Append(", ");
							FlatFocusingMethods.Append(", ");
							FlatSelectionMethods.Append(", ");
						}
					}

					FlatIds.Append( " ]" );
					FlatTypes.Append( " ]");
					FlatParametersSets.Append( " ]");
					FlatContentRetrievingMethods.Append(" ]");
					FlatFocusingMethods.Append(" ]");
					FlatSelectionMethods.Append(" ]");
				}
			}

			bso::sBool HandleWidgets_(
				const xdhcmn::digest_ &Digest,
				xdhcuc::cSingle &Callback )
			{
				bso::sBool Success = true;
			qRH
				str::wStrings Ids, Types, ParametersSets, ContentRetrievingMethods, FocusingMethods, SelectionMethods, TagValues;
				str::wString FlatIds, FlatTypes, FlatParametersSets, FlatContentRetrievingMethods, FlatFocusingMethods, FlatSelectionMethods;
			qRB
				tol::Init(Ids, Types, ParametersSets, ContentRetrievingMethods, FocusingMethods, SelectionMethods);
				xdhutl::ExtractWidgetsFeaturesAndIds( Digest, Ids, Types, ParametersSets, ContentRetrievingMethods, FocusingMethods, SelectionMethods );

				if ( Ids.Amount() != 0 ) {
					tol::Init(FlatIds, FlatTypes, FlatParametersSets, FlatContentRetrievingMethods, FlatFocusingMethods, FlatSelectionMethods);
					HandleWidgets_( Ids, Types, ParametersSets, ContentRetrievingMethods, FocusingMethods, SelectionMethods, FlatIds, FlatTypes, FlatParametersSets, FlatContentRetrievingMethods, FlatFocusingMethods, FlatSelectionMethods );

					TagValues.Init();
					TagValues.AppendMulti(FlatIds, FlatTypes, FlatParametersSets, FlatContentRetrievingMethods, FlatFocusingMethods, FlatSelectionMethods);

					Success = BaseProcess_(ss_::InstantiateWidgets, TagValues, Callback);
				}
			qRR
			qRT
			qRE
				return Success;
			}
		}

		bso::sBool HandleLayout_(
			const char *ScriptName,
			const str::dStrings &Values,
			xdhcuc::cSingle &Callback,
			tht::rBlocker &Blocker,
			bso::sBool *SuccessPointer)
		{
			bso::sBool Success = false;
		qRH
			xdhcmn::digest Digests, EventsDigest, WidgetsDigest;
			xdhcmn::retriever__ Retriever;
		qRB
			Digests.Init();

			if ( ( Success = ExecuteAndGetDigest_(ScriptName, Values, Callback, Digests, Blocker, SuccessPointer) ) ) {
				Retriever.Init(Digests);

				tol::Init(EventsDigest, WidgetsDigest);

				Retriever.GetTable(EventsDigest);
				Retriever.GetTable(WidgetsDigest);

				if ( ( Success = HandleEvents_(EventsDigest, Callback) ) ) {
          Success = HandleWidgets_(WidgetsDigest, Callback);
				}
			}
		qRR
		qRE
		qRT
			return Success;
		}

		namespace {
			struct sShared_ {
				const char *ScriptName;
				const str::dStrings *Values;
				bso::sBool Success;
				xdhcuc::cSingle *Callback;
			};
		}

		void HandleLayoutRoutine_(
			void *UP,
			mtk::gBlocker &Blocker )
		{
				sShared_ &Shared = *(sShared_ *)UP;

				if ( !HandleLayout_(Shared.ScriptName, *Shared.Values, *Shared.Callback, Blocker.Blocker(), &Shared.Success ) )
					qRGnr();  // NOTA: failure handled by launching thread through 'Shared.Success'.
		}

		bso::sBool HandleLayout_(
			const char *ScriptName,
			const str::dStrings &Values,
			xdhcuc::cSingle &Callback,
			str::dString *ReturnValue)
		{
			sShared_ Shared;

			if ( ScriptName !=psn_::HandleLayout )
        qRFwk();

			Shared.ScriptName = ScriptName;
			Shared.Values = &Values;
			Shared.Success = false;
			Shared.Callback = &Callback;

			if ( true )
				mtk::Launch(HandleLayoutRoutine_, &Shared);	// Was conflicting with broadcasting.
			else
				mtk::SyncLaunch(HandleLayoutRoutine_, &Shared);	// In this case, with the 'arora' browser, dealing with XSL will blocks all the clients.

			/*
			The 'HandleLayout' primitive awaits a value from client, which is NOT sent to the backend. But an empty return value is sent to
			the backend to avoid launching a command which returned value replaces the one awaited by the 'HandleLayout' primitive.
			The settings of event handler and widget handler is done asynchronously and should be achieved before next command.
			*/

			if ( ReturnValue != NULL )  // If == NULL, old deprecated behavior : no (dummy) value sent to backend.
        *ReturnValue = str::Empty; // To synchronize calling functions.

			return Shared.Success;
		}

	}

	namespace {
		inline bso::sBool IsEqual_(
			const char *Op1,
			const char *Op2 )
		{
			return !strcmp(Op1, Op2);
		}

		inline bso::sBool IsEqual_(
			const str::dString &Op1,
			const char *Op2 )
		{
			return Op1 == Op2;
		}
	}

	template <typename type> bso::sBool Process_(
		const type &ScriptName,
		const str::dStrings &Values,
		xdhcuc::cSingle &Callback,
		str::dString *ReturnValue)
	{
		if ( IsEqual_(ScriptName, psn_::HandleLayout) )
			return HandleLayout_(psn_::HandleLayout, Values, Callback, ReturnValue);
		else
			return BaseProcess_(ScriptName, Values, Callback, NULL, NULL, ReturnValue);
	}

	template <typename ts, typename tl> bso::sBool Process_(
		const ts &TaggedScript,
		const tl &MergedTagList,
		const str::dStrings &TagValues,
		xdhcuc::cSingle &Callback,
		str::dString *ReturnValue = NULL)
	{
		bso::sBool Success = false;
	qRH
		str::wString Script;
		str::wStrings TagList;
	qRB
		TagList.Init();
		Split_(str::wString(MergedTagList), TagList);

		Script.Init();
		SubstituteTags_(str::wString(TaggedScript), TagList, TagValues, Script);

		Success = Callback.Process(Script, NULL, NULL, ReturnValue);
	qRR
	qRT
	qRE
		return Success;
	}
}

bso::sBool xdhdws::sProxy::Process(
	const char *ScriptName,
	const str::dStrings &TagValues,
	str::dString *ReturnValue )
{
	return Process_(ScriptName, TagValues, C_(), ReturnValue);
}

bso::sBool xdhdws::sProxy::Process(
	const str::dString &ScriptName,
	const str::dStrings &TagValues,
	str::dString *ReturnValue )
{
	return Process_(ScriptName, TagValues, C_(), ReturnValue);
}

bso::sBool xdhdws::sProxy::Process(
	const char *TaggedScript,
	const char *TagList,
	const str::dStrings &TagValues,
	str::dString *ReturnValue )
{
	return Process_(TaggedScript, TagList, TagValues, C_(), ReturnValue);
}

bso::sBool xdhdws::sProxy::Process(
	const str::dString &TaggedScript,
	const char *TagList,
	const str::dStrings &TagValues,
	str::dString *ReturnValue )
{
	return Process_(TaggedScript, TagList, TagValues, C_(), ReturnValue);
}

namespace {
	namespace {
		template <typename action, typename id> const str::dString &GetBroadcastActionScript_(
			const action &Action,
			const id &Id,
			str::dString &Script)
		{
		qRH
			str::wStrings Arguments;
		qRB
			Arguments.Init(Action);
			Arguments.Append(Id);

			GetScript_(ss_::BroadcastAction, Arguments, Script);
		qRR
		qRT
		qRE
			return Script;
		}
	}

	template <typename action, typename id> void BroadcastAction_(
		xdhcuc::cGlobal &Callback,
		const action &Action,
		const id &Id,
		faas::sRow FaasRow)
	{
	qRH
		str::wString Script;
	qRB
		Script.Init();
		GetBroadcastActionScript_(Action, Id, Script);

		Callback.Broadcast(Script, FaasRow);
	qRR
	qRT
	qRE
	}
}

void xdhdws::BroadcastAction(
	xdhcuc::cGlobal &Callback,
	const char *Action,
	const char *Id,
	faas::sRow FaasRow)
{
	BroadcastAction_(Callback, Action, Id, FaasRow);
}

void xdhdws::BroadcastAction(
	xdhcuc::cGlobal &Callback,
	const str::dString &Action,
	const str::dString &Id,
	faas::sRow FaasRow)
{
	BroadcastAction_(Callback, Action, Id, FaasRow);
}
