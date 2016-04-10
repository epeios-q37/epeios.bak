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

#include "flx.h"

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

namespace {
	void BuildParts_(
		flw::sIFlow &Flow,
		str::dStrings &Parts )
	{
	qRH
		str::wString Part;
		bso::char__ C;
	qRB
		Part.Init();

		while ( !Flow.EndOfFlow() ) {
			switch ( C = Flow.Get() ) {
			case '/':
			case '\\':
				if ( ( Part.Amount() != 0 ) || ( Parts.Amount() == 0 ) )	// So we can have a leading '/'.
					Parts.Append( Part );
				Part.Init();
				break;
			default:
				Part.Append( C );
				break;
			}
		}

		if ( Part.Amount() != 0 )
			Parts.Append( Part );
	qRR
	qRT
	qRE
	}

	void BuildParts_(
		const str::dString &Name,
		str::dStrings &Parts )
	{
	qRH
		flx::E_STRING_IFLOW__ Flow;
	qRB
		Flow.Init( Name );
		BuildParts_( Flow, Parts );
	qRR
	qRT
	qRE
	}

	void BuildParts_(
		const fnm::rName &Name,
		str::dStrings &Parts )
	{
	qRH
		str::wString Buffer;
	qRB
		Buffer.Init();
		BuildParts_( Name.UTF8( Buffer ), Parts );
	qRR
	qRT
	qRE
	}

	void BuildPath_(
		const str::dStrings &Parts,
		sdr::sRow Last,
		str::dString &Path )
	{
		sdr::sRow Row = Parts.First();

		while ( ( Row != qNIL ) && ( Row != Last ) )
		{
			Path.Append( Parts( Row ) );
			Path.Append( '/' );

			Row = Parts.Next( Row );
		}

		if ( Row != qNIL ) {
			Path.Append( Parts( Row ) );
			if ( Path( Path.Last() ) == ':' )
				Path.Append('/');
		} else
			qRFwk();
	}

	eState CreateDirs_(
		const str::dStrings &Parts,
		sdr::sRow First )
	{
		eState State = sOK;
	qRH
		str::wString Path;
	qRB
		sdr::sRow &Row = First;

		while ( (Row != qNIL) && ( State == sOK ) ) {
			Path.Init();
			BuildPath_( Parts, Row, Path );

			State = CreateDir( Path );

			Row = Parts.Next( Row );
		}
	qRR
	qRT
	qRE
		return State;
	}

	eState CreateDirs_( const str::dStrings &Parts )
	{
		eState State = s_Undefined;
	qRH
		sdr::sRow Row = qNIL;
		str::wString Path;
		bso::sBool Continue = true;
	qRB
		Path.Init();
		Row = Parts.Last();

		while ( ( Row != qNIL ) && Continue ) {
			Path.Init();
			BuildPath_( Parts, Row, Path );

			if ( Continue = !fil::Exists( Path ) )
				Row = Parts.Previous( Row );
		}

		if ( Row == Parts.Last() )
			State = sExists;
		else
			State = CreateDirs_( Parts, Row == qNIL ? Parts.First() : Parts.Next( Row ) );
	qRR
	qRT
	qRE
		return State;
	}
}

eState dir::CreateDirWithParents( const fnm::rName &Name )
{
	eState State = s_Undefined;
qRH
	str::wStrings Parts;
qRB
	Parts.Init();
	BuildParts_( Name, Parts );

	State = CreateDirs_( Parts );
qRR
qRT
qRE
	return State;
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

	switch ( Name( Name.Last() ) ) {
	case '/':
	case '\\':
		Name.Truncate();
		break;
	}

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
