/*
	'nsxpcm.cpp' by Claude SIMON (http://zeusw.org/).

	'nsxpcm' is part of the Epeios framework.

    The Epeios framework is free software: you can redistribute it and/or
	modify it under the terms of the GNU General Public License as published
	by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Epeios framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The Epeios framework.  If not, see <http://www.gnu.org/licenses/>.
*/

#define NSXPCM__COMPILATION

#include "nsxpcm.h"

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

#pragma warning( push )
#pragma warning( disable : 4800 )	// forcing value to bool 'true' or 'false' (performance warning)

#include "nsMemory.h"
#include "nsIDOMEvent.h"
#include "nsIDOMEventTarget.h"
#include "nsIFilePicker.h"
#include "nsILocalFile.h"
#include "nsIConsoleService.h"
#include "nsIXSLTProcessor.h"
#include "nsIVariant.h"
#include "nsIDOMParser.h"
#include "nsIXMLHttpRequest.h"
#include "nsIIOService.h"
#include "nsIExternalProtocolService.h"
#include "nsIDirectoryService.h"
#include "nsIFormHistory.h"
#include "nsIDOMNodeList.h"
#include "nsIDOMMozNamedAttrMap.h"
//#include "nsDOMAttributeMap.h"
#include "nsIDOMAttr.h"
#include "nsITreeColumns.h"

#pragma warning( pop )

#include "xpp.h"
#include "txf.h"
//#include "mtk.h"
#include "fnm.h"
#include "mtx.h"

#define T( f )\
	if ( ( f ) != NS_OK )\
		ERRLbr()

#if defined NSXPCM__ENABLE_FORMHISTORY
#	define ENABLE_FORMHISTORY
#endif

using namespace nsxpcm;

nsIDOMWindow *MasterWindow_ = NULL;
bch::E_BUNCH( nsIDOMWindow *) MasterWindows_;
mtx::handler__ MasterWindowMutex_;
bso::uint__ MasterWindowCounter_ = 0;

void nsxpcm::AddMasterWindow( nsIDOMWindow *Window )
{
ERRProlog
	bso::bool__ Locked = false;
ERRBegin
	if ( Window == NULL )
		ERRPrm();

	mtx::Lock( MasterWindowMutex_ );

	Locked = true;

	if ( MasterWindow_ == NULL )
		MasterWindow_ = Window;
	else
		MasterWindows_.Append( Window );
ERRErr
ERREnd
	if ( Locked )
		mtx::Unlock( MasterWindowMutex_ );
ERREpilog
}

void nsxpcm::RemoveMasterWindow( nsIDOMWindow *Window )
{
ERRProlog
	bso::bool__ Locked = false;
ERRBegin
	if ( Window == NULL )
		ERRPrm();

	mtx::Lock( MasterWindowMutex_ );

	Locked = true;

	if ( MasterWindow_ == Window ) {
		while ( MasterWindowCounter_ != 0 ) {
			mtx::Unlock( MasterWindowMutex_ );
			Locked = false;

			tht::Defer();

			mtx::Lock( MasterWindowMutex_ );
			Locked = true;
		}

		if ( MasterWindows_.Amount() != 0 ) {
			MasterWindow_ = MasterWindows_( MasterWindows_.First() );

			MasterWindows_.Remove( MasterWindows_.First() );
		} else
			MasterWindow_ = NULL;
	} else {
		sdr::row__ Row = MasterWindows_.Search( Window );

		if ( Row == E_NIL )
			ERRFwk();

		MasterWindows_.Remove( Row );
	}
ERRErr
ERREnd
	if ( Locked )
		mtx::Unlock( MasterWindowMutex_ );
ERREpilog
}

nsIDOMWindowInternal *nsxpcm::RetrieveMasterWindow( void )
{
	nsIDOMWindow *Window = NULL;
ERRProlog
	bso::bool__ Locked = false;
ERRBegin
	mtx::Lock( MasterWindowMutex_ );
	Locked = true;

	if ( MasterWindow_ != NULL ) {
		if ( MasterWindowCounter_ == BSO_UINT_MAX )
			ERRLmt();

		Window = MasterWindow_;
		MasterWindowCounter_++;
	}
ERRErr
ERREnd
	if ( Locked )
		mtx::Unlock( MasterWindowMutex_ );
ERREpilog
	return GetWindowInternal( Window );
}

void nsxpcm::ReleaseMasterWindow( nsIDOMWindow *Window )
{
ERRProlog
	bso::bool__ Locked = false;
ERRBegin
	mtx::Lock( MasterWindowMutex_ );
	Locked = true;

	if ( MasterWindow_ != Window )
		ERRPrm();

	if ( MasterWindowCounter_ == 0 )
		ERRFwk();

	MasterWindowCounter_--;
ERRErr
ERREnd
	if ( Locked )
		mtx::Unlock( MasterWindowMutex_ );
ERREpilog
}

nsIDOMWindowInternal* nsxpcm::GetRoot( nsIDOMWindow *Window )
{
	nsIDOMWindowInternal *Current = GetTop( Window ), *Candidate = GetOpener( Current );

	while ( Candidate != NULL ) {
		Current = GetTop( Candidate );
		Candidate = GetOpener( Current );
	}

	return Current;
}


static nsIDOMWindow *JSConsoleWindow_ = NULL;

static nsCOMPtr<nsIFormHistory2> FormHistory_;
#ifdef CPE__MT
mtx::mutex_handler__ FormHistoryMutex_;
#endif

#define CASE( m )\
	case t##m:\
	return NSXPCM_NAME "_" #m;\
	break

const char *nsxpcm::GetLabel( text__ Text )
{
	switch ( Text ) {
		CASE( XPRJFilterLabel );
		CASE( DynamicLibraryFilterLabel );
	default:
		ERRPrm();
		break;
	}

	return NULL;	// Pour viter un 'warning'.
}


void nsxpcm::GetJSConsole( nsIDOMWindow *ParentWindow )
{
	nsxpcm::GetJSConsole( ParentWindow, &JSConsoleWindow_ );
}

void nsxpcm::Transform(
	const char *String,
	sdr::size__ Size,
	char **JString )
{
    if ( !JString )
        ERRPrm();
	else
	    *JString = (char*) nsMemory::Clone( String, sizeof(char) * Size );
    
	if ( !JString )
		ERRFwk();
}

void nsxpcm::Transform(
	const char *String,
	char **JString )
{
	Transform( String, strlen( String ) + 1, JString );
}




void nsxpcm::Transform(
	const nsACString &CString,
	nsAString &WString )
{
#if 1
	NS_CStringToUTF16(CString, NS_CSTRING_ENCODING_UTF8, WString );
# else
	NS_CStringToUTF16(CString, NS_CSTRING_ENCODING_NATIVE_FILESYSTEM, WString );
#endif
}

void nsxpcm::Transform(
	const nsAString &WString,
	nsACString &CString )
{
#if 1
	NS_UTF16ToCString( WString, NS_CSTRING_ENCODING_UTF8, CString );
#else
	NS_UTF16ToCString( WString, NS_CSTRING_ENCODING_NATIVE_FILESYSTEM, CString );
#endif
}


void nsxpcm::Transform(
	const nsAString &WString,
	char **JString )
{
ERRProlog
	str::string String;
ERRBegin
	String.Init();

	Transform( WString, String );
	Transform( String, JString );
ERRErr
ERREnd
ERREpilog
}

void nsxpcm::Transform(
	const str::string_ &String,
	char **JString )
{
ERRProlog
	TOL_CBUFFER___ Buffer;
ERRBegin
	Transform( String.Convert( Buffer ), String.Amount() + 1, JString );
ERRErr
ERREnd
ERREpilog
}

void nsxpcm::Transform(
	const str::string_ &String,
	nsAString &WString )
{
	nsCString CString;

	Transform( String, CString );
	Transform( CString, WString );
}

void nsxpcm::TransformW(
	const str::string_ &String,
	nsAString &WString )
{
ERRProlog
	TOL_CBUFFER___ Buffer;
ERRBegin
	WString.Append( (const nsString::char_type *)String.Convert( Buffer ), String.Amount() / sizeof( nsString::char_type ) );
ERRErr
ERREnd
ERREpilog
}

void nsxpcm::Transform(
	const nsAString &WString,
	str::string_ &String )
{
	nsCString CString;

	Transform( WString, CString );
	Transform( CString, String );
}

void nsxpcm::TransformW(
	const nsAString &WString,
	str::string_ &String )
{
	String.Append( (const char *)WString.BeginReading(), WString.Length() * sizeof( nsString::char_type) );
}

void nsxpcm::Transform(
	const nsACString &CString,
	str::string_ &String )
{
	String.Append( (const char *)CString.BeginReading(), CString.Length() * sizeof( nsCString::char_type ) );
}

void nsxpcm::Transform(
	const char *String,
	nsACString &ECString )
{
	ECString.Assign( (const nsACString::char_type *)String );
}

void nsxpcm::Transform(
	const str::string_ &String,
	nsACString &CString )
{
ERRProlog
	TOL_CBUFFER___ Buffer;
ERRBegin
	Transform( String.Convert( Buffer ), CString );
ERRErr
ERREnd
ERREpilog
}

void nsxpcm::Split( 
	const string_ &Joined,
	bso::char__ Separator,
	strings_ &Splitted )
{
ERRProlog
	sdr::row__ Row = E_NIL;
	string Item;
ERRBegin
	if ( Joined.Amount() != 0 ) {	

		Item.Init();

		Row = Joined.First();

		while ( Row != E_NIL ) {
			if ( Joined( Row ) != Separator )
				Item.Append( Joined( Row ) );
			else {
				Splitted.Append( Item );
				Item.Init();
			}

			Row = Joined.Next( Row );
		}

		Splitted.Append( Item );
	}
ERRErr
ERREnd
ERREpilog
}

void nsxpcm::Split(
	const char *Joined,
	bso::char__ Separator,
	strings_ &Splitted )
{
ERRProlog
	string SJoined;
ERRBegin
	SJoined.Init( Joined );

	Split( SJoined, Separator, Splitted );
ERRErr
ERREnd
ERREpilog
}

void nsxpcm::Join(
	const strings_ &Splitted,
	bso::char__ Separator,
	string_ &Joined )
{
	sdr::row__ Row = Splitted.First();
	ctn::E_CMITEM( string_ ) Item;

	Item.Init( Splitted );

	if ( Row != E_NIL ) {
		Joined.Append( Item( Row ) );
		Row = Splitted.Next( Row );
	}

	while ( Row != E_NIL ) {
		Joined.Append( Separator );
		Joined.Append( Item( Row ) );
		Row = Splitted.Next( Row );
	}
}

void nsxpcm::JoinAndTransform(
	const strings_ &Splitted,
	bso::char__ Separator,
	char **JString )
{
ERRProlog
	string Joined;
ERRBegin
	Joined.Init();
	
	Join( Splitted, Separator, Joined );

	Transform( Joined, JString );
ERRErr
ERREnd
ERREpilog
}

void nsxpcm::SetSelectedItem(
	nsIDOMElement *Element,
	bso::bool__ SelectFirstOneIfE_NIL )
{
ERRProlog
	str::string Value;
	browser__ Browser;
	nsIDOMElement *Current = Element, *Selected = NULL;
ERRBegin
	Browser.Init( Element );

	while ( ( Current != NULL ) && ( Selected == NULL ) ) {
		Value.Init();

		GetAttribute( Current, NSCPM__XUL_EXTENSION_DEFAULT_NAMESPACE ":selected", Value );

		if ( Value == "true" )
			Selected = Current;

		Current = Browser.GetNext();
	}

	SetSelectedItem( Element, Selected );

	if ( ( Selected == NULL ) && ( SelectFirstOneIfE_NIL ) )
		nsxpcm::SetSelectedIndex( Element, 0 );
	
ERRErr
ERREnd
ERREpilog
}


void nsxpcm::listbox__::RemoveContent( void )
{
ERRProlog
	str::string Name;
	nsIDOMNode *Node = NULL, *Next = NULL;
ERRBegin
	Node = GetFirstChild( GetNode() );

	while ( Node != NULL ) {
		Next = GetNextSibling( Node );

		Name.Init();

		if ( GetNodeName( Node, Name ) == "listitem" )
			RemoveChild( GetNode(), Node );

		Node = Next;
	}

ERRErr
ERREnd
ERREpilog
}

nsIDOMNode *nsxpcm::_FindParent(
	nsIDOMNode *Node,
	const str::string_ &NodeName )
{
ERRProlog
	str::string Name;
ERRBegin
	while ( Node != NULL ) {
		Name.Init();

		GetNodeName( Node, Name );

		if ( NodeName == Name )
			break;

		Node = GetParentNode( Node );
	}

ERRErr
ERREnd
ERREpilog
	return Node;
}
/*
void nsxpcm::Handle( 
	nsIDOMEvent *Event,
	const element_cores_ &Cores )
{
ERRProlog
	nsIDOMEventTarget *EventTarget = NULL;
	nsIDOMElement *Element = NULL;
	nsEmbedString String;
	str::string S;
	mdr::row__ Row = E_NIL;
ERRBegin
	Event->GetTarget( &EventTarget );
	Element = nsxpcm::QueryInterface<nsIDOMElement>( EventTarget, err::hSkip  );

	if ( Element == NULL )
		ERRReturn;

	Row = Cores.First();

	while ( ( Row != E_NIL ) && ( Cores( Row )->GetElement() != Element ) )
		Row = Cores.Next( Row );

	if ( Row == E_NIL )
		ERRu();

	Event->GetType( String );

	S.Init();

	nsxpcm::Transform( String, S );

	Cores( Row )->Handle( Event, S );
ERRErr
ERREnd
ERREpilog
}
*/

void nsxpcm::file_picker_::AddFilter(
	const str::string_ &Title,
	const str::string_ &Mask )
{
ERRProlog
	file_picker_filter FilePickerFilter;
ERRBegin
	FilePickerFilter.Init();

	FilePickerFilter.Title = Title;
	FilePickerFilter.Mask = Mask;

	Filters.Append( FilePickerFilter );
ERRErr
ERREnd
ERREpilog
}

static inline PRInt16 ConvertType_( file_picker_type__ Type )
{
	switch( Type ) {
	case fptOpen :
		return nsIFilePicker::modeOpen;
		break;
	case fptSave :
		return nsIFilePicker::modeSave;
		break;
	case fptFolder :
		return nsIFilePicker::modeGetFolder;
		break;
	default:
		ERRPrm();
		break;
	}

	return 0;	// Pour viter un 'warning'.
}

static inline void AddFilter_(
	const char *Title,
	const char *Mask,
	nsIFilePicker *FilePicker )
{
	nsString WTitle, WMask;

	Transform( Title, WTitle );
	Transform( Mask, WMask );

	FilePicker->AppendFilter( WTitle, WMask );
}


static inline void AddFilter_(
	const file_picker_filter_ &Filter,
	nsIFilePicker *FilePicker )
{
ERRProlog
	TOL_CBUFFER___ TitleBuffer, MaskBuffer;
ERRBegin
	AddFilter_( Filter.Title.Convert( TitleBuffer ), Filter.Mask.Convert( MaskBuffer ), FilePicker );
ERRErr
ERREnd
ERREpilog
}


static void AddFilters_(
	const file_picker_filters_ &Filters,
	nsIFilePicker *FilePicker )
{
	ctn::E_CITEM( file_picker_filter_ ) Filter;
	sdr::row__ Row = Filters.First();

	Filter.Init( Filters );

	while( Row != E_NIL ) {
		AddFilter_( Filter( Row ), FilePicker );

		Row = Filters.Next( Row );
	}
}

#define H( mask, value )	ConvertedPredefinedFilters |= ( Filters & fpmf##mask ? nsIFilePicker::filter##value : 0 )
#define SH( mask )	H( mask, mask )


static inline PRInt32 ConvertPredefinedFilters_( int Filters )
{
	PRInt32 ConvertedPredefinedFilters = 0;

	SH( All );
	SH( HTML );
	SH( Text );
	SH( Images );
	SH( XML );
	SH( XUL );
	SH( Apps );

	return ConvertedPredefinedFilters;
}

void AddExtraPredefinedFilters_(
	int Filters,
	const lcl::locale_ &Locale,
	const char *Language,
	nsIFilePicker *FilePicker )
{
ERRProlog
	str::string Translation;
	TOL_CBUFFER___ Buffer;
ERRBegin
	if ( Filters & fpmfXPRJ ) {
		Translation.Init();
		AddFilter_( Locale.GetTranslation( GetLabel( nsxpcm::tXPRJFilterLabel ), Language, Translation ).Convert( Buffer ), "*.xprj", FilePicker );
	}

	if ( Filters & fpmfDynamicLibrary ) {
		Translation.Init();
		AddFilter_( Locale.GetTranslation( GetLabel( nsxpcm::tDynamicLibraryFilterLabel ), Language, Translation ).Convert( Buffer ), "*." NSXPCM__DYNAMIC_LIBRARY_EXTENSION, FilePicker );
	}
ERRErr
ERREnd
ERREpilog
}

#undef H
#undef SH

bso::bool__ nsxpcm::file_picker_::Show(
	nsIDOMWindow *ParentWindow,
	file_picker_type__ Type,
	const char *DefaultExtension,
	const lcl::locale_ &Locale,
	const char *Language,
	str::string_ &FileName )
{
	bso::bool__ FileSelected = false;
ERRProlog
	nsCOMPtr<nsIFilePicker> FilePicker = NULL;
	nsresult Error = NS_OK;
	nsString WString;
	nsIDOMWindow *MasterWindow = NULL;
ERRBegin
	CreateInstance( "@mozilla.org/filepicker;1", FilePicker );

	Transform( Title, WString );

	if ( ParentWindow == NULL )
		ERRPrm();
	/* 'ParentWindow' doit tre fourni et ne peut tr edduit de 'MAsterWindow', car,
	cette fonction bloquant toute action sur 'ParentWindow', si plusieurs sessions,
	c'est une mauvaise fentre (c'est--dire une qui n'a pas initi l'ouverture de
	slecteur) qui rique d'tre bloque). */

	if ( ( Error = FilePicker->Init( ParentWindow, WString, ConvertType_( Type ) ) ) != NS_OK )
		ERRLbr();

	if ( ( Error = FilePicker->AppendFilters( ConvertPredefinedFilters_( S_.PredefinedFilters ) ) ) != NS_OK )
		ERRLbr();

	AddFilters_( Filters, FilePicker );

	AddExtraPredefinedFilters_( S_.PredefinedFilters, Locale, Language, FilePicker );

	if ( ( DefaultExtension != NULL ) && ( *DefaultExtension != 0 ) ) {
		Transform( DefaultExtension, WString );
		T( FilePicker->SetDefaultExtension( WString ) );
	}

	PRInt16 _retval = 0;

	if ( ( Error = FilePicker->Show( &_retval ) ) != NS_OK )
		ERRLbr();

	if ( _retval == nsIFilePicker::returnCancel ) {
		FileSelected = false;
		ERRReturn;
	} else if ( ( _retval != nsIFilePicker::returnOK ) && ( _retval != nsIFilePicker::returnReplace ) )
		ERRLbr();

	nsIFile *File;

	if ( ( Error = FilePicker->GetFile( &File ) ) != NS_OK )
		ERRLbr();

	if ( ( Error = File->GetPath( WString ) ) != NS_OK )
		ERRLbr();

	Transform( WString, FileName );

	FileSelected = true;
ERRErr
ERREnd
	if ( MasterWindow != NULL )
		ReleaseMasterWindow( MasterWindow );
ERREpilog
	return FileSelected;
}

