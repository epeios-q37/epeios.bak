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

#ifndef XDHUJR__INC
# define XDHUJR__INC

# define XDHUJR_NAME		"XDHUJR"

# if defined( E_DEBUG ) && !defined( XDHUJR_NODBG )
#  define XDHUJR_DBG
# endif

// X(SL)/DH(TML) Upstream Javascript-related Registry

# include "err.h"
# include "rgstry.h"

namespace xdhujr {
	using rgstry::entry___;
	using rgstry::rEntry;

	extern entry___ CustomItems;

	namespace custom_item {
		extern entry___ AttributeNames;

		namespace attribute_name {
			extern entry___ Widget;
			extern entry___ Result;
			/*
			extern entry___ OnEvent;
			extern entry___ OnEvents;
			extern entry___ Cast;
			extern entry___ Casts;
			*/
		}
	}

	extern entry___ Scripts;
		
	namespace script {
		extern rEntry Dummy;	// Not used; only to facilitate creation of new entries.
		extern entry___ Log;
		extern entry___ DraggableSetter;
		extern entry___ DroppableSetter;
		extern entry___ WidgetsInstantiator;
		extern entry___ Focuser;
		extern entry___ Selector;
		extern rEntry Parent;
		extern rEntry CreateElement;
		namespace setter {
			extern entry___ Layout;
			extern entry___ EventHandlers;
			extern entry___ Contents;
			extern rEntry Timeout;
		}
		namespace child {
			extern rEntry
				Insert,
				Append,
				First,
				Last;
		}
		namespace sibling {
			extern rEntry
				Previous,
				Next;
		}
		namespace insert {
			extern rEntry
				After,
				Before;
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
		namespace value {
			extern entry___ Setter;
			extern entry___ Getter;
		}
		namespace dialog {
			extern entry___ Alert;
			extern entry___ Confirm;
		}
		namespace widget {
			extern entry___ ValueRetriever;
			extern entry___ Focuser;
			extern entry___ Selector;
			extern entry___ Fetcher;
		}
		/*
		namespace css_rule {
			extern rEntry
				Insert,
				Append,
				Remove;
		}
		*/
		namespace classes	{
			extern rEntry
				Add,
				Remove,
				Toggle;

		};
		namespace elements {
			extern rEntry
				Enable,
				Disable;
		}
	}

}

#endif
