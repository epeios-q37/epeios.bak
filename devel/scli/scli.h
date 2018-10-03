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

// SoCLe Info

#ifndef SCLI_INC_
# define SCLI_INC_

# define SCLI_NAME		"SCLI"

# if defined( E_DEBUG ) && !defined( SCLI_NODBG )
#  define SCLI_DBG
# endif

# include "err.h"

namespace scli {

	// If the structure is changed, modify the global id int he '.cpp' !!!
	struct sInfo
	{
	private:
		// Must be first member.
		static const char *Id_;
		// Throws an exception if the id of the object doesn't fit the global one.
		static void Control_( void );
		const char *Return_( const char *Name ) const
		{
			Control_();

			if ( Name == NULL )
				qRFwk();

			return Name;
		}
		const char
			*Target_,
			*Product_,
			*Organization_;
	public:
		sInfo( void )
		{
			Target_ = Product_ = Organization_ = NULL;
		}
		sInfo(
			const char *Name,
			const char *Product,
			const char *Organization )
		{
			Init( Name, Product, Organization );
		}
		void Init(
			const char *Target,
			const char *Product,
			const char *Organization )
		{
			Target_ = Target;
			Product_ = Product;
			Organization_ = Organization;
		}
		const char *Target( void ) const
		{
			return Return_( Target_ );
		}
		const char *Product( void ) const
		{
			return Return_( Product_ );
		}
		const char *Organization( void ) const
		{
			return Return_( Organization_ );
		}
	};
}

// To use in '.h'.
# define SCLI_DEC( name )\
	namespace name {\
		extern const scli::sInfo Info;\
	}\

// To use in '.cpp'.
# define SCLI_DEFX( name, target, product, organization )	const scli::sInfo name::Info( target, product, organization )
# define SCLI_DEF( name, target, product )	SCLI_DEFX( name, target, product, "q37.info" )

#endif
