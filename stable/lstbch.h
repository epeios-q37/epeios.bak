/*
	Header for the 'lstbch' library by Claude SIMON (http://zeusw.org/intl/contact.html)
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

//	$Id: lstbch.h,v 1.46 2013/04/06 14:50:46 csimon Exp $

#ifndef LSTBCH__INC
#define LSTBCH__INC

#define LSTBCH_NAME		"LSTBCH"

#define	LSTBCH_VERSION	"$Revision: 1.46 $"

#define LSTBCH_OWNER		"Claude SIMON (http://zeusw.org/intl/contact.html)"

#include "ttr.h"

extern class ttr_tutor &LSTBCHTutor;

#if defined( E_DEBUG ) && !defined( LSTBCH_NODBG )
#define LSTBCH_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.46 $
//C Claude SIMON (http://zeusw.org/intl/contact.html)
//R $Date: 2013/04/06 14:50:46 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D LiST BunCH 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"
#include "bch.h"
#include "lst.h"

namespace lstbch {

	using lst::list_;
	using bch::bunch_;

	//c Bunch associated to a list.
	template <typename type, typename row, typename row_t> class list_bunch_
	: public list_<row, row_t>,
	  public bunch_<type, row>
	{
	protected:
		virtual void LSTAllocate(
			sdr::size__ Size,
			aem::mode__ Mode )
		{
			bunch_<type, row>::Allocate( Size, Mode );
		}
	public:
		struct s
		: public list_<row, row_t>::s,
		  public bunch_<type, row>::s
		{};
		list_bunch_( s &S )
		: list_<row, row_t>( S ),
		  bunch_<type, row>( S )
		{}
		void reset( bso::bool__ P = true )
		{
			list_<row, row_t>::reset( P );
			bunch_<type, row>::reset( P );
		}
		void plug( ags::E_ASTORAGE_ &AS )
		{
			list_<row, row_t>::plug( AS );
			bunch_<type, row>::plug( AS );
		}
		list_bunch_ &operator =( const list_bunch_ &LB )
		{
			list_<row, row_t>::operator =( LB );
			bunch_<type, row>::operator =( LB );

			return *this;
		}
		//f Initialization.
		void Init( void )
		{
			list_<row, row_t>::Init();
			bunch_<type, row>::Init();
		}
		E_NAVt( list_<E_COVER2(row,row_t)>::, row )
		//f Add 'Object' and return its row.
		row Add( const type &Object )
		{
			row Row = list_<row, row_t>::New();

			bunch_<type, row>::Store( Object, Row );

			return Row;
		}
		//f Delete entry 'Row'.
		void Delete( row Row )
		{
			list_<row, row_t>::Delete( Row );
		}
		//f Create new entry and return its row.
		row New( void )
		{
			return list_<row, row_t>::New();
		}
		//f Create new entry with row 'Row'.
		row New( row Row )
		{
			return list_<row, row_t>::New( Row );
		}
		// To avoid the use of herited 'Append' methods.
		void Append( void ) const
		{
			ERRFbd();
		}
		// To avoid the use of herited 'Remove' methods.
		void Remove( void ) const
		{
			ERRFbd();
		}
		bunch_<type, row> &Bunch( void )
		{
			return *this;
		}
		const bunch_<type, row> &Bunch( void ) const
		{
			return *this;
		}
		list_<row, row_t> &List( void )
		{
			return *this;
		}
		const list_<row, row_t> &List( void ) const
		{
			return *this;
		}
	};

	E_AUTO3( list_bunch )

#ifndef FLS__COMPILATION

	class list_bunch_file_manager___
	{
	private:
		bch::bunch_file_manager___ _BunchFileManager;
		lst::list_file_manager___ _ListFileManager;
	public:
		void reset( bso::bool__ P = true )
		{
			_BunchFileManager.ReleaseFile();	// Sinon le 'Settle()'  ci-dessous ne fonctionne pas correctement.

			if ( P ) {
				Settle();	// Lancé explicitement, car le 'reset(...)' de '_ListFileManager' ne peut lancer son propre 'Settle(...)'.
			}

			_BunchFileManager.reset( P );
			_ListFileManager.reset( P );
		}
		list_bunch_file_manager___( void )
		{
			reset( false );
		}
		~list_bunch_file_manager___( void )
		{
			reset();
		}
		void Init(
			const char *BunchFileName,
			const char *ListFileName,
			fil::mode__ Mode,
			bso::bool__ Persistent,
			fls::id__ ID )
		{
			reset();

			_BunchFileManager.Init( BunchFileName, Mode, Persistent, ID );
			_ListFileManager.Init( ListFileName, Mode, Persistent );

		}
		uys::state__ Bind( void )	// A n'appeler qu'aprés un appel à 'Plug(...)'.
		{
			uys::state__ State = _BunchFileManager.Bind();

			if ( uys::IsError( State ) )
				return State;
			
			if ( _ListFileManager.Bind( _BunchFileManager.TimeStamp() ) != State )
				State = uys::sInconsistent;

			return State;
		}
		uys::state__ Settle( void )
		{
			uys::state__ State = _BunchFileManager.Settle();

			if ( _BunchFileManager.IsPersistent() && _BunchFileManager.Exists() )
				if ( _ListFileManager.Settle( _BunchFileManager.TimeStamp() ) != State )
					State = uys::sInconsistent;

			return State;
		}
		void Drop( void )
		{
			_ListFileManager.Drop();
			_BunchFileManager.Drop();
		}
		uys::state__ State( void ) const
		{
			uys::state__ State = _BunchFileManager.State();

			if ( State != _ListFileManager.State() )
				State = uys::sInconsistent;

			return State;
		}
#ifdef CPE__VC
#	undef CreateFile
#endif
		bso::bool__ CreateFiles( err::handling__ ErrorHandling = err::h_Default )
		{
			bso::bool__ Success = _BunchFileManager.CreateFiles( ErrorHandling );

			if ( !Success )
				return false;

			if ( uys::IsError( Settle() ) ) {
				_BunchFileManager.Drop();
				Success = false;
			}

			return Success;
		}
		time_t TimeStamp( void ) const
		{
			time_t BunchTimeStamp = _BunchFileManager.TimeStamp();
			time_t ListTimeStamp = _ListFileManager.TimeStamp();

			if ( BunchTimeStamp > ListTimeStamp )
				return BunchTimeStamp;
			else
				return ListTimeStamp;
		}
		void ReleaseFile( void )
		{
			_BunchFileManager.ReleaseFile();
		}
		void Mode( fil::mode__ Mode )
		{
			_BunchFileManager.Mode( Mode );
		}
		bso::bool__ IsPersistent( void ) const
		{
			bso::bool__ Is = _BunchFileManager.IsPersistent();

			if ( Is != _ListFileManager.IsPersistent() )
				ERRFwk();

			return Is;
		}
		template <typename list_bunch> friend uys::state__ Plug(
			list_bunch &ListBunch,
			list_bunch_file_manager___ &FileManager );
	};


	template <typename list_bunch> uys::state__ Plug(
		list_bunch &ListBunch,
		list_bunch_file_manager___ &FileManager )
	{
		uys::state__ State = bch::Plug( ListBunch.Bunch(), FileManager._BunchFileManager );

		if ( uys::IsError( State ) )
			FileManager.reset();
		else {
			fil::size__ Size = FileManager._BunchFileManager.FileSize() / ListBunch.GetItemSize();

			if ( Size > SDR_SIZE_MAX )
				ERRDta();

			if ( lst::Plug( ListBunch, FileManager._ListFileManager, (sdr::size__)Size, FileManager._BunchFileManager.TimeStamp() ) != State ) {
				FileManager.reset();
				State = uys::sInconsistent;
			}
		}

		return State;
	}
#endif

	#define E_LBUNCHtx_( type, row, row_t )		list_bunch_<type, row, row_t>
	#define E_LBUNCHtx( type, row, row_t )		list_bunch<type, row, row_t>

	#define E_LBUNCHt_( type, row )	E_LBUNCHtx_( type, row, sdr::row_t__)
	#define E_LBUNCHt( type, row )	E_LBUNCHtx( type, row, sdr::row_t__)

	#define  E_LBUNCH_( type )		E_LBUNCHt_( type, sdr::row__ )
	#define  E_LBUNCH( type )		E_LBUNCHt( type, sdr::row__ )
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
