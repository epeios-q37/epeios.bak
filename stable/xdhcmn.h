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

#ifndef XDHCMN_INC_
# define XDHCMN_INC_

# define XDHCMN_NAME		"XDHCMN"

# if defined( E_DEBUG ) && !defined( XDHCMN_NODBG )
#  define XDHCMN_DBG
# endif

// X(SL)/DH(TML) CoMmoN

# include "err.h"
# include "flw.h"
# include "strmrg.h"
# include "sclm.h"

# include <stdarg.h>

namespace xdhcmn {
	// Declarations used in FaaS context.
	// Enclosed in a namespace to ease its inclusion.
	namespace faas {
		qROW(Row); // Token row.

		typedef bso::sS16 sId;	// For the multiplexing between the FaaS proxy and the backend.
		qCDEF( sId, MaxId, bso::S8Max );
		qCDEF( sId, MinId, 0 );	// Values this value are action codes.
		qCDEF( sId, UndefinedId, -1 );

		namespace upstream {
			qCDEF( sId, CreationId, -2 );
			qCDEF( sId, ClosingId, -3 );
			qCDEF( sId, HeadRetrievingId, -4 ); // Since FaaS protocol v1.
		}

		namespace downstream {
			qCDEF( sId, BroadcastScriptId, -2 );  // Probably obsolete!
			qCDEF( sId, BroadcastActionId, -3 );  // Even if above removed, DON'T CHANGE VALUE!!!
			qCDEF( sId, HeadSendingId, -4 ); // Since FaaS protocol v1.
		}
	}

  typedef bso::sU8 sScriptsVersion;

  extern const char SpecialScriptNameMarker;

  // Script name with no correspondence in 'XDHScripts.xcfg'.
  // Is intercepted to do a special action.
  extern const char *ScriptNameForStandBy;  // Awaiting new action.
  extern const char *ScriptNameForDismiss;  // Cease handling of this session.
  extern const char *ScriptNameForInform;  // Sends a report to proxy.

	qMIMICs( bso::sU16, sIndex );	// For the 'CSS' rules.

	static E_CDEF( char *, CloseActionLabel, "Q37Close" );
	static E_CDEF( char *, RefreshActionLabel, "Q37Refresh" );

	const str::dString &Escape(
		const str::string_ &Source,
		str::string_ &Target,
		bso::char__ Delimiter,	// Should be '\'', '"' or 0. If 0, escapes '\'' and '\"', otherwise escapes 'Delimiter'.
		bso::char__ EscapeChar = strmrg::DefaultEscapeToken );

	const str::dString &Escape(
		str::string_ &String,
		bso::char__ Delimiter,	// Should be '\'', '"' or 0. If 0, escapes '\'' and '\"', otherwise escapes 'Delimiter'.
		bso::char__ EscapeChar = strmrg::DefaultEscapeToken );

#if 0
	void Unescape(
		const str::string_ &Source,
		str::string_ &Target,
		bso::char__ EscapeChar = DefaultEscapeChar );
#endif

	typedef strmrg::table_ digest_;
	E_AUTO( digest );

	using strmrg::Split;
	using strmrg::Merge;

	using strmrg::retriever__;
	using strmrg::rRetriever;

	// Merge 'Splitted', delimited by '"', and separated by ','.
	void FlatMerge(
		const str::dStrings &Splitted,
		str::dString &Merged,
		bso::sBool AsJSArray );	// If 'true', the splitted items are enclosed between '[]', so the result can be used as JS array; above function does then NOT work.

	// Reverse of above (if 'AsJSArray' was to 'false').
	void FlatSplit(
		flw::rRFlow &Flow,
		str::dStrings &Splitted );

	// Variant of above.
	void FlatSplit(
		const str::dString &Merged,
		str::dStrings &Splitted );
}

/************/
/**** NEW ****/
/************/

namespace xdhcmn {
	typedef digest_ dDigest;
	qW( Digest )
}

#endif
