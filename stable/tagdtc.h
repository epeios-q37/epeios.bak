/*
	Header for the 'tagdtc' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Copyright (C) 2000-2001, 2004 Claude SIMON (http://zeusw.org/intl/contact.html).

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

//	$Id: tagdtc.h,v 1.11 2012/11/14 16:06:37 csimon Exp $

#ifndef TAGDTC__INC
#define TAGDTC__INC

#define TAGDTC_NAME		"TAGDTC"

#define	TAGDTC_VERSION	"$Revision: 1.11 $"

#define TAGDTC_OWNER		"Claude SIMON (http://zeusw.org/intl/contact.html)"

#if defined( E_DEBUG ) && !defined( TAGDTC_NODBG )
#define TAGDTC_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.11 $
//C Claude SIMON (http://zeusw.org/intl/contact.html)
//R $Date: 2012/11/14 16:06:37 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D TAG DeTeCtor 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/
//D TAG DeTeCtor.

#include "err.h"
#include "bso.h"
#include "str.h"
#include "bch.h"
#include "xtf.h"


//d Special value indicating end of flow.
#define TAGDTC_UNKNOW			NONE
// depend on the type of tagdtc__id.

//d Special value indicating an unknow tag.
#define TAGDTC_EOF				(TAGDTC_UNKNOW - 1 )

//d Last available tag id.
#define TAGDTC_MAX_ID		(TAGDTC_EOF - 1 )

#define TAGDTC_AMOUNT_OF_CHAR	64

#define TAGDTC_NO	255


namespace tagdtc {

	typedef bso::ubyte__	char__;
	typedef epeios::row__	id__;
	typedef epeios::row__	path__;

	extern const char__ Table[];

	//e Action to do for the output.
	enum action {
		//i Print to output.
		aPrint,
		//i Don't print to output.
		aSkip
	};


	struct card__
	{
	private:
		path__ Table_[TAGDTC_AMOUNT_OF_CHAR];
	public:
		card__( void )
		{
			memset( Table_, 255, sizeof( Table_ ) );
		}
		path__ &operator[]( int P )
		{
			return Table_[P];
		}
		const path__ &operator[]( int P ) const
		{
			return Table_[P];
		}
	};


	//c Tag detector.
	class tag_detector_
	{
	private:
		path__ Start_;	
		path__ GetBlankCardAndValue_( void )
		{
			card__ Card;
			path__ Path = Cards.New();

			Cards.Store( Card, Path );

			if ( Ids.New() != Path )
				ERRc();

			Ids.Store( TAGDTC_UNKNOW, Path );

			return Path;
		}
		path__ GetNext_(
			path__ Path,
			char__ C )
		{
			card__ Card;

			Card = Cards( Path );

			C = Table[C];

			if ( C == TAGDTC_NO )
				ERRu();
		
			if ( Card[C] == TAGDTC_UNKNOW )
			{
				Card[C] = GetBlankCardAndValue_();
				Cards.Store( Card, Path );
			}

			return Card[C];
		}
	public:
		bch::E_BUNCH_( card__ ) Cards;
		bch::E_BUNCH_( id__ ) Ids;
		struct s 
		{
			bch::E_BUNCH_( card__ )::s Cards;
			bch::E_BUNCH_( id__ )::s Ids;
		};
		tag_detector_( s &S )
		: Cards( S.Cards ),
		  Ids( S.Ids )
		{}
		void reset( bso::bool__ P = true )
		{
			Cards.reset( P );
			Ids.reset( P );
		}
		void plug( mmm::multimemory_ & M)
		{
			Cards.plug( M );
			Ids.plug( M );
		}
		tag_detector_ &operator =( const tag_detector_ &D )
		{
			Cards = D.Cards;
			Ids = D.Ids;

			return *this;
		}
		//f Initialization.
		void Init( void )
		{
			Cards.Init();
			Ids.Init();

			Start_ = GetBlankCardAndValue_();
		}
		//f Add 'Tag' and affect it 'Id'.
		void Add(
			const char *Tag,
			id__ Id );
		//f Add 'Tag' and affect it 'Id'.
		void Add(
			const str::string_ &Tag,
			id__ Id );
		/*f Parse 'IFlow' and write it to 'OFLow' until it
		encounters a tag.between 2 'Delimiter' or 'EOX()'.
		Return the tag id or an error code if error occurs */
		epeios::row_t__ Parse(
			xtf::extended_text_iflow__ &IFlow,
			txf::text_oflow__ &Flow,
			char Delimiter,
			tagdtc::action Action = tagdtc::aPrint,
			err::handle ErrHandle = err::hUsual ) const;
	};

	E_AUTO( tag_detector )
};

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
