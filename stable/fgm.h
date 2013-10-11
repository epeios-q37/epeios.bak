/* begin of 'entete.txt' template file V1.1 */
/* Best viewed with a tab size of 4 */
/*
	This file is part of the Epeios project. For all information
	concerning the Epeios project, this file and its conditions of use,
	consult the site: 

			http://www.epeios.org/
*/
/* end of 'entete.txt' template file */
/* begin of 'entete.h' template file V1.3 */
/*
Version:
	1.0.4 06/03/2000 22:23:42

Instigators:
	Claude SIMON

Authors:
	Claude SIMON

Contributors:
	/

Coordinator:
	Claude SIMON

Description:
	FraGmented Memory
*/

/* Begin of automatic documentation generation part. */

//V 1.0.4
//C Claude SIMON
//D FraGmented Memory
//R 06/03/2000 22:23:42

/* End of automatic documentation generation part. */

/* end of 'entete.h' template file */
/* begin of 'xxx.h' template file V2.3 */

#ifndef FGM__INC
#define FGM__INC

#define FGM_NAME		"FGM"

#define	FGM_VERSION		"1.0.4"	
#define FGM_OWNER		"The Epeios Project (www.epeios.org)"	

//  	Substitution is not applied to the rest when emancipated

#include "ttr.h"

extern class ttr_tutor &FGMTutor;

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

#include "err.h"
#include "flw.h"
#include "mdr.h"

// Nombre de fragments
typedef unsigned long fgm__amount;

// Indice d'un fragment.
typedef fgm__amount	fgm__indice;

// Taille d'un fragment
typedef size_t fgm__size;

#define FGM_TAILLE_DEFAUT_FRAGMENT	10000

// Une mémoire fragmentée.
class fgm_memoire_fragmentee
{
	// Les pointeurs sur les fragments
	char **Fragments_;
	// Le nombre de fragments
	fgm__amount Nombre_;
	// La taille d'un fragment;
	fgm__size Taille_;
	// Libere la place occupée par les fragments 'Debut' à 'Fin' inclus.
	void Liberer_(
		fgm__indice Debut,
		fgm__indice Fin );
	// Preparer (met à NULL) les fragments de 'Debut' à 'Fin'.
	void Preparer_(
		fgm__indice Debut,
		fgm__indice Fin )
	{
		memset( Fragments_ + Debut, 0, ( Fin - Debut + 1 ) * sizeof( char * ) );
	}
	// Alloue l'espace nécessaire pour contenir 'Quantite' (!= 0) octets.
	void Allouer_( unsigned long Quantite )
	{
		unsigned long Necessaire = ( Quantite  - 1U ) / Taille_ + 1U;
		char *NouveauFragment;

		if ( Necessaire >= ( ULONG_MAX / sizeof( char * ) ) )
			ERRl();

		if ( Necessaire < Nombre_ )
		{
			Liberer_( Necessaire, Nombre_ - 1 );

			Nombre_ = Necessaire;
		}
		else if ( Necessaire > Nombre_ )
		{
			char **Nouveau;

			if ( ( Nouveau = (char **)realloc( Fragments_, sizeof( char * ) * Necessaire ) ) == NULL )
				ERRa();

			Fragments_ = Nouveau;

			Preparer_( Nombre_, Necessaire - 1 );

			if ( Nombre_ < Necessaire )
				AllouerFragments_( Nombre_ - 1, Necessaire - 2 );

			Nombre_ = Necessaire;
		}

		Quantite %= Taille_;

		if ( !Quantite )
			Quantite = Taille_;

		if ( ( NouveauFragment = (char *)realloc( Fragments_[Nombre_ - 1], Quantite ) ) == NULL )
			ERRa();

		Fragments_[Nombre_-1] = NouveauFragment;
	}
	// Alloue la place nécessaire au fragments 'Debut' à 'Fin'.
	void AllouerFragments_(
		fgm__indice Debut,
		fgm__indice Fin );
public:
	void reset( bool P = true )
	{
		if ( P )
		{
			if ( Fragments_ )
				Liberer_( 0, Nombre_ - 1 );

			free( Fragments_ );
		}

		Taille_ = 0;
		Nombre_ = 0;
		Fragments_ = NULL;
	}
	void plug( class mmm_multimemory_ &)
	{
		// stadardization
	}
	fgm_memoire_fragmentee( void )
	{
		reset( false );
	}
	~fgm_memoire_fragmentee( void )
	{
		reset( true );
	}
	// Initialisation avec la taille de fragment 'Taille'
	void Init( void )
	{
		if ( !Taille_ )
			Init( FGM_TAILLE_DEFAUT_FRAGMENT );
	}
	void Init( fgm__size Taille )
	{
		reset();

		if ( ( Fragments_ = (char **)calloc( 1, sizeof( char *) ) ) == NULL )
			ERRa();

		Nombre_ = 1;
		// simplifie certains tests et permet de s'assurer
		// qu'il y a unn minimum de mémoire disponible.

		Taille_ = Taille;
	}
	// Ecrit à 'Position' 'Quantite' octets de 'Tampon'.
	void Lire(
		unsigned long Position,
		unsigned long Quantite,
		char *Tampon );
	// Place dans 'Tampon' 'Quantite' octets situés à 'Position'.
	void Ecrire(
		const char *Tampon,
		size_t Taille,
		unsigned long Position );
	// Alloue la place nécessaire à 'Quantite' octets.
	void Allouer( unsigned long Quantite )
	{
		if ( !Quantite )
		{
			Liberer_( 0, Nombre_ - 1 );
			Nombre_ = 1;
		}
		else
			Allouer_( Quantite );
	}
};


//c The standard memory driver for fragmented RAM.
class fgm_fragmented_memory_driver
: public MEMORY_DRIVER,
  public fgm_memoire_fragmentee
{
protected:
	// lit à partir de 'Position' et place dans 'Tampon' 'Nombre' octets
	virtual void MDRRead(
		mdr__position Position,
		mdr__bsize Amount,
		void *Buffer )
	{
		fgm_memoire_fragmentee::Lire( Position, Amount, (char *)Buffer );
	}
	// écrit 'Nombre' octets à la position 'Position'
	virtual void MDRWrite(
		const void *Buffer,
		mdr__bsize Amount,
		mdr__position Position )
	{
		fgm_memoire_fragmentee::Ecrire( (char *)Buffer, Amount, Position );
	}
	// alloue 'Taille' octets
	virtual void MDRAllocate( mdr__size Size )
	{
		fgm_memoire_fragmentee::Allouer( Size );
	}
public:
	void reset( bool P = true )
	{
		fgm_memoire_fragmentee::reset( P );
		MEMORY_DRIVER::reset( P );
	}
	void plug( class mmm_multimemory_ &M )
	{
		fgm_memoire_fragmentee::plug( M );
		MEMORY_DRIVER::plug( M );
	}
	//f Initialization.
	void Init( void )
	{
		fgm_memoire_fragmentee::Init();
		MEMORY_DRIVER::Init();
	}
	//f Initialization with 'Size' as fragment size.
	void Init( fgm__size Taille )
	{
		fgm_memoire_fragmentee::Init( Taille );
		MEMORY_DRIVER::Init();
	}
};

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
/* end of 'xxx.h' template file */