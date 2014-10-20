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

# define XHTCLLBK_SHARED_DATA_VERSION_NUMBER	"1"

# define XHTCLLBK_SHARED_DATA_VERSION	XHTCLLBK_SHARED_DATA_VERSION_NUMBER "-" CPE_ARCHITECTURE_LABEL

# define XHTCLLBK_RETRIEVE_FUNCTION_NAME		XHTCLLBKRetrieve


namespace xhtcllbk {
	class event_handler__
	{
	protected:
		virtual void XHTCLLBKHandle( void ) = 0;
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
			XHTCLLBKHandle();
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
		void plug( ags::E_ASTORAGE_ &AS )
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

			if ( Row == E_NIL )
				return NULL;

			return Handlers( Row );
		}
	};

	E_AUTO( event_handlers );

	class upstream_callback__ {
	protected:
		virtual void XHTCLLBKSetChildren(
			const char *Id,
			const char *XML,
			const char *XSL ) = 0;
		virtual void XHTCLLBKExecuteJavascript( const char *Script ) = 0;
		virtual void XHTCLLBKSet(
			const char *Id,
			const char *Name,
			const char *Value ) = 0;
		virtual void XHTCLLBKGet(
			const char *Id,
			const char *Name,
			TOL_CBUFFER___ &Buffer ) = 0;
		virtual void XHTCLLBKRemove(
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
			XHTCLLBKSetChildren( Id, XML, XSL );
		}
		void ExecuteJavascript( const char *Script )
		{
			XHTCLLBKExecuteJavascript( Script );
		}
		const char *Get(
			const char *Id,
			const char *Name,
			TOL_CBUFFER___ &Buffer )
		{
			XHTCLLBKGet( Id, Name, Buffer );

			return Buffer;
		}
		void Set(
			const char *Id,
			const char *Name,
			const char *Value )
		{
			XHTCLLBKSet( Id, Name, Value );
		}
		void Remove(
			const char *Id,
			const char *Name )
		{
			XHTCLLBKRemove( Id, Name );
		}
	};

	class downstream_callback__
	{
	private:
		const event_handlers_ *_Handlers;
		const event_handlers_ &_H( void ) const
		{
			if ( _Handlers ==  NULL )
				ERRFwk();

			return *_Handlers;
		}
	protected:
		virtual void XHTCLLBKStart( void ) = 0;
		virtual const char *XHTCLLBKLanguage( void ) = 0;
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
		void Start( void )
		{
			return XHTCLLBKStart();
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
		virtual void Handle( const char *EventName )
		{
			event_handler__ *Handler = _H().Get( str::string(  EventName ) );

			if ( Handler == NULL )
				ERRFwk();

			Handler->Handle();
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
			_Version = XHTCLLBK_SHARED_DATA_VERSION;
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
				ERRFwk();

			return *_Callback;
		}
	};
#pragma pack( pop )

	typedef downstream_callback__ *(retrieve)( const shared_data__ &Data );

	void EscapeQuotes(
		const str::string_ &Source,
		str::string_ &Target );


}

				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
