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

#ifndef FRDCLLBCK_INC_
# define FRDCLLBCK_INC_

# include "str.h"

namespace frdcllbck {
	class cXML
	{
	protected:
		virtual void FRDCLLBCKGetBackendPluginLabel( str::dString &Id ) = 0;
		virtual void FRDCLLBCKToXML(
			const str::dString &Input,
			str::dString &Output ) = 0;
	public:
		void ToXML(
			const str::dString &Input,
			str::dString &Output )
		{
			return FRDCLLBCKToXML( Input, Output );
		}
		const str::dString &GetBackendPluginLabel( str::dString &Id )
		{
			FRDCLLBCKGetBackendPluginLabel( Id );

			return Id;
		}

	};
}

#endif
