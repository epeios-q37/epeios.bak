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

#define XDHUPS_COMPILATION_

#include "xdhups.h"

#include "xdhutl.h"

#include "mtk.h"

using namespace xdhups;

extern "C" typedef xdhcdc::retrieve retrieve;

namespace {
  // In scripts definitions, tags beginning with this character are NOT normalized, but put as is!
  qCDEF(char, RawTagValueMarker_, '_'); // Tag with name
}

// Special primitives, for internal use,
// which are not directly launched by the user.
namespace sp_ {	// Special Script
	qCDEF(char *, SetEventHandlers, "@SetEventHandlers_1");
	qCDEF(char *, InstantiateWidgets, "@InstantiateWidgets_1" );
	qCDEF(char *, BroadcastAction, "@BroadcastAction_1");
}

// Name of primitives, called by the user, which are 'patched',
// i.e. some special additional actions are made.
namespace pp_ {	// Patched primitive
	qCDEF(char *, HandleLayout, "HandleLayout_1");
}

namespace {
  rgstry::rEntry Scripts_("Scripts", sclr::Definitions);
}

namespace {
  rgstry::rEntry ScriptsVersion_("@Version", Scripts_);
}

xdhcmn::sScriptsVersion xdhups::GetScriptsVersion(void)
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

						if ( Name(Name.First()) == RawTagValueMarker_ )
							AppendRawTag_(Name, Value, Names, Values );
						else
							AppendEscapedTag_(Name, Value, Names, Values );
					}
				}

				bso::sBool SubstituteTags_(
					const str::string_ &TaggedScript,	// Script with tags. When returning, tags are substitued.
					const str::dStrings &TagList,
					const str::dStrings &RawTagValues,
					str::dString &Script )
				{
				  bso::sBool Success = false;
				qRH
					str::wStrings Tags, TagValues;
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

					Success = tagsbs::SubstituteLongTags(TaggedScript, Tags, TagValues, Script);
				qRR
				qRT
				qRE
          return Success;
				}
			}

			template <typename type> bso::sBool GetScript_(
				const type &Name,
				const str::dStrings &TagValues,
				str::string_ &Script)
			{
			  bso::sBool Success = false;
			qRH
				str::string TaggedScript;
				str::wStrings TagList;
			qRB
				TaggedScript.Init();
				GetTaggedScript_( Name, TaggedScript );

				TagList.Init();
				GetScriptTags_(Name, TagList);

				Success = SubstituteTags_(TaggedScript, TagList, TagValues, Script);
			qRR
			qRT
			qRE
        return Success;
			}
		}

		template <typename type> bso::sBool BaseProcess_(
			const type &Primitive,
			const str::dStrings &TagValues,
			cEngine &Engine,
			tht::rBlocker *Blocker = NULL,
			bso::sBool *SuccessPointer = NULL,
			str::dString *ReturnValue = NULL)
		{
			bso::sBool Success = false;
		qRH
			str::wString Script;
		qRB
			Script.Init();

			if ( GetScript_(Primitive, TagValues, Script) )
        Success = Engine.Process(Script, Blocker, SuccessPointer, ReturnValue );
		qRR
		qRT
		qRE
			return Success;
		}
	}

	namespace {
		namespace {
			bso::sBool ExecuteAndGetDigest_(
				const char *Primitive,
				const str::dStrings &TagValues,
				cEngine &Engine,
				xdhcmn::digest_ &Digest,
				tht::rBlocker &Blocker,
				bso::sBool *SuccessPointer)
			{
				bso::sBool Success = false;
			qRH
				str::wString RawDigest;
			qRB
				RawDigest.Init();

				if ( ( Success = BaseProcess_(Primitive, TagValues, Engine, &Blocker, SuccessPointer, &RawDigest) ) ) {
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
				cEngine &Engine )
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

					Success = BaseProcess_(sp_::SetEventHandlers, Arguments, Engine);
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
				cEngine &Engine )
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

					Success = BaseProcess_(sp_::InstantiateWidgets, TagValues, Engine);
				}
			qRR
			qRT
			qRE
				return Success;
			}
		}

		bso::sBool HandleLayout_(
			const str::dStrings &TagValues,
			cEngine &Engine,
			tht::rLocker &CallbackLocker, // Ensure that above 'Callback' is not prematurely destroyed.
			tht::rBlocker &ReturnValueBlocker, // Ensure that the potential return value is read before another primitive is launched.
			bso::sBool *SuccessPointer)
		{
			bso::sBool Success = false;
		qRH
			xdhcmn::digest Digests, EventsDigest, WidgetsDigest;
			xdhcmn::retriever__ Retriever;
			tht::rLockerHandler Lock;
		qRB
      Lock.Init(CallbackLocker);  // Also locks. Unlocks automatically when destructed.

			Digests.Init();

			if ( ( Success = ExecuteAndGetDigest_(pp_::HandleLayout, TagValues, Engine, Digests, ReturnValueBlocker, SuccessPointer) ) ) {
				Retriever.Init(Digests);

				tol::Init(EventsDigest, WidgetsDigest);

				Retriever.GetTable(EventsDigest);
				Retriever.GetTable(WidgetsDigest);

				if ( ( Success = HandleEvents_(EventsDigest, Engine) ) ) {
          Success = HandleWidgets_(WidgetsDigest, Engine);
				}
			}
		qRR
		qRE
		qRT
			return Success;
		}

		namespace {
			struct sShared_ {
				const str::dStrings *TagValues;
				bso::sBool Success;
				cEngine *Engine;
				tht::rLocker *EngineLocker;
			};
		}

		void HandleLayoutRoutine_(
			void *UP,
			mtk::gBlocker &Blocker )
		{
				sShared_ &Shared = *(sShared_ *)UP;

				if ( !HandleLayout_(*Shared.TagValues, *Shared.Engine, *Shared.EngineLocker, Blocker.Blocker(), &Shared.Success ) )
					qRGnr();  // NOTA: failure handled by launching thread through 'Shared.Success'.
		}

		bso::sBool HandleLayout_(
			const str::dStrings &TagValues,
			cEngine &Engine,
			tht::rLocker &EngineLocker,
			str::dString *ReturnValue)
		{
			sShared_ Shared;

			Shared.TagValues = &TagValues;
			Shared.Success = false;
			Shared.Engine = &Engine;
			Shared.EngineLocker = &EngineLocker;

			if ( true )
				mtk::Launch(HandleLayoutRoutine_, &Shared);	// Was conflicting with broadcasting.
			else
				mtk::SyncLaunch(HandleLayoutRoutine_, &Shared);	// In this case, with the 'arora' browser, dealing with XSL will blocks all the clients.

			/*
			The 'HandleLayout' primitive awaits a value from client, which is NOT sent to the backend. But an empty return value is sent to
			the backend to avoid launching a command which returned value replaces the one awaited by the 'HandleLayout' primitive.
			The settings of event handler and widget handler is done asynchronously and should be achieved before next command.
			*/


// Useless, as, if 'ReturnValue' != NULL, '*ReturnValue' is already an empty string.
#if 0
			if ( ReturnValue != NULL )  // If == NULL, old deprecated behavior : no (dummy) value sent to backend.
        *ReturnValue = str::Empty; // To synchronize calling functions.
#endif

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
		const type &Primitive,
		const str::dStrings &TagValues,
		cEngine &Engine,
    tht::rLocker &EngineLocker,
    str::dString *ReturnValue)
	{
		if ( IsEqual_(Primitive, pp_::HandleLayout) )
			return HandleLayout_(TagValues, Engine, EngineLocker, ReturnValue);
		else
			return BaseProcess_(Primitive, TagValues, Engine, NULL, NULL, ReturnValue);
	}
}

