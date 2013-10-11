/*
	'tsrcpr' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Requires the 'tsrcpr' header file ('tsrcpr.h').
	Copyright (C) 2000-2004 Claude SIMON (http://zeusw.org/intl/contact.html).

	This file is part of the Epeios (http://zeusw.org/epeios/) project.

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 3
	of the License, or (at your option) any later version.
 
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, go to http://www.fsf.org/
	or write to the:
  
         	         Free Software Foundation, Inc.,
           59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/



//	$Id: tsrcpr.cpp,v 1.18 2012/11/14 16:06:38 csimon Exp $

#define TSRCPR__COMPILATION

#include "tsrcpr.h"

class tsrcprtutor
: public ttr_tutor
{
public:
	tsrcprtutor( void )
	: ttr_tutor( TSRCPR_NAME )
	{
#ifdef TSRCPR_DBG
		Version = TSRCPR_VERSION "\b\bD $";
#else
		Version = TSRCPR_VERSION;
#endif
		Owner = TSRCPR_OWNER;
		Date = "$Date: 2012/11/14 16:06:38 $";
	}
	virtual ~tsrcprtutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace tsrcpr;

#include <ctype.h>

#define isidcar( c )	( isalnum( c ) || ( c == '_' ) || ( c == ':' ) )

enum type
// type de la doc
{
	aucune,	// pas de docs trouvée
	function_long,	// doc. sur function/méthode dans commentaire long /**/
	function_court, // doc. sur function/méthode dans commentaire court //
	classe_long, // doc. sur classe dans commnentaire long /**/
	classe_court, // doc. sur classe dans commentaire court //
	objet_long, // doc. sur objet dans commentaire long /**/
	objet_court, // doc. sur objet dans commentaire court //
	typedef_long, // doc. sur typedef dans commentaire long /**/
	typedef_court, // doc. sur typedef dans commentaire court //
	define_long, // doc. sur define dans commentaire long /**/
	define_court, // doc. sur define dans commentaire court //
	virtuel_long, // doc. sur methode virtuelle dans commentaire long /**/
	virtuel_court, // doc. sur methode virtuelle dans commentaire court //
	restreint_long, // doc. sur objet restreint dans commentaire long /**/
	restreint_court, // doc. sur objet restreint dans commentaire court //
	shortcut_long, // doc. sur shortcut dans commentaire long /**/
	shortcut_court, // doc. sur shortcut dans commentaire court //
	enum_long, // doc. sur enum dans commentaire long /**/
	enum_court, // doc. sur enum dans commentaire court //
	item_long, // doc. sur item dans commentaire long /**/
	item_court, // doc. sur item dans commentaire court //
	description,
	version,
	coordinators,
	release,
};

static void SauterBlancs_( xtf::extended_text_iflow___ &Flot )
{
	while ( !Flot.EOX() && isspace( Flot.View() ) )
		Flot.Get();
}
/*
static void SauterLigne_( flo_entree_texte_fichier &Flot )
{
	int C;

	C = Flot.Get();

	while ( Flot && ( C != '\n' ) )
		C = Flot.Get();
}
*/
static void LireIdentificateur_(
	xtf::extended_text_iflow___ &Flot,
	str::string_ &String )
{
	bso::char__ C = Flot.Get();

	while ( !Flot.EOX() && isidcar( C ) ) {
		String.Append( C );
		C = Flot.Get();
	}

	if ( !Flot.EOX() ) {
		if ( ( ( String.Amount() >= 6 )
				 && !str::Compare( String, str::string( "signed" ), String.Amount() - 6 ) ) ) {
			String.Append( ' ' );

			SauterBlancs_( Flot );

			C = Flot.Get();

			while ( isidcar( C ) ) {
				String.Append( C );
				C = Flot.Get();
			}
		}

		Flot.Unget( C );
	}
	else
		String.Append( C );
}

