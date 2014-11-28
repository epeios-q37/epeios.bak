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
# include "stsfsm.h"
# include "xtf.h"
# include "strmrg.h"

# define XHTCLLBK_SHARED_DATA_VERSION_NUMBER	"4"

# define XHTCLLBK_SHARED_DATA_VERSION	XHTCLLBK_SHARED_DATA_VERSION_NUMBER "-" CPE_ARCHITECTURE_LABEL

# define XHTCLLBK_RETRIEVE_FUNCTION_NAME		XHTCLLBKRetrieve

# define XHTCLLBK_ATRIIBUTE_PREFIX	"data-q37-"

namespace xhtcllbk {
	E_CDEF( char *, EventAttribute, XHTCLLBK_ATRIIBUTE_PREFIX "event" );
	E_CDEF( char *, PaddingAttribute, XHTCLLBK_ATRIIBUTE_PREFIX "padding" );
	E_CDEF( char *, ParamsAttribute, XHTCLLBK_ATRIIBUTE_PREFIX "params" );
	E_CDEF( char *, ResultAttribute, XHTCLLBK_ATRIIBUTE_PREFIX "result" );

	typedef ntvstr::char__ nchar__;
	typedef ntvstr::string___ nstring___;

	E_ROW( token__ );

	E_CDEF( token__, UndefinedToken, sdr::NIL );

	class event_handler__
	{
	protected:
		virtual void XHTCLLBKHandle( const char *Id ) = 0;
	public:
		void reset( bso::bool__ = true )
		{
			// Standardisation.
		}
		E_CVDTOR( event_handler__ );
		void Init( void )
		{
			// Standardisation.
		}
		void Handle( const char *Id )
		{
			XHTCLLBKHandle( Id );
		}
	};

	E_ROW( hrow__ );	// (event) handler row;

	typedef bch::E_BUNCHt_( event_handler__ *, hrow__ ) _event_handlers_;

	class event_manager_
	{
	public:
		struct s {
			stsfsm::automat_::s Automat;
			_event_handlers_::s Handlers;
		};
		stsfsm::automat_ Automat;
		_event_handlers_ Handlers;
		event_manager_( s &S )
		: Automat( S.Automat ),
		  Handlers( S.Handlers )
		{}
		void reset( bso::bool__ P = true )
		{
			Automat.reset( P );
			Handlers.reset( P );
		}
		void plug( ags::E_ASTORAGE_ &AS )
		{
			Automat.plug( AS );
			Handlers.plug( AS );
		}
		event_manager_ &operator =(const event_manager_ &EM)
		{
			Automat = EM.Automat;
			Handlers = EM.Handlers;

			return *this;
		}
		void Init( void )
		{
			Automat.Init();
			Handlers.Init();
		}
		bso::bool__ Add(
			const char *Name,
			event_handler__ &Handler )
		{
			return stsfsm::Add( Name, *Handlers.Append( &Handler ), Automat ) == stsfsm::UndefinedId;
		}
		event_handler__ *Get( const str::string_ &Name ) const
		{
			hrow__ Row = stsfsm::GetId( Name, Automat );

			if ( Row == E_NIL )
				return NULL;

			return Handlers( Row );
		}
	};

	E_AUTO( event_manager );

