/*
	Copyright (C) 2016-2017 Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of 'MMUAq' software.

    'MMUAq' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'MMUAq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'MMUAq'.  If not, see <http://www.gnu.org/licenses/>.
*/

// MUA FoLDer

#ifndef MUAFLD_INC_
# define MUAFLD_INC_

# ifdef XXX_DBG
#	define MUAFLD__DBG
# endif

# include "muabsc.h"

# include "dtr.h"
# include "lstcrt.h"

namespace muafld {
	class dFolder {
	public:
		struct s {
			str::dString::s Name;
		};
		str::dString Name;
		dFolder( s &S )
		: Name( S.Name )
		{}
		void reset( bso::sBool P = true )
		{
			tol::reset( P, Name );
		}
		void plug( uys::sHook &Hook )
		{
			Name.plug( Hook );
		}
		void plug( qASd *AS )
		{
			tol::plug( AS, Name );
		}
		dFolder &operator =(const dFolder &F)
		{
			Name = F.Name;

			return *this;
		}
		void Init( const str::dString &Name )
		{
			this->Name.Init( Name );
		}
	};

	qW( Folder );

	qROW( Row );
	qROWS( Row );

	typedef lstcrt::qLMCRATEd( dFolder, sRow ) dFolders_;
	qW( Folders_ );

	typedef dtr::qDTREEd( sRow ) dTree_;
	qW( Tree_ );

	class dFoldersTree
	: public dTree_
	{
	private:
		dFolders_ Folders_;
	public:
		struct s
		: public dTree_::s
		{
			dFolders_::s Folders;
			sRow Root;
		} &S_;
		dFoldersTree( s &S )
		: S_( S ),
		  dTree_( S ),
		  Folders_( S.Folders )
		{}
		void reset( bso::sBool P = true )
		{
			dTree_::reset( P );
			tol::reset( P, Folders_ );

			S_.Root = qNIL;
		}
		void plug( qASd *AS )
		{
			dTree_::plug( AS );
			tol::plug( AS, Folders_ );
		}
		dFoldersTree &operator =(const dFoldersTree &FD)
		{
			dTree_::operator =( FD );
			Folders_ = FD.Folders_;
			S_.Root = FD.S_.Root;

			return *this;
		}
		// Returns the root.
		sRow Init( void )
		{
			dTree_::Init();
			tol::Init( Folders_ );

			S_.Root = Folders_.New();
			Folders_( S_.Root ).Init( str::wString() );
			dTree_::Allocate( 1 );

			return S_.Root;
		}
		sRow Search(
			sRow Folder,
			const str::dString &Name ) const;
		bso::sBool IsDescendant(
			sRow Folder,
			sRow Ascendant ) const;
		bso::sBool Exists( sRow Row ) const
		{
			return Folders_.Exists( Row );
		}
		const str::dString &GetName(
			sRow Row,
			str::dString &Name ) const
		{
			Name.Append( Folders_( Row ).Name );

			return Name;
		}
		void GetNames(
			const dRows &Folders,
			str::dStrings &Names ) const;
		// if 'Row' ==qNIL, return the root row.
		const dRows &GetChildren(
			sRow Row,
			dRows &Rows ) const;
		sRow CreateChild(
			const str::dString &Name,
			sRow Parent )
		{
			sRow Row = Folders_.New();

			Folders_( Row ).Init( Name );

			dTree_::Allocate( Folders_.Amount() );

			BecomeLastChild( Row, Parent );

			return Row;
		}
		void Rename(
			sRow Row,
			const str::dString &Name )
		{
			Folders_(Row).Init( Name );
		}
		void MoveTo(
			sRow Folder,
			sRow Parent )
		{
			Cut( Folder );

			BecomeLastChild( Folder, Parent );
		}
	};
}


#endif