static void LireType_(
	xtf::extended_text_iflow___ &Flot,
	str::string_ &String )
{
ERRProlog
	bso::char__ C;
	str::string Tampon;
ERRBegin
	Tampon.Init();
	
	SauterBlancs_( Flot );

	LireIdentificateur_( Flot, Tampon );

	if ( ( Tampon == str::string( "template" ) ) || ( Tampon == str::string( "inline" ) ) )
	{
		String.Append( Tampon );
		return;
	}

	if ( Tampon == str::string( "virtual" ) )
	{
		Tampon.Init();
		SauterBlancs_( Flot );
		LireIdentificateur_( Flot, Tampon );
	}

	if ( Tampon == str::string( "typename" ) )
	{
		Tampon.Init();
		SauterBlancs_( Flot );
		LireIdentificateur_( Flot, Tampon );
	}

	if ( Tampon == str::string( "extern" ) )
	{
		Tampon.Init();
		SauterBlancs_( Flot );
		LireIdentificateur_( Flot, Tampon );
	}

	if ( Tampon == str::string( "const" ) )
	{
		Tampon.Append( ' ' );

		SauterBlancs_( Flot );

		LireIdentificateur_( Flot, Tampon );
	}

	String.Append( Tampon );

	SauterBlancs_( Flot );

	C = Flot.Get();

	while ( ( ( C == ':' ) || ( C == '<' ) || ( C == '(' ) ) )
	{
		if ( C == ':' )
		{
			C = Flot.Get();

			if ( C == ':' )
			{
				String.Append( ':' );
				String.Append( ':' );
				LireIdentificateur_( Flot, String );
			}
			else
				ERRu();

		}
		else if ( C == '<' )
		{
			String.Append( '<' );
			do
			{
				SauterBlancs_( Flot );
				LireType_( Flot, String );
				SauterBlancs_( Flot );

				C = Flot.Get();

				if ( C == ',' )
					String.Append( ", " );
			}
			while ( C ==',' );

			if ( C != '>' )
				ERRu();

			String.Append('>');
		}
		else if ( C == '(' )
		{
			String.Append( '(' );
			do
			{
				SauterBlancs_( Flot );
				LireType_( Flot, String );
				SauterBlancs_( Flot );

				C = Flot.Get();

			}
			while ( C ==',' );

			if ( C != ')' )
				ERRu();

			String.Append(')');
		}
		else
			ERRu();

		C = Flot.Get();
	}

	Flot.Unget( C );

	SauterBlancs_( Flot );

	C = Flot.Get();

	if ( ( C == '&' ) || ( C == '*' ) )
	{
		String.Append( ' ' );
		String.Append( C );
	}
	else
		Flot.Unget( C );
ERRErr
ERREnd
ERREpilog
}

static void LireValeur_(
	xtf::extended_text_iflow___ &Flot,
	str::string_ &String )
{
	int C = Flot.Get();

	if ( C == '=' )
	{
		int P = 0;

		SauterBlancs_( Flot );

		C = Flot.Get();

		while ( ( ( P != 0 )
				  || ( C !=')' ) )
				&& ( C != ',' ) )
		{
			String.Append( C );

			if ( C == '(' )
				P++;
			else if ( C == ')' )
				P--;

			C = Flot.Get();
		}
	}

	Flot.Unget( C );
}




void LireDocCourt_(
	xtf::extended_text_iflow___ &Flot,
	str::string_ &Commentaire )
// lit la doc. dans  'Flot' sachant qu'elle est inclue dans un commentaire court
{
	int C;

	C = Flot.Get();
	
	if ( C == ' ' )
		C = Flot.Get();

	while ( Flot.Column() != 1 )
	{
		Commentaire.Append( C );
		C = Flot.Get();
	}
}


void LireDocLong_(
	xtf::extended_text_iflow___ &Flot,
	str::string_ &Commentaire )
// lit la doc. dans 'Flot' sachant qu'elle est inclue dans un commentaire long
{
	int C;

	C = Flot.Get();
	
	if ( C == ' ' )
		C = Flot.Get();

	do
	{
		while ( C != '*' )
		{
			if ( C == '\n' )
			{
				SauterBlancs_( Flot );
				C = ' ';
			}

			Commentaire.Append( C );
			C = Flot.Get();
		}

		if ( ( C = Flot.Get() ) != '/' )
			Commentaire.Append( '*' );
	}
	while ( C != '/' );
}

