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
	using plgncore::sAbstract;

	extern sAbstract * NonNullAbstractPointer;	// To use as error pointer to report that the error is handled externally, but the error pointer has no other use.

	typedef bch::qBUNCHdl( plgncore::sAbstract * ) dAbstracts;

	qW( Abstracts );

	extern wAbstracts EmptyAbstracts;	// Sort of default parameter value.

	class rLooseRetriever
	{
	private:
		dlbrry::dynamic_library___ Library_;
		qRMV( plgncore::callback__, C_, Callback_ );
		void *Plugin_;
		void ReleasePlugin_( void )
		{
			bso::sBool Rethrow = false;

			if ( Plugin_ == NULL )
				qRFwk();

			Rethrow = !C_().ReleasePlugin( Plugin_ );

			Plugin_ = NULL;

			if ( Rethrow )
				ERRT();
		}
		void SubInitialize_(
			const ntvstr::string___ &PluginPath,
			const char *Label,
			const char *Identifier );
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
		/* For the three below method :
		If returned value == 'qNIL', there was no error, or the error was handled internally.
		if returned value != 'qNIL', the error data are in the 'UPs' of given row. */
		sdr::sRow Initialize(
			const ntvstr::string___ &PluginPath,
			const char *Label,
			const char *Identifier,
			const rgstry::entry__ &Configuration,
			const str::string_ &Arguments,
			const dAbstracts& Abstracts );
		// See above.
		sdr::sRow Initialize(
			const ntvstr::string___ &PluginPath,
			const char *Label,
			const char *Identifier,
			const rgstry::tentry__ &Configuration,
			const rgstry::multi_level_registry_ &Registry,
			const str::string_ &Arguments,
			const dAbstracts &Abstracts )
		{
			sdr::sRow Row = qNIL;
		qRH
			rgstry::entry__ ConfigurationEntry;
		qRB
			ConfigurationEntry.Init();

			if ( Registry.Convert( Configuration, ConfigurationEntry ) )
				Row = Initialize( PluginPath, Label, Identifier, ConfigurationEntry, Arguments, Abstracts );
		qRR
		qRT
		qRE
			return Row;
		}
		sdr::sRow Initialize(
			const ntvstr::string___ &PluginPath,
			const char *Label,
			const char *Identifier,
			const str::string_ &Arguments,
			const dAbstracts &Abstracts );
		bso::sBool IsReady( void ) const
		{
			return Plugin_ != NULL;
		}
		const char *Identifier( void ) const
		{
			return C_().PluginIdentifier();
		}
		const char *Details( void ) const
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
		const char *Identifier( sRow Row ) const
		{
			rLooseRetriever Retriever;

			wRetrievers::Recall( Row, Retriever );

			const char *Identifier = Retriever.Identifier();

			Retriever.reset( false );

			return Identifier;
		}
		const char *Details( sRow Row ) const
		{
			rLooseRetriever Retriever;

			wRetrievers::Recall( Row, Retriever );

			const char *Details = Retriever.Details();

			Retriever.reset( false );

			return Details;
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
		sdr::sRow Initialize(
			const ntvstr::string___ &PluginPath,
			const char *Identifier,
			const rgstry::entry__ &Configuration,
			const str::string_ &Arguments,
			const dAbstracts &Abstracts )
		{
			return LooseRetriever_.Initialize( PluginPath, plugin::Label(), Identifier, Configuration, Arguments, Abstracts );
		}
		sdr::sRow Initialize(
			const ntvstr::string___ &PluginPath,
			const char *Identifier,
			const rgstry::tentry__ &Configuration,
			const rgstry::multi_level_registry_ &Registry,
			const str::string_ &Arguments,
			const dAbstracts &Abstracts )
		{
			return LooseRetriever_.Initialize( PluginPath, plugin::Label(), Identifier, Configuration, Registry, Arguments, Abstracts );
		}
		sdr::sRow Initialize(
			const ntvstr::string___ &PluginPath,
			const char *Identifier,
			const str::string_ &Arguments,
			const dAbstracts &Abstracts )
		{
			return LooseRetriever_.Initialize( PluginPath, plugin::Label(), Identifier, Arguments, Abstracts );
		}
		bso::sBool IsReady( void ) const
		{
			return LooseRetriever_.IsReady();
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

	void IdentifierAndDetails(
		const ntvstr::string___ &PluginPath,
		str::dString &Identifier,
		str::dString &Details );

	qENUM( ErrorReporting ) {
		rhInternally,		// Errors are handled internally (mainly with a 'ReportAndAbort(...)').
		rhDetailed,			// Errors will be handled externally, details are given.
		rhBasic,			// Errors will be handled externally, but without details.
		rh_amount,
		rh_Undefined
	};

	inline eErrorReporting ErrorReporting( const sAbstract *Abstract )
	{
		if ( Abstract == NULL )
			return rhInternally;
		else if ( Abstract == NonNullAbstractPointer )
			return rhBasic;
		else
			return rhDetailed;
	}

	inline void Test(
		sAbstract *Abstract,
		const char *Identifier )
	{
		if ( ErrorReporting( Abstract ) == rhDetailed )
			if ( strcmp( Abstract->Identifier(), Identifier ) )
				qRFwk();
	}

}

/*************************/
/****** Old version ******/
/*************************/

namespace plgn {
	template <typename plugin> E_TTCLONE__( plgn::rRetriever<plugin>, retriever___ );
}

#endif
