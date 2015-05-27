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

# error "Obsolete ! Use 'xht...' libraries instead."

# include "err.h"
# include "flw.h"
# include "str.h"
# include "stsfsm.h"
# include "ids.h"

# define WKCLLBCK_SHARED_DATA_VERSION_NUMBER	"1"

# define WKCLLBCK_SHARED_DATA_VERSION	WKCLLBCK_SHARED_DATA_VERSION_NUMBER "-" CPE_ARCHITECTURE_LABEL


# define WKCLLBCK_LAUNCH_FUNCTION_NAME		WKCLLBCKLaunch

namespace wkcllbck {
	class action_callback__
	{
		virtual void WKCLLBCKExecute( void ) = 0;
	public:
		void reset( bso::bool__ = true )
		{
			// Standardisation.
		}
		E_CVDTOR( action_callback__ );
		void Init( void )
		{
			// Standardisation.
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
		}
		E_CVDTOR( downstream_callback__ )
		void Init( const actions_ &Actions )
		{
			_Actions = &Actions;
		}
		void Launch( const char *ActionName )
		{
			WKCLLBCKLaunch( ActionName );
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