void ExtractCVSTag_( str::string_ &Tag )
{
	epeios::row_t__ P;

	if ( ( ( P = *Tag.Search( ':' ) ) != NONE ) && ( Tag.Amount() >= P + 2 ) ) {
		Tag.Delete( 0, P + 2 );

		if ( ( P = *Tag.Search( '$' ) ) != NONE ) {
			Tag.Allocate( P - 1 );
		}
	}
}

void LireDocumentation_(
	xtf::extended_text_iflow___ &Flot,
	str::string_ &Documentation )
{
	LireDocCourt_( Flot, Documentation );

	if ( ( Documentation.Amount() > 1 ) && ( Documentation( 0 ) == '$' ) )
		ExtractCVSTag_( Documentation );
}

void LireDoc_(
	xtf::extended_text_iflow___ &Flot,
	bso::bool__ Long,
	str::string_ &Commentaire )
// lit la doc. dans 'Flot' et la place dans 'Commentaire';
// si 'Long', alors la doc est située dans un commentaire long (/**/),
// sinon elle est située dans un commentaire court(//)
{
	if ( Long )
		LireDocLong_( Flot, Commentaire );
	else
		LireDocCourt_( Flot, Commentaire );
}

static type TrouverDoc_(
	xtf::extended_text_iflow___ &Flot,
	bso::bool__ Classe )
// positionne 'Flot' sur la doc, sachant que l'on est en train d'analyser une classe si 'Classe'
{
	static int Crochet = 0;
	bso::bool__ Long;
	volatile int C;
	bso::bool__ TaggedCommentFound = false;

	do
	{
		do
		{
			if ( Flot.EOX() )
				if ( Crochet )
					ERRf();
				else
					return aucune;

			C = Flot.Get();

			while( C != '/' )
			{
				if ( Classe )
					if ( C == '{' )
					{
						if ( Crochet == 255 )
							ERRl();

						Crochet++;
					}
					else if ( C == '}' )
					{
						Crochet--;

						if ( !Crochet )
							return aucune;
					}


				SauterBlancs_( Flot );

				if ( Flot.EOX() )
					if ( Crochet )
						ERRf();
					else
						return aucune;
				
				C = Flot.Get();

			}

			if ( Flot.EOX() )
				if ( Crochet )
					ERRf();
				else
					return aucune;

			C = Flot.Get();

		} while ( ( C != '*' ) && ( C != '/' ) );

		if ( C == '*' )
			Long = true;
		else if ( C == '/' )
			Long = false;
		else
			ERRc();

		if ( Flot.EOX() )
			if ( Crochet )
				ERRf();
			else
				return aucune;

		C = Flot.Get();

		if ( ( strchr( "eifcvdrtosVDCR", C ) == NULL )
			|| ( Flot.View() != ' ' ) ) {
			if ( Long ) {
				do {
					do {
						if ( Flot.EOX() )
							ERRf();
	
					} while ( Flot.Get() != '*' );

					if ( Flot.EOX() )
						ERRf();

				} while( Flot.View() != '/' );

				Flot.Get();
			} else
				Flot.SkipLine();
		} else 
			TaggedCommentFound = true;
	} while ( !TaggedCommentFound );

	if ( C == 'f' )
		if ( Long )
			return function_long;
		else
			return function_court;
	else if ( (char)C == 'o' )
		if ( Long )
			return objet_long;
		else
			return objet_court;
	else if ( (char)C == 'c' )
		if ( Long )
			return classe_long;
		else
			return classe_court;
	else if ( (char)C == 't' )
		if ( Long )
			return typedef_long;
		else
			return typedef_court;
	else if ( (char)C == 'd' )
		if ( Long )
			return define_long;
		else
			return define_court;
	else if ( (char)C == 'v' )
		if ( Long )
			return virtuel_long;
		else
			return virtuel_court;
	else if ( (char)C == 'r' )
		if ( Long )
			return restreint_long;
		else
			return restreint_court;
	else if ( (char)C == 's' )
		if ( Long )
			return shortcut_long;
		else
			return shortcut_court;
	else if ( (char)C == 'e' )
		if ( Long )
			return enum_long;
		else
			return enum_court;
	else if ( (char)C == 'i' )
		if ( Long )
			return item_long;
		else
			return item_court;
	else if ( Long )
		ERRu();
	else if ( (char)C == 'V' )
		return version;
	else if ( (char)C == 'D' )
		return description;
	else if ( (char)C == 'C' )
		return coordinators;
	else if ( (char)C == 'R' )
		return release;
	else
		ERRu();

	return aucune;	// ne sert qu'à éviter l'apparition d'un warning
}

