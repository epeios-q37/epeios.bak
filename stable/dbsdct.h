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

//	$Id: dbsdct.h,v 1.32 2012/11/14 16:06:26 csimon Exp $

#ifndef DBSDCT_INC_
#define DBSDCT_INC_

#define DBSDCT_NAME		"DBSDCT"

#define	DBSDCT_VERSION	"$Revision: 1.32 $"

#define DBSDCT_OWNER		"Claude SIMON (http://zeusw.org/intl/contact.html)"

#if defined( E_DEBUG ) && !defined( DBSDCT_NODBG )
#define DBSDCT_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.32 $
//C Claude SIMON (http://zeusw.org/intl/contact.html)
//R $Date: 2012/11/14 16:06:26 $

/* End of automatic documentation generation part. */

/* Addendum to the automatic documentation generation part. */
//D DataBaSe Dynamic Content 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"
#include "str.h"
#include "dtfptb.h"
#include "stk.h"
#include "lstbch.h"
#include "lstctn.h"
#include "flm.h"
#include "que.h"
#include "dbsbsc.h"

namespace dbsdct {

	using mdr::size__;

	using dbsbsc::atom__;
	using dbsbsc::datum_;
	using dbsbsc::datum;
	using dbsbsc::rrow__;
	using dbsbsc::_cache_;

	E_ROW( datum_row__ );

	typedef tym::memory_<atom__, datum_row__> memory_;

