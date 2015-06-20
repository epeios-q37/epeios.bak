/*
	Copyright (C) 2000-2015 Claude SIMON (http://q37.info/contact/).

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

//	$Id: xmlelm.h,v 1.2 2013/07/25 15:59:16 csimon Exp $

#ifndef XMLELM__INC
#define XMLELM__INC

#define XMLELM_NAME		"XMLELM"

#define	XMLELM_VERSION	"$Revision: 1.2 $"

#define XMLELM_OWNER		"Claude SIMON"

#if defined( E_DEBUG ) && !defined( XMLELM_NODBG )
#define XMLELM_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.2 $
//C Claude SIMON (csimon at zeusw dot org)
//R $Date: 2013/07/25 15:59:16 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D XML ELeMent 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"
#include "xmlbsc.h"
#include "str.h"
#include "xtf.h"
#include "bch.h"

namespace xmlelm {

	using namespace xmlbsc;

	enum type__ {
		tTag,
		tAttribute,
		tOrphan,
//		tRoot,
		t_amount,
		t_Undefined,
	};

	typedef item_	name_;
	typedef item		name;

	qROW( erow__ );

	typedef basic_<type__, erow__> element_tree_;
	E_AUTO( element_tree );

/*

	class element_tree_
	: public _basic_,
	  public names_
	{
	public:
		struct s
		: public _basic_::s,
		  public names_::s
		{};
		element_tree_( s &S )
		: _basic_( S ),
		  names( S )
		{}
		void reset( bso::bool__ P = true )
		{
			_basic_::reset( P );
			names_::reset( P );
		}
		void plug( mmm::E_MULTIMEMORY_ &MM )
		{
			_basic_::plug( MM );
			names_::plug( MM );
		}
		element_tree_ &operator=( const element_tree_ &ET )
		{
			_basic_::operator =( ET );
			names_::operator =( ET );

			return *this;
		}
		void Init( void )
		{
			_basic_::Init();
			names_::Init();
		}
	};

	E_AUTO( element_tree )
*/
/*
	//f Write 'Tags' in XML format to 'Flow' according 'Mode'.
	void WriteXML(
		const element_tree_ &ElementTree,
		txf::text_oflow__ &Flow );
*/

	erow__ Search(
		const name_ &Name,
		type__ Type,
		const element_tree_ &ElementTree,
		erow__ Parent );


	//t Correspondances between 2 tag tables.
	typedef bch::E_BUNCHt_( erow__, erow__ ) element_map_;
	E_AUTO( element_map )

	/*f Merge 'Source' into 'Dest'. Put a table of correspondances
	between 'source' and 'Dest' into 'TagMap'. */
	void Merge(
		const element_tree_ &Source,
		erow__ SRoot,
		element_tree_ &Target,
		erow__ TRoot,
		element_map_ &ElementMap );
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
