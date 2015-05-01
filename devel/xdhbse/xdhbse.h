/*
	'xdhbse.h' by Claude SIMON (http://zeusw.org/).

	'xdhbse' is part of the Epeios framework.

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

#ifndef XDHBSE__INC
# define XDHBSE__INC

# define XDHBSE_NAME		"XDHBSE"

# if defined( E_DEBUG ) && !defined( XDHBSE_NODBG )
#  define XDHBSE_DBG
# endif

// X(SL)/DH(TML) BaSE

# include "xdhagt.h"

# include "frdbse.h"

# include "err.h"
# include "tol.h"
# include "xml.h"
# include "flx.h"

namespace xdhbse {

	E_CDEF( char *, ContentTagName, "Content" );
	E_CDEF( char *, ContextTagName, "Context" );

	class corpus_callback__
	{
	protected:
		virtual void XDHBSEDump( xml::writer_ &Writer ) = 0;
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
			XDHBSEDump( Writer );
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

# define XDHBSE_RACK( Generator, Type )\
	class _##Type##_rack___\
	: public xdhbse::Type##_rack___\
	{\
	public:\
		void Init(\
			const char *View,\
			str::string_ &Target,\
			xdhbse::corpus_callback__ &Callback )\
		{\
			xdhbse::Type##_rack___::Init( Generator, View, Target, Callback );\
		}\
	};

# define XDHBSE_RACKS( Generator )\
	XDHBSE_RACK( Generator, content );\
	XDHBSE_RACK( Generator, context )

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

	namespace prolog {
		E_CDEF( char *, ProjectTypeId, "ProjectType" );
		E_CDEF( char *, PredefinedProjectId, "PredefinedProject" );
		E_CDEF( char *, UserProjectId, "UserProject" );

		void GetContent(
			xdhagt::agent___ &Agent,
			xml::writer_ &Writer );

		void GetContext(
			xdhagt::agent___ &Agent,
			xml::writer_ &Writer );

		frdbse::project_type__ GetProjectFeatures(
			xdhagt::agent___ &Agent,
			str::string_ &ProjectFeature );

		void DisplaySelectedProjectFileName(
			xdhagt::agent___ &Agent,
			const char *Id );
		}

	namespace login {
		E_CDEF( char *, BackendTypeId, "BackendType" );
		E_CDEF( char *, DaemonBackendId, "DaemonBackend" );
		E_CDEF( char *, EmbeddedBackendId, "EmbeddedBackend" );
		E_CDEF( char *, PredefinedBackendId, "PredefinedBackend" );

		void GetContent(
			xdhagt::agent___ &Agent,
			xml::writer_ &Writer );

		void GetContext(
			xdhagt::agent___ &Agent,
			xml::writer_ &Writer );

		frdbse::backend_type__ GetBackendFeatures(
			xdhagt::agent___ &Agent,
			str::string_ &Feature );

		void DisplaySelectedEmbeddedBackendFileName(
			xdhagt::agent___ &Agent,
			const char *Id );
	}


}

#endif
