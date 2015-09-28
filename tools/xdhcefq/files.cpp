/*
	'files' module by Claude SIMON (http://zeusw.org/epeios/contact.html).
	Part of the 'xdhcefq' tool.
	Copyright (C) 2014 by Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of the Epeios project (http://zeusw.org/epeios/).

    This file is part of 'xdhcefq'.

    'xdhcefq' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'xdhcefq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'xdhcefq'.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "files.h"
#include "xdhcmn.h"
#include "flx.h"

using misc::nstring___;

static class run_file_dialog_callback___
: public  cef_run_file_dialog_callback_t
{
private:
	str::string _Id;
	str::string _UserAction;
	_cef_browser_host_t *_BrowserHost;
public:
	void reset( bso::bool__ P = true )
	{
		misc::Set<cef_run_file_dialog_callback_t>( this );

		_Id.reset( P );
		_UserAction.reset( P );
		_BrowserHost = NULL;
	}
	E_CDTOR( run_file_dialog_callback___ );
	void Init(
		const str::string_ &Id,
		const str::string_ &UserAction,
		_cef_browser_host_t *BrowserHost )
	{
		misc::Set<cef_run_file_dialog_callback_t>( this );

		_Id.Init( Id );
		_UserAction.Init( UserAction );
		_BrowserHost = BrowserHost;
	}
	const str::string_ &Id( void )
	{
		return _Id;
	}
	const str::string_ &UserAction( void )
	{
		return _UserAction;
	}
	cef_browser_host_t *BrowserHost( void )
	{
		if ( _BrowserHost == NULL )
			qRGnr();

		return _BrowserHost;
	}
} RunFileDialogCallback_;

void Fill_(
	cef_string_list_t List,
	strmrg::table_ &Table )
{
qRH
	int Total = 0;
	int Count = 0;
	cef_string_t Value;
	cef_nstring_t CEFBuffer;
	str::string Buffer;
qRB
	misc::Set( &CEFBuffer );
	Total = cef_string_list_size( List );

	while ( Count < Total ) {
		misc::Set( &Value );
		if ( cef_string_list_value( List, Count, &Value ) != 1 )
			qRGnr();

		cef_convert_from( &Value, &CEFBuffer );

		Buffer.Init();
		Table.Append( nstring___( CEFBuffer.str ).UTF8( Buffer ) );

		misc::Clear( &CEFBuffer );

		Count++;
	}
qRR
qRT
	misc::Clear( &CEFBuffer );
qRE
}

static void CEF_CALLBACK Cont_(
      struct _cef_run_file_dialog_callback_t* self,
	  int selected_accept_filter,
      cef_string_list_t file_paths)
{
qRH
	strmrg::table Table, SubTable;
	str::string Param;
	cef_browser_host_t *BrowserHost = NULL;
	cef_browser_t *Browser = NULL;
qRB
	run_file_dialog_callback___ &RunFileDialogCallback = *(run_file_dialog_callback___ *)self;
	BrowserHost = RunFileDialogCallback.BrowserHost();
	Browser = BrowserHost->get_browser( BrowserHost );

	Table.Init();
	Table.Append( RunFileDialogCallback.Id() );
	Table.Append( RunFileDialogCallback.UserAction() );

	SubTable.Init();
	Fill_( file_paths, SubTable );

	Table.AppendMono( SubTable );

	Param.Init();
	strmrg::Merge( Table, Param );

	misc::SendMessage( misc::rmHandleSelectedFiles, Browser, Param );
qRR
qRT
	if ( Browser != NULL )
		misc::Release( Browser );

	misc::Release( self );
//	misc::Release( browser_host );
qRE
}

static void FileDialog_(
	cef_browser_t *Browser,
	cef_file_dialog_mode_t FileDialogMode,
	const str::string_ &Id,
	const str::string_ &UserAction,
	const str::string_ &Title,
	const str::string_ &DefaultFileName,
	const str::strings_ &AcceptTypes )
{
qRH
	cef_string_list_t List = NULL;
	_cef_browser_host_t *BrowserHost = NULL;
qRB
	BrowserHost = Browser->get_host( Browser );

	if ( BrowserHost == NULL )
		qRGnr();

	RunFileDialogCallback_.Init( Id, UserAction, BrowserHost );

	RunFileDialogCallback_.on_file_dialog_dismissed = Cont_;

	List = cef_string_list_alloc();

	if ( List == NULL )
		qRGnr();

	misc::FillList( AcceptTypes, List );

	BrowserHost->run_file_dialog( BrowserHost, FileDialogMode, misc::cef_string___( Title ), misc::cef_string___( DefaultFileName ), List, 0, &RunFileDialogCallback_ );
qRR
qRT
	if ( List != NULL )
		cef_string_list_free( List );

	if ( BrowserHost != NULL )
		misc::Release( BrowserHost );
qRE
}

static void GetTDT_(
	const xdhcbk::args_ &Args,
	str::string_ &Title,
	str::string_ &Default,
	str::strings_ &Types )
{
	xdhcbk::retriever__ Retriever;

	Retriever.Init( Args );

	Retriever.GetString( Title );

	if ( Retriever.Availability() != strmrg::aNone )
		Retriever.GetStrings( Types );

	if ( Retriever.Availability() != strmrg::aNone )
		Retriever.GetString( Default );
}

static void Get_(
	const xdhcbk::args_ &Args,
	str::string_ &Id,
	str::string_ &Action,
	str::string_ &Title,
	str::string_ &Default,
	str::strings_ &Types )
{
qRH
	xdhcbk::retriever__ Retriever;
	xdhcbk::args SubArgs;
qRB
	Retriever.Init( Args );

	Retriever.GetString( Id );
	Retriever.GetString( Action );

	SubArgs.Init();
	Retriever.GetTable( SubArgs );
	GetTDT_( SubArgs, Title, Default, Types );
qRR
qRT
qRE
}

static void FileDialog_(
	cef_browser_t *Browser,
	cef_file_dialog_mode_t FileDialogMode,
	const xdhcbk::args_ &Args )
{
qRH
	str::string Id, Action, Title, Default;
	str::strings Types;
qRB
	Id.Init();
	Action.Init();
	Title.Init();
	Default.Init();
	Types.Init();

	Get_( Args, Id, Action, Title, Default, Types );

	FileDialog_( Browser, FileDialogMode, Id, Action, Title, Default, Types );
qRR
qRT
qRE
}

static void FileDialog_(
	cef_browser_t *Browser,
	cef_file_dialog_mode_t FileDialogMode,
	const str::string_ &MergedParams )
{
qRH
	xdhcbk::args Args;
qRB
	Args.Init();
	xdhcbk::Split( MergedParams, Args );

	FileDialog_( Browser, FileDialogMode, Args );
qRR
qRT
qRE
}

void files::FileDialog(
	cef_browser_t *Browser,
	cef_file_dialog_mode_t FileDialogMode,
	const nstring___  &Params )
{
qRH
	str::string Buffer;
qRB
	Buffer.Init();	
	FileDialog_( Browser, FileDialogMode, Params.UTF8( Buffer ) );
qRR
qRT
qRE
}

static void GetIAFN_(
	const xdhcbk::args_ &Args,
	str::string_ &Id,
	str::string_ &Action,
	str::strings_ &FileNames )
{
	xdhcbk::retriever__ Retriever;

	Retriever.Init( Args );

	Retriever.GetString( Id );
	Retriever.GetString( Action );
	Retriever.GetStrings( FileNames );
}

void files::HandleResult(
	cef_list_value_t *ListValue,
	str::string_ &Id,
	str::string_ &Action,
	str::strings_ &FileNames )
{
qRH
	xdhcbk::args Args;
qRB
	Args.Init();
	misc::GetArgs( ListValue, Args );

	GetIAFN_( Args, Id, Action, FileNames );
qRR
qRT
qRE
}


