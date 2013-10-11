/*
  Header for the 'rqm' library by Claude SIMON (http://zeusw.org/intl/contact.html)
  Copyright (C) 2000 Claude SIMON (http://zeusw.org/intl/contact.html) 

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
  along with this program; if not, go to http://www.fsf.org or write to the
  
                        Free Software Foundation, Inc.,
           59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

//	$Id: rqm.h,v 1.8 2012/11/14 16:06:32 csimon Exp $

#ifndef RQM__INC
#define RQM__INC

#define RQM_NAME		"RQM"

#define	RQM_VERSION	"$Revision: 1.8 $"	

#define RQM_OWNER		"the Epeios project (http://zeusw.org/epeios/)"

#include "ttr.h"

extern class ttr_tutor &RQMTutor;

#if defined( E_DEBUG ) && !defined( RQM_NODBG )
#define RQM_DBG 
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.8 $
//C Claude SIMON (http://zeusw.org/intl/contact.html)
//R $Date: 2012/11/14 16:06:32 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

//D ReQuest Manager. Osolete. Use 'BRKRQM' instead.

#error Obsolete. Use 'BRKRQM' instead.

#include <stdarg.h>

#include "err.h"
#include "bso.h"
#include "set.h"
#include "str.h"
#include "ctn.h"
#include "brkcst.h"

#define RQM_INVALID_COMMAND		BSO_USHORT_MAX
#define RQM_COMMAND_MAX			( RQM_INVALID_COMMAND - 1 )

#define RQM_INVALID_TYPE		BSO_USHORT_MAX
#define RQM_TYPE_MAX			( RQM_INVALID_TYPE - 1 )

#define RQM_INVALID_OBJECT		BSO_USHORT_MAX
#define RQM_OBJECT_MAX			( RQM_INVALID_OBJECT - 1 )


namespace rqm
{
	using namespace brkcst;

	//e Item kind.
	enum kind {
		//i Undefined value kind.
		kUndefined,
		//i Normal item.
		kNormal,
		//i Array item.
		kArray,
		//i LAst array item.
		kArrayLast,
		//i Amount ot item type
		k_amount
	};

	typedef bso__ushort		tcommand__;
	//t Command
	E_TYPEDEF( tcommand__, command__ );

	typedef bso__ushort		tobject__;
	//t Object
	E_TYPEDEF( bso__ushort, object__ );

	typedef bso__ushort		ttype__;
	//t Type
	E_TYPEDEF( ttype__, type__ );

	typedef bso__ubyte tcast__;
	//t Cast
	E_TYPEDEF( tcast__, cast__ );

	//c The description of a request.
	class description_
	{
	private:
		cast__ GetCast_( brkcst::cast Cast )
		{
#ifdef RQM_DBG
			if ( S_.Casts == NULL )
				ERRu();
#endif
			return S_.Casts[Cast];
		}
	public:
		//o Name of the command.
		str_string_ Name;
		/*o Parameters and return value types. Separated by 'cEnd'. The 'cEnd'
		to signalize the end of the request must NOT be put. */
		SET_( cast__ ) Casts;
		struct s
		{
			str_string_::s Name;
			SET_( cast__ )::s Casts;
			// Cast correspondance.
			cast__ *Casts;
		} &S_;
		description_( s &S )
		: S_( S ),
		  Name( S.Name ),
		  Casts( S.Casts )
		{}
		void reset( bool P = true )
		{
			Name.reset( P );
			Casts.reset( P );
			S_.Casts = NULL;
		}
		description_ &operator =( const description_ &D )
		{
			Name = D.Name;
			Casts = D.Casts;
			S_.Casts = D.S_.Casts;

			return *this;
		}
		void plug( mmm_multimemory_ &M )
		{
			Name.plug( M );
			Casts.plug( M );
		}
		/*f Initialization with casts values 'Casts', which must have a size of 'brkcst::c_amount'.
		'Casts' is not copied, so it should not be modified. */
		void Init( cast__ Casts )
		{
			Name.Init();
			Casts.Init();

			S_.Casts == Casts;
		}
#if 0
		/*f Initializing with name 'Name' and a list of casts. The list must
		contain 2 'cEnd', the first at the end of the parameters casts,
		and 1 of the end of returned values casts. 'Cast' is only here to avoid 
		ambiguity with the next function. */
		void Init(
			const char *Name,
			cast Cast,
			... );
#endif
		/*f Initializing with name 'Name' and a the list of casts 'Casts'. The list
		must contain 2 'cEnd', the first at the end of the parameters casts,
		and 1 of the end of returned values casts. */
		void Init(
			const char *Name,
			const cast *Casts );
#if 0
		//f Adding the cast 'Cast'.
		void Add( cast Cast )
		{
			Casts.Add( Cast );
		}
#endif
		//f Return the amount of casts.
		SIZE__ Amount( void ) const
		{
			return Casts.Amount();
		}
		//f Return the cas at position 'Position'.
		cast Read( POSITION__ Position ) const
		{
			return (cast)Casts( Position );
		}
	};

	AUTO( description )

	//c Request descriptions.
	class descriptions_
	: public XCONTAINER_( description_ )
	{
	public:
		//o The descriptions
		struct s
		: public XCONTAINER_( description_ )::s
		{};
		descriptions_( s &S )
		: XCONTAINER_( description_ )( S )
		{}
		void reset( bool P = true )
		{
			XCONTAINER_( description_ )::reset( P );
		}
		void plug( mmm_multimemory_ &M )
		{
			XCONTAINER_( description_ )::plug( M );
		}
		descriptions_ &operator =( const descriptions_ &D )
		{
			XCONTAINER_( description_ )::operator =( D );

			return *this;
		}
		//f Initialization.
		void Init( void )
		{
			XCONTAINER_( description_ )::Init();
		}
		//f Adding 'Description'. Return the position where added.
		POSITION__ Add( const description_ &Description )
		{
			return XCONTAINER_( description_ )::Add( Description );
		}
# if 0
		/*f See 'descriptions_::Add()' for the descriptions of the parameters.
		return the position where request description added. 'Cast' is only here to avoid
		ambihity with the n,ext function. */
		POSITION__ Add(
			const char *Name,
			cast Cast,
			... );
#endif
		/*f See 'descriptions_::Add()' for the descriptions of the parameters.
		return the position where request description added. */
		POSITION__ Add(
			const char *Name,
			const cast *Casts )
		{
			description Description;

			Description.Init( Name, Casts );
			
			return Add( Description );
		}
		/*f Return the position of the description of the request named 'Name' or 
		'NONE' if non-existant. */
		POSITION__ Position( const char *Name ) const;
		/*f Return the position of the description 'Description'. */
		POSITION__ Position( const description_ &Description ) const;
		//f Return the amount of descriptions.
		SIZE__ Amount( void ) const
		{
			return XCONTAINER_( description_ )::Amount();
		}
	};

	AUTO( descriptions )


	/*f Put the object id. 'Object' in 'Request'. You must begin with this function
	to create a request. */
	inline void PutObject(
		object__ Object,
		flw::oflow___ &Request )
	{
		flw::Put( Object, Request );
	}

	//f Put command id 'Command' in request. Succeed immediatly after 'PutObject()'.
	inline void PutCommand(
		command__ Command,
		flw::oflow___ &Request )
	{
		flw::Put( Command, Request );
	}

	//f Add to 'Request' the value 'Value' of type 'Type'.
	void AddValue(
		cast Cast,
		const void *Value,
		flw::oflow___ &Request );

	/*f Add cast id. 'Cast' to request. Usually used to put 'cMulti' and the
	required 'cEnd'. */
	inline void AddCast(
		cast Cast,
		flw::oflow___ &Request )
	{
		cast__ C = Cast;

		flw::Put( C, Request );
	}

	//f Complete 'Request'.
	inline void Complete( flw::oflow___ &Request )
	{
		AddCast( cEnd, Request );
		Request.Synchronize();
	}

	//f Return the id of the cast 'Cast', or 'cInvalid' if non-existent.
	cast IdCaste( const str_string_ &Cast );


	//c A request manager.
	class request_manager___
	{
	private:
		// The corresponding request description.
		const description_ *Description_;
		// Position in the Description_;
		POSITION__ Position_;
		// Position of the beginning of th array, il any.
		POSITION__ Array_;
		// At true if all the answer be sent.
		bso__bool Closed_;
		// Cast buffer.
		cast Cast_;
		// At true if all the request parsed.
		bso__bool Parsed_;
		// The input/output channle for the request.
		flw::ioflow___ *Channel_;
		cast GetCast_( void )
		{
			cast__ Cast = Cast_;

			if ( Cast != cInvalid )
				Cast_ = cInvalid;
			else
			{
				flw::Get( *Channel_, Cast );

				if ( ( Array_ != NONE )
					  && ( Cast != cEnd )
					  && ( Description_->Read( Position_ ) == cEnd ) )
						Position_ = Array_;

				if ( Cast != Description_->Read( Position_++ ) )
					ERRu();
			}

			return (cast)Cast;
		}
		void GetValue_(
			cast Caste,
			void *Valeur );
		void TestEndOfParsing_( bso__bool ExplanationMessage = false )
		{
			if ( !Parsed_ )
				if ( GetCast_() != cEnd )
					ERRu();
				else if ( !ExplanationMessage )
					Channel_->Put( 0 );	// Empty explanation message.

			Parsed_ = true;
		}

	public:
		void reset( bool = true )
		{
			Description_ = NULL;

			Position_ = NONE;
			Array_ = NONE;
			Closed_ = true;
			Cast_ = cInvalid;
			Parsed_ = false;

			Channel_ = NULL;
		}
		request_manager___( void )
		{
			reset( false );
		}
		~request_manager___( void )
		{
			reset( true );
		}
		//f Initialization with 'Channel' to parse/answer the request.
		void Init( flw::ioflow___ &Channel )
		{
			reset();

			Closed_ = false;

			Channel_ = &Channel;
		}
		//f 'Description' is the description of the request being handled.
		void SetDescription( const description_ &Description )
		{
			Description_ = &Description;
			Position_ = 0;
		}
		/*f Put to 'Value' the next return value which is of cast 'Cast'.
		Return if the value red is the last item of an array. */
		kind GetValue(
			cast Cast,
			void *Value );
		//f Tell that the request is complete (parsed and answered).
		void Complete( void )
		{
			if ( Closed_ )
				return;

			if ( Description_ ) /* If == 'NULL', it means that the request was handled
								   by handling DIRECTLY the underlying flows. */
			{
				TestEndOfParsing_();

				if ( Position_ != Description_->Amount() )
					ERRu();

				if ( Array_ != NONE )
					ERRu();

			}

			AddCast( cEnd, *Channel_ );

			Closed_ = true;

			Parsed_ = false;

			Channel_->Synchronize();
		}
		//f Add 'Value' of type 'Type'.
		void AddValue(
			cast Cast,
			const void *Value )
		{
			TestEndOfParsing_();

			if ( Array_ != NONE )
				if ( ( Position_ >= Description_->Amount() )
					|| ( Description_->Read( Position_ ) != Cast ) )
					ERRu();
				else
					Position_++;

				rqm::AddValue( Cast, Value, *Channel_ );
		}
		//f Signalize that a array session begins.
		void BeginArray( void )
		{
			TestEndOfParsing_();

			if ( Array_ != NONE )
				ERRu();

			if ( ( Position_ >= Description_->Amount() )
				  || ( Description_->Read( Position_ ) != cArray ) )
				ERRu();
			else
				Position_++;


			rqm::AddCast( cArray, *Channel_) ;

			Array_ = Position_;
		}
		//f Signalize the end of a array session.
		void EndArray( void )
		{
			if ( Array_ == NONE )
				ERRu();

			AddCast( cEnd, *Channel_ ) ;

			Array_ = NONE;
		}
		//f Send a message that explain the reason of no treatment.
		void SendExplanationMessage( const char *Message )
		{
			TestEndOfParsing_( true );

			if ( !Message[0] )
				ERRu();

			flw::PutString( Message, *Channel_ );

			Closed_ = true;

			Channel_->Synchronize();
		}
		//f Return the channel used to handle the request as input flow.
		flw::iflow___ &Input( void )
		{
			return *Channel_;
		}
		//f Return the channel used to handle the request as ouput flow.
		flw::oflow___ &Output( void )
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
