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

//	$Id: gnumll.h,v 1.11 2012/11/14 16:06:29 csimon Exp $

#ifndef GNUMLL__INC
#define GNUMLL__INC

#define GNUMLL_NAME		"GNUMLL"

#define	GNUMLL_VERSION	"$Revision: 1.11 $"

#define GNUMLL_OWNER		"Claude SIMON (http://zeusw.org/intl/contact.html)"


#if defined( E_DEBUG ) && !defined( GNUMLL_NODBG )
#define GNUMLL_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.11 $
//C Claude SIMON (http://zeusw.org/intl/contact.html)
//R $Date: 2012/11/14 16:06:29 $

/* End of automatic documentation generation part. */

/* Addendum to the automatic documentation generation part. */
//D GNU MaiLing List 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "txf.h"
#include "txmpgh.h"

namespace gnumll {

	typedef ctn::E_XCONTAINER_( txmpgh::paragraph_ )	section_;
	typedef ctn::E_XCONTAINER( txmpgh::paragraph_ )		section;

	//t The before section
	typedef section_ head_section_;
	typedef section	head_section;

	//c The general section
	class general_section_
	{
	public:
		//o Title of the section.
		str::string_ Title;
		//o The section itself.
		section_ Section;
		struct s {
			str::string_::s Title;
			section_::s Section;
		};
		general_section_( s &S )
		: Title( S.Title ),
		  Section( S.Section )
		{}
		void reset( bool P = true )
		{
			Title.reset( P );
			Section.reset( P );
		}
		void plug( mmm::multimemory_ &M )
		{
			Title.plug( M );
			Section.plug( M );
		}
		general_section_ &operator =( const general_section_ &S )
		{
			Title = S.Title;
			Section = S.Section;

			return *this;
		}
		//f Initialization.
		void Init( void )
		{
			Title.Init();
			Section.Init();
		}
	};

	E_AUTO( general_section )



	typedef ctn::E_XCONTAINER( general_section_ ) general_sections;
	typedef ctn::E_XCONTAINER_( general_section_ ) general_sections_;

	typedef ctn::E_XMCONTAINER_( str::string_ ) misc_;
	typedef ctn::E_XMCONTAINER( str::string_ ) misc;

	//c Section header.
	class section_header_
	{
	public:
		struct s {
			str::string_::s Name;
			str::string_::s Email;
			str::string_::s OtherEmail;
			str::string_::s TextOtherEmail;
			str::string_::s Newsgroup;
			str::string_::s NewsgroupComment;
			str::string_::s Contribution;
			str::string_::s Announcement;
			str::string_::s FAQ_URL;
			misc_::s Misc;
		};
		//o Name of the mailing-list.
		str::string_ Name;
		//o E-mail of the mailing-list.
		str::string_ Email;
		//o Other E-mail (bug reports, source code, articles, ...)
		str::string_ OtherEmail;
		//o Text associated to the other E-mail.
		str::string_ TextOtherEmail;
		//o Newsgroup name.
		str::string_ Newsgroup;
		//o Newsgroup comment.
		str::string_ NewsgroupComment;
		//o Contribution e-mail
		str::string_ Contribution;
		//o Announcement e-mail
		str::string_ Announcement;
		//o The FAQ_URL
		str::string_ FAQ_URL;
		//o Misc.
		misc_ Misc;
		section_header_( s &S )
		: Name( S.Name ),
		  Email( S.Email ),
		  OtherEmail( S.OtherEmail ),
		  TextOtherEmail( S.TextOtherEmail ),
		  Newsgroup( S.Newsgroup ),
		  NewsgroupComment( S.NewsgroupComment ),
		  Contribution( S.Contribution ),
		  Announcement( S.Announcement ),
		  FAQ_URL( S.FAQ_URL ),
		  Misc( S.Misc )
		{}
		void reset( bool P = true )
		{
			Name.reset( P );
			Email.reset( P );
			OtherEmail.reset( P );
			TextOtherEmail.reset( P );
			Newsgroup.reset( P );
			NewsgroupComment.reset( P );
			Contribution.reset( P );
			Announcement.reset( P );
			FAQ_URL.reset( P );
			Misc.reset( P );
		}
		void plug( mmm::multimemory_ & M )
		{
			Name.plug( M );
			Email.plug( M );
			OtherEmail.plug( M );
			TextOtherEmail.plug( M );
			Newsgroup.plug( M );
			NewsgroupComment.plug( M );
			Contribution.plug( M );
			Announcement.plug( M );
			FAQ_URL.plug( M );
			Misc.plug( M );
		}
		section_header_ &operator =( const section_header_ &H )
		{
			Name = H.Name;
			Email = H.Email;
			OtherEmail = H.OtherEmail;
			TextOtherEmail = H.TextOtherEmail;
			Newsgroup = H.Newsgroup;
			NewsgroupComment = H.NewsgroupComment;
			Contribution = H.Contribution;
			Announcement = H.Announcement;
			FAQ_URL = H.FAQ_URL;
			Misc = H.Misc;

			return *this;
		}
		//f Initialization
		void Init( void )
		{
			Name.Init();
			Email.Init();
			OtherEmail.Init();
			TextOtherEmail.Init();
			Newsgroup.Init();
			NewsgroupComment.Init();
			Contribution.Init();
			Announcement.Init();
			FAQ_URL.Init();
			Misc.Init();
		}
		//f Parse the line 'Line'.
		void Parse( const str::string_ &L );
	};

