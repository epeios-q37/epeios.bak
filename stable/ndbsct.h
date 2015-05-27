/*
	Copyright (C) 2000-2015 Claude SIMON (http://q37.info/contact/).

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

//	$Id: ndbsct.h,v 1.22 2013/04/11 19:36:51 csimon Exp $

#ifndef NDBSCT__INC
#define NDBSCT__INC

#define NDBSCT_NAME		"NDBSCT"

#define	NDBSCT_VERSION	"$Revision: 1.22 $"

#define NDBSCT_OWNER		"Claude SIMON"


#if defined( E_DEBUG ) && !defined( NDBSCT_NODBG )
#define NDBSCT_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.22 $
//C Claude SIMON (csimon at zeusw dot org)
//R $Date: 2013/04/11 19:36:51 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D New DataBase Static Content (will be moved on its own project in the future) 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"
#include "ndbbsc.h"
#include "lst.h"

namespace ndbsct {
	using ndbbsc::rrow__;
	using ndbbsc::datum_;
	using ndbbsc::datum;

	typedef tys::E_STORAGE_( ndbbsc::atom__ )	storage_;

	typedef lst::E_LISTt_( rrow__ ) _list_;

	class static_content_
	: public _list_
	{
	protected:
		virtual void LSTAllocate(
			sdr::size__ Amount,
			aem::mode__ Mode )
		{
			Storage.Allocate( Amount * S_.Size );
		}
		void _Touch( void )
		{
			S_.ModificationEpochTimeStamp = tol::EpochTime( false );
		}
	public:
		storage_ Storage;
		struct s
		: public _list_::s
		{
			storage_::s Storage;
			// Taille de l'lment.
			sdr::size__ Size;
			time_t ModificationEpochTimeStamp;
		} &S_;
		static_content_( s &S )
		: S_( S ),
		  _list_( S ),
		  Storage( S.Storage )
		{}
		void reset( bso::bool__ P = true )
		{
			_list_::reset( P );
			Storage.reset( P );

			S_.Size = 0;
			S_.ModificationEpochTimeStamp = 0;
		}
		void plug( ags::E_ASTORAGE_ &AS )
		{
			_list_::plug( AS );
			Storage.plug( AS );
		}
		static_content_ &operator =( const static_content_ &SC )
		{
			if ( ( Amount() != 0 ) || ( SC.Amount() != 0 ) ) {
				_list_::operator =( SC );
				Storage.Allocate( SC.Extent() * S_.Size );
				Storage.Store( SC.Storage, SC.Extent() * S_.Size );
				//S_.ModificationTimeStamp = SC.S_.ModificationTimeStamp;	// Ecras par le '_Touch()' ci-dessous.

				_Touch();
			}

			S_.Size = SC.S_.Size;

			return *this;
		}
		void Init( sdr::size__ Size )
		{
			_list_::Init();
			Storage.Init();

			S_.Size = Size;
			S_.ModificationEpochTimeStamp = 0;
		}
		void Erase( rrow__ Row )
		{
			_list_::Delete( Row );

			_Touch();
		}
		void Store(
			const datum_ &Datum,
			rrow__ Row )
		{
			if ( Datum.Amount() != S_.Size )
				ERRDta();

			Storage.Store( Datum, S_.Size, *Row * S_.Size );

			_Touch();
		}
		rrow__ Store( const datum_ &Datum )
		{
			rrow__ Row = _list_::New();

			Store( Datum, Row );

			return Row;
		}
		// Retourne 'true' si l'enregistrement existe, faux sinon.
		void Retrieve(
			rrow__ Row,
			datum_ &Datum ) const
		{
			Datum.Append( Storage, S_.Size, *Row * S_.Size );
		}
		// Retourne 'true' si l'enregistrement existe, faux sinon.
		void Retrieve(
			rrow__ Row,
			datum_ &Datum,
			ndbbsc::cache_ &Cache ) const
		{
			if ( ( &Cache == NULL ) || !Cache.Retrieve( Row, Datum ) ) {

				Retrieve( Row, Datum );

				if ( &Cache != NULL )
					Cache.Store( Datum, Row );
			}
		}
		// Reconstruction de la liste des items disponibles dans 'Entries' (sous-objet 'list_').
		void RebuildLocations( void )
		{
			ERRVct();
		}
		E_RODISCLOSE_( time_t, ModificationEpochTimeStamp );
		E_RODISCLOSE_( sdr::size__, Size );
	};

	E_AUTO( static_content )

	class files_hook___
	{
	private:
		static_content_ *_Content;
		str::string _BaseFileName;
		tys::files_hook___ _Memory;
		lst::files_hook___ _List;
		fil::mode__ _Mode;
		void _ErasePhysically( void )
		{
			_Memory.Drop();
			_List.Drop();
		}
	public:
		void reset( bso::bool__ P = true )
		{
			_Memory.ReleaseFile();	// Pour que les 'TimeStamp' des fichiers soient mis  jour.

			if ( P ) {
				Settle();	// Lanc explicitement, car le 'reset(...)' de '_List' ne peut lancer son propre 'Settle(...)'.
			}

			_Memory.reset( P );
			_List.reset( P );
			_BaseFileName.reset( P );
			_Mode = fil::m_Undefined;
			_Content = NULL;
		}
		files_hook___( void )
		{
			reset( false );
		}
		~files_hook___( void )
		{
			reset();
		}
		void Init(
			const str::string_ &BaseFileName,
			fil::mode__ Mode,
			fls::id__ ID );
		time_t GetUnderlyingFilesLastModificationTime( void ) const
		{
			time_t TimeStamp = _Memory.TimeStamp();
			// Lors d'une suppression d'un enregistrement le fichier derrire '_Memory' n'est pas touch, mais les fichiers accessoires
			// doivent quand mme tre sauvs, d'o le code ci-dessous.

			if ( ( _Content != NULL ) &&
				 ( ( TimeStamp == 0 ) || ( _Content->ModificationEpochTimeStamp() > TimeStamp ) ) )
				 TimeStamp = _Content->ModificationEpochTimeStamp();

			return TimeStamp;
		}
		void Set( static_content_ &Content )
		{
			if ( _Content != NULL )
				ERRPrm();

			_Content = &Content;
		}
		uys::state__ Bind( void )
		{
			uys::state__ State = _Memory.Bind();

			if ( State != _List.Bind( GetUnderlyingFilesLastModificationTime() ) )
				return uys::sInconsistent;

			return State;
		}
		uys::state__ Settle( void )
		{
			uys::state__ State = _Memory.Settle();

			if ( ( _Content != NULL ) && ( _Content->ModificationEpochTimeStamp() != 0 ) )
				_List.Settle( GetUnderlyingFilesLastModificationTime() );

			return State;
		}
		void ReleaseFiles( void )	// Pour librer les 'file handlers'.
		{
			_Memory.ReleaseFile();
			_List.ReleaseFiles();
		}
		void SwitchMode( fil::mode__ Mode )
		{
			if ( Mode != _Mode ) {
				_Memory.Mode( Mode );

				_Mode = Mode;
			}
		}
		tys::files_hook___ &MemoryFilesHook( void )
		{
			return _Memory;
		}
		lst::files_hook___ &ListFilesHook( void )
		{
			return _List;
		}
		E_RODISCLOSE__( str::string_, BaseFileName );

	};

	inline uys::state__ Plug(
		static_content_ &Content,
		files_hook___ &Hook )
	{
		uys::state__ State = tys::Plug( Content.Storage, Hook.MemoryFilesHook() );

		if ( uys::IsError( State ) ) {
			Hook.reset();
		} else {
			if ( State != lst::Plug( Content, Hook.ListFilesHook(), Content.Storage.Size()/Content.Size(), Hook.GetUnderlyingFilesLastModificationTime() ) ) {
				Hook.reset();
				return uys::sInconsistent;
			} else 
				Hook.Set( Content );
		}

		return State;
	}


}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
