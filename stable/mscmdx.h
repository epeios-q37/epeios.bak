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

//	$Id: mscmdx.h,v 1.3 2012/09/07 16:18:22 csimon Exp $

#ifndef MSCMDX_INC_
#define MSCMDX_INC_

#define MSCMDX_NAME		"MSCMDX"

#define	MSCMDX_VERSION	"$Revision: 1.3 $"

#define MSCMDX_OWNER		"Claude SIMON"


#if defined( E_DEBUG ) && !defined( MSCMDX_NODBG )
#define MSCMDX_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.3 $
//C Claude SIMON (csimon at zeusw dot org)
//R $Date: 2012/09/07 16:18:22 $

/* End of automatic documentation generation part. */

/* Addendum to the automatic documentation generation part. */
//D MuSiC MiDi Xml features 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

# include "err.h"
# include "flw.h"

# include "mscmdm.h"
# include "mscmdf.h"
# include "xml.h"

namespace mscmdx {
	using mscmdm::extraneous__;

	bso::bool__ MIDIToXMID(
		flw::iflow__ &IFlow,
		extraneous__ Extraneous,
		xml::writer_ &Writer,
		err::handling__ ErrHandling = err::h_Default );

	xml::status__ XMIDToMIDI(
		xtf::extended_text_iflow__ &IFlow,
		extraneous__ Extraneous,
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
		mscmdm::event_ &Event );

	status__ ParseEvents(
		xml::parser___ &Parser,
		mscmdm::events_ &Events );

}

/*$END$*/
#endif
