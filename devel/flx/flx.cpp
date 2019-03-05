/*
	Copyright (C) 1999 Claude SIMON (http://q37.info/contact/).

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

#include "mtk.h"
#include "str.h"

# ifdef FLX__WIN
	E_CDEF(char *, flx::_ReadMode, "rb" );
	E_CDEF(char *, flx::_WriteMode, "wb" );
# elif defined( FLX__POSIX )
	E_CDEF(char *, flx::_ReadMode, "r" );
	E_CDEF(char *, flx::_WriteMode, "w" );
# else
#  error
#endif


flx::void_odriver___ flx::VoidODriver;
flx::void_odriver___ flx::PlaceholderODriver;
flx::void_oflow__ flx::VoidOFlow;
flx::void_oflow__ flx::PlaceholderOFlow;

flx::void_idriver___ flx::VoidIDriver;
flx::void_idriver___ flx::PlaceholderIDriver;
flx::void_iflow__ flx::VoidIFlow;
flx::void_iflow__ flx::PlaceholderIFlow;

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
	if ( CreateProcessW( NULL,
						 Command.ExposedInternal(), 0, 0, TRUE,
						 0, 0, 0,
						 &siStartupInfo, &piProcessInfo ) ) {

		//   WaitForSingleObject( piProcessInfo.hProcess, INFINITE );

		In = hChildStdinWr;
		Out = hChildStdoutRd;
		Err = hChildStderrRd;

		CloseHandle( piProcessInfo.hProcess );
		CloseHandle( piProcessInfo.hThread );

		Success = true;
	}
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

const str::dString &flx::GetString(
	fdr::rRDriver &Driver,
	str::dString &String )
{
qRH
	flx::rStringODriver SDriver;
qRB
	SDriver.Init( String, fdr::ts_Default );

	fdr::Copy( Driver, SDriver );
qRR
qRT
qRE
	return String;
}

fdr::sSize flx::rRWMonitor::FDRRead(
	fdr::sSize Maximum,
	fdr::sByte *Buffer )
{
qRH
	tht::rLockerHandler Locker;
qRB
	Locker.Init( Locker_ );

	Maximum = ID_().Read( Maximum, Buffer, fdr::bNonBlocking );

	if ( IsIn_() ) {
		if ( Maximum != 0 ) {
			if ( !Undismissed_ ) {
				if ( M_().In.Before != NULL )
					FI_() << M_().In.Before;
				if ( !M_().Async )
					Undismissed_ = true;
			}

			FI_().Flow().Write( Buffer, Maximum );

			if ( M_().Async ) {
				if ( M_().In.After != NULL )
					FI_() << M_().In.After;
				FI_().Commit();
			}
		}
	}
qRR
qRT
qRE
	return Maximum;
}

bso::sBool flx::rRWMonitor::FDRDismiss(
	bso::sBool Unlock,
	qRPN )
{
	if ( Undismissed_ ) {
		if ( M_().In.After != NULL )
			FI_() << M_().In.After;
		FI_().Commit();
	}

	Undismissed_ = false;

	return ID_().Dismiss( Unlock, ErrHandling );
}

fdr::sTID flx::rRWMonitor::FDRRTake( fdr::sTID Owner )
{
	return ID_().RTake( Owner );
}

fdr::sSize flx::rRWMonitor::FDRWrite(
	const fdr::sByte *Buffer,
	fdr::sSize Maximum )
{
	if ( IsOut_() ) {
		if ( !Uncommited_ ) {
			if ( M_().Out.Before != NULL )
				FO_() << M_().Out.Before;
			if ( !M_().Async )
				Uncommited_ = true;
		}
	}

	Maximum = OD_().Write( Buffer, Maximum );

	if ( IsOut_() && ( Maximum != 0 ) ) {
		FO_().Flow().Write( Buffer, Maximum );
		if ( M_().Async ) {
			if ( M_().Out.After != NULL )
				FO_() << M_().Out.After;
			FO_().Commit();
		}
	}

	return Maximum;
}

bso::sBool flx::rRWMonitor::FDRCommit(
	bso::sBool Unlock,
	qRPN )
{
	if ( IsOut_() ) {
		if ( Uncommited_ ) {
			if ( M_().Out.After != NULL )
				FO_() << M_().Out.After;
			FO_().Commit();
		}
	}

	Uncommited_ = false;

	return OD_().Commit( Unlock, ErrHandling );
}

fdr::sTID flx::rRWMonitor::FDRWTake( fdr::sTID Owner )
{
	return OD_().WTake( Owner );
}

namespace async_ {
	struct rData {
		fdr::rRDriver *IDriver;
		fdr::rWDriver *ODriver;
		void reset( bso::sBool P = true )
		{
			tol::reset( P, IDriver, ODriver );
		}
		qCDTOR( rData );
	};

	void Routine(
		void *UP,
		mtk::gBlocker &Blocker )
	{
	qRH
		flw::rDressedRFlow<> IFlow;
		flw::rDressedWFlow<> OFlow;
		fdr::sByte Buffer[100];
		rData &Data = *(rData *)UP;
	qRB
		IFlow.Init( *Data.IDriver );
		OFlow.Init( *Data.ODriver );

		Blocker.Release();

		while ( !IFlow.EndOfFlow() )
			OFlow.Write(Buffer, IFlow.ReadUpTo( sizeof( Buffer ), Buffer ) );

		IFlow.Dismiss();
	qRR
	qRT
	qRE
	}
}

fdr::rWDriver &flx::rASync_::Init(
	rRelay_ &Relay,
	fdr::rWDriver &ODriver )
{
qRH
	async_::rData Data;
qRB
	In_.Init( Relay );
	Out_.Init( Relay );

	Data.IDriver = &In_;
	Data.ODriver = &ODriver;

	mtk::Launch( async_::Routine, &Data );
qRR
qRT
qRE
	return Out_;
}

fdr::rRDriver &flx::VoidRDriver = flx::VoidIDriver;
fdr::rWDriver &flx::VoidWDriver = flx::VoidODriver;

flw::rRFlow &flx::VoidRFlow = flx::VoidIFlow;
flw::rWFlow &flx::VoidWFlow = flx::VoidOFlow;


Q37_GCTOR( flx )
{
	flx::VoidODriver.Init( fdr::ts_Default, flx::aAllowed );
	flx::VoidOFlow.Init();
	flx::PlaceholderODriver.Init( fdr::ts_Default, flx::aForbidden );
	flx::PlaceholderOFlow.Init();

	flx::VoidIDriver.Init( fdr::ts_Default, flx::aAllowed );
	flx::VoidIFlow.Init();
	flx::PlaceholderIDriver.Init( fdr::ts_Default, flx::aForbidden );
	flx::PlaceholderIFlow.Init();
}

Q37_GDTOR( flx )
{
	// Below line should not be necessary, but if missing, 'pure virtual function call' problem (at least under VC++10).
	tol::reset( flx::VoidODriver, flx::VoidOFlow, flx::PlaceholderODriver, flx::PlaceholderOFlow, flx::VoidIDriver, flx::VoidIFlow, flx::PlaceholderIDriver, flx::PlaceholderIFlow );
}