	class storage_
	{
	private:
		/* Retourne le nombre d'octets que l'on peut effectivement crire sachant que l'on veut crire un maximum de 
		'Wanted' octets et qu'un maximum de 'Available' octets sont disponibles, et que l'on veut galement y stocker la
		taille */
		size__ _AmountWritable(
			size__ Wanted,
			size__ Available )
		{
#ifdef DBSDCT_DBG
			if ( Available == 0 )
				ERRc();
#endif
			if ( Wanted >= Available )
				Wanted = Available - 1;	// Il faut au moins 1 octet pour stocker la taille.

			while ( Available < ( dtfptb::GetSizeLength( Wanted ) + Wanted ) )
				Wanted--;

			return Wanted;
		}
		size__ _Store(
			const datum_ &Data,
			size__ Offset,
			datum_row__ &Row,
			size__ Available )
		{
			size__ &Amount = Available;
			
			Amount = _AmountWritable( Data.Amount() - Offset, Available );

			dtfptb::size_buffer__ SizeBuffer;
			bso::ubyte__ SizeLength = dtfptb::GetSizeLength( Amount );

			dtfptb::PutSize( Amount, SizeBuffer );

			Memory.Store( (const atom__ *)SizeBuffer, SizeLength, Row );

			*Row += SizeLength;

			Memory.Store( *(const memory_ *)&Data, Amount, Row, Offset );

			*Row += Data.Amount() - Offset;

			return Amount;
		}
		/* Ajoute 'Data'  la position 'Row', sachant que 'Unallocated' est la position du premier octet non allou.
		L'espace necessaire est allou. Retourne la position du nouveau premier octet non allou. */
		datum_row__ _Store(
			const datum_ &Data,
			size__ Offset,
			datum_row__ Unallocated )
		{
			size__ DataAmount = Data.Amount() - Offset;
			size__ TotalSize = DataAmount + dtfptb::GetSizeLength( DataAmount );

			Memory.Allocate( *Unallocated + TotalSize );

			_Store( Data, Offset, Unallocated, TotalSize );	// 'Unallocated' mis  jour par cette mthode.

			return Unallocated;
		}
		size__ _GetComputedSize(
			datum_row__ Row,
			datum_row__ Unallocated ) const
		{
			dtfptb::size_buffer__ SizeBuffer;
			bso::ubyte__ PossibleSizeLength = sizeof( SizeBuffer );

			if ( ( *Unallocated - *Row ) < PossibleSizeLength )
				PossibleSizeLength = (bso::ubyte__)( *Unallocated - *Row );

			Memory.Recall( Row, PossibleSizeLength, (atom__ *)SizeBuffer );

			return dtfptb::GetSize( SizeBuffer );
		}
		size__ _GetRawSize(
			datum_row__ Row,
			datum_row__ Unallocated ) const
		{
			size__ Size = _GetComputedSize( Row, Unallocated );

			return Size + dtfptb::GetSizeLength( Size );
		}
	public:
		memory_ Memory;
		struct s
		{
			memory_::s Memory;
		};
		storage_( s &S )
		: Memory( S.Memory )
		{}
		void reset( bso::bool__ P = true )
		{
			Memory.reset( P );
		}
		void plug( mdr::E_MEMORY_DRIVER__ &MD )
		{
			Memory.plug( MD );
		}
		void plug( mmm::E_MULTIMEMORY_ &MM )
		{
			Memory.plug( MM );
		}
		storage_ operator =( const storage_ &S )
		{
			ERRu();	// Impossible de dupliquer : ne connat pas sa taille.

			return *this;	// Pour viter un 'warning'.
		}
		void Init( void )
		{
			Memory.Init();
		}
		/* Stocke 'Data'  la position 'Row'. sachant qu'il y a 'Available' octets de disponibles  cette position.
		'Unallocated' est la position du premier octet non allou. S'il n'y a pas assez de place  'Row', alors
		une partie de 'Data' est plac  'Unavailable' (en allouant l'espace ncessaire), qui est alors modifi
		pour pointer sur la nouvelle position du premier octet non allou. Si 'Row', au retour, est != 'NONE',
		alors il reste 'Available' octets  'Row'. */
/*
		datum_row__ Write(
			const datum_ &Data,
			datum_row__ &Row,
			size__ &Available,
			datum_row__ &Unallocated )
		{
			size__ Written = _Store( Data, Row, Available );
			size__ TotalWritten = Written + dtfptb::GetSizeLength( Written );
			datum_row__ Rest = NONE;

			Available -= TotalWritten;

			if ( Available != 0 )
				*Row += TotalWritten;
			else
				Row = NONE;

			if ( Written != Data.Amount() ) {
				Rest = Unallocated;
				Unallocated = _Store( Data, Written, Unallocated );
			}

			return Rest;
		}
*/
		size__ Store(
			const datum_ &Data,
			size__ Offset,
			datum_row__ &Row,
			size__ &Available )
		{
			size__ Written = _Store( Data, Offset, Row, Available );
			size__ TotalWritten = Written + dtfptb::GetSizeLength( Written );

			Available -= TotalWritten;

			if ( Available == 0 )
				Row = NONE;

			return Written;
		}
		/* Ajoute 'Data'  'Row', qui est la position du premier octet non allou. La place ncessaire est alloue et
		la nouvelle position du premier octet non allou et retourn. */
		datum_row__ Append(
			const datum_ &Data,
			size__ Offset,
			datum_row__ Row )
		{
			return _Store( Data, Offset, Row );
		}
		size__ GetComputedSize(
			datum_row__ Row,
			datum_row__ Unallocated ) const
		{
			return _GetComputedSize( Row, Unallocated );
		}
		size__ GetRawSize(
			datum_row__ Row,
			datum_row__ Unallocated ) const
		{
			return _GetRawSize( Row, Unallocated );
		}
		/* Place un marqueur de taille  'DataRow' sachant qu'il y a 'Size' octets de disponibles. Retourne le nombre
		d'octets effectivement disponibles  cette position aprs y avoir plac le marquer de taille. */
		size__ StoreSize(
			datum_row__ Row,
			size__ Size )
		{
#ifdef DBSDCT_DBG
			if ( Size == 0 )
				ERRu();
#endif
			dtfptb::size_buffer__ SizeBuffer;

			Size = _AmountWritable( Size, Size );

			dtfptb::PutSize( Size, SizeBuffer );

			Memory.Store( (const atom__ *)SizeBuffer, dtfptb::GetSizeLength( Size ), Row );

			return Size;
		}
		void Retrieve(
			datum_row__ Row,
			datum_ &Data,
			datum_row__ Unallocated ) const
		{
			mdr::size__ Size = _GetComputedSize( Row, Unallocated );

			Data.Append( *(const tym::memory_<bso::char__, epeios::row__>*)&Memory, Size, *Row + dtfptb::GetSizeLength( Size ) );
		}
	};

	E_AUTO( storage )

	// Caratristique d'un emplacement libre.
	struct available__
	{
		// Position.
		datum_row__ Row;
		// Taille brute, c'est--dire sans tenir compte de la place occupe par le marqueur de taille.
		size__ RawSize;
		available__( void )
		{
			Row = NONE;
			RawSize = 0;
		}
		bool operator !=( const available__ &A ) const
		{
			return ( Row != A.Row ) || ( RawSize != A.RawSize );
		}
	};

	// Pile contenant tous les emplacement libres.
	typedef stk::E_BSTACK_( available__ ) availables_;
	E_AUTO( availables )


	// 'datum_row__' portable
	typedef bso::p_ulong__	p_datum_row__;

	struct entry__
	{
	public:
		datum_row__ Head;
		datum_row__ Tail;
		entry__( void )
		{
			Head = Tail = NONE;
		}
	};

	typedef lstbch::E_LBUNCHt_( entry__, rrow__ ) entries_;

	typedef lstbch::list_bunch_file_manager___ entries_file_manager___;

	typedef dbsbsc::file_features_	_file_features_;

