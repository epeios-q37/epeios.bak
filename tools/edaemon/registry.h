/*
	Header for the 'registry' module by Claude SIMON (http://zeusw.org/epeios/contact.html).
	Part of th 'xxx_app' tool.
	Copyright (C) 2011 by Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of the Epeios project (http://zeusw.org/epeios/).

    This file is part of 'expp'.

    'expp' is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'expp' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with 'expp'.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef REGISTRY__INC
# define REGISTRY__INC

# include "sclrgstry.h"

namespace registry {
	namespace {
		using rgstry::entry___;
	}

	extern entry___ ModuleService;
	extern entry___ ModuleServiceType;
	extern entry___ ModuleFileName;
	extern entry___ ModuleLogFileName;
	extern entry___ ModuleLogMode;

	inline const char *GetModuleLogFileName( TOL_CBUFFER___ &FileName )
	{
		return sclrgstry::GetOptionalValue( ModuleLogFileName, FileName );
	}

	inline const str::string_ &GetRawModuleLogMode( str::string_ &Mode )
	{
		return sclrgstry::GetOptionalValue( ModuleLogMode, Mode );
	}

	inline const str::string_ &GetModuleFileName( str::string_ &FileName )
	{
		return sclrgstry::GetMandatoryValue( ModuleFileName, FileName );
	}

	inline bso::u16__ GetRawModuleService( void )
	{
		return sclrgstry::GetMandatoryU16( ModuleService );
	}

	inline const str::string_ &GetRawModuleServiceType( str::string_ &Type )
	{
		return sclrgstry::GetMandatoryValue( ModuleServiceType, Type );
	}
}

#endif