bso::bool__ xdhups::sEventSession::Handle(
  const char *EventDigest,
  const str::dString &UserId)
{
	bso::sBool Cont = true;
qRH;
	str::wString Id, Action;
qRB;
	tol::Init(Id, Action);

	if ( ( EventDigest == NULL ) || ( !EventDigest[0] ) )
		Cont = C_().Handle(UserId, str::Empty);
	else if ( xdhutl::Extract(str::wString(EventDigest), Id, Action) )
		Cont = C_().Handle(Id, Action);
qRR;
qRT;
qRE;
	return Cont;
}

bso::sBool xdhups::rPrimitiveSession::Execute(
  const str::dString &Primitive,
  const str::dStrings &TagValues,
  str::dString *ReturnedValue)
{
  return Process_(Primitive, TagValues, C_(), Locker_, ReturnedValue);
}

namespace {
  template <typename action, typename id> void GetBroadcastPrimitive_(
    const action &Action,
    const id &Id,
    str::dString &Primitive,
    str::dStrings &TagValues)
  {
    TagValues.Append(Action);
    TagValues.Append(Id);

    Primitive = sp_::BroadcastAction;
  }
}

void xdhups::GetBroadcastPrimitive(
  const str::dString &Action,
  const str::dString &Id,
  str::dString &Primitive,
  str::dStrings &TagValues)
{
  GetBroadcastPrimitive_(Action, Id, Primitive, TagValues);
}

bso::bool__ xdhups::rAgent::Init(
	xdhcuc::cGlobal &Upstream,
	xdhcdc::eMode Mode,
	const str::string_ &ModuleFileName,
	dlbrry::eNormalization Normalization,
	const char *Identification)
{
	bso::bool__ Success = false;
qRH
	xdhcdc::sData Data;
	fnm::name___ Location;
	TOL_CBUFFER___ Buffer;
qRB
	Location.Init();
	Data.Init(Mode, Identification, fnm::GetLocation(ModuleFileName, Location).UTF8(Buffer));

	Library_.Init( ModuleFileName, Normalization );

	retrieve *Retrieve = dlbrry::GetFunction<retrieve *>( E_STRING( XDHCDC_RETRIEVE_FUNCTION_NAME ), Library_ );

	if ( Retrieve != NULL ) {
		Callback_ = Retrieve();

		if ( Callback_ == NULL )
			qRFwk();

		if ( GetScriptsVersion() < Callback_->Initialize(Data, Upstream) )
      qRFwk();

    Success = true;
	}
qRR
qRT
qRE
	return Success;
}

bso::sBool xdhups::rAgent::IsValid(const str::dString &Token)
{
	bso::sBool ReturnValue = false;
qRH
	str::wString DummmyHead;
qRB
	DummmyHead.Init();

	ReturnValue = GetHead(Token, DummmyHead);
qRR
qRT
qRE
	return ReturnValue;
}

