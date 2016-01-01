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

#define DWTGHT__COMPILATION

#include "dwtght.h"

#include "dir.h"

using namespace dwtght;
using namespace dwtbsc;

static inline status__ CreateGhostDir_( const fnm::name___ &Name )
{
	if ( dir::CreateDir( Name ) != dir::sOK ) {
		return sFailed;
# ifdef CPE_WIN
	} else if ( !fil::MakeSystem( Name ) ) {
		if ( dir::DropDir( Name ) != dir::sOK )
			ERRSys();
		return sFailed;
# endif
	} else
		return sCreated;
}

status__ dwtght::CreateGhost(
	const str::string_ &Root,
	const str::string_ &Path,
	const str::string_ &BaseName,
	const dwtbsc::ghosts_oddities_ &GO,
	grow__ Parent,
	ghosts_ &Ghosts,
	grow__ &Row )
{
	status__ Status = s_Undefined;
qRH
	ghost Ghost;
	fnm::name___ LocalizedName;
qRB
	Ghost.Init();

	Ghost.S_.Parent = Parent;
	Ghost.Name = BaseName;

	LocalizedName.Init();
	GetGhostLocalizedName( Row = Ghosts.Add( Ghost ), Root, Path, GO, LocalizedName );

	Status = CreateGhostDir_( LocalizedName );
qRR
qRT
qRE
	return Status;
}

static void SetGhostsFilesHook_(
	const fnm::name___ &Path,
	uys::mode__ Mode,
	ghosts_fh___ &FilesHook )
{
qRH
	lstctn::hf___ Filenames;
qRB
	Filenames.Init( Path, "Ghosts_" );

	FilesHook.Init( Filenames, Mode, uys::bPersistent, flsq::GetId() );
qRR
qRT
qRE
}

namespace {
	void GetGhosts_(
		fnm::name___ &DataDirName,
		uys::mode__ Mode,
		rack___ &Rack )
	{
		SetGhostsFilesHook_( DataDirName, Mode, Rack.FilesHook );

		if ( !lstctn::Plug( Rack.Ghosts, Rack.FilesHook ).Boolean() )
			Rack.Ghosts.Init();
	}
}

ghosts_ &dwtght::GetRWGhosts(
	const str::string_ &Root,
	const dwtbsc::ghosts_oddities_ &GO,
	rack___ &Rack )
{
qRH
	fnm::name___ Name;
qRB
	Name.Init();
	GetGhostsDataDirName( Root, GO, Name );

	if ( !fil::Exists(Name) ) {
		switch ( CreateGhostDir_( Name ) ) {
		case sCreated:
			break;
		case sFailed:
			qRGnr();
			break;
		default:
			qRGnr();
			break;
		}
	}

	GetGhosts_( Name, uys::mReadWrite, Rack );
qRR
qRT
qRE
	return Rack.Ghosts;
}

const ghosts_ &dwtght::GetROGhosts(
	const str::string_ &Root,
	const dwtbsc::ghosts_oddities_ &GO,
	rack___ &Rack )
{
qRH
	fnm::name___ Name;
qRB
	Name.Init();
	GetGhostsDataDirName( Root, GO, Name );

	if ( fil::Exists( Name ) )
		GetGhosts_( Name, uys::mReadOnly, Rack );
	else
		Rack.Ghosts.Init();
qRR
qRT
qRE
	return Rack.Ghosts;
}

const str::string_ &dwtght::GetPath(
	grow__ Row,
	const ghosts_ &Ghosts,
	str::string_ &Path,
	depth__ &Depth )
{
	ctn::E_CMITEMt( ghost_, grow__ ) Ghost;

	Ghost.Init( Ghosts );

	if ( Row != qNIL) {
		Path.Append( Ghost( Row ).Name );

		Row = Ghost( Row ).S_.Parent;
	}

	while ( Row != qNIL ) {
		Depth++;
		Path.InsertAt( '/' );
		Path.InsertAt( Ghost( Row ).Name );

		Row = Ghost( Row ).S_.Parent;
	}

	return Path;
}

static void ShowGhosts_(
	const ghosts_ &Ghosts,
	const ghosts_oddities_ &GO,
	txf::text_oflow__ &TFlow )
{
qRH
	dwtght::grow__ Row = qNIL;
	str::string Path;
	ctn::E_CMITEMt( ghost_, dwtght::grow__ ) Ghost;
qRB
	Ghost.Init( Ghosts );
	Row = Ghosts.First();

	if ( Row != qNIL )
		Row = Ghosts.Next( Row );	// On passe la racine.

	while ( Row != qNIL ) {

		TFlow << *Row << ' ';

		TFlow << '(' << *Ghost( Row ).S_.Parent << ')';

		Path.Init();
		TFlow << " : " << GetPath( Row, Ghosts, Path ) << txf::nl;

		Row = Ghosts.Next( Row );
	}
qRR
qRT
qRE
}

void dwtght::ShowGhosts(
	const str::string_ &Root,
	const ghosts_oddities_ &GO,
	txf::text_oflow__ &TFlow )
{
qRH
	rack___ Rack;
qRB
	Rack.Init();

	ShowGhosts_( GetROGhosts( Root, GO, Rack ), GO, TFlow );
qRR
qRT
qRE
}
