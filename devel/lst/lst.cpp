/*
	'lst.cpp' by Claude SIMON (http://zeusw.org/).

	'lst' is part of the Epeios framework.

    The Epeios framework is free software: you can redistribute it and/or
	modify it under the terms of the GNU General Public License as published
	by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Epeios framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The Epeios framework.  If not, see <http://www.gnu.org/licenses/>.
*/

#define LST__COMPILATION

#include "lst.h"

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

#include "flf.h"
#include "str.h"

static inline void Save_(
	sdr::row__ Row,
	flw::oflow__ &Flow )
{
	dtfptb::FPut( *Row, Flow );
}

static void Save_(
	const stk::E_BSTACK_( sdr::row__ ) &Bunch,
	flw::oflow__ &Flow )
{
	stk::row__ Row = Bunch.First();

	while ( Row != E_NIL ) {
		Save_( Bunch( Row ), Flow );

		Row = Bunch.Next( Row );
	}
}

uys::state__ lst::WriteToFile_(
	const store_ &Store,
	const fnm::name___ &FileName )
{
	uys::state__ State = uys::s_Undefined;
ERRProlog
	flf::file_oflow___ Flow;
ERRBegin
	if ( Flow.Init( FileName, err::hUserDefined ) != tol::rSuccess ) {
		State = uys::sInconsistent;
		ERRReturn;
	}

	Save_( Store.Released, Flow );

	State = uys::sExists;
ERRErr
ERREnd
ERREpilog
	return State;
}

static inline void Load_(
	flw::iflow__ &Flow,
	sdr::row__ &Row )
{
	dtfptb::FGet( Flow, *Row );
}
	
static void Load_(
	flw::iflow__ &Flow,
	sdr::size__ Amount,
	stk::E_BSTACK_( sdr::row__ ) &Stack )
{
	sdr::row__ Row;

	while ( Amount-- ) {
		Load_( Flow, Row );
		Stack.Append( Row );
	}
}

uys::state__ lst::ReadFromFile_(
	const fnm::name___ &FileName,
	store_ &Store )
{
	uys::state__ State = uys::s_Undefined;
ERRProlog
	flf::file_iflow___ Flow;
	fil::size__ Size = 0;
ERRBegin
	if ( Flow.Init( FileName, err::hUserDefined ) != tol::rSuccess ) {
		State = uys::sInconsistent;
		ERRReturn;
	}

	Size = fil::GetSize( FileName );

	if ( Size > SDR_SIZE_MAX )
		ERRDta();

	Load_( Flow, (bso::size__)Size / sizeof( sdr::row__ ), Store.Released );

	State = uys::sExists;
ERRErr
ERREnd
ERREpilog
	return State;
}

// Retourne l'lment succdant  'Element', ou LST_INEXISTANT si inexistant.
sdr::row_t__ lst::Successeur_(
	sdr::row_t__ Element,
	sdr::size__ Amount,
	const store_ &Libres )
{
	while( ( ++Element < Amount ) && Libres.IsAvailable( Element ) ) {};

	if ( Element >= Amount )
		return E_NIL;
	else
		return Element;
}

// Retourne l'lment prcdent 'Element', ou LST_INEXISTANT si inexistant.
sdr::row_t__ lst::Predecesseur_(
	sdr::row_t__ Element,
	const store_ &Libres )
{
	bso::bool__ Trouve = false;

	while( ( Element > 0 ) && !( Trouve = !Libres.IsAvailable( --Element ) ) ) {};

	if ( Trouve )
		return Element;
	else
		return E_NIL;
}

void lst::MarkAsReleased_(
	sdr::row_t__ First,
	sdr::row_t__ Last,
	store_ &Store )
{
	while ( First < Last )
		Store.RestorationRelease( Last-- );

	Store.RestorationRelease( First );
}

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class lstpersonnalization
{
public:
	lstpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~lstpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the ending of the application  */
	}
};


				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

static lstpersonnalization Tutor;
