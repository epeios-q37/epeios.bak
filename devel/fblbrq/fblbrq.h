/*
	Copyright (C) 1999 Claude SIMON (http://q37.info/contact/).

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

// Frontend/Backend Layout Backend ReQuest

#ifndef FBLBRQ_INC_
# define FBLBRQ_INC_

# define FBLBRQ_NAME		"FBLBRQ"

# if defined( E_DEBUG ) && !defined( FBLBRQ_NODBG )
#  define FBLBRQ_DBG
# endif

# include "err.h"
# include "flw.h"

# include "fbltyp.h"
# include "fblbrd.h"
# include "fblovl.h"

# ifdef CPE_C_MSC
#  undef GetObject
# endif

# define FBLBRQ_INVALID_COMMAND		((unsigned long)BSO_U16_MAX)
# define FBLBRQ_COMMAND_MAX			( FBLBRQ_INVALID_COMMAND - 1UL )

# define FBLBRQ_INVALID_TYPE		((unsigned long)BSO_U16_MAX)
# define FBLBRQ_TYPE_MAX			( FBLBRQ_INVALID_TYPE - 1UL )

# define FBLBRQ_INVALID_OBJECT		((unsigned long)BSO_U16_MAX)
# define FBLBRQ_OBJECT_MAX			( FBLBRQ_INVALID_OBJECT - 1UL )

/***************/
/***** OLD *****/
/***************/

namespace fblbrq {
	using namespace fblcst;

