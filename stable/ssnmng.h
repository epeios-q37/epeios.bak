/*
	Header for the 'ssnmng' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Copyright (C) 2000-2004 Claude SIMON (http://zeusw.org/intl/contact.html).

	This file is part of the Epeios (http://zeusw.org/epeios/) project.

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 3
	of the License, or (at your option) any later version.
 
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, go to http://www.fsf.org/
	or write to the:
  
         	         Free Software Foundation, Inc.,
           59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

//	$Id: ssnmng.h,v 1.40 2013/05/15 14:40:37 csimon Exp $

#ifndef SSNMNG__INC
#define SSNMNG__INC

#define SSNMNG_NAME		"SSNMNG"

#define	SSNMNG_VERSION	"$Revision: 1.40 $"

#define SSNMNG_OWNER		"Claude SIMON (http://zeusw.org/intl/contact.html)"

#include "ttr.h"

extern class ttr_tutor &SSNMNGTutor;

#if defined( E_DEBUG ) && !defined( SSNMNG_NODBG )
#define SSNMNG_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.40 $
//C Claude SIMON (http://zeusw.org/intl/contact.html)
//R $Date: 2013/05/15 14:40:37 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D SeSsioN MaNaGer 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

# include "err.h"
# include "flw.h"
# include "lst.h"
# include "idxbtq.h"
# include "str.h"
# include "lstbch.h"

#ifndef SSNMNG_SIZE
//d Size of the session id.
#define SSNMNG_SIZE	5
#endif

namespace ssnmng {

	//t The type of the row of a session.
	E_ROW( row__ );

	typedef bch::E_BUNCH_( row__ ) rows_;
	E_AUTO( rows )

	//c A session id.
	class session_id__
	{
		char Raw_[SSNMNG_SIZE+1];
	public:
		session_id__( void )
		{
			Raw_[0] = 0;
		}
		//f Initialization.
		void Init( void )
		{
			Raw_[0] = 0;
		}
		//f Create a new session id.
		void New( void );
		//f Return the value of the session id.
		const char *Value( void ) const
		{
			return Raw_;
		}
		//f Dump.
		void Dump( void )
		{
			Raw_[0] = 0;
		}
		//f Return true if empty.
		bso::bool__ IsEmpty( void ) const
		{
			return Raw_[0] == 0;
		}
	};


	struct chrono__ {
		time_t Relative;
		time_t Absolute;
		bso::bool__ Immortal;	// A vrai si la session est toujours considèré comme valide, quel que soit sa durée d'existence.
	};

	class user_functions__
	{
	protected:
		virtual void SSNMNGDelete( void *UP ) = 0;
	public:
		void reset( bso::bool__ = true )
		{
			// Standardisation.
		}
		E_CVDTOR( user_functions__ );
		void Init( void )
		{
			// Standardisation.
		}
		void Delete( void *UP )
		{
			SSNMNGDelete( UP );
		}
	};


	//c A session manager.
	class sessions_manager_
	{
	private:
		user_functions__ *_UserFunctions;
		user_functions__ &_UF( void )
		{
			if ( _UserFunctions == NULL )
				ERRFwk();

			return *_UserFunctions;
		}
		void _Close( const rows_ &Rows );
		void _AdjustSizes( void )
		{
			sdr::size__ Size = Pointers.Extent();

			Queue.Allocate( Size, aem::mFitted );
			Table.Allocate( Size, aem::mFitted );
			Index.Allocate( Size, aem::mFitted );
			Chronos.Allocate( Size, aem::mFitted );
			
		}
	public:
		struct s
		{
			lstbch::E_LBUNCHt_( void *, row__ )::s Pointers;
			que::E_MQUEUEt_( row__ )::s Queue;
			bch::E_BUNCHt_( session_id__, row__ )::s Table;
			idxbtq::E_INDEXt_( row__ )::s Index;
			bch::E_BUNCHt_( chrono__, row__ )::s Chronos;
			bso::u16__ Absolute;
			bso::u16__ Relative;
			row__ Root;
		} &S_;
		lstbch::E_LBUNCHt_( void *, row__ ) Pointers;
		que::E_MQUEUEt_( row__ ) Queue;
		bch::E_BUNCHt_( session_id__, row__ ) Table;
		idxbtq::E_INDEXt_( row__ ) Index;
		bch::E_BUNCHt_( chrono__, row__ ) Chronos;
		sessions_manager_( s &S )
		: S_( S ),
		  Pointers( S.Pointers ),
		  Queue( S.Queue ),
		  Table( S.Table ),
		  Index( S.Index ),
		  Chronos( S.Chronos )
		{}
		void reset( bool P = true )
		{
			S_.Absolute = S_.Relative = 0;
			S_.Root = E_NIL;
			_UserFunctions = NULL;

			Pointers.reset( P );
			Queue.reset( P );
			Table.reset( P );
			Index.reset( P );
			Chronos.reset( P );
		}
		void plug( ags::E_ASTORAGE_ &AS )
		{
			Pointers.plug( AS );
			Queue.plug( AS );
			Table.plug( AS);
			Index.plug( AS );
			Chronos.plug( AS );
		}
		sessions_manager_ &operator =( const sessions_manager_ &SM )
		{
			Pointers = SM.Pointers;
			Queue = SM.Queue;
			Table = SM.Table;
			Index = SM.Index;
			Chronos = SM.Chronos;

			S_.Relative = SM.S_.Relative;
			S_.Absolute = SM.S_.Absolute;
			S_.Root = SM.S_.Root;

			return *this;
		}
		//f Initialization with 'Relative' and 'Absolute' amonut of second.
		void Init(
			bso::u16__ Relative,
			bso::u16__ Absolute,
			user_functions__ &UserFunctions )
		{
			Pointers.Init();
			Table.Init();
			Index.Init();
			Chronos.Init();
			Queue.Init();

			S_.Relative = Relative;
			S_.Absolute = Absolute;
			S_.Root = E_NIL;
			_UserFunctions = &UserFunctions;
		}
		//f Return the position of a mandatory new session.
		row__ New( void *UP );
		//f Remove the session id at position 'Position'.
		void Close( row__ Row )
		{
			_UF().Delete( Pointers( Row ) );
			S_.Root = Index.Delete( Row, S_.Root );
			Pointers.Delete( Row );
			Queue.Delete( Row );
		}
		//f Return the position of 'SessionID' or E_NIL if non-existent.
		row__ Search( const session_id__ &SessionID ) const
		{
			return Search( SessionID.Value() );
		}
		//f Return the position of 'SessionID' or E_NIL if non-existent.
		row__ Search( const char *SessionID ) const;
		//f Return the position of 'SessionID' or E_NIL if non-existent.
		row__ Search( const str::string_ &SessionID ) const;
		//f Return the session id. corresponding to 'Position'.
		session_id__ SessionID( row__ Position )
		{
			return Table( Position );
		}
		//f Touch the session corresponding at position 'P'.
		void Touch( row__ P )
		{
			chrono__ C = Chronos.Get( P );

			if ( time( &C.Relative ) == -1 )
				ERRLbr();

			C.Immortal = false;

			Chronos.Store( C, P );
	#ifdef SSNMNG_DBG
			if ( Queue.Amount() == 0 )
				ERRFwk();
	#endif

			if ( ( Queue.Amount() != 1 ) && ( Queue.Tail() != P ) ) {
				Queue.Delete( P );
				Queue.BecomeNext( P, Queue.Tail() );
			}
		}
		//f La session concernée est toujours considèrée comme valide, jusqu'au prochain 'Touch()',
		void MarkAsImmortal( row__ P )
		{
			chrono__ C = Chronos.Get( P );

			C.Immortal = true;

			Chronos.Store( C, P );
		}
		//f Return true if session corresponding to 'P' is valid.
		bso::bool__ IsAlive( row__ P ) const
		{
			chrono__ C = Chronos.Get( P );

			return ( C.Immortal
				     || ( ( difftime( time( NULL ), C.Absolute ) < S_.Absolute )
				          && ( difftime( time( NULL ), C.Relative ) < S_.Relative ) ) );
		}
		//f Return true if session (which must exists) corresponding to row 'Row' is expired, false otherwise.
		bso::bool__ IsExpired( row__ Row ) const
		{
			return !IsAlive( Row );
		}
		//f Balance the index. 
		void Balance( void )
		{
			if ( S_.Root != E_NIL )
				S_.Root = Index.Balance( S_.Root );
		}
		//f Put in 'Expired' the expired sessions.
		void GetExpired( rows_ &Expired ) const;
		void GetAll( rows_ &Rows ) const;
		void CloseAll( void );
		void CloseExpired( void );
		E_NAVt( Queue., row__ )
	};

	E_AUTO( sessions_manager )
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
