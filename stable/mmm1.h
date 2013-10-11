/*
	Header for the 'mmm1' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Copyright (C) 2004 Claude SIMON (http://zeusw.org/intl/contact.html).

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

//	$Id: mmm1.h,v 1.9 2012/11/14 16:06:31 csimon Exp $

#ifndef MMM1__INC
#define MMM1__INC

#define MMM1_NAME		"MMM1"

#define	MMM1_VERSION	"$Revision: 1.9 $"

#define MMM1_OWNER		"Claude SIMON (http://zeusw.org/intl/contact.html)"

#include "ttr.h"

extern class ttr_tutor &MMM1Tutor;

#if defined( E_DEBUG ) && !defined( MMM1_NODBG )
#define MMM1_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.9 $
//C Claude SIMON (http://zeusw.org/intl/contact.html)
//R $Date: 2012/11/14 16:06:31 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D MultiMeMory v1 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#error Do noy use. Use 'MMM' instead.

#include "err.h"
#include "flw.h"
#include "mmm0.h"
#include "uym.h"

namespace mmm {

	// Taille d'un bloc.
	#define MMM1_TAILLE_BLOC	( sizeof( indicateur__ ) )

	// taille d'un descripteur
	#define MMM1_TAILLE_INDICATEUR	( sizeof( indicateur__ ) )

	// nombre de bloc qu'occupe un descripteur
	#define MMM1_NOMBRE_INDICATEUR	1

	// Position du bit signalant l'occupation du bloc précédent.
	#define MMM1_BIT_OCCUPATION_PREDECESSEUR	30

	// Masque relatif à l'occupation di bloc précédent.
	#define MMM1_MASQUE_OCCUPATION_PREDECESSEUR	( 1 << MMM1_BIT_OCCUPATION_PREDECESSEUR )

	// Position du bit signalant l'occupation de la portion.
	#define MMM1_BIT_OCCUPATION		31

	// Masque relatif à l'occupation d'un bloc.
	#define MMM1_MASQUE_OCCUPATION	( 1 << MMM1_BIT_OCCUPATION )

	// Masque relatif au nombre de bloc occupée par une portion
	#define MMM1_MASQUE_NOMBRE	(~( MMM1_MASQUE_OCCUPATION | MMM1_MASQUE_OCCUPATION_PREDECESSEUR ) )

	// type du nombre de blos d'une sous-mémoire.
	typedef mdr::size__ 		nombre__;

	// type d'un indicateur affecté à un bloc de la mutimémoire.
	typedef mdr::size__		indicateur__;

	class multimemory_
	{
	private:
		// L'éventuel pilote multimémoire.
		multimemory_driver__ PiloteMultimemoire_;
		// memoire décomposee en plusieurs
		uym::untyped_memory_ Memoire_;
	// fonctions
		// Convertit un nombre en taille.
		mdr::size__ NombreEnTaille_( nombre__ Nombre ) const
		{
			return Nombre * MMM1_TAILLE_BLOC;
		}
		// Convertir une taille en nombre.
		nombre__ TailleEnNombre_( mdr::size__ Size ) const
		{
			if ( !Size )
				return 0;
			else
				return ( ( Size - 1 ) / MMM1_TAILLE_BLOC ) + 1;
		}
		// Alloue 'Capacité' octets dans la mémoire principale.
		void AllouerMemoire_( mdr::size__ Size )
		{
			Memoire_.Allocate( Size );
			S_.Capacite = Size;
		}
		// Lit de la mémoire.
		void MemoireLire_(
			mdr::row_t__ Position,
			mdr::size__ Nombre,
			mdr::datum__ *Tampon ) const
		{
			Memoire_.Recall( Position, Nombre, Tampon );
		}
		// écrit dans la mémoire.
		void MemoireEcrire_(
			const mdr::datum__ *Tampon,
			mdr::size__ Nombre,
			mdr::row_t__ Position )
		{
			Memoire_.Store( Tampon, Nombre, Position );
		}
		/* Remplace 'Nombre' octets à la position 'Destination avec autant d'octet de
		la position 'Source'. */
		void MemoireEcraser_(
			mdr::row_t__ Destination,
			mdr::row_t__ Source,
			mdr::size__ Taille )
		{
			Memoire_.Store_( Memoire_, Taille, Destination, Source );
		}
		// Ajuste la taille de la mémoire principale en éliminant la portion de descripteur 'Descripteur'.
		void AjusterMemoireAvantDescripteur_( descriptor__ Descripteur )
		{
			AllouerMemoire_( Descripteur - MMM1_TAILLE_INDICATEUR );

			if ( S_.Libre >= Descripteur )
				S_.Libre = 0;
		}
		/* Ajuste la taille de la mémoire principale sachant que la portion de descripteur
		'Descripteur' doit être la dernière */
		void AjusterMemoireApresDescripteur_( descriptor__ Descripteur )
		{
			AllouerMemoire_( Descripteur + Taille_( Descripteur ) );

			if ( S_.Libre >= Descripteur )
				S_.Libre = 0;
		}
		// Ecrase la portion 'Destination' par la portion 'Source'. 'Source' est totalement perdu.
		void Ecraser_(
			descriptor__ Destination,
			descriptor__ Source )
		{
			MemoireEcraser_( Destination - MMM1_TAILLE_INDICATEUR,
							Source - MMM1_TAILLE_INDICATEUR,
							Taille_( Source ) + MMM1_TAILLE_INDICATEUR );
		}
		/* Teste la portion libre de descripteur 'Descripteur' par rapport au libre
		de réserve ( 'Libre_' ) et l'affecte à ce dernier s'il satisfait aux conditions. */
		void TesterEtAffecterLibre_( descriptor__ Descripteur )
		{
			if ( !S_.Libre
				 || ( Nombre_( S_.Libre ) < Nombre_( Descripteur ) ) )
				S_.Libre = Descripteur;
		}
		/* La portion de descripteur 'Descripteur' est étendu par raccourcissment
		de la portion libre sui suit. Si celle-ci devient de taille nulle,
		elle est éliminée. */
		void Etendre_(
			descriptor__ Descripteur,
			nombre__ Nombre )
		{
			descriptor__ Successeur = Successeur_( Descripteur );
			nombre__ NombreLibre = Nombre_( Successeur ) - ( Nombre - Nombre_( Descripteur ) ) + MMM1_NOMBRE_INDICATEUR;

			if ( S_.Libre == Successeur )
				S_.Libre = 0;

			if ( NombreLibre <= 2 )
			{
				SignalerPredecesseurOccupe_( Successeur_( Successeur ) );
				Nombre = Nombre_( Descripteur ) + Nombre_( Successeur ) + MMM1_NOMBRE_INDICATEUR;

				EcrireIndicateurOccupe_( Descripteur, Nombre, PredecesseurEstOccupe_( Descripteur ) );
			}
			else
			{
				NombreLibre--;

				EcrireIndicateurOccupe_( Descripteur, Nombre, PredecesseurEstOccupe_( Descripteur ) );
				EcrireIndicateurLibre_( Successeur_( Descripteur ), NombreLibre );
			}
		}
		// Permute la portion de descripteur 'Descripteur' avec la portion libre qui précède.
		void Permuter_( descriptor__ Descripteur )
		{
			descriptor__ Libre = PredecesseurLibre_( Descripteur );
			descriptor__ Successeur = Successeur_( Descripteur );
			nombre__ NombreLibre = Nombre_( Libre );

			if ( S_.Libre == S_.Libre )
				S_.Libre = 0;

			Ecraser_( Libre, Descripteur );
			SignalerPredecesseurOccupe_( Libre );

			EcrireIndicateurLibre_( Successeur_( Libre ), NombreLibre );

			if ( !HorsLimite_( Successeur ) && EstOccupe_( Successeur ) )
				SignalerPredecesseurLibre_( Successeur );
		}
		/* La portion libre qui suit 'Descripteur' est fusionnée avec la portion
		libre dont elee est elle-même suivi. */
		void Fusionner_( descriptor__ Descripteur )
		{
			descriptor__ Libre = Successeur_( Descripteur ), LibreS = Successeur_( Libre );

			EcrireIndicateurLibre_( Libre, Nombre_( Libre ) + Nombre_( LibreS ) + MMM1_NOMBRE_INDICATEUR );
		}
		/* Raccourcit la portion 'Descripteur' pour qu'elle contienne le nombre
		de bloc 'Nombre' en allongeant la portion libre qui la suit. */
		void Raccourcir_(
			descriptor__ Descripteur,
			nombre__ Nombre )
		{
			descriptor__ Successeur = Successeur_( Descripteur );
			nombre__ NombreLibre = Nombre_( Descripteur ) - Nombre + Nombre_( Successeur );

			if ( S_.Libre == Successeur )
				S_.Libre = 0;

			EcrireIndicateurOccupe_( Descripteur, Nombre, EstOccupe_( Descripteur ) );
			EcrireIndicateurLibre_( Successeur_( Descripteur ), NombreLibre );
		}
		/* Raccourcit la portion 'Descripteur' pour qu'elle contienne le nombre
		de bloc 'Nombre' en insérant une portion libre juste aprés. */
		void Inserer_(
			descriptor__ Descripteur,
			nombre__ Nombre )
		{
			nombre__ NombreLibre = Nombre_( Descripteur ) - Nombre;

			if ( NombreLibre > 1 )
			{
				descriptor__ Successeur;

				EcrireIndicateurOccupe_( Descripteur, Nombre, EstOccupe_( Descripteur ) );

				Successeur = Successeur_( Descripteur );

				EcrireIndicateurLibre_( Successeur, NombreLibre - 1 );
				SignalerPredecesseurLibre_( Successeur_( Successeur ) );
			}
		}
		/* Ajuste la portion de descripteur 'Descripteur' pour qu'elle contienne le nombre
		de bloc 'Bloc' etsachant qu'elle est en bout de mémoire (ajustement de la mémoire principale). */
		void Ajuster_(
			descriptor__ Descripteur,
			nombre__ Nombre )
		{
			EcrireIndicateurOccupe_( Descripteur, Nombre, PredecesseurEstOccupe_( Descripteur ) );
			AjusterMemoireApresDescripteur_( Descripteur );
		}
		// Ecrit l'indicateur 'Indicateur' pour la portion libre de descripteur 'Descripteur'.
		void EcrireIndicateurLibre_(
			descriptor__ Descripteur,
			indicateur__ Indicateur )
		{
			MemoireEcrire_( (mdr::datum__ *)&Indicateur, MMM1_TAILLE_INDICATEUR, Descripteur - MMM1_TAILLE_INDICATEUR );
			MemoireEcrire_( (mdr::datum__ *)&Indicateur, MMM1_TAILLE_INDICATEUR, Descripteur + NombreEnTaille_( Indicateur ) - MMM1_TAILLE_INDICATEUR );

			TesterEtAffecterLibre_( Descripteur );
		}
		/* Ecrit l'indicateur pour la portion d'indicateur 'Indicateur' correspondant au nombre
		'Nombre' et à l'occupation du prdecesseur 'OccupationPredecesseur' */
		void EcrireIndicateurOccupe_(
			descriptor__ Descripteur,
			nombre__ Nombre,
			bso::bool__ PredecesseurOccupe )
		{
			indicateur__ Indicateur = Nombre | MMM1_MASQUE_OCCUPATION | ( PredecesseurOccupe ? MMM1_MASQUE_OCCUPATION_PREDECESSEUR : 0 );

			MemoireEcrire_( (mdr::datum__ *)&Indicateur, MMM1_TAILLE_INDICATEUR, Descripteur - MMM1_TAILLE_INDICATEUR );
		}
		/* Signale à la portion 'Descripteur' (occupée) qu'elle est précédée par une portion
		occupée */
		void SignalerPredecesseurOccupe_( descriptor__ Descripteur )
		{
			EcrireIndicateurOccupe_( Descripteur, Nombre_( Descripteur ), true );
		}
		/* Signale à la portion 'Descripteur' (occupée) qu'elle est précédée par une portion
		libre */
		void SignalerPredecesseurLibre_( descriptor__ Descripteur )
		{
			EcrireIndicateurOccupe_( Descripteur, Nombre_( Descripteur ), false );
		}
		// Retourne l'indicateur correspondant au bloc de descripteur 'Descripteur'.
		indicateur__ Indicateur_( descriptor__ Descripteur ) const
		{
			indicateur__ Indicateur;

			MemoireLire_( Descripteur - MMM1_TAILLE_INDICATEUR, MMM1_TAILLE_INDICATEUR, (mdr::datum__ *)&Indicateur );

			return Indicateur;
		}
		// Retourne le nombre de bloc occupée par la portion de descripteur 'Descripteur'.
		nombre__ Nombre_( descriptor__ Descripteur ) const
		{
			return Indicateur_( Descripteur ) & MMM1_MASQUE_NOMBRE;
		}
		// Retourne la capacité de la portion de descripteur 'Descripteur'.
		mdr::size__ Taille_( descriptor__ Descripteur ) const
		{
			return NombreEnTaille_( Nombre_( Descripteur ) );
		}
		// Retourne vrai si le bloc de descripteur 'Descripteur' est occupé.
		bso::bool__ EstOccupe_( descriptor__ Descripteur ) const
		{
			return ( Indicateur_( Descripteur ) & MMM1_MASQUE_OCCUPATION ) != 0;
		}
		// Retourne vrai si le bloc de descripteur 'Descripteur' est libre.
		bso::bool__ EstLibre_( descriptor__ Descripteur ) const
		{
			return !( Indicateur_( Descripteur ) & MMM1_MASQUE_OCCUPATION );
		}
		// Retourne vrai si le bloc de descripteur 'Descritpeur' est occupé.
		bso::bool__ PredecesseurEstOccupe_( descriptor__ Descripteur ) const
		{
			return ( Indicateur_( Descripteur ) & MMM1_MASQUE_OCCUPATION_PREDECESSEUR ) != 0;
		}
		// Retourne vrai si le bloc de descripteur 'Descripteur' est libre.
		bso::bool__ PredecesseurEstLibre_( descriptor__ Descripteur ) const
		{
			return !( Indicateur_( Descripteur ) & MMM1_MASQUE_OCCUPATION_PREDECESSEUR );
		}
		/* Retourne l'indicateur du predecesseur de la portion de descripteur 'Descripteur'.
		N'a de sens que si le predecesseur est libre. */
		indicateur__ IndicateurPredecesseur_( descriptor__ Descripteur ) const
		{
			indicateur__ Indicateur;

			MemoireLire_( Descripteur - 2 * MMM1_TAILLE_INDICATEUR, MMM1_TAILLE_INDICATEUR, (mdr::datum__ *)&Indicateur );

			return Indicateur;
		}
		/* Retourne le nombre de bloc du predecesseur de la portion de descripteur 'Descripteur'.
		N'a de sens que si le predecesseur est libre. */
		nombre__ NombrePredecesseur_( descriptor__ Descripteur ) const
		{
			return IndicateurPredecesseur_( Descripteur ) & MMM1_MASQUE_NOMBRE;
		}
		/* Retourne la capacité du predecesseur de la portion de descripteur 'Descripteur'.
		N'a de sens que si le predecesseur est libre. */
		mdr::size__ TaillePredecesseur_( descriptor__ Descripteur ) const
		{
			return NombreEnTaille_( NombrePredecesseur_( Descripteur ) );
		}
		/* Retourne le descripteur du predecesseur de la portion de descripteur 'Descripteur'.
		N'a de sens que si le prédecesseur est libre. */
		descriptor__ PredecesseurLibre_( descriptor__ Descripteur ) const
		{
			return Descripteur - MMM1_TAILLE_INDICATEUR - TaillePredecesseur_( Descripteur );
		}
		// retourne le descripteur pointant sur le premier espace
		descriptor__ PremierDescripteur_( void ) const
		{
			return sizeof( indicateur__ );
		}
		// retourne vrai si la portion de descripteur 'Descripteur' est la dernière de la multimémoire.
		bso::bool__ EstDernier_( descriptor__ Descripteur ) const
		{
			return HorsLimite_( Successeur_( Descripteur ) );
		}
		// Libère la portion de descripteur 'Descripteur'. Effectue tous les ajustements.
		void Liberer_( descriptor__ Descripteur )
		{
			if( EstDernier_( Descripteur ) )
			{
				if ( PredecesseurEstLibre_( Descripteur ) )
					AjusterMemoireAvantDescripteur_( PredecesseurLibre_( Descripteur ) );
				else
					AjusterMemoireAvantDescripteur_( Descripteur );
			}
			else
			{
				descriptor__ Successeur = Successeur_( Descripteur );
				nombre__ Nombre;

				if ( PredecesseurEstLibre_( Descripteur ) )
				{
					descriptor__ Predecesseur = PredecesseurLibre_( Descripteur );

					Nombre = Nombre_( Predecesseur ) + Nombre_( Descripteur ) + MMM1_NOMBRE_INDICATEUR;

					if ( EstLibre_( Successeur ) )
						Nombre += Nombre_( Successeur ) + MMM1_NOMBRE_INDICATEUR;
					else
						SignalerPredecesseurLibre_( Successeur );

					EcrireIndicateurLibre_( Predecesseur, Nombre );
				}
				else
				{
					Nombre = Nombre_( Descripteur );

					if ( EstLibre_( Successeur ) )
						Nombre += Nombre_( Successeur ) + MMM1_NOMBRE_INDICATEUR;
					else
						SignalerPredecesseurLibre_( Successeur );

					EcrireIndicateurLibre_( Descripteur, Nombre );
				}
			}
		}
		descriptor__ Successeur_( descriptor__ Descripteur ) const
		{
			return Descripteur + Taille_( Descripteur ) + MMM1_TAILLE_INDICATEUR;
		}
		// Retourne vrai si le descripteur 'Descripteur' pointe en dehors de la multimémoire.
		bso::bool__ HorsLimite_( descriptor__ Descripteur ) const
		{
			return Descripteur >= S_.Capacite;
		}
		/* Retourne le nombre de blocs disponibles pour la portion 'Descripteur'
		en tenant compte des éventuels portion libre qui suivent ou qui précèdent. */
		nombre__ NombreDisponibles_( descriptor__ Descripteur ) const
		{
			nombre__ Nombre = Nombre_( Descripteur );
			descriptor__ Successeur = Successeur_( Descripteur );

			if ( PredecesseurEstLibre_( Descripteur ) )
				Nombre += Nombre_( PredecesseurLibre_( Descripteur ) ) + MMM1_NOMBRE_INDICATEUR;

			if ( EstLibre_( Successeur ) )
				Nombre += Nombre_( Successeur ) + MMM1_NOMBRE_INDICATEUR;

			return Nombre;
		}
		// Affiche la structure de la memoire. A des fins de déboguage.
		void AfficherStructure_( txf::text_oflow__ &Flow ) const;
		// Retourne un descripteur sur une portion nouvellement allouée.
		descriptor__ Allouer_( nombre__ Nombre )
		{
			descriptor__ Descripteur = 0;

			if ( Nombre )
			{
				if ( !S_.Libre || ( Nombre_( S_.Libre ) < Nombre ) )
				{
					Descripteur = S_.Capacite + MMM1_TAILLE_INDICATEUR;

					AllouerMemoire_( Descripteur + NombreEnTaille_( Nombre ) );

					EcrireIndicateurOccupe_( Descripteur, Nombre, true );
				}
				else
				{
					nombre__ NombreLibre = Nombre_( S_.Libre ) - Nombre;

					Descripteur = S_.Libre;

					S_.Libre = 0;

					if ( NombreLibre <= 1 )
					{
						SignalerPredecesseurOccupe_( Successeur_( Descripteur ) );

						EcrireIndicateurOccupe_( Descripteur, Nombre_( Descripteur ), true );
					}
					else
					{
						NombreLibre--;

						EcrireIndicateurOccupe_( Descripteur, Nombre, true );
						EcrireIndicateurLibre_( Successeur_( Descripteur ), NombreLibre );
					}
				}
			}

			return Descripteur;
		}
		void Deplacer_(
			descriptor__ Source,
			descriptor__ Destination )
		{
			MemoireEcraser_( Destination, Source, Taille_( Source ) );
		}
	public:
		struct s 
		{
			descriptor__ MultimemoryDriverDescriptor;
			mdr::size__ MultimemoryDriverExtent;
			bso::ubyte__ MultimemoryDriverAddendum;
			uym::untyped_memory_::s Memoire_;
			mdr::size__ Capacite;
				// Descripteur à essayer lorsque l'on a besoin d'un emplacement libre.
			descriptor__ Libre;
		} &S_;
		multimemory_( s &S )
		: S_( S ),
		  PiloteMultimemoire_( S.MultimemoryDriverDescriptor, S.MultimemoryDriverAddendum, S.MultimemoryDriverExtent ),
		  Memoire_( S.Memoire_ )
		{}
		void reset( bool P = true )
		{
			PiloteMultimemoire_.reset( P );
			Memoire_.reset( P );
			S_.Capacite = 0;
			S_.Libre = 0;
			/* doit contenir une valeur valide
			(c'est-à-dire comme une de celle après l'initilisation),
			sans quoi la librairie CTN ne fonctionnera plus */
		}
		// preparation d'un objet vierge
		void plug( multimemory_ &M )
		{
			PiloteMultimemoire_.Init( M );
			Memoire_.plug( PiloteMultimemoire_ );
		}
		void plug( mdr::E_MEMORY_DRIVER__ &MD )
		{
			PiloteMultimemoire_.reset();
			Memoire_.plug( MD );
		}
