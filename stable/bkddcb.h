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

//	$Id: bkddcb.h,v 1.3 2011/06/11 18:16:00 csimon Exp $

#ifndef BKDDCB__INC
#define BKDDCB__INC

#define BKDDCB_NAME		"BKDDCB"

#define	BKDDCB_VERSION	"$Revision: 1.3 $"

#define BKDDCB_OWNER		"Claude SIMON (http://zeusw.org/intl/contact.html)"

#if defined( E_DEBUG ) && !defined( BKDDCB_NODBG )
#define BKDDCB_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.3 $
//C Claude SIMON (http://zeusw.org/intl/contact.html)
//R $Date: 2011/06/11 18:16:00 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D BacKenD DesCriBer 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

# error "Obsolete ! Use 'FBL...' libraries instead !"

#include "err.h"
#include "flw.h"
#include "bkdacc.h"
#include "bso.h"
#include "bkdtpm.h"

namespace bkddcb {
	//t Broker parameter.
	typedef bkdacc::id8__	parameter__;

	//t Broker parameters.
	typedef bkdacc::ids8_ 	parameters_;
	typedef bkdacc::ids8	parameters;

	BKDTPM_ITEM( bkdacc::id16__, command_item )
	BKDTPM_ITEMS( command_item, commands_items )

	typedef bso::ushort__	type_id__;

	typedef bkdacc::xitem16		type_item;
	typedef bkdacc::xitem16_	type_item_;
	
	typedef bkdacc::xitems16	types_items;
	typedef bkdacc::xitems16_	types_items_;
	
	//c A broker Command.
	class command_
	{
	public:
		//o The command identification.
		command_item_ Identification;
		//o The parameters.
		parameters_ Parameters;
		struct s {
			command_item_::s Identification;
			parameters_::s Parameters;
		};
		command_( s &S )
		: Identification ( S.Identification ),
		  Parameters( S.Parameters )
		 {}
		void reset( bool P = true )
		{
	 		Identification.reset( P );
			Parameters.reset( P );
		}
		void plug( mmm::multimemory_ &M )
		{
			Identification .plug( M );
			Parameters.plug( M );
		}
		command_ &operator =( const command_ &BC )
		{
			Identification = BC.Identification;
			Parameters = BC.Parameters;

			return *this;
		}
		//f Initialization.
		void Init( void )
		{
			Identification.Init();
			Parameters.Init();
		}
	};

	E_AUTO( command )

	//t Broker commands.
	typedef ctn::E_XCONTAINER_( command_ ) commands_;
	typedef ctn::E_XCONTAINER( command_ ) commands;

	//c A broker object type.
	class type_
	{
	private:
		// The type identification.
		type_id__ ID_;
	public:
		//o Name of the type.
		str::string_ Name;
		//o Prefix of the type.
		str::string_ Prefix;
		//o The commands of the type.
		commands_ Commands;
		struct s {
			str::string_::s Prefix;
			str::string_::s Name;
			commands_::s Commands;
		};
		type_( s &S )
		: Prefix( S.Prefix ),
		  Name( S.Name ),
		  Commands( S.Commands )
		{}
		void reset( bool P = true )
		{
			ID_ = BKDACC_UNDEFINED_TYPE;

			Prefix.reset( P );
			Name.reset( P );
			Commands.reset( P );
		}
		void plug( mmm::multimemory_ &M )
		{
			Name.plug( M );
			Prefix.plug( M );
			Commands.plug( M );
		}
		type_ &operator =( const type_ &BT )
		{
			ID_ = BT.ID_;

			Prefix = BT.Prefix;
			Name = BT.Name;
			Commands = BT.Commands;

			return *this;
		}
		//f Initialization.
		void Init( void )
		{
			ID_ = BKDACC_UNDEFINED_TYPE;

			Prefix.Init();
			Name.Init();
			Commands.Init();
		}
		//f Return the ID.
		type_id__ ID( void ) const
		{
			return ID_;
		}
		//f Set the ID to 'ID'.
		void ID( type_id__ ID )
		{
			ID_ = ID;
		}
	};

	E_AUTO( type )

	//t Broker types.
	typedef ctn::E_XCONTAINER_( type_ ) types_;
	typedef ctn::E_XCONTAINER( type_ ) types;

	//f Fill 'Types' with contents of 'Backend'.
	void GetDescription(
		bkdacc::backend_access___ &Backend,
		types_ &Types );
#if 0	
	//f Put in 'RawErrorMessages' the raw error messages from 'Backend'.
	inline void GetRawMessages(
		bkdacc::backend_access___ &Backend,
		bkdacc::strings_ &RawMessages )
	{
		Backend.GetRawMessages( RawMessages );
	}
#endif
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
