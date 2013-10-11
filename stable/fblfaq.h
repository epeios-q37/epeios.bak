/*
	Header for the 'fblfaq' library by Claude SIMON (csimon at zeusw dot org)
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

//	$Id: fblfaq.h,v 1.7 2013/07/25 15:59:14 csimon Exp $

#ifndef FBLFAQ__INC
#define FBLFAQ__INC

#define FBLFAQ_NAME		"FBLFAQ"

#define	FBLFAQ_VERSION	"$Revision: 1.7 $"

#define FBLFAQ_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &FBLFAQTutor;

#if defined( E_DEBUG ) && !defined( FBLFAQ_NODBG )
#define FBLFAQ_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.7 $
//C Claude SIMON (csimon at zeusw dot org)
//R $Date: 2013/07/25 15:59:14 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D Frontend/Backend Layout Frontend API Querying 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

# include "err.h"
# include "flw.h"

# include "fblfrd.h"

namespace fblfaq {
	//t Broker parameter.
	typedef fblfrd::id8__	parameter__;

	//t Broker parameters.
	typedef fblfrd::id8s_ 	parameters_;
	typedef fblfrd::id8s	parameters;

	FBLTYP_ITEM( fblfrd::id16, command_item )
	FBLTYP_ITEMS( command_item )

	typedef fbltyp::id16__	type_id__;

	typedef fblfrd::xitem16		type_item;
	typedef fblfrd::xitem16_	type_item_;
	
	typedef fblfrd::xitem16s	types_items;
	typedef fblfrd::xitem16s_	types_items_;
	
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
		void plug( ags::E_ASTORAGE_ &AS )
		{
			Identification .plug( AS );
			Parameters.plug( AS );
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
	typedef ctn::E_CONTAINER_( command_ ) commands_;
	typedef ctn::E_CONTAINER( command_ ) commands;

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
			ID_ = FBLFRD_UNDEFINED_TYPE;

			Prefix.reset( P );
			Name.reset( P );
			Commands.reset( P );
		}
		void plug( ags::E_ASTORAGE_ &AS )
		{
			Name.plug( AS );
			Prefix.plug( AS );
			Commands.plug( AS );
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
			ID_ = FBLFRD_UNDEFINED_TYPE;

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
	typedef ctn::E_CONTAINER_( type_ ) types_;
	typedef ctn::E_CONTAINER( type_ ) types;

	//f Fill 'Types' with contents of 'Backend'.
	void GetDescription(
		fblfrd::backend_access___ &Backend,
		types_ &Types );
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
