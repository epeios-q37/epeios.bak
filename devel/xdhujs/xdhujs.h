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

#ifndef XDHUJS__INC
# define XDHUJS__INC

# define XDHUJS_NAME		"XDHUJS"

# if defined( E_DEBUG ) && !defined( XDHUJS_NODBG )
#  define XDHUJS_DBG
# endif

// X(SL)/DH(TML) Upstream Javascript Scripter.

# include "err.h"

# include "xdhcmn.h"

# include "ntvstr.h"
# include "ctn.h"
# include "str.h"

namespace xdhujs {
	using xdhcmn::nchar__;
	using xdhcmn::nstring___;

	E_ENUM(script_name) {
		snExecute,
		snLayoutPrepender,
		snLayoutSetter,
		snLayoutAppender,
		snAttributeGetter,
		snAttributeRemover,
		snAttributeSetter,
		snValueGetter,
		snValueSetter,
		snDialogAlert,
		snDialogConfirm,
		snFocuser,
		snSelector,
		snLog,
		snPropertyGetter,
		snPropertySetter,
		snWidgetValueRetriever,
		snWidgetFocuser,
		snWidgetSelector,
		snWidgetFetcher,
		snEventHandlersSetter,
		snContentsSetter,
		snTimeoutSetter,
		snParent,
		snFirstChild,
		snLastChild,
		snPreviousSibling,
		snNextSibling,
		snCreateElement,
		snInsertChild,
		snAppendChild,
		snInsertBefore,
		snInsertAfter,
		snWidgetsInstantiator,
		snCSSRuleInserter,
		snCSSRuleAppender,
		snCSSRuleRemover,
		snClassesAdder,
		snClassesRemover,
		snClassesToggler,
		snElementsEnabler,
		snElementsDisabler,
		snDummy,
		sn_amount,
		sn_Undefined,
	};

	const str::string_ &GetTaggedScript(
		script_name__ ScriptName,
		str::string_ &Script );

	void GetScript(
		script_name__ ScriptName,
		str::string_ &Script,
		va_list List );

	const str::string_ &GetScript(
		script_name__ ScriptName,
		str::string_ *Script,	// Was '&Script', but should not work due 'va_start(...)' restrictions concerning references (but it worked under MSVC).
		... );
	/*
	inline void DialogAlert(
		const nstring___ &XML,
		const nstring___ &XSL,
		const nstring___ &Title,
		const nstring___ &CloseText,
		str::string_ &Script )
	{
		GetScript( snDialogAlert, &Script, XML.Internal()(), XSL.Internal()(), Title.Internal()(), CloseText.Internal()() );
	}

	inline void RemoveAttribute(
		const nstring___ &Id,
		const nstring___ &Name,
		str::string_ &Script )
	{
		GetScript( snAttributeRemover, &Script, Id.Internal()(), Name.Internal()() );
	}

	inline void GetAttribute(
		const nstring___ &Id,
		const nstring___ &Name,
		str::string_ &Script )
	{
		GetScript( snAttributeGetter, &Script, Id.Internal()(), Name.Internal()() );
	}

	inline void SetAttribute(
		const nstring___ &Id,
		const nstring___ &Name,
		const nstring___ &Value,
		str::string_ &Script )
	{
		GetScript( snAttributeSetter, &Script, Id.Internal()(), Name.Internal()(), Value.Internal()() );
	}

	inline void SetValue(
		const nstring___ &Id,
		const nstring___ &Value,
		str::string_ &Script )
	{
		GetScript( snValueSetter, &Script, Id.Internal()(), Value.Internal()() );
	}

	inline void GetProperty(
		const nstring___ &Id,
		const nstring___ &Name,
		str::string_ &Script )
	{
		GetScript( snPropertyGetter, &Script, Id.Internal()(), Name.Internal()() );
	}
	*/

}

#endif
