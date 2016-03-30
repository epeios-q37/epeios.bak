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

#ifndef XDHDWS__INC
# define XDHDWS__INC

# define XDHDWS_NAME		"XDHDWS"

# if defined( E_DEBUG ) && !defined( XDHDWS_NODBG )
#  define XDHDWS_DBG
# endif

// X(SL) DH(TML) DowNStream

# include "xdhcmn.h"

# include "err.h"
# include "tol.h"

namespace xdhdws {
	using xdhcmn::nchar__;
	using xdhcmn::nstring___;

	E_CDEF( char *, ContentTagName, "Content" );
	E_CDEF( char *, ContextTagName, "Context" );

	class proxy__
	{
	private:
		Q37_MRMDF( xdhcmn::proxy_callback__, C_, Callback_ );
	public:
		void reset( bso::bool__ P = true )
		{
			if ( P )
				if ( Callback_ != NULL )
					delete( Callback_ );

			Callback_ = NULL;
		}
		E_CVDTOR( proxy__ );
		void Init( xdhcmn::proxy_callback__ *Callback )	// 'Callback' is destroyed by a 'delete' when this object is dsetroyed.
		{
			reset();

			Callback_ = Callback;
		}
		void Log( const nstring___ &Message )
		{
			C_().Process( xdhcmn::fLog, NULL, Message.Internal()() );
		}
		void Alert(
			const nstring___ &XML,
			const nstring___ &XSL,
			const nstring___ &Title,
			const nstring___ &CloseText )
		{
			C_().Process( xdhcmn::fAlert, NULL, XML.Internal()(), XSL.Internal()(), Title.Internal()(), CloseText.Internal()() );
		}
		bso::bool__ Confirm(
			const nstring___ &XML,
			const nstring___ &XSL,
			const nstring___ &Title,
			const nstring___ &CloseText )
		{
			bso::bool__ Confirmed  = false;
		qRH
			TOL_CBUFFER___ Buffer;
		qRB
			C_().Process( xdhcmn::fConfirm, &Buffer, XML.Internal()( ), XSL.Internal()( ), Title.Internal()(), CloseText.Internal()() );

			Confirmed = !strcmp( "true", Buffer );
		qRR
		qRT
		qRE
			return Confirmed;
		}
		void FillDocumentCastings(
			const nstring___ &XML,
			const nstring___ &XSL )
		{
			C_().Process( xdhcmn::fFillCastings, NULL, nstring___().Internal()(), XML.Internal()( ), XSL.Internal()( ) );
		}
		void FillElementCastings(
			const nstring___ &Id,
			const nstring___ &XML,
			const nstring___ &XSL )
		{
			C_().Process( xdhcmn::fFillCastings, NULL, Id.Internal()( ), XML.Internal()( ), XSL.Internal()( ) );
		}
		void FillElement(
			const nstring___ &Id,
			const nstring___ &XML,
			const nstring___ &XSL )
		{
			C_().Process( xdhcmn::fFillElement, NULL, Id.Internal()( ), XML.Internal()( ), XSL.Internal()( ) );
		}
		void FillDocument(
			const nstring___ &XML,
			const nstring___ &XSL )
		{
			C_().Process( xdhcmn::fFillDocument, NULL, XML.Internal()( ), XSL.Internal()( ) );
		}
		const char *GetProperty(
			const nstring___ &Id,
			const nstring___ &Name,
			TOL_CBUFFER___ &Buffer )
		{
			C_().Process( xdhcmn::fGetProperty, &Buffer, Id.Internal()( ), Name.Internal()( ) );

			return Buffer;
		}
		const str::string_ &GetProperty(
			const nstring___ &Id,
			const nstring___ &Name,
			str::string_ &Property )
		{
		qRH
			TOL_CBUFFER___ Buffer;
		qRB
			Property.Append( GetProperty( Id, Name, Buffer ) );
		qRR
		qRT
		qRE
			return Property;
		}
		template <typename type> bso::sBool GetNumericalProperty(
			const nstring___ &Id,
			const nstring___ &Name,
			type &Value )
		{
			bso::sBool IsEmpty = true;
		qRH
			str::wString Property;
		qRB
			Property.Init();

			GetProperty( Id, Name, Property );

			if ( Property.Amount() != 0 ) {
				Property.ToNumber( Value );
				IsEmpty = false;
			}
		qRR
		qRT
		qRE
			return !IsEmpty;
		}
		void SetProperty(
			const nstring___ &Id,
			const nstring___ &Name,
			const nstring___ &Value )
		{
			C_().Process( xdhcmn::fSetProperty, NULL, Id.Internal()( ), Name.Internal()( ), Value.Internal()( ) );
		}
		const char *GetAttribute(
			const nstring___ &Id,
			const nstring___ &Name,
			TOL_CBUFFER___ &Buffer )
		{
			C_().Process( xdhcmn::fGetAttribute, &Buffer, Id.Internal()( ), Name.Internal()( ) );

			return Buffer;
		}
		const str::string_ &GetAttribute(
			const nstring___ &Id,
			const nstring___ &Name,
			str::string_ &Attribute )
		{
		qRH
			TOL_CBUFFER___ Buffer;
		qRB
			Attribute.Append( GetAttribute( Id, Name, Buffer ) );
		qRR
		qRT
		qRE
			return Attribute;
		}
		void SetAttribute(
			const nstring___ &Id,
			const nstring___ &Name,
			const nstring___ &Value )
		{
			C_().Process( xdhcmn::fSetAttribute, NULL, Id.Internal()( ), Name.Internal()( ), Value.Internal()( ) );
		}
		const char *GetResult(
			const nstring___ &Id,
			TOL_CBUFFER___ &Buffer )
		{
			C_().Process( xdhcmn::fGetResult, &Buffer, Id.Internal()( ) );

			return Buffer;
		}
		const str::string_ &GetResult(
			const nstring___ &Id,
			str::string_ &Result )
		{
		qRH
			TOL_CBUFFER___ Buffer;
		qRB
			Result.Append( GetResult( Id, Buffer ) );
		qRR
		qRT
		qRE
			return Result;
		}
		void RemoveAttribute(
			const nstring___ &Id,
			const nstring___ &Name )
		{
			C_().Process( xdhcmn::fRemoveAttribute, NULL, Id.Internal()( ), Name.Internal()( ) );
		}
		const char *GetContent(
			const nstring___ &Id,
			TOL_CBUFFER___ &Buffer )
		{
			C_().Process( xdhcmn::fGetContent, &Buffer, Id.Internal()( ) );

			return Buffer;
		}
		const str::string_ &GetContent(
			const nstring___ &Id,
			str::string_ &Content )
		{
		qRH
			TOL_CBUFFER___ Buffer;
		qRB
			Content.Append( GetContent( Id, Buffer ) );
		qRR
		qRT
		qRE
			return Content;
		}
		void SetContent(
			const nstring___ &Id,
			const nstring___ &Value )
		{
			C_().Process( xdhcmn::fSetContent, NULL, Id.Internal()( ), Value.Internal()( ) );
		}
		void Focus( const nstring___ &Id )
		{
			C_().Process( xdhcmn::fFocus, NULL, Id.Internal()( ) );
		}
	};