	class upstream_callback__ {
	protected:
		virtual void XHTCLLBKGetLanguage(
			token__ Token,
			TOL_CBUFFER___ &Buffer ) = 0;
		virtual void XHTCLLBKExecuteJavascript(
			token__ Token,
			const nchar__ *Script,
			TOL_CBUFFER___ &Buffer ) = 0;
		virtual void XHTCLLBKOpenDialog(
			token__ Token,
			const nchar__ *XML,
			const nchar__ *XSL,
			TOL_CBUFFER___ &Buffer ) = 0;
		virtual void XHTCLLBKSetChildren(
			token__ Token,
			const nchar__ *Id,
			const nchar__ *XML,
			const nchar__ *XSL ) = 0;
		virtual void XHTCLLBKSetPaddings(
			token__ Token,
			const nchar__ *XML,
			const nchar__ *XSL ) = 0;
		virtual void XHTCLLBKSetProperty(
			token__ Token,
			const nchar__ *Id,
			const nchar__ *Name,
			const nchar__ *Value ) = 0;
		virtual void XHTCLLBKGetProperty(
			token__ Token,
			const nchar__ *Id,
			const nchar__ *Name,
			TOL_CBUFFER___ &Buffer ) = 0;
		virtual void XHTCLLBKSetAttribute(
			token__ Token,
			const nchar__ *Id,
			const nchar__ *Name,
			const nchar__ *Value ) = 0;
		virtual void XHTCLLBKGetAttribute(
			token__ Token,
			const nchar__ *Id,
			const nchar__ *Name,
			TOL_CBUFFER___ &Buffer ) = 0;
		virtual void XHTCLLBKRemoveAttribute(
			token__ Token,
			const nchar__ *Id,
			const nchar__ *Name ) = 0;
		virtual void XHTCLLBKGetSelectValue(
			token__ Token,
			const nchar__ *Id,
			TOL_CBUFFER___ &Buffer ) = 0;
	public:
		void reset( bso::bool__ P = true )
		{
			//Standardisation.
		}		
		E_CVDTOR( upstream_callback__ );
		void Init( void )
		{
			//Standardisation.
		}
		const char *GetLanguage(
			token__ Token,
			TOL_CBUFFER___ &Buffer )
		{
			XHTCLLBKGetLanguage( Token, Buffer );

			return Buffer;
		}
		const char *ExecuteJavascript(
			token__ Token,
			const nstring___ &Script,
			TOL_CBUFFER___ &Buffer )
		{
			XHTCLLBKExecuteJavascript( Token, Script.Internal(), Buffer );

			return Buffer;
		}
		const char *OpenDialog(
			token__ Token,
			const nstring___ &XML,
			const nstring___ &XSL,
			TOL_CBUFFER___ &Buffer )
		{
			XHTCLLBKOpenDialog( Token, XML.Internal(), XSL.Internal(), Buffer );

			return Buffer;
		}
		void SetChildren(
			token__ Token,
			const nstring___ &Id,
			const nstring___ &XML,
			const nstring___ &XSL )
		{
			XHTCLLBKSetChildren( Token, Id.Internal(), XML.Internal(), XSL.Internal() );
		}
		void SetPaddings(
			token__ Token,
			const nstring___ &XML,
			const nstring___ &XSL )
		{
			XHTCLLBKSetPaddings( Token, XML.Internal(), XSL.Internal() );
		}
		void ExecuteJavascript(
			token__ Token,
			const nstring___ &Script )
		{
		ERRProlog
			TOL_CBUFFER___ Buffer;
		ERRBegin
			ExecuteJavascript( Token, Script, Buffer );
		ERRErr
		ERREnd
		ERREpilog
		}
		const char *GetProperty(
			token__ Token,
			const nstring___ &Id,
			const nstring___ &Name,
			TOL_CBUFFER___ &Buffer )
		{
			XHTCLLBKGetProperty( Token, Id.Internal(), Name.Internal(), Buffer );

			return Buffer;
		}
		void SetProperty(
			token__ Token,
			const nstring___ &Id,
			const nstring___ &Name,
			const nstring___ &Value )
		{
			XHTCLLBKSetProperty( Token, Id.Internal(), Name.Internal(), Value.Internal() );
		}
		const char *GetAttribute(
			token__ Token,
			const nstring___ &Id,
			const nstring___ &Name,
			TOL_CBUFFER___ &Buffer )
		{
			XHTCLLBKGetAttribute( Token, Id.Internal(), Name.Internal(), Buffer );

			return Buffer;
		}
		void SetAttribute(
			token__ Token,
			const nstring___ &Id,
			const nstring___ &Name,
			const nstring___ &Value )
		{
			XHTCLLBKSetAttribute( Token, Id.Internal(), Name.Internal(), Value.Internal() );
		}
		void RemoveAttribute(
			token__ Token,
			const nstring___ &Id,
			const nstring___ &Name )
		{
			XHTCLLBKRemoveAttribute( Token, Id.Internal(), Name.Internal() );
		}
		const char *GetSelectValue(
			token__ Token,
			const nstring___ &Id,
			TOL_CBUFFER___ &Buffer )
		{
			XHTCLLBKGetSelectValue( Token, Id.Internal(), Buffer );

			return Buffer;
		}
	};

