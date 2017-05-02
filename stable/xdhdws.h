/*
	Copyright (C) 1999-2017 Claude SIMON (http://q37.info/contact/).

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

	static E_CDEF( char *, LayoutTagName, "Layout" );
	static E_CDEF( char *, CastingTagName, "Casting" );

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
		void SetDocumentCasting(
			const nstring___ &XML,
			const nstring___ &XSL)
		{
			C_().Process(xdhcmn::fSetCasting, NULL, nstring___().Internal()(), XML.Internal()(), XSL.Internal()());
		}
		void SetElementCasting(
			const nstring___ &Id,
			const nstring___ &XML,
			const nstring___ &XSL)
		{
			C_().Process(xdhcmn::fSetCasting, NULL, Id.Internal()(), XML.Internal()(), XSL.Internal()());
		}
		void SetDocumentData( void )
		{
			C_().Process(xdhcmn::fSetData, NULL, nstring___().Internal()() );
		}
		void SetElementData( const nstring___ &Id )
		{
			C_().Process(xdhcmn::fSetData, NULL, Id.Internal()() );
		}
		void SetElementLayout(
			const nstring___ &Id,
			const nstring___ &XML,
			const nstring___ &XSL )
		{
			C_().Process( xdhcmn::fSetLayout, NULL, Id.Internal()( ), XML.Internal()( ), XSL.Internal()( ) );
		}
		void SetDocumentLayout(
			const nstring___ &XML,
			const nstring___ &XSL )
		{
			C_().Process(xdhcmn::fSetLayout, NULL, XML.Internal()( ), XSL.Internal()( ) );
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
		/*
		template <typename type> bso::sBool GetNumericalProperty(
			const nstring___ &Id,
			const nstring___ &Name,
			type &Value )
		{
			bso::sBool IsEmpty = true;
		qRH
			str::wString RawValue;
		qRB
			RawValue.Init();

			GetProperty( Id, Name, RawValue );

			if ( RawValue.Amount() != 0 ) {
				RawValue.ToNumber( Value );
				IsEmpty = false;
			}
		qRR
		qRT
		qRE
			return !IsEmpty;
		}
		*/
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
		/*
		template <typename type> bso::sBool GetNumericalAttribute(
			const nstring___ &Id,
			const nstring___ &Name,
			type &Value )
		{
			bso::sBool IsEmpty = true;
		qRH
			str::wString RawValue;
		qRB
			RawValue.Init();

			GetAttribute( Id, Name, RawValue );

			if ( RawValue.Amount() != 0 ) {
				RawValue.ToNumber( Value );
				IsEmpty = false;
			}
		qRR
		qRT
		qRE
			return !IsEmpty;
		}
		*/
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
		template <typename type> bso::sBool GetNumericalContent(
			const nstring___ &Id,
			type &Value,
			bso::sBool *Error = NULL )
		{
			bso::sBool IsEmpty = true;
		qRH
			str::wString RawValue;
			sdr::sRow ErrPos = qNIL;
		qRB
			RawValue.Init();

			GetContent( Id, RawValue );

			if ( RawValue.Amount() != 0 ) {
				RawValue.ToNumber( Value, Error != NULL ? &ErrPos : NULL );

				if ( ErrPos != qNIL ) {
					if ( Error != NULL )
						*Error = true;
					else
						qRFwk();
				} else
					IsEmpty = false;
			}
		qRR
		qRT
		qRE
			return !IsEmpty;
		}
		bso::sBool GetBooleanContent( const nstring___ &Id )
		{
			bso::sBool Value = false;
		qRH
			str::wString RawValue;
		qRB
			RawValue.Init();

			GetContent( Id, RawValue );

			if ( RawValue.Amount() != 0 ) {
				Value = RawValue == "true";
			}
		qRR
		qRT
		qRE
			return Value;
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
		void Select( const nstring___ &Id )
		{
			C_().Process( xdhcmn::fSelect, NULL, Id.Internal()( ) );
		}
	};

	class cCorpus
	{
	protected:
		virtual void XDHDWSDump( xml::writer_ &Writer ) = 0;
	public:
		qCALLBACK( Corpus );
		void Dump( xml::writer_ &Writer )
		{
			XDHDWSDump( Writer );
		}
	};

	class rGenericRack
	{
	private:
		str::wString Target_;
		mutable flx::E_STRING_TOFLOW___ _Flow;
		xml::writer _Writer;
	public:
		void reset( bso::bool__ P = true )
		{
			tol::reset( P, Target_, _Flow, _Writer );
		}
		E_CDTOR( rGenericRack );
		void Init(
			const char *Generator,
			const char *View,
			const char *Background,
			cCorpus &Callback )
		{
			tol::buffer__ Buffer;

			Target_.Init();
			_Flow.Init( Target_ );
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
		const str::dString &Target(void)
		{
			_Writer.reset();	// To close all tags.
			_Flow.Commit();

			return Target_;
		}
	};

	class rLayoutRack
	: public rGenericRack
	{
	public:
		void Init(
			const char *Generator,
			const char *View,
			cCorpus &Callback )
		{
			rGenericRack::Init( Generator, View, LayoutTagName, Callback );
		}
	};

	class rCastingRack
	: public rGenericRack
	{
	public:
		void Init(
			const char *Generator,
			const char *View,
			cCorpus &Callback )
		{
			rGenericRack::Init( Generator, View, CastingTagName, Callback );
		}
	};

# define XDHDWS_RACK( Generator, Type )\
	class r##Type##Rack_\
	: public xdhdws::r##Type##Rack\
	{\
	public:\
		void Init(\
			const char *View,\
			xdhdws::cCorpus &Callback )\
		{\
			xdhdws::r##Type##Rack::Init( Generator, View, Callback );\
		}\
	};

# define XDHDWS_RACKS( Generator )\
	XDHDWS_RACK( Generator, Layout );\
	XDHDWS_RACK( Generator, Casting )
}

#endif
