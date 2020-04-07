/*
	Copyright (C) 1999 Claude SIMON (http://q37.info/contact/).

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

// SoCLe Plugin

#ifndef SCLP_INC_
# define SCLP_INC_

# define SCLP_NAME		"SCLP"

# if defined( E_DEBUG ) && !defined( SCLP_NODBG )
#  define SCLP_DBG
# endif

# include "scli.h"

# include "err.h"
# include "plgncore.h"

namespace sclp {
	typedef plgncore::callback__ _callback__;

	class callback__
	: public _callback__
	{
	protected:
		virtual void PLGNCOREInitialize(
			const plgncore::sData *Data,
			const rgstry::entry__ &Configuration ) override;
		virtual void PLGNCOREInitialize(
			const plgncore::sData *Data,
			const fnm::name___ &Directory ) override;
		virtual void *PLGNCORERetrievePlugin( plgncore::sAbstract *Abstract ) override;
		virtual bso::sBool PLGNCOREReleasePlugin( void *Plugin ) override;
		virtual const char *PLGNCOREPluginIdentifier( void ) override;
		virtual const char *PLGNCOREPluginDetails( void ) override;
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
	void SCLPPluginIdentifier( str::dString &Identifier );
	void SCLPPluginDetails( str::dString &Details );
	void SCLPPluginParameters( str::dStrings &Parameters );
	const scli::sInfo &SCLPInfo( void );	// To define by user.
	// The following ones are defined by below macro.
	const char *SCLPPluginLabel( void );
	void *SCLPRetrievePlugin( plgncore::sAbstract *Abstract );
	bso::sBool SCLPReleasePlugin( void * );
}

// NOTA : needed parameters are generally retrieved from the registry,
// which is automatically filled by this module.
# define SCLP_DEF( plugin )\
	const char *sclp::SCLPPluginLabel( void )\
	{\
		return plugin::Label();\
	}\
\
	void *sclp::SCLPRetrievePlugin( plgncore::sAbstract *Abstract )\
	{\
		plugin *Plugin = NULL;\
	qRH\
	qRB\
		Plugin = new plugin;\
		\
		if ( Plugin == NULL )\
			qRAlc();\
		\
		if ( !Plugin->SCLPInitialize( Abstract ) ) {\
			delete Plugin;\
			Plugin = NULL;\
		}\
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
	bso::sBool sclp::SCLPReleasePlugin( void *Plugin )\
	{\
		bso::sBool Success = false;\
	qRH\
	qRB\
		if ( Plugin == NULL )\
			qRFwk();\
		\
		delete (plugin *)Plugin;\
		\
		Success = true;\
	qRR\
		ERRRst();\
	qRT\
	qRE\
		return Success;\
	}

#endif
