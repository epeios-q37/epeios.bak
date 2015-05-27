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

//	$Id: bkdrqm.h,v 1.4 2011/06/11 18:16:00 csimon Exp $

#ifndef BKDRQM__INC
#define BKDRQM__INC

#define BKDRQM_NAME		"BKDRQM"

#define	BKDRQM_VERSION	"$Revision: 1.4 $"

#define BKDRQM_OWNER		"Claude SIMON (http://zeusw.org/intl/contact.html)"

#if defined( E_DEBUG ) && !defined( BKDRQM_NODBG )
#define BKDRQM_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.4 $
//C Claude SIMON (http://zeusw.org/intl/contact.html)
//R $Date: 2011/06/11 18:16:00 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D BacKenD ReQuest Manager 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

# error "Obsolete ! Use 'FBL...' libraries instead !"

#include "err.h"
#include "flw.h"
#include "bso.h"
#include "str.h"
#include "ctn.h"
#include "bkdrpl.h"
#include "bkdtpm.h"
#include "bkdcst.h"
#include "bkddsc.h"
#include "cpe.h"

#ifdef CPE__VC
#	undef GetObject
#endif

#define BKDRQM_INVALID_COMMAND		((unsigned long)BSO_USHORT_MAX)
#define BKDRQM_COMMAND_MAX			( BKDRQM_INVALID_COMMAND - 1UL )

#define BKDRQM_INVALID_TYPE			((unsigned long)BSO_USHORT_MAX)
#define BKDRQM_TYPE_MAX				( BKDRQM_INVALID_TYPE - 1UL )

#define BKDRQM_INVALID_OBJECT		((unsigned long)BSO_USHORT_MAX)
#define BKDRQM_OBJECT_MAX			( BKDRQM_INVALID_OBJECT - 1UL )

namespace bkdrqm {