void tsrcpr::parametre_::Analyze( xtf::extended_text_iflow___ &Flot )
{
	SauterBlancs_( Flot );

	LireType_( Flot, Type );

	if ( Type.Amount() )
	{
		char C;

		LireIdentificateur_( Flot, Name );

		SauterBlancs_( Flot );

		if ( ( C = Flot.Get() ) == '[' )
			if ( Flot.Get() == ']' ) {
				Type.Append( " []" );
				SauterBlancs_( Flot );
			} else
				ERRf();
		else
			Flot.Unget( C );
			

		LireValeur_( Flot, Valeur );

	}
	else
	{
		if ( Flot.Get() != '.' )
			ERRu();

		if ( Flot.Get() != '.' )
			ERRu();

		if ( Flot.Get() != '.' )
			ERRu();

		Name.StoreAndAdjust( "..." );
	}
}


void tsrcpr::typedef_::Analyze(
	xtf::extended_text_iflow___ &Flot,
	bso::bool__ Long )
{
ERRProlog
	str::string S;
	bso::ubyte__ Level = 0;
ERRBegin
	LireDoc_( Flot, Long, Commentaire );

	S.Init();

	SauterBlancs_( Flot );

	LireIdentificateur_( Flot, S );

	SauterBlancs_( Flot );
	
	if ( S != str::string( "typedef" ) )
		if ( S == str::string( "E_TYPEDEF" ) ) {
			SauterBlancs_( Flot );
			
			if ( Flot.Get() != '(' )
				ERRf();
		}
		else
			ERRf();
			
	SauterBlancs_( Flot );
	
	LireIdentificateur_( Flot, S );
	
	SauterBlancs_( Flot );
	
	if ( ( Flot.View() == '(' ) || ( Flot.View() == '<' ) ) {
		Level++;
		Flot.Get();
		
		while( Level != 0 )
			switch( Flot.Get() ) {
			case '<':
			case '(':
				Level++;
				break;
			case '>':
			case ')':
				if ( Level == BSO_UBYTE_MAX )
					ERRl();
				Level--;
				break;
			default:
				break;
			}
	} 
	
	SauterBlancs_( Flot );
	
	if ( Flot.View() == ',' ) {
		Flot.Get();
		SauterBlancs_( Flot );
	}
	
	LireIdentificateur_( Flot, Name );
	
	Flot.SkipLine();
ERRErr
ERREnd
ERREpilog
}

void tsrcpr::shortcut_::Analyze(
	xtf::extended_text_iflow___ &Flot,
	bso::bool__ Long )
{

	define_::Analyze( Flot, Long );

	SauterBlancs_( Flot );

	LireIdentificateur_( Flot, Alias );
}

void tsrcpr::define_::AnalyserArguments_( xtf::extended_text_iflow___ &Flot )
{
ERRProlog
	volatile int C;
	str::string S;
ERRBegin

	C = Flot.Get();

	if ( C != '(' )
		ERRp();

	C = Flot.Get();

	while ( C != ')' )
	{
		if ( C != ',')
			Flot.Unget( C );

		S.Init();

		SauterBlancs_( Flot );

		LireIdentificateur_( Flot, S );

		Arguments.Add( S );

		SauterBlancs_( Flot );

		C = Flot.Get();
	}
ERRErr
ERREnd
ERREpilog
}

