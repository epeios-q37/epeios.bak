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

//	$Id: xmldoc.h,v 1.3 2013/07/25 15:59:16 csimon Exp $

#ifndef XMLDOC__INC
#define XMLDOC__INC

#define XMLDOC_NAME		"XMLDOC"

#define	XMLDOC_VERSION	"$Revision: 1.3 $"

#define XMLDOC_OWNER		"Claude SIMON"

#if defined( E_DEBUG ) && !defined( XMLDOC_NODBG )
#define XMLDOC_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.3 $
//C Claude SIMON (csimon at zeusw dot org)
//R $Date: 2013/07/25 15:59:16 $

/* End of automatic documentation generation part. */

/* Addendum to the automatic documentation generation part. */
//D XML DOCument 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"
#include "xmlelm.h"
#include "xmlval.h"

namespace xmldoc {

	class document_
	{
	public:
		struct s {
			xmlelm::element_tree_::s Structure;
			xmlval::value_tree_::s Data;
		};
		xmlelm::element_tree_ Structure;
		xmlval::value_tree_ Data;
		document_( s &S )
		: Structure( S.Structure ),
		  Data( S.Data )
		{}
		void reset( bso::bool__ P =true )
		{
			Structure.reset( P );
			Data.reset( P );
		}
		void plug( qAS_ &AS )
		{
			Structure.plug( AS );
			Data.plug( AS );
		}
		document_ &operator =( const document_ &D )
		{
			Structure = D.Structure;
			Data = D.Data;

			return *this;
		}
		void Init( void )
		{
			Structure.Init();
			Data.Init();
		}
		xmlval::vrow__ Cut( xmlval::vrow__ Node )
		{
			Data.Cut( Node );

			return Node;
		}
	};

	E_AUTO( document )

	void WriteXML(
		const document_ &Document,
		xmlval::vrow__ Root,
		txf::text_oflow__ &Flow );

}

/*$END$*/
#endif
