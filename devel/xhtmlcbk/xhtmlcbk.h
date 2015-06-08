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

#ifndef XHTMLCBK__INC
# define XHTMLCBK__INC

# define XHTMLCBK_NAME		"XHTMLCBK"

# if defined( E_DEBUG ) && !defined( XHTMLCBK_NODBG )
#  define XHTMLCBK_DBG
# endif

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

// XHTML CallBacKs

# error "Obsolete ! Use 'xhtcllbk' instead."

# include "err.h"
# include "flw.h"
# include "tol.h"
# include "sdr.h"
# include "bch.h"
# include "stsfsm.h"

# define XHTMLCBK_SHARED_DATA_VERSION_NUMBER	"1"

# define XHTMLCBK_SHARED_DATA_VERSION	XHTMLCBK_SHARED_DATA_VERSION_NUMBER "-" CPE_ARCHITECTURE_LABEL

# define XHTMLCBK_LAUNCH_FUNCTION_NAME		XHTMLCBKLaunch


namespace xhtmlcbk {
	class event_handler__
	{
		virtual void XHTLCBKHandle( void ) = 0;
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
		void Handle( void )
		{
			XHTLCBKHandle();
		}
	};

	E_ROW( hrow__ );	// (event) handler row;

	typedef bch::E_BUNCHt_( event_handler__ *, hrow__ ) _handlers_;

	class event_handlers_
	{
	public:
		struct s {
			stsfsm::automat_::s Automat;
			_handlers_::s Handlers;
		};
		stsfsm::automat_ Automat;
		_handlers_ Handlers;
		event_handlers_( s &S )
		: Automat( S.Automat ),
		  Handlers( S.Handlers )
		{}
		void reset( bso::bool__ P = true )
		{
			Automat.reset( P );
			Handlers.reset( P );
		}
		void plug( qAS_ &AS )
		{
			Automat.plug( AS );
			Handlers.plug( AS );
		}
		event_handlers_ &operator =(const event_handlers_ &EH)
		{
			Automat = EH.Automat;
			Handlers = EH.Handlers;

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

			if ( Row == qNIL )
				return NULL;

			return Handlers( Row );
		}
	};

	E_AUTO( event_handlers );

	class upstream_callback__ {
	protected:
		virtual void WKCLLBCKSetChildren(
			const char *Id,
			const char *XML,
			const char *XSL ) = 0;
		virtual void WKCLLBCKExecuteJavascript(
			const char *Javascript,
			TOL_CBUFFER___ &Buffer ) = 0;
		virtual void WKCLLBCKSet(
			const char *Id,
			const char *Name,
			const char *Value ) = 0;
		virtual void WKCLLBCKGet(
			const char *Id,
			const char *Name,
			TOL_CBUFFER___ &Buffer ) = 0;
		virtual void WKCLLBCKRemove(
			const char *Id,
			const char *Name ) = 0;
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
		void SetChildren(
			const char *Id,
			const char *XML,
			const char *XSL )
		{
			WKCLLBCKSetChildren( Id, XML, XSL );
		}
		const char *ExecuteJavascript(
			const char *Script,
			TOL_CBUFFER___ &Buffer )
		{
			WKCLLBCKExecuteJavascript( Script, Buffer );

			return Buffer;
		}
		const char *Get(
			const char *Id,
			const char *Name,
			TOL_CBUFFER___ &Buffer )
		{
			WKCLLBCKGet( Id, Name, Buffer );

			return Buffer;
		}
		void Set(
			const char *Id,
			const char *Name,
			const char *Value )
		{
			WKCLLBCKSet( Id, Name, Value );
		}
		void Remove(
			const char *Id,
			const char *Name )
		{
			WKCLLBCKRemove( Id, Name );
		}
	};

	/*
		Pas vraiment un 'callback' ; le contenu des mthodes virtuelles est connu  ce point-ci,
		et ne seront jamais surcharges par un classe mre, mais c'est pour s'assurer que les objets
		cres en aval soient manipuls en aval, le compilateur amont ou ses options pouvant ne pas
		tre les mmes qu'en aval.
	*/
	class downstream_callback__
	{
	private:
		const event_handlers_ *_Handlers;
		const event_handlers_ &_H( void ) const
		{
			if ( _Handlers ==  NULL )
				qRFwk();

			return *_Handlers;
		}
	protected:
		virtual void WKCLLBCKHandle( const char *EventName )
		{
			xhtmlcbk::event_handler__ *Handler = _H().Get( str::string(  EventName ) );

			if ( Handler == NULL )
				qRFwk();

			Handler->Handle();
		}
	public:
		void reset( bso::bool__ P = true )
		{
			_Handlers = NULL;
		}
		E_CVDTOR( downstream_callback__ )
		void Init( const event_handlers_ &Handlers)
		{
			_Handlers = &Handlers;
		}
		void Handle( const char *EventName )
		{
			WKCLLBCKHandle( EventName );
		}
	};

#pragma pack( push, 1)
		// NOTA : Si modifi, modifier 'CSDLEO_SHARED_DATA_VERSION' !
	class shared_data__
	{
	private:
		const char *_Version;	// Toujours en premire position.
		bso::uint__ _Control;	// Une valeur relative au contenu de la structure,  des fins de test primaire de compatibilit.
		upstream_callback__ *_Callback;
	public:
		void reset( bso::bool__ = true )
		{
			_Version = NULL;
			_Control = 0;
			_Callback = NULL;
		}
		E_CDTOR( shared_data__ );
		void Init( upstream_callback__ &Callback )
		{
			_Version = XHTMLCBK_SHARED_DATA_VERSION;
			_Control = ControlComputing();
			_Callback = &Callback;
		}
		size_t ControlComputing( void )
		{
			return sizeof( fdr::oflow_driver___<> );
		}
		upstream_callback__ &Callback( void ) const
		{
			if ( _Callback == NULL )
				qRFwk();

			return *_Callback;
		}
	};
#pragma pack( pop )

	typedef downstream_callback__ *(launch)( const shared_data__ &Data );
}

				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