#if 0
static bso::bool__ FileDialogBox_(
	nsIDOMWindow *Parent,
	const str::string_ &Title,
	const char *DefaultExtension,
	PRInt16 Mode,
	PRInt32 FilterMask,
	str::string_ &Name )
{
	bso::bool__ Success = false;
ERRProlog
	nsCOMPtr<nsIFilePicker> FilePicker = NULL;
	nsresult Error = NS_OK;
	nsEmbedString EString;
ERRBegin
	CreateInstance( "@mozilla.org/filepicker;1", FilePicker );

	Transform( Title, EString );

	if ( Parent == NULL )
		Parent = ::MasterWindow;

	if ( Parent == NULL )
		ERRu();

	if ( ( Error = FilePicker->Init( Parent, EString, Mode ) ) != NS_OK )
		ERRExt();

	if ( ( Error = FilePicker->AppendFilters( FilterMask ) ) != NS_OK )
		ERRExt();

	if ( ( DefaultExtension != NULL ) && ( *DefaultExtension != 0 ) ) {
		Transform( DefaultExtension, EString );
		T( FilePicker->SetDefaultExtension( EString ) );
	}

	PRInt16 _retval = 0;

	if ( ( Error = FilePicker->Show( &_retval ) ) != NS_OK )
		ERRExt();

	if ( _retval == nsIFilePicker::returnCancel ) {
		Success = false;
		ERRReturn;
	} else if ( ( _retval != nsIFilePicker::returnOK ) && ( _retval != nsIFilePicker::returnReplace ) )
		ERRExt();

	nsILocalFile *File;

	if ( ( Error = FilePicker->GetFile( &File ) ) != NS_OK )
		ERRExt();

	if ( ( Error = File->GetPath( EString ) ) != NS_OK )
		ERRExt();

	nsxpcm::Transform( EString, Name );

	Success = true;
ERRErr
ERREnd
ERREpilog
	return Success;
}
#else
static bso::bool__ FileDialogBox_(
	nsIDOMWindow *ParentWindow,
	const str::string_ &Title,
	file_picker_type__ Type,
	const char *DefaultExtension,
	int PredefineFilters,
	const lcl::locale_ &Locale,
	const char *Language,
	str::string_ &FileName )
{
	bso::bool__ FileSelected = false;
ERRProlog
	file_picker FilePicker;
ERRBegin
	FilePicker.Init( Title );

	FilePicker.SetPredefinedFilter( PredefineFilters );

	FileSelected = FilePicker.Show( ParentWindow, Type, DefaultExtension, Locale, Language, FileName );
ERRErr
ERREnd
ERREpilog
	return FileSelected;
}
#endif

bso::bool__ nsxpcm::FileOpenDialogBox(
	nsIDOMWindow *ParentWindow,
	const str::string_ &Title,
	const char *DefaultExtension,
	int PredefinedFilters,
	const lcl::locale_ &Locale,
	const char *Language,
	str::string_ &FileName )
{
	return FileDialogBox_( ParentWindow, Title, fptOpen, DefaultExtension, PredefinedFilters, Locale, Language, FileName );
}

bso::bool__ nsxpcm::FileSaveDialogBox(
	nsIDOMWindow *ParentWindow,
	const str::string_ &Title,
	const char *DefaultExtension,
	int PredefinedFilters,
	const lcl::locale_ &Locale,
	const char *Language,
	str::string_ &FileName )
{
	return FileDialogBox_( ParentWindow, Title, fptSave, "", PredefinedFilters, Locale, Language, FileName );
}

bso::bool__ nsxpcm::DirectorySelectDialogBox(
	nsIDOMWindow *ParentWindow,
	const str::string_ &Title,
	const lcl::locale_ &Locale,
	const char *Language,
	str::string_ &FileName )
{
	return FileDialogBox_( ParentWindow, Title, fptFolder, "", fpmf_E_NIL, Locale, Language, FileName );
}

void nsxpcm::Delete( widget_cores_ &Widgets )
{
	sdr::row__ Row = Widgets.First();

	while ( Row != E_NIL ) {
		delete Widgets( Row );

		Widgets.Store( NULL, Row );

		Row = Widgets.Next( Row );
	}

	Widgets.Init();
}


#ifdef NSXPCM__FBL

void nsxpcm::Convert(
	const strings_ &Items,
	fblfrd::id32s_ &Ids )
{
	sdr::row__ Row = Items.First();
	sdr::row__ Error = E_NIL;
	ctn::E_CMITEM( string_ ) Item;

	Item.Init( Items );

	while( Row != E_NIL ) {
		Ids.Append( Item( Row ).ToU32( &Error ) );

		Row = Items.Next( Row );
	}

	if ( Error != E_NIL )
		ERRDta();
}

void nsxpcm::Convert(
	const strings_ &Items,
	fblfrd::id16s_ &Ids )
{
	sdr::row__ Row = Items.First();
	sdr::row__ Error = E_NIL;
	ctn::E_CMITEM( string_ ) Item;

	Item.Init( Items );

	while( Row != E_NIL ) {
		Ids.Append( Item( Row ).ToU16( &Error ) );

		Row = Items.Next( Row );
	}

	if ( Error != E_NIL )
		ERRDta();
}

void nsxpcm::Convert(
	const strings_ &Items,
	fblfrd::id8s_ &Ids )
{
	sdr::row__ Row = Items.First();
	sdr::row__ Error = E_NIL;
	ctn::E_CMITEM( string_ ) Item;

	Item.Init( Items );

	while( Row != E_NIL ) {
		Ids.Append( Item( Row ).ToU8( &Error ) );

		Row = Items.Next( Row );
	}

	if ( Error != E_NIL )
		ERRDta();
}

void nsxpcm::SplitAndConvert(
	const char *Joined,
	bso::char__ Separator,
	fblfrd::id32s_ &Ids )
{
ERRProlog
	strings Splitted;
ERRBegin
	Splitted.Init();

	Split( Joined, Separator, Splitted );

	Convert( Splitted, Ids );
ERRErr
ERREnd
ERREpilog
}

void nsxpcm::SplitAndConvert(
	const char *Joined,
	bso::char__ Separator,
	fblfrd::id16s_ &Ids )
{
ERRProlog
	strings Splitted;
ERRBegin
	Splitted.Init();

	Split( Joined, Separator, Splitted );

	Convert( Splitted, Ids );
ERRErr
ERREnd
ERREpilog
}

void nsxpcm::SplitAndConvert(
	const char *Joined,
	bso::char__ Separator,
	fblfrd::id8s_ &Ids )
{
ERRProlog
	strings Splitted;
ERRBegin
	Splitted.Init();

	Split( Joined, Separator, Splitted );

	Convert( Splitted, Ids );
ERRErr
ERREnd
ERREpilog
}

template< typename t> inline static const char *Convert_(
	t Value,
	bso::integer_buffer__ &Buffer )
{
	if ( Value == E_NIL )
		return "";
	else
		return bso::Convert( Value, Buffer );
}

template<typename id__, typename id_t__> static inline void ConvertAndJoin_(
	const bch::E_BUNCH_( id__ ) &RawIds,
	bso::char__ Separator,
	string_ &Joined )
{
	const bch::E_BUNCH_( id_t__ ) &Ids = *(const bch::E_BUNCH_( id_t__ ) *)&RawIds;
	sdr::row__ Row = Ids.First();
	bso::integer_buffer__ Buffer;

	if ( Row != E_NIL ) {
		Joined.Append( Convert_( Ids( Row ), Buffer ) );
		Row = Ids.Next( Row );
	}

	while ( Row != E_NIL ) {
		Joined.Append( Separator );
		Joined.Append( Convert_( Ids( Row ), Buffer ) );
		Row = Ids.Next( Row );
	}
}

template<typename id__, typename id_t__> static inline void ConvertJoinAndTransform_(
	const bch::E_BUNCH_( id__ ) &Ids,
	bso::char__ Separator,
	char **JString )
{
ERRProlog
	string Joined;
ERRBegin
	Joined.Init();

	ConvertAndJoin_<id__, id_t__>( Ids, Separator, Joined );

	Transform( Joined, JString );
ERRErr
ERREnd
ERREpilog
}

void nsxpcm::ConvertAndJoin(
	const fblfrd::id8s_ &Ids,
	bso::char__ Separator,
	string_ &Joined )
{
	ConvertAndJoin_<fblfrd::id8__, fblfrd::id8_t__>( Ids, Separator, Joined );
}

void nsxpcm::ConvertJoinAndTransform(
	const fblfrd::id8s_ &Ids,
	bso::char__ Separator,
	char **JString )
{
	ConvertJoinAndTransform_<fblfrd::id8__, fblfrd::id8_t__>( Ids, Separator, JString );
}

void nsxpcm::ConvertAndJoin(
	const fblfrd::id16s_ &Ids,
	bso::char__ Separator,
	string_ &Joined )
{
	ConvertAndJoin_<fblfrd::id16__, fblfrd::id16_t__>( Ids, Separator, Joined );
}

void nsxpcm::ConvertJoinAndTransform(
	const fblfrd::id16s_ &Ids,
	bso::char__ Separator,
	char **JString )
{
	ConvertJoinAndTransform_<fblfrd::id16__, fblfrd::id16_t__>( Ids, Separator, JString );
}

void nsxpcm::ConvertAndJoin(
	const fblfrd::id32s_ &Ids,
	bso::char__ Separator,
	string_ &Joined )
{
	ConvertAndJoin_<fblfrd::id32__, fblfrd::id32_t__>( Ids, Separator, Joined );
}

void nsxpcm::ConvertJoinAndTransform(
	const fblfrd::id32s_ &Ids,
	bso::char__ Separator,
	char **JString )
{
	ConvertJoinAndTransform_<fblfrd::id32__, fblfrd::id32_t__>( Ids, Separator, JString );
}

void nsxpcm::ConvertAndJoin(
	const fblfrd::sints_ &Ids,
	bso::char__ Separator,
	string_ &Joined )
{
	ConvertAndJoin_<fblfrd::sint__, fblfrd::sint__>( Ids, Separator, Joined );
}

