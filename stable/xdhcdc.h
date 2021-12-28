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

// XDH(TML) Common Downstream Callbacks

#ifndef XDHCDC_INC_
# define XDHCDC_INC_

# define XDHCDC_NAME		"XDHCDC"

# if defined( E_DEBUG ) && !defined( XDHCDC_NODBG )
#  define XDHCDC_DBG
# endif

# include "xdhcuc.h"

# include "bso.h"
# include "err.h"
# include "sclm.h"

# define XDHCDC_DATA_VERSION_NUMBER	    "13"

# define XDHCDC_DATA_VERSION		    XDHCDC_DATA_VERSION_NUMBER "-" CPE_AL

# define XDHCDC_RETRIEVE_FUNCTION_NAME	XDHCDCRetrieve

namespace xdhcdc {
	qENUM( Mode )
	{
		mMonoUser,	// One use only. The content of the project (i.e. which backend type to use) is defined by the user.
		mMultiUser,	// Several users. The project (which defines the backend to use) is predefined in the configuration file.
		m_amount,
		m_Undefined
	};

	class cSingle
	{
	protected:
		virtual bso::sBool XDHCDCInitialize(
			xdhcuc::cSingle &Callback,
			const char *Language,
			const str::dString &Token) = 0;// If empty, SlfH session, else token used for the FaaS session.
		virtual bso::bool__ XDHCDCHandle(
      const str::dString &Id,
      const str::dString &Action) = 0;
	public:
		qCALLBACK( Single );
		bso::sBool Initialize(
			xdhcuc::cSingle &Callback,
			const char *Language,
			const str::dString &Token)	// If empty, SlfH session, else token used for the FaaS session.
		{
			return XDHCDCInitialize(Callback, Language, Token);
		}
		bso::bool__ Handle(
      const str::dString &Id,
      const str::dString &Action)
		{
			return XDHCDCHandle(Id, Action);
		}
	};

#pragma pack( push, 1)
	// NOTA : is modified, increment 'XDHCDC_DATA_VERSION_NUMBER' !
	class sData
	{
	private:
		const char *Version_;	// Always at first position
		bso::uint__ Control_;	// A value which depends on the content of this structure, for basic compatibility test.
		eMode Mode_;
		const char *LauncherIdentification_;
		const char *Localization_;
		sclm::sRack SCLRack_;
	public:
		void reset( bso::bool__ P = true )
		{
			Version_ = NULL;
			Control_ = 0;
			Mode_ = m_Undefined;
			LauncherIdentification_ = NULL;
			Localization_ = NULL;
			SCLRack_.reset( P );
		}
		E_CDTOR( sData );
		void Init(
			eMode Mode,
			const char *LauncherIdentification,
			const char *Localization)
		{
			Version_ = XDHCDC_DATA_VERSION;
			Control_ = ComputeControl();
			Mode_ = Mode;
			LauncherIdentification_ = LauncherIdentification;
			Localization_ = Localization;
			SCLRack_.Init();
		}
		size_t ComputeControl( void )
		{
			return sizeof( sData );
		}
		qRWDISCLOSEs( sclm::sRack, SCLRack );
		Q37_PMDF( const char, LauncherIdentification, LauncherIdentification_ );
		Q37_PMDF( const char, Localization, Localization_ );
		qRODISCLOSEs( eMode, Mode );
	};
#pragma pack( pop )

	class cGlobal
	{
	protected:
		virtual xdhcmn::sScriptsVersion XDHCDCInitialize(
			const sData &Data,
			xdhcuc::cGlobal &Callback) = 0;
		virtual void XDHCDCBaseLanguage( TOL_CBUFFER___ &Buffer ) = 0;
		virtual cSingle *XDHCDCFetchCallback(void) = 0;	// Store 'Id', as it will requested by the 'XDHCUC' module.
		virtual void XDHCDCDismissCallback( cSingle *Callback ) = 0;
		virtual const scli::sInfo &XDHCDCGetInfo( void ) = 0;
		// The returned value is only for 'FaaS' mode. In other mode, must always return 'true',
		// or false on error accordingly with 'qRP'.
		// In 'FaaS' mode, returns true when a application for the given 'Token' exists, false otherwise.
		virtual bso::sBool XDHCDCGetHead(
			const str::dString &Token,  // Always empty when NOT in FaaS mode.
			str::dString &Head,
			qRPD ) = 0;
	public:
		qCALLBACK( Global )
		xdhcmn::sScriptsVersion Initialize(
			const sData &Data,
			xdhcuc::cGlobal &Callback)
		{
			return XDHCDCInitialize(Data, Callback);
		}
		const char *BaseLanguage( TOL_CBUFFER___ &Buffer )
		{
			XDHCDCBaseLanguage( Buffer );

			return Buffer;
		}
		cSingle *FetchCallback(void)
		{
			return XDHCDCFetchCallback();
		}
		void DismissCallback( cSingle *Callback )
		{
			return XDHCDCDismissCallback( Callback );
		}
		const scli::sInfo &GetInfo( void )
		{
			return XDHCDCGetInfo();
		}
		bso::sBool GetHead(
			const str::dString &Token,
			str::dString &Head)
		{
			return XDHCDCGetHead(Token, Head);
		}
	};

	typedef cGlobal *(retrieve)( void );

}

#endif
