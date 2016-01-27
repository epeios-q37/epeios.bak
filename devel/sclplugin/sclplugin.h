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

#ifndef SCLPLUGIN__INC
# define SCLPLUGIN__INC

# define SCLPLUGIN_NAME		"SCLPLUGIN"

# if defined( E_DEBUG ) && !defined( SCLPLUGIN_NODBG )
#  define SCLPLUGIN_DBG
# endif

// SoCLe PLUGIN

# include "plgncore.h"

# include "err.h"
# include "flw.h"

namespace sclplugin {
	typedef plgncore::callback__ _callback__;

	class callback__
	: public _callback__
	{
	protected:
		virtual void PLGNCOREInitialize(
			const plgncore::data__ *Data,
			const rgstry::entry__ &Configuration ) override;
		virtual void PLGNCOREInitialize(
			const plgncore::data__ *Data,
			const fnm::name___ &Directory,
			str::string_ &Locale ) override;
		virtual void *PLGNCORERetrievePlugin( void ) override;
		virtual void PLGNCOREReleasePlugin( void *Plugin ) override;
		virtual const char *PLGNCOREPluginIndentifier( void ) override;
	public:
		void reset( bso::bool__ P = true )
		{
			_callback__::reset( P );
		}
		E_CVDTOR( callback__ );
		void Init( void )
		{
			_callback__::Init();
		}
	};

	// Functions to overload.
	const char *SCLPLUGINPluginIdentifier( void );
	// The following ones are defined by below macro.
	const char *SCLPLUGINPluginLabel( void );
	void *SCLPLUGINRetrievePlugin( void );
	void SCLPLUGINReleasePlugin( void * );
}

// If this macro is used, you have to define a 'SCLPLUGINInitialize()' member.
// NOTA : needed parameters are generally retrieved from the registry,
// which is automatically filled by this module.
# define SCLPLUGIN_DEF( plugin )\
	const char *sclplugin::SCLPLUGINPluginLabel( void )\
	{\
		return plugin::Label();\
	}\
\
	void *sclplugin::SCLPLUGINRetrievePlugin( void )\
	{\
		plugin *Plugin = NULL;\
	qRH\
	qRB\
		Plugin = new plugin;\
		\
		if ( Plugin == NULL )\
			qRAlc();\
		\
		Plugin->SCLPLUGINInitialize();\
	qRR\
		if ( Plugin != NULL ) {\
			delete Plugin;\
			Plugin = NULL;\
		}\
	qRT\
	qRE\
		return Plugin;\
	}\
\
	void sclplugin::SCLPLUGINReleasePlugin( void *Plugin )\
	{\
		if ( Plugin == NULL )\
			qRFwk();\
		\
		delete (plugin *)Plugin;\
	}

#endif