void nsxpcm::ConvertJoinAndTransform(
	const fblfrd::sints_ &Ids,
	bso::char__ Separator,
	char **JString )
{
	ConvertJoinAndTransform_<fblfrd::sint__, fblfrd::sint__>( Ids, Separator, JString );
}

void nsxpcm::ConvertAndJoin(
	const fblfrd::uints_ &Ids,
	bso::char__ Separator,
	string_ &Joined )
{
	ConvertAndJoin_<fblfrd::uint__, fblfrd::uint__>( Ids, Separator, Joined );
}

void nsxpcm::ConvertJoinAndTransform(
	const fblfrd::uints_ &Ids,
	bso::char__ Separator,
	char **JString )
{
	ConvertJoinAndTransform_<fblfrd::uint__, fblfrd::uint__>( Ids, Separator, JString );
}


void nsxpcm::ConvertAndJoin(
	const fblfrd::booleans_ &Booleans,
	bso::char__ Separator,
	string_ &Joined )
{
	sdr::row__ Row = Booleans.First();

	if ( Row != E_NIL ) {
		Joined.Append( Booleans( Row ) ? "1" : "0" );
		Row = Booleans.Next( Row );
	}

	while ( Row != E_NIL ) {
		Joined.Append( Separator );
		Joined.Append( Booleans( Row ) ? "1" : "0" );
		Row = Booleans.Next( Row );
	}
}

void nsxpcm::ConvertJoinAndTransform(
	const fblfrd::booleans_ &Booleans,
	bso::char__ Separator,
	char **JString )
{
ERRProlog
	string Joined;
ERRBegin
	Joined.Init();

	ConvertAndJoin( Booleans, Separator, Joined );

	Transform( Joined, JString );
ERRErr
ERREnd
ERREpilog
}

void GetData_(
	bso::uint__ Amount,
	const strings_ &Splitted,
	strings_ &Data,
	sdr::row__ &Row )
{
	ctn::E_CMITEM( str::string_ ) Item;

	Item.Init( Splitted );

	while ( Amount-- ) {
#ifdef NSXPCM_DBG
		if ( Row == E_NIL )
			ERRFwk();
#endif
		Data.Append( Item( Row ) );

		Row = Splitted.Next( Row );
	}
}

void nsxpcm::Split(
	const char *Joined,
	const char *JAmounts,
	bso::char__ Separator,
	fblfrd::xstrings_ &DataCluster )
{
ERRProlog
	nsxpcm::strings Amounts;
	nsxpcm::strings Splitted;
	sdr::row__ Error = E_NIL;
	sdr::row__ ARow = E_NIL, SRow = E_NIL;
	strings Data;
ERRBegin
	Splitted.Init();
	Split( Joined, Separator, Splitted );

	Amounts.Init();
	Split( JAmounts, Separator, Amounts );

	ARow = Amounts.First();

	SRow = Splitted.First();

	while ( ARow != E_NIL )
	{
		Data.Init();

		GetData_( Amounts( ARow ).ToUInt( &Error ), Splitted, Data, SRow );

		ARow = Amounts.Next( ARow );
	}
ERRErr
ERREnd
ERREpilog
}
#endif

template <typename t> static void Alert_(
	nsIDOMWindow *Window,
	t Text )
{
ERRProlog
	nsString WString;
ERRBegin
	Transform( Text, WString );

	T( GetWindowInternal( Window )->Alert( WString ) );
ERRErr
ERREnd
ERREpilog
}

void nsxpcm::Alert(
	nsIDOMWindow *Window,
	const char *Text )
{
	Alert_<const char *>( Window, Text );
}

void nsxpcm::Alert(
	nsIDOMWindow *Window,
	const str::string_ &Text )
{
	Alert_<const str::string_ &>( Window, Text );
}

template <typename t> bso::bool__ Confirm_(
	nsIDOMWindow *Window,
	t Text )
{
	NSXPCM__BOOL Answer = 0;
ERRProlog
	nsString WString;
ERRBegin
	Transform( Text, WString );

	T( GetWindowInternal( Window )->Confirm( WString, &Answer ) );
ERRErr
ERREnd
ERREpilog
	return Answer != 0;
}

bso::bool__ nsxpcm::Confirm(
	nsIDOMWindow *Window,
	const char *Text )
{
	return Confirm_<const char *>( Window, Text );
}

bso::bool__ nsxpcm::Confirm(
	nsIDOMWindow *Window,
	const str::string_ &Text )
{
	return Confirm_<const str::string_ &>( Window, Text );
}


void nsxpcm::Log( const str::string_ &Text )
{
ERRProlog
	nsString WString;
	nsCOMPtr<nsIConsoleService> ConsoleService = NULL;
	str::string StampedText;
	tol::buffer__ Buffer;
ERRBegin
	StampedText.Init( "[" );
	StampedText.Append( tol::DateAndTime( Buffer ) );
	StampedText.Append( "]\n" );
	StampedText.Append( Text );

	nsxpcm::GetService<nsIConsoleService>( NS_CONSOLESERVICE_CONTRACTID, ConsoleService );

	Transform( StampedText, WString );

	T( ConsoleService->LogStringMessage( WString.get() ) );
ERRErr
ERREnd
ERREpilog
}

void nsxpcm::event_handler__::_Test( bso::bool__ IgnoreInitializationTest )
{
	if ( _Control != this )
		ERRFwk();

	if ( !IgnoreInitializationTest ) {
		if ( _EventData._EventListener.get() == NULL )
			ERRFwk();

		if ( !_EventData._EventListener->IsInitialized() )
			ERRFwk();
	}
}

void nsxpcm::event_handler__::Init( void )
{
	_Test( true );

	reset();

	nsxpcm::CreateInstance( NSXPCM_EVENT_LISTENER_CONTRACTID, _EventData._EventListener );

	_EventData._EventListener->Init( *this );
}


void nsxpcm::event_handler__::Add(
	nsISupports *Supports,
	int Events )
{
	_Test( false );

	nsIDOMEventTarget *EventTarget = nsxpcm::QueryInterface<nsIDOMEventTarget>( Supports );

	if ( Events & efCommand )
		if ( EventTarget->AddEventListener( NS_LITERAL_STRING( "command" ), _EventData._EventListener, false ) != NS_OK )
			ERRFwk();

	if ( Events & efInput )
		if ( EventTarget->AddEventListener( NS_LITERAL_STRING( "input" ), _EventData._EventListener, false ) != NS_OK )
			ERRFwk();

	if ( Events & efChange )
		if ( EventTarget->AddEventListener( NS_LITERAL_STRING( "change" ), _EventData._EventListener, false ) != NS_OK )
			ERRFwk();

	if ( Events & efClick )
		if ( EventTarget->AddEventListener( NS_LITERAL_STRING( "click" ), _EventData._EventListener, false ) != NS_OK )
			ERRFwk();

	if ( Events & efDblClick )
		if ( EventTarget->AddEventListener( NS_LITERAL_STRING( "dblclick" ), _EventData._EventListener, false ) != NS_OK )
			ERRFwk();

	if ( Events & efFocus )
		if ( EventTarget->AddEventListener( NS_LITERAL_STRING( "focus" ), _EventData._EventListener, false ) != NS_OK )
			ERRFwk();

	if ( Events & efBlur )
		if ( EventTarget->AddEventListener( NS_LITERAL_STRING( "blur" ), _EventData._EventListener, false ) != NS_OK )
			ERRFwk();

	if ( Events & efSelect )
		if ( EventTarget->AddEventListener( NS_LITERAL_STRING( "select" ), _EventData._EventListener, false ) != NS_OK )
			ERRFwk();

	if ( Events & efAttributeChange )
		if ( EventTarget->AddEventListener( NS_LITERAL_STRING( "DOMAttrModified" ), _EventData._EventListener, false ) != NS_OK )
			ERRFwk();

	if ( Events & efKeyPress )
		if ( EventTarget->AddEventListener( NS_LITERAL_STRING( "keypress" ), _EventData._EventListener, false ) != NS_OK )
			ERRFwk();

	if ( Events & efLoad )
		if ( EventTarget->AddEventListener( NS_LITERAL_STRING( "load" ), _EventData._EventListener, false ) != NS_OK )
			ERRFwk();

	if ( Events & efClose )
		if ( EventTarget->AddEventListener( NS_LITERAL_STRING( "close" ), _EventData._EventListener, false ) != NS_OK )
			ERRFwk();
}

static event__ Convert_(
	const char *RawEvent,
	bso::bool__ WithOnPrefix )
{
	event__ Event = e_Undefined;

	if ( WithOnPrefix )
		if ( strncmp( RawEvent, "on", 2 ) )
			return Event;
		else
			RawEvent += 2;

	if ( !strcmp( RawEvent, "command" ) )
		Event = eCommand;
	else if ( !strcmp( RawEvent, "input" ) )
		Event = eInput;
	else if ( !strcmp( RawEvent, "change" ) )
		Event = eChange;
	else if ( !strcmp( RawEvent, "click" ) )
		Event = eClick;
	else if ( !strcmp( RawEvent, "dblclick" ) )
		Event = eDblClick;
	else if ( !strcmp( RawEvent, "focus" ) )
		Event = eFocus;
	else if ( !strcmp( RawEvent, "blur" ) )
		Event = eBlur;
	else if ( !strcmp( RawEvent, "select" ) )
		Event = eSelect;
	else if ( !strcmp( RawEvent, "DOMAttrModified" ) )
		Event = eAttributeChange;
	else if ( !strcmp( RawEvent, "keypress" ) )
		Event = eKeyPress;
	else if ( !strcmp( RawEvent, "load" ) )
		Event = eLoad;
	else if ( !strcmp( RawEvent, "close" ) )
		Event = eClose;

	return Event;
}

