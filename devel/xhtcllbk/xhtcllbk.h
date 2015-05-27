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

#ifndef XHTCLLBK__INC
# define XHTCLLBK__INC

# define XHTCLLBK_NAME		"XHTCLLBK"

# if defined( E_DEBUG ) && !defined( XHTCLLBK_NODBG )
#  define XHTCLLBK_DBG
# endif

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

// XHT(ML) CaLLBacK

# include "err.h"
# include "flw.h"
# include "tol.h"
# include "sdr.h"
# include "bch.h"
# include "xtf.h"
# include "strmrg.h"
# include "iof.h"
# include "sclerror.h"

# define XHTCLLBK_SHARED_DATA_VERSION_NUMBER	"9"

# define XHTCLLBK_SHARED_DATA_VERSION	XHTCLLBK_SHARED_DATA_VERSION_NUMBER "-" CPE_ARCHITECTURE_LABEL

# define XHTCLLBK_RETRIEVE_FUNCTION_NAME		XHTCLLBKRetrieve

namespace xhtcllbk {
	E_CDEF( char *, CloseActionLabel, "Q37Close" );

	typedef ntvstr::char__ nchar__;
	typedef ntvstr::string___ nstring___;

	enum function__ {		// Paramtres. 'Buffer' est un 'TOL_CBUFFER___' destin  receuillir la valeur de retour.
		fLog,				// Message,
		fAlert,				// XML, XSL, Title.
		fConfirm,			// XML, XSL, Title, Buffer.
		fSetChildren,		// Id, XML, XSL.
		fSetCasting,		// Id, XML, XSL.
		fSetProperty,		// Id, Name, Value.
		fGetProperty,		// Id, Name, Buffer.
		fSetAttribute,		// Id, Name, Value.
		fGetAttribute,		// Id, Name, Buffer.
		fRemoveAttribute,	// Id, Name.
		fGetResult,			// Id, Buffer.
		fSetContent,		// Id, Value.
		fGetContent,		// Id, Buffer.
		fFocus,				// Id.
		f_amount,
		f_Undefined
	};

	const char *GetLabel( function__ Function );

	class proxy_callback__ {
	protected:
		virtual void XHTCLLBKProcess(
			function__ Function,
			...	) = 0;
	public:
		void reset( bso::bool__ P = true )
		{
			// Standardisation.
		}		
		E_CVDTOR( proxy_callback__ );
		void Init( void )
		{
			// Standardisation.
		}
		void Log(
			const nstring___ &Message )
		{
			XHTCLLBKProcess( fLog, Message.Internal()() );
		}
		void Alert(
			const nstring___ &XML,
			const nstring___ &XSL,
			const nstring___ &Title )
		{
			XHTCLLBKProcess( fAlert, XML.Internal()(), XSL.Internal()(), Title.Internal()() );
		}
		void Confirm(
			const nstring___ &XML,
			const nstring___ &XSL,
			const nstring___ &Title,
			TOL_CBUFFER___ &Buffer )
		{
			XHTCLLBKProcess( fConfirm, XML.Internal()( ), XSL.Internal()( ), Title.Internal()( ), &Buffer );
		}
		void SetChildren(
			const nstring___ &Id,
			const nstring___ &XML,
			const nstring___ &XSL )
		{
			XHTCLLBKProcess( fSetChildren, Id.Internal()( ), XML.Internal()( ), XSL.Internal()( ) );
		}
		void SetCasting(
			const nstring___ &Id,
			const nstring___ &XML,
			const nstring___ &XSL )
		{
			XHTCLLBKProcess( fSetCasting, Id.Internal()( ), XML.Internal()( ), XSL.Internal()( ) );
		}
		const char *GetProperty(
			const nstring___ &Id,
			const nstring___ &Name,
			TOL_CBUFFER___ &Buffer )
		{
			XHTCLLBKProcess( fGetProperty, Id.Internal()( ), Name.Internal()( ), &Buffer );

			return Buffer;
		}
		void SetProperty(
			const nstring___ &Id,
			const nstring___ &Name,
			const nstring___ &Value )
		{
			XHTCLLBKProcess( fSetProperty, Id.Internal()( ), Name.Internal()( ), Value.Internal()( ) );
		}
		const char *GetAttribute(
			const nstring___ &Id,
			const nstring___ &Name,
			TOL_CBUFFER___ &Buffer )
		{
			XHTCLLBKProcess( fGetAttribute, Id.Internal()( ), Name.Internal()( ), &Buffer );

			return Buffer;
		}
		void SetAttribute(
			const nstring___ &Id,
			const nstring___ &Name,
			const nstring___ &Value )
		{
			XHTCLLBKProcess( fSetAttribute, Id.Internal()( ), Name.Internal()( ), Value.Internal()( ) );
		}
		const char *GetResult(
			const nstring___ &Id,
			TOL_CBUFFER___ &Buffer )
		{
			XHTCLLBKProcess( fGetResult, Id.Internal()( ), &Buffer );

			return Buffer;
		}
		void RemoveAttribute(
			const nstring___ &Id,
			const nstring___ &Name )
		{
			XHTCLLBKProcess( fRemoveAttribute, Id.Internal()( ), Name.Internal()( ) );
		}
		const char *GetContent(
			const nstring___ &Id,
			TOL_CBUFFER___ &Buffer )
		{
			XHTCLLBKProcess( fGetContent, Id.Internal()( ), &Buffer );

			return Buffer;
		}
		void SetContent(
			const nstring___ &Id,
			const nstring___ &Value )
		{
			XHTCLLBKProcess( fSetContent, Id.Internal()( ), Value.Internal()( ) );
		}
		void Focus( const nstring___ &Id )
		{
			XHTCLLBKProcess( fFocus, Id.Internal()( ) );
		}
# if 0
		void ExecuteJavascript( const nstring___ &Script )
		{
		ERRProlog
			TOL_CBUFFER___ Buffer;
		ERRBegin
			ExecuteJavascript( Script, Buffer );
		ERRErr
		ERREnd
		ERREpilog
		}
# endif
	};

