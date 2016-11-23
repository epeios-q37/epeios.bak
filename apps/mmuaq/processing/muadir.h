/*
	Copyright (C) 2016 Claude SIMON (http://zeusw.org/epeios/contact.html).

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

// MUA DIRectory

#ifndef MUADIR__INC
# define MUADIR__INC

# ifdef XXX_DBG
#	define MUADIR__DBG
# endif

# include "muabsc.h"
# include "muamel.h"
# include "muafld.h"

# include "bch.h"
# include "crt.h"

namespace muadir {
	// Link from mail to folder.
	typedef bch::qBUNCHd( muafld::sRow, muamel::sRow ) dM2F_;

	// Link From folder 2 mails.
	typedef crt::qCRATEd( muamel::dRows, muafld::sRow ) dF2M_;

	class dDirectory
	{
	private:
		void Adjust_( void )
		{
			M2F.Allocate( Mails.Amount() );
			F2M.Allocate( Folders.Amount() );
		}
	public:
		struct s {
			muamel::dMails::s Mails;
			muafld::dFoldersTree::s Folders;
			dM2F_::s M2F;
			dF2M_::s F2M;
			muafld::sRow Inbox;
		} &S_;
		muamel::dMails Mails;
		muafld::dFoldersTree Folders;
		dM2F_ M2F;
		dF2M_ F2M;
		dDirectory( s &S )
		: S_( S ),
		  Mails( S.Mails ),
		  Folders( S.Folders ),
		  M2F( S.M2F ),
		  F2M( S.F2M )
		{}
		void reset( bso::bool__ P = true )
		{
			tol::reset( P, Mails, Folders, M2F, F2M  );
		}
		void plug( qASd *AS )
		{
			tol::plug( AS, Mails, Folders, M2F, F2M  );
		}
		dDirectory &operator =( const dDirectory &D )
		{
			Mails = D.Mails;
			Folders = D.Folders;
			M2F = D.M2F;
			F2M = D.F2M;
			S_.Inbox = D.S_.Inbox;

			return *this;
		}
		void Init( void )
		{
			tol::Init( Mails, Folders, M2F, F2M  );

			S_.Inbox = Folders.CreateChild( str::wString(), qNIL );
		}
		muafld::sRow CreateFolder(
			const str::dString &Name,
			muafld::sRow Parent )
		{
			muafld::sRow Row = Folders.CreateChild( Name, Parent );
			Adjust_();

			F2M( Row ).Init();
			F2M.Flush();

			return Row;
		}
		void GetFolders(
			muafld::sRow Folder,
			muafld::dRows &Folders ) const
		{
			this->Folders.GetFolders( Folder, Folders );
		}
		muamel::sRow AddMail( const str::dString &Id )
		{
			muamel::sRow Row = Mails.New();

			Mails( Row ).Init( Id );
			Mails.Flush();

			Adjust_();

			M2F.Store( S_.Inbox, Row );

			F2M( S_.Inbox ).Add( Row );
			F2M.Flush();
		}
		void GetFoldersNames(
			const muafld::dRows &Folders,
			str::dStrings &Names ) const;
		void GetAllMails( muamel::dRows &Rows ) const;
	};

	qW( Directory );
}


#endif
