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

#ifndef PLGN__INC
# define PLGN__INC

# define PLGN_NAME		"PLGN"

# if defined( E_DEBUG ) && !defined( PLGN_NODBG )
#  define PLGN_DBG
# endif

// PLuGiN

# include "plgncore.h"

# include "err.h"
# include "flw.h"

# include "dlbrry.h"
# include "fnm.h"

# include "sclerror.h"

namespace plgn {
	template <typename plugin> class retriever___
	{
	private:
		dlbrry::dynamic_library___ _Library;
		Q37_MRMDF( plgncore::callback__, C_, Callback_ );
		plugin *_Plugin;
		void _ReleasePlugin( void )
		{
			if ( _Plugin == NULL )
				qRFwk();

			C_().ReleasePlugin( _Plugin );
		}
		bso::bool__ _IsCompatible( void )
		{
			plgncore::plugin_identification *Function = dlbrry::GetFunction<plgncore::plugin_identification *>( E_STRING( PLGNCORE_PLUGIN_IDENTIFICATION_FUNCTION_NAME ), _Library );

			if ( Function == NULL )
				qRFwk();

			return !strcmp(plugin::Identification(), Function() );
		}
	private:
		bso::bool__ SubInit_(
			const ntvstr::string___ &PluginPath,
			err::handling__ ErrHandling )
		{
			plgncore::retrieve_callback *Function = NULL;
			if ( !_Library.Init( PluginPath, ErrHandling ) )
				if ( ErrHandling == err::hThrowException )
					qRFwk();
				else
					return false;

			if ( !_IsCompatible() )
				if ( ErrHandling == err::hThrowException )
					qRFwk();
				else
					return false;

			Function = dlbrry::GetFunction<plgncore::retrieve_callback *>( E_STRING( PLGNCORE_RETRIEVE_CALLBACK_FUNCTION_NAME ), _Library );

			if ( Function == NULL )
				if ( ErrHandling == err::hThrowException )
					qRFwk();
				else
					return false;

			Callback_ = &Function();

			return true;
		}
	public:
		void reset( bso::bool__ P = true )
		{
			if ( P ) {
				if ( _Plugin != NULL )
					_ReleasePlugin();
			}

			_Library.reset( P );
			Callback_ = NULL;
			_Plugin = NULL;
		}
		E_CDTOR( retriever___ );
		bso::bool__ Init(
			const ntvstr::string___ &PluginPath,
			const rgstry::entry__ &Configuration,
			const rgstry::entry__ &Locale,
			const str::string_ &Arguments,
			err::handling__ ErrHandling = err::h_Default )
		{
		qRH
			plgncore::data__ Data;
		qRB
			if ( !SubInit_( PluginPath, ErrHandling ) )
				return false;

			Data.Init( err::qRRor, sclerror::SCLERRORError, Configuration, Locale, Arguments );

			C_().Initialize( &Data );

			_Plugin = (plugin *)C_().RetrievePlugin();

			if ( ( _Plugin == NULL) && ( ErrHandling == err::hThrowException ) )
				qRFwk();
		qRR
		qRT
		qRE
			return _Plugin != NULL;
		}
		bso::bool__ Init(
			const ntvstr::string___ &PluginPath,
			const rgstry::tentry__ &Configuration,
			const rgstry::tentry__ &Locale,
			const rgstry::multi_level_registry_ &Registry,
			const str::string_ &Arguments,
			err::handling__ ErrHandling = err::h_Default )
		{
			bso::bool__ Success = false;
		qRH
			rgstry::entry__ ConfigurationEntry, LocaleEntry;
		qRB
			ConfigurationEntry.Init();

			if ( !Registry.Convert( Configuration, ConfigurationEntry, ErrHandling ) )
				qRReturn;

			LocaleEntry.Init();

			if ( !Registry.Convert( Locale, LocaleEntry, ErrHandling ) )
				qRReturn;

			if ( !Init( PluginPath, ConfigurationEntry, LocaleEntry, Arguments, ErrHandling ) )
				qRReturn;

			Success = true;
		qRR
		qRT
		qRE
			return Success;
		}
		bso::bool__ Init(
			const ntvstr::string___ &PluginPath,
			const str::string_ &Arguments,
			err::handling__ ErrHandling = err::h_Default )
		{
		qRH
			plgncore::data__ Data;
			fnm::name___ Location;
		qRB
			if ( !SubInit_( PluginPath, ErrHandling ) )
				return false;

			Location.Init();
			fnm::GetLocation( PluginPath, Location );

			Data.Init( err::qRRor, sclerror::SCLERRORError, Location, Arguments );

			C_().Initialize( &Data );

			_Plugin = (plugin *)C_().RetrievePlugin();

			if ( ( _Plugin == NULL) && ( ErrHandling == err::hThrowException ) )
				qRFwk();
		qRR
		qRT
		qRE
			return _Plugin != NULL;
		}
		plugin &Plugin( void )
		{
			if ( _Plugin == NULL )
				qRFwk();

			return *_Plugin;
		}
	};
}

#endif
