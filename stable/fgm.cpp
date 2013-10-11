/* begin of 'entete.txt' template file V1.1 */
/* Best viewed with a tab size of 4 */
/*
	This file is part of the Epeios project. For all information
	concerning the Epeios project, this file and its conditions of use,
	consult the site: 

			http://www.epeios.org/
*/
/* end of 'entete.txt' template file */
/* begin of 'xxx.cpp' template file V2.21 */
/* 
	Pour plus de détails, consultez le fichier 'fgm.h'.
	See the 'fgm.h' file  for more details.
*/

#define FGM__COMPILATION

#include "fgm.h"

class fgmtutor
: public ttr_tutor
{
public:
	fgmtutor( void )
	: ttr_tutor( FGM_NAME )
	{
		Version = FGM_VERSION;
		Owner = FGM_OWNER;
		Date = "06/03/2000 22:23:42";
	}
	virtual ~fgmtutor( void ){}
};

//  	Substitution is not applied to the rest.
/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/


// Libere la place occupée par les fragments 'Debut' à 'Fin' inclus.
void fgm_memoire_fragmentee::Liberer_(
	fgm__indice Debut,
	fgm__indice Fin )
{
	while( Debut <= Fin )
	{
		if ( Fragments_[Debut] )
		{
			free( Fragments_[Debut] );
			Fragments_[Debut] = NULL;
		}

		Debut++;
	}
}

// Alloue la place nécessaire au fragments 'Debut' à 'Fin'.
void fgm_memoire_fragmentee::AllouerFragments_(
	fgm__indice Debut,
	fgm__indice Fin )
{
	char *Nouveau;

	while ( Debut <= Fin )
	{
		if ( ( Nouveau = (char *)realloc( Fragments_[Debut], Taille_ ) ) == NULL )
			ERRa();

		Fragments_[Debut++] = Nouveau;
	}
}

// Ecrit à 'Position' 'Quantite' octets de 'Tampon'.
void fgm_memoire_fragmentee::Lire(
	unsigned long Position,
	unsigned long Quantite,
	char *Tampon )
{
	fgm__size QuantDansFrag, PosDansFrag, Fragment;

	while( Quantite )
	{
		PosDansFrag = Position % Taille_;
		QuantDansFrag = Taille_ - PosDansFrag;

		if ( QuantDansFrag > Quantite )
			QuantDansFrag = Quantite;

		Fragment = Position / Taille_;

		memcpy( Tampon, Fragments_[Fragment] + PosDansFrag, QuantDansFrag );

		Position += QuantDansFrag;
		Quantite -= QuantDansFrag;
		Tampon += QuantDansFrag;
	}
}

// Place dans 'Tampon' 'Quantite' octets situés à 'Position'.
void fgm_memoire_fragmentee::Ecrire(
	const char *Tampon,
	size_t Quantite,
	unsigned long Position )
{
	fgm__size QuantDansFrag, PosDansFrag, Fragment;

	while( Quantite )
	{
		PosDansFrag = Position % Taille_;
		QuantDansFrag = Taille_ - PosDansFrag;

		if ( QuantDansFrag > Quantite )
			QuantDansFrag = Quantite;

		Fragment = Position / Taille_;

		memcpy( Fragments_[Fragment] + PosDansFrag, Tampon, QuantDansFrag );

		Position += QuantDansFrag;
		Quantite -= QuantDansFrag;
		Tampon += QuantDansFrag;
	}
}






/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */
class fgmpersonnalization
: public fgmtutor
{
public:
	fgmpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~fgmpersonnalization( void )
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
static fgmpersonnalization Tutor;

ttr_tutor &FGMTutor = Tutor;

/* end of 'xxx.cpp' template file */