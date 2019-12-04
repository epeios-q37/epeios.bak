/*
	Copyright (C) 2015-2016 Claude SIMON (http://q37.info/contact/).

	This file is part of 'orgnzq' software.

    'orgnzq' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'orgnzq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'orgnzq'.  If not, see <http://www.gnu.org/licenses/>.
*/

// OrGaniZer field TYPe

#ifndef OGZTYP_INC_
# define OGZTYP_INC_

# ifdef XXX_DBG
#	define OGZTYP__DBG
# endif

# include "ogzbsc.h"

# include "lstbch.h"
# include "str.h"

namespace ogztyp {
	qROW( Row );

	class cType
	{
	protected:
		// Returns the label.
		virtual const char *OGZTYPGetLabel( void ) = 0;
		// Convert into 'XML' the content of 'Buffer' of size 'Size' in a XML value form.
		virtual void OGZTYPToXML(
			const ogzbsc::dDatum &Datum,
			str::dString &XML ) = 0;
		virtual bso::sBool OGZTYPTest( const str::dString &Pattern ) = 0;
	public:
		qCALLBACK( Type );
		const char *GetLabel( void )
		{
			return OGZTYPGetLabel();
		}
		const str::dString &ToXML(
			const ogzbsc::dDatum &Datum,
			str::dString &XML )
		{
			OGZTYPToXML( Datum, XML );

			return XML;
		}
		bso::sBool Test( const str::dString &Pattern )
		{
			return OGZTYPTest( Pattern );
		}
	};

	class sType
	{
	private:
		qRMV( cType, C_, Callback_ );
		const char *Identifier_;
	public:
		qCDTOR( sType );
		void reset( bso::bool__ = true )
		{
			Callback_ = NULL;
			Identifier_ = NULL;
		}
		void Init(
			cType &Callback,
			const char *Identifier )
		{
			Callback_ = &Callback;
			Identifier_ = Identifier;
		}
		const char *GetLabel( void ) const
		{
			return C_().GetLabel();
		}
		const char *GetIdentifier( void ) const
		{
			if ( Identifier_ == NULL )
				qRGnr();

			return Identifier_;
		}
		const str::dString &ToXML(
			const ogzbsc::dDatum &Datum,
			str::dString &XML ) const
		{
			return C_().ToXML( Datum, XML );
		}
		bso::sBool Test( const str::dString &Pattern ) const
		{
			return C_().Test( Pattern );
		}
	};

	typedef lstbch::qLBUNCHd( sType, sRow ) dTypes;
	qW( Types );
}


#endif
