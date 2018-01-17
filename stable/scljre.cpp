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

#define SCLJRE_COMPILATION_

#include "scljre.h"

#include "sclerror.h"
#include "scllocale.h"
#include "sclmisc.h"

#include "str.h"

using namespace scljre;

fdr::sSize scljre::rInputStreamRDriver::FDRRead(
	fdr::sSize Maximum,
	fdr::sByte *Buffer )
{
qRH
	n4jre::rJByteArray Array;
qRB
	if ( Maximum > 5 )
		Maximum = 5;

	Array.Init( (long)Maximum );

	Maximum = Stream_.Read( Array, 0, (long)Maximum );

	if ( Maximum != -1 )
		memcpy( Buffer, Array.Core(), Array.Size() );
	else
		Maximum = 0;
qRR
qRT
	// No need to 'delete' 'Array'.
qRE
	return Maximum;
}

namespace {
	typedef n4all::cLauncher cLauncher_;

	class sLauncher_
	: public cLauncher_ {
	protected:
		virtual void N4ALLCall(
			void *Function,
			n4all::cCaller &RawCaller ) override
		{
			sCaller Caller;
			sJObject Return;

			Caller.Init( RawCaller );

			Return = ( (fFunction *)Function )( Caller );

			RawCaller.SetReturnValue( n4jre::t_Undefined, Return );	// 'sJObject' is a generic type, so no type is specified.
		}
		virtual void N4ALLInfo( str::dString &Info ) override
		{
		qRH
			flx::rStringOFlow BaseFlow;
			txf::sWFlow Flow;
		qRB
			BaseFlow.Init( Info );
			Flow.Init( BaseFlow );

			SCLJREInfo( Flow );
		qRR
		qRT
		qRE
		}
	public:
		void Init( void )
		{}
	};
}

n4jre::fNewObject scljre::NewObject_ = NULL;
n4jre::fNewObjectArray scljre::NewObjectArray_ = NULL;
namespace {
	n4jre::fDelete Delete_ = NULL;
}
n4jre::fThrow scljre::Throw_ = NULL;

n4jre::fMalloc n4jre::N4JREMalloc = NULL;
n4jre::fFree n4jre::N4JREFree = NULL;

namespace {
	template <typename function> void Assign_(
		const function &Source,
		function &Target )
	{
		if ( Source == NULL )
			qRFwk();

		Target = Source;
	}
}

n4all::cLauncher *scln4a::SCLN4ARegister(
	n4all::cRegistrar &RegistrarCallback,
	void *UP )
{
	n4all::cLauncher *Launcher = NULL;
qRH
	scljre::sRegistrar Registrar;
qRB
	const n4jre::gShared &Shared = *( const n4jre::gShared* )UP;

	Assign_( Shared.NewObject, NewObject_ );
	Assign_( Shared.NewObjectArray, NewObjectArray_ );
	Assign_( Shared.Delete, Delete_ );
	Assign_( Shared.Malloc, n4jre::N4JREMalloc );
	Assign_( Shared.Free, n4jre::N4JREFree );
	Assign_( Shared.Throw, Throw_ );

	Registrar.Init( RegistrarCallback );
	SCLJRERegister( Registrar );

	Launcher = new sLauncher_;

	if ( Launcher == NULL )
		qRAlc();
qRR
	if ( Launcher != NULL )
		delete Launcher;
qRT
qRE
	return Launcher;
}

void scljre::Throw( const str::dString &Message )
{
qRH
	qCBUFFERr Buffer;
qRB
	Throw( Message.Convert( Buffer ) );
qRR
qRT
qRE
}

void scljre::rObject::reset( bso::sBool P )
{
	if ( P ) {
		if ( Object_ != NULL )
			Delete_( Object_ );
	}

	Object_ = NULL;
}


sJObject scljre::Null( void )
{
	return NULL;
}

namespace {
	template <typename primitive, typename wrapper> sJObject Wrap_( primitive Value )
	{
		sJObject Object = NULL;
	qRH
		wrapper Wrapper;
	qRB
		Wrapper.Init( Value );

		Object = Wrapper();

		Wrapper.reset( false );
	qRR
	qRT
	qRE
		return Object;
	}
}

sJObject scljre::Boolean( sJBoolean Value )
{
	return Wrap_<sJBoolean, java::lang::rBoolean>( Value );
}

sJObject scljre::Integer( sJInt Value )
{
	return Wrap_<sJInt, java::lang::rInteger>( Value );
}

sJObject scljre::Long( sJLong Value )
{
	return Wrap_<sJLong, java::lang::rLong>( Value );
}

sJObject scljre::String( const str::dString &UTF )
{
	sJObject Object = NULL;
qRH
	rJString Charset;
	rJByteArray Bytes;
	scljre::java::lang::rString JString;
qRB
	Charset.Init( "UTF-8", n4jre::hOriginal );

	Bytes.Init( UTF );

	JString.Init( Bytes, Charset );

	Object = JString();

	JString.reset( false );	// So the underlying object is not destroyed, as it will be used later.
qRR
qRT
qRE
	return Object;
}

sJObject scljre::Strings( const str::dStrings &Strings )
{
	sJObject Buffer = NULL;
qRH;
	rObject Object;
	sdr::sRow Row = qNIL;
qRB;
	if ( Strings.Amount() > n4jre::SizeMax )
		qRLmt();

	Object.Init( NewObjectArray_( (sJSize)Strings.Amount(), "Ljava/lang/String;" ) );

	Row = Strings.First();

	while ( Row != qNIL ) {
		Object.SetElement( (sJSize)*Row, String( Strings( Row ) ) );

		Row = Strings.Next( Row );
	}

	Buffer = Object();

	Object.reset( false );	// So the underlying object is not destroyed, as it will be used later.
qRR;
qRT;
qRE;
	return Buffer;
}

