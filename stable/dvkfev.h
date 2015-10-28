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

//	$Id: dvkfev.h,v 1.1 2012/12/04 16:06:37 csimon Exp $

#ifndef DVKFEV__INC
#define DVKFEV__INC

#define DVKFEV_NAME		"DVKFEV"

#define	DVKFEV_VERSION	"$Revision: 1.1 $"

#define DVKFEV_OWNER		"Claude SIMON"

#if defined( E_DEBUG ) && !defined( DVKFEV_NODBG )
#define DVKFEV_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.1 $
//C Claude SIMON (csimon at zeusw dot org)
//R $Date: 2012/12/04 16:06:37 $

/* End of automatic documentation generation part. */

/* Addendum to the automatic documentation generation part. */
//D DalViK Frontend EVent 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

# include <stdarg.h>

# include "err.h"
# include "flw.h"
# include "tol.h"
# include "str.h"

# include "jni.h"

# include "dvkbse.h"

namespace dvkfev {
	class event_handler_common__
	{
	public:
		void reset( bso::bool__ P = true )
		{}
		E_CVDTOR( event_handler_common__ )
		void Init( void )
		{}
	};


	class on_click_event_handler__
	: public event_handler_common__
	{
	protected:
		virtual void DVKEVTOnClick(
			JNIEnv * Env,
			jobject Activity,
			jobject View ) = 0;
	public:
		void OnClick(
			JNIEnv * Env,
			jobject Activity,
			jobject View )
		{
			DVKEVTOnClick( Env, Activity, View );
		}
		void VAOnClick(
			JNIEnv * Env,
			jobject Activity,
			jobject View,
			va_list )
		{
			OnClick( Env, Activity, View );
		}
	};

	class on_item_selected_event_handler__
	: public event_handler_common__
	{
	protected:
		virtual void DVKEVTOnItemSelected(
			JNIEnv * Env,
			jobject Activity,
			jobject View,
			bso::uint__ Id ) = 0;
		virtual void DVKEVTOnNothingSelected(
			JNIEnv * Env,
			jobject Activity,
			jobject View ) = 0;
	public:
		void OnItemSelected(
			JNIEnv * Env,
			jobject Activity,
			jobject View,
			bso::uint__ Id )
		{
			DVKEVTOnItemSelected( Env, Activity, View, Id );
		}
		void VAOnItemSelected(
			JNIEnv * Env,
			jobject Activity,
			jobject View,
			va_list VAList )
		{
			jlong Id = va_arg( VAList, jlong );

			OnItemSelected( Env, Activity, View, Id );
		}
		void OnNothingSelected(
			JNIEnv * Env,
			jobject Activity,
			jobject View )
		{
			DVKEVTOnNothingSelected( Env, Activity, View);
		}
		void VAOnNothingSelected(
			JNIEnv * Env,
			jobject Activity,
			jobject View,
			va_list )
		{
			OnNothingSelected( Env, Activity, View );
		}
	};

	void Install(
		on_click_event_handler__ &Handler,
		jobject View,
		jobject Activity,
		JNIEnv *Env );

	void Install(
		on_item_selected_event_handler__ &Handler,
		jobject View,
		jobject Activity,
		JNIEnv *Env );

	template <typename event_handler> inline void Install(
		event_handler &Handler,
		jint Id,
		jobject Activity,
		JNIEnv *Env )
	{
		Install( Handler, dvkbse::GetView( Id, Activity, Env ), Activity, Env );
	}

}

/*$END$*/
#endif
