/*
	'trunk.h' by Claude SIMON (http://q37.info/contact/).

	This file is part of 'eSketch' software.

    'eSketch' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'eSketch' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'eSketch'.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef TRUNK_INC_
# define TRUNK_INC_

# include "sktinf.h"

# include "frdkernl.h"

# include "sclrgstry.h"

# include "ui.h"

# define NAME					SKTINF_LC_AFFIX	"dvk"
# define DESCRIPTION			"Dalvik component for '" SKTINF_MC_AFFIX "' software"
# define PROJECT_AFFILIATION	SKTINF_PROJECT_AFFILIATION
# define SOFTWARE_AFFILIATION	SKTINF_SOFTWARE_AFFILIATION
# define AUTHOR_NAME			SKTINF_AUTHOR_NAME
# define AUTHOR_CONTACT			SKTINF_AUTHOR_CONTACT
# define HELP					EPSMSC_HELP_INVITATION( NAME )
# define WEBSITE_URL			SKTINF_WEBSITE_URL

# ifdef XXX_DBG
#  define DEBUG_FLAG	" (D)"
# else
#  define DEBUG_FLAG	""
# endif

namespace trunk {
	class trunk___
	{
	public:
		ui::ui__ UI;
		void reset( bso::bool__ P = true )
		{
			UI.reset( P );
		}
		E_CDTOR( trunk___ );
		void Init(
			JNIEnv *Env,
			jobject Activity )
		{
			LOC
			UI.Init( *this, Env );
			LOC
			UI.Show( ui_base::pHome, Env, Activity );
			LOC
		}
	};
}

#endif