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

//	$Id: fblfub.h,v 1.8 2013/04/09 17:57:24 csimon Exp $

#ifndef FBLFUB__INC
#define FBLFUB__INC

#define FBLFUB_NAME		"FBLFUB"

#define	FBLFUB_VERSION	"$Revision: 1.8 $"

#define FBLFUB_OWNER		"Claude SIMON"


#if defined( E_DEBUG ) && !defined( FBLFUB_NODBG )
#define FBLFUB_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.8 $
//C Claude SIMON (csimon at zeusw dot org)
//R $Date: 2013/04/09 17:57:24 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D Frontend/Backend Layout Frontend for Universal Backend 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

# error "Obsolete ! Use 'FBLFRD' instead !"

# include "err.h"
# include "flw.h"

# include "fblfrd.h"
# include "fblfeb.h"
# include "fblfrb.h"

namespace fblfub {

	using namespace fblovl;

	using fblfrd::compatibility_informations__;

	using fblfrd::incompatibility_informations_;
	using fblfrd::incompatibility_informations;

	typedef fblfrd::parameters_handling_functions__ _parameters_handling_functions__;


	class universal_parameters_handling_functions__
	: public _parameters_handling_functions__
	{
	private:
		fblfeb::backend_local_access_base__ _Embedded;
		fblfrb::backend_remote_access_base___ _Remote;
		mode__ _Mode;
	protected:
		virtual void FBLFRDPreProcess( void )
		{
			switch ( _Mode ) {
			case mEmbedded:
				_Embedded.PreProcess();
				break;
			case mRemote:
				_Remote.PreProcess();
				break;
			default:
				ERRFwk();
				break;
			}
		}
		virtual void FBLFRDIn(
			fblcst::cast Cast,
			const void *Pointer,
			flw::ioflow__ &Flow )
		{
			switch ( _Mode ) {
			case mEmbedded:
				_Embedded.In( Cast, Pointer, Flow );
				break;
			case mRemote:
				_Remote.In( Cast, Pointer, Flow );
				break;
			default:
				ERRFwk();
				break;
			}
		}
		virtual void FBLFRDOut(
			flw::ioflow__ &Flow,
			fblcst::cast Cast,
			void *Pointer )
		{
			switch ( _Mode ) {
			case mEmbedded:
				_Embedded.Out( Flow, Cast, Pointer );
				break;
			case mRemote:
				_Remote.Out( Flow, Cast, Pointer );
				break;
			default:
				ERRFwk();
				break;
			}
		}
		virtual void FBLFRDPostProcess( flw::ioflow__ &Flow )
		{
			switch ( _Mode ) {
			case mEmbedded:
				_Embedded.PostProcess( Flow );
				break;
			case mRemote:
				_Remote.PostProcess( Flow );
				break;
			default:
				ERRFwk();
				break;
			}
		}
	public:
		void reset( bso::bool__ P = true )
		{
			_Embedded.reset( P );
			_Remote.reset( P );
			_parameters_handling_functions__::reset( P );
			_Mode = m_Undefined;
		}
		universal_parameters_handling_functions__( void )
		{
			reset( false );
		}
		virtual ~universal_parameters_handling_functions__( void )
		{
			reset();
		}
		void Init( mode__ Mode )
		{
			reset();

			switch ( Mode ) {
			case mEmbedded:
				_Embedded.Init();
				break;
			case mRemote:
				_Remote.Init();
				break;
			default:
				ERRFwk();
				break;
			}

			_Mode = Mode;

			_parameters_handling_functions__::Init();
		}
	};

	typedef fblfrd::backend___ _backend___;

	class backend_universal_access___
	: public _backend___
	{
	private:
		universal_parameters_handling_functions__ _Functions;
	public:
		void reset( bso::bool__ P = true )
		{
			_backend___::reset( P );
			_Functions.reset( P );
		}
		backend_universal_access___( void )
		{
			reset( false );
		}
		~backend_universal_access___( void )
		{
			reset();
		}
		bso::bool__ Init(
			const char *Language,
			const compatibility_informations__ &CompatibilityInformations,
			flw::ioflow__ &Flow,
			mode__ Mode,
			fblfrd::reporting_functions__ &ReportingFunctions,
			incompatibility_informations_ &IncompatibilityInformations )
		{
			_Functions.Init( Mode );

			return _backend___::Init( Language, CompatibilityInformations, Flow, _Functions, ReportingFunctions, IncompatibilityInformations  );
		}
	};

}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