	class downstream_callback__
	{
	private:
		const event_manager_ *_Manager;
		const event_manager_ &_M( void ) const
		{
			if ( _Manager ==  NULL )
				ERRFwk();

			return *_Manager;
		}
	protected:
		virtual void XHTCLLBKStart( void ) = 0;
		virtual const char *XHTCLLBKLanguage( void ) = 0;
		virtual bso::bool__ XHTCLLBKOnClose( void ) = 0;
	public:
		void reset( bso::bool__ P = true )
		{
			_Manager = NULL;
		}
		E_CVDTOR( downstream_callback__ )
		void Init( const event_manager_ &Manager)
		{
			_Manager = &Manager;
		}
		void Start( void )
		{
			return XHTCLLBKStart();
		}
		bso::bool__ OnClose( void )
		{
			return XHTCLLBKOnClose();
		}
		const char *Language( void )
		{
			return XHTCLLBKLanguage();
		}
		/*
			Déclaré en 'vritual' pour s'assurer que cette méthode est bien exécutée dans le même contexte
			que lors de la création de l'instance de cet objet. Cet objet est destiné à être instancié à partir
			d'une bilbiothèque dynamique, mais la méthode ci-dessous est destinée à être lancée par l'exécutable
			ayant chargé la	bibliothèque. Or, le compilateur utilisé en amont peut ne pas être le même/ne pas
			avoir été lancé avec les mêmes options que le compilateur aval.
		*/
		// 'const char *' pour avoir une strucutre de données simple, car donnée passée entre codes compilés séparément.
		virtual void Handle(
			const char *EventName,
			const char *Id )
		{
			event_handler__ *Handler = _M().Get( str::string(  EventName ) );

			if ( Handler == NULL )
				ERRFwk();

			Handler->Handle( Id );
		}
	};

#pragma pack( push, 1)
		// NOTA : Si modifié, modifier 'CSDLEO_SHARED_DATA_VERSION' !
	class shared_data__
	{
	private:
		const char *_Version;	// Toujours en première position.
		bso::uint__ _Control;	// Une valeur relative au contenu de la structure, à des fins de test primaire de compatibilité.
		token__ _Token;
		upstream_callback__ *_Callback;
		const char *_LauncherIdentification;
	public:
		void reset( bso::bool__ = true )
		{
			_Version = NULL;
			_Control = 0;
			_Token = UndefinedToken;
			_Callback = NULL;
		}
		E_CDTOR( shared_data__ );
		void Init(
			token__ Token,
			upstream_callback__ &Callback,
			const char *LauncherIndetification )
		{
			_Version = XHTCLLBK_SHARED_DATA_VERSION;
			_Control = ControlComputing();
			_Token = Token;
			_Callback = &Callback;
			_LauncherIdentification = LauncherIndetification;
		}
		size_t ControlComputing( void )
		{
			return sizeof( fdr::oflow_driver___<> );
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
		E_RODISCLOSE__( token__, Token );
	};
#pragma pack( pop )

	E_CDEF( char, DefaultEntrySeparator, '|' );
	E_CDEF( char, DefaultFieldSeparator, '%' );
	E_CDEF( char, DefaultEscapeChar, '\\' );

	void Escape(
		const str::string_ &Source,
		str::string_ &Target,
		bso::char__ EscapeChar = DefaultEscapeChar );
#if 0
	void Unescape(
		const str::string_ &Source,
		str::string_ &Target,
		bso::char__ EscapeChar = DefaultEscapeChar );
#endif
	typedef downstream_callback__ *(retrieve)( const shared_data__ &Data );

	typedef strmrg::table_ params_;
	E_AUTO( params );

	inline bso::bool__ Split(
		const str::string_ &Input,
		params_ &Params,
		bso::char__ EntrySeparator = DefaultEntrySeparator,
		bso::char__ FieldSeparator = DefaultFieldSeparator,
		bso::char__ EscapeChar = DefaultEscapeChar )
	{
		return strmrg::Split( Input, Params, EntrySeparator, FieldSeparator, EscapeChar );
	}

	inline void Merge(
		const params_ &Params,
		str::string_ &Target,
		bso::char__ EntrySeparator = DefaultEntrySeparator,
		bso::char__ FieldSeparator = DefaultFieldSeparator,
		bso::char__ EscapeChar = DefaultEscapeChar )
	{
		return strmrg::Merge( Params, Target, EntrySeparator, FieldSeparator, EscapeChar );
	}


	using strmrg::retriever__;

}

				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
