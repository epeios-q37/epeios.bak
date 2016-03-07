/*
	Copyright (C) 1999-2016 Claude SIMON (http://q37.info/contact/).

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

// Directory WaTcher GHosTs

#ifndef DWTGHT__INC
# define DWTGHT__INC

# define DWTGHT_NAME		"DWTGHT"

# if defined( E_DEBUG ) && !defined( DWTGHT_NODBG )
#  define DWTGHT_DBG
# endif

# include "dwtbsc.h"

# include "lstctn.h"

namespace dwtght {
	using dwtbsc::depth__;

	using dwtbsc::grow__;

	inline grow__ GetGhostRow(
		const str::string_ &Pattern,
		const dwtbsc::ghosts_oddities_ &GO )
	{
		sdr::row__ Error = qNIL;
		grow__ Row = qNIL;

		if ( ( Pattern.Amount() > ( GO.Prefix.Amount() + GO.Suffix.Amount() + 2 ) )	// '+2' pour les '.' ajoutés automatiquement.
			 && ( str::Compare( Pattern, GO.Prefix, Pattern.First(), GO.Prefix.First(), GO.Prefix.Amount() ) == 0 ) ) {

			Row = Pattern.ToUInt( Pattern.First( GO.Suffix.Amount() + 1 ), &Error, str::bAuto );

			if ( ( Error != qNIL ) && ( Pattern( Error ) != '.' ) )
				Row = qNIL;
		}

		return Row;
	}

	using str::cHook;

	// Pour ma gestion du renommage/déplacement de répertoires.
	class ghost_
	{
	public:
		struct s {
			grow__ Parent;	// Référence au 'ghost' parent (c'est-à-dire au 'ghost' contenant les données du répertoire parent.
			str::string_::s Name;
		} &S_;
		str::string_ Name;
		ghost_( s &S )
		: S_( S ),
		  Name( S.Name )
		{}
		void reset( bso::bool__ P = true )
		{
			S_.Parent = qNIL;
			Name.reset( P );
		}
		void plug( cHook &Hook )
		{
			Name.plug( Hook );
		}
		void plug( qASd &AS )
		{
			Name.plug( AS );
		}
		ghost_ &operator =( const ghost_ &G )
		{
			S_.Parent = G.S_.Parent;
			Name = G.Name;

			return *this;
		}
		void Init( void )
		{
			S_.Parent = qNIL;

			Name.Init();
		}
	};

	E_AUTO( ghost );


	typedef lstctn::E_LXMCONTAINERt_( ghost_, grow__ ) ghosts_;
	E_AUTO( ghosts );

	using lstctn::rFH;

	struct rRack {
		rFH FilesHook;
		ghosts Ghosts;
		flsq::rId Id;
		void reset( bso::bool__ P = true )
		{
			FilesHook.reset( P );
			Ghosts.reset( P );

			if ( P ) {
				if ( Id != flsq::Undefined )
					flsq::ReleaseId( Id );
			}

			Id = flsq::Undefined;
		}
		qCDTOR( rRack );
		void Init( void )
		{
			if ( Id != flsq::Undefined )
				flsq::ReleaseId( Id );

			Id = flsq::GetId();

			Ghosts.reset();
			FilesHook.reset();

			// L'initialisation proprement dite sera réalisée par des fonctions dédiées.
		}
	};

	enum status__ {
		sCreated,
		sSkipped,
		sUpdated,
		sFailed,
		s_amount,
		s_Undefined
	};

	inline const fnm::name___ &GetGhostLocalizedName(
		grow__ Row,
		const str::string_ &Root,
		const str::string_ &Path,
		const dwtbsc::ghosts_oddities_ &GO,
		fnm::name___ &LocalizedName )
	{
		return dwtbsc::GetGhostLocalizedName( *Row, Root, Path, GO, LocalizedName );
	}

	ghosts_ &GetRWGhosts(
		const str::string_ &Root,
		const dwtbsc::ghosts_oddities_ &GO,
		rRack &Rack );

	const ghosts_ &GetROGhosts(
		const str::string_ &Root,
		const dwtbsc::ghosts_oddities_ &GO,
		rRack &Rack );

	status__ CreateGhost(
		const str::string_ &Root,
		const str::string_ &Path,
		const str::string_ &BaseName,
		const dwtbsc::ghosts_oddities_ &GO,
		grow__ Parent,
		ghosts_ &Ghosts,
		grow__ &Row );	// Crée un 'ghost', et retourne son 'Row' dans 'Ghosts' si effectivement crée (ce que l'on sait grâce au 'status__' retourné).

	const str::string_ &GetPath(
		grow__ Row,
		const ghosts_ &Ghosts,
		str::string_ &Path,
		depth__ &Depth );

	inline const str::string_ &GetPath(
		grow__ Row,
		const ghosts_ &Ghosts,
		str::string_ &Path )
	{
		depth__ Dummy = dwtbsc::UndefinedDepth;

		return GetPath( Row, Ghosts, Path, Dummy );
	}

	void ShowGhosts(
		const str::string_ &Root,
		const dwtbsc::ghosts_oddities_ &GO,
		txf::text_oflow__ &TFlow );
}

#endif
