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

#define WINTOL__COMPILATION

#include "wintol.h"

using namespace wintol;
#include "tol.h"
#include "cio.h"

#ifdef CIO__AUTOMATIC_INITIALIZATION
#	error "'CIO' is automatically initialized, which will crash the program if launched (not installed) as Windows service :"
#endif

static wintol::service__ *Service_ = NULL;

// Inspired from http://cpp.developpez.com/faq/vc/?page=ProcessThread#HowToMakeService 

#include <windows.h>

static BOOL SendStatusToSCM_(DWORD dwCurrentState,
					 DWORD dwWin32ExitCode,
					 DWORD dwServiceSpecificExitCode,
					 DWORD dwCheckPoint,
					 DWORD dwWaitHint);

static HANDLE hTerminateEvent_ = NULL;
static HANDLE hTerminateThread_ = NULL;
static HANDLE ThreadHandle_ = NULL;

static SERVICE_STATUS_HANDLE ServiceStatusHandle_;

static BOOL bPauseService_ = FALSE;
static BOOL bRunningService_ = FALSE;



static DWORD ServiceThread_(LPDWORD param)
{		
/*
	if(::WaitForSingleObject(hTerminateThread, 0) == WAIT_OBJECT_0)
	{
		// signale l'objet event d'attente et sort.
		::SetEvent(hTerminateEvent);
		return 0;
	}
*/
	if ( Service_ == NULL )
		qRFwk();

	::Service_->Process();

	return 0;
}
// ------------------------------------------------------
static BOOL InitService_( void )
{
	DWORD id=0;	
	ThreadHandle_ = CreateThread(0,0,(LPTHREAD_START_ROUTINE)ServiceThread_,NULL,0,&id);
	if(ThreadHandle_==0)
	{
		if(ServiceStatusHandle_)  SendStatusToSCM_(SERVICE_STOPPED,0,0,0,0);
		if(hTerminateEvent_)		 CloseHandle(hTerminateEvent_);
		if(hTerminateThread_)	 CloseHandle(hTerminateThread_);

		return FALSE;
	}	
	bRunningService_=TRUE;
	return TRUE;	
}
// ------------------------------------------------------
static void ResumeService_()
{
	bPauseService_ = FALSE;
	ResumeThread(ThreadHandle_);
}
// ------------------------------------------------------
static void PauseService_()
{
	bPauseService_ = TRUE;
	SuspendThread(ThreadHandle_);
}
// ------------------------------------------------------
static void StopService_()
{
	bRunningService_=FALSE;
	SetEvent(hTerminateThread_);
}
// ------------------------------------------------------
static BOOL SendStatusToSCM_(DWORD dwCurrentState,
                     DWORD dwWin32ExitCode,
                     DWORD dwServiceSpecificExitCode,
                     DWORD dwCheckPoint,
                     DWORD dwWaitHint)
{
    BOOL bSuccess;
    SERVICE_STATUS ServiceStatus;
    ServiceStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
    ServiceStatus.dwCurrentState= dwCurrentState;
    
    ServiceStatus.dwControlsAccepted=0;
    if(dwCurrentState != SERVICE_START_PENDING)        
    ServiceStatus.dwControlsAccepted= SERVICE_ACCEPT_STOP |
                                      SERVICE_ACCEPT_SHUTDOWN|         
                                      SERVICE_ACCEPT_PAUSE_CONTINUE;

    ServiceStatus.dwWin32ExitCode=ERROR_SERVICE_SPECIFIC_ERROR;
    if(!dwServiceSpecificExitCode) ServiceStatus.dwWin32ExitCode=dwWin32ExitCode;
    
    ServiceStatus.dwServiceSpecificExitCode=dwServiceSpecificExitCode;
    
    ServiceStatus.dwCheckPoint = dwCheckPoint;
    ServiceStatus.dwWaitHint = dwWaitHint;
    
    bSuccess = SetServiceStatus(ServiceStatusHandle_,&ServiceStatus);
    if(!bSuccess) StopService_();
    return bSuccess;
}
// ------------------------------------------------------
static void ServiceCtrlHandler_(DWORD controlCode)
{
	DWORD currentState= 0;
	BOOL  bSuccess;
	switch(controlCode)
	{
		case SERVICE_CONTROL_STOP:
				currentState= SERVICE_STOP_PENDING;
				bSuccess= SendStatusToSCM_(SERVICE_STOP_PENDING,NO_ERROR,0,1,5000);
				StopService_();
				return;

		case SERVICE_CONTROL_PAUSE:
				if(bRunningService_ && !bPauseService_)
				{
					bSuccess= SendStatusToSCM_(SERVICE_PAUSE_PENDING,NO_ERROR,0,1,1000);
					PauseService_();
					currentState= SERVICE_PAUSED;
				}
				break;
		case SERVICE_CONTROL_CONTINUE:
				if(bRunningService_ && bPauseService_)
				{
					bSuccess= SendStatusToSCM_(SERVICE_CONTINUE_PENDING,NO_ERROR,0,1,1000);
					ResumeService_();
					currentState= SERVICE_RUNNING;
				}
				break;
		case SERVICE_CONTROL_INTERROGATE:break;

		case SERVICE_CONTROL_SHUTDOWN:
//			ERRExit( EXIT_SUCCESS );
			if ( ::Service_ == NULL )
				qRFwk();
			::Service_->Shutdown();
			return;

		default:break;						
	}
	SendStatusToSCM_(currentState,NO_ERROR,0,0,0);
}
// ------------------------------------------------------
static void Terminate_(DWORD error)
{		
	if(ServiceStatusHandle_) SendStatusToSCM_(SERVICE_STOPPED,error,0,0,0);
	if(ThreadHandle_)		 CloseHandle(ThreadHandle_);	
	if(hTerminateEvent_)		 CloseHandle(hTerminateEvent_);
	if(hTerminateThread_)	 CloseHandle(hTerminateThread_);

	if ( error != 0 )
		qRSys();
}
// ------------------------------------------------------
static bso::bool__ AddService_(
	const char *ServiceName,
	const char *DisplayName,
	const char *Description )
{
	SC_HANDLE newService,scm;
//	CString strPath;
	char szFilename[255];

	GetModuleFileName(NULL,szFilename,sizeof(szFilename));
//	strPath=szFilename;

	scm= OpenSCManager(0,0,SC_MANAGER_CREATE_SERVICE);

//	if(!scm) ErrorHandler("Dans OpenScManager",GetLastError());

	if ( !scm )
		return false;

	newService= CreateService(scm,ServiceName,
							  ( DisplayName == NULL ? ServiceName : DisplayName ),
							  SERVICE_ALL_ACCESS,
							  SERVICE_WIN32_OWN_PROCESS,
							  SERVICE_AUTO_START,
							  SERVICE_ERROR_NORMAL,
							  szFilename,
							  0,0,0,0,0);
	if (!newService )
		return false;

	if ( Description != NULL ) {
		SERVICE_DESCRIPTION ServiceDescriptionStruct = { (char *)Description };

		ChangeServiceConfig2( newService, SERVICE_CONFIG_DESCRIPTION, &ServiceDescriptionStruct );
	}

	return true;
}
// ------------------------------------------------------
static bso::bool__ RemoveService_( const char *Name )
{
	SC_HANDLE newService,scm;
//	CString strPath;
	char szFilename[255];

	GetModuleFileName(NULL,szFilename,sizeof(szFilename));
//	strPath=szFilename;

	scm = OpenSCManager(0,0,SC_MANAGER_ALL_ACCESS);

//	if (!scm) ErrorHandler("Dans OpenScManager",GetLastError());

	if ( !scm )
		return false;

	newService=OpenService(scm,Name,SERVICE_ALL_ACCESS| DELETE);

	if(newService)	DeleteService(newService);	

//	if(!newService)	ErrorHandler("Dans DeleteService",GetLastError());

	if ( !newService )
		return false;

	return true;
}
// ------------------------------------------------------
static void ServiceMain_(DWORD argc,LPTSTR *argv)
{
	BOOL bSuccess;

	if ( argc < 1 )
		qRFwk();
    
	ServiceStatusHandle_ = RegisterServiceCtrlHandler(argv[0],(LPHANDLER_FUNCTION)ServiceCtrlHandler_);
	if(!ServiceStatusHandle_)
	{
		Terminate_(GetLastError());
		return;
	}
	bSuccess= SendStatusToSCM_(SERVICE_START_PENDING,NO_ERROR,0,1,5000);
	if(!bSuccess)
	{
		Terminate_(GetLastError());
		return;
	}
	hTerminateEvent_ = CreateEvent(0,TRUE,FALSE,0);
	if(!hTerminateEvent_)
	{
		Terminate_(GetLastError());
		return;
	}
	hTerminateThread_ = CreateEvent(0,TRUE,FALSE,0);
	if(!hTerminateThread_)
	{
		Terminate_(GetLastError());
		return;
	}
	bSuccess= SendStatusToSCM_(SERVICE_START_PENDING,NO_ERROR,0,2,1000);
	if(!bSuccess)
	{
		Terminate_(GetLastError());
		return;
	}
	bSuccess = InitService_();
	if(!bSuccess)
	{
		Terminate_(GetLastError());
		return;
	}
	bSuccess= SendStatusToSCM_(SERVICE_RUNNING,NO_ERROR,0,0,0);
	if(!bSuccess)
	{
		Terminate_(GetLastError());
		return;
	}		
	WaitForSingleObject(hTerminateEvent_,INFINITE);
	Terminate_(0);
}

bso::bool__ wintol::InstallService(
	const char *ServiceName,
	const char *DisplayName,
	const char *Description )
{
	return AddService_( ServiceName, DisplayName, Description );
}

bso::bool__ wintol::Remove( const char *ServiceName )
{
	return RemoveService_( ServiceName );
}

bso::bool__ wintol::service__::Launch( const char *ServiceName )
{
	PatchSignalHandlingForWindowsService();

	SERVICE_TABLE_ENTRY serviceTable[]=
	{
		{ (char *)ServiceName,(LPSERVICE_MAIN_FUNCTION)ServiceMain_},
		{NULL , NULL}
	};

	if ( Service_ != NULL )
		qRFwk();

	Service_ = this;
 
	return ( StartServiceCtrlDispatcher(serviceTable) == TRUE );
}

bso::bool__ wintol::service__::TestTermination( void )
{
	if(::WaitForSingleObject(hTerminateThread_, 0) == WAIT_OBJECT_0)
	{
		// signale l'objet event d'attente et sort.
		::SetEvent(hTerminateEvent_);
		return true;
	} else
		return false;
}

