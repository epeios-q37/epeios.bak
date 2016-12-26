/*
	Copyright (C) 2014-2016 by Claude SIMON (http://zeusw.org/epeios/contact.html).

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

#include "browser.h"
#include "registry.h"
#include "scltool.h"
#include "files.h"
#include "render.h"
#include "dir.h"

namespace {
	static class rack__ {
		public:
			cef_browser_process_handler_t BrowserProcessHandler;
			cef_window_info_t WindowInfo;
			cef_browser_settings_t BrowserSettings;
			cef_client_t Client;
			cef_keyboard_handler_t KeyboardHandler;
			cef_life_span_handler_t LifeSpanHandler;
			cef_load_handler_t LoadHandler;
			cef_jsdialog_handler_t JSDialogHandler;
			bso::bool__ ClosePending;	// If 'true', wating for user agreement for closing;
			void reset( bso::bool__ P = true )
			{
#ifdef CPE_S_WIN
				if ( P )
					misc::Clear( &WindowInfo.window_name );
#endif
				ClosePending = false;
			}
			E_CDTOR( rack__ );
			// The members are intiialized when required.
	} Rack_;
}

#ifdef CPE_S_WIN
static void SetAsPopup_(
	cef_window_handle_t Parent,
	const misc::nstring___ &Name,
	cef_window_info_t &Info )
{
    Info.style = WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE;
    Info.parent_window = Parent;
    Info.x = CW_USEDEFAULT;
    Info.y = CW_USEDEFAULT;
    Info.width = CW_USEDEFAULT;
    Info.height = CW_USEDEFAULT;

    misc::Set( &Info.window_name, Name );
}
#endif

static bso::bool__ HandleModifier_(
	bso::uint__ Flags,
	bso::uint__ Mask,
	bso::char__ ModifierChar,
	str::string_ &Shortcut )
{
	if ( (Flags & Mask) != 0 ) {
		Shortcut.Append( ModifierChar );
		return true;
	} else {
		Shortcut.Append( '.' );
		return false;
	}
}

// Don't ask about the 'Offset' stuff ; seems to be needed when 'Ctrl' is used...
static void BuildShortcut_(
	const struct _cef_key_event_t* event,
	str::string_ &Shortcut )
{
	bso::uint__ Modifiers = event->modifiers;
	int Offset = 0;

	HandleModifier_(Modifiers, EVENTFLAG_ALT_DOWN, 'a', Shortcut );	// Alt
	Offset += ( HandleModifier_(Modifiers, EVENTFLAG_CONTROL_DOWN, 'c', Shortcut ) ? 'a' - 1 : 0 );	// Ctrl.
	HandleModifier_(Modifiers, EVENTFLAG_SHIFT_DOWN, 's', Shortcut );	// Shift

	if ( isalpha( event->unmodified_character + Offset ) )
		Shortcut.Append( tolower( event->unmodified_character + Offset) );
}

/*
static int CEF_CALLBACK OnKeyEvent_(
	struct _cef_keyboard_handler_t* self,
	struct _cef_browser_t* browser,
	const struct _cef_key_event_t* event,
	cef_event_handle_t os_event )
{
	bso::bool__ Handled = false;
qRH
	str::string Action;
	str::string Shortcut;
qRB
	if ( event->type == KEYEVENT_CHAR ) {
		Shortcut.Init();
		BuildShortcut_( event, Shortcut );

		Action.Init();
		if ( Rack_.Shortcuts.GetEventName( Shortcut, Action ) ) {
			if ( Action.Amount() != 0 ) {
				misc::SendMessage( misc::rmHandleAction, browser, Action );
				Handled = true;
			}
		}
	}
qRR
qRT
	misc::Release( self );
	misc::Release( browser );
//	misc::Release( event );	No 'base' member.
qRE
	return Handled;
}
*/

