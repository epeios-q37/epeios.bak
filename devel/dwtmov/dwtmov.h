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

#ifndef DWTMOV__INC
# define DWTMOV__INC

# define DWTMOV_NAME		"DWTMOV"

# if defined( E_DEBUG ) && !defined( DWTMOV_NODBG )
#  define DWTMOV_DBG
# endif

// Directory WaTcher MOVings

# include "dwtbsc.h"
# include "dwtdct.h"

# include "err.h"
# include "str.h"
# include "xml.h"

namespace dwtmov {
	// XML flow content version.
	enum version__ {
		v0_1,
		v0_2,
		v_amount,
		v_Current = v_amount - 1,
		v_Undefined
	};

	const char *GetLabel( version__ Version );

	version__ GetVersion( const str::string_ &Pattern );

	using dwtbsc::name_;
	using dwtbsc::nrow__;
	using dwtbsc::names_;

	using dwtbsc::depth__;

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
		const dwtdct::content_ &Content,
		const dwtbsc::ghosts_oddities_ &GO,
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

	class callback__
	{
	protected:
		virtual bso::bool__ DWTMOVCreate( const str::string_ &Path ) = 0;
		virtual bso::bool__ DWTMOVMove(
			const str::string_ &OldPath,
			const str::string_ &NewPath ) = 0;
		virtual bso::bool__ DWTMOVDelete( const str::string_ &Path ) = 0;
	public:
		void reset( bso::bool__ = true )
		{
			// Standadisation.
		}
		E_CVDTOR( callback__ );
		void Init( void )
		{
			// Standardisation.
		}
		bso::bool__ Create( const str::string_ &Path )
		{
			return DWTMOVCreate( Path );
		}
		bso::bool__ Move(
			const str::string_ &OldPath,
			const str::string_ &NewPath )
		{
			return DWTMOVMove( OldPath, NewPath );
		}
		bso::bool__ Delete( const str::string_ &Path )
		{
			return DWTMOVDelete( Path );
		}
	};

	bso::bool__ Apply(
		const str::string_ &Path,
		const movings_ &Movings,
		dwtbsc::tamount__ ThreadAmountMax,
		mrows_ &Failures );	// Return false if failure, 'Failures' containing the failed ones.

	void Display(
		const movings_ &Movings,
		const mrows_ &Rows,
		txf::text_oflow__ &Flow );

}

#endif
