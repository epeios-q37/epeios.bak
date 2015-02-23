/*
	'xhtcllbk.h' by Claude SIMON (http://zeusw.org/).

	'xhtcllbk' is part of the Epeios framework.

    The Epeios framework is free software: you can redistribute it and/or
	modify it under the terms of the GNU General Public License as published
	by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Epeios framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The Epeios framework.  If not, see <http://www.gnu.org/licenses/>.
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

# define XHTCLLBK_SHARED_DATA_VERSION_NUMBER	"6"

# define XHTCLLBK_SHARED_DATA_VERSION	XHTCLLBK_SHARED_DATA_VERSION_NUMBER "-" CPE_ARCHITECTURE_LABEL

# define XHTCLLBK_RETRIEVE_FUNCTION_NAME		XHTCLLBKRetrieve

namespace xhtcllbk {
	typedef ntvstr::char__ nchar__;
	typedef ntvstr::string___ nstring___;

	enum function__ {		// Paramètres. 'Buffer' est un 'TOL_CBUFFER___' destiné à receuillir la valeur de retour.
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

	class upstream_callback__ {
	private:
		// Tout ce qui esgt 'Err' sert à transfèrer les rapports d'erreurs entre bibliothèque dynamiques.
		// Les fonctions virtuelles sont en effet appelées à partir d'une bibliothèque dynamique, mais executées dans une autre,
		// et comme les données relatives à la gestion des erreurs sont propres à chaque bibliothèque...
		const char *_ErrFile;
		int _ErrLine;
		err::type _ErrType;
		void _ResetErr( void )
		{
			_ErrFile = NULL;
			_ErrLine = 0;
			_ErrType = err::t_Undefined;
		}
		void _TestErr( void )
		{
			if ( _ErrType != err::t_Undefined ) {
				 err::ERR.Set( _ErrFile, _ErrLine, _ErrType );
				_ResetErr();
				ERRT();
			}
		}
	protected:
		virtual void XHTCLLBKLaunch(
			function__ Function,
			...	) = 0;
		void TransferError( )
		{
			if ( _ErrType != err::t_Undefined )
				ERRFwk();

			if ( err::ERR.Type == err::t_Undefined )
				ERRFwk();

			_ErrFile = err::ERR.File;
			_ErrLine = err::ERR.Line;
			_ErrType = err::ERR.Type;

			ERRRst();
		}
		virtual void XHTCLLBKGetLanguage(
			TOL_CBUFFER___ &Buffer ) = 0;
		virtual void XHTCLLBKExecuteJavascript(
			const nchar__ *Script,
			TOL_CBUFFER___ &Buffer ) = 0;
	public:
		void reset( bso::bool__ P = true )
		{
			_ResetErr();
		}		
		E_CVDTOR( upstream_callback__ );
		void Init( void )
		{
			_ResetErr();
		}
		const char *GetLanguage( TOL_CBUFFER___ &Buffer )
		{
			XHTCLLBKGetLanguage( Buffer );

			_TestErr();

			return Buffer;
		}
		const char *ExecuteJavascript(
			const nstring___ &Script,
			TOL_CBUFFER___ &Buffer )
		{
			XHTCLLBKExecuteJavascript( Script.Internal(), Buffer );

			_TestErr();

			return Buffer;
		}
		void Alert(
			const nstring___ &XML,
			const nstring___ &XSL,
			const nstring___ &Title )
		{
			XHTCLLBKLaunch( fAlert, XML.Internal()(), XSL.Internal()(), Title.Internal()() );

			_TestErr();
		}
		void Confirm(
			const nstring___ &XML,
			const nstring___ &XSL,
			const nstring___ &Title,
			TOL_CBUFFER___ &Buffer )
		{
			XHTCLLBKLaunch( fConfirm, XML.Internal()(), XSL.Internal()(), Title.Internal()(), &Buffer );

			_TestErr();
		}
		void SetChildren(
			const nstring___ &Id,
			const nstring___ &XML,
			const nstring___ &XSL )
		{
			XHTCLLBKLaunch( fSetChildren, Id.Internal()(), XML.Internal()(), XSL.Internal()() );

			_TestErr();
		}
		void SetCasting(
			const nstring___ &Id,
			const nstring___ &XML,
			const nstring___ &XSL )
		{
			XHTCLLBKLaunch( fSetCasting, Id.Internal()(), XML.Internal()(), XSL.Internal()() );

			_TestErr();
		}
		const char *GetProperty(
			const nstring___ &Id,
			const nstring___ &Name,
			TOL_CBUFFER___ &Buffer )
		{
			XHTCLLBKLaunch( fGetProperty, Id.Internal()(), Name.Internal()(), &Buffer );

			_TestErr();

			return Buffer;
		}
		void SetProperty(
			const nstring___ &Id,
			const nstring___ &Name,
			const nstring___ &Value )
		{
			XHTCLLBKLaunch( fSetProperty, Id.Internal()(), Name.Internal()(), Value.Internal()() );

			_TestErr();
		}
		const char *GetAttribute(
			const nstring___ &Id,
			const nstring___ &Name,
			TOL_CBUFFER___ &Buffer )
		{
			XHTCLLBKLaunch( fGetAttribute, Id.Internal()(), Name.Internal()(), &Buffer );

			_TestErr();

			return Buffer;
		}
		void SetAttribute(
			const nstring___ &Id,
			const nstring___ &Name,
			const nstring___ &Value )
		{
			XHTCLLBKLaunch( fSetAttribute, Id.Internal()(), Name.Internal()(), Value.Internal()() );

			_TestErr();
		}
		const char *GetResult(
			const nstring___ &Id,
			TOL_CBUFFER___ &Buffer )
		{
			XHTCLLBKLaunch( fGetResult, Id.Internal()(), &Buffer );

			_TestErr();

			return Buffer;
		}
		void RemoveAttribute(
			const nstring___ &Id,
			const nstring___ &Name )
		{
			XHTCLLBKLaunch( fRemoveAttribute, Id.Internal()(), Name.Internal()() );

			_TestErr();
		}
		const char *GetContent(
			const nstring___ &Id,
			TOL_CBUFFER___ &Buffer )
		{
			XHTCLLBKLaunch( fGetContent, Id.Internal()(), &Buffer );

			_TestErr();

			return Buffer;
		}
		void SetContent(
			const nstring___ &Id,
			const nstring___ &Value )
		{
			XHTCLLBKLaunch( fSetContent, Id.Internal()(), Value.Internal()() );

			_TestErr();
		}
		void Focus( const nstring___ &Id )
		{
			XHTCLLBKLaunch( fFocus, Id.Internal()() );

			_TestErr();
		}
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
	};

	class session_callback__
	{
	protected:
		virtual bso::bool__ XHTCLLBKLaunch(
			const char *Id,
			const char *Action ) = 0;	// Return 'true' if the event propagation had to be stopped.
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
	};

	class downstream_callback__
	{
	protected:
		virtual void XHTCLLBKOnLoad( const char *Launcher ) = 0;
		virtual void XHTCLLBKBaseLanguage( TOL_CBUFFER___ &Buffer ) = 0;
		virtual session_callback__ *XHTCLLBKNew( upstream_callback__ &Callback ) = 0;
		// Destruction by destructor member.
		virtual void XHTCLLBKOnUnload( void ) = 0;
	public:
		void reset( bso::bool__ = true )
		{
			// Standardisation.
		}
		E_CVDTOR( downstream_callback__ );
		void Init( void )
		{
			// Standardisation.
		}
		void OnLoad( const char *Launcher )
		{
			return XHTCLLBKOnLoad( Launcher );
		}
		const char *BaseLanguage( TOL_CBUFFER___ &Buffer )
		{
			XHTCLLBKBaseLanguage( Buffer );

			return Buffer;
		}
		session_callback__ *New( upstream_callback__ &Callback )
		{
			return XHTCLLBKNew( Callback );
		}
		void OnUnload( void )
		{
			return XHTCLLBKOnUnload();
		}
	};

#pragma pack( push, 1)
		// NOTA : Si modifié, modifier 'CSDLEO_SHARED_DATA_VERSION' !
	class shared_data__
	{
	private:
		const char *_Version;	// Toujours en première position.
		bso::uint__ _Control;	// Une valeur relative au contenu de la structure, à des fins de test primaire de compatibilité.
		upstream_callback__ *_Callback;
		const char *_LauncherIdentification;
		fdr::oflow_driver_base___ *_OFlowDriver;
	public:
		void reset( bso::bool__ = true )
		{
			_Version = NULL;
			_Control = 0;
			_Callback = NULL;
		}
		E_CDTOR( shared_data__ );
		void Init(
			upstream_callback__ &Callback,
			const char *LauncherIndetification,
			fdr::oflow_driver_base___ &OFlowDriver )
		{
			_Version = XHTCLLBK_SHARED_DATA_VERSION;
			_Control = ControlComputing();
			_Callback = &Callback;
			_LauncherIdentification = LauncherIndetification;
			_OFlowDriver = &OFlowDriver;
		}
		size_t ControlComputing( void )
		{
			return sizeof( shared_data__ );
		}
		upstream_callback__ &Callback( void ) const
		{
			if ( _Callback == NULL )
				ERRFwk();

			return *_Callback;
		}
		const char *LauncherIdentification( void ) const
		{
			if ( _LauncherIdentification == NULL )
				ERRFwk();

			return _LauncherIdentification;
		}
		fdr::oflow_driver_base___ &OFlowDriver( void ) const
		{
			if ( _OFlowDriver == NULL )
				ERRFwk();

			return *_OFlowDriver;
		}
	};
#pragma pack( pop )

	void Escape(
		const str::string_ &Source,
		str::string_ &Target,
		bso::char__ Delimiter,	// Devrait être '\'', '"' ou 0. Si 0, échappe '\'' et '\"', sinon échappe 'Delimiter'.
		bso::char__ EscapeChar = strmrg::DefaultEscapeToken );
#if 0
	void Unescape(
		const str::string_ &Source,
		str::string_ &Target,
		bso::char__ EscapeChar = DefaultEscapeChar );
#endif
	typedef downstream_callback__ *(retrieve)( const shared_data__ &Data );

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
