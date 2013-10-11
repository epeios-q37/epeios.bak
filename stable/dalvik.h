/*
	Header for the 'dalvik' library by Claude SIMON (csimon at zeusw dot org)
	Copyright (C) 20122004 Claude SIMON.

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

//	$Id: dalvik.h,v 1.1 2012/12/04 16:04:27 csimon Exp $

#ifndef DALVIK__INC
#define DALVIK__INC

#define DALVIK_NAME		"DALVIK"

#define	DALVIK_VERSION	"$Revision: 1.1 $"

#define DALVIK_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &DALVIKTutor;

#if defined( E_DEBUG ) && !defined( DALVIK_NODBG )
#define DALVIK_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.1 $
//C Claude SIMON (csimon at zeusw dot org)
//R $Date: 2012/12/04 16:04:27 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D DALVIK 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

# include "err.h"
# include "flw.h"
# include "tol.h"

# include "jni.h"

# include "dvkbse.h"
# include "dvkfev.h"


namespace dalvik {
	extern const char *PackageName;	// A définir par l'utilisateur.

	inline void SetContentView(
		const char *ViewResourceName,
		jobject Activity,
		JNIEnv * Env )
	{
		dvkbse::SetContentView( ViewResourceName, PackageName, Activity, Env );
	}

	inline jint GetRidToken(
		const char *Name,
		JNIEnv *Env )
	{
		return dvkbse::GetRidToken( Name, PackageName, Env );
	}

	inline jint GetRlayoutToken(
		const char *Name,
		JNIEnv *Env )
	{
		return dvkbse::GetRlayoutToken( Name, PackageName, Env );
	}

	template <typename event_handler> void Install(
		event_handler &Handler,
		const char *Name,
		jobject Activity,
		JNIEnv *Env )
	{
		dvkfev::Install( Handler, GetRidToken( Name, Env ), Activity, Env );
	}

	inline jobject GetView(
		const char *Name,
		jobject Activity,
		JNIEnv *Env )
	{
		return dvkbse::GetView( Name, PackageName, Activity, Env );
	}

	inline void SetVisibility(
			jint Id,
			JNIEnv * Env,
			jobject Activity,
			const char *Visibility )
	{
		jobject View = dvkbse::GetView( Id, Activity, Env );

		Env->CallVoidMethod(
			View,
			jvabse::GetMethodID( Env, View, "setVisibility", "(I)V" ),
			jvabse::GetStaticIntField( Env, Env->FindClass( "android/view/View" ), Visibility ) );
	}

	inline void SetVisibility(
			const char *Name,
			JNIEnv * Env,
			jobject Activity,
			const char *Visibility )
	{
		jobject View = dalvik::GetView( Name, Activity, Env );

		Env->CallVoidMethod(
			View,
			jvabse::GetMethodID( Env, View, "setVisibility", "(I)V" ),
			jvabse::GetStaticIntField( Env, Env->FindClass( "android/view/View" ), Visibility ) );
	}

	inline void Hide(
			jint Id,
			JNIEnv * Env,
			jobject Activity )
	{
		SetVisibility( Id, Env, Activity, "GONE" );
	}

	inline void Hide(
			const char *Name,
			JNIEnv * Env,
			jobject Activity )
	{
		SetVisibility( Name, Env, Activity, "GONE" );
	}

	inline void Show(
			jint Id,
			JNIEnv * Env,
			jobject Activity )
	{
		SetVisibility( Id, Env, Activity, "VISIBLE" );
	}

	inline void Show(
			const char *Name,
			JNIEnv * Env,
			jobject Activity )
	{
		SetVisibility( Name, Env, Activity, "VISIBLE" );
	}

	class steering_callback___ {
	protected:
		virtual void DALVIKOnCreate(
			JNIEnv *Env,
			jobject Activity,
			jobject Bundle ) = 0;
	public:
		void reset( bso::bool__ P = true )
		{}
		E_CVDTOR( steering_callback___ )
		void Init( void )
		{}
		void OnCreate(
			JNIEnv *Env,
			jobject Activity,
			jobject Bundle )
		{
			DALVIKOnCreate( Env, Activity, Bundle );
		}
	};
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
