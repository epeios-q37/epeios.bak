/*
	Copyright (C) 2015 Claude SIMON (http://q37.info/contact/).

	This file is part of fwtchrq.

    fwtchrq is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

    fwtchrq is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with fwtchrq.  If not, see <http://www.gnu.org/licenses/>
*/

// File WaTcher eXCLusion

#ifndef FWTXCL__INC
# define FWTXCL__INC

# ifdef XXX_DBG
#  define FWTXCL__DBG
# endif

# include "fwtbsc.h"

# include "stsfsm.h"
# include "ags.h"

namespace fwtxcl {
	namespace {
		typedef stsfsm::automat_ _automat_;
	}

	enum state__ {
		sExcludedFile,
		sExcludedDir,
		sGhostDir,	// Répertoire fantôme pour la gestion du renommage de répertoire.
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
		void plug( qAS_ &AS )
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
		void Init( const fwtbsc::ghosts_oddities_ &GO )
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
