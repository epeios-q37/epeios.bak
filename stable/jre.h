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

// Java Runtime Environment

#ifndef JRE_INC_
# define JRE_INC_

# define JRE_NAME		"JRE"

# if defined( E_DEBUG ) && !defined( JRE_NODBG )
#  define JRE_DBG
# endif

# include "jniobj.h"

# include "err.h"
# include "fdr.h"

namespace jre {

	typedef jniobj::java::lang::sString sString_;

	class rString
	: public sString_
	{
	public:
		void reset( bso::sBool P = true )
		{
			sString_::reset( P );
		}
		qCDTOR( rString );
		void Init(
			JNIEnv *Env, 
			const char *Text )
		{
			sString::Init(Env, Env->NewStringUTF( Text ), true );
		}
	};

	typedef fdr::rIDressedDriver rIDriver_;

	class rInputStreamIDriver
	: public rIDriver_
	{
	private:
		jniobj::java::io::sInputStream Stream_;
		qPMV( JNIEnv, E_, Env_ );
	protected:
		virtual fdr::sSize FDRRead(
			fdr::sSize Maximum,
			fdr::sByte *Buffer ) override;
		virtual void FDRDismiss( bso::sBool Unlock ) override
		{}
		virtual fdr::sTID FDRITake( fdr::sTID Owner ) override
		{
			return Owner;
		}
	public:
		void reset( bso::sBool P = true )
		{
			rIDriver_::reset( P );
			tol::reset( P, Stream_ );
			Env_ = NULL;
		}
		qCVDTOR( rInputStreamIDriver );
		void Init(
			JNIEnv *Env,
			jobject Stream )
		{
			rIDriver_::Init( fdr::ts_Default );
			Stream_.Init( Env, Stream );
			Env_ = Env;
		}
	};
}

#endif
