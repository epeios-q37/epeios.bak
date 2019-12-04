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

#ifndef FRDMISC_INC_
# define FRDMISC_INC_

# include "frdcllbck.h"

# include "sclfrntnd.h"

# include "fbltyp.h"

# include "ctn.h"

namespace frdmisc {

	frdcllbck::cXML &Get( const str::dString &Label );

	SCLF_I( type, Type, Id );

	struct dXType {
	public:
		struct s {
			sType Id;
			frdcllbck::cXML *Callback;
			str::dString::s Label;
		} &S_;
		str::dString Label;
		dXType( s &S )
		: S_( S ),
		  Label( S_.Label )
		{}
		void reset( bso::sBool P = true )
		{
			S_.Id = fbltyp::UndefinedId;
			S_.Callback = NULL;
			Label.reset( P );
		}
		void plug( str::sHook &Hook )
		{
			Label.plug( Hook );
		}
		void plug( qASd *AS )
		{
			Label.plug( AS );
		}
		dXType &operator =(const dXType &XT)
		{
			S_.Id = XT.S_.Id;
			S_.Callback = XT.S_.Callback;
			Label = XT.Label;

			return *this;
		}
		void Init(
			sType Id,
			const str::dString &Label,
			frdcllbck::cXML &Callback )
		{
			S_.Id = Id;
			S_.Callback = &Callback;
			this->Label.Init( Label );
		}
		sType Id( void ) const
		{
			if ( S_.Id == fbltyp::UndefinedId )
				qRGnr();

			return S_.Id;
		}
		frdcllbck::cXML &Callback( void ) const
		{
			if ( S_.Callback == NULL )
				qRGnr();

			return *S_.Callback;
		}
	};

	qW( XType );

	typedef ctn::qMCONTAINERdl( dXType ) dXTypes;
	qW( XTypes );

	void LoadPlugins( void );
}

#endif
