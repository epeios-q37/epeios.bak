/*
	Header for the 'jvastf' library by Claude SIMON (csimon at zeusw dot org)
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

//	$Id: jvastf.h,v 1.7 2013/04/09 17:57:32 csimon Exp $

#ifndef JVASTF__INC
#define JVASTF__INC

#define JVASTF_NAME		"JVASTF"

#define	JVASTF_VERSION	"$Revision: 1.7 $"

#define JVASTF_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &JVASTFTutor;

#if defined( E_DEBUG ) && !defined( JVASTF_NODBG )
#define JVASTF_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.7 $
//C Claude SIMON (csimon at zeusw dot org)
//R $Date: 2013/04/09 17:57:32 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D JaVA STream Flow. 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

# include "err.h"
# include "flw.h"
# include "iof.h"
# include "tol.h"

# include "jvabse.h"

# include <jni.h>

# define JVASTF_BUFFER_SIZE	1000

namespace jvastf {
	using namespace jvabse;

	class _flow_driver_common__
	{
	private:
		jvabse::jni_param__ *_JNIParam;
	protected:
		jbyteArray _JByteArray;
		JNIEnv *_JEnv( void )
		{
			if ( _JNIParam == NULL )
				ERRFwk();

			if ( _JNIParam->Env == NULL )
				ERRFwk();

			return _JNIParam->Env;
		}
		jobject _JObject( void )
		{
			if ( _JNIParam == NULL )
				ERRFwk();

			if ( _JNIParam->Object == NULL )
				ERRFwk();

			return _JNIParam->Object;
		}
	public:
		void reset( bso::bool__ P = true )
		{
			if ( P && false )
				if( _JByteArray != NULL ) 
					_JEnv()->DeleteLocalRef( _JByteArray );

			_JNIParam = NULL;
			_JByteArray = NULL;
		}
		E_CVDTOR( _flow_driver_common__ )
		void Init( jni_param__ &JNIParam )
		{
			reset();

			_JNIParam = &JNIParam;
		}
	};

	typedef fdr::iflow_driver___<> _iflow_driver___;

	class input_stream__iflow_driver___
	: public _flow_driver_common__,
	  public _iflow_driver___
	{
	private:
		fdr::size__ _JRead(
			fdr::size__ Maximum,
			fdr::datum__ *Buffer )
		{
			jobject InputStream = _JObject();

			_JByteArray = _JEnv()->NewByteArray( JVASTF_BUFFER_SIZE );
			jsize Amount = _JEnv()->CallIntMethod( InputStream, GetMethodID( _JEnv(),  InputStream, "read", "([BII)I" ), _JByteArray, (jint)0, (jint)( Maximum > JVASTF_BUFFER_SIZE ? JVASTF_BUFFER_SIZE : Maximum ) );

			if ( Amount == -1 )
				Amount = 0;
			else
				_JEnv()->GetByteArrayRegion( _JByteArray, 0, Amount, (jbyte *)Buffer );

			return Amount;
		}
	protected:
		virtual fdr::size__ FDRRead(
			fdr::size__ Maximum,
			fdr::datum__ *Buffer )
		{
			return _JRead( Maximum, Buffer );
		}
		virtual void FDRDismiss( void )
		{
			// Rien  faire.
		}
	public:
		void reset( bso::bool__ P = true )
		{
			_flow_driver_common__::reset( P );
			_iflow_driver___::reset( P );
		}
		E_CVDTOR( input_stream__iflow_driver___ )
		void Init( jni_param__ &JNIParam )
		{
			reset();

			_iflow_driver___::Init( fdr::ts_Default );

			_flow_driver_common__::Init( JNIParam );
		}
	};

	typedef flw::standalone_iflow__<> _iflow__;

	class input_stream_iflow___
	: public _iflow__
	{
	private:
		input_stream__iflow_driver___ _Driver;
	public:
		void reset( bso::bool__ P = true )
		{
			_Driver.reset( P );
			_iflow__::reset( P );
		}
		E_CVDTOR( input_stream_iflow___ )
		void Init(
			jni_param__ &JNIParam,
			bso::size__ AmountMax = FLW_AMOUNT_MAX )
		{
			_Driver.Init( JNIParam );
			_iflow__::Init( _Driver, AmountMax );
		}
	};


	typedef fdr::oflow_driver___<> _oflow_driver___;

	class output_stream__oflow_driver___
	: public _flow_driver_common__,
	  public _oflow_driver___
	{
	private:
		fdr::size__ _JWrite(
			const fdr::datum__ *Buffer,
			fdr::size__ Amount )
		{
			jobject OutputStream = _JObject();

			if ( Amount > JVASTF_BUFFER_SIZE )
				Amount = JVASTF_BUFFER_SIZE;

			_JByteArray = _JEnv()->NewByteArray( JVASTF_BUFFER_SIZE );

			_JEnv()->SetByteArrayRegion( _JByteArray, 0, (jsize)Amount, (jbyte *)Buffer );

			_JEnv()->CallVoidMethod( OutputStream, GetMethodID( _JEnv(),  OutputStream, "write", "([BII)V" ), _JByteArray, (jint)0, (jint)Amount );

			return Amount;
		}
	protected:
		virtual fdr::size__ FDRWrite(
			const fdr::datum__ *Buffer,
			fdr::size__ Maximum )
		{
			return _JWrite( Buffer, Maximum );
		}
		virtual void FDRCommit( void )
		{
			jobject OutputStream = _JObject();

			_JEnv()->CallVoidMethod( OutputStream, GetMethodID( _JEnv(),  OutputStream, "flush", "()V" ) );
		}
	public:
		void reset( bso::bool__ P = true )
		{
			_flow_driver_common__::reset( P );
			_oflow_driver___::reset( P );
		}
		E_CVDTOR( output_stream__oflow_driver___ )
		void Init( jni_param__ &JNIParam )
		{
			reset();

			_oflow_driver___::Init( fdr::ts_Default );

			_flow_driver_common__::Init( JNIParam );
		}
	};

	typedef flw::standalone_oflow__<> _oflow__;

	class output_stream_oflow___
	: public _oflow__
	{
	private:
		output_stream__oflow_driver___ _Driver;
	public:
		void reset( bso::bool__ P = true )
		{
			_Driver.reset( P );
			_oflow__::reset( P );
		}
		E_CVDTOR( output_stream_oflow___ )
		void Init(
			jni_param__ &JNIParam,
			bso::size__ AmountMax = FLW_AMOUNT_MAX )
		{
			_Driver.Init( JNIParam );
			_oflow__::Init( _Driver, AmountMax );
		}
	};

}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