/*
static struct _cef_keyboard_handler_t* CEF_CALLBACK GetKeyboardHandler_( struct _cef_client_t* self )
{
	misc::Set( &Rack_.KeyboardHandler );
	Rack_.KeyboardHandler.on_key_event = OnKeyEvent_;

	misc::Release( self );

	return &Rack_.KeyboardHandler;
}
*/

static int CEF_CALLBACK OnBeforePopup_(
	struct _cef_life_span_handler_t* self,
	struct _cef_browser_t* browser,
	struct _cef_frame_t* frame,
	const cef_string_t* target_url,
	const cef_string_t* target_frame_name,
	cef_window_open_disposition_t target_disposition,
	int user_gesture,
	const struct _cef_popup_features_t* popupFeatures,
	struct _cef_window_info_t* windowInfo,
	struct _cef_client_t** client,
	struct _cef_browser_settings_t* settings,
	int* no_javascript_access)
{
qRH
	ntvstr::string___ Document;
	cef_nstring_t URL;
qRB
	misc::Set( &URL );
	cef_convert_from( target_url, &URL );

	Document.Init( URL.str );

	tol::Launch( Document );

	misc::Release( self );
	misc::Release( frame );
	misc::Release( browser );
qRR
qRT
	misc::Clear( &URL );
qRE
	return true;
}

static void CEF_CALLBACK OnBeforeClose_(
	struct _cef_life_span_handler_t* self,
	struct _cef_browser_t* browser)
{
	cef_quit_message_loop();

	misc::Release( self );
	misc::Release( browser );
}

static int CEF_CALLBACK DoClose_(
	struct _cef_life_span_handler_t* self,
	struct _cef_browser_t* browser )
{
	bso::bool__ ClosingAborted = false;
	
	if ( Rack_.ClosePending ) {
		Rack_.ClosePending = false;
		ClosingAborted = false;
	} else {
		Rack_.ClosePending = true;
		ClosingAborted = true;
		misc::SendMessage( misc::rmClose, browser );
	}

	misc::Release( self );
	misc::Release( browser );

	return ClosingAborted;
}

static struct _cef_life_span_handler_t* CEF_CALLBACK GetLifeSpanHandler_( struct _cef_client_t* self )
{
	misc::Set( &Rack_.LifeSpanHandler );
	Rack_.LifeSpanHandler.on_before_popup = OnBeforePopup_;
	Rack_.LifeSpanHandler.on_before_close = OnBeforeClose_;
	Rack_.LifeSpanHandler.do_close = DoClose_;

	misc::Release( &Rack_.LifeSpanHandler );

	misc::Release( self );

	return &Rack_.LifeSpanHandler;
}

static void CEF_CALLBACK OnLoadStart_(
	struct _cef_load_handler_t* self,
    struct _cef_browser_t* browser,
	struct _cef_frame_t* frame,
    cef_transition_type_t transition_type)
{
	if ( frame->is_main( frame ) )
	{

	}

	misc::Release( self );
	misc::Release( frame );
	misc::Release( browser );
}

static void CEF_CALLBACK OnLoadEnd_(
	struct _cef_load_handler_t* self,
	struct _cef_browser_t* browser,
	struct _cef_frame_t* frame,
	int httpStatusCode )
{
qRH
	str::string ModuleFilename;
qRB
	if ( frame->is_main(frame) ) {
		ModuleFilename.Init();
		sclmisc::MGetValue( registry::ModuleFilename, ModuleFilename );
		misc::SendMessage( misc::rmStart, browser, ModuleFilename );
	}
qRR
qRT
	misc::Release( self );
	misc::Release( browser );
	misc::Release( frame );
qRE
}

static struct _cef_load_handler_t* CEF_CALLBACK GetLoadHandler_( struct _cef_client_t* self )
{
	misc::Set( &Rack_.LoadHandler );

	Rack_.LoadHandler.on_load_start = OnLoadStart_;
	Rack_.LoadHandler.on_load_end = OnLoadEnd_;

	misc::Release( &Rack_.LoadHandler );

	misc::Release( self );

	return &Rack_.LoadHandler;
}

