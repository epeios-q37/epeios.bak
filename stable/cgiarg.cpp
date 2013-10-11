/*
	'cgiarg' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Requires the 'cgiarg' header file ('cgiarg.h').
	Copyright (C) 2001-2004 Claude SIMON (http://zeusw.org/intl/contact.html).

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



//	$Id: cgiarg.cpp,v 1.4 2013/05/15 14:40:35 csimon Exp $

#define CGIARG__COMPILATION

#include "cgiarg.h"

class cgiargtutor
: public ttr_tutor
{
public:
	cgiargtutor( void )
	: ttr_tutor( CGIARG_NAME )
	{
#ifdef CGIARG_DBG
		Version = CGIARG_VERSION "\b\bD $";
#else
		Version = CGIARG_VERSION;
#endif
		Owner = CGIARG_OWNER;
		Date = "$Date: 2013/05/15 14:40:35 $";
	}
	virtual ~cgiargtutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace cgiarg;

row__ arguments_::Locate(
	const name_ &Name,
	row__ Begin,
	comparison_method__ Method ) const
{
	row__ &P = Begin;
ERRProlog
	name NameBuffer;
ERRBegin
	NameBuffer.Init();

	if ( Begin == E_NIL )
		P = First();
	else
		P = Next( Begin );

	while( P != E_NIL ) {
		if ( GetName( P, NameBuffer ).Amount() >= Name.Amount() )
			if ( str::Compare( NameBuffer, Name, 0, 0, Name.Amount() ) == 0 )
				if ( Method == cmBegins )
					break;
				else if ( Method == cmStrictly ) {
					if ( NameBuffer.Amount() == Name.Amount() ) 
						break;
				} else
					ERRFwk();

		P = Next( P );
	}
ERRErr
ERREnd
ERREpilog
	return P;
}

row__ arguments_::Locate(
	const name_ &Name,
	const suffix_ &Suffix,
	comparison_method__ Method ) const
{
	row__ P = E_NIL;
ERRProlog
	name NameBuffer;
	suffix SuffixBuffer;
ERRBegin
	NameBuffer.Init();
	SuffixBuffer.Init();

	P = First();

	while( P != E_NIL ) {
		if ( GetName( P, NameBuffer ).Amount() >= Name.Amount() )
			if ( ( str::Compare( NameBuffer, Name, 0, 0, Name.Amount() ) == 0 )
				&& ( str::Compare( GetSuffix( P, SuffixBuffer ), Suffix ) == 0 ) )
				if ( Method == cmBegins )
					break;
				else if ( Method == cmStrictly ) {
					if ( NameBuffer.Amount() == Name.Amount() ) 
						break;
				} else
					ERRFwk();

		P = Next( P );
	}
ERRErr
ERREnd
ERREpilog
	return P;
}

void arguments_::Parse(
	flw::iflow__ &Flow,
	bso::char__ SuffixDelimiter )
{
	char Buffer[1000];
	row__ P;

	if ( !flw::GetString( Flow, Buffer, sizeof( Buffer ) ) )
		ERRLmt();

	while( strlen( Buffer ) )
	{
		char *SuffixDelimiterPosition = strchr( Buffer, SuffixDelimiter );

		if ( SuffixDelimiterPosition != NULL ) {
			*SuffixDelimiterPosition = 0;
			if ( *++SuffixDelimiterPosition != 0 )
				P = Add( Buffer, SuffixDelimiterPosition, "" );
			else
				P = Add( Buffer );
		} else
			P = Add( Buffer );

		if ( !flw::GetString( Flow, Buffer, sizeof( Buffer ) ) )
			ERRLmt();

		PutValue( str::string( Buffer ), P );

		if ( !flw::GetString( Flow, Buffer, sizeof( Buffer ) ) )
			ERRLmt();
	}
}

#ifndef CPE__MT
const name_ &arguments_::GetName( row__ Position ) const
{
	static name Name;

	Name.Init();

	GetName( Position, Name );

	return Name;
}

const value_ &arguments_::GetValue( row__ Position ) const
{
	static value Value;

	Value.Init();

	GetValue( Position, Value );

	return Value;
}

const suffix_ &arguments_::GetSuffix( row__ Position ) const
{
	static suffix Suffix;

	Suffix.Init();

	GetSuffix( Position, Suffix );

	return Suffix;
}
#endif

bso::bool__ arguments_::GetValue(
	const name_ &Name,
	value_ &Value ) const
{
	row__ P = Locate( Name );

	while( ( P != E_NIL ) && ( GetSuffixPosition( P ) != E_NIL ) )
		P = Locate( Name, P );

	if ( P != E_NIL ) {
		GetValue( P, Value );
		return true;
	} else
		return false;
}

txf::text_oflow__ &operator <<(
	txf::text_oflow__ &F,
	const cgiarg::arguments_ &A )
{
ERRProlog
	cgiarg::row__ P = E_NIL;
	str::string Buffer;
ERRBegin
	P = A.First();

	while( P != E_NIL )
	{
		Buffer.Init();
		F << A.GetName( P, Buffer );

		if ( A.GetSuffixPosition( P ) != E_NIL ) {
			Buffer.Init();
			F << CGIARG_SUFFIX_DELIMITER << A.GetSuffix( P, Buffer );
		}

		Buffer.Init();
		F << A.GetSuffix( P, Buffer ) << txf::nl;

		P = A.Next( P );
	}
ERRErr
ERREnd
ERREpilog
	return F;
}


/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class cgiargpersonnalization
: public cgiargtutor
{
public:
	cgiargpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~cgiargpersonnalization( void )
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

static cgiargpersonnalization Tutor;

ttr_tutor &CGIARGTutor = Tutor;