	class session_callback__
	{
	protected:
		virtual bso::bool__ XHTCLLBKLaunch(
			const char *Id,
			const char *Action ) = 0;	// Return 'true' if the event propagation had to be stopped.
		virtual const char *XHTCLLBKLanguage( TOL_CBUFFER___ &Buffer ) = 0;
	public:
		void reset( bso::bool__ = true )
		{
			// Standardisation.
		}
		E_CVDTOR( session_callback__ );
		void Init( void )
		{
			// Standardisation.
		}
		bso::bool__ Launch(
			const char *Id,
			const char *Action )
		{
			return XHTCLLBKLaunch( Id, Action );
		}
		const char *Language( TOL_CBUFFER___ &Buffer )
		{
			return XHTCLLBKLanguage( Buffer );
		}
	};

#pragma pack( push, 1)
		// NOTA : Si modifi, modifier 'CSDLEO_SHARED_DATA_VERSION' !
	class shared_data__
	{
	private:
		const char *_Version;	// Toujours en premire position.
		bso::uint__ _Control;	// Une valeur relative au contenu de la structure,  des fins de test primaire de compatibilit.
		err::err___ *_ERRError;
		sclerror::error___ *_SCLError;
		const cio::set__ *_CIO;
		proxy_callback__ *_Callback;
		const char *_LauncherIdentification;
		const char *_Localization;
	public:
		void reset( bso::bool__ = true )
		{
			_Version = NULL;
			_Control = 0;
			_ERRError = NULL;
			_SCLError = NULL;
			_CIO = NULL;
			_Callback = NULL;
			_LauncherIdentification = NULL;
			_Localization = NULL;
		}
		E_CDTOR( shared_data__ );
		void Init(
			proxy_callback__ &Callback,
			const char *LauncherIdentification,
			const char *Localization )
		{
			_Version = XHTCLLBK_SHARED_DATA_VERSION;
			_Control = ControlComputing();
			_ERRError = err::ERRError;
			_SCLError = sclerror::SCLERRORError;
			_CIO = &cio::GetCurrentSet();
			_Callback = &Callback;
			_LauncherIdentification = LauncherIdentification;
			_Localization = Localization;
		}
		size_t ControlComputing( void )
		{
			return sizeof( shared_data__ );
		}
		Q37_PMDF( err::err___, ERRError, _ERRError );
		Q37_PMDF( sclerror::error___, SCLError, _SCLError );
		Q37_PMDF( proxy_callback__, Callback, _Callback );
		Q37_PMDF( const char, LauncherIdentification, _LauncherIdentification );
		Q37_PMDF( const char, Localization, _Localization );
		Q37_RMDF( const cio::set__, CIO, _CIO );
	};
#pragma pack( pop )

	class callback__
	{
	protected:
		virtual void XHTCLLBKInitialize( const shared_data__ &Data ) = 0;
		virtual void XHTCLLBKBaseLanguage( TOL_CBUFFER___ &Buffer ) = 0;
		virtual session_callback__ *XHTCLLBKNew( void ) = 0;
	public:
		void reset( bso::bool__ = true )
		{
			// Standardisation.
	}
		E_CVDTOR( callback__ );
		void Init( void )
		{
			// Standardisation.
		}
		void Initialize( const shared_data__ &Data )
		{
			return XHTCLLBKInitialize( Data );
		}
		const char *BaseLanguage( TOL_CBUFFER___ &Buffer )
		{
			XHTCLLBKBaseLanguage( Buffer );

			return Buffer;
		}
		session_callback__ *New( void )
		{
			return XHTCLLBKNew();
		}
	};

	typedef callback__ *(retrieve)( void );

	void Escape(
		const str::string_ &Source,
		str::string_ &Target,
		bso::char__ Delimiter,	// Devrait tre '\'', '"' ou 0. Si 0, chappe '\'' et '\"', sinon chappe 'Delimiter'.
		bso::char__ EscapeChar = strmrg::DefaultEscapeToken );
#if 0
	void Unescape(
		const str::string_ &Source,
		str::string_ &Target,
		bso::char__ EscapeChar = DefaultEscapeChar );
#endif

	typedef strmrg::table_ args_;
	E_AUTO( args );

	using strmrg::Split;
	using strmrg::Merge;

	using strmrg::retriever__;

}

				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