bso::bool__ nsxpcm::event_handler__::Handle( nsIDOMEvent *RawEvent )
{
	_Test( false );

	bso::bool__ Success = false;
ERRProlog
	nsString WString;
	str::string EventString;
	event__ Event = e_Undefined;
	TOL_CBUFFER___ StrBuffer;
ERRBegin
	// Sauvegarde pour la gestion d'vnements imbriqus.
	nsIDOMEvent *RawEventBuffer = _EventData._RawEvent;
	nsIDOMMutationEvent *MutationEventBuffer = _EventData._MutationEvent;
	nsIDOMKeyEvent *KeyEventBuffer = _EventData._KeyEvent;

	_EventData._RawEvent = RawEvent;

	T( RawEvent->GetType( WString ) );

	EventString.Init();

	Transform( WString, EventString );

	Event = Convert_( EventString.Convert( StrBuffer ), false );

	if ( Event == e_Undefined )
		ERRLmt();

	if ( EventString == "DOMAttrModified" )
		_EventData._MutationEvent = QueryInterface<nsIDOMMutationEvent>( RawEvent );

	if ( EventString == "keypress" )
		_EventData._KeyEvent = QueryInterface<nsIDOMKeyEvent> ( RawEvent );

	if ( _EventData._EventImbricationLevel++ == NSXPCM__EVENT_IMBRICATION_LEVEL_MAX )
		ERRLmt();

	if ( !( _EventData._EventsToIgnore & ( 1 << Event ) ) )
/*		if ( _EventHandler != NULL )
			_EventHandler->OnEvent( _EventData, Event );
		else
	*/		_OnEvent( _EventData, Event );

	if ( _EventData._EventImbricationLevel-- < -1 )
		ERRFwk();

	_EventData._RawEvent = RawEventBuffer;
	_EventData._MutationEvent = MutationEventBuffer;
	_EventData._KeyEvent = KeyEventBuffer;

	Success = true;
ERRErr
	if ( ERRType == err::t_Abort ) 
		Success = true;	// L'erreur a t dj traite (ou va l'tre ci-dessous) ; plus la peine de la remonter.
//	NSXPCM_ERR( _Window );
ERREnd
ERREpilog
	return Success;
}

static event__ GetEventIfConcerned_(
	nsIDOMNode *Node,
	const char *Id,
	const char *NameSpace )
{
	event__ Event = e_Undefined;
ERRProlog
	nsString RawName, RawValue;
	str::string Name, Value;
	TOL_CBUFFER___ Buffer;
	str::string NameSpaceWithSeparator;
ERRBegin
	nsIDOMAttr *Attribute = nsxpcm::QueryInterface<nsIDOMAttr>( Node );

	T( Attribute->GetName( RawName ) );

	Name.Init();
	Transform( RawName, Name );

	NameSpaceWithSeparator.Init( NameSpace );
	NameSpaceWithSeparator.Append( ':' );

	if ( Name.Amount() < NameSpaceWithSeparator.Amount() )
		ERRReturn;

	if ( str::Compare( Name, NameSpaceWithSeparator, Name.First(), NameSpaceWithSeparator.First(), NameSpaceWithSeparator.Amount() ) != 0 )
		ERRReturn;

	Name.Remove( Name.First(), NameSpaceWithSeparator.Amount() );

	Event = Convert_( Name.Convert( Buffer ), true );

	T( Attribute->GetValue( RawValue ) );

	Value.Init();
	Transform ( RawValue, Value );

	if ( Value != Id )
		Event = e_Undefined;
ERRErr
ERREnd
ERREpilog
	return Event;
}

#include "nsIAutoCompleteInput.h"
#include "nsIAutoCompletePopup.h"

static nsISupports *Patch_( nsIDOMNode *Node )
{
	nsISupports *Supports = Node;
ERRProlog
	nsString RawName;
	str::string Name;
	nsIDOMDocument *Document = NULL;
	nsIAutoCompleteController *Controller = NULL;
	nsIAutoCompletePopup *Popup = NULL;
ERRBegin
	T( Node->GetNodeName( RawName ) );

	Name.Init();
	Transform( RawName, Name );

	if ( Name == "window" )
		ERRLmt();	// J'ignore comment,  partir de cet lment, rcuprer un lment sur lequel un gestionnaire d'vnement soit actif.
	else if ( Name == "textbox" ) {
		// Supports = nsxpcm::QueryInterface<nsIAutoCompleteInput>( Node );
		// Supports = Popup;
		Supports = Supports;
	}

/*	else if ( ( Name == "a" ) || ( Name == "A" ) )
		Supports = nsxpcm::QueryInterface<nsIDOMHTMLAnchorElement>( Node );
*/
ERRErr
ERREnd
ERREpilog
	return Supports;
}

static void AttachIfConcerned_(
	nsIDOMNode *Node,
	nsIDOMMozNamedAttrMap *Attributes,
	const char *Id,
	event_handler__ &EventHandler,
	const char *NameSpace )
{
	PRUint32 Amount;
	nsIDOMAttr *AttributeNode = NULL;
	event__ Event = e_Undefined;
	
	T( Attributes->GetLength( &Amount ) );

	while ( Amount-- != 0 )  {
		T( Attributes->Item( Amount, &AttributeNode ) );

		if ( ( Event = GetEventIfConcerned_( AttributeNode, Id, NameSpace ) ) != e_Undefined )
			EventHandler.Add( Patch_( Node ), 1 << Event );
	}
}

void nsxpcm::AttachEventHandler(
	nsIDOMElement *Element,
	const char *Id,
	event_handler__ &EventHandler,
	const char *NameSpace )
{
ERRProlog
	nsxpcm::browser__ Browser;
	nsIDOMMozNamedAttrMap *Attributes = NULL;
	nsIDOMNode *Node = NULL;
ERRBegin
	Node = Element;

	Browser.Init( Element );

	while ( Node != NULL ) {
		T( GetElement( Node )->GetAttributes( &Attributes ) );

		if ( Attributes != NULL )
			AttachIfConcerned_( Node, Attributes, Id, EventHandler, NameSpace );

		Node = Browser.GetNext();
	}
ERRErr
ERREnd
ERREpilog
}

bso::bool__ GetRelevantBroadcasterId_(
	const str::string_ &CustomizedObserverAttributeName,
	nsIDOMNode *AttributeNode,
	str::string_ &ObserverId )
{
	bso::bool__ Relevant = false;
ERRProlog
	nsString RawName, RawValue;
	str::string Name;
	TOL_CBUFFER___ Buffer;
ERRBegin
	nsIDOMAttr *Attribute = nsxpcm::QueryInterface<nsIDOMAttr>( AttributeNode );

	T( Attribute->GetName( RawName ) );

	Name.Init();
	Transform( RawName, Name );

	if ( Name != CustomizedObserverAttributeName )
		ERRReturn;

	Relevant = true;

	T( Attribute->GetValue( RawValue ) );

	Transform ( RawValue, ObserverId );
ERRErr
ERREnd
ERREpilog
	return Relevant;
}

bso::bool__ GetRelevantBroadcasterId_(
	const str::string_ &CustomizedObserverAttributeName,
	nsIDOMNode *Node,
	nsIDOMMozNamedAttrMap *Attributes,
	str::string_ &ObserverId )
{
	PRUint32 Amount;
	nsIDOMAttr *AttributeNode = NULL;
	event__ Event = e_Undefined;
	bso::bool__ Relevant = false;
	
	T( Attributes->GetLength( &Amount ) );

	while ( !Relevant && ( Amount-- != 0 ) ) {
		Attributes->Item( Amount, &AttributeNode );

		Relevant = GetRelevantBroadcasterId_( CustomizedObserverAttributeName, AttributeNode, ObserverId );
	}

	return Relevant;
}

static inline void RefreshObserver_(
	nsIDOMNode *Node,
	const str::string_ &ObserverId )
{
	nsxpcm::RemoveAttribute( Node , "observes" );
	nsxpcm::SetAttribute( Node, "observes", ObserverId );
}

/*
	Comme certains 'broadcasters' sont gnrs dynamiquement,  l'aide d'une transformation 'XSL',
	il est ncessaire de rafrachir les attributs 'observes' des lments qui s'y rfrenet,
	sinon les modifications de seont pas prises en comptes.
*/

void nsxpcm::RefreshObservers(
	nsIDOMElement *Element,
	const char *NameSpace )
{
ERRProlog
	nsxpcm::browser__ Browser;
	nsIDOMMozNamedAttrMap *Attributes = NULL;
	str::string CustomizedObserverAttributeName;
	str::string ObserverId;
ERRBegin
	CustomizedObserverAttributeName.Init();
	CustomizedObserverAttributeName.Append( NameSpace );
	CustomizedObserverAttributeName.Append( ":observes" );

	Browser.Init( Element );

	while ( Element != NULL ) {
		Element->GetAttributes( &Attributes ); 

		if ( Attributes != NULL ) {
			ObserverId.Init();

			if ( GetRelevantBroadcasterId_( CustomizedObserverAttributeName, Element, Attributes, ObserverId ) )
				RefreshObserver_( Element, ObserverId );
		}

		Element = Browser.GetNext();
	}
ERRErr
ERREnd
ERREpilog
}

/* Dbut 'event_listener__' */

NS_IMPL_ISUPPORTS1(nsxpcm::event_listener__, nsxpcm::event_listener__ );

NS_IMETHODIMP nsxpcm::event_listener__::HandleEvent(nsIDOMEvent *Event)
{
	nsresult NSResult = NS_OK;

	if ( !_EventHandler->Handle( Event ) )
		NSResult = NS_ERROR_FAILURE;

    return NSResult;
}

/* Fin 'event_listener__' */
/*
NS_GENERIC_FACTORY_CONSTRUCTOR(tree_view__)

NS_DEFINE_STATIC_IID_ACCESSOR(tree_view__, NSXPCM_ITREE_VIEW_IID)

NS_IMPL_ISUPPORTS0( tree_view__ )
*/

void nsxpcm::tree__::_SetTreeView( class tree_view_callback__ &Callback )
{
	CreateInstance( NSXPCM_TREE_VIEW_CONTRACTID, _TreeView );

	_TreeView->Init( Callback );

	T( GetWidget()->SetView( _TreeView ) );
}

