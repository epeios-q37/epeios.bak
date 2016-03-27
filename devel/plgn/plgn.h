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

/*************************/
/****** New version ******/
/*************************/

namespace plgn {
	class rLooseRetriever
	{
	private:
		dlbrry::dynamic_library___ Library_;
		qRMV( plgncore::callback__, C_, Callback_ );
		void *Plugin_;
		void ReleasePlugin_( void )
		{
			if ( Plugin_ == NULL )
				qRFwk();

			C_().ReleasePlugin( Plugin_ );
		}
			private:
		bso::sBool SubInitialize_(
			const ntvstr::string___ &PluginPath,
			const char *Label,
			const char *Identifier,
			err::handling__ ErrHandling );
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
			const char *Label,
			const char *Identifier,
			const rgstry::entry__ &Configuration,
			const str::string_ &Arguments,
			err::handling__ ErrHandling = err::h_Default );
		bso::bool__ Initialize(
			const ntvstr::string___ &PluginPath,
			const char *Label,
			const char *Identifier,
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

			if ( !Initialize( PluginPath, Label, Identifier, ConfigurationEntry, Arguments, ErrHandling ) )
				qRReturn;

			Success = true;
		qRR
		qRT
		qRE
			return Success;
		}
		bso::bool__ Initialize(
			const ntvstr::string___ &PluginPath,
			const char *Label,
			const char *Identifier,
			const str::string_ &Arguments,
			err::handling__ ErrHandling = err::h_Default );
		const char *Identifier( void )
		{
			return C_().PluginIdentifier();
		}
		const char *Details( void )
		{
			return C_().PluginDetails();
		}
		void *Plugin( void ) const
		{
			if ( Plugin_ == NULL )
				qRFwk();

			return Plugin_;
		}
	};

	qROW( Row );

	typedef bch::qBUNCHd( rLooseRetriever, sRow ) dRetrievers;
	qW( Retrievers );

	void Delete_( dRetrievers &Retrievers );

	template <typename plugin> class rRetrievers
	: public wRetrievers
	{
	public:
		void reset( bso::sBool P = true )
		{
			if ( P )
				Delete_( *this );

			wRetrievers::reset( P );
		}
		qCDTOR( rRetrievers);
		void Init( void )
		{
			Delete_( *this );

			wRetrievers::Init();
		}
		plugin &Plugin( sRow Row ) const
		{
			rLooseRetriever Retriever;

			wRetrievers::Recall( Row, Retriever );

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
			const char *Identifier,
			const rgstry::entry__ &Configuration,
			const str::string_ &Arguments,
			err::handling__ ErrHandling = err::h_Default )
		{
			return LooseRetriever_.Initialize( PluginPath, plugin::Label(), Identifier, Configuration, Arguments, ErrHandling );
		}
		bso::bool__ Initialize(
			const ntvstr::string___ &PluginPath,
			const char *Identifier,
			const rgstry::tentry__ &Configuration,
			const rgstry::multi_level_registry_ &Registry,
			const str::string_ &Arguments,
			err::handling__ ErrHandling = err::h_Default )
		{
			return LooseRetriever_.Initialize( PluginPath, plugin::Label(), Identifier, Configuration, Registry, Arguments, ErrHandling );
		}
		bso::bool__ Initialize(
			const ntvstr::string___ &PluginPath,
			const char *Identifier,
			const str::string_ &Arguments,
			err::handling__ ErrHandling = err::h_Default )
		{
			return LooseRetriever_.Initialize( PluginPath, plugin::Label(), Identifier, Arguments, ErrHandling );
		}
		const char *Identifier( void )
		{
			return LooseRetriever_.Identifier();
		}
		const char *Details( void )
		{
			return LooseRetriever_.Details();
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

	bso::sBool IdentifierAndDetails(
		const ntvstr::string___ &PluginPath,
		str::dString &Identifier,
		str::dString &Details,
		err::handling__ ErrHandling = err::h_Default );
}

#endif
