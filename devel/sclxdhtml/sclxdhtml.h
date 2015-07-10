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

#ifndef SCLXDHTML__INC
# define SCLXDHTML__INC

# define SCLXDHTML_NAME		"SCLXDHTML"

# if defined( E_DEBUG ) && !defined( SCLXDHTML_NODBG )
#  define SCLXDHTML_DBG
# endif

// SoCLe X(SL)/DH(TML)

# include "xdhdws.h"

# include "sclfrntnd.h"

# include "sclrgstry.h"
# include "sclmisc.h"

# include "err.h"

# define SCLXDHTML_DEFAULT_SUFFIX "xdh"

namespace sclxdhtml {
	const sclrgstry::registry_ &GetRegistry( void );

	const char *GetLauncher( void );

	template <typename session> class action_callback__
	{
	private:
		const char *_Name;
	protected:
		virtual bso::bool__ SCLXDHTMLLaunch(
			session &Session,
			const char *Id ) = 0;
	public:
		void reset( bso::bool__ = true )
		{
			_Name = NULL;
		}
		E_CVDTOR( action_callback__ );
		void Init( const char *Name )
		{
			_Name = Name;
		}
		bso::bool__ Launch(
			session &Session,
			const char *Id )
		{
			return SCLXDHTMLLaunch( Session, Id );
		}
		const char *Name( void ) const
		{
			if ( _Name == NULL )
				qRFwk();

			return _Name;
		}
	};

	qROW( crow__ );	// callback row;

	template <typename session> E_TTCLONE_( bch::E_BUNCHt_( action_callback__<session> *, crow__ ), action_callbacks_ );

	template <typename session> class action_handler_
	{
	private:
		action_callback__<session> *_Get( const str::string_ &Action ) const
		{
			crow__ Row = stsfsm::GetId( Action, Automat );

			if ( Row == qNIL )
				return NULL;

			return Callbacks( Row );
		}
	public:
		struct s {
			stsfsm::automat_::s Automat;
			typename action_callbacks_<session>::s Callbacks;
		};
		stsfsm::automat_ Automat;
		action_callbacks_<session> Callbacks;
		action_handler_( s &S )
		: Automat( S.Automat ),
			Callbacks( S.Callbacks )
		{}
		void reset( bso::bool__ P = true )
		{
			Automat.reset( P );
			Callbacks.reset( P );
		}
		void plug( qAS_ &AS )
		{
			Automat.plug( AS );
			Callbacks.plug( AS );
		}
		action_handler_ &operator =(const action_handler_ &AH )
		{
			Automat = AH.Automat;
			Callbacks = AH.Callbacks;

			return *this;
		}
		void Init( void )
		{
			Automat.Init();
			Callbacks.Init();
		}
		bso::bool__ Add(
			const char *Name,
			action_callback__<session> &Callback )
		{
			return stsfsm::Add( Name, *Callbacks.Append( &Callback ), Automat ) == stsfsm::UndefinedId;
		}
		bso::bool__ Launch(
			session &Session,
			const char *Id,
			const char *Action )
		{
			action_callback__<session> *Callback = _Get( str::string(  Action ) );

			if ( Callback == NULL )
				qRFwk();	// L'action affecte  un vnement n'existe pas. Contrler le fichier '.xsl'.

			return Callback->Launch( Session, Id );
		}
	};

	E_AUTO1( action_handler );

	template <typename session> class action_helper_callback__
	{
	protected:
		virtual bso::bool__ SCLXHTMLOnBeforeAction(
			session &Session,
			const char *Id,
			const char *Action ) = 0;
		virtual bso::bool__ SCLXHTMLOnClose( session &Session ) = 0;
	public:
		void reset( bso::bool__ = true )
		{
			// Standardisation.
		}
		E_CVDTOR( action_helper_callback__ );
		void Init( void )
		{
			// Standardisation.
		}
		bso::bool__ OnBeforeAction(
			session &Session,
			const char *Id,
			const char *Action )
		{
			return SCLXHTMLOnBeforeAction( Session, Id, Action );
		}
		bso::bool__ OnClose( session &Session )
		{
			return SCLXHTMLOnClose( Session );
		}
	};

	typedef fblfrd::reporting_callback__ _reporting_callback__;

	typedef xdhcbk::session_callback__ _session_callback__;

	using xdhdws::proxy__;

	class reporting_callback__
	: public _reporting_callback__
	{
	private:
		Q37_MRMDF( proxy__, P_, Proxy_ );
	protected:
		virtual void FBLFRDReport(
			fblovl::reply__ Reply,
			const char *Message ) override
		{
			P_().Alert( Message );
		}
	public:
		void reset( bso::bool__ P = true )
		{
			_reporting_callback__::reset( P );
			Proxy_ = NULL;
		}
		E_CVDTOR( reporting_callback__ );
		void Init( proxy__ &Proxy )
		{
			_reporting_callback__::Init();
			Proxy_ = &Proxy;
		}
	};

	void HandleError(
		proxy__ &Proxy,
		const char *Language );

	/*********** 1 *****************/

