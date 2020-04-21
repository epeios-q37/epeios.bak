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

#ifndef XDHCMN_INC_
# define XDHCMN_INC_

# define XDHCMN_NAME		"XDHCMN"

# if defined( E_DEBUG ) && !defined( XDHCMN_NODBG )
#  define XDHCMN_DBG
# endif

// X(SL)/DH(TML) CoMmoN

# include "err.h"
# include "flw.h"
# include "strmrg.h"
# include "sclm.h"

# include <stdarg.h>

# define XDHCMN_SHARED_DATA_VERSION_NUMBER	"11"

# define XDHCMN_SHARED_DATA_VERSION			XDHCMN_SHARED_DATA_VERSION_NUMBER "-" CPE_AL

# define XDHCMN_RETRIEVE_FUNCTION_NAME		XDHCMNRetrieve

namespace xdhcmn {
	qMIMICs( bso::sU16, sIndex );	// For the 'CSS' rules.

	static E_CDEF( char *, CloseActionLabel, "Q37Close" );
	static E_CDEF( char *, RefreshActionLabel, "Q37Refresh" );

	qENUM( Mode ) {
		mMonoUser,	// One use only. The content of the project (i.e. which backend type to use) is defined by the user.
		mMultiUser,	// Several users. The project (which defines the backend to use) is predefiend in the configuration file.
		m_amount,
		m_Undefined
	};

	class cUpstream
	{
	protected:
        // The value returned by the script has to be stored
        // in 'ReturnedValue', unless it is equal to 'NULL'.
		virtual void XDHCMNProcess(
			const str::dString &Script,
			str::dString *ReturnedValue ) = 0;
        // Broadcast 'Id' (which can't be empty) to all clients.
//        virtual void XDHCMNBroadcast(const str::dString &Id) = 0;
	public:
		qCALLBACK( Upstream );
		void Process(
			const str::dString &Script,
			str::dString *ReturnValue = NULL)
		{
            return XDHCMNProcess(Script, ReturnValue);
		}
/*		void Braodcast(const str::dString &Id)
		{
		    return XDHCMNBroadcast(Id);
		}
	*/};

	class cSession
	{
	protected:
		virtual bso::sBool XDHCMNInitialize(
			cUpstream &Callback,
			const char *Language,
			const str::dString &Token) = 0;	// If empty, PROD session, else token used for the FaaS session.
		virtual bso::bool__ XDHCMNLaunch(
			const char *Id,
			const char *Action ) = 0;
	public:
		qCALLBACK( Session );
		bso::sBool Initialize(
			cUpstream &Callback,
			const char *Language,
			const str::dString &Token)	// If empty, PROD session, else token used for the FaaS session.
		{
			return XDHCMNInitialize(Callback, Language, Token);
		}
		bso::bool__ Launch(
			const char *Id,
			const char *Action )
		{
			return XDHCMNLaunch( Id, Action );
		}
	};

#pragma pack( push, 1)
	// NOTA : is modified, increment 'CSDLEO_SHARED_DATA_VERSION' !
	class shared_data__
	{
	private:
		const char *_Version;	// Toujours en premire position.
		bso::uint__ _Control;	// Une valeur relative au contenu de la structure,  des fins de test primaire de compatibilit.
		eMode Mode_;
		const char *_LauncherIdentification;
		const char *_Localization;
		sclm::sRack SCLRack_;
	public:
		void reset( bso::bool__ P = true )
		{
			_Version = NULL;
			_Control = 0;
			Mode_ = m_Undefined;
			_LauncherIdentification = NULL;
			_Localization = NULL;
			SCLRack_.reset( P );
		}
		E_CDTOR( shared_data__ );
		void Init(
			eMode Mode,
			const char *LauncherIdentification,
			const char *Localization )
		{
			_Version = XDHCMN_SHARED_DATA_VERSION;
			_Control = ControlComputing();
			Mode_ = Mode;
			_LauncherIdentification = LauncherIdentification;
			_Localization = Localization;
			SCLRack_.Init();
		}
		size_t ControlComputing( void )
		{
			return sizeof( shared_data__ );
		}
		qRWDISCLOSEs( sclm::sRack, SCLRack );
		Q37_PMDF( const char, LauncherIdentification, _LauncherIdentification );
		Q37_PMDF( const char, Localization, _Localization );
		qRODISCLOSEs( eMode, Mode );
	};
#pragma pack( pop )