void tsrcpr::define_::Analyze(
	xtf::extended_text_iflow___ &Flot,
	bso::bool__ Long )
{
ERRProlog
	str::string S;
ERRBegin

	LireDoc_( Flot, Long, Commentaire );

	S.Init();

	SauterBlancs_( Flot );

	if ( Flot.Get() != '#' )
		ERRf();

	SauterBlancs_( Flot );

	LireIdentificateur_( Flot, S );

	SauterBlancs_( Flot );

	if ( S != str::string( ( "define" ) ) )
		ERRf();

	LireIdentificateur_( Flot, Name );

	if ( Flot.View() == '(' )
		AnalyserArguments_( Flot );

	Flot.SkipLine();
ERRErr
ERREnd
ERREpilog

}

void tsrcpr::methode_::Analyze(
	xtf::extended_text_iflow___ &Flot,
	bso::bool__ Long )
// lit une methode sachant que sa doc. est située
//dans un commentaire long si 'Long', dans un commentaire court sinon
{
	int C;

	LireDoc_( Flot, Long, Commentaire );

	SauterBlancs_( Flot );

	LireType_( Flot, Type );

	if ( Type == str::string( "inline" ) )
	{
		Type.Init();
		LireType_( Flot, Type );
	}

	SauterBlancs_( Flot );

	LireIdentificateur_( Flot, Name );

	if ( Name == str::string( "operator" ) )
	{
		bso::bool__ Premier = true;

		Name.Append( ' ' );

		SauterBlancs_( Flot );

		while ( !isspace( C = Flot.Get() ) && ( ( C != '(' ) || Premier ) )
		{
			Name.Append( C );
			Premier = false;
		}

		Flot.Unget( C );
	}

	SauterBlancs_( Flot );

	AnalyserParametres_( Flot );
}

void tsrcpr::function_::Analyze(
	xtf::extended_text_iflow___ &Flot,
	bso::bool__ Long )
// lit une methode sachant que sa doc. est située
//dans un commentaire long si 'Long', dans un commentaire court sinon
{
	int C;

	LireDoc_( Flot, Long, Commentaire );

	SauterBlancs_( Flot );

	LireType_( Flot, Type );

	if( Type == str::string( "template" ) )
	{
		Template.Analyze( Flot );
		Type.Init();
		LireType_( Flot, Type );
	}

	if ( Type == str::string( "inline" ) )
	{
		Type.Init();
		LireType_( Flot, Type );
	}

	SauterBlancs_( Flot );

	LireIdentificateur_( Flot, Name );

	if ( Name == str::string( "operator" ) )
	{
		bso::bool__ Premier = true;

		Name.Append( ' ' );

		SauterBlancs_( Flot );

		while ( !isspace( C = Flot.Get() ) && ( ( C != '(' ) || Premier ) )
		{
			Name.Append( C );
			Premier = false;
		}

		Flot.Unget( C );
	}

	SauterBlancs_( Flot );

	AnalyserParametres_( Flot );
}


void tsrcpr::argument_::Analyze( xtf::extended_text_iflow___ &Flot )
{
	SauterBlancs_( Flot );

	LireIdentificateur_( Flot, Type );


	SauterBlancs_( Flot );

	LireIdentificateur_( Flot, Name );

	SauterBlancs_( Flot );

}

void tsrcpr::template_::Analyze( xtf::extended_text_iflow___ &Flot )
{
ERRProlog
	int C;
	tsrcpr::argument T;
ERRBegin

	SauterBlancs_( Flot );

	C = Flot.Get();

	if ( C != '<' )
		ERRu();

	while ( C != '>' )
	{
		SauterBlancs_( Flot );

		T.Init();

		T.Analyze( Flot );

		Arguments.Add( T );

		SauterBlancs_( Flot );

		C = Flot.Get();
	}
ERRErr
ERREnd
ERREpilog
}


void tsrcpr::methode_::AnalyserParametres_( xtf::extended_text_iflow___ &Flot )
{
ERRProlog
	volatile int C;
	tsrcpr::parametre P;
ERRBegin

	C = Flot.Get();

	if ( C != '(' )
		ERRu();

	C = Flot.Get();

	while ( C != ')' )
	{
		if ( C != ',')
			Flot.Unget( C );

		P.Init();

		P.Analyze( Flot );

		Parametres.Add( P );

		SauterBlancs_( Flot );

		C = Flot.Get();
	}
ERRErr
ERREnd
ERREpilog
}

