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

/*************************/
/****** New version ******/
/*************************/

namespace plgn {
	class rLooseRetriever
	{
	private:
		dlbrry::dynamic_library___ Library_;
		qRVM( plgncore::callback__, C_, Callback_ );
		void *Plugin_;
		void ReleasePlugin_( void )
		{
			if ( Plugin_ == NULL )
				qRFwk();

			C_().ReleasePlugin( Plugin_ );
		}
		bso::bool__ _IsCompatible( const char *Identification )
		{
			plgncore::plugin_identification *Function = dlbrry::GetFunction<plgncore::plugin_identification *>( E_STRING( PLGNCORE_PLUGIN_IDENTIFICATION_FUNCTION_NAME ), Library_ );

			if ( Function == NULL )
				qRFwk();

			return !strcmp( Identification, Function() );
		}
	private:
		bso::bool__ SubInitialize_(
			const ntvstr::string___ &PluginPath,
			const char *Identification,
			err::handling__ ErrHandling )
		{
			plgncore::retrieve_callback *Function = NULL;

			if ( !Library_.Init( PluginPath, ErrHandling ) ) {
				if ( ErrHandling == err::hThrowException )
					qRFwk();
				else
					return false;
			}

			if ( !_IsCompatible( Identification ) ) {
				if ( ErrHandling == err::hThrowException )
					qRFwk();
				else
					return false;
			}

			Function = dlbrry::GetFunction<plgncore::retrieve_callback *>( E_STRING( PLGNCORE_RETRIEVE_CALLBACK_FUNCTION_NAME ), Library_ );

			if ( Function == NULL ) {
				if ( ErrHandling == err::hThrowException )
					qRFwk();
				else
					return false;
			}

			Callback_ = &Function();

			return true;
		}
	public:
		void reset( bso::bool__ P = true )
		{
			if ( P ) {
				if ( Plugin_ != NULL )
					ReleasePlugin_();
			}

			Library_.reset( P );
			Callback_ = NULL;
			Plugin_ = NULL;
		}
		E_CDTOR( rLooseRetriever );
		void Init( void )
		{
			reset();
		}
		bso::bool__ Initialize(
			const ntvstr::string___ &PluginPath,
			const char *Identification,
			const rgstry::entry__ &Configuration,
			const str::string_ &Arguments,
			err::handling__ ErrHandling = err::h_Default )
		{
		qRH
			plgncore::data__ Data;
		qRB
			if ( !SubInitialize_( PluginPath, Identification, ErrHandling ) )
				return false;

			Data.Init( err::qRRor, sclerror::SCLERRORError, Arguments );

			C_().Initialize( &Data, Configuration );

			Plugin_ = C_().RetrievePlugin();

			if ( ( Plugin_ == NULL) && ( ErrHandling == err::hThrowException ) )
				qRFwk();
		qRR
		qRT
		qRE
			return Plugin_ != NULL;
		}
		bso::bool__ Initialize(
			const ntvstr::string___ &PluginPath,
			const char *Identification,
			const rgstry::tentry__ &Configuration,
			const rgstry::multi_level_registry_ &Registry,
			const str::string_ &Arguments,
			err::handling__ ErrHandling = err::h_Default )
		{
			bso::bool__ Success = false;
		qRH
			rgstry::entry__ ConfigurationEntry;
		qRB
			ConfigurationEntry.Init();

			if ( !Registry.Convert( Configuration, ConfigurationEntry, ErrHandling ) )
				qRReturn;

			if ( !Initialize( PluginPath, Identification, ConfigurationEntry, Arguments, ErrHandling ) )
				qRReturn;

			Success = true;
		qRR
		qRT
		qRE
			return Success;
		}
		bso::bool__ Initialize(
			const ntvstr::string___ &PluginPath,
			const char *Identification,
			const str::string_ &Arguments,
			err::handling__ ErrHandling = err::h_Default )
		{
		qRH
			plgncore::data__ Data;
			fnm::name___ Location;
			str::string Locale;
		qRB
			if ( !SubInitialize_( PluginPath, Identification, ErrHandling ) )
				return false;

			Location.Init();
			fnm::GetLocation( PluginPath, Location );

			Data.Init( err::qRRor, sclerror::SCLERRORError, Arguments );

			Locale.Init();
			C_().Initialize( &Data, Location, Locale );

			// Temporary workaround for the shared data from twice loaded plugin.
			/* When a plugin is loaded several time by the same excutable, its data
			is common to all library. Actually, the 'Configuration' and 'Locale'
			registry are the one from the foirst loaded library. The 'Argument'
			registry is the one from the last one. So, the 'Configuration' and
			'Locale' section should be the same for all the plugin, and you should
			read what be needed from the registry before returoing from here. */
			if ( Locale.Amount() != 0 )
				scllocale::Fill(scllocale::tMain, NULL, Location, Locale );

			Plugin_ = C_().RetrievePlugin();

			if ( ( Plugin_ == NULL) && ( ErrHandling == err::hThrowException ) )
				qRFwk();
		qRR
		qRT
		qRE
			return Plugin_ != NULL;
		}
		void *Plugin( void ) const
		{
			if ( Plugin_ == NULL )
				qRFwk();

			return Plugin_;
		}
	};