	class dynamic_content_
	: public _file_features_
	{
	protected:
		virtual void DBSBSCCompleteInitialization( bso::bool__ )
		{
			// Rien  faire.
		}
		virtual void DBSBSCErasePhysically( void )
		{
			reset();
		}
	private:
		size__ _StoreInAvailable(
			const datum_ &Data,
			size__ Offset,
			datum_row__ &Row )
		{
			available__ Available;
			size__ Written;

			Available = Availables.Pop();

			Row = Available.Row;

			Written = Storage.Store( Data, Offset, Available.Row, Available.RawSize );

			if ( Available.Row != NONE ) {
#ifdef DBSDCT_DBG
				if ( Written != ( Data.Amount() - Offset ) )
					ERRc();
#endif
				Storage.StoreSize( Available.Row, Available.RawSize );
				Availables.Push( Available );
			}

			return Written;
		}
		void _Append(
			const datum_ &Data,
			size__ Offset,
			datum_row__ &Row )
		{
			entry__ Entry;

			Row = S_.Unallocated;

			S_.Unallocated = Storage.Append( Data, Offset, S_.Unallocated );
		}
		rrow__ _Store(
			const datum_ &Data,
			rrow__ Row )
		{
			entry__ Entry;

			if ( Availables.Amount() != 0 ) {
				size__ Written;
	
				Written = _StoreInAvailable( Data, 0, Entry.Head );

				if ( ( Written != Data.Amount() )
					 && ( Availables.Amount() != 0 )
					 && ( Storage.GetComputedSize( Availables.Top().Row, S_.Unallocated ) >= ( Data.Amount() - Written ) ) )
					_StoreInAvailable( Data, Written, Entry.Tail );
				else
					_Append( Data, Written, Entry.Tail );
			} else
				_Append( Data, 0, Entry.Head );

			Entries.Store( Entry, Row );

			return Row;
		}
		void _Erase( datum_row__ DataRow )
		{
			available__ Available;

			Available.Row = DataRow;
			Available.RawSize = Storage.GetRawSize( DataRow, S_.Unallocated );

			Availables.Push( Available );
		}
		void _Erase( rrow__ Row )
		{
			entry__ Entry = Entries.Get( Row );

			if ( Entry.Tail != NONE )
				_Erase( Entry.Tail );

			if ( Entry.Head != NONE )
				_Erase( Entry.Head );

			Entry.Head = Entry.Tail = NONE;

			Entries.Store( Entry, Row );
		}
		void _Retrieve(
			datum_row__ Row,
			datum_ &Data ) const
		{
			Storage.Retrieve( Row, Data, S_.Unallocated );
		}
		void _Touch( void )
		{
			S_.ModificationTimeStamp = tol::Clock( false );
		}
	public:
		storage_ Storage;
		availables_ Availables;
		entries_ Entries;
		struct s
		: public _file_features_::s
		{
			storage_::s Storage;
			availables_::s Availables;
			entries_::s Entries;
			_cache_::s Cache;
			// Position du premier octet non allou.
			datum_row__ Unallocated;
			time_t ModificationTimeStamp;
		} &S_;
		dynamic_content_( s &S )
		: S_( S ),
		  _file_features_( S ),
		  Storage( S.Storage ),
		  Availables( S.Availables ),
		  Entries( S.Entries )
		{}
		void reset( bso::bool__ P = true )
		{
			Storage.reset( P );
			Availables.reset( P );
			Entries.reset( P );

			S_.Unallocated = 0;
			S_.ModificationTimeStamp = 0;
			_file_features_::reset( P );
		}
		void plug( mmm::E_MULTIMEMORY_ &MM )
		{
			Storage.plug( MM );
			Availables.plug( MM );
			Entries.plug( MM );
		}
		dynamic_content_ &operator =( const dynamic_content_ &DC )
		{
			Storage.Memory.Allocate( *DC.S_.Unallocated );
			Storage.Memory.Store( DC.Storage.Memory, *DC.S_.Unallocated );
			S_.Unallocated = DC.S_.Unallocated;
			S_.ModificationTimeStamp = DC.S_.ModificationTimeStamp;

			Availables = DC.Availables;

			Entries = DC.Entries;

			return *this;
		}
		void Init( bso::bool__ Partial = false )
		{
			Storage.Init();
			Availables.Init();
			Entries.Init();

			S_.Unallocated = 0;
			S_.ModificationTimeStamp = 0;

			_file_features_::Init( Partial );
		}
		rrow__ Store( const datum_ &Data )
		{
			rrow__ Row = Entries.New();

			_Store( Data, Row );

			_Touch();

			return Row;
		}
		void Erase( rrow__ Row )
		{
			_Erase( Row );

			Entries.Delete( Row );

			_Touch();
		}
		void Store(
			const datum_ &Data,
			rrow__ Row )
		{
			_Erase( Row );

			_Store( Data, Row );

			_Touch();
		}
		// Retourne 'true' si l'enregistrement existe, faux sinon.
		bso::bool__ Retrieve(
			rrow__ Row,
			datum_ &Datum ) const
		{
			entry__ Entry = Entries.Get( Row );

			if ( Entry.Head != NONE )
				_Retrieve( Entry.Head, Datum );
			else
				return false;

			if ( Entry.Tail != NONE )
				_Retrieve( Entry.Tail, Datum );

			return true;
		}
		// Retourne 'true' si l'enregistrement existe, faux sinon.
		bso::bool__ Retrieve(
			rrow__ Row,
			datum_ &Datum,
			_cache_ &Cache ) const
		{
			bso::bool__ Exists = true;

			if ( ( &Cache == NULL ) || !Cache.Retrieve( Row, Datum ) ) {

				Exists = Retrieve( Row, Datum );

				if ( Exists && ( &Cache != NULL ) )
					Cache.Store( Datum, Row );
			}

			return Exists;
		}
		// Reconstruction de la liste des items disponibles dans 'Entries' (sous-objet 'list_').
		void RebuildLocations( void )
		{
			ERRl();
		}
		// Reconstruit la liste des portions inoccups dans 'Storage'.
		void _RebuildAvailables( void )
		{
			ERRl();
		}
		void ErasePhysically( void )	// Efface tout, y compris la table sous-jacente.
		{
			_file_features_::ErasePhysically();
		}
		E_NAVt( Entries., rrow__ )
		E_RODISCLOSE_( time_t, ModificationTimeStamp );
	};

