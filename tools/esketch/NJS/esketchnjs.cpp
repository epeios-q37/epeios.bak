// hello.cc
#include <node.h>

#include "cio.h"
#include "scln.h"
#include "v8qnjs.h"

namespace {
	void Test0_( const v8q::sArguments &Arguments )
	{
		v8qnjs::sRStream Stream;
		v8q::sFunction Test;

		tol::Init( Stream, Test );
		Arguments.Get( Stream, Test );
  
		Stream.Launch( "on", v8q::sString( "data" ), Test );
	}

	void Test1_( const v8q::sArguments &Arguments )
	{
		v8qnjs::sRStream Stream;
		v8q::sFunction Test;

		tol::Init( Stream, Test );
		Arguments.Get( Stream, Test );
  
		Stream.OnData( Test );
	}

	void Test2_( const v8q::sArguments &Arguments )
	{
		v8qnjs::sRStream Stream;

		Stream.Init();
		Arguments.Get( Stream );
  
		v8q::sFunction Function;
		Function.Init( v8q::Execute( "function display( chunk ) { process.stdout.write( chunk.toString().toUpperCase() ); } function getFunc() { return display; } try { getFunc(); } catch(err) { console.error( err.message );}" ) );
		Stream.OnData( Function );
	}

	namespace {
		void DisplayChunk_( const v8q::sFunctionInfos &Infos )
		{
		qRFH
			v8qnjs::sBuffer Buffer;
		qRFB
			v8q::Get( Infos, Buffer );

			cio::COut << Buffer;
		qRFR
		qRFT
		qRFE( scln::ErrFinal() )
		}
	}
	
	void Test3_( const v8q::sArguments &Arguments )
	{
		v8qnjs::sRStream Stream;

		Stream.Init();
		Arguments.Get( Stream );
  
		v8q::sFunction Function;
		Function.Init( DisplayChunk_ );
		Stream.OnData( Function );
	}
}

void scln::SCLNRegister( scln::sRegistrar &Registrar )
{
	cio::Initialize(cio::GetConsoleSet() );
	Registrar.Register( Test0_ );
	Registrar.Register( Test1_ );
	Registrar.Register( Test2_ );
	Registrar.Register( Test3_ );
}

SCLN_MODULE( xppq );

