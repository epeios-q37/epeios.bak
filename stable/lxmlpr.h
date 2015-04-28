/*
	Header for the 'lxmlpr' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Copyright (C) 2004 Claude SIMON (http://zeusw.org/intl/contact.html).

	This file is part of the Epeios (http://zeusw.org/epeios/) project.

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 3
	of the License, or (at your option) any later version.
 
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, go to http://www.fsf.org/
	or write to the:
  
         	         Free Software Foundation, Inc.,
           59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

//	$Id: lxmlpr.h,v 1.8 2012/11/14 16:06:30 csimon Exp $

#ifndef LXMLPR__INC
#define LXMLPR__INC

#define LXMLPR_NAME		"LXMLPR"

#define	LXMLPR_VERSION	"$Revision: 1.8 $"

#define LXMLPR_OWNER		"Claude SIMON (http://zeusw.org/intl/contact.html)"

#if defined( E_DEBUG ) && !defined( LXMLPR_NODBG )
#define LXMLPR_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.8 $
//C Claude SIMON (http://zeusw.org/intl/contact.html)
//R $Date: 2012/11/14 16:06:30 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D Light XML PaRser 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#error "Obsolete ! Use 'XML' library instead."

#include "err.h"
#include "flw.h"
#include "xtf.h"
#include "lstctn.h"
#include "txf.h"

namespace lxmlpr {
	struct callback__
	{
		virtual void LXMLPRTag(
			const str::string_ &Prefix,
			const str::string_ &Name ) = 0;
		virtual void LXMLPRValue( const str::string_ &Value ) = 0;
		virtual void LXMLPRAttribute(
			const str::string_ &Prefix,
			const str::string_ &Name,
			const str::string_ &Value ) = 0;
		virtual void LXMLPRTagClosed( void ) = 0;
	};

	bso::bool__ Parse(
		xtf::extended_text_iflow__ &Flow,
		callback__ &Callback,
		xtf::location__ &ErrorLine,
		xtf::location__ &ErrorColumn );

	void Transform( str::string_ &Target );	// Transformation des caractres spciaux, comm '<' qui devient '&lt;'.

	inline void Convert(
		const str::string_ &Source,
		str::string_ &Target )	// Conversion des caractres spciaux, comm '<' qui devient '&lt;'.
	{
		Target = Source;

		Transform( Target );
	}


	typedef str::string_	name_;
	typedef str::string		name;

	typedef str::string_	value_;
	typedef str::string		value;

	class writer_
	{
	private:
		void _CloseAllTags( void );
	public:
		struct s {
			stk::E_XMCSTACK_( name_ )::s Tags;
			txf::text_oflow__ *Flow;
			bso::bool__ TagNameInProgress;
		} &S_;
		stk::E_XMCSTACK_( name_ ) Tags;
		writer_( s &S )
		: S_( S ),
		  Tags( S.Tags )
		{}
		void reset( bso::bool__ P = true )
		{
			if ( P )
				_CloseAllTags();

			S_.TagNameInProgress = false;

			Tags.reset( P );
			S_.Flow = NULL;
		}
		void plug( mmm::E_MULTIMEMORY_ &MM )
		{
			Tags.plug( MM );
		}
		writer_ &operator =( const writer_ &W )
		{
			Tags = W.Tags;

			S_.TagNameInProgress = W.S_.TagNameInProgress;
			S_.Flow = W.S_.Flow;

			return *this;
		}
		void Init( txf::text_oflow__ &Flow )
		{
			reset();

			Tags.Init();
			S_.Flow = &Flow;
		}
		void PushTag( const name_ &Name )
		{
			if ( S_.TagNameInProgress )
				*S_.Flow << '>';

			*S_.Flow << '<' << Name;
			Tags.Push( Name );
			S_.TagNameInProgress = true;
		}
		void PushTag( const char *Name )
		{
			PushTag( name( Name ) );
		}
		void PutValue( const value_ &Value );
		void PutValue( const char *Value )
		{
			PutValue( value( Value ) );
		}
		void PutValue(
			const value_ &Value,
			const name_ &Name )
		{
			PushTag( Name );

			PutValue( Value );

			PopTag();
		}
		void PutValue(
			const value_ &Value,
			const char *Name )
		{
			PutValue( Value, name( Name ) );
		}
		void PutValue(
			const char *Value,
			const char *Name )
		{
			PutValue( value( Value ), name( Name ) );
		}
		void PutAttribute(
			const name_ &Name,
			const value_ &Value )
		{
			if ( !S_.TagNameInProgress )
				ERRu();

			*S_.Flow << ' ' << Name << "=\"" << Value << '"';
		}
		void PutAttribute(
			const char *Name,
			const char *Value )
		{
			PutAttribute( name( Name ), value( Value ) );
		}
		void PutAttribute(
			const char *Name,
			const value_ &Value )
		{
			PutAttribute( name( Name ), Value );
		}
		void PopTag( void );
	};

	E_AUTO( writer )
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
