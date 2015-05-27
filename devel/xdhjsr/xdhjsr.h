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

#ifndef XDHJSR__INC
# define XDHJSR__INC

# define XDHJSR_NAME		"XDHJSR"

# if defined( E_DEBUG ) && !defined( XDHJSR_NODBG )
#  define XDHJSR_DBG
# endif

// X(SL)/DH(TML) JavaScript-related Registry

# include "err.h"
# include "rgstry.h"

namespace xdhjsr {
	using rgstry::entry___;

	extern entry___ RootTagId;

	extern entry___ CustomItems;

	namespace custom_item {
		extern entry___ AttributeNames;

		namespace attribute_name {
			extern entry___ Widget;
			extern entry___ Result;
			extern entry___ OnEvent;
			extern entry___ OnEvents;
			extern entry___ Cast;
			extern entry___ Casts;
		}
	}

	extern entry___ Scripts;
		
	namespace script {
		extern entry___ Log;
		extern entry___ DocumentSetter;
		extern entry___ ChildrenSetter;
		extern entry___ Focusing;
		extern entry___ EventHandler;
		namespace casting {
			extern entry___ Definer;
			extern entry___ Handler;
		}
		namespace property {
			extern entry___ Setter;
			extern entry___ Getter;
		}
		namespace attribute {
			extern entry___ Setter;
			extern entry___ Getter;
			extern entry___ Remover;
		}
		namespace content {
			extern entry___ Setter;
			extern entry___ Getter;
		}
		namespace dialog {
			extern entry___ Alert;
			extern entry___ Confirm;
		}
		namespace widget {
			extern entry___ Instantiation;
			extern entry___ ContentRetriever;
			extern entry___ Focusing;
		}
	}

}

#endif