	class corpus_callback__
	{
	protected:
		virtual void XDHDWSDump( xml::writer_ &Writer ) = 0;
	public:
		void reset( bso::bool__ = true )
		{
			//Standardisation.
		}
		E_CVDTOR( corpus_callback__ )
		void Init( void )
		{
			//Standardisation.
		}
		void Dump( xml::writer_ &Writer )
		{
			XDHDWSDump( Writer );
		}
	};

	class generic_rack___
	{
	private:
		flx::E_STRING_TOFLOW___ _Flow;
		xml::writer _Writer;
	public:
		void reset( bso::bool__ P = true )
		{
			_Writer.reset( P );
			_Flow.reset( P );
		}
		E_CDTOR( generic_rack___ );
		void Init(
			const char *Generator,
			const char *View,
			const char *Background,
			str::string_ &Target,
			corpus_callback__ &Callback )
		{
			tol::buffer__ Buffer;

			_Flow.Init( Target );
			_Writer.Init( _Flow, xml::oIndent, xml::e_Default );
			_Writer.PushTag( "RichFrontEnd" );
			_Writer.PutAttribute("View", View );
			_Writer.PutAttribute("Background", Background );
			_Writer.PutAttribute("Generator", Generator );
			_Writer.PutAttribute("TimeStamp", tol::DateAndTime( Buffer ) );
			_Writer.PutAttribute( "OS", cpe::GetOSDigest() );
			_Writer.PushTag( "Corpus" );
			Callback.Dump( _Writer );
			_Writer.PopTag();
			_Writer.PushTag( Background );
		}
		operator xml::writer_ &()
		{
			return _Writer;
		}
		xml::writer_ &operator()( void )
		{
			return _Writer;
		}
	};

	class content_rack___
	: public generic_rack___
	{
	public:
		void Init(
			const char *Generator,
			const char *View,
			str::string_ &Target,
			corpus_callback__ &Callback )
		{
			generic_rack___::Init( Generator, View, ContentTagName, Target, Callback );
		}
	};

	class context_rack___
	: public generic_rack___
	{
	public:
		void Init(
			const char *Generator,
			const char *View,
			str::string_ &Target,
			corpus_callback__ &Callback )
		{
			generic_rack___::Init( Generator, View, ContextTagName, Target, Callback );
		}
	};

# define XDHDWS_RACK( Generator, Type )\
	class _##Type##_rack___\
	: public xdhdws::Type##_rack___\
	{\
	public:\
		void Init(\
			const char *View,\
			str::string_ &Target,\
			xdhdws::corpus_callback__ &Callback )\
		{\
			xdhdws::Type##_rack___::Init( Generator, View, Target, Callback );\
		}\
	};

# define XDHDWS_RACKS( Generator )\
	XDHDWS_RACK( Generator, content );\
	XDHDWS_RACK( Generator, context )

	class event_callback__
	{
	public:
		void reset( bso::bool__ = true )
		{
			//Standardisation.
		}
		E_CVDTOR( event_callback__ )
		void Init( void )
		{
			// Standadisation;
		}
	};
}

#endif
