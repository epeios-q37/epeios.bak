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

#ifndef FBLFAQ_INC_
#define FBLFAQ_INC_

#define FBLFAQ_NAME		"FBLFAQ"

#if defined( E_DEBUG ) && !defined( FBLFAQ_NODBG )
#define FBLFAQ_DBG
#endif

// Frontend/Backend Layout Frontend API Querying

#include "err.h"
#include "flw.h"

# include "fblfrd.h"

namespace fblfaq {
	typedef fblfrd::id8__	parameter__;

	typedef fblfrd::id8s_ 	parameters_;
	typedef fblfrd::id8s	parameters;

	FBLTYP_ITEM( fblfrd::id16, command_item )
	FBLTYP_ITEMS( command_item )

	typedef fbltyp::id16__	type_id__;

	typedef fblfrd::xitem16		type_item;
	typedef fblfrd::xitem16_	type_item_;

	typedef fblfrd::xitem16s	types_items;
	typedef fblfrd::xitem16s_	types_items_;

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
		void plug( qASd *AS )
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

	typedef ctn::E_CONTAINER_( command_ ) commands_;
	typedef ctn::E_CONTAINER( command_ ) commands;

	class type_
	{
	public:
		struct s {
			str::string_::s Prefix;
			str::string_::s Name;
			commands_::s Commands;
			type_id__ ID;
		} &S_;
		str::string_ Prefix;
		str::string_ Name;
		commands_ Commands;
		type_( s &S )
		: S_( S),
		  Prefix( S.Prefix ),
		  Name( S.Name ),
		  Commands( S.Commands )
		{}
		void reset( bool P = true )
		{
			S_.ID = FBLFRD_UNDEFINED_TYPE;

			Prefix.reset( P );
			Name.reset( P );
			Commands.reset( P );
		}
		void plug( qASd *AS )
		{
			Name.plug( AS );
			Prefix.plug( AS );
			Commands.plug( AS );
		}
		type_ &operator =( const type_ &BT )
		{
			S_.ID = BT.S_.ID;

			Prefix = BT.Prefix;
			Name = BT.Name;
			Commands = BT.Commands;

			return *this;
		}
		//f Initialization.
		void Init( void )
		{
			S_.ID = FBLFRD_UNDEFINED_TYPE;

			Prefix.Init();
			Name.Init();
			Commands.Init();
		}
		qRWDISCLOSEd( type_id__, ID );
	};

	E_AUTO( type )

	typedef ctn::E_CONTAINER_( type_ ) types_;
	typedef ctn::E_CONTAINER( type_ ) types;

	void GetDescription(
		fblfrd::frontend___ &Frontend,
		types_ &Types );
}
#endif