#if 0

#include <Xm/RowColumn.h>
#include <Xm/MessageB.h>
#include <Xm/PushB.h>

namespace jsdialog_ {


void Test_( void )
{
    XtAppContext     app;
    Widget           toplevel, rc, hpb, gpb; 
    /* callbacks for the pushbuttons -- pops up dialog */
    void             popup_callback(Widget, XtPointer, XtPointer);
    void             exit_callback(Widget, XtPointer, XtPointer);

    XtSetLanguageProc (NULL, NULL, NULL);
    toplevel = XtVaOpenApplication (&app, "Demos", NULL, 0, &argc, argv, NULL, 
                                     sessionShellWidgetClass, NULL);   

    rc = XmCreateRowColumn (toplevel, "rowcol", NULL, 0);

    hpb = XmCreatePushButton (rc, "Hello", NULL, 0);
    XtAddCallback (hpb,
           XmNactivateCallback,
           popup_callback,
           (XtPointer) "Hello World");

    gpb = XmCreatePushButton (rc, "Goodbye", NULL, 0);
    XtAddCallback (gpb, XmNactivateCallback, exit_callback, NULL);

    XtManageChild (hpb);
    XtManageChild (gpb);
    XtManageChild (rc);
    XtRealizeWidget (toplevel);
    XtAppMainLoop (app);
}

/* callback for the "Hello" PushButton. 
** Popup an InformationDialog displaying the text passed as the client
** data parameter. 
*/
void popup_callback (Widget button, XtPointer client_data,
                     XtPointer call_data)
{
    Widget     dialog;
    XmString   xm_string;
    void       activate_callback(Widget, XtPointer, XtPointer);
    Arg        args[5];
    int        n = 0;
    char       *text = (char *) client_data;

    /* set the label for the dialog */
    xm_string = XmStringCreateLocalized (text);
    XtSetArg (args[n], XmNmessageString, xm_string); n++;
    /* Create the InformationDialog as child of button */
    dialog = XmCreateInformationDialog (button, "info", args, n); 
    /* no longer need the compound string, free it */
    XmStringFree (xm_string);
    /* add the callback routine */
    XtAddCallback (dialog, XmNokCallback, activate_callback, NULL);
    /* manage the MessageBox: has the side effect of displaying the */
    /*       XmDialogShell parent */
    XtManageChild (dialog);
}

/*
** callback routine when the user pressed the "Goodbye" button 
*/
void exit_callback (Widget w, XtPointer client_data, XtPointer call_data){
    exit (0);
}

/* callback routine for when the user presses the OK button. 
** Yes, despite the fact that the OK button was pressed, the 
** widget passed to this callback routine is the dialog! 
*/
void activate_callback (Widget dialog,
                        XtPointer client_data,
                        XtPointer call_data)
{
    puts ("OK was pressed.");
}

	void Test_(void) {
 Display *d;
   Window w;
   XEvent e;
   const char *msg = "Hello, World!";
   int s;
 
   d = XOpenDisplay(NULL);
   if (d == NULL) {
      fprintf(stderr, "Cannot open display\n");
      exit(1);
   }
 
   s = DefaultScreen(d);
   w = XCreateSimpleWindow(d, RootWindow(d, s), 10, 10, 100, 100, 1,
                           BlackPixel(d, s), WhitePixel(d, s));
   XSelectInput(d, w, ExposureMask | KeyPressMask);
   XMapWindow(d, w);
 
   while (1) {
      XNextEvent(d, &e);
      if (e.type == Expose) {
         XFillRectangle(d, w, DefaultGC(d, s), 20, 20, 10, 10);
         XDrawString(d, w, DefaultGC(d, s), 10, 50, msg, strlen(msg));
      }
      if (e.type == KeyPress)
         break;
   }

 
   XCloseDisplay(d);
	
	}