void tsrcpr::function_::AnalyserParametres_( xtf::extended_text_iflow___ &Flot )
{
ERRProlog
	bso::char__ C;
	tsrcpr::parametre P;
ERRBegin

	C = Flot.Get();

	if ( C != '(' )
		ERRu();

	C = Flot.Get();

	while ( C != ')' )
	{
		if ( C != ',')
			Flot.Unget( C );

		P.Init();

		P.Analyze( Flot );

		Parametres.Add( P );

		SauterBlancs_( Flot );

		C = Flot.Get();
	}
ERRErr
ERREnd
ERREpilog
}


void tsrcpr::objet_::Analyze(
	xtf::extended_text_iflow___ &Flot,
	bso::bool__ Long )
// lit une methode sachant que sa doc. est située
//dans un commentaire long si 'Long', dans un commentaire court sinon
{
	LireDoc_( Flot, Long, Commentaire );

	SauterBlancs_( Flot );

	LireType_( Flot, Type );

	SauterBlancs_( Flot );

	LireIdentificateur_( Flot, Name );

	SauterBlancs_( Flot );
}

template <class t> void inline AnalyserItem_(
	table_<t> &TI,
	xtf::extended_text_iflow___ &Flot,
	bso::bool__ Long )
{
ERRProlog
	ctn::E_ITEM( t ) Element;
ERRBegin
	tym::row__ P = TI.New();

	Element.Init( TI );

	Element( P ).Init();
	Element().Analyze( Flot, Long );
ERRErr
ERREnd
ERREpilog
}

static void AdjustEndOfBase_( str::string_ &S )
{
	bso::char__ C;

	while( ( C = S( S.Last() ) == ' ' ) || ( C == ',' ) )
		S.Truncate();
}

void tsrcpr::classe_::AnalyzeBases_( xtf::extended_text_iflow___ &Flot )
{
ERRProlog
	str::string S;
ERRBegin
	Flot.Get();

	while( Flot.View() != '{' ) {	
		S.Init();
		SauterBlancs_( Flot );
		LireIdentificateur_( Flot, S );

		if ( S == str::string( "public" ) ) {
			S.Init();
			SauterBlancs_( Flot );
			Flot.GetLine( S );
			AdjustEndOfBase_( S );
			Bases.Add( S );
		} else
			Flot.SkipLine();

		SauterBlancs_( Flot );
	}

ERRErr
ERREnd
ERREpilog

}

void tsrcpr::item_::Analyze(
	xtf::extended_text_iflow___ &Flot,
	bso::bool__ Long )
{
	LireDoc_( Flot, Long, Commentary );

	SauterBlancs_( Flot );

	LireIdentificateur_( Flot, Name );

	Flot.SkipLine();
}

void tsrcpr::enum_::Analyze(
	xtf::extended_text_iflow___ &Flot,
	bso::bool__ Long )
{

	type Type;

	LireDoc_( Flot, Long, Commentary );

	SauterBlancs_( Flot );

	LireIdentificateur_( Flot, Name );

	if ( Name != str::string( "enum" ) )
		ERRf();

	Name.Init();
	
	SauterBlancs_( Flot );

	LireIdentificateur_( Flot, Name );

	SauterBlancs_( Flot );

	if ( Flot.Get() != '{' )
		ERRf();

	SauterBlancs_( Flot );

	while( Flot.View() != '}' ) {
	
		Type = TrouverDoc_( Flot, false );

		if ( Type == item_long )
			AnalyserItem_<tsrcpr::item_>( Items, Flot, true );
		else if ( Type == item_court )
			AnalyserItem_<tsrcpr::item_>( Items, Flot, false );
		else
			ERRc();

		SauterBlancs_( Flot );
	}


}

void tsrcpr::classe_::Analyze(
	xtf::extended_text_iflow___ &Flot,
	bso::bool__ Long )
