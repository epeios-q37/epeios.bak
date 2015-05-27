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

//	$Id: xmltag.h,v 1.5 2012/11/14 16:06:40 csimon Exp $

#ifndef XMLTAG__INC
#define XMLTAG__INC

#define XMLTAG_NAME		"XMLTAG"

#define	XMLTAG_VERSION	"$Revision: 1.5 $"	

#define XMLTAG_OWNER		"the Epeios project (http://epeios.org/)"

#if defined( E_DEBUG ) && !defined( XMLTAG_NODBG )
#define XMLTAG_DBG 
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.5 $
//C Claude L. SIMON (csimon@webmails.com)
//R $Date: 2012/11/14 16:06:40 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

/* Addendum to the automatic documentation generation part. */
//D eXtended Markup Langage TAG.
/* End addendum to automatic documentation generation part. */

#error "Obsolete. Use library 'HOSTAG' instead."

#include "err.h"
#include "flw.h"
#include "xmlbsc.h"
#include "str.h"
#include "xtf.h"

namespace xmltag {
	//e The type of a name.
	enum type {
		//i Unknow type.
		tUnknow,
		//i An attribute.
		tAttribute,
		//i A field
		tField,
		//i the root.
		tRoot,
		//i Amount of type.
		t_amount
	};
	
	//t Type of a type (sorry !).
	typedef bso::ubyte__ type__;

	//t A tag name.
	typedef str::string_	name_;
	typedef str::string		name;

	//c A tag.
	class tag_
	{
	public:
		name_ Name;
		struct s
		{
			name_::s Name;
			type__ Type;
		} &S_;
		tag_( s &S )
		: S_( S ),
		  Name( S.Name )
		{}
		void reset( bool P  = true )
		{
			Name.reset( P );
			S_.Type = tUnknow;
		}
		void plug( mmm::multimemory_ &M )
		{
			Name.plug( M );
		}
		void plug( mdr::E_MEMORY_DRIVER_ &MD )
		{
			Name.plug( MD );
		}
		tag_ &operator =( const tag_ &N )
		{
			Name = N.Name;
			S_.Type = N.S_.Type;

			return *this;
		}
		//f Initialization.
		void Init( void )
		{
			Name.Init();
			S_.Type = xmltag::tUnknow;
		}
		//f Initialization with name 'Name' and type 'Type'.
		void Init(
			const name_ &Name,
			type Type )
		{
			this->Name.Init();
			this->Name = Name;

			S_.Type = Type;
		}
		//f Returns its type.
		type Type( void ) const
		{
			return (xmltag::type)S_.Type;
		}
		//f 'Type' become its type.
		void Type( type Type )
		{
			S_.Type = Type;
		}
		//f Return true if an attribute, false otherwise.
		bso::bool__ IsAttribute( void ) const
		{
			return Type() == xmltag::tAttribute;
		}

	};

	AUTO( tag )

	//t A reference to a name.
	TYPEDEF( epeios::row_t__, tag_row__ );

	//t The basic manager witouht templates.
	typedef xmlbsc::basic_< tag_, tag_row__ > basic_;

	//c The tag manager
	class tags_
	: public basic_
	{
	public:
		struct s
		: public basic_::s
		{};
		tags_( s &S )
		: basic_( S )
		{}
		void reset( bool P = true )
		{
			basic_::reset( P );
		}
		void plug( mmm::multimemory_ &M )
		{
			basic_::plug( M );
		}
		tags_ &operator =( const tags_ &NM )
		{
			basic_::operator =( NM );

			return *this;
		}
		//f Initialization. Returns root's reference.
		tag_row__ Init( void );
		/*f Returns the position of tag named 'Name' which belongs
		to 'TagRow' and of type 'Type', or 'NONE' if non-existant. */
		tag_row__ Position(
			const name_ &Name,
			tag_row__ TagRow,
			type Type ) const;
	};

	AUTO( tags )

	//e How to handle tag with no child in XML format.
	enum mode {
		//i "<tag/>"
		mWithEmptyTag,
		//i "<tag></tag>".
		mNoEmptyTag,
		//i amount of mode.
		m_amount
	};
	
	//f Write 'Tags' in XML format to 'Flow' according 'Mode'.
	void WriteXML(
		const tags_ &Tags,
		txf::text_oflow___ &Flow,
		mode Mode = mWithEmptyTag );
		
	//t Correspondances between 2 tag tables.
	typedef bch::E_BUNCHt_( tag_row__, tag_row__ ) tag_map_;
	typedef bch::E_BUNCHt( tag_row__, tag_row__ ) tag_map;
		
	/*f Merge 'Source' into 'Dest'. Put a table of correspondances
	between 'source' and 'Dest' into 'TagMap'. */
	void Merge(
		const tags_ &Source,
		tags_ &Dest,
		tag_map_ &TagMap );
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