	// internal use.
	inline void Print_(
		const str::string_ &S,
		const char *L,
		txf::text_oflow__ &F )
	{
		if ( S.Amount() )
			F << txf::tab << L << ": >" << S << '<' << txf::nl;
	}

	typedef ctn::E_XMCONTAINER_( str::string_ ) titles_;
	typedef ctn::E_XMCONTAINER( str::string_ ) titles;


	//c A list section.
	class list_section_
	{
	public:
		//o Section header.
		section_header_ Header;
		//o The section itself.
		section_ Section;
		struct s {
			section_header_::s Header;
			section_::s Section;
		};
		list_section_( s &S )
		: Header( S.Header ),
		  Section( S.Section )
		{}
		void reset( bool P = true )
		{
			Header.reset( P );
			Section.reset( P );
		}
		void plug( mmm::multimemory_ &M )
		{
			Header.plug( M );
			Section.plug( M );
		}
		list_section_ &operator =( const list_section_ &S )
		{
			Header = S.Header;
			Section = S.Section;

			return *this;
		}
		//f Initialization.
		void Init( void )
		{
			Header.Init();
			Section.Init();
		}
	};

	E_AUTO( list_section );

	typedef ctn::E_XCONTAINER_( list_section_ ) list_sections_;
	typedef ctn::E_XCONTAINER( list_section_ )	list_sections;



	//c A GNU mailing lists.
	class mailing_lists_
	{
	public:
		//o The head section.
		head_section_ Head;
		//o the general sections.
		general_sections_ Generals;
		//o The list sections.
		list_sections_ Lists;
		struct s {
			head_section_::s Head;
			general_sections_::s Generals;
			list_sections_::s Lists;
		};
		mailing_lists_( s &S )
		: Head( S.Head ),
		  Generals( S.Generals ),
		  Lists( S.Lists )
		{}
		void reset( bool P = true )
		{
			Head.reset( P );
			Generals.reset( P );
			Lists.reset( P );
		}
		void plug( mmm::multimemory_ &M )
		{
			Head.plug( M );
			Generals.plug( M );
			Lists.plug( M );
		}
		mailing_lists_ &operator =( const mailing_lists_ &M )
		{
			Head = M.Head;
			Generals = M.Generals;
			Lists = M.Lists;

			return *this;
		}
		//f Initialization.
		void Init( void )
		{
			Head.Init();
			Generals.Init();
			Lists.Init();
		}
		//f Parse the flow 'Flow'.
		void Parse( xtf::extended_text_iflow__ &Flow );
	};

	E_AUTO( mailing_lists )
}

txf::text_oflow__ &operator <<(
	txf::text_oflow__ &F,
	const gnumll::section_ &S );
	
inline txf::text_oflow__ &operator <<(
	txf::text_oflow__ &F,
	const gnumll::general_section_ &S )
{
	return F << "-----> Title: " << S.Title << txf::nl << S.Section << txf::nl;
}

txf::text_oflow__ &operator <<(
	txf::text_oflow__ &F,
	const gnumll::misc_ &S );

inline txf::text_oflow__ &operator <<(
	txf::text_oflow__ &F,
	const gnumll::section_header_ &S )
{
	gnumll::Print_( S.Name, "Name", F );
	gnumll::Print_( S.Email, "E-mail", F );
	gnumll::Print_( S.OtherEmail, "Other e-mail", F );
	gnumll::Print_( S.TextOtherEmail, "Text other e-mail", F );
	gnumll::Print_( S.Newsgroup, "Newsgroup", F );
	gnumll::Print_( S.NewsgroupComment, "Newsgroup comment", F );
	gnumll::Print_( S.Contribution, "Contribution", F );
	gnumll::Print_( S.FAQ_URL, "FAQ URL", F );
	gnumll::Print_( S.Announcement, "Announcement", F );

	if ( S.Misc.Amount() )
		F << txf::nl << "Misc:" << txf::nl << S.Misc << txf::nl;

	return F;
}
	
txf::text_oflow__ &operator <<(
	txf::text_oflow__ &F,
	const gnumll::titles_ &S );

txf::text_oflow__ &operator <<(
	txf::text_oflow__ &F,
	const gnumll::general_sections_ &S );

inline txf::text_oflow__ &operator <<(
	txf::text_oflow__ &F,
	const gnumll::list_section_ &S )
{
	F << "---> Header:" << txf::nl << S.Header << txf::nl;
	return F << S.Section << txf::nl;
}

txf::text_oflow__ &operator <<(
	txf::text_oflow__ &F,
	const gnumll::list_sections_ &S );


inline txf::text_oflow__ &operator <<(
	txf::text_oflow__ &F,
	const gnumll::mailing_lists_ &L )
{
	F << "----- HEAD -----" << txf::nl << L.Head << txf::nl;
	F << "----- GENERAL -----" << txf::nl << L.Generals << txf::nl;
	F << "----- LISTS -----" << txf::nl << L.Lists << txf::nl;
	return F;
}

/*$END$*/
#endif