	qROW( Row );

	typedef bch::qBUNCHv( rLooseRetriever, fRow ) vRetrievers;
	qW( Retrievers );

	void Delete_( iRetrievers &Retrievers );

	template <typename plugin> class rRetrievers
	: public iRetrievers
	{
	public:
		void reset( bso::fBool P = true )
		{
			if ( P )
				Delete_( *this );

			vRetrievers::reset( P );
		}
		qCDTOR( rRetrievers);
		void Init( void )
		{
			Delete_( *this );

			iRetrievers::Init();
		}
		plugin &Plugin( fRow Row ) const
		{
			rLooseRetriever Retriever;

			iRetrievers::Recall( Row, Retriever );

			plugin &Plugin = *(plugin *)Retriever.Plugin();

			Retriever.reset( false );

			return Plugin;
		}
	};


	template <typename plugin> class rRetriever
	{
	private:
		rLooseRetriever LooseRetriever_;
	public:
		void reset( bso::bool__ P = true )
		{
			LooseRetriever_.reset( P );
		}
		E_CDTOR( rRetriever );
		void Init( void )
		{
			reset();
		}
		bso::bool__ Initialize(
			const ntvstr::string___ &PluginPath,
			const rgstry::entry__ &Configuration,
			const str::string_ &Arguments,
			err::handling__ ErrHandling = err::h_Default )
		{
			return LooseRetriever_.Initialize( PluginPath, plugin::Identification(), Configuration, Arguments, ErrHandling );
		}
		bso::bool__ Initialize(
			const ntvstr::string___ &PluginPath,
			const rgstry::tentry__ &Configuration,
			const rgstry::multi_level_registry_ &Registry,
			const str::string_ &Arguments,
			err::handling__ ErrHandling = err::h_Default )
		{
			return LooseRetriever_.Initialize( PluginPath, plugin::Identification(), Configuration, Registry, Arguments, ErrHandling );
		}
		bso::bool__ Initialize(
			const ntvstr::string___ &PluginPath,
			const str::string_ &Arguments,
			err::handling__ ErrHandling = err::h_Default )
		{
			return LooseRetriever_.Initialize( PluginPath, plugin::Identification(), Arguments, ErrHandling );
		}
		plugin &Plugin( void )
		{
			return *(plugin *)LooseRetriever_.Plugin();
		}
	};
}

/*************************/
/****** Old version ******/
/*************************/

namespace plgn {
	template <typename plugin> E_TTCLONE__( plgn::rRetriever<plugin>, retriever___ );
}

#endif
