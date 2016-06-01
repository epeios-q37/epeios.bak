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

// Hypertext Transfert Protocol 

#ifndef HTP__INC
# define HTP__INC

#define HTP_NAME		"HTP"

# if defined( E_DEBUG ) && !defined( HTP_NODBG )
#  define HTP_DBG
# endif

# include "err.h"
# include "flw.h"
# include "bso.h"
# include "str.h"
# include "txf.h"
# include "crt.h"

namespace htp {
	qENUM( Status ) {
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
		s_Undefined,
	};

	qENUM( Field ) {
		fURL,
		fHost,
		fReferrer,
		fAccept,
		fContentType,
		fContentLength,
		fLocation,
		f_amount,
		f_Undefined
	};

	typedef str::dString	dValue;
	typedef str::wString	wValue;

	using str::cHook;

	class dField {
	public :
		struct s {
			eField Name;
			dValue::s Value;
		} &S_;
		dValue Value;
		dField( s &S )
		: S_( S ),
		  Value( S.Value )
		{}
		void reset( bso::bool__ P = true )
		{
			Value.reset( P );
			S_.Name = f_Undefined;
		}
		void plug( cHook &Hook )
		{
			Value.plug( Hook );
		}
		void plug( qASd *AS )
		{
			Value.plug( AS );
		}
		dField &operator =( const dField &F )
		{
			Value = F.Value;
			S_.Name = F.S_.Name;

			return *this;
		}
		void Init( void )
		{
			S_.Name = f_Undefined;
			Value.Init();
		}
		void Init(
			eField Name,
			const str::string_ &Value )
		{
			S_.Name = Name;
			this->Value.Init( Value );
		}
		void Init(
			eField Name,
			const char *Value )
		{
			S_.Name = Name;
			this->Value.Init( Value );
		}
		E_RODISCLOSE_( eField, Name )
	};

	qW( Field );

	typedef crt::qCRATEdl( dField )	dFields;
	qW( Fields );

	typedef bso::sUInt sContentLength;

	class dHeader
	{
	public:
		struct s
		{
			sContentLength ContentLength;
			str::dString::s Location;
			str::dString::s ContentType;
		} &S_;
		dHeader( s &S )
		: S_( S ),
		  Location( S.Location ),
		  ContentType( S.ContentType )
		{}
		str::dString Location;
		str::dString ContentType;
		void reset( bso::sBool P = true )
		{
			S_.ContentLength = 0;
			Location.reset( P );
			ContentType.reset( P );
		}
		void plug( qASd *AS )
		{
			Location.plug( AS );
			ContentType.plug( AS );
		}
		dHeader &operator =( const dHeader &H )
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
		qRODISCLOSEd( sContentLength, ContentLength );
	};

	qW( Header )

	//f Parse 'flow' and fill 'Header' with it. Return status. 'Header' is only fill when return value is 'sOK'.
	eStatus Parse(
		flw::iflow__ &IFlow,
		dHeader &Header );

	void Post( 
		const str::string_ &URL,
		const dFields &Fields,
		const str::string_ &Content,
		txf::text_oflow__ &Flow );
}

#endif
