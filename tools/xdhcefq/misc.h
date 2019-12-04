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

#ifndef MISC_INC_
# define MISC_INC_

# undef system	// Overidden by 'tol'. Not useful here.

# include "include/cef_version.h"
# include "include/capi/cef_app_capi.h"
# include "include/capi/cef_client_capi.h"

# include "bso.h"
//# include "ntvstr.h"
# include "str.h"
# include "xdhutl.h"
# include "xdhujp.h"

# define MISC_NAME_MC			"xdhcefq"	// X(SL)/DH(TML) with CEF (Chromium Embedded Framework)
# define MISC_NAME_LC			"xdhcefq"
# define MISC_NAME_UC			"XDHCEFQ"
# define MISC_WEBSITE_URL		"http://q37.info/computing/epeios/tools/" MISC_NAME_LC "/"
# define MISC_AUTHOR_NAME		"Claude SIMON"
# define MISC_AUTHOR_CONTACT	"http://q37.info/contact.html"
# define MISC_OWNER_NAME		"Claude SIMON"
# define MISC_OWNER_CONTACT		"http://q37.info/contact.html"
# define MISC_COPYRIGHT			COPYRIGHT_YEARS " " MISC_OWNER_NAME " (" MISC_OWNER_CONTACT ")"	
# define MISC_VERSION			VERSION


# ifdef CPE_S_WIN
typedef cef_string_wide_t cef_nstring_t;
#  define	cef_convert_to_		cef_string_wide_to_utf16
#  define	cef_convert_from_	cef_string_utf16_to_wide
# elif defined( CPE_S_POSIX )
typedef cef_string_utf8_t	cef_nstring_t;
#  define	cef_convert_to_		cef_string_utf8_to_utf16
#  define	cef_convert_from_	cef_string_utf16_to_utf8
# else
#  error
# endif

// # define LOC	cio::COut << txf::nl << "------------------------->" << __LOC__ << txf::nl << txf::commit

void cef_convert_to(
	const cef_nstring_t *From,
	cef_string_t *To );

void cef_convert_from(
	const cef_string_t *From,
	cef_nstring_t *To );

namespace misc {
	using namespace xdhutl;

	inline cef_frame_t *GetFrame( cef_browser_t *Browser )
	{
		return Browser->get_main_frame( Browser );
	}

	inline cef_browser_host_t *GetHost( cef_browser_t *Browser )
	{
		return Browser->get_host( Browser );
	}

	str::string_ &GetName(
		cef_domnode_t *Node,
		str::string_ &Name );

	const char *GetName(
		cef_domnode_t *Node,
		TOL_CBUFFER___ &Buffer );
	
	str::string_ &GetName(
		const nstring___ &Id,
		cef_frame_t *Frame,
		str::string_ &Name );

	const char *GetName(
		const nstring___ &Id,
		cef_frame_t *Frame,
		TOL_CBUFFER___ &Buffer );

	enum client_message__ {	// Messages  destination du client.
		cmOpenFile,
		cmOpenFiles,
		cmSaveFile,
		cmClosed,
		cm_amount,
		cm_Undefined
	};

	const char *GetLabel( client_message__ Message );

	void GetString(
		cef_list_value_t *ListValue,
		str::string_ &String );

	void Convert(
		cef_list_value_t *ListValue,
		xdhcmn::digest_ &Digest );

	void PutString(
		const str::string_ &String,
		cef_list_value_t *ListValue );

	void Convert(
		const xdhcmn::digest_ &Digest,
		cef_list_value_t *ListValue );

	// Same as above function with somehow weird name to facilitate template deduction.
	inline void ConvertD2LV(
		const xdhcmn::digest_ &Digest,
		cef_list_value_t *ListValue )
	{
		Convert( Digest, ListValue );
	}

	client_message__ GetClientMessage( const cef_string_t *Pattern );

	enum render_message__ {	// Messages � destination du 'render'.
		rmStart,
		rmHandleSelectedFiles,	// Gestion des fichiers slectionns.
//		rmHandleAction,
		rmClose,	// Envoit de la demande de fermeture.
		rm_amount,
		rm_Undefined,
	};

	const char *GetLabel( render_message__ Message );

	render_message__ GetRenderMessage( const cef_string_t *Pattern );

	const str::string_ &GenerateId( str::string_ &Id );

	const char *ExecuteJavascript(
		const nstring___ &Script,
		TOL_CBUFFER___ *Buffer,
		cef_frame_t *Frame = NULL );