	E_AUTO( dynamic_content )

	// Content stock dans des fichiers.
	class exploded_dynamic_content_
	: public dynamic_content_
	{
	protected:
		virtual void DBSBSCCompleteInitialization( bso::bool__ )
		{
			_ConnectToFiles();
		}
		virtual void DBSBSCErasePhysically( void )
		{
			dynamic_content_::DBSBSCErasePhysically();
			_ErasePhysically();
		}
	private:
		time_t _GetUnderlyingFilesLastModificationTime( void ) const
		{
			time_t ContentTimeStamp, EntriesTimeStamp;

			ContentTimeStamp = S_.StorageFileManager.TimeStamp();
			EntriesTimeStamp = S_.EntriesFileManager.TimeStamp();

			if ( ContentTimeStamp > EntriesTimeStamp )
				return ContentTimeStamp;
			else
				return EntriesTimeStamp;
		}
		void _SaveLocationsAndAvailables( void ) const;
		bso::bool__ _ConnectToFiles( void );
		void _ErasePhysically( void );
	public:
		str::string_ RootFileName;
		struct s
		: public dynamic_content_::s
		{
			tym::memory_file_manager___ StorageFileManager;
			entries_file_manager___ EntriesFileManager;
			str::string_::s RootFileName;
			mdr::mode__ Mode;
		} &S_;
		exploded_dynamic_content_( s &S )
		: S_( S ), 
		  dynamic_content_( S ),
		  RootFileName( S.RootFileName )
		{}
		void reset( bso::bool__ P = true )
		{
			S_.StorageFileManager.ReleaseFile();
			S_.EntriesFileManager.ReleaseFile();
			// Pour que les 'TimeStamp' des fichiers soient mis  jour.

			if ( P ) {
				if ( ( RootFileName.Amount() != 0 ) && ( ModificationTimeStamp() != 0 ) )
					_SaveLocationsAndAvailables();
			}

			S_.StorageFileManager.reset( P );
			S_.EntriesFileManager.reset( P );
			dynamic_content_::reset( P );
			S_.Mode = mdr::m_Undefined;
			RootFileName.reset( P );
		}
		void plug( mmm::E_MULTIMEMORY_ & )
		{
			ERRu();	// Cette mthode n'a pas de sens dans ce contexte.
		}
		exploded_dynamic_content_ &operator =( const exploded_dynamic_content_ &XDC )
		{
			dynamic_content_::operator =( XDC );

			return *this;
		}
		void Init(
			const str::string_ &RootFileName,
			mdr::mode__ Mode,
			bso::bool__ Partial,
			flm::id__ ID );
		void WriteLocationsAndAvailablesFiles( void )	// Met  jour les fichiers.
		{
			_SaveLocationsAndAvailables();
		}
		void CloseFiles( void )	// Pour librer les 'file handlers'.
		{
			S_.StorageFileManager.ReleaseFile();
			S_.EntriesFileManager.ReleaseFile();
		}
		void SwitchMode( mdr::mode__ Mode )
		{
			if ( Mode != S_.Mode ) {
				S_.StorageFileManager.Mode( Mode );
				S_.EntriesFileManager.Mode( Mode );

				S_.Mode = Mode;
			}
		}
	};

	E_AUTO( exploded_dynamic_content )
}

/*$END$*/
#endif