/* Dbut 'tree_view__' */

NS_IMPL_ISUPPORTS2(nsxpcm::tree_view__, nsxpcm::tree_view__, nsITreeView );

NS_IMETHODIMP nsxpcm::tree_view__::GetRowCount(PRInt32* aRowCount)
{
	*aRowCount = this->_C().GetRowCount();

	return NS_OK;
}

NS_IMETHODIMP nsxpcm::tree_view__::GetSelection(nsITreeSelection** aSelection)
{
  *aSelection = mSelection;
  NS_IF_ADDREF(*aSelection);

  return NS_OK;
}

NS_IMETHODIMP nsxpcm::tree_view__::SetSelection(nsITreeSelection* aSelection)
{
  mSelection = aSelection;

  return NS_OK;
}

NS_IMETHODIMP nsxpcm::tree_view__::GetRowProperties(int32_t index, nsAString & _retval)
{
  return NS_OK;
}

NS_IMETHODIMP nsxpcm::tree_view__::GetCellProperties(int32_t row, nsITreeColumn *col, nsAString & _retval)
{
#if 0
  PRUint32 dirCount;
  mDirList->Count(&dirCount);

  if (aRow < (PRInt32) dirCount)
    aProperties->AppendElement(mDirectoryAtom);
  else if (aRow < mTotalRows)
    aProperties->AppendElement(mFileAtom);
#endif
  return NS_OK;
}

NS_IMETHODIMP nsxpcm::tree_view__::GetColumnProperties(nsITreeColumn *col, nsAString & _retval)
{
  return NS_OK;
}

NS_IMETHODIMP nsxpcm::tree_view__::IsContainer(PRInt32 aIndex, NSXPCM__BOOL* aIsContainer)
{
  *aIsContainer = false;
  return NS_OK;
}

NS_IMETHODIMP nsxpcm::tree_view__::IsContainerOpen(PRInt32 aIndex, NSXPCM__BOOL* aIsOpen)
{
  *aIsOpen = false;
  return NS_OK;
}

NS_IMETHODIMP nsxpcm::tree_view__::IsContainerEmpty(PRInt32 aIndex, NSXPCM__BOOL* aIsEmpty)
{
  *aIsEmpty = false;
  return NS_OK;
}

NS_IMETHODIMP nsxpcm::tree_view__::IsSeparator(PRInt32 aIndex, NSXPCM__BOOL* aIsSeparator)
{
  *aIsSeparator = false;
  return NS_OK;
}

NS_IMETHODIMP nsxpcm::tree_view__::IsSorted(NSXPCM__BOOL* aIsSorted)
{
//  *aIsSorted = (mSortType >= 0);
  return NS_OK;
}

NS_IMETHODIMP nsxpcm::tree_view__::CanDrop(PRInt32 aIndex, PRInt32 aOrientation,
                    nsIDOMDataTransfer* dataTransfer, NSXPCM__BOOL* aCanDrop)
{
  *aCanDrop = false;
  return NS_OK;
}

NS_IMETHODIMP nsxpcm::tree_view__::Drop(PRInt32 aRow, PRInt32 aOrientation, nsIDOMDataTransfer* dataTransfer)
{
  return NS_OK;
}

NS_IMETHODIMP nsxpcm::tree_view__::GetParentIndex(PRInt32 aRowIndex, PRInt32* aParentIndex)
{
  *aParentIndex = -1;
  return NS_OK;
}

NS_IMETHODIMP nsxpcm::tree_view__::HasNextSibling(PRInt32 aRowIndex, PRInt32 aAfterIndex, 
                           NSXPCM__BOOL* aHasSibling)
{
//  *aHasSibling = (aAfterIndex < (mTotalRows - 1));
  return NS_OK;
}

NS_IMETHODIMP nsxpcm::tree_view__::GetLevel(PRInt32 aIndex, PRInt32* aLevel)
{
  *aLevel = 0;
  return NS_OK;
}

NS_IMETHODIMP nsxpcm::tree_view__::GetImageSrc(PRInt32 aRow, nsITreeColumn* aCol,
                        nsAString& aImageSrc)
{
  return NS_OK;
}

NS_IMETHODIMP nsxpcm::tree_view__::GetProgressMode(PRInt32 aRow, nsITreeColumn* aCol,
                            PRInt32* aProgressMode)
{
  return NS_OK;
}

NS_IMETHODIMP nsxpcm::tree_view__::GetCellValue(PRInt32 aRow, nsITreeColumn* aCol,
                         nsAString& aCellValue)
{
  return NS_OK;
}

NS_IMETHODIMP nsxpcm::tree_view__::GetCellText(
	PRInt32 aRow,
	nsITreeColumn* aCol,
	nsAString& aCellText)
{
 ERRProlog
	str::string Text;
	PRInt32 Index = 0;
 ERRBegin
	 aCol->GetIndex( &Index );

	Text.Init();
	 _C().GetCellText( aRow, Index, Text );
	 Transform( Text, aCellText );
ERRErr
ERREnd
ERREpilog
  return NS_OK;
}

NS_IMETHODIMP nsxpcm::tree_view__::SetTree(nsITreeBoxObject* aTree)
{
  mTree = aTree;
  return NS_OK;
}

NS_IMETHODIMP nsxpcm::tree_view__::ToggleOpenState(PRInt32 aIndex)
{
  return NS_OK;
}

NS_IMETHODIMP nsxpcm::tree_view__::CycleHeader(nsITreeColumn* aCol)
{
  return NS_OK;
}

NS_IMETHODIMP nsxpcm::tree_view__::SelectionChanged()
{
  return NS_OK;
}

NS_IMETHODIMP nsxpcm::tree_view__::CycleCell(PRInt32 aRow, nsITreeColumn* aCol)
{
  return NS_OK;
}

NS_IMETHODIMP nsxpcm::tree_view__::IsEditable(PRInt32 aRow, nsITreeColumn* aCol,
                       NSXPCM__BOOL* aIsEditable)
{
  *aIsEditable = false;
  return NS_OK;
}

NS_IMETHODIMP nsxpcm::tree_view__::IsSelectable(PRInt32 aRow, nsITreeColumn* aCol,
                        NSXPCM__BOOL* aIsSelectable)
{
  *aIsSelectable = false;
  return NS_OK;
}

NS_IMETHODIMP nsxpcm::tree_view__::SetCellValue(PRInt32 aRow, nsITreeColumn* aCol,
                         const nsAString& aValue)
{
  return NS_OK;
}


NS_IMETHODIMP nsxpcm::tree_view__::SetCellText(PRInt32 aRow, nsITreeColumn* aCol,
                        const nsAString& aValue)
{
  return NS_OK;
}

NS_IMETHODIMP nsxpcm::tree_view__::PerformAction(const char16_t* aAction)
{
  return NS_OK;
}

NS_IMETHODIMP nsxpcm::tree_view__::PerformActionOnRow(const char16_t* aAction, PRInt32 aRow)
{
  return NS_OK;
}

NS_IMETHODIMP nsxpcm::tree_view__::PerformActionOnCell(const char16_t* aAction, PRInt32 aRow,
                                nsITreeColumn* aCol)
{
  return NS_OK;
}

/* Fin 'tree_view' */

/* Dbut 'textbox' 'autocomplete' */

NS_IMPL_ISUPPORTS2 ( nsxpcm::autocomplete_result___, nsxpcm::autocomplete_result___, nsIAutoCompleteResult );

NS_IMETHODIMP nsxpcm::autocomplete_result___::GetSearchString(nsAString & aSearchString)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP nsxpcm::autocomplete_result___::GetSearchResult(PRUint16 *aSearchResult)
{
	*aSearchResult = nsIAutoCompleteResult::RESULT_SUCCESS;

    return NS_OK;
}

NS_IMETHODIMP nsxpcm::autocomplete_result___::GetDefaultIndex(PRInt32 *aDefaultIndex)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP nsxpcm::autocomplete_result___::GetErrorDescription(nsAString & aErrorDescription)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP nsxpcm::autocomplete_result___::GetMatchCount(PRUint32 *aMatchCount)
{
	*aMatchCount = _C().GetMatchingCount( GetSearchString() );

    return NS_OK;
}

# define NS_OUTPARAM

NS_IMETHODIMP nsxpcm::autocomplete_result___::GetValueAt(PRInt32 index, nsAString & _retval NS_OUTPARAM)
{
ERRProlog
	str::string Value;
ERRBegin
	Value.Init();

	_C().GetValue( GetSearchString(), index, Value );

	Transform( Value , _retval );
ERRErr
ERREnd
ERREpilog
    return NS_OK;
}

NS_IMETHODIMP nsxpcm::autocomplete_result___::GetLabelAt(PRInt32 index, nsAString & _retval NS_OUTPARAM)
{
ERRProlog
	str::string Label;
ERRBegin
	Label.Init();

	_C().GetLabel( GetSearchString(), index, Label );

	Transform( Label , _retval );
ERRErr
ERREnd
ERREpilog
    return NS_OK;
}

NS_IMETHODIMP nsxpcm::autocomplete_result___::GetCommentAt(PRInt32 index, nsAString & _retval NS_OUTPARAM)
{
ERRProlog
	str::string Comment;
ERRBegin
	Comment.Init();

	_C().GetComment( GetSearchString(), index, Comment );

	Transform( Comment , _retval );
ERRErr
ERREnd
ERREpilog
    return NS_OK;
}

NS_IMETHODIMP nsxpcm::autocomplete_result___::GetStyleAt(PRInt32 index, nsAString & _retval NS_OUTPARAM)
{
    return NS_OK;
}

NS_IMETHODIMP nsxpcm::autocomplete_result___::GetImageAt(PRInt32 index, nsAString & _retval NS_OUTPARAM)
{
    return NS_OK;
}

NS_IMETHODIMP nsxpcm::autocomplete_result___::RemoveValueAt(PRInt32 rowIndex, NSXPCM__BOOL removeFromDb)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP nsxpcm::autocomplete_result___::GetTypeAheadResult(bool *aTypeAheadResult)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}