	inline void ExecuteJavascript(
		const nstring___ &Script,
		cef_frame_t *Frame = NULL )
	{
		ExecuteJavascript( Script, NULL, Frame );
	}

	void AppendTag(
		const char *Name,
		const nstring___ &Value,
		str::strings_ &Names,
		str::strings_ &Values );

	void Report(
		const nstring___ &Message,
		cef_frame_t *Frame );

	const char *OGetProperty(
		const nstring___ &Id,
		const nstring___ &Name,
		cef_frame_t *Frame,
		TOL_CBUFFER___ &Buffer );

	inline const char *MGetProperty(
		const nstring___ &Id,
		const nstring___ &Name,
		cef_frame_t *Frame,
		TOL_CBUFFER___ &Buffer )
	{
		if ( OGetProperty( Id, Name, Frame, Buffer ) == NULL )
			qRGnr();

		return Buffer;
	}

	void SetAttribute(
		const nstring___ &Id,
		const nstring___ &Name,
		const nstring___ &Value,
		cef_frame_t *Frame );

	const char *OGetAttribute(
		const nstring___ &Id,
		const nstring___ &Name,
		cef_frame_t *Frame,
		TOL_CBUFFER___ &Buffer );

	inline const char *MGetAttribute(
		const nstring___ &Id,
		const nstring___ &Name,
		cef_frame_t *Frame,
		TOL_CBUFFER___ &Buffer )
	{
		if ( OGetAttribute( Id, Name, Frame, Buffer ) == NULL )
			qRGnr();

		return Buffer;
	}

	bso::bool__ OGetAttribute(
		const nstring___ &Id,
		const nstring___ &Name,
		cef_frame_t *Frame,
		str::string_ &Value );

	inline void MGetAttribute(
		const nstring___ &Id,
		const nstring___ &Name,
		cef_frame_t *Frame,
		str::string_ &Value )
	{
		if ( !OGetAttribute( Id, Name, Frame, Value ) )
			qRGnr();
	}

	void RemoveAttribute(
		const nstring___ &Id,
		const nstring___ &Name,
		cef_frame_t *Frame  );

	void SetAttribute(
		cef_domnode_t *Node,
		const nstring___ &Name,
		const nstring___ &Value );

	const char *OGetAttribute(
		cef_domnode_t *Node,
		const nstring___ &Name,
		TOL_CBUFFER___ &Buffer );

	inline const char *MGetAttribute(
		cef_domnode_t *Node,
		const nstring___ &Name,
		TOL_CBUFFER___ &Buffer )
	{
		if ( OGetAttribute( Node, Name, Buffer ) == NULL )
			qRGnr();

		return Buffer;
	}

	bso::bool__ OGetAttribute(
		cef_domnode_t *Node,
		const nstring___ &Name,
		str::string_ &Value );

	inline void MGetAttribute(
		cef_domnode_t *Node,
		const nstring___ &Name,
		str::string_ &Value )
	{
		if ( !OGetAttribute( Node, Name, Value ) )
			qRGnr();
	}

	void SetValue(
		const nstring___ &Id,
		const nstring___ &Value,
		cef_frame_t *Frame );

	const str::string_ &GetOrGenerateId(	// Get the id of an element, or generated one if it has no id.
		cef_domnode_t *Node,
		str::string_ &Id );

	const char *GetOrGenerateId(	// Get the id of an element, or generated one if it has no id.
		cef_domnode_t *Node,
		TOL_CBUFFER___ &Buffer );

#if CHROME_VERSION_BUILD < 2987
# define cef_base_ref_counted_t cef_base_t
# define _cef_base_ref_counted_t _cef_base_t
#endif

	inline void CEF_CALLBACK AddRef_( struct _cef_base_ref_counted_t* self )
	{
	}

	inline  int CEF_CALLBACK Release_( struct _cef_base_ref_counted_t* self )
	{
		return 0;
	}

	inline  int CEF_CALLBACK HasOneRef_( struct _cef_base_ref_counted_t* self )
	{
		return 0;
	}

	template <typename t> inline void Reset( t *T )
	{
		memset( T, 0, sizeof( t ) );
	}

	template <typename t> inline void SetBase( cef_base_ref_counted_t *Base )
	{
		Base->size = sizeof( t );

		Base->add_ref = AddRef_;
		Base->release = Release_;
		Base->has_one_ref = HasOneRef_;

	//	Base.add_ref( &Base );
	}