#if 0
		void write( flw::oflow___ &OFlow ) const
		{
			flw::Put( S_.Capacite, OFlow );
			Memoire_.write( 0, S_.Capacite, OFlow );
		}
		void read( flw::iflow___ &IFlow )
		{
			flw::Get( IFlow, S_.Capacite );

			Memoire_.Allocate( S_.Capacite );

			Memoire_.read( IFlow, 0, S_.Capacite );
		}
#endif
		//f Initialization.
		void Init( void )
		{
			Memoire_.Init();

			S_.Capacite = 0;
			S_.Libre = 0;
		}
		//f Return a descriptor of a memory of 'Size' size.
		descriptor__ Allocate( mdr::size__ Size )
		{
			return Allouer_( TailleEnNombre_( Size ) );
		}
		/*f Return a descriptor of a memory of 'Size' size
		based on the 'Descriptor' descriptor. The returned descriptor
		must be used instead of 'Descriptor'. The contents of the memory
		is conserved. */
		descriptor__ Reallocate(
			descriptor__ Descriptor,
			mdr::size__ Size,
			bso::ubyte__ )	// Facilite la mise en oeuvre de la V2.
		{
			nombre__ Nombre = TailleEnNombre_( Size );

			if ( !Descriptor )
				Descriptor = Allouer_( Nombre );
			else if ( !Size )
			{
				Liberer_( Descriptor );
				Descriptor = 0;
			}
			else if ( HorsLimite_( Successeur_( Descriptor ) ) )
			{
				if ( PredecesseurEstLibre_( Descriptor ) )
				{
					descriptor__ NouveauDescripteur = PredecesseurLibre_( Descriptor );

					Permuter_( Descriptor );

					Descriptor = NouveauDescripteur;
				}

				Ajuster_( Descriptor, Nombre );
			}
			else if ( NombreDisponibles_( Descriptor ) >= Nombre )
			{
				bso::bool__ SuccesseurLibre = EstLibre_( Successeur_( Descriptor ) );

				if ( PredecesseurEstLibre_( Descriptor ) )
				{
					descriptor__ NouveauDescripteur = PredecesseurLibre_( Descriptor );

					Permuter_( Descriptor );

					Descriptor = NouveauDescripteur;

					if ( SuccesseurLibre )
						Fusionner_( Descriptor );

					SuccesseurLibre = true;
				}

				if ( SuccesseurLibre )
					if ( Nombre_( Descriptor ) >= Nombre )
						Raccourcir_( Descriptor, Nombre );
					else
						Etendre_( Descriptor, Nombre );
				else
					Inserer_( Descriptor, Nombre );
			}
			else
			{
				descriptor__ NouveauDescripteur = Allouer_( Nombre );

				Deplacer_( Descriptor, NouveauDescripteur );

				Liberer_( Descriptor );

				Descriptor = NouveauDescripteur;
			}

			return Descriptor;
		}
		//f Free the 'Descriptor' memory.
		void Free( descriptor__ Descriptor )
		{
			if ( Descriptor )
				Liberer_( Descriptor );
		}
		//f Return true if the multimemory empty, false otherwise.
		bso::bool__ IsEmpty( void ) const
		{
			return !S_.Capacite;
		}
		//f Put 'Amount' bytes in 'Buffer' from the 'Descriptor' memory at 'Position' .
		void Read(
			descriptor__ Descriptor,
			mdr::row_t__ Position,
			mdr::size__ Amount,
			mdr::datum__ *Buffer,
			bso::ubyte__ )	const // Facilite la mise en oeuvre de la V2.
		{
			MemoireLire_( Descriptor + Position, Amount, Buffer );
		}
		//f Put 'Amount' bytes at 'Position' to the 'Descriptor' memory from 'Buffer'.
		void Write(
			const mdr::datum__ *Buffer,
			mdr::size__ Amount,
			descriptor__ Descriptor,
			mdr::row_t__ Position,
			bso::ubyte__ )	// Facilite la mise en oeuvre de la V2.
		{
			MemoireEcrire_( Buffer, Amount, Position + Descriptor );
		}
		// écrit 'Nombre' octets à la position 'Position'
	/*	descriptor__ Descripteur( mmm__indice_bloc IndiceBloc )
		{
			if ( !IndiceBloc )
				return 0;
			else
				return S_.Descripteur( IndiceBloc );
		}
		// retourne le descripteur correspondant au bloc d'indice 'IndiceBloc'
		mmm__indice_bloc IndiceBloc( descriptor__ Descripteur )
		{
			if ( !Descripteur )
				return 0;
			else
				return IndiceBloc_( Descripteur );
		}*/
		//f Print the multimemory structure.
		void PrintStructure( txf::text_oflow__ &Flow ) const
		{
			AfficherStructure_( Flow );
		}
		multimemory_ &operator =( const multimemory_ &Op )
		{
			S_.Capacite = Op.S_.Capacite;
			S_.Libre = Op.S_.Libre;

			Memoire_.Allocate( Op.S_.Capacite );

			Memoire_.Store_( Op.Memoire_, Op.S_.Capacite, 0 );

			return *this;
		}
		//f Flushes the caches.
		void Flush( void ) const
		{
			Memoire_.Flush();
		}
		//f Retourne la capacite de la portion de descripteur 'Descripteur'.
		mdr::size__ Size( descriptor__ Descriptor ) const
		{
			if ( !Descriptor )
				return 0;
			else
				return Taille_( Descriptor );
		}
		//f The size of all the multimemory.
		mdr::size__ Size( void )
		{
			return S_.Capacite;
		}
		uym::untyped_memory_ &GetUnderlyingMemory( void )
		{
			return Memoire_;
		}
	};

#ifndef FLM__COMPILATION
	typedef uym::untyped_memory_file_manager___	multimemory_file_manager___;

	inline bso::bool__ Connect(
		multimemory_ &Memory,
		multimemory_file_manager___ &FileManager )
	{
		bso::bool__ Exists = uym::Connect( Memory.GetUnderlyingMemory(), FileManager );

		if ( Exists )
			Memory.S_.Capacite = FileManager.FileSize();

		return Exists;
	}
#endif


}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