	inline void Report(
		fblovl::reply__ Reply,
		const char *Message,
		flw::oflow__ &Channel )
	{
		flw::Put( (flw::byte__)Reply, Channel );

		if ( Reply == fblovl::rOK )
				qRFwk();

		if ( Message == NULL )
			qRFwk();

		flw::PutString( Message, Channel );

		Channel.Put( 0 );

		Channel.Commit();
	}


# define FBLBRQ_M( name, type )\
	const fbltyp::type &name##In( void )\
	{\
		if ( _DismissPending )\
			qRFwk();\
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
			qRFwk();

			return 0;

			// Pour dtecter et liminer cette mthode obsolete.
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
	class request___
	{
	private:
		Q37_MRMDF( callbacks__, C_, Callbacks_ );
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
		flw::rDressedRWFlow<> Channel_;
		bso::bool__ _DismissPending;	// Pour grer la prsence d'un flux dans les paramtres entrants.
		bso::sBool Disconnect_;	// If at 'true', the client asked for disconnection.
		const void *_Get(
			sdr::row__ Row,
			cast__ Cast )
		{
			return C_().Get( Row, Cast );
		}
		const void *_Put(
			sdr::row__ Row,
			cast__ Cast )
		{
			return C_().Put( Row, Cast );
		}
		void _Pop(
			flw::iflow__ &Flow,
			const casts_ &Casts );
		void _Push(
			bso::bool__ FirstCall,
			const casts_ &Casts,
			flw::oflow__ &Flow )
		{
			C_().Push( FirstCall, Casts, Flow );
		}
		void Test_( cast__ Cast )
		{
			if ( Position_ == qNIL )
				Position_ = Casts_.First();
			else
				Position_ = Casts_.Next( Position_ );

			if ( Casts_.Get( Position_ ) != Cast )
				qRFwk();
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

				Channel_.Dismiss();
			}

			Test_( Cast );
		}
	public:
		void reset( bool P = true )
		{
			Callbacks_ = NULL;
			Casts_.reset( P );

			Position_ = qNIL;
			Closed_ = true;
			Cast_ = c_Undefined;
			Parsed_ = false;
			_DismissPending = false;
			Disconnect_ = false;

			tol::reset( P, Channel_ );
		}
		request___( void )
		{
			reset( false );
		}
		virtual ~request___( void )
		{
			reset( true );
		}
		//f Initialization with 'Channel' to parse/answer the request.
		void Init(
			callbacks__ &Callbacks,
			fdr::rRWDriver &Driver )
		{
			reset();
			Callbacks_ = &Callbacks;
			Channel_.Init( Driver );
			Closed_ = false;
			Casts_.Init();
			_DismissPending = false;
			Disconnect_ = false;
		}
		//f Initialization with 'Channel' to parse/answer the request.
		void Prepare( const casts_ &Casts )
		{
			Position_ = qNIL;
			Closed_ = false;
			Cast_ = c_Undefined;
			Parsed_ = false;
			Casts_ = Casts;

			_Pop( Channel_, Casts );
		}
		FBLBRQ_M( Object, object__)
		FBLBRQ_M( Boolean, boolean_t__ )
		FBLBRQ_M( Booleans, booleans_t_ )
		FBLBRQ_M( SInt, sint_t__ )
		FBLBRQ_M( SInts, sints_t_ )
		FBLBRQ_M( UInt, uint_t__ )
		FBLBRQ_M( UInts, uints_t_ )
		FBLBRQ_M( SBig, sbig_t__ )
		FBLBRQ_M( SBigs, sbigs_t_ )
		FBLBRQ_M( UBig, ubig_t__ )
		FBLBRQ_M( UBigs, ubigs_t_ )
		FBLBRQ_M( Id8, id8__ )
		FBLBRQ_M( Id8s, id8s_ )
		FBLBRQ_M( XId8s, xid8s_t_ )
		FBLBRQ_M( Id16, id16__ )
		FBLBRQ_M( Id16s, id16s_ )
		FBLBRQ_M( XId16s, xid16s_t_ )
		FBLBRQ_M( Id32, id32__ )
		FBLBRQ_M( Id32s, id32s_ )
		FBLBRQ_M( XId32s, xid32s_ )
		FBLBRQ_M( Id, id__ )
		FBLBRQ_M( Ids, ids_ )
		FBLBRQ_M( XIds, xids_t_ )
		FBLBRQ_M( Char, char__ )
		FBLBRQ_M( String, string_ )
		FBLBRQ_M( Strings, strings_ )
		FBLBRQ_M( XStrings, xstrings_ )
		FBLBRQ_M( StringsSet, xstrings_ )
		FBLBRQ_M( Byte, byte_t__ )
		FBLBRQ_M( Binary, binary_ )
		FBLBRQ_M( Binaries, binaries_ )
		FBLBRQ_M( IStrings, dIStrings );
		FBLBRQ_M( Item8s, item8s_ )
		FBLBRQ_M( Item16s, item16s_ )
		FBLBRQ_M( Item32s, item32s_ )
		FBLBRQ_M( XItem8s, xitem8s_ )
		FBLBRQ_M( XItem16s, xitem16s_ )
		FBLBRQ_M( XItem32s, xitem32s_ )
		FBLBRQ_M( CommandsDetails, commands_details_ )
		FBLBRQ_M( ObjectsReferences, objects_references_ )
		flw::iflow__ &FlowIn( void )
		{
			TestInput_( cFlow );

			_DismissPending = true;

			return C_().GetFlow( Position_ );
		}
		void FlowOut( flw::iflow__ &Flow )
		{
			TestOutput_( cFlow );

			C_().PutFlow( Position_, Flow );
		}
		//f Tell that the request is complete (parsed and answered).
		void Complete( void )
		{
			if ( Closed_ )
				return;

			if ( !_DismissPending )
				Channel_.Dismiss();

			if ( Casts_.Amount() != 0 ) /* If == 0, it means that the request was handled
								   by handling DIRECTLY the underlying flows. */
			{
				if ( !Parsed_ )
					Test_( cEnd );

				Channel_.Put( 0 );	// Empty explanation message.

				_Push( true, Casts_, Channel_ );

				if ( Casts_.Last() != Position_  )
					qRFwk();
			}

			fbltyp::PutId8( cEnd, Channel_ );

			if ( Casts_.Amount() != 0 ) /* If == 0, it means that the request was handled
								   by handling DIRECTLY the underlying flows. */
				_Push( false, Casts_, Channel_ );

			Closed_ = true;

			Parsed_ = false;

			Channel_.Commit();
		}
		//f Send a message that explain the reason of no treatment.
		void Report(
			fblovl::reply__ Reply,
			const char *Message )
		{
			fblbrq::Report( Reply, Message, Channel_ );

			Closed_ = true;
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
			return Channel_;
		}
		//f Return the channel used to handle the request as ouput flow.
		flw::oflow__ &Output( void )
		{
			return Channel_;
		}
		qRWDISCLOSEr( bso::sBool, Disconnect );
	};

}

/***************/
/***** OLD *****/
/***************/

namespace fblbrq {
	typedef request___ rRequest;
}
#endif