// lit une methode sachant que sa doc. est située
//dans un commentaire long si 'Long', dans un commentaire court sinon
{
	type Type;

	LireDoc_( Flot, Long, Commentaire );

	SauterBlancs_( Flot );

	LireIdentificateur_( Flot, this->Type );

	if ( this->Type == str::string( "template" ) )
	{
		Template.Analyze( Flot );

		SauterBlancs_( Flot );

		this->Type.Init();

		LireIdentificateur_( Flot, this->Type );
	}

	SauterBlancs_( Flot );

	LireIdentificateur_( Flot, this->Name );

	SauterBlancs_( Flot );

	if ( Flot.View() == ':' )
		AnalyzeBases_( Flot );

	while( ( Type = TrouverDoc_( Flot, true ) ) != aucune )
	{

		if ( Type == function_long )
			AnalyserItem_<tsrcpr::methode_>( Methodes, Flot, true );
		else if ( Type == function_court )
			AnalyserItem_<tsrcpr::methode_>( Methodes, Flot, false );
		else if ( Type == virtuel_long )
			AnalyserItem_<tsrcpr::methode_>( Virtuels, Flot, true );
		else if ( Type == virtuel_court )
			AnalyserItem_<tsrcpr::methode_>( Virtuels, Flot, false );
		else if ( Type == objet_long )
			AnalyserItem_<tsrcpr::objet_>( Objets, Flot, true );
		else if ( Type == objet_court )
			AnalyserItem_<tsrcpr::objet_>( Objets, Flot, false );
		else if ( Type == restreint_long )
			AnalyserItem_<tsrcpr::objet_>( Restreints, Flot, true );
		else if ( Type == restreint_court )
			AnalyserItem_<tsrcpr::objet_>( Restreints, Flot, false );
		else
			ERRc();
	}

}

void tsrcpr::library_::Analyze( xtf::extended_text_iflow___ &Flot )
{
	type Type;

	while( ( Type = TrouverDoc_( Flot, false ) ) != aucune )
	{

		if ( Type == function_long )
			AnalyserItem_<tsrcpr::function_>( Functions, Flot, true );
		else if ( Type == function_court )
			AnalyserItem_<tsrcpr::function_>( Functions, Flot, false );
		else if ( Type == objet_long )
			AnalyserItem_<tsrcpr::objet_>( Objets, Flot, true );
		else if ( Type == objet_court )
			AnalyserItem_<tsrcpr::objet_>( Objets, Flot, false );
		else if ( Type == classe_long )
			AnalyserItem_<tsrcpr::classe_>( Classes, Flot, true );
		else if ( Type == classe_court )
			AnalyserItem_<tsrcpr::classe_>( Classes, Flot, false );
		else if ( Type == typedef_long )
			AnalyserItem_<tsrcpr::typedef_>( Typedefs, Flot, true );
		else if ( Type == typedef_court )
			AnalyserItem_<tsrcpr::typedef_>( Typedefs, Flot, false );
		else if ( Type == define_long )
			AnalyserItem_<tsrcpr::define_>( Defines, Flot, true );
		else if ( Type == define_court )
			AnalyserItem_<tsrcpr::define_>( Defines, Flot, false );
		else if ( Type == shortcut_long )
			AnalyserItem_<tsrcpr::shortcut_>( Shortcuts, Flot, true );
		else if ( Type == shortcut_court )
			AnalyserItem_<tsrcpr::shortcut_>( Shortcuts, Flot, false );
		else if ( Type == enum_long )
			AnalyserItem_<tsrcpr::enum_>( Enums, Flot, true );
		else if ( Type == enum_court )
			AnalyserItem_<tsrcpr::enum_>( Enums, Flot, false );
		else if ( Type == version )
			LireDocumentation_( Flot, Version );
		else if ( Type == release )
			LireDocumentation_( Flot, Release );
		else if ( Type == description )
			LireDocumentation_( Flot, Description );
		else if ( Type == coordinators )
			LireDocumentation_( Flot, Coordinators );
		else
			ERRc();
	}
}




/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class tsrcprpersonnalization
: public tsrcprtutor
{
public:
	tsrcprpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~tsrcprpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the ending of the application  */
	}
};


/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

// 'static' by GNU C++.

static tsrcprpersonnalization Tutor;

ttr_tutor &TSRCPRTutor = Tutor;
