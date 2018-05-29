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

#define GNUMLL__COMPILATION

#include "gnumll.h"

enum tag {
	tEmailAndName,
	tEmailOnly,
	tNewsgroup,
	tContribution,
	tAnnouncement,
	tFAQ_URL,
	tOtherEmail,
	amount,
	tUnknow = amount	// Must be last, just after 'amount'.
};

const char *Tag[] = {
	" to subscribe to ",
	" to subscribe",
	"gnUSENET newsgroup: ",
	"Send contributions to: ",
	"Send announcements to: ",
	"FAQ-URL: ",
	" to: ",
};

static tag GetType_(
	const str::string_ &L,
	epeios::row__ &P,
	bso::bsize__ &S )
{
	int t = 0;

	while( ( t < amount )
		   && ( ( P = L.Search( str::string( Tag[t] ) ) ) == NONE ) )
		t++;

	if ( t != amount )
		S = strlen( Tag[t] );

	return (tag)t;
}
	

static const str::string_ &FilterTitlePrefix_( const str::string_ &Title )
{
	static str::string String;
	epeios::row__ P = 0;

	String.Init();

	while( ( *P < Title.Amount() ) && ( Title( P ) == '*' ) )
		(*P)++;

	if ( ( *P < Title.Amount() ) && ( Title( P ) == ' ' ) )
		(*P)++;

	String.StoreAndAdjust( Title, Title.Amount() - *P, *P );

	return String;
}

static void GetListSectionHeader_(
	const txmpgh::paragraph_ &Paragraph,
	gnumll::section_header_ &Header )
{
	ctn::E_CMITEM( str::string_ ) L;
	epeios::row__ P = Paragraph.First();

	L.Init( Paragraph );

	Header.Init();

	while( P != NONE ) {
		Header.Parse( FilterTitlePrefix_( L( P ) ) );
		P = Paragraph.Next( P );
	}
}

static void GetSection_(
	txmpgh::paragraph_ &Paragraph,
	gnumll::section_ &Section,
	xtf::extended_text_iflow__ &Flow )
{
	bso::bool__ Cont = true;
	ctn::E_CMITEM( str::string_ ) Item;

	do {
		Paragraph.Init();

		Paragraph.Parse( Flow, txmpgh::sSplit );

		Item.Init( Paragraph );

		if ( ( Paragraph.Amount() ) && ( Item( Paragraph.First() )( 0 ) != '*' ) )
			Section.Append( Paragraph );
		else
			Cont = false;

		Paragraph.Flush();
	
	} while ( Cont );
}


static void GetHead_(
	txmpgh::paragraph_ &Paragraph,
	gnumll::head_section_ &Head,
	xtf::extended_text_iflow__ &Flow )
{
	bso::bool__ Cont = true;

	do {
		Paragraph.Init();

		Paragraph.Parse( Flow, txmpgh::sSplit );

		if ( ( Paragraph.Amount() ) && ( Paragraph( Paragraph.First() )( 0 ) != '*' ) )
			Head.Append( Paragraph );
		else
			Cont = false;

		Paragraph.Flush();

	} while ( Cont );

}

static void GetGeneralSections_(
	txmpgh::paragraph_ &Paragraph,
	gnumll::general_sections_ &Generals,
	xtf::extended_text_iflow__ &Flow )
{
qRH
	gnumll::general_section General;
qRB

	do {
		General.Init();
		General.Title = FilterTitlePrefix_( Paragraph( 0 ) );

		Paragraph.Flush();

		GetSection_( Paragraph, General.Section, Flow );

		Generals.Append( General );
	} while ( Paragraph.Amount() == 1 );
qRR
qRT
qRE
}

static void GetListSections_(
	txmpgh::paragraph_ &Paragraph,
	gnumll::list_sections_ &Lists,
	xtf::extended_text_iflow__ &Flow )
{
qRH
	gnumll::list_section List;
qRB

	do {
		List.Init();

		GetListSectionHeader_( Paragraph, List.Header );

		GetSection_( Paragraph, List.Section, Flow );

		if ( List.Section.Amount() )
			Lists.Append( List );
	} while ( !Flow.EOX() );
qRR
qRT
qRE
}

static inline void Correct_( gnumll::section_ &Section )
{
	if ( Section.Amount() )
		Section.Allocate( Section.Amount() - 1 );
}