		int CEF_CALLBACK OnJSDialog_(
		struct _cef_jsdialog_handler_t* self,
		struct _cef_browser_t* browser,
			const cef_string_t* origin_url,
			cef_jsdialog_type_t dialog_type,
			const cef_string_t* message_text,
			const cef_string_t* default_prompt_text,
		struct _cef_jsdialog_callback_t* callback,
			int* suppress_message )
		{
			Test_();

			*suppress_message = 1;

			return false;
		}
}
#endif

namespace jsdialog_ {
	namespace {
		int CEF_CALLBACK OnJSDialog_(
			struct _cef_jsdialog_handler_t* self,
			struct _cef_browser_t* browser,
			const cef_string_t* origin_url,
			cef_jsdialog_type_t dialog_type,
			const cef_string_t* message_text,
			const cef_string_t* default_prompt_text,
			struct _cef_jsdialog_callback_t* callback,
			int* suppress_message )
		{
			*suppress_message = 1;
			str::wString UTF;
			ntvstr::rString Natif;
			cef_nstring_t Text;

//			callback->cont( callback, true, default_prompt_text );

			misc::Release( self );
			misc::Release( browser );
			misc::Release( callback );

			cef_convert_from( message_text, &Text );

			Natif.Init( Text.str );

			UTF.Init();
			cio::COut << "---------------------> OnJSDialog !    " << Natif.UTF8( UTF ) << txf::nl << txf::commit;

			return false;
		}

		int CEF_CALLBACK OnBeforeUnloadDialog_(
			struct _cef_jsdialog_handler_t* self,
			struct _cef_browser_t* browser,
			const cef_string_t* message_text,
			int is_reload,
			struct _cef_jsdialog_callback_t* callback )
		{
			callback->cont( callback, true, message_text );

			misc::Release( self );
			misc::Release( browser );
			misc::Release( callback );

			cio::COut << "---------------------> OnBefore !" << txf::nl << txf::commit;

			return true;
		}

		void CEF_CALLBACK OnResetDialogState_(
			struct _cef_jsdialog_handler_t* self,
			struct _cef_browser_t* browser )
			{
			misc::Release( self );
			misc::Release( browser );

						cio::COut << "---------------------> Reset !" << txf::nl << txf::commit;


			}

		 void CEF_CALLBACK OnDialogClosed_(
			struct _cef_jsdialog_handler_t* self,
			struct _cef_browser_t* browser)
		 {
			misc::Release( self );
			misc::Release( browser );

						cio::COut << "---------------------> OnClosed !" << txf::nl << txf::commit;

		 }
	}

	void Set( cef_jsdialog_handler_t &Handler )
	{
		Handler.on_before_unload_dialog = OnBeforeUnloadDialog_;
		Handler.on_dialog_closed = OnDialogClosed_;
		Handler.on_jsdialog = OnJSDialog_;
		Handler.on_reset_dialog_state = OnResetDialogState_;
	}
}


static struct _cef_jsdialog_handler_t* CEF_CALLBACK GetJSDialogHandler_( struct _cef_client_t* self )
{
	misc::Set( &Rack_.JSDialogHandler );

	jsdialog_::Set( Rack_.JSDialogHandler );

	misc::Release( &Rack_.JSDialogHandler );

	misc::Release( self );

	return &Rack_.JSDialogHandler;
}

static void FileDialog_(
	cef_list_value_t *ListValue,
	cef_browser_t *Browser,
	cef_file_dialog_mode_t FileDialogMode )
{
qRH
	str::string Param;
qRB
	Param.Init();

	misc::GetString( ListValue, Param );

	files::FileDialog( Browser, FileDialogMode, Param );
qRR
qRT
qRE
}

static void HandleClosingAnswer_(
	const xdhcmn::digest_ &Args,
	cef_browser_t *Browser )
{
	strmrg::retriever__ Retriever;

	Retriever.Init( Args );
	
	if ( Retriever.Availability() != strmrg::aNone ) {
		cef_browser_host_t *Host = misc::GetHost( Browser );
		Host->close_browser( Host, false );	// 'Rack_.ClosePending' is set to 'false' through 'DoClose_(...)'.
	} else
		Rack_.ClosePending = false;
}

