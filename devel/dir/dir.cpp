/*
	Copyright (C) 1999-2016 Claude SIMON (http://q37.info/contact/).

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

#define DIR__COMPILATION

#include "dir.h"

using namespace dir;

state__ dir::HandleError( void )
{
	switch ( errno ) {
	case EEXIST:
		return sExists;
		break;
	case EPERM:
	case EACCES:
	case EROFS:
# ifdef CPE_C_MINGW	// 'MinGW' ne connait pas 'ELOOP'.
	case WSAELOOP:
# else
	case ELOOP:
# endif
	case ENAMETOOLONG:
	case ENOENT:
		return sInadequatePath;
		break;
	case ENOTDIR:
		return sBadPath;
		break;
	case EFAULT:
		qRFwk();
		break;
	case ENOTEMPTY:
		return sNotEmpty;
		break;
	default:
		qRSys();
		break;
	}

	return s_Undefined;	// Pour viter un 'warning'.
}

void dir::FileSystem( str::dStrings &Paths )
{
# ifdef DIR__WIN
	qRH
		ntvstr::string___ RawString;
		str::wString Path;
	qRB
		DWORD dwSize = MAX_PATH;
		WCHAR szLogicalDrives[MAX_PATH] = {0};
		DWORD dwResult = GetLogicalDriveStringsW(dwSize,szLogicalDrives);

		if ( (dwResult <= 0) || ( dwResult > MAX_PATH ) )
			qRFwk();

		WCHAR* szSingleDrive = szLogicalDrives;
		while(*szSingleDrive)
		{
			RawString.Init( szSingleDrive );

			Path.Init();
			Paths.Append( RawString.UTF8( Path ) );

			szSingleDrive += wcslen(szSingleDrive) + 1;
		}
	qRR
	qRT
	qRE
# else
	qRVct();
# endif
}

namespace {
# ifdef DIR__WIN
	eBusType Convert_( STORAGE_BUS_TYPE BusType )
	{
		switch ( BusType ) {
		case BusTypeUsb:
			return btUSB;
			break;
		default:
			return bt_Undefined;
			break;
		}
	}
# endif
}

eBusType dir::GetDiskBusType( const fnm::rName &Disk )
{
	eBusType Type = bt_Undefined;
# ifdef DIR__WIN
qRH
	str::wString Name;
	DWORD Size = 0;
	HANDLE Handle = INVALID_HANDLE_VALUE;
	STORAGE_PROPERTY_QUERY Query;
	STORAGE_DEVICE_DESCRIPTOR Descriptor;
qRB
	Name.Init( "\\\\.\\" );
	Disk.UTF8( Name );

	Handle = CreateFileW( fnm::rName( Name ).Internal(), 0, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);

	if ( Handle == INVALID_HANDLE_VALUE )
		qRReturn;

	Query.PropertyId = StorageDeviceProperty;
	Query.QueryType = PropertyStandardQuery;


	if ( DeviceIoControl(Handle, IOCTL_STORAGE_QUERY_PROPERTY, &Query, sizeof( Query ), &Descriptor, sizeof( Descriptor ), &Size ,NULL ) == 0 )
		qRReturn;

	Type = Convert_( Descriptor.BusType );
qRR
qRE
qRT
# else
	qRVct();
# endif
	return Type;
}
