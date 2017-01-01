/*
	Copyright (C) 1999-2017 Claude SIMON (http://q37.info/contact/).

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

//	$Id: xxx.h,v 1.9 2012/11/14 16:06:23 csimon Exp $

#ifndef FRDFRD__INC
#define FRDFRD__INC

#define FRDFRD_NAME		"FRDFRD"

#define	FRDFRD_VERSION	"$Revision: 1.9 $"

#define FRDFRD_OWNER		"Claude SIMON"


#if defined( E_DEBUG ) && !defined( FRDFRD_NODBG )
#define FRDFRD_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.9 $
//C Claude SIMON (csimon at zeusw dot org)
//R $Date: 2012/11/14 16:06:23 $

/* End of automatic documentation generation part. */

/* Addendum to the automatic documentation generation part. */
//D FRontenD FRontenD 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

# include "frdbse.h"

# include "fblfrd.h"

# include "csducl.h"

# include "err.h"
# include "flw.h"

namespace frdfrd {
# if 0
	using fblfrd::compatibility_informations__;

	using fblfrd::incompatibility_informations_;
	using fblfrd::incompatibility_informations;

	typedef fblfrd::universal_frontend___ _frontend___;

	class frontend___
	: public _frontend___
	{
	private:
		csducl::universal_client_ioflow___ _Flow;
	public:
		void reset( bso::bool__ P = true )
		{
			_frontend___::reset( P );
			_Flow.reset( P );
		}
		E_CVDTOR( frontend___ );
		bso::bool__ Init(
			const char *Language,
			const compatibility_informations__ &CompatibilityInformations,
			csducl::universal_client_core &ClientCore,
			fblfrd::reporting_callback__ &ReportingCallback,
			incompatibility_informations_ &IncompatibilityInformations )
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
				qRFwk();
				break;
			}

			return _frontend___::Init( Language, CompatibilityInformations, _Flow, Mode, ReportingCallback, IncompatibilityInformations );
		}
	};
# endif
}

/*$END$*/
#endif
