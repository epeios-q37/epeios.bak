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

// File WaTcher MOVings

#ifndef FWTMOV__INC
# define FWTMOV__INC

# include "fwtdct.h"

# include "xml.h"
# include "txf.h"

namespace fwtmov {

	// Version du contenu du flux XML.
	enum version__ {
		v0_1,
		v0_2,
		v_amount,
		v_Current = v_amount - 1,
		v_Undefined
	};

	const char *GetLabel( version__ Version );

	version__ GetVersion( const str::string_ &Pattern );

	using fwtbsc::name_;
	using fwtbsc::nrow__;
	using fwtbsc::names_;

	using fwtbsc::depth__;

	struct move__
	{
		nrow__
			Old,		// Nom de l'ancien chemin/nom courant du répertoire.
			New;		// Nom du nouveau chemin/nom du répertoire.
		void reset( bso::bool__ = true )
		{
			Old = New = qNIL;
		}
		E_CDTOR( move__ );
		void Init( void )
		{
			Old = New = qNIL;
		}
	};

	qROW( mrow__ );	// Moving row.

	typedef bch::E_BUNCHt_( move__, mrow__ ) moves_;
	E_AUTO( moves );

	class movings_
	{
	public:
		struct s {
			moves_::s Moves;
			names_::s Names;
		};
		moves_ Moves;
		names_ Names;
		movings_( s &S )
		: Moves( S.Moves ),
		  Names( S.Names )
		{}
		void reset( bso::bool__ P = true )
		{
			Moves.reset( P );
			Names.reset( P );
		}
		void plug( qAS_ &AS )
		{
			Moves.plug( AS );
			Names.plug( AS );
		}
		movings_ &operator =(const movings_ &M)
		{
			Moves = M.Moves;
			Names = M.Names;

			return *this;
		}
		void Init( void )
		{
			Moves.Init();
			Names.Init();
		}
		E_NAVt( Moves., mrow__ );
		void GetMove(
			mrow__ Row,
			str::string_ &Old,
			str::string_ &New ) const
		{
			ctn::E_CMITEMt( name_, nrow__ ) Name;

			Name.Init( Names );

			if ( Moves( Row ).Old != qNIL )
				Old = Name( Moves( Row ).Old );

			if ( Moves( Row ).New != qNIL )
				New = Name( Moves( Row ).New );
		}
	};

	E_AUTO( movings );

	void Explore(
		const str::string_ &Root,
		const fwtdct::content_ &Content,
		const fwtbsc::ghosts_oddities_ &GO,
		movings_ &Movings );

	void Load(
		xml::parser___ &Parser,
		version__ Version,
		movings_ &Movings );

	void Dump(
		const movings_ &Movings,
		xml::writer_ &Writer,
		version__ Version = v_Current );

	void Display(
		const movings_ &Movings,
		txf::text_oflow__ &Flow );

	typedef bch::E_BUNCH_( mrow__ ) mrows_;
	E_AUTO( mrows );

	bso::bool__ Apply(
		const str::string_ &Path,
		const movings_ &Movings,
		fwtbsc::tamount__ ThreadAmountMax,
		mrows_ &Failures );	// Return false if failure, 'Failures' containing the failed ones.

	void Display(
		const movings_ &Movings,
		const mrows_ &Rows,
		txf::text_oflow__ &Flow );
}

#endif