/* Fin 'textbox' 'atutocomplete'. */

NS_IMPL_ISUPPORTS1(nsxpcm::clh__, nsxpcm::iclh__)

void nsxpcm::xslt_parameters_::Append(
	const str::string_ &Name,
	const str::string_ &Value )
{
ERRProlog
	xslt_parameter Parameter;
ERRBegin
	Parameter.Init();

	Parameter.Name = Name;
	Parameter.Value = Value;

	_xslt_parameters_::Append( Parameter );
ERRErr
ERREnd
ERREpilog
}


nsIDOMDocumentFragment *nsxpcm::XSLTransform(
	nsIDOMDocument *XMLDocument,
	nsIDOMDocument *XSLStylesheet,
	nsIDOMWindow *Owner,
	const xslt_parameters_ &Parameters )
{
	nsIDOMDocumentFragment *Fragment = NULL;
ERRProlog
	nsCOMPtr<nsIXSLTProcessor> Processor;
	nsString Name;
	nsCOMPtr<nsIWritableVariant> Value;
	TOL_CBUFFER___ NameBuffer;
	ctn::E_CITEM( xslt_parameter_ ) Parameter;
	nsresult Result = NS_OK;
	sdr::row__ Row = E_NIL;
ERRBegin
	nsxpcm::CreateInstance( "@mozilla.org/document-transformer;1?type=xslt", Processor );

	Result = Processor->ImportStylesheet( XSLStylesheet );

	if ( Result != NS_OK )
		ERRLbr();

	Parameter.Init( Parameters );

	Row = Parameters.First();

	while ( Row != E_NIL ) {
		Transform( Parameter( Row ).Name, Name );

		nsxpcm::CreateInstance( NS_VARIANT_CONTRACTID, Value );
		T( Value->SetAsString( Parameter( Row ).Value.Convert( NameBuffer ) ) );

		Result = Processor->SetParameter( NS_LITERAL_STRING( "" ), Name, Value );

		if ( Result != NS_OK )
			ERRLbr();

		Row = Parameters.Next( Row );
	}


	Result = Processor->TransformToFragment( XMLDocument, GetDocument( Owner ), &Fragment );

	if ( Result != NS_OK )
		ERRLbr();
ERRErr
	Fragment = NULL;
ERREnd
ERREpilog
	return Fragment;
}

static void Preprocess_(
	xtf::extended_text_iflow__ XFlow,
	const str::string_ &BaseDirectory,
	str::string_ &ProcessedXMLString )
{
ERRProlog
	flx::E_STRING_OFLOW___ SFlow;
	txf::text_oflow__ TFlow;
ERRBegin
	SFlow.Init( ProcessedXMLString );
	TFlow.Init( SFlow );

	if ( xpp::Process( XFlow, BaseDirectory, xml::oCompact, TFlow ) != xpp::sOK )
		ERRDta();
ERRErr
ERREnd
ERREpilog
}

static bso::bool__ _GetXMLDocument(
	const str::string_ &XMLString,
	nsIDOMDocument *&XMLDocument )
{
	nsresult Result = NS_OK;

	nsString WString;
	nsCOMPtr<nsIDOMParser> Parser;

	Transform( XMLString, WString );
	
	CreateInstance( NS_DOMPARSER_CONTRACTID, Parser );
	Result = Parser->ParseFromString( WString.get(), "text/xml", &XMLDocument );

	return Result == NS_OK;
}

static bso::bool__ _GetXSLStylesheet(
	const str::string_ &XSLStylesheetFileName,
	nsIDOMDocument *&XSLStylesheet )
{
	nsresult Result = NS_OK;
	nsCString Method, URL;
	nsString Empty;
	nsCOMPtr<nsIXMLHttpRequest> HTTPRequest;

	Transform( "GET", Method );
	Transform( XSLStylesheetFileName, URL );

	CreateInstance( NS_XMLHTTPREQUEST_CONTRACTID, HTTPRequest );

	Result = HTTPRequest->Open( Method, URL, false, Empty, Empty );

	if ( Result != NS_OK )
		ERRLbr();

	Result = HTTPRequest->Send( NULL );

	if ( Result != NS_OK )
		ERRLbr();

	Result = HTTPRequest->GetResponseXML( &XSLStylesheet );

	if ( Result != NS_OK )
		ERRLbr();

	return true;
}

nsIDOMDocumentFragment *nsxpcm::XSLTransformByContent(
	const str::string_ &XMLString,
	const str::string_ &XSLString,
	nsIDOMWindow *Owner,
	const xslt_parameters_ &Parameters )
{
	nsIDOMDocumentFragment *Fragment = NULL;
ERRProlog
	nsIDOMDocument *XMLDocument = NULL, *XSLStylesheet = NULL;
ERRBegin
	if ( !_GetXMLDocument( XMLString, XMLDocument ) )
		ERRDta();

	if ( !_GetXMLDocument( XSLString, XSLStylesheet ) )
		ERRDta();

	Fragment = XSLTransform( XMLDocument, XSLStylesheet, Owner, Parameters );
ERRErr
	Fragment = NULL;
ERREnd
ERREpilog
	return Fragment;
}

static nsIDOMDocumentFragment *XSLTransformProvidingChromeXSLFileName_(
	const str::string_ &XMLString,
	const str::string_ &XSLFileName,
	nsIDOMWindow *Owner,
	const xslt_parameters_ &Parameters )
{
	nsIDOMDocumentFragment *Fragment = NULL;
ERRProlog
	nsIDOMDocument *XMLDocument = NULL, *XSLStylesheet = NULL;
ERRBegin
	if ( !_GetXMLDocument( XMLString, XMLDocument ) )
		ERRDta();

	if ( !_GetXSLStylesheet( XSLFileName, XSLStylesheet ) )
		ERRDta();

	Fragment = XSLTransform( XMLDocument, XSLStylesheet, Owner, Parameters );
ERRErr
	Fragment = NULL;
ERREnd
ERREpilog
	return Fragment;
}

static inline void Process_(
	const str::string_ &FileName,
	str::string_ &Result )
{
ERRProlog
	flx::E_STRING_OFLOW___ SFlow;
	txf::text_oflow__ TFlow;
	flf::file_iflow___ FFlow;
	xtf::extended_text_iflow__ XFlow;
	TOL_CBUFFER___ STRBuffer;
	TOL_CBUFFER___ FNMBuffer;
	fnm::name___ Buffer;
	const char *XSLLocation = NULL;
ERRBegin
	if ( FFlow.Init( FileName.Convert( STRBuffer ) ) != tol::rSuccess )
		ERRLmt();	// Devrait normalement faire remonter un message d'erreur explicatif.

	XFlow.Init( FFlow, utf::f_Default );

	Buffer.Init();
	XSLLocation = fnm::GetLocation( FileName.Convert( STRBuffer ), Buffer).UTF8( FNMBuffer );

	SFlow.Init( Result );
	TFlow.Init( SFlow );

	if ( xpp::Process( XFlow, str::string( XSLLocation ), xml::oCompact, TFlow ) != xpp::sOK )
		ERRDta();
ERRErr
ERREnd
ERREpilog
}

static inline nsIDOMDocumentFragment *XSLTransformProvidingXSLContent_(
	const str::string_ &XMLContent,
	const str::string_ &XSLFileName,
	nsIDOMWindow *Owner,
	const xslt_parameters_ &Parameters )
{
	nsIDOMDocumentFragment *Fragment = NULL;
ERRProlog
	str::string XSLContent;
ERRBegin
	XSLContent.Init();
	Process_( XSLFileName, XSLContent );

	Fragment = nsxpcm::XSLTransformByContent( XMLContent, XSLContent, Owner, Parameters );
ERRErr
ERREnd
ERREpilog
	return Fragment; 
}

nsIDOMDocumentFragment *nsxpcm::XSLTransformByFileName(
	const str::string_ &XMLContent,
	const str::string_ &XSLFileName,
	nsIDOMWindow *Owner,
	const xslt_parameters_ &Parameters )
{
	if ( ( XSLFileName.Amount() > 9 ) && ( !str::Compare( XSLFileName, str::string ("chrome://" ), 0, 0, 9 ) ) )
		return XSLTransformProvidingChromeXSLFileName_( XMLContent, XSLFileName, Owner, Parameters );
	else
		return XSLTransformProvidingXSLContent_( XMLContent, XSLFileName, Owner, Parameters );
}

nsIDOMElement *browser__::GetNext( void )
{
	if ( _Current != NULL ) {
		if ( _Current == _Root )
			_Current = GetFirstChild( _Root );
		else if ( GetFirstChild( _Current ) != NULL )
			_Current = GetFirstChild( _Current );
		else if ( GetNextSibling( _Current ) != NULL )
			_Current = GetNextSibling( _Current );
		else {
			do
				_Current = GetParentNode( _Current );
			while ( ( _Current != _Root ) && ( GetNextSibling( _Current ) == NULL ) );

			if ( _Current == _Root )
				_Current = NULL;
			else
				_Current = GetNextSibling( _Current );
		}
	}

	if ( _Current != NULL )
		return GetElement( _Current );
	else
		return NULL;
}

void nsxpcm::LaunchURI( const str::string_ &RawURI )
{
	nsCOMPtr<nsIIOService> IOService = NULL;
	nsCOMPtr<nsIExternalProtocolService> ExternalProtocolService = NULL;
	nsCString TransformedURI;
	nsIURI *URI = NULL;

	nsresult Result = NS_OK;	// Uniquement  des fins de dbogage.

	CreateInstance( "@mozilla.org/network/io-service;1", IOService );
	CreateInstance( "@mozilla.org/uriloader/external-protocol-service;1", ExternalProtocolService );

	Transform( RawURI, TransformedURI );

	if ( ( Result = IOService->NewURI( TransformedURI, NULL, NULL, &URI ) ) != NS_OK )
		ERRLbr();

	if ( ( Result = ExternalProtocolService->LoadURI( URI, NULL ) ) != NS_OK )
		ERRLbr();
}

