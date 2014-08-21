/*
	'wkcllbck.h' by Claude SIMON (http://zeusw.org/).

	'wkcllbck' is part of the Epeios framework.

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

#ifndef WKCLLBCK__INC
# define WKCLLBCK__INC

# define WKCLLBCK_NAME		"WKCLLBCK"

# if defined( E_DEBUG ) && !defined( WKCLLBCK_NODBG )
#  define WKCLLBCK_DBG
# endif

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

// WebKit CaLLBaCKs

# include "err.h"
# include "flw.h"
# include "str.h"
# include "stsfsm.h"
# include "ids.h"

# define WKCLLBCK_SHARED_DATA_VERSION_NUMBER	"1"

# define WKCLLBCK_SHARED_DATA_VERSION	WKCLLBCK_SHARED_DATA_VERSION_NUMBER "-" CPE_ARCHITECTURE_LABEL


# define WKCLLBCK_LAUNCH_FUNCTION_NAME		WKCLLBCKLaunch

namespace wkcllbck {
	// Prédéclaration.
	class upstream_callback__;

	class action_callback__
	{
	private:
		upstream_callback__ *_Callback;
	protected:
		upstream_callback__ &C( void )
		{
			if ( _Callback == NULL )
				ERRFwk();

			return *_Callback;
		}
		virtual void WKCLLBCKExecute( void ) = 0;
	public:
		void reset( bso::bool__ = true )
		{
			_Callback = NULL;
		}
		E_CVDTOR( action_callback__ );
		void Init( upstream_callback__ &Callback )
		{
			_Callback = &Callback;
		}
		void Execute( void )
		{
			WKCLLBCKExecute();
		}
	};

	E_ROW( arow__ );	// Action row;

	typedef bch::E_BUNCHt_( action_callback__ *, arow__ ) _actions_;

	class actions_
	{
	public:
		struct s {
			stsfsm::automat_::s Automat;
			_actions_::s Actions;
		};
		stsfsm::automat_ Automat;
		_actions_ Actions;
		actions_( s &S )
		: Automat( S.Automat ),
		  Actions( S.Actions )
		{}
		void reset( bso::bool__ P = true )
		{
			Automat.reset( P );
			Actions.reset( P );
		}
		void plug( ags::E_ASTORAGE_ &AS )
		{
			Automat.plug( AS );
			Actions.plug( AS );
		}
		actions_ &operator =(const actions_ &A)
		{
			Automat = A.Automat;
			Actions = A.Actions;

			return *this;
		}
		void Init( void )
		{
			Automat.Init();
			Actions.Init();
		}
		bso::bool__ Add(
			const char *Name,
			action_callback__ &Callback )
		{
			return stsfsm::Add( Name, *Actions.Append( &Callback ), Automat ) == stsfsm::UndefinedId;
		}
		action_callback__ *Get( const str::string_ &Name ) const
		{
			arow__ Row = stsfsm::GetId( Name, Automat );

			if ( Row == E_NIL )
				return NULL;

			return Actions( Row );
		}
	};

	E_AUTO( actions );

	class upstream_callback__ {
	protected:
		virtual void WKCLLBCKSetChildren(
			const str::string_ &Id,
			const str::string_ &XML,
			const str::string_ &XSL ) = 0;
		virtual void WKCLLBCKExecuteJavascript( const str::string_ &Javascript ) = 0;
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
			const str::string_ &Id,
			const str::string_ &XML,
			const str::string_ &XSL )
		{
			WKCLLBCKSetChildren( Id, XML, XSL );
		}
		void ExecuteJavascript( const str::string_ &Script )
		{
			WKCLLBCKExecuteJavascript( Script );
		}
	};

	/*
		Pas vraiment un 'callback' ; le contenu des méthodes virtuelles est connu à ce pont-ci (void classe suivante),
		mais c'est pour s'assurer que les objets crées en aval soient manipulés en aval.
		Le compilateur amont ou ses options peuvent ne pas être les mêmes qu'en aval.
	*/
	class downstream_callback__
	{
	protected:
		virtual void WKCLLBCKLaunch( const char *ActionName ) = 0;
	public:
		void reset( bso::bool__ = true )
		{
			// Standadisation.
		}
		E_CVDTOR( downstream_callback__ )
		void Init( void )
		{
			// Standadisation.
		}
		void Launch( const char *ActionName )
		{
			WKCLLBCKLaunch( ActionName );
		}
	};

	class standalone_downstream_callback__
	: public downstream_callback__
	{
	private:
		const actions_ *_Actions;
		const actions_ &_A( void ) const
		{
			if ( _Actions ==  NULL )
				ERRFwk();

			return *_Actions;
		}
	protected:
		virtual void WKCLLBCKLaunch( const char *ActionName )
		{
			wkcllbck::action_callback__ *Action = _A().Get( str::string(  ActionName ) );

			if ( Action == NULL )
				ERRFwk();

			Action->Execute();
		}
	public:
		void reset( bso::bool__ P = true )
		{
			_Actions = NULL;
			downstream_callback__::reset( P );
		}
		E_CVDTOR( standalone_downstream_callback__ )
		void Init( const actions_ &Actions )
		{
			_Actions = &Actions;
			downstream_callback__::Init();
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
			_Version = WKCLLBCK_SHARED_DATA_VERSION;
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

	typedef downstream_callback__ *(launch)( const shared_data__ &Data );
}

				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
