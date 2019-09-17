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

#ifndef DWTXCL_INC_
# define DWTXCL_INC_

# define DWTXCL_NAME		"DWTXCL"

# if defined( E_DEBUG ) && !defined( DWTXCL_NODBG )
#  define DWTXCL_DBG
# endif

// Directory WaTcher eXCLusion

# include "dwtbsc.h"

# include "stsfsm.h"
# include "ags.h"

namespace dwtxcl {
	namespace {
		typedef stsfsm::automat_ _automat_;
	}

	enum state__ {
		sExcludedFile,
		sExcludedDir,
		sGhost,	// R�pertoire/fichier fant�me pour la gestion du renommage de r�pertoire.
		sIncluded,
		s_amount,
		s_Undefined,
	};


	class excluder_ {
	private:
		void _AddGhostSuffix( const str::string_ &Tag );
	public:
		struct s {
			_automat_::s Automat;
		};
		_automat_ Automat;
		excluder_( s &S )
		: Automat( S.Automat )
		{}
		void reset( bso::bool__ P = true )
		{
			Automat.reset( P );
		}
		void plug( qASd *AS )
		{
			Automat.plug( AS );
		}
		excluder_ &operator =( const excluder_ &E )
		{
			Automat = E.Automat;

			return *this;
		}
		void Init( void )
		{
			Automat.Init();
		}
		void Init( const dwtbsc::ghosts_oddities_ &GO )
		{
			Init();

			if ( GO.Suffix.Amount() != 0 )
				_AddGhostSuffix( GO.Suffix );
		}
		void AddExtension( const str::string_ &Extension );
		void AddDirectory( const str::string_ &Directory );
		void Add( const str::string_ &DirectoryOrExtension );
		state__ GetState(
			const str::string_ &DirectoryOrFile,
			bso::bool__ ForceAsDirectory ) const;
	};

	E_AUTO( excluder );
}
#endif