bso::bool__ nsxpcm::GetDirectory(
	const char *Name,
	nsACString &Path,
	err::handling__ ErrHandling )
{
	nsresult Result = NS_OK;
ERRProlog
	nsCOMPtr<nsIDirectoryServiceProvider> DirectoryServiceProvider;
	nsIFile *File = NULL;
	NSXPCM__BOOL Persistent = false;
ERRBegin
	GetService<nsIDirectoryServiceProvider>( "@mozilla.org/file/directory_service;1", DirectoryServiceProvider );

	if ( ( Result = DirectoryServiceProvider->GetFile( Name, &Persistent, &File ) ) != NS_OK )
		if ( ErrHandling != err::hUserDefined )
			ERRFwk();
		else
			ERRReturn;

	T( File->GetNativePath( Path ) );
ERRErr
ERREnd
ERREpilog
	return Result == NS_OK;
}

bso::bool__ nsxpcm::GetDirectory(
	const char *Name,
	str::string_ &Directory,
	err::handling__ ErrHandling )
{
	nsCString RawDirectory;
	bso::size__ Length = Directory.Amount();

	if ( !GetDirectory( Name, RawDirectory, ErrHandling ) )
		return false;
	
	Transform( RawDirectory, Directory );

	if ( Directory.Amount() != Length )
		Directory.Append( '/' );

	return true;
}

void nsxpcm::LaunchEmbedFile( const char *RawFile )
{
ERRProlog
	nsCOMPtr<nsILocalFile> LocalFile = NULL;
	nsCString Directory;
	nsString Transformed;
	str::string Joined;
	strings Splitted;
	sdr::row__ Row = E_NIL;
ERRBegin
	GetWorkingDirectory( Directory );

	CreateInstance( "@mozilla.org/file/local;1", LocalFile );
	T( LocalFile->InitWithNativePath( Directory ) );

	Joined.Init( RawFile );
	Splitted.Init();

	Split( Joined, '/', Splitted );

	if ( Splitted.Amount() == 0 )
		Splitted.Append( Joined );

	Row = Splitted.First();

	while ( Row != E_NIL ) {
		Transform( Splitted( Row ), Transformed );

		T( LocalFile->AppendRelativePath( Transformed ) );

		Row = Splitted.Next( Row );
	}

	T( LocalFile->Launch() );
ERRErr
ERREnd
ERREpilog
}

void nsxpcm::AddFormHistoryEntry(
	const str::string_ &RawName,
	const str::string_ &RawEntry )
{
#ifdef ENABLE_FORMHISTORY
	nsEmbedString Name, Entry;

	nsxpcm::Transform( RawName, Name );
	nsxpcm::Transform( RawEntry, Entry );

#	ifdef CPE__MT
	mtx::Lock( FormHistoryMutex_ );
#	endif
	FormHistory_->AddEntry( Name, Entry );
#	ifdef CPE__MT
	mtx::Unlock( FormHistoryMutex_ );
#	endif
#else
	ERRLmt();
#endif
}

void nsxpcm::RemoveFormHistoryEntry(
	const str::string_ &RawName,
	const str::string_ &RawEntry )
{
#ifdef ENABLE_FORMHISTORY
	nsEmbedString Name, Entry;

	nsxpcm::Transform( RawName, Name );
	nsxpcm::Transform( RawEntry, Entry );

#	ifdef CPE__MT
	mtx::Lock( FormHistoryMutex_ );
#	endif
	FormHistory_->RemoveEntry( Name, Entry );
#	ifdef CPE__MT
	mtx::Unlock( FormHistoryMutex_ );
#	endif
#else
	ERRLmt();
#endif
}

void nsxpcm::RemoveEntriesForName( const str::string_ &RawName )
{
#ifdef ENABLE_FORMHISTORY
	nsEmbedString Name;

	nsxpcm::Transform( RawName, Name );

#	ifdef CPE__MT
	mtx::Lock( FormHistoryMutex_ );
#	endif
	FormHistory_->RemoveEntriesForName( Name );
#	ifdef CPE__MT
	mtx::Unlock( FormHistoryMutex_ );
#	endif
#else
	ERRLmt();
#endif
}

bso::u32__ nsxpcm::GetArguments(
	nsICommandLine *CommandLine,
	arguments_ &Arguments )
{
	PRInt32 Amount = 0;
ERRProlog
	PRInt32 Counter = 0;
	str::string Argument;
	nsString RawArgument;
ERRBegin
	T( CommandLine->GetLength( &Amount ) );

	if ( ( Amount < 0 ) || ( Amount > BSO_UINT_MAX ) )
		ERRSys();

	while ( Counter < Amount ) {
		T( CommandLine->GetArgument( Counter++, RawArgument ) );

		Argument.Init();
		Transform( RawArgument, Argument );
		Arguments.Append( Argument );
	}
		
ERRErr
ERREnd
ERREpilog
	return (bso::u32__ )Amount;
}

static void GetBroadcasterAttributeValue_(
	nsIDOMDocument *Document,
	const str::string_ &Id,
	const char *AttributeName,
	str::string_ &Value )
{
	nsIDOMNode *Node = nsxpcm::GetElementById( Document, Id );

	if ( Node != NULL )
		nsxpcm::GetAttribute( Node, AttributeName, Value );
}

static void CopyBroadcasterCommand_(
	nsIDOMDocument *Document,
	nsIDOMNode *Node )
{
ERRProlog
	str::string ObservesAttributeValue;
	str::string CommandAttributeValue;
ERRBegin
	ObservesAttributeValue.Init();

	nsxpcm::GetAttribute( Node, "observes", ObservesAttributeValue );

	if ( ObservesAttributeValue.Amount() != 0 ) {
		CommandAttributeValue.Init();
		GetBroadcasterAttributeValue_( Document, ObservesAttributeValue, "command", CommandAttributeValue );

		if ( CommandAttributeValue.Amount() != 0 )
			if ( !HasAttribute( Node, "oncommand" ) )
				nsxpcm::SetAttribute( Node, "command", CommandAttributeValue );
	}
ERRErr
ERREnd
ERREpilog
}

static void CopyBroadcasterCommand_(
	nsIDOMDocument *Document,
	nsIDOMNodeList *List )
{
	PRUint32 Length = 0;
	nsIDOMNode *Node = NULL;

	T( List->GetLength( &Length ) );

	while ( Length-- ) {
		T( List->Item( Length, &Node ) );

		CopyBroadcasterCommand_( Document, Node );
	}
}

void nsxpcm::DuplicateBroadcasterCommand( nsIDOMDocument *Document )
{
	nsIDOMNode *Node = GetElement( Document );
	bso::bool__ Ascending = false;

	while ( ( Node != Document ) || ( !Ascending ) ) {
		if ( !Ascending  ) {
			CopyBroadcasterCommand_( Document, Node );
		}

		if ( ( GetFirstChild( Node ) != NULL ) && !Ascending ) {
			Ascending = false;
			Node = GetFirstChild( Node );
		} else if ( GetNextSibling( Node ) != NULL ) {
			Ascending = false;
			Node = GetNextSibling( Node );
		} else if ( GetParentNode( Node ) != NULL ) {
			Ascending = true;
			Node = GetParentNode( Node );
		} else
			ERRFwk();
	}
}

static void AddSemiColonCommand_(nsIDOMNodeList *List )
{
	PRUint32 Length = 0;
	nsIDOMNode *Node = NULL;

	T( List->GetLength( &Length ) );

	while ( Length-- ) {
		T( List->Item( Length, &Node ) );

		if ( !HasAttribute( Node, "oncommand" ) )
			nsxpcm::SetAttribute( Node, "oncommand", ";" );
	}
}

void nsxpcm::PatchBadCommandBehaviorforKeysetListener( nsIDOMDocument *Document )
{
	nsIDOMNodeList *List = NULL;
	nsString Id;

	Transform( "key", Id );

	if ( Document->GetElementsByTagName( Id, &List ) != NS_OK )
		ERRLbr();

	AddSemiColonCommand_( List );
}

static nsIDOMWindowInternal *GetWindow_(
	nsIDOMDocument *Document,
	const char *TagName )
{
	nsString Id;
	nsIDOMNodeList *List = NULL;
	PRUint32 Length = 0;
	nsIDOMNode *Node = NULL;
	nsIDOMWindow *Window = NULL;

	Transform( TagName, Id );

	if ( Document->GetElementsByTagName( Id, &List ) != NS_OK )
		ERRLbr();

	T( List->GetLength( &Length ) );

	if ( Length != 1 )
		return NULL;

	T( List->Item( Length, &Node ) );

	return GetWindowInternal( Node );

}


nsIDOMWindowInternal *nsxpcm::GetWindow( nsIDOMDocument *Document )
{
	nsIDOMWindowInternal *Window = NULL;

	if ( ( Window = GetWindow_( Document, "window" ) ) == NULL )
		if ( ( Window = GetWindow_( Document, "page" ) ) == NULL )
			if ( ( Window = GetWindow_( Document, "dialog" ) ) == NULL )
				ERRFwk();

	return Window;
}

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class nsxpcmpersonnalization
{
public:
	nsxpcmpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
#ifdef ENABLE_FORMHISTORY
		nsxpcm::GetService( "@mozilla.org/satchel/form-history;1", FormHistory_ );
# ifdef CPE__MT
		FormHistoryMutex_ = mtx::Create( mtx::mOwned );
# endif
#endif
		MasterWindowMutex_ = mtx::Create();
		MasterWindows_.Init();
	}
	~nsxpcmpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the ending of the application  */
#ifdef ENABLE_FORMHISTORY
# ifdef CPE__MT
		mtx::Delete( FormHistoryMutex_ );
# endif
#endif
		mtx::Delete( MasterWindowMutex_ );

		if ( ::JSConsoleWindow_ != NULL )
			Close( JSConsoleWindow_ );
	}
};


				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

static nsxpcmpersonnalization Tutor;