	class cDownstream
	{
	protected:
		virtual void XDHCMNInitialize( const shared_data__ &Data ) = 0;
		virtual void XDHCMNBaseLanguage( TOL_CBUFFER___ &Buffer ) = 0;
		virtual cSession *XDHCMNRetrieveSession(void) = 0;
		virtual void XDHCMNReleaseSession( cSession *Session ) = 0;
		virtual const scli::sInfo &XDHCMNGetInfo( void ) = 0;
		// The returned value is only for 'FaaS' mode. In other mode, must always return 'true',
		// or false on error accordingly with 'qRP'.
		// In 'FaaS' mode, returns true when a application for the given 'Token' exists, false otherwise.
		virtual bso::sBool XDHCMNGetHead(
			const str::dString &Token,  // Always empty when NOT in FaaS mode.
			str::dString &Head,
			qRPD ) = 0;
	public:
		qCALLBACK( Downstream )
		void Initialize( const shared_data__ &Data )
		{
			return XDHCMNInitialize( Data );
		}
		const char *BaseLanguage( TOL_CBUFFER___ &Buffer )
		{
			XDHCMNBaseLanguage( Buffer );

			return Buffer;
		}
		cSession *RetrieveSession(void)
		{
			return XDHCMNRetrieveSession();
		}
		void ReleaseSession( cSession *Session )
		{
			return XDHCMNReleaseSession( Session );
		}
		const scli::sInfo &GetInfo( void )
		{
			return XDHCMNGetInfo();
		}
		const bso::sBool GetHead(
			const str::dString &Token,
			str::dString &Head)
		{
			return XDHCMNGetHead(Token, Head);
		}
	};

	typedef cDownstream *(retrieve)( void );

	void Escape(
		const str::string_ &Source,
		str::string_ &Target,
		bso::char__ Delimiter,	// Should be '\'', '"' or 0. If 0, escapes '\'' and '\"', otherwise escapes 'Delimiter'.
		bso::char__ EscapeChar = strmrg::DefaultEscapeToken );

	const str::dString &Escape(
		str::string_ &String,
		bso::char__ Delimiter,	// Should be '\'', '"' or 0. If 0, escapes '\'' and '\"', otherwise escapes 'Delimiter'.
		bso::char__ EscapeChar = strmrg::DefaultEscapeToken );

#if 0
	void Unescape(
		const str::string_ &Source,
		str::string_ &Target,
		bso::char__ EscapeChar = DefaultEscapeChar );
#endif

	typedef strmrg::table_ digest_;
	E_AUTO( digest );

	using strmrg::Split;
	using strmrg::Merge;

	using strmrg::retriever__;

	// Merge 'Splitted', delimited by '"', and separated by ','.
	void FlatMerge(
		const str::dStrings &Splitted,
		str::dString &Merged,
		bso::sBool AsJSArray );	// If 'true', the splitted items are enclosed between '[]', so the result can be used as JS array; above function does then NOT work.

	// Reverse of above (if 'AsJSArray' was to 'false').
	void FlatSplit(
		flw::rRFlow &Flow,
		str::dStrings &Splitted );

	// Variant of above.
	void FlatSplit(
		const str::dString &Merged,
		str::dStrings &Splitted );
}

/************/
/**** NEW ****/
/************/

namespace xdhcmn {
	typedef digest_ dDigest;
	qW( Digest )
}

#endif
