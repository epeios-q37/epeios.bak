/*
	Copyright (C) 1999-2016 Claude SIMON (http://q37.info/contact/).

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

//	$Id: txmpgh.h,v 1.9 2012/11/14 16:06:38 csimon Exp $

#ifndef TXMPGH__INC
#define TXMPGH__INC

#define TXMPGH_NAME		"TXMPGH"

#define	TXMPGH_VERSION	"$Revision: 1.9 $"

#define TXMPGH_OWNER		"Claude SIMON (http://zeusw.org/intl/contact.html)"

#if defined( E_DEBUG ) && !defined( TXMPGH_NODBG )
#define TXMPGH_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.9 $
//C Claude SIMON (http://zeusw.org/intl/contact.html)
//R $Date: 2012/11/14 16:06:38 $

/* End of automatic documentation generation part. */

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
#endif
