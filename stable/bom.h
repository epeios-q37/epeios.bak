/*
	Header for the 'bom' library by Claude SIMON (csimon at zeusw dot org)
	Copyright (C) 20132004 Claude SIMON.

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

//	$Id: bom.h,v 1.4 2013/07/18 19:46:21 csimon Exp $

#ifndef BOM__INC
#define BOM__INC

#define BOM_NAME		"BOM"

#define	BOM_VERSION	"$Revision: 1.4 $"

#define BOM_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &BOMTutor;

#if defined( E_DEBUG ) && !defined( BOM_NODBG )
#define BOM_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.4 $
//C Claude SIMON (csimon at zeusw dot org)
//R $Date: 2013/07/18 19:46:21 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D Byte Order Marker 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

# include "err.h"
# include "flw.h"
# include "tol.h"
# include "stsfsm.h"

# define BOM_SIZE_MAX	4

# define BOM_UTF_32_BE "\x00\x00\xFE\xFF"
# define BOM_UTF_32_LE	"\xFF\xFE\x00\x00"
# define BOM_UTF_16_BE "\xFE\xFF"
# define BOM_UTF_16_LE	"\xFF\xFE"
# define BOM_UTF_8		"\xEF\xBB\xBF"

namespace bom {

	struct bom__ {
		const char *Data;
		fdr::size__ Size;
		void reset( bso::bool__ = true )
		{
			Data = NULL;
			Size = 0;
		}
		E_CDTOR( bom__ );
		bom__(
			const char *Data,
			fdr::size__ Size )
		{
			this->Data = Data;
			this->Size = Size;
		}
		void Init( void )
		{
			reset();
		}
	};

	enum byte_order_marker__ {
		// Important ! Doivent se succèder par ordre de taille décroissant.
		bom_First,
		bomUTF_32_BE = bom_First,
		bomUTF_32_LE,
		bomUTF_8,
		bomUTF_16_BE,
		bomUTF_16_LE,
		bom_Last = bomUTF_16_LE,
		bom_amount,
		bom_UnknownOrNone,
		bom_Undefined
	};

	void InitializeParser( stsfsm::parser__ &Parser );

	template <typename feeder> byte_order_marker__ DetectBOM(
		feeder &Feeder,
		fdr::size__ &Size )	// Size contient la taille du BOM.
	{
		stsfsm::parser__ Parser;
		Size = 0;

		InitializeParser( Parser );

		if ( Feeder.IsEmpty() )
			return bom_UnknownOrNone;

		while ( !Feeder.IsEmpty() && ( Parser.Handle( Feeder.Get() ) != stsfsm::sLost ) )
			Size++;

		if ( Parser.GetId() == stsfsm::UndefinedId ) {
			Size = 0;
			return bom_UnknownOrNone;
		} else
			return (byte_order_marker__)Parser.GetId();
	}


	byte_order_marker__ DetectBOM(
		const fdr::datum__ *Buffer,
		fdr::size__ &Size );	// 'Size' est modifié pour contenir la taille du 'BOM'.

	const bom__ &GetBOM( byte_order_marker__ BOM );


}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
