/*
	Header for the 'txmpgh' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Copyright (C) 2000-2001,2004 Claude SIMON (http://zeusw.org/intl/contact.html).

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

//	$Id: txmpgh.h,v 1.9 2012/11/14 16:06:38 csimon Exp $

#ifndef TXMPGH__INC
#define TXMPGH__INC

#define TXMPGH_NAME		"TXMPGH"

#define	TXMPGH_VERSION	"$Revision: 1.9 $"

#define TXMPGH_OWNER		"Claude SIMON (http://zeusw.org/intl/contact.html)"

#include "ttr.h"

extern class ttr_tutor &TXMPGHTutor;

#if defined( E_DEBUG ) && !defined( TXMPGH_NODBG )
#define TXMPGH_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.9 $
//C Claude SIMON (http://zeusw.org/intl/contact.html)
//R $Date: 2012/11/14 16:06:38 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D TeXt Manager ParaGrapH 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "str.h"
#include "xtf.h"
#include "ctn.h"

namespace txmpgh {

	//e state on a line. Odd name, would change if I found a better one.
	enum state
	{
		//i The line is not split.
		sIntegral,
		//i The line is splitted.
		sSplit
	};

	//t A line.
	typedef str::string_ line_;
	typedef str::string line;
	
	//t Lines.
	typedef ctn::E_XMCONTAINER_( line_ ) lines_;

	//c A paragraph manager.
	class paragraph_
	: public lines_
	{
	private:
		// Get a split paragraph from 'Flow'.
		void GetSplitParagraph_( xtf::extended_text_iflow__ &Flow );
	public:
		struct s 
		: lines_::s
		{};
		paragraph_( s &S )
		: lines_( S )
		{}
		void reset( bso::bool__ P = true )
		{
			lines_::reset( P );
		}
		void plug( mmm::multimemory_ &M )
		{
			lines_::plug( M );
		}
		paragraph_ &operator =( const paragraph_ &P )
		{
			lines_::operator =( P );

			return *this;
		}
		//f Initialization.
		void Init( void )
		{
			lines_::Init();
		}
		//f Parse the 'Flow' flow. Extract one line for it, 'State' is the paragraph state.
		void Parse(
			xtf::extended_text_iflow__ &Flow,
			txmpgh::state State = txmpgh::sIntegral )
		{
			if ( State == txmpgh::sIntegral )
			{
				lines_::Allocate( 1 );
				Flow.GetLine( lines_::operator()( 0 ) );
				lines_::Flush();
			}
			else
				GetSplitParagraph_( Flow );
		}
		//f Merge the contain of the paragraph and put into 'Line'. Paragraph remains unmodified.
		void Merge( line_ &Line ) const;
	};

	E_AUTO( paragraph )
}

txf::text_oflow__ &operator <<(
	txf::text_oflow__ &Flow,
	const txmpgh::paragraph_ &Paragraph );


/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
