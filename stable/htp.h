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

//	$Id: htp.h,v 1.13 2010/06/05 16:21:59 csimon Exp $

#ifndef HTP__INC
#define HTP__INC

#define HTP_NAME		"HTP"

#define	HTP_VERSION	"$Revision: 1.13 $"

#define HTP_OWNER		"Claude SIMON (http://zeusw.org/intl/contact.html)"


#if defined( E_DEBUG ) && !defined( HTP_NODBG )
#define HTP_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.13 $
//C Claude SIMON (http://zeusw.org/intl/contact.html)
//R $Date: 2010/06/05 16:21:59 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D Hypertext Transfert Protocol 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

/* Addendum to the automatic documentation generation part. */
//D Hypertext Transfer Protocol
/* End addendum to automatic documentation generation part. */


#include "err.h"
#include "flw.h"
#include "bso.h"
#include "str.h"
#include "txf.h"
#include "ctn.h"

namespace htp {
	//e Status of the parsing.
	enum status__ {
		//i OK (HTTP code 200).
		sOK,
		//i Continue (HTTP code 100)
		sContinue,
		//i NotFound (HTTP code 404).
		sFound,
		//i Found (HTTP code 302).
		sNotFound,
		//i Unhandle HTTP response code.
		sUnhandledCode,
		//i Not HTTP protocol or bad version.
		sUnknowProtocol,
		//i Amount of status.
		s_amount,
		s_unknow,
	};

	enum field__ {
		fURL,
		fHost,
		fReferrer,
		fAccept,
		fContentType,
		fContentLength,
		fLocation,
		f_amount,
		f_unknow,
		f_end
	};

	typedef str::string_	field_value_;
	typedef str::string		field_value;

	class field_ {
	public :
		struct s {
			field__ Name;
			field_value_::s Value;
		} &S_;
		field_value_ Value;
		field_( s &S )
		: S_( S ),
		  Value( S.Value )
		{}
		void reset( bso::bool__ P = true )
		{
			Value.reset( P );
			S_.Name = f_unknow;
		}
		void plug( mdr::E_MEMORY_DRIVER__ &MD )
		{
			Value.plug( MD );
		}
		void plug( mmm::E_MULTIMEMORY_ &MM )
		{
			Value.plug( MM );
		}
		field_ &operator =( const field_ &F )
		{
			Value = F.Value;
			S_.Name = F.S_.Name;

			return *this;
		}
		void Init( void )
		{
			S_.Name = f_unknow;
			Value.Init();
		}
		void Init(
			field__ Name,
			const str::string_ &Value )
		{
			S_.Name = Name;
			this->Value.Init( Value );
		}
		void Init(
			field__ Name,
			const char *Value )
		{
			S_.Name = Name;
			this->Value.Init( Value );
		}
		E_RODISCLOSE_( field__, Name )
	};

	E_AUTO( field );

	typedef ctn::E_XMCONTAINER_( field_ )	fields_;
	E_AUTO( fields )


	typedef bso::ulong__ content_length__;

	//c An http 1.1 header.
	class header_
	{
	public:
		struct s
		{
			content_length__ ContentLength;
			str::string_::s Location;
			str::string_::s ContentType;
		} &S_;
		header_( s &S )
		: S_( S ),
		  Location( S.Location ),
		  ContentType( S.ContentType )
		{}
		str::string_ Location;
		str::string_ ContentType;
		void reset( bso::bool__ P = true )
		{
			S_.ContentLength = 0;
			Location.reset( P );
			ContentType.reset( P );
		}
		void plug( mmm::E_MULTIMEMORY_ &MM )
		{
			Location.plug( MM );
			ContentType.plug( MM );
		}
		header_ &operator =( const header_ &H )
		{
			S_.ContentLength = H.S_.ContentLength;
			Location = H.Location;
			ContentType = H.ContentType;

			return *this;
		}
		void Init( void )
		{
			S_.ContentLength = 0;
			Location.Init();
			ContentType.Init();
		}
		E_RODISCLOSE_( content_length__, ContentLength );
	};

	E_AUTO( header )

	//f Parse 'flow' and fill 'Header' with it. Return status. 'Header' is only fill when return value is 'sOK'.
	status__ Parse(
		flw::iflow__ &IFlow,
		header_ &Header );

	void Post( 
		const str::string_ &URL,
		const fields_ &Fields,
		const str::string_ &Content,
		txf::text_oflow__ &Flow );



}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
