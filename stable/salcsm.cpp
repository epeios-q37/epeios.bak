/*
	'salcsm' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Requires the 'salcsm' header file ('salcsm.h').
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



//	$Id: salcsm.cpp,v 1.13 2012/11/14 16:06:32 csimon Exp $

#define SALCSM__COMPILATION

#include "salcsm.h"

class salcsmtutor
: public ttr_tutor
{
public:
	salcsmtutor( void )
	: ttr_tutor( SALCSM_NAME )
	{
#ifdef SALCSM_DBG
		Version = SALCSM_VERSION "\b\bD $";
#else
		Version = SALCSM_VERSION;
#endif
		Owner = SALCSM_OWNER;
		Date = "$Date: 2012/11/14 16:06:32 $";
	}
	virtual ~salcsmtutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace salcsm;

using bch::bunch;

bso::bsize__ salcsm::tampon::Ecrire(
	const flw::datum__ *Tampon,
	bso::bsize__ Taille )
{
	E_BUNCH(flw::datum__)::Allocate( Ecriture_ + Taille );

	E_BUNCH(flw::datum__)::Store( Tampon, Taille, Ecriture_ );

	Ecriture_ += Taille;

	return Taille;
}

bso::bsize__ salcsm::tampon::Lire(
	bso::bsize__ Minimum,
	flw::datum__ *Tampon,
	bso::bsize__ Desire )
{
	if ( Desire > ( Ecriture_ - Lecture_ ) )
		Desire = Ecriture_ - Lecture_;

	if ( Desire < Minimum )
		ERRf();

	E_BUNCH( flw::datum__ )::Recall( Lecture_, Desire, Tampon );

	Lecture_ += Desire;

	if ( Lecture_ == Ecriture_ )
		Lecture_ = Ecriture_ = 0;

	return Desire;
}


flw::amount__ salcsm::flot::FLWGet(
	flw::amount__ Minimum,
	flw::datum__ *Buffer,
	flw::amount__ Wanted )
{
	return (flw::amount__)Lecture_->Lire( (bso::bsize__)Minimum, Buffer, (bso::bsize__)Wanted );
}

flw::amount__ salcsm::flot::FLWPut(
	const flw::datum__ *Buffer,
	flw::amount__ Wanted,
	flw::amount__ Minimum,
	bool Synchronization )
{
	return (flw::amount__)Ecriture_->Ecrire( Buffer, (bso::bsize__)Minimum );
}

flw::amount__ salcsm::flot_serveur_actif::FLWPut(
	const flw::datum__ *Buffer,
	flw::amount__ Wanted,
	flw::amount__ Minimum,
	bool Synchronization )
{
	flw::amount__ &Amount = Minimum;

	Amount = flot::FLWPut( Buffer, Wanted, Minimum, Synchronization );

#ifdef SALCSM_DBG
	if ( Functions_ == NULL )
		ERRc();
#endif

	if ( Synchronization && ( Amount == Wanted ) )
		Functions_->SP( *Flot_, UP_ );

	return Amount; 
}

flw::amount__ salcsm::flot_client_actif::FLWPut(
	const flw::datum__ *Buffer,
	flw::amount__ Wanted,
	flw::amount__ Minimum,
	bool Synchronization )
{
	flw::amount__ &Amount = Minimum;

	Amount = flot::FLWPut( Buffer, Wanted, Minimum, Synchronization );

#ifdef SALCSM_DBG
	if ( Functions_ == NULL )
		ERRc();
#endif

	if ( Synchronization && ( Amount == Wanted ) )
			Functions_->CP( *Client_, *Serveur_, UP_ );

	return Amount;
}

flw::ioflow___ &salcsm::manager___::Process( void )
{ 
	void *PUS = NULL;
	void *PUC = NULL;

	TamponServeurActif_.Init();
	TamponServeurPassif_.Init();
	TamponClientActif_.Init();
	TamponClientPassif_.Init();

	PUS = SI();

	ServeurPassif_.Init( TamponServeurPassif_, TamponServeurActif_ );
	ServeurActif_.Init( TamponServeurActif_, TamponServeurPassif_, this, ServeurPassif_, PUS );

	ClientPassif_.Init( TamponClientPassif_, TamponClientActif_ );

	PUC = CI( ClientPassif_, ServeurActif_ );

	ClientActif_.Init( TamponClientActif_, TamponClientPassif_, this, ServeurActif_, ClientPassif_, PUC );

	return ClientActif_;
}

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */
class salcsmpersonnalization
: public salcsmtutor
{
public:
	salcsmpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~salcsmpersonnalization( void )
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

static salcsmpersonnalization Tutor;

ttr_tutor &SALCSMTutor = Tutor;
