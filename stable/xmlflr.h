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

//	$Id: xmlflr.h,v 1.2 2013/07/25 15:59:16 csimon Exp $

#ifndef XMLFLR__INC
#define XMLFLR__INC

#define XMLFLR_NAME		"XMLFLR"

#define	XMLFLR_VERSION	"$Revision: 1.2 $"

#define XMLFLR_OWNER		"Claude SIMON"

#if defined( E_DEBUG ) && !defined( XMLFLR_NODBG )
#define XMLFLR_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.2 $
//C Claude SIMON (csimon at zeusw dot org)
//R $Date: 2013/07/25 15:59:16 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D XML FiLleR 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"
#include "xmlbsc.h"
#include "xmldoc.h"

namespace xmlflr {
	using xmlelm::erow__;
	using xmlelm::type__;
	using xmlelm::name_;
	using xmlelm::name;

	using xmlval::vrow__;
	using xmlval::value_;
	using xmlval::value;

	class filler__
	{
	private:
		xmldoc::document_ *_Document;
		erow__ _CurrentElement;
		vrow__ _CurrentValue;
		vrow__ _RootValue;
		erow__ _GetOrCreateChildElement(
			const name_ &Name,
			type__ Type )
		{
			erow__ Row = xmlelm::Search( Name, Type, _Document->Structure, _CurrentElement );

			if ( Row == qNIL ) {
				Row = _Document->Structure.Create( Name, Type );
				_Document->Structure.BecomeLastChild( Row, _CurrentElement );
			}

			return Row;
		}
	public:
		void reset( bso::bool__ = true )
		{
			_Document = NULL;
			_CurrentElement = qNIL;
			_RootValue = _CurrentValue = qNIL;
		}
		filler__( void )
		{
			reset( false );
		}
		~filler__( void )
		{
			reset();
		}
		vrow__ Init(
			xmldoc::document_ &Document,
			vrow__ Root )
		{
			reset();

			_Document = &Document;
			_CurrentElement = Document.Data.GetOddity( Root );
			return _RootValue = _CurrentValue = Root;
		}
		vrow__ Init(
			xmldoc::document_ &Document,
			const name_ &RootName )
		{
			reset();

			_Document = &Document;

			_CurrentElement = Document.Structure.Create( RootName, xmlelm::tTag );
			return _RootValue = _CurrentValue = Document.Data.Create( value( "" ), _CurrentElement );
		}
		vrow__ Init(
			xmldoc::document_ &Document,
			const char *RootName )
		{
			return Init( Document, name( RootName ) );
		}
		vrow__ PushTag( const name_ &Name )
		{
			_CurrentElement = _GetOrCreateChildElement( Name, xmlelm::tTag );
			return _CurrentValue = _Document->Data.BecomeLastChild( value( "" ), _CurrentElement, _CurrentValue );
		}
		vrow__ PushTag( const char *Name )
		{
			return PushTag( name( Name ) );
		}
		vrow__ PutValue( const value_ &Value )
		{
			_Document->Data.Store( Value, _CurrentValue );

			return _CurrentValue;
		}
		vrow__ PutValue( const char *Value )
		{
			return PutValue( value( Value ) );
		}
		void PutValue(
			const value_ &Value,
			const name_ &Name )
		{
			PushTag( Name );

			PutValue( Value );

			PopTag();
		}
		void PutValue(
			const value_ &Value,
			const char *Name )
		{
			PutValue( Value, name( Name ) );
		}
		void PutValue(
			const char *Value,
			const char *Name )
		{
			PutValue( value( Value ), name( Name ) );
		}
		vrow__ PutAttribute(
			const name_ &Name,
			const value_ &Value )
		{
			erow__ ERow = _GetOrCreateChildElement( Name, xmlelm::tAttribute );

			return _Document->Data.BecomeLastChild( Value, ERow, _CurrentValue );	
		}
		vrow__ PutAttribute(
			const char *Name,
			const value_ &Value )
		{
			return PutAttribute( name( Name ), Value );
		}
		vrow__ PutAttribute(
			const char *Name,
			const char *Value )
		{
			return PutAttribute( name( Name ), value( Value ) );
		}
		erow__ PopTag( void )
		{
			_CurrentValue = _Document->Data.Parent( _CurrentValue );

			return _CurrentElement = _Document->Structure.Parent( _CurrentElement );
		}
		vrow__ GetRoot( void )
		{
			return _RootValue;
		}
		void Put(
			const xmldoc::document_ &Document,
			vrow__ Root );
	};
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