static void HandleClosingAnswer_(
	cef_list_value_t *ListValue,
	cef_browser_t *Browser )
{
qRH
	xdhcmn::digest Args;
qRB
	Args.Init();
	misc::Convert( ListValue, Args );

	HandleClosingAnswer_( Args, Browser );
qRR
qRT
qRE
}

static int CEF_CALLBACK ClientOnProcessMessageReceived_(
    struct _cef_client_t* self,
    struct _cef_browser_t* browser,
	cef_process_id_t source_process,
    struct _cef_process_message_t* message)
{
	bso::bool__ Handled = false;
qRH
	cef_string_userfree_t Message = NULL;
	cef_list_value_t *ListValue = NULL;
qRB
	ListValue = message->get_argument_list( message );
	Message = message->get_name( message );

	if ( Message == NULL )
		qRGnr();

	switch ( misc::GetClientMessage( Message ) ) {
	case misc::cmOpenFile:
		FileDialog_( ListValue, browser, FILE_DIALOG_OPEN );
		break;
	case misc::cmOpenFiles:
		FileDialog_( ListValue, browser, FILE_DIALOG_OPEN_MULTIPLE );
		break;
	case misc::cmSaveFile:
		FileDialog_( ListValue, browser, FILE_DIALOG_SAVE );
		break;
	case misc::cmClosed:
		HandleClosingAnswer_( ListValue, browser );
		break;
	default:
		qRGnr();
		break;
	}

	Handled = true;
qRR
qRT
	if ( ListValue != NULL )
		misc::Release( ListValue );

	if ( Message != NULL )
		cef_string_userfree_free( Message );

	misc::Release( self );
	misc::Release( browser );
	misc::Release( message );
qRE
	return Handled;
}

namespace {
	const str::dString &GetURL_( str::dString &URL )
	{
	qRH
		fnm::rName Dir;
	qRB
		if ( !sclmisc::OGetValue( registry::URL, URL ) ) {
			Dir.Init();
			dir::GetSelfDir( Dir );

			URL.Append("file://");
			Dir.UTF8( URL );
			URL.Append("xdhcefq.html");
		}
	qRR
	qRT
	qRE
		return URL;
	}
}

static void CEF_CALLBACK OnContextInitialized_( struct _cef_browser_process_handler_t* self )
{
qRH
	str::string URL;
qRB
	URL.Init();
	GetURL_( URL );

	misc::Set( &Rack_.WindowInfo );

#ifdef CPE_S_WIN
	SetAsPopup_(NULL, MISC_NAME_MC, Rack_.WindowInfo );
#endif

	misc::Set( &Rack_.BrowserSettings );

	misc::Set( &Rack_.Client );

//	Rack_.Client.get_keyboard_handler = GetKeyboardHandler_;
	Rack_.Client.get_life_span_handler = GetLifeSpanHandler_;
	Rack_.Client.get_load_handler = GetLoadHandler_;
	Rack_.Client.get_jsdialog_handler = GetJSDialogHandler_;
	Rack_.Client.on_process_message_received = ClientOnProcessMessageReceived_;

	cef_browser_host_create_browser( &Rack_.WindowInfo, &Rack_.Client, misc::cef_string___( URL ), &Rack_.BrowserSettings, NULL );
qRR
qRT
	misc::Release( self );
qRE
}

static void SetBrowserProcessHandler_( void )
{
	misc::Set( &Rack_.BrowserProcessHandler );

	Rack_.BrowserProcessHandler.on_context_initialized = OnContextInitialized_;
}

cef_browser_process_handler_t* CEF_CALLBACK browser::GetBrowserProcessHandler( struct _cef_app_t* self )
{
//	misc::Release( self );

	return &Rack_.BrowserProcessHandler;
}

Q37_GCTOR( browser )		{
	SetBrowserProcessHandler_();
}
