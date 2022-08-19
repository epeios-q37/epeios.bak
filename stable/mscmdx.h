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

//D MuSiC MiDi X(ML) features

#ifndef MSCMDX_INC_
# define MSCMDX_INC_

# define MSCMDX_NAME		"MSCMDX"

# if defined( E_DEBUG ) && !defined( MSCMDX_NODBG )
#  define MSCMDX_DBG
# endif

# include "err.h"
# include "flw.h"

# include "mscmdm.h"
# include "mscmdf.h"
# include "xml.h"

namespace mscmdx {
	using mscmdm::eExtraneous;

	bso::bool__ MIDIToXMID(
		flw::iflow__ &IFlow,
		eExtraneous Extraneous,
		xml::rWriter &Writer,
		err::handling__ ErrHandling = err::h_Default );

	xml::status__ XMIDToMIDI(
		xtf::extended_text_iflow__ &IFlow,
		eExtraneous Extraneous,
		flw::oflow__ &OFlow );

	enum status__ {
		sOK,
		sUnexpectedTag,
		sUnexpectedValue,
		sBadDataAttributeValue,
		sBadIdValue,
		sUnexpectedContext,
		sXML,
		s_amount,
		s_Undefined
	};

	status__ ParseEvent(
		xml::parser___ &Parser,
		mscmdm::dEvent &Event );

	status__ ParseEvents(
		xml::parser___ &Parser,
		mscmdm::dEvents &Events );

}

/*$END$*/
#endif
