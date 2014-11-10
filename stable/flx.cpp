/*
	'flx' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Requires the 'flx' header file ('flx.h').
	Copyright (C) 2000-2004 Claude SIMON (http://zeusw.org/intl/contact.html).

	This file is part of the Epeios (http://zeusw.org/epeios/) project.

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 3
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



//	$Id: flx.cpp,v 1.62 2013/04/05 16:33:35 csimon Exp $

#define FLX__COMPILATION

#include "flx.h"

class flxtutor
: public ttr_tutor
{
public:
	flxtutor( void )
	: ttr_tutor( FLX_NAME )
	{
#ifdef FLX_DBG
		Version = FLX_VERSION "\b\bD $";
#else
		Version = FLX_VERSION;
#endif
		Owner = FLX_OWNER;
		Date = "$Date: 2013/04/05 16:33:35 $";
	}
	virtual ~flxtutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

#include "str.h"

flx::void_oflow_driver___ flx::VoidOFlowDriver;
flx::void_oflow__ flx::VoidOFlow;

flx::void_iflow_driver___ flx::VoidIFlowDriver;
flx::void_iflow__ flx::VoidIFlow;

cslio::descriptor__ flx::_POpen(
	const ntvstr::string___ &Command,
	const ntvstr::string___ &Mode )
{
	cslio::descriptor__ Descriptor = cslio::UndefinedDescriptor;
# ifdef CPE_WIN
ERRProlog
	str::string ModifiedCommand;
	TOL_CBUFFER___ Buffer;
ERRBegin
	// '_popen()' lance en fait "cmd /c ...". Or; lorsque cette commande reçoit un paramètre commençant par '"' (hors espaces),
	// et avec plus d'un jeu de '"', elle en enlève certains (void "cmd /?"). Placer 'echo >NUL && ' en tête de paramètre résoud ce problème...
	ModifiedCommand.Init("echo >NUL && ");
	ModifiedCommand.Append( Command.UTF8( Buffer ) );

	Descriptor = _wpopen( ntvstr::string___( ModifiedCommand ).Internal(), Mode.Internal() );
ERRErr
ERREnd
ERREpilog
# elif defined( CPE_POSIX )
	Descriptor = popen( Command.Internal(), Mode.Internal() );
# else
#  error
#endif
	return Descriptor;
}

# ifdef CPE_WIN
static bso::bool__ POpen2_(
	const ntvstr::string___ &ConstCommand,
	HANDLE &In,
	HANDLE &Out,
	HANDLE &Err )
{
	bso::bool__ Success = false;
ERRProlog
	PROCESS_INFORMATION	piProcessInfo;
	SECURITY_ATTRIBUTES sa_attr; 
	HANDLE hChildStdinRd, hChildStdinWr, hChildStdoutRd, hChildStdoutWr, hChildStderrRd, hChildStderrWr;
	ntvstr::string___ Command;
ERRBegin
	// Set the bInheritHandle flag so pipe handles are inherited. 
	sa_attr.nLength              = sizeof( SECURITY_ATTRIBUTES ); 
	sa_attr.bInheritHandle       = TRUE; 
	sa_attr.lpSecurityDescriptor = NULL; 
 
 
	// Create a pipe for the child process's STDERR. 
 	if ( !CreatePipe( &hChildStderrRd, &hChildStderrWr, &sa_attr, 0 ) )
		ERRSys();
 
	// Ensure that the read handle to the child process's pipe for STDOUT is not inherited.
	if ( !SetHandleInformation( hChildStderrRd, HANDLE_FLAG_INHERIT, 0 ) )
		ERRSys();
 
 
	// Create a pipe for the child process's STDOUT. 
 	if ( !CreatePipe( &hChildStdoutRd, &hChildStdoutWr, &sa_attr, 0 ) )
		ERRSys();
		
	// Ensure that the read handle to the child process's pipe for STDOUT is not inherited.
	if ( !SetHandleInformation( hChildStdoutRd, HANDLE_FLAG_INHERIT, 0 ) )
		ERRSys();
 
 
	// Create a pipe for the child process's STDIN. 
    if ( !CreatePipe( &hChildStdinRd, &hChildStdinWr, &sa_attr, 0 ) )
		ERRSys();

	// Ensure that the write handle to the child process's pipe for STDIN is not inherited. 
    if ( !SetHandleInformation( hChildStdinWr, HANDLE_FLAG_INHERIT, 0 ) )
		ERRSys();
 
 
	// Startup information.
	STARTUPINFOW  siStartupInfo;
	ZeroMemory( &siStartupInfo, sizeof(STARTUPINFOW) );
	siStartupInfo.cb         = sizeof( STARTUPINFOW ); 
	siStartupInfo.hStdError  = hChildStderrWr;
	siStartupInfo.hStdOutput = hChildStdoutWr;
	siStartupInfo.hStdInput  = hChildStdinRd;
	siStartupInfo.dwFlags   |= STARTF_USESTDHANDLES;

	Command.Init( ConstCommand );	/* On passe par 'Command', car 'CreateProcessW(...)' est suceptible de modifier le contenu de son second paramètre ;
									   on ne peut donc pas lui passer directement 'ConstCommand', qui est 'const' (voir doc. 'CreateProcessW(...)'. */
 
	// Launch the process.
	if ( !CreateProcessW( NULL,
		Command.ExposedInternal(), 0, 0, TRUE,
		0, 0, 0,
		&siStartupInfo, &piProcessInfo ) )
		ERRReturn;