	// L'utilisateur met dans le type 'instances' ses propres objets et instancie le tout par un 'new' (en surchargeant 'SCLXHTMLNew(...)', et il est assur qu'un 'delete' sera fait une fois la bibliothque dcharge.
	template <typename instances, typename frontend, typename page, page UndefinedPage > class session___
	: public _session_callback__,
	  public proxy__,
	  public instances,
	  public frontend
	{
	private:
		page _Page;	// Current page;
		reporting_callback__ _ReportingCallback;
	protected:
		virtual void SCLXDHTMLRefresh( page Page ) = 0;
	public:
		void reset( bso::bool__ P = true )
		{
			_session_callback__::reset( P );
			instances::reset( P );
			frontend::reset( P );
			_Page = UndefinedPage;
			_ReportingCallback.reset( P );
		}
		E_CVDTOR( session___ )
		void Init(
			const char *Language,
			xdhcbk::proxy_callback__ *Callback )
		{
			proxy__::Init( Callback );
			_ReportingCallback.Init( *this );
			frontend::Init( Language, _ReportingCallback, sclmisc::GetRegistry() );
			_session_callback__::Init();
			_Page = UndefinedPage;
			// instances::Init( *this );	// Made on connection.
		}
		bso::bool__ Connect(
			csducl::universal_client_core &ClientCore,
			const fblfrd::compatibility_informations__ &CompatibilityInformations,
			fblfrd::incompatibility_informations_ &IncompatibilityInformations )
		{
			if ( !frontend::Connect( ClientCore, CompatibilityInformations, IncompatibilityInformations ) )
				return false;

			instances::Init( *this );	// Made on connection.
			
			return true;
		}
		const char *Language( void )
		{
			return frontend::Language();
		}
		void Refresh( void )
		{
			if ( _Page == UndefinedPage )
				qRFwk();
			else
				SCLXDHTMLRefresh( _Page );
		}
		void SwitchTo( page Page = UndefinedPage )
		{
			if ( Page != UndefinedPage )
				_Page = Page;
			else
				qRFwk();
		}
		const str::string_ &GetTranslation(
			const char *Message,
			str::string_ &Translation )
		{
			return scllocale::GetTranslation( Message, Language(), Translation );
		}
	};

	/********** 2 ******************/

	// L'utilisateur met dans le type 'instances' ses propres objets et instancie le tout par un 'new' (en surchargeant 'SCLXHTMLNew(...)', et il est assur qu'un 'delete' sera fait une fois la bibliothque dcharge.
	template <typename session> class core___
	{
	private:
		action_handler<session> _Handler;
		xdhcbk::mode__ _Mode;
		Q37_MRMDF( action_helper_callback__<session>, _AH, _ActionHelperCallback );
		bso::bool__ _OnBeforeAction(
			session &Session,
			const char *Id,
			const char *Action )
		{
			return _AH().OnBeforeAction( Session, Id, Action );
		}
		bso::bool__ _OnClose( session &Session )
		{
			return _AH().OnClose( Session );
		}
	public:
		void reset( bso::bool__ P = true )
		{
			_Handler.reset( P );
			_Mode = xdhcbk::m_Undefined;
			_ActionHelperCallback = NULL;
		}
		E_CVDTOR( core___ )
		void Init(
			xdhcbk::mode__ Mode,
			action_helper_callback__<session> &ActionHelperCallback )
		{
			_ActionHelperCallback = &ActionHelperCallback;
			_Mode = Mode;
			_Handler.Init();
		}
		void AddActionCallback(
			const char *ActionName,
			action_callback__<session> &Callback )
		{
			_Handler.Add( ActionName, Callback );
		}
		bso::bool__ Launch(
			session &Session,
			const char *Id,
			const char *Action )
		{
			bso::bool__ Success = false;
		qRH
			TOL_CBUFFER___ Buffer;
		qRB
			if ( _OnBeforeAction( Session, Id, Action ) )
				if ( !strcmp( Action, xdhcbk::CloseActionLabel ) )
					Success = _OnClose( Session );	// Dans ce cas, si 'Success' est  'false', la fermeture de l'application est suspendue.
				else
					Success = _Handler.Launch( Session, Id, Action );
		qRR
			HandleError( Session, Session.Language() );
		qRT
		qRE
			return Success;
		}
		E_RODISCLOSE__( xdhcbk::mode__, Mode );
	};


	/*********************************/

	inline void LoadXSLAndTranslateTags(
		const rgstry::tentry__ &FileName,
		const sclrgstry::registry_ &Registry,
		str::string_ &String,
		bso::char__ Marker = '#' )
	{
		sclmisc::LoadXMLAndTranslateTags( FileName, Registry, String, Marker );
	}

	void LoadProject( proxy__ &Proxy );

	void LaunchProject(
		proxy__ &Proxy ,
		sclfrntnd::kernel___ &Kernel );

	void SCLXDHTMLInitialization( xdhcbk::mode__ Mode );	// To overload.
	xdhcbk::session_callback__ *SCLXDHTMLNew(
		const char *Language,
		xdhcbk::proxy_callback__ *ProxyCallback );	// To override.

}

#endif