static inline void WriteEnd_(
	const str::string_ &Source,
	str::string_ &Dest,
	epeios::row__ P,
	bso::bsize__ S )
{
	Dest.StoreAndAdjust( Source, Source.Amount() - ( *P + S ), *P + S );
}

static bso::bool__ Test_( const str::string_ &S )
{
	bso::bool__ R = false;
qRH
	str::string T;
qRB
	T.Init();

	T = S;

	str::ToUpper( T );

	R = ( T == str::string( "NONE" ) ) || ( T == str::string( "NEVER" ) );
qRR
qRT
qRE
	return R;
}

static void HandleNewsgroup_(
	const str::string_ &Source,
	epeios::row__ P,
	bso::bsize__ S,
	str::string_ &NG,
	str::string_ &NGC )
{
qRH
	str::string T;
	epeios::size__ L;
	bso::char__ C;
qRB
	NG.Init();
	NGC.Init();

	T.Init();

	WriteEnd_( Source, T, P, S );

	L = T.Amount();

	while( ( *P < L ) && ( !isspace( C = T( P ) ) ) )
	{
		NG.Append( C );
		(*P)++;
	}

	if ( Test_( NG ) ) {
		NG.Init();
		NGC = T;
	}
	else if ( P != L ) {
		while( ( *P < L ) && ( isspace( T( P ) ) ) )
			(*P)++;

		if ( P != L )
			while( *P < L )
				{
					NGC.Append( T( P ) );
					(*P)++;
				}
	}
qRR
qRT
qRE
}


void gnumll::section_header_::Parse( const str::string_ &L )
{
	epeios::row__ P;
	bso::bsize__ S;

	switch( GetType_( L, P, S ) ) {
	case tEmailAndName:
		WriteEnd_( L, Name, P, S );
	case tEmailOnly:
		Email.StoreAndAdjust( L, *P, 0 );
		break;
	case tNewsgroup:
		HandleNewsgroup_( L, P, S, Newsgroup, NewsgroupComment );
		break;
	case tAnnouncement:
		WriteEnd_( L, Announcement, P, S );
		break;
	case tContribution:
		WriteEnd_( L, Contribution, P, S );
		break;
	case tFAQ_URL:
		WriteEnd_( L, FAQ_URL, P, S );
		break;
	case tOtherEmail:
		TextOtherEmail.StoreAndAdjust( L, *P + S );
		WriteEnd_( L, OtherEmail, P, S );
		break;
	case tUnknow:
		Misc.Append( L );
		break;
	default:
		ERRc();
		break;
	}
}

void gnumll::mailing_lists_::Parse( xtf::extended_text_iflow__ &Flow )
{
qRH
txmpgh::paragraph Paragraph;
qRB
	Paragraph.Init();

	GetHead_( Paragraph, Head, Flow );

	GetGeneralSections_( Paragraph, Generals, Flow );

	GetListSections_( Paragraph, Lists, Flow );

	if ( Lists.Amount() )
	{
		Correct_( Lists( Lists.Last() ).Section );
		Lists.Flush();
	}
qRR
qRT
qRE
}

template <class t> void Print_(
	txf::text_oflow__ &F,
	const ctn::E_CONTAINER_( t ) &C )
{
	epeios::row__ P = C.First();
	ctn::E_CITEM( t ) Element;

	Element.Init( C );

	while( P != NONE ) {
		F << "> " << Element( P );
		P = C.Next( P );
	}
}


txf::text_oflow__ &operator <<(
	txf::text_oflow__ &F,
	const gnumll::section_ &S )
{
	Print_<txmpgh::paragraph_>( F, S );
	return F;
}

txf::text_oflow__ &operator <<(
	txf::text_oflow__ &F,
	const gnumll::general_sections_ &S )
{
	Print_<gnumll::general_section_>( F, S );
	return F;
}

txf::text_oflow__ &operator <<(
	txf::text_oflow__ &F,
	const gnumll::list_sections_ &S )
{
	Print_<gnumll::list_section_>( F, S );
	return F;
}


txf::text_oflow__ &operator <<(
	txf::text_oflow__ &F,
	const gnumll::titles_ &S )
{
	epeios::row__ P = S.First();
	ctn::E_CMITEM( str::string_ ) Element;

	Element.Init( S );

	while( P != NONE ) {
		F << txf::tab << Element( P ) << txf::nl;
		P = S.Next( P );
	}

	return F;
}

