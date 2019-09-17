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

#ifndef BOMHDL_INC_
#define BOMHDL_INC_

#define BOMHDL_NAME		"BOMHDL"

#define BOMHDL_OWNER		"Claude SIMON"

#if defined( E_DEBUG ) && !defined( BOMHDL_NODBG )
#define BOMHDL_DBG
#endif
// Byte Order Marker 

# include "err.h"
# include "flw.h"
# include "tol.h"
# include "stsfsm.h"

namespace bomhdl {
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
		// Important ! Doivent se succder par ordre de taille dcroissant.
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
		const fdr::byte__ *Buffer,
		fdr::size__ &Size );	// 'Size' est modifi pour contenir la taille du 'BOM'.

	const bom__ &GetBOM( byte_order_marker__ BOM );


}

#endif
