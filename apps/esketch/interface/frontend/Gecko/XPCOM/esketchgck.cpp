/*
	'esketchgck.cpp' by Claude SIMON (http://zeusw.org/).

	 This file is part of 'eSketch' software.

    'eSketch' is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'eSketch' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with 'eSketch'.  If not, see <http://www.gnu.org/licenses/>.
*/

// $Id: esketchgck.cpp,v 1.3 2013/04/05 16:33:22 csimon Exp $

#include "trunk.h"
#include "ui.h"

#include "sclgecko.h"
#include "sclrgstry.h"
#include "scllocale.h"

const char *sclmisc::SCLMISCTargetName = PROGRAM_NAME;

typedef xulfui::steering_callback__ _steering_callback__;

class callback__
: public _steering_callback__
{
private:
	trunk::trunk___ _Trunk;
	frdkernl::kernel___ _Kernel;
	trunk::user_callback__ _UserCallback;
protected:
	bso::bool__ XULFUIRegister(
		nsIDOMWindow *Window,
		const str::string_ &Id )
	{
		if ( Id == UI_MAIN_WINDOW_ID ) {
			_Trunk.UI().Main.Register( Window );
			_Trunk.FinalizeLaunching();
		} else if ( Id == UI_SSN_FRM_PAGE_ID )
			_Trunk.UI().SessionForm.Register( Window );
		else if ( Id == UI_SSN_VEW_PAGE_ID )
			_Trunk.UI().SessionView.Register( Window );
		else if ( Id == XULFDG_WINDOW_ID )
			_Trunk.UI().DebugDialog().Register( Window );
		else
			return false;

		return true;
	}
public:
	void reset( bso::bool__ P = true )
	{
		_Kernel.reset( P );
		_Trunk.reset( P );
		_steering_callback__::reset( P );
		_UserCallback.reset( P );
	}
	E_CDTOR(callback__ )
	xulftk::status__ Init(
		const char *LauncherIdentification,
		sclrgstry::registry_ &Registry,
		const lcl::locale_ &Locale,
		const char *Language )
	{
		xulftk::status__ Status = _Kernel.Init(  Registry, Locale, Language, _Trunk.DefaultReportingFunctions() );
		_Trunk.Init( _UserCallback, _Kernel,  Locale, Language, *this );
		_steering_callback__::Init( _Trunk );
		_UserCallback.Init( LauncherIdentification,  _Trunk );

		return Status;

	}
};

xulfui::steering_callback__ *sclgecko::SCLGECKOCreateSteering(
	const char *LauncherIdentification,
	const lcl::locale_ &Locale,
	const char *Language,
	void *UP )
{
	callback__ *Callback = NULL;
ERRProlog
ERRBegin
	if ( ( Callback = new callback__ ) == NULL )
		ERRAlc();

	Callback->Init( LauncherIdentification, sclrgstry::GetRegistry(), Locale, Language );
ERRErr
	if ( Callback != NULL )
		delete Callback;

	Callback = NULL;

	ERRRst();	// Error catched to avoid that it goes further.
				// Error reported by the fact that the returned value is 'NULL'.
ERREnd
ERREpilog
	return Callback;
}
