/*
	'sclfrntnd.h' by Claude SIMON (http://zeusw.org/).

	'sclfrntnd' is part of the Epeios framework.

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

#ifndef SCLFRNTND__INC
# define SCLFRNTND__INC

# define SCLFRNTND_NAME		"SCLFRNTND"

# if defined( E_DEBUG ) && !defined( SCLFRNTND_NODBG )
#  define SCLFRNTND_DBG
# endif

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

// SoCLe FRoNTeND

# include "sclrgstry.h"

# include "frdbse.h"

# include "fblfrd.h"

# include "csducl.h"

# include "err.h"
# include "flw.h"
# include "xml.h"

namespace sclfrntnd {
	using csducl::features___;

	class kernel___
	{
	private:
		csducl::universal_client_core _ClientCore;
	public:
		void reset( bso::bool__ P = true )
		{
			_ClientCore.reset( P );
		}
		E_CVDTOR( kernel___ );
		bso::bool__ Init(
			const features___ &Features,
			csdsnc::log_callback__ *LogCallback = NULL );
		bso::bool__ Init(
			const features___ &Features,
			csdsnc::log_callback__ &LogCallback )
		{
			return Init( Features, &LogCallback );
		}
		csducl::universal_client_core &Core( void )
		{
			return _ClientCore;
		}
	};

	using fblfrd::compatibility_informations__;

	using fblfrd::incompatibility_informations_;
	using fblfrd::incompatibility_informations;

	typedef fblfrd::universal_frontend___ _frontend___;

	class frontend___
	: public _frontend___
	{
	private:
		csducl::universal_client_ioflow___ _Flow;
		rgstry::multi_level_registry _Registry;
		rgstry::level__ _RegistryLevel;
		bso::bool__ _IsOpen;
		Q37_MPMDF( const char, _L, _Language );
	public:
		void reset( bso::bool__ P = true )
		{
			_frontend___::reset( P );
			_Flow.reset( P );
			_Registry.reset( P );
			_IsOpen = false;
			_RegistryLevel = rgstry::UndefinedLevel;
			_Language = NULL;
		}
		E_CVDTOR( frontend___ );
		void Init(
			const char *Language,
			fblfrd::reporting_callback__ &ReportingCallback,
			const rgstry::multi_level_registry_ &Registry )
		{
			// _Flow.Init(...);	// Made on connection.
			_Registry.Init();
			_Registry.Push( Registry );
			_RegistryLevel = _Registry.CreateEmbedded( rgstry::name( "Session" ) );
			_IsOpen = false;
			_Language = Language;


			return _frontend___::Init( ReportingCallback );
		}
		bso::bool__ Connect(
			csducl::universal_client_core &ClientCore,
			const fblfrd::compatibility_informations__ &CompatibilityInformations,
			fblfrd::incompatibility_informations_ &IncompatibilityInformations )
		{
			fblfrd::mode__ Mode = fblfrd::m_Undefined;

			_Flow.Init( ClientCore );

			switch ( ClientCore.GetType() ) {
			case csducl::tPlugin:
				Mode = fblfrd::mEmbedded;
				break;
			case csducl::tDaemon:
				Mode = fblfrd::mRemote;
				break;
			default:
				ERRFwk();
				break;
			}

			return _frontend___::Connect( _L(), _Flow, Mode, CompatibilityInformations, IncompatibilityInformations );
		}
		const rgstry::multi_level_registry_ &Registry( void ) const
		{
			return _Registry;
		}
		const char *Language( void ) const
		{
			return _L();
		}
	};




	// Action  effectuer sur le projet par dfaut.
	enum action__ {
		aNone,		// No action : another project can be choosen, and the selected project is manually loaded and launched.
		aLoad,		// Project is loaded, but not launched.
		aLaunch,	// Project is loaded and launched.
		a_amount,
		a_Undefined
	};

	const char *GetLabel( action__ Action );

	action__ GetAction( const str::string_ &Label );

	action__ GetProjectsFeatures(
		const char *Language,
		xml::writer_ &Writer );

	void GetBackendsFeatures(
		const char *Language,
		xml::writer_ &Writer );

	bso::uint__ GetBackendPingDelay( void );

	void LoadProject(
		frdbse::project_type__ ProjectType,
		const str::string_ &ProjectFeature );

	void LaunchProject(
		kernel___ &Kernel,
		frdbse::backend_type__ BackendType,
		const str::string_ &BackendFeature );

}

				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
