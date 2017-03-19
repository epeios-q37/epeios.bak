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

// Socle Java Runtime Environment

#ifndef SCLJRE_INC_
# define SCLJRE_INC_

# define SCLJRE_NAME		"SCLJRE"

# if defined( E_DEBUG ) && !defined( SCLJRE_NODBG )
#  define SCLJRE_DBG
# endif

# include "jniq.h"

# include "err.h"

namespace scljre {
	void Register_( JNIEnv *Env );

	jobject Launch_(
		JNIEnv *Env,
		int Index,
		jobjectArray Args );

	class sArguments
	{
	private:
		jobjectArray Args_;
		mutable jint Index_;
	public:
		void reset( bso::sBool P = true )
		{
			Args_ = NULL;
			Index_ = 0;
		}
		qCDTOR( sArguments );
		void Init( jobjectArray Args )
		{
			Args_ = Args;
			Index_ = 0;
		}
		jobject Get( JNIEnv *Env ) const
		{
			if ( Args_ == NULL )
				qRFwk();

			if ( Index_ >= Env->GetArrayLength( Args_ ) )
				qRFwk();

			return Env->GetObjectArrayElement( Args_, Index_++ );
		}
	};

	typedef jobject (* sFunction_)( JNIEnv *, const sArguments &);

	class sRegistrar
	{
	public:
		void reset( bso::sBool P = true )
		{
		}
		qCDTOR( sRegistrar )
		void Init( void )
		{
		}
		void Register( sFunction_ Function );
		template <typename function, typename ...functions> void Register(
			function Function,
			functions... Functions )
		{
			Register( Function );
			Register( Functions... );
		}
	};

	void SCLJRERegister( sRegistrar &Registrar );	// To overload by user.
	extern const char *SCLNJSProductVersion;	// To define by user.
}

#define SCLJRE_DEF( name )\
	extern "C" JNIEXPORT void JNICALL Java_##name##_register(\
		JNIEnv *Env,\
		jclass)\
	{\
		scljre::Register_( Env );\
	}\
\
	extern "C" JNIEXPORT jobject JNICALL Java_##name##_wrapper(\
		JNIEnv *Env,\
		jclass,\
		jint Index,\
		jobjectArray Args )\
	{\
		return scljre::Launch_( Env, Index, Args );\
	}


#endif
