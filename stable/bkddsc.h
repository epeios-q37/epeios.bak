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

//	$Id: bkddsc.h,v 1.2 2012/11/14 16:06:25 csimon Exp $

#ifndef BKDDSC__INC
#define BKDDSC__INC

#define BKDDSC_NAME		"BKDDSC"

#define	BKDDSC_VERSION	"$Revision: 1.2 $"

#define BKDDSC_OWNER		"Claude SIMON (http://zeusw.org/intl/contact.html)"

#if defined( E_DEBUG ) && !defined( BKDDSC_NODBG )
#define BKDDSC_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.2 $
//C Claude SIMON (http://zeusw.org/intl/contact.html)
//R $Date: 2012/11/14 16:06:25 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D BacKenD DeSCriptions 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"
#include "bkdcst.h"
#include "bkdtpm.h"
#include "ctn.h"

namespace bkddsc {

	namespace {
		using namespace bkdcst;
		using namespace ctn;
	}

	using namespace bkdtpm;

	typedef id8__ cast__;
	typedef ids8_ casts_;
	typedef ids8 casts;
	using bkdcst::cast;

	//c The description of a request.
	class description_
	{
	public:
		//o Name of the command.
		str::string_ Name;
		/*o Parameters and return value types. Separated by 'cEnd'. The 'cEnd'
		to signalize the end of the request must NOT be put. */
		ids8_ Casts;
		struct s
		{
			str::string_::s Name;
			ids8_::s Casts;
		} &S_;
		description_( s &S )
		: S_( S ),
		  Name( S.Name ),
		  Casts( S.Casts )
		{}
		void reset( bool P = true )
		{
			Name.reset( P );
			Casts.reset( P );
		}
		description_ &operator =( const description_ &D )
		{
			Name = D.Name;
			Casts = D.Casts;

			return *this;
		}
		void plug( mmm::multimemory_ &M )
		{
			Name.plug( M );
			Casts.plug( M );
		}
		//f Initialization. The name ans casts are added one per one.
		void Init( void )
		{
			Name.Init();
			Casts.Init();
		}
		/*f Initializing with name 'Name' and a the list of casts 'Casts'. The list
		must contain 2 'cEnd', the first at the end of the parameters casts,
		and 1 of the end of returned values casts. */
		void Init(
			const char *Name,
			const cast *Casts );
		//f Initialization with name 'Name'. The casts would be given using 'New()'.
		void Init( const char *Name )
		{
			this->Name.Init();
			Casts.Init();
			
			this->Name = Name;
		}
		//f Append 'Cast'.
		void Append( bkdcst::cast Cast )
		{
			Casts.Append( (cast__)Cast );
		}
	};

	E_AUTO( description )

	//c Requests descriptions.
	class descriptions_
	: public E_XCONTAINER_( description_ )
	{
	public:
		struct s
		: public E_XCONTAINER_( description_ )::s
		{};
		descriptions_( s &S )
		: ctn::E_XCONTAINER_( description_ )( S )
		{}
		void reset( bool P = true )
		{
			E_XCONTAINER_( description_ )::reset( P );
		}
		void plug( mmm::multimemory_ &M )
		{
			E_XCONTAINER_( description_ )::plug( M );
		}
		descriptions_ &operator =( const descriptions_ &D )
		{
			E_XCONTAINER_( description_ )::operator =( D );

			return *this;
		}
		//f Initialization.
		void Init( void )
		{
			E_XCONTAINER_( description_ )::Init();
		}
		//f Adding 'Description'. Return the position where added.
		epeios::row__ Add( const description_ &Description )
		{
			return E_XCONTAINER_( description_ )::Append( Description );
		}
		/*f See 'descriptions_::Add()' for the descriptions of the parameters.
		return the position where request description added. */
		epeios::row__ Add(
			const char *Name,
			const cast *Casts )
		{
			description Description;

			Description.Init( Name, Casts );
			
			return Add( Description );
		}
		/*f Return the position of the description 'Description'. */
		epeios::row__ Position( const description_ &Description ) const;
	};

	E_AUTO( descriptions )
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
