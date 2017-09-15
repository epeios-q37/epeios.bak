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

// Native 4(for) Java Runtime Environment
// Extension of the 'N4ALL' library for use with Java.
// Common part between wrapper and component ('scln4a').

#ifndef N4JRE_INC_
# define N4JRE_INC_

# define N4JRE_NAME		"N4JRE"

# if defined( E_DEBUG ) && !defined( N4JRE_NODBG )
#  define N4JRE_DBG
# endif

# include "n4all.h"
# include "err.h"
# include "tol.h"

# ifdef H
#  define N4JRE_H_ H
#  undef H
# endif

namespace n4jre {
	typedef long jint;
	typedef __int64 jlong;
	typedef signed char jbyte;
	typedef bso::sByte *jbyteArray;
	class jobject_ {};
	typedef jobject_ *jobject;
	class jstring_
	: public jobject_
	{};
	typedef jstring_ *jstring;

	struct gShared {
	public:
		void reset( bso::sBool P = true )
		{
		}
		qCDTOR( gShared );
	};

	qENUM( Type_ )
	{
		tByteArray,
		tString,
		t_amount,
		t_Undefined
	};

	struct sValue_ {
	public:
		eType_ Type;
		void *Content;
		void reset( bso::sBool = true )
		{
			Type = t_Undefined;
			Content = NULL;
		}
		qCDTOR( sValue_ );
		template <typename content> void Init(
			eType_ Type = t_Undefined,
			content *Content = NULL )
		{
			this->Type = Type;
			this->Content = (void *)Content;
		}
	};

	class object {};

	class cObject {
	protected:
		virtual void N4JREInit( const sValue_ *Values ) = 0;
		virtual void N4JRESet(
			const char *Name,
			const char *Signature,
			jobject Value ) = 0;
# define H( type, name )\
	protected:\
		virtual type N4JRECall##name##Method(\
			const char *Method,\
			const char *Signature,\
			const sValue_ *Values ) const = 0;\
	public:\
		type Call##name##Method(\
			const char *Method,\
			const char *Signature,\
			const sValue_ *Values )\
		{\
			return N4JRECall##name##Method( Method, Signature, Values );\
		}
		H( void, Void );
		H( jint, Int );
		H( jlong, Long );
		H( jobject, Object );
# undef H
	public:
		void Init( const sValue_ *Values )
		{
			return N4JREInit( Values );
		}
		void Set(
			const char *Name,
			const char *Signature,
			jobject Value )
		{
			return N4JRESet( Name, Signature, Value );
		}
	};
}

# ifdef N4JRE_H_
#  define H N4JRE_H_
# endif

#endif
