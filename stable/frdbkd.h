/*
	Header for the 'frdbkd' library by Claude SIMON (csimon at zeusw dot org)
	Copyright (C) 2004 Claude SIMON.

	This file is part of the Epeios (http://zeusw.org/epeios/) project.

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
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

//	$Id: frdbkd.h,v 1.15 2013/06/11 10:32:24 csimon Exp $

#ifndef FRDBKD__INC
#define FRDBKD__INC

#define FRDBKD_NAME		"FRDBKD"

#define	FRDBKD_VERSION	"$Revision: 1.15 $"

#define FRDBKD_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &FRDBKDTutor;

#if defined( E_DEBUG ) && !defined( FRDBKD_NODBG )
#define FRDBKD_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.15 $
//C Claude SIMON (csimon at zeusw dot org)
//R $Date: 2013/06/11 10:32:24 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D FRontenD BacKenD 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

# error "Obsolete ! Use 'FRDFRD' instead !'"

#include "err.h"
#include "flw.h"

#include "csducl.h"
#include "fblfub.h"

namespace frdbkd {

	using fblfub::compatibility_informations__;

	using fblfub::incompatibility_informations_;
	using fblfub::incompatibility_informations;


	class backend___
	{
	private:
		csducl::universal_client_ioflow___ _Flow;
		fblfub::backend_universal_access___ _Backend;
	public:
		void reset( bso::bool__ P = true )
		{
			_Backend.reset( P );
			_Flow.reset( P );
		}
		bso::bool__ Init(
			const char *Language,
			const compatibility_informations__ &CompatibilityInformations,
			csducl::universal_client_core &ClientCore,
			fblfrd::reporting_functions__ &ReportingFunctions,
			incompatibility_informations_ &IncompatibilityInformations )
		{
			fblfub::mode__ Mode = fblfub::m_Undefined;

			_Flow.Init( ClientCore );

			switch ( ClientCore.GetType() ) {
			case csducl::tLibrary:
				Mode = fblfub::mEmbedded;
				break;
			case csducl::tDaemon:
				Mode = fblfub::mRemote;
				break;
			default:
				ERRFwk();
				break;
			}

			return _Backend.Init( Language, CompatibilityInformations, _Flow, Mode, ReportingFunctions, IncompatibilityInformations );
		}
		fblfrd::backend___ &Backend( void )
		{
			return _Backend;
		}
		bso::bool__ IsConnected( void ) const
		{
			return _Backend.IsConnected();
		}
		const char *GetMessage( void ) const
		{
			return _Backend.GetMessage();
		}
		void About(
			str::string_ &ProtocolVersion,
			str::string_ &BackendLabel,
			str::string_ &APIVersion,
			str::string_ &BackendInformations,
			str::string_ &BackendCopyright,
			str::string_ &SoftwareInformations )
		{
			_Backend.About( ProtocolVersion, BackendLabel, APIVersion, BackendInformations, BackendCopyright, SoftwareInformations );
		}
		void ThrowERRFwk( void )
		{
			_Backend.ThrowERRFwk();
		}
	};

# if 0
	template <typename statics> class backend___
	: public _backend___,
	  public statics
	{
	public:
		void reset( bso::bool__ P = true )
		{
			_backend___::reset( P );
			statics::reset( P );
		}
		E_CVDTOR( backend___ )
		bso::bool__ Init(
			const char *Language,
			const char *BackendLabel,
			const char *APIBackendVersion,
			csducl::universal_client_core &ClientCore,
			fblfrd::error_reporting_functions__ &ErrorReportingFunctions,
			str::string_ &Message,
			str::string_ &URL )
		{
			statics::Init( BackendAccess() );
			return _backend___::Init( Language, BackendLabel, APIVersion,ClientCore, ErrorReportingFunctions, MEssage, URL );
		}
	};

	template <typename common> class backend_object_common___
	{
	protected:
		common Common;
	public:
		void reset( bso::bool__ P = true )
		{
			Common.reset( P );
		}
		void Init( _backend___ &Backend )
		{
			Common.Init( Backend.BackendAccess() );
		}
	};

	template <typename object> E_TMIMIC__( object, backend_object___ );
# endif

	template <typename type> inline void _PutId(
		type Id,
		const char *Name,
		type UndefinedValue,
		bso::bool__ Always,
		xml::writer_ &Writer )
	{
		if ( Id != UndefinedValue )
			xml::PutAttribute( Name, *Id, Writer );
		else if ( Always )
			Writer.PutAttribute( Name, "" );
	}

	inline void PutId(
		fbltyp::id__ Id,
		const char *Name,
		bso::bool__ Always,
		xml::writer_ &Writer )
	{
		_PutId( Id, Name, FBLTYP_UNDEFINED_ID, Always, Writer );
	}

	inline void PutId(
		fbltyp::id32__ Id,
		const char *Name,
		bso::bool__ Always,
		xml::writer_ &Writer )
	{
		_PutId( Id, Name, FBLTYP_UNDEFINED_ID32, Always, Writer );
	}

	inline void PutId(
		fbltyp::id16__ Id,
		const char *Name,
		bso::bool__ Always,
		xml::writer_ &Writer )
	{
		_PutId( Id, Name, FBLTYP_UNDEFINED_ID16, Always, Writer );
	}

	inline void PutId(
		fbltyp::id8__ Id,
		const char *Name,
		bso::bool__ Always,
		xml::writer_ &Writer )
	{
		_PutId( Id, Name, FBLTYP_UNDEFINED_ID8, Always, Writer );
	}

}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
