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

#ifndef XHTBASE__INC
# define XHTBASE__INC

# define XHTBASE_NAME		"XHTBASE"

# if defined( E_DEBUG ) && !defined( XHTBASE_NODBG )
#  define XHTBASE_DBG
# endif

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

// XHT(ML) BASE

# include "err.h"
# include "flw.h"
# include "tol.h"
# include "flx.h"
# include "xml.h"

namespace xhtbase {

	E_CDEF( char *, ContentTagName, "Content" );
	E_CDEF( char *, ContextTagName, "Context" );

	class corpus_callback__
	{
	protected:
		virtual void XHTBASEDump( xml::writer_ &Writer ) = 0;
	public:
		void reset( bso::bool__ = true )
		{
			//Standardisation.
		}
		E_CVDTOR( corpus_callback__ )
		void Init( void )
		{
			//Standardisation.
		}
		void Dump( xml::writer_ &Writer )
		{
			XHTBASEDump( Writer );
		}
	};

	class generic_rack___
	{
	private:
		flx::E_STRING_TOFLOW___ _Flow;
		xml::writer _Writer;
	public:
		void reset( bso::bool__ P = true )
		{
			_Writer.reset( P );
			_Flow.reset( P );
		}
		E_CDTOR( generic_rack___ );
		void Init(
			const char *Generator,
			const char *View,
			const char *Background,
			str::string_ &Target,
			corpus_callback__ &Callback )
		{
			_Flow.Init( Target );
			_Writer.Init( _Flow, xml::oIndent, xml::e_Default );
			_Writer.PushTag( "RichFrontEnd" );
			_Writer.PutAttribute("View", View );
			_Writer.PutAttribute("Background", Background );
			_Writer.PutAttribute("Generator", Generator );
			_Writer.PutAttribute("TimeStamp", tol::DateAndTime() );
			_Writer.PutAttribute("Enviroment", CPE_ENVIROMENT_LABEL );
			_Writer.PutAttribute("Architecture", CPE_ARCHITECTURE_LABEL );
			_Writer.PushTag( "Corpus" );
			Callback.Dump( _Writer );
			_Writer.PopTag();
			_Writer.PushTag( Background );
		}
		operator xml::writer_ &()
		{
			return _Writer;
		}
		xml::writer_ &operator()( void )
		{
			return _Writer;
		}
	};

	class content_rack___
	: public generic_rack___
	{
	public:
		void Init(
			const char *Generator,
			const char *View,
			str::string_ &Target,
			corpus_callback__ &Callback )
		{
			generic_rack___::Init( Generator, View, ContentTagName, Target, Callback );
		}
	};

	class context_rack___
	: public generic_rack___
	{
	public:
		void Init(
			const char *Generator,
			const char *View,
			str::string_ &Target,
			corpus_callback__ &Callback )
		{
			generic_rack___::Init( Generator, View, ContextTagName, Target, Callback );
		}
	};

# define XHTBASE_RACK( Generator, Type )\
	class _##Type##_rack___\
	: public xhtbase::Type##_rack___\
	{\
	public:\
		void Init(\
			const char *View,\
			str::string_ &Target,\
			xhtbase::corpus_callback__ &Callback )\
		{\
			xhtbase::Type##_rack___::Init( Generator, View, Target, Callback );\
		}\
	};

# define XHTBASE_RACKS( Generator )\
	XHTBASE_RACK( Generator, content );\
	XHTBASE_RACK( Generator, context )

	class event_callback__
	{
	public:
		void reset( bso::bool__ = true )
		{
			//Standardisation.
		}
		E_CVDTOR( event_callback__ )
		void Init( void )
		{
			// Standadisation;
		}
	};

}

				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
