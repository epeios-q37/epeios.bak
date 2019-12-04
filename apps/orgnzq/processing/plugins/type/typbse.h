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

// TYPe plugin BaSE

#ifndef TYPBSE_INC_
# define TYPBSE_INC_

# include "typrgy.h"

# include "ogzplg.h"

namespace typbse {
	typedef ogzplg::fTypePlugin fPlugin_; 

	class rPluginCommon
	: public fPlugin_
	{
	private:
		TOL_CBUFFER___ LabelBuffer_;
	protected:
		const char *OGZTYPGetLabel( void ) override;
	public:
		void reset( bso::bool__ P = true )
		{
			fPlugin_::reset( P );
			LabelBuffer_.reset( P );
		}
		qCVDTOR( rPluginCommon );
		void Init( void )
		{
			fPlugin_::Init();
			LabelBuffer_.reset();
		}
	};
}

#endif
