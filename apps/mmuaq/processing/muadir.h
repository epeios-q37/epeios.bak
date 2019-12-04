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

// MUA DIRectory

#ifndef MUADIR_INC_
# define MUADIR_INC_

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
		muamel::dMails Mails_;
		muafld::dFoldersTree Folders_;
		dM2F_ M2F_;
		dF2M_ F2M_;
		void Adjust_( void )
		{
			M2F_.Allocate( Mails_.Amount() );
			F2M_.Allocate( Folders_.Amount() );
		}
		bso::sBool IsSpecial_( muafld::sRow Folder ) const
		{
			return ( S_.Root == Folder ) || ( S_.Inbox == Folder );
		}
	public:
		struct s {
			muamel::dMails::s Mails;
			muafld::dFoldersTree::s Folders;
			dM2F_::s M2F;
			dF2M_::s F2M;
			muafld::sRow
				Root,
				Inbox;
		} &S_;
		dDirectory( s &S )
		: S_( S ),
		  Mails_( S.Mails ),
		  Folders_( S.Folders ),
		  M2F_( S.M2F ),
		  F2M_( S.F2M )
		{}
		void reset( bso::bool__ P = true )
		{
			tol::reset( P, Mails_, Folders_, M2F_, F2M_  );

			S_.Root = qNIL;
			S_.Inbox = qNIL;
		}
		void plug( qASd *AS )
		{
			tol::plug( AS, Mails_, Folders_, M2F_, F2M_  );
		}
		dDirectory &operator =( const dDirectory &D )
		{
			Mails_ = D.Mails_;
			Folders_ = D.Folders_;
			M2F_ = D.M2F_;
			F2M_ = D.F2M_;

			S_.Root = D.S_.Root;
			S_.Inbox = D.S_.Inbox;

			return *this;
		}
		void Init( void )
		{
			tol::Init( Mails_, M2F_, F2M_  );

			S_.Root = Folders_.Init();
			S_.Inbox = Folders_.CreateChild( str::wString(), S_.Root );
		}
		qRODISCLOSEd( muafld::sRow, Root );
		qRODISCLOSEd( muafld::sRow, Inbox );
		const muafld::dFoldersTree &Folders( void ) const
		{
			return Folders_;
		}
		const muamel::dMails &Mails( void ) const
		{
			return Mails_;
		}
		bso::sBool Exists( muafld::sRow Folder ) const
		{
			return Folders_.Exists( Folder );
		}
		bso::sBool IsRenameable( muafld::sRow Folder ) const
		{
			return !IsSpecial_( Folder );
		}
		bso::sBool IsMoveable( muafld::sRow Folder ) const
		{
			return !IsSpecial_( Folder );
		}
		muafld::sRow CreateFolder(
			const str::dString &Name,
			muafld::sRow Parent )	// if 'Parent' == qNIL, The folder is attache to 'Root'.
		{
			if ( Parent == qNIL )
				Parent = S_.Root;

			muafld::sRow Row = Folders_.CreateChild( Name, Parent );
			Adjust_();

			F2M_( Row ).Init();
			F2M_.Flush();

			return Row;
		}
		void RenameFolder(
			muafld::sRow Folder,
			const str::dString &Name )
		{
			Folders_.Rename( Folder, Name );
		}
		muamel::sRow AddMail( const str::dString &Id )
		{
			muamel::sRow Row = Mails_.New();

			Mails_( Row ).Init( Id );
			Mails_.Flush();

			Adjust_();

			M2F_.Store( S_.Inbox, Row );

			F2M_( S_.Inbox ).Add( Row );
			F2M_.Flush();

			return Row;
		}
		void MoveMailTo(
			muamel::sRow Mail,
			muafld::sRow Folder )
		{
			F2M_( M2F_( Mail ) ).Remove( Mail );
			M2F_.Store( Folder, Mail );
			F2M_(Folder).Add( Mail );
			F2M_.Flush();
		}
		void MoveFolderTo(
			muafld::sRow Folder,
			muafld::sRow Parent )
		{
			Folders_.MoveTo( Folder, Parent );
		}
		void Remove( muamel::sRow Mail )
		{
			muafld::sRow Folder = M2F_( Mail );

			if ( Folder == qNIL )
				qRGnr();	// A mail should not be removed twice.

			if ( !F2M_(Folder).Remove( Mail ) )
				qRGnr();

			M2F_.Store( qNIL, Mail );

			Mails_.Remove( Mail );
		}
		void Remove( const muamel::dRows &Mails )
		{
			sdr::sRow Row = Mails.First();

			while ( Row != qNIL ) {
				Remove( Mails( Row ) );

				Row = Mails.Next( Row );
			}
		}
		muamel::dRows &GetMails(
			muafld::sRow Folder,
			muamel::dRows &Mails ) const
		{
			F2M_.Recall( Folder, Mails);

			return Mails;
		}
		muamel::sRow Search(
			const str::dString &Id,
			const muamel::dRows &Rows ) const
		{
			return muamel::Search( Id, Rows, Mails_ );
		}
		const muamel::dIds &GetIds(
			const muamel::dRows &Rows,
			muamel::dIds &UMIDs ) const
		{
			sdr::sRow Row = Rows.First();

			while ( Row != qNIL ) {
				UMIDs.Add( Mails_( Rows( Row ) ).Id );

				Row = Rows.Next( Row );
			}

			return UMIDs;
		}
	};

	qW( Directory );
}


#endif
