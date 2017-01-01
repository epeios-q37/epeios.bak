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

#define FLX__COMPILATION

#include "flx.h"

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
# ifdef FLX__WIN
qRH
	str::string ModifiedCommand;
	TOL_CBUFFER___ Buffer;
qRB
	// '_popen()' lance en fait "cmd /c ...". Or; lorsque cette commande reoit un paramtre commenant par '"' (hors espaces),
	// et avec plus d'un jeu de '"', elle en enlve certains (void "cmd /?"). Placer 'echo >NUL && ' en tte de paramtre rsoud ce problme...
	ModifiedCommand.Init("echo >NUL && ");
	ModifiedCommand.Append( Command.UTF8( Buffer ) );

	Descriptor = _wpopen( ntvstr::string___( ModifiedCommand ).Internal(), Mode.Internal() );
qRR
qRT
qRE
# elif defined( FLX__POSIX )
	Descriptor = popen( Command.Internal(), Mode.Internal() );
# else
#  error
#endif
	return Descriptor;
}

# ifdef FLX__WIN
static bso::bool__ POpen2_(
	const ntvstr::string___ &ConstCommand,
	HANDLE &In,
	HANDLE &Out,
	HANDLE &Err )
{
	bso::bool__ Success = false;
qRH
	PROCESS_INFORMATION	piProcessInfo;
	SECURITY_ATTRIBUTES sa_attr; 
	HANDLE hChildStdinRd, hChildStdinWr, hChildStdoutRd, hChildStdoutWr, hChildStderrRd, hChildStderrWr;
	ntvstr::string___ Command;
qRB
	// Set the bInheritHandle flag so pipe handles are inherited. 
	sa_attr.nLength              = sizeof( SECURITY_ATTRIBUTES ); 
	sa_attr.bInheritHandle       = TRUE; 
	sa_attr.lpSecurityDescriptor = NULL; 
 
 
	// Create a pipe for the child process's STDERR. 
 	if ( !CreatePipe( &hChildStderrRd, &hChildStderrWr, &sa_attr, 0 ) )
		qRSys();
 
	// Ensure that the read handle to the child process's pipe for STDOUT is not inherited.
	if ( !SetHandleInformation( hChildStderrRd, HANDLE_FLAG_INHERIT, 0 ) )
		qRSys();
 
 
	// Create a pipe for the child process's STDOUT. 
 	if ( !CreatePipe( &hChildStdoutRd, &hChildStdoutWr, &sa_attr, 0 ) )
		qRSys();
		
	// Ensure that the read handle to the child process's pipe for STDOUT is not inherited.
	if ( !SetHandleInformation( hChildStdoutRd, HANDLE_FLAG_INHERIT, 0 ) )
		qRSys();
 
 
	// Create a pipe for the child process's STDIN. 
    if ( !CreatePipe( &hChildStdinRd, &hChildStdinWr, &sa_attr, 0 ) )
		qRSys();

	// Ensure that the write handle to the child process's pipe for STDIN is not inherited. 
    if ( !SetHandleInformation( hChildStdinWr, HANDLE_FLAG_INHERIT, 0 ) )
		qRSys();
 
 
	// Startup information.
	STARTUPINFOW  siStartupInfo;
	ZeroMemory( &siStartupInfo, sizeof(STARTUPINFOW) );
	siStartupInfo.cb         = sizeof( STARTUPINFOW ); 
	siStartupInfo.hStdError  = hChildStderrWr;
	siStartupInfo.hStdOutput = hChildStdoutWr;
	siStartupInfo.hStdInput  = hChildStdinRd;
	siStartupInfo.dwFlags   |= STARTF_USESTDHANDLES;

	Command.Init( ConstCommand );	/* On passe par 'Command', car 'CreateProcessW(...)' est suceptible de modifier le contenu de son second paramtre ;
									   on ne peut donc pas lui passer directement 'ConstCommand', qui est 'const' (voir doc. 'CreateProcessW(...)'. */
 
	// Launch the process.
	if ( !CreateProcessW( NULL,
		Command.ExposedInternal(), 0, 0, TRUE,
		0, 0, 0,
		&siStartupInfo, &piProcessInfo ) )
		qRReturn;

//   WaitForSingleObject( piProcessInfo.hProcess, INFINITE );

	In = hChildStdinWr;
	Out = hChildStdoutRd;
	Err = hChildStderrRd;

	CloseHandle( piProcessInfo.hProcess );
	CloseHandle( piProcessInfo.hThread );

	Success = true;
qRR
qRT
qRE
	return Success;
}

#elif defined( FLX__POSIX )

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
	sdr::byte__ *Buffer )
{
# ifdef FLX__WIN
	DWORD Red = 0;

	if( Amount > MAXDWORD )
		Amount = MAXDWORD;

	if ( !ReadFile( _Out, Buffer, (DWORD)Amount, &Red, NULL ) )
		qRFwk();
#elif defined( FLX__POSIX )
	ssize_t Red = 0;

	if ( ( Red = read( _Out, Buffer, Amount ) ) == -1 )
		qRFwk();
#else
# error
#endif
	return Red;	// Si == 0, 'EOF' atteint.
}

sdr::size__ flx::exec_ioflow_driver___::FDRWrite(
	const sdr::byte__ *Buffer,
	sdr::size__ Amount )
{
# ifdef FLX__WIN
	DWORD Written = 0;

	if( Amount > MAXDWORD )
		Amount = MAXDWORD;

	if ( !WriteFile( _In, Buffer, (DWORD)Amount, &Written, NULL ) )
		qRFwk();

	if ( Written == 0 )	// Ne devrait pas arriver.
		qRFwk();
#elif defined( FLX__POSIX )
	ssize_t Written = 0;


	if ( (Written = write( _In, Buffer, Amount ) ) == -1 )
		qRFwk();
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

Q37_GCTOR( flx )
{
	flx::VoidOFlowDriver.Init( fdr::ts_Default, flx::a_Default );
	flx::VoidOFlow.Init();

	flx::VoidIFlowDriver.Init( fdr::ts_Default, flx::a_Default );
	flx::VoidIFlow.Init();
}

Q37_GDTOR( flx )
{
	// Below 4 lines should not be necessary, but if missing, 'pure virtual function call' problem (at least under VC++10).
	flx::VoidIFlow.reset();
	flx::VoidIFlowDriver.reset();

	flx::VoidOFlow.reset();
	flx::VoidOFlowDriver.reset();
}
