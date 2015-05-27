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

//	$Id: cgitol.h,v 1.2 2013/07/25 15:59:14 csimon Exp $

#ifndef CGITOL__INC
#define CGITOL__INC

#define CGITOL_NAME		"CGITOL"

#define	CGITOL_VERSION	"$Revision: 1.2 $"

#define CGITOL_OWNER		"Claude SIMON"

#if defined( E_DEBUG ) && !defined( CGITOL_NODBG )
#define CGITOL_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.2 $
//C Claude SIMON (csimon at zeusw dot org)
//R $Date: 2013/07/25 15:59:14 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D CGI (Common Gateway Interface) TOoL 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"
#include "str.h"
#include "cgiarg.h"

namespace cgitol {
	//e States.
	enum state__ {
		//i Unknow state.
		sUnknow,
		//i Missing target.
		sNoTarget,
		//i Missing session id.
		sNoSession,
		//i All present.
		sOK,
		//i amount os states.
		s_amount
	};


	//c Data.
	class parameters_ {
	public:
		struct s {
			str::string_::s Target;
			str::string_::s Skin;
			str::string_::s Session;
		};
		//o Target application.
		str::string_ Target;
		//o Skin.
		str::string_ Skin;
		//o Session id.
		str::string_ Session;
		parameters_( s &S )
		: Target( S.Target ),
		  Skin( S.Skin ),
		  Session( S.Session )
		{}
		void reset( bso::bool__ P = true )
		{
			Target.reset( P );
			Skin.reset( P );
			Session.reset( P );
		}
		void plug( ags::E_ASTORAGE_ &AS )
		{
			Target.plug( AS );
			Skin.plug( AS );
			Session.plug( AS );
		}
		parameters_ &operator =( const parameters_ &D )
		{
			Target = D.Target;
			Skin = D.Skin;
			Session = D.Session;

			return *this;
		}
		//f Initialization.
		void Init( void )
		{
			Target.Init();
			Skin.Init();
			Session.Init();
		}
	};

	E_AUTO( parameters )

	//f Dump the content of 'Data' into 'XMLFiller'.
	template <typename xmlf_> inline void Dump(
		const parameters_ &Parameters,
		xmlf_ &XMLFiller )
	{
		XMLFiller.PutAttribute( "Target", Parameters.Target );
		XMLFiller.PutAttribute( "Skin", Parameters.Skin );
		XMLFiller.PutAttribute( "Session", Parameters.Session );
	}

	//f Dump 'Arguments' into 'XMLFiller'.
	template <typename xmlf_> inline void Dump(
		const cgiarg::arguments_ &Arguments,
		xmlf_ &XMLFiller )
	{
ERRProlog
			cgiarg::row__ Row = E_NIL;
			cgiarg::name Name;
			cgiarg::suffix Suffix;
			cgiarg::value Value;
ERRBegin
			Row = Arguments.First();

			while( Row != E_NIL ) {
				XMLFiller.PushTag( "Argument" );
				Name.Init();
				XMLFiller.PutAttribute( "Name", Arguments.GetName( Row, Name ) );
				if ( Arguments.GetSuffixPosition( Row ) != E_NIL ) {
					Suffix.Init();
					XMLFiller.PutAttribute( "Suffix", Arguments.GetSuffix( Row, Suffix ) );
				}
				Value.Init();
				XMLFiller.PutValue( Arguments.GetValue( Row, Value ) );
				XMLFiller.PopTag();

				Row = Arguments.Next( Row );
			}
ERRErr
ERREnd
ERREpilog
	}


	/*f Fill 'Data' with the content of 'Arguments'.
	Return 'sOK' if needed data present, or what missing otherwise. */
	inline state__ Fill(
		parameters_ &Parameters,
		const cgiarg::arguments_ &Arguments )
	{
		Arguments.GetValue( "_skin", Parameters.Skin );

		if ( !Arguments.GetValue( "_target", Parameters.Target ) )
			return sNoTarget;

		if ( !Arguments.GetValue( "_session", Parameters.Session ) )
			return sNoSession;

		return sOK;
	}

}

#endif