	using namespace bkddsc;

#define BKDRQM_M( name, type )\
	const bkdtpm::type &name##In( void )\
	{\
		TestInput_( c##name );\
		return *(const bkdtpm::type *)_Get( Position_, c##name );\
	}\
	bkdtpm::type &name##Out( void )\
	{\
		TestOutput_( c##name );\
		return *(bkdtpm::type *)_Put( Position_, c##name );\
	}\


	//c A request manager.
	class request_manager__
	{
	protected:
		virtual const void *BKDRQMGet(
			epeios::row__ Row,
			cast__ Cast ) = 0;
		virtual void *BKDRQMPut(
			epeios::row__ Row,
			cast__ Cast ) = 0;
		virtual void BKDRQMPop(
			flw::iflow__ &Flow,
			const casts_ &Casts ) = 0;
		virtual void BKDRQMPush(
			flw::oflow__ &Flow,
			const casts_ &Casts ) = 0;
	private:
		casts Casts_;
		// Position in the Description_;
		epeios::row__ Position_;
		// At true if all the answer be sent.
		bso::bool__ Closed_;
		// Cast buffer.
		cast Cast_;
		// At true if the request parsed.
		bso::bool__ Parsed_;
		// The input/output channel for the request.
		flw::ioflow__ *Channel_;
		const void *_Get(
			epeios::row__ Row,
			cast__ Cast )
		{
			return BKDRQMGet( Row, Cast );
		}
		const void *_Put(
			epeios::row__ Row,
			cast__ Cast )
		{
			return BKDRQMPut( Row, Cast );
		}
		void _Pop(
			flw::iflow__ &Flow,
			const casts_ &Casts )
		{
				BKDRQMPop( Flow, Casts );
		}
		void _Push(
			flw::oflow__ &Flow,
			const casts_ &Casts )
		{
			BKDRQMPush( Flow, Casts );
		}
		void Test_( cast Cast )
		{
			if ( Position_ == NONE )
				Position_ = Casts_.First();
			else
				Position_ = Casts_.Next( Position_ );

			if ( Casts_.Get( Position_ ) != Cast )
				ERRu();
		}
		void TestInput_( cast Cast )
		{
			Test_( Cast );
		}
		void TestOutput_( cast Cast )
		{
			if ( !Parsed_ ) {
				Test_( cEnd );
				
				Parsed_ = true;

				Channel_->Dismiss();
			}

			Test_( Cast );
		}
	public:
		void reset( bool P = true )
		{
			Casts_.reset( P );

			Position_ = NONE;
			Closed_ = true;
			Cast_ = c_Unknow;
			Parsed_ = false;

			Channel_ = NULL;
		}
		request_manager__( void )
		{
			reset( false );
		}
		virtual ~request_manager__( void )
		{
			reset( true );
		}
		//f Initialization with 'Channel' to parse/answer the request.
		void Init( flw::ioflow__ &Channel )
		{
			reset();
			Channel_ = &Channel;
			Closed_ = false;
			Casts_.Init();
		}
		//f Initialization with 'Channel' to parse/answer the request.
		void Prepare( const casts_ &Casts )
		{
			Init( *Channel_ );
			Casts_ = Casts;
			_Pop( *Channel_, Casts );
		}
		BKDRQM_M( Object, object__)
		BKDRQM_M( Boolean, boolean__ )
		BKDRQM_M( Booleans, booleans_ )
		BKDRQM_M( SLong, slong__ )
		BKDRQM_M( SLongs, slongs_ )
		BKDRQM_M( ULong, ulong__ )
		BKDRQM_M( ULongs, ulongs_ )
		BKDRQM_M( Id8, id8__ )
		BKDRQM_M( Ids8, ids8_ )
		BKDRQM_M( XIds8, xids8_ )
		BKDRQM_M( Id16, id16__ )
		BKDRQM_M( Ids16, ids16 )
		BKDRQM_M( XIds16, xids16_ )
		BKDRQM_M( Id32, id32__ )
		BKDRQM_M( Ids32, ids32_ )
		BKDRQM_M( XIds32, xids32_ )
		BKDRQM_M( Char, char__ )
		BKDRQM_M( String, string_ )
		BKDRQM_M( Strings, strings_ )
		BKDRQM_M( XStrings, xstrings_ )
		BKDRQM_M( Byte, byte__ )
		BKDRQM_M( Binary, binary_ )
		BKDRQM_M( Binaries, binaries_ )
		BKDRQM_M( Items8, items8_ )
		BKDRQM_M( Items16, items16_ )
		BKDRQM_M( Items32, items32_ )
		BKDRQM_M( XItems8, xitems8_ )
		BKDRQM_M( XItems16, xitems16_ )
		BKDRQM_M( XItems32, xitems32_ )
		BKDRQM_M( CommandsDetails, commands_details_ )
		BKDRQM_M( ObjectsReferences, objects_references_ )
		//f Tell that the request is complete (parsed and answered).
		void Complete( void )
		{
			if ( Closed_ )
				return;

			Channel_->Dismiss();
				
			if ( Casts_.Amount() != 0 ) /* If == 0, it means that the request was handled
								   by handling DIRECTLY the underlying flows. */
			{
				if ( !Parsed_ )
					Test_( cEnd );
					
				Channel_->Put( 0 );	// Empty explanation message.

				_Push( *Channel_, Casts_ );

				if ( Casts_.Last() != Position_  )
					ERRu();
			}

			bkdtpm::PutId8( cEnd, *Channel_ );

			Closed_ = true;

			Parsed_ = false;

			Channel_->Commit();
		}
		//f Send a message that explain the reason of no treatment.
		void Report(
			bkdrpl::reply__ Reply,
			const char *Message )
		{
			flw::Put( (flw::datum__)Reply, *Channel_ );

			if ( Reply == bkdrpl::rOK )
					ERRu();

			if ( ( Message == NULL ) || ( Message[0] == 0 ) )
				ERRu();

			flw::PutString( Message, *Channel_ );
			Channel_->Put( 0 );

			Closed_ = true;

			Channel_->Commit();
		}
		void ReportError( const char *Message )	// 'User error'.
		{
			Report( bkdrpl::rUserError, Message );
		}
		void ReportBackendError( const char *Message )
		{
			Report( bkdrpl::rBackendError, Message );
		}
		const casts_ &GetCasts( void ) const
		{
			return Casts_;
		}
		//f Return the channel used to handle the request as input flow.
		flw::iflow__ &Input( void )
		{
			return *Channel_;
		}
		//f Return the channel used to handle the request as ouput flow.
		flw::oflow__ &Output( void )
		{
			return *Channel_;
		}
	};
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