	template <typename t> inline void Set( t *T )
	{
		Reset( T );

		SetBase<t>( &T->base );
	}

	template <typename t> inline void AlternateSet_( t *T )
	{
		Reset( T );

		T->size = sizeof( t );
	}

	inline void Set( cef_window_info_t *WindowInfo )
	{
		Reset( WindowInfo );
	}

	inline void Set( cef_settings_t *Settings )
	{
		AlternateSet_( Settings );
	}

	inline void Set( cef_browser_settings_t *Settings )
	{
		AlternateSet_( Settings );
	}

	inline void Set( cef_string_t *String )
	{
		Reset( String );
	}

	inline void Set( cef_nstring_t *String )
	{
		Reset( String );
	}

	inline void Set(
		cef_string_t *String,
		const ntvstr::char__ *Text )
	{
		Set( String );
		cef_convert_to_( Text, ntvstr::Amount( Text ), String );
#if 0
# ifdef CPE_WIN
		cef_string_wide_to_utf16( Text, ntvstr::Size( Text ), String );
# elif defined( CPE_POSIX )
		cef_string_utf8_to_utf16( Text, ntvstr::Size( Text ), String );
# else
#  error
# endif
#endif
	}

	inline void Set(
		cef_string_t *String,
		const ntvstr::string___ &Text )
	{
		Set( String, Text.Internal() );
	}

# ifdef CPE_WIN
	inline void Set(
		cef_string_t *String,
		const char *Text )
	{
		Set( String, nstring___(Text) );
	}
# endif

	template <typename t> inline void AddRef( t *T )
	{
		T->base.add_ref( &T->base );
	}

	template <typename t> inline void Release( t *T )
	{
		T->base.release( &T->base );
	}

	inline void Clear( cef_string_t *String )
	{
		cef_string_clear( String );
	}

	inline void Clear( cef_string_wide_t *String )
	{
		cef_string_wide_clear( String );
	}

	inline void Clear( cef_string_utf8_t *String )
	{
		cef_string_utf8_clear( String );
	}

	class cef_string___
	: public cef_string_t
	{
	public:
		void reset( bso::bool__ P = true )
		{
			if ( P )
				if ( dtor != NULL )
					dtor( str );

			misc::Reset<cef_string_t>( this );
		}
		E_CDTOR( cef_string___ );
		cef_string___( const nstring___ &String )
		{
			reset( false );

			Init( String );
		}
		void Init( void )
		{
			reset();
		}
		void Init( const nstring___ &String )
		{
			Init();

			if ( String.Size() != 0 )
				Set( *this, String );
		}
		cef_string_t *operator()( void )
		{
			return this;
		}
		operator cef_string_t *( )
		{
			return this;
		}
	};

	void FillList(
		const str::strings_ &Source,
		cef_string_list_t &Target );


# undef SendMessage	// Windows API has some macros whith this name.

	void SendMessage(
		client_message__ Message,
		cef_browser_t *Browser,
		const str::string_ &Param = str::string() );

	void SendMessage(
		client_message__ Message,
		cef_browser_t *Browser,
		const xdhcmn::digest_ &Args );

	void SendMessage(
		render_message__ Message,
		cef_browser_t *Browser,
		const str::string_ &Param = str::string() );

	void SendMessage(
		render_message__ Message,
		cef_browser_t *Browser,
		const xdhcmn::digest_ &Args );

	cef_v8value_t *GetFunction(
		cef_v8value_t *Object,
		const str::string_ &Name,
		err::handling__ ErrHandling = err::h_Default );

	// Both below functions helps transforming asynchronous JS functions in synchronous ones.

	// To call after the launch of the asynchronous function. Wait that the functions is terminated, and put the evntual response in 'Buffer'.
	const str::string_ &WaitForResponse( str::string_ &Response );

	// To call once the asynchronous function is terminated, with 'Response' as eventual response (send to 'WaitForResponse()').
	void SendResponse( const str::string_ &Response );
}

inline void cef_convert_to(
	const cef_nstring_t *From,
	cef_string_t *To )
{
	misc::Set( To );
	cef_convert_to_( From->str, From->length, To );
}

inline void cef_convert_from(
	const cef_string_t *From,
	cef_nstring_t *To )
{
	misc::Set( To );
	cef_convert_from_( From->str, From->length, To );
}


#endif
