/*
	Header for the 'fblbrq' library by Claude SIMON (csimon at zeusw dot org)
	Copyright (C) 2004 Claude SIMON.

	This file is part of the Epeios (http://zeusw.org/epeios/) project.

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
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

//	$Id: fblbrq.h,v 1.12 2013/04/15 10:50:50 csimon Exp $

#ifndef FBLBRQ__INC
#define FBLBRQ__INC

#define FBLBRQ_NAME		"FBLBRQ"

#define	FBLBRQ_VERSION	"$Revision: 1.12 $"

#define FBLBRQ_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &FBLBRQTutor;

#if defined( E_DEBUG ) && !defined( FBLBRQ_NODBG )
#define FBLBRQ_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.12 $
//C Claude SIMON (csimon at zeusw dot org)
//R $Date: 2013/04/15 10:50:50 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D Frontend/Backend Layout Backend ReQuest 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

# include "err.h"
# include "flw.h"

# include "fbltyp.h"
# include "fblbrd.h"
# include "fblovl.h"

# ifdef CPE__VC
#  undef GetObject
# endif

# define FBLBRQ_INVALID_COMMAND		((unsigned long)BSO_U16_MAX)
# define FBLBRQ_COMMAND_MAX			( FBLBRQ_INVALID_COMMAND - 1UL )

# define FBLBRQ_INVALID_TYPE		((unsigned long)BSO_U16_MAX)
# define FBLBRQ_TYPE_MAX			( FBLBRQ_INVALID_TYPE - 1UL )

# define FBLBRQ_INVALID_OBJECT		((unsigned long)BSO_U16_MAX)
# define FBLBRQ_OBJECT_MAX			( FBLBRQ_INVALID_OBJECT - 1UL )

namespace fblbrq {
	using namespace fblcst;

# define FBLBRQ_M( name, type )\
	const fbltyp::type &name##In( void )\
	{\
		TestInput_( c##name );\
		return *(const fbltyp::type *)_Get( Position_, c##name );\
	}\
	fbltyp::type &name##Out( void )\
	{\
		TestOutput_( c##name );\
		return *(fbltyp::type *)_Put( Position_, c##name );\
	}\

	struct callbacks__
	{
	protected:
		virtual void FBLBRQPopIn(
			sdr::row__ CRow,	// Cast row.
			flw::iflow__ &Flow,
			cast__ Cast ) = 0;
		virtual void FBLBRQPopInEnd(
			sdr::row__ CRow,	// Cast row.
			flw::iflow__ &Flow ) = 0;
		virtual void FBLBRQPopOut(
			sdr::row__ CRow,	// Cast row.
			flw::iflow__ &Flow,
			cast__ Cast ) = 0;
		virtual void FBLBRQPush(
			bso::bool__ FirstCall,
			const casts_ &Casts,
			flw::oflow__ &Flow ) = 0;
		virtual int FBLBRQPush(
			const casts_ &Casts,
			flw::oflow__ &Flow)
		{
			ERRFwk();

			return 0;

			// Pour détecter et éliminer cette méthode obsolete.
		}
		virtual const void *FBLBRQGet(
			sdr::row__ Row,
			cast__ Cast ) = 0;
		virtual void *FBLBRQPut(
			sdr::row__ Row,
			cast__ Cast ) = 0;
		virtual flw::iflow__ &FBLBRQGetFlow( sdr::row__ Row ) = 0;
		virtual void FBLBRQPutFlow(
			sdr::row__ Row,
			flw::iflow__ &Flow ) = 0;
	public:
		void reset( bso::bool__ = true )
		{
			// Standardisation.
		}
		callbacks__( void )
		{
			reset( false );
		}
		~callbacks__( void )
		{
			reset();
		}
		void Init( void )
		{
			// Standardisation.
		}
		const void *Get(
			sdr::row__ Row,
			cast__ Cast )
		{
			return FBLBRQGet( Row, Cast );
		}
		void *Put(
			sdr::row__ Row,
			cast__ Cast )
		{
			return FBLBRQPut( Row, Cast );
		}
		flw::iflow__ &GetFlow( sdr::row__ Row )
		{
			return FBLBRQGetFlow( Row );
		}
		void PutFlow(
			sdr::row__ Row,
			flw::iflow__ &Flow )
		{
			FBLBRQPutFlow( Row, Flow );
		}
		void PopIn(
			sdr::row__ CRow,
			flw::iflow__ &Flow,
			cast__ Cast )
		{
			FBLBRQPopIn( CRow, Flow, Cast );
		}
		void PopInEnd(
			sdr::row__ CRow,
			flw::iflow__ &Flow )
		{
			FBLBRQPopInEnd( CRow, Flow );
		}
		void PopOut(
			sdr::row__ CRow,
			flw::iflow__ &Flow,
			cast__ Cast )
		{
			FBLBRQPopOut( CRow, Flow, Cast );
		}
		void Push(
			bso::bool__ FirstCall,
			const casts_ &Casts,
			flw::oflow__ &Flow )
		{
			FBLBRQPush( FirstCall, Casts, Flow );
		}
	};

	//c A request manager.
	class request__
	{
	private:
		callbacks__ *_Callbacks;
		casts Casts_;
		// Position in the Description_;
		sdr::row__ Position_;
		// At true if all the answer be sent.
		bso::bool__ Closed_;
		// Cast buffer.
		cast__ Cast_;
		// At true if the request parsed.
		bso::bool__ Parsed_;
		// The input/output channel for the request.
		flw::ioflow__ *Channel_;
		callbacks__ &_C( void )
		{
# ifdef FBLBRQ_DBG
			if ( _Callbacks == NULL )
				ERRFwk();
# endif

			return *_Callbacks;
		}
		const void *_Get(
			sdr::row__ Row,
			cast__ Cast )
		{
			return _C().Get( Row, Cast );
		}
		const void *_Put(
			sdr::row__ Row,
			cast__ Cast )
		{
			return _C().Put( Row, Cast );
		}
		void _Pop(
			flw::iflow__ &Flow,
			const casts_ &Casts );
		void _Push(
			bso::bool__ FirstCall,
			const casts_ &Casts,
			flw::oflow__ &Flow )
		{
			_C().Push( FirstCall, Casts, Flow );
		}
		void Test_( cast__ Cast )
		{
			if ( Position_ == E_NIL )
				Position_ = Casts_.First();
			else
				Position_ = Casts_.Next( Position_ );

			if ( Casts_.Get( Position_ ) != Cast )
				ERRFwk();
		}
		void TestInput_( cast__ Cast )
		{
			Test_( Cast );
		}
		void TestOutput_( cast__ Cast )
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
			_Callbacks = NULL;
			Casts_.reset( P );

			Position_ = E_NIL;
			Closed_ = true;
			Cast_ = c_Undefined;
			Parsed_ = false;

			Channel_ = NULL;
		}
		request__( void )
		{
			reset( false );
		}
		virtual ~request__( void )
		{
			reset( true );
		}
		//f Initialization with 'Channel' to parse/answer the request.
		void Init(
			callbacks__ &Callbacks,
			flw::ioflow__ &Channel )
		{
			reset();
			_Callbacks = &Callbacks;
			Channel_ = &Channel;
			Closed_ = false;
			Casts_.Init();
		}
		//f Initialization with 'Channel' to parse/answer the request.
		void Prepare( const casts_ &Casts )
		{
			Position_ = E_NIL;
			Closed_ = false;
			Cast_ = c_Undefined;
			Parsed_ = false;
			Casts_ = Casts;

			_Pop( *Channel_, Casts );
		}
		FBLBRQ_M( Object, object__)
		FBLBRQ_M( Boolean, boolean_t__ )
		FBLBRQ_M( Booleans, booleans_t_ )
		FBLBRQ_M( SInt, sint_t__ )
		FBLBRQ_M( SInts, sints_t_ )
		FBLBRQ_M( UInt, uint_t__ )
		FBLBRQ_M( UInts, uints_t_ )
		FBLBRQ_M( Id8, id8_t__ )
		FBLBRQ_M( Id8s, id8s_t_ )
		FBLBRQ_M( XId8s, xid8s_t_ )
		FBLBRQ_M( Id16, id16_t__ )
		FBLBRQ_M( Id16s, id16s_t_ )
		FBLBRQ_M( XId16s, xid16s_t_ )
		FBLBRQ_M( Id32, id32_t__ )
		FBLBRQ_M( Id32s, id32s_t_ )
		FBLBRQ_M( XId32s, xid32s_t_ )
		FBLBRQ_M( Id, id_t__ )
		FBLBRQ_M( Ids, ids_t_ )
		FBLBRQ_M( XIds, xids_t_ )
		FBLBRQ_M( Char, char__ )
		FBLBRQ_M( String, string_ )
		FBLBRQ_M( Strings, strings_ )
		FBLBRQ_M( XStrings, xstrings_ )
		FBLBRQ_M( Byte, byte_t__ )
		FBLBRQ_M( Binary, binary_ )
		FBLBRQ_M( Binaries, binaries_ )
		FBLBRQ_M( Item8s, item8s_t_ )
		FBLBRQ_M( Item16s, item16s_t_ )
		FBLBRQ_M( Item32s, item32s_t_ )
		FBLBRQ_M( XItem8s, xitem8s_t_ )
		FBLBRQ_M( XItem16s, xitem16s_t_ )
		FBLBRQ_M( XItem32s, xitem32s_t_ )
		FBLBRQ_M( CommandsDetails, commands_details_ )
		FBLBRQ_M( ObjectsReferences, objects_references_ )
		flw::iflow__ &FlowIn( void )
		{
			TestInput_( cFlow );
			return _C().GetFlow( Position_ );
		}
		void FlowOut( flw::iflow__ &Flow )
		{
			TestOutput_( cFlow );

			_C().PutFlow( Position_, Flow );
		}
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

				_Push( true, Casts_, *Channel_ );

				if ( Casts_.Last() != Position_  )
					ERRFwk();
			}

			fbltyp::PutId8( cEnd, *Channel_ );

			if ( Casts_.Amount() != 0 ) /* If == 0, it means that the request was handled
								   by handling DIRECTLY the underlying flows. */
				_Push( false, Casts_, *Channel_ );

			Closed_ = true;

			Parsed_ = false;

			Channel_->Commit();
		}
		//f Send a message that explain the reason of no treatment.
		void Report(
			fblovl::reply__ Reply,
			const char *Message )
		{
			flw::Put( (flw::datum__)Reply, *Channel_ );

			if ( Reply == fblovl::rOK )
					ERRPrm();

			if ( ( Message == NULL ) || ( Message[0] == 0 ) )
				ERRFwk();

			flw::PutString( Message, *Channel_ );
			Channel_->Put( 0 );

			Closed_ = true;

			Channel_->Commit();
		}
		void ReportSoftwareError( const char *Message )	// 'User error'.
		{
			Report( fblovl::rSoftwareError, Message );
		}
		void ReportRequestError( const char *Message )
		{
			Report( fblovl::rRequestError, Message );
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