//   WaitForSingleObject( piProcessInfo.hProcess, INFINITE );

	In = hChildStdinWr;
	Out = hChildStdoutRd;
	Err = hChildStderrRd;

	CloseHandle( piProcessInfo.hProcess );
	CloseHandle( piProcessInfo.hThread );

	Success = true;
ERRErr
ERREnd
ERREpilog
	return Success;
}

#elif defined( CPE_POSIX )

static bso::bool__ POpen2_(
	const ntvstr::string___ &Command,
	int &In,
	int &Out,
	int &Err )
{
    pid_t p;
    int pipe_stdin[2], pipe_stdout[2], pipe_stderr[2];

    if(pipe(pipe_stdin)) return false;
    if(pipe(pipe_stdout)) return false;
    if(pipe(pipe_stderr)) return false;

    p = fork();
    if(p < 0) return false; /* Fork failed */

    if(p == 0) { /* child */

        close(pipe_stdin[1]);
        dup2(pipe_stdin[0], 0);
        close(pipe_stdout[0]);
        dup2(pipe_stdout[1], 1);
        close(pipe_stderr[0]);
        dup2(pipe_stderr[1], 2);
        execl("/bin/sh", "sh", "-c", (const char *)Command.Internal(), (const char *)NULL);

        perror("execl"); exit(99);
    }

    In = pipe_stdin[1];
    Out = pipe_stdout[0];
    Err = pipe_stderr[0];
    return true; 
}
#else
# error
#endif

sdr::size__ flx::exec_ioflow_driver___::FDRRead(
	sdr::size__ Amount,
	sdr::datum__ *Buffer )
{
# ifdef CPE_WIN
	DWORD Red = 0;

	if( Amount > MAXDWORD )
		Amount = MAXDWORD;

	if ( !ReadFile( _Out, Buffer, (DWORD)Amount, &Red, NULL ) )
		ERRDta();
#elif defined( CPE_POSIX )
	ssize_t Red = 0;

	if ( ( Red = read( _Out, Buffer, Amount ) ) == -1 )
		ERRDta();
#else
# error
#endif
	return Red;	// Si == 0, 'EOF' atteint.
}

sdr::size__ flx::exec_ioflow_driver___::FDRWrite(
	const sdr::datum__ *Buffer,
	sdr::size__ Amount )
{
# ifdef CPE_WIN
	DWORD Written = 0;

	if( Amount > MAXDWORD )
		Amount = MAXDWORD;

	if ( !WriteFile( _In, Buffer, (DWORD)Amount, &Written, NULL ) )
		ERRDta();

	if ( Written == 0 )	// Ne devrait pas arriver.
		ERRDta();
#elif defined( CPE_POSIX )
	ssize_t Written = 0;


	if ( (Written = write( _In, Buffer, Amount ) ) == -1 )
		ERRDta();
#else
# error
#endif

	return Written;
}

bso::bool__ flx::exec_ioflow_driver___::Init(
	const ntvstr::string___ &Command,
	fdr::thread_safety__ ThreadSafety )
{
	reset();

	_ioflow_driver___::Init( ThreadSafety );

	return POpen2_( Command, _In, _Out, _Err );
}



/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */
class flxpersonnalization
: public flxtutor
{
public:
	flxpersonnalization( void )
	{
		flx::VoidOFlowDriver.Init( fdr::ts_Default );
		flx::VoidOFlow.Init();

		flx::VoidIFlowDriver.Init( fdr::ts_Default );
		flx::VoidIFlow.Init();

		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~flxpersonnalization( void )
	{
		// Les 4 lignes ci-dessous ne devraient pas être nécessaire, mais si absentes, problême de 'pure virtual function call' (du moins sous VC++10).
		flx::VoidIFlow.reset();
		flx::VoidIFlowDriver.reset();

		flx::VoidOFlow.reset();
		flx::VoidOFlowDriver.reset();


		/* place here the actions concerning this library
		to be realized at the ending of the application  */
	}
};


/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

// 'static' by GNU C++.

static flxpersonnalization Tutor;

ttr_tutor &FLXTutor = Tutor;
