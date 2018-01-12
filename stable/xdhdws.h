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
	using xdhcmn::rNString;

	class sProxy
	{
	private:
		Q37_MRMDF( xdhcmn::cProxy, C_, Callback_ );
	public:
		void reset( bso::bool__ P = true )
		{
			if ( P )
				if ( Callback_ != NULL )
					delete( Callback_ );

			Callback_ = NULL;
		}
		E_CVDTOR( sProxy );
		void Init( xdhcmn::cProxy *Callback )	// 'Callback' is destroyed by a 'delete' when this object is dsetroyed.
		{
			reset();

			Callback_ = Callback;
		}
		const char *Execute(
			const nstring___ &Message,
			TOL_CBUFFER___ &Buffer )
		{
			C_().Process( xdhcmn::fExecute, &Buffer, Message.Internal()() );

			return Buffer;
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
		void SetLayout(
			const nstring___ &Id,
			const nstring___ &XML,
			const nstring___ &XSL )
		{
			C_().Process( xdhcmn::fSetLayout, NULL, Id.Internal()( ), XML.Internal()( ), XSL.Internal()( ) );
		}
		void DressWidgets( const nstring___ &Id )
		{
			C_().Process(xdhcmn::fDressWidgets, NULL, Id.Internal()());
		}
		void SetContents(
			const nstring___ &Ids,
			const nstring___ &Contents )
		{
			C_().Process(xdhcmn::fSetContents, NULL, Ids.Internal()(), Contents.Internal()() );
		}
		void InsertCSSRule(
			const rNString &Rule,
			xdhcmn::sIndex Index )
		{
		qRH;
			rNString SBuffer;
			bso::bInteger IBuffer;
		qRB;
			SBuffer.Init( bso::Convert( *Index, IBuffer ) );
			C_().Process( xdhcmn::fInsertCSSRule, NULL, Rule.Internal()(), SBuffer.Internal()() );
		qRR;
		qRT;
		qRE;
		}
		xdhcmn::sIndex AppendCSSRule( const rNString &Rule )
		{
			xdhcmn::sIndex Index = 0;
		qRH;
			TOL_CBUFFER___ Buffer;
		qRB;
			C_().Process( xdhcmn::fAppendCSSRule, &Buffer, Rule.Internal()() );

			str::wString( Buffer ).ToNumber( *Index );
		qRR;
		qRT;
		qRE;
			return Index;
		}
		void RemoveCSSRule( xdhcmn::sIndex Index )
		{
		qRH;
			rNString SBuffer;
			bso::bInteger Buffer;
		qRB;
			SBuffer.Init( bso::Convert( *Index, Buffer ) );

			C_().Process( xdhcmn::fRemoveCSSRule, NULL, SBuffer.Internal()() );
		qRR;
		qRT;
		qRE;
		}
		void AddClasses(
			const rNString &Ids,
			const rNString &Classes )
		{
			C_().Process( xdhcmn::fAddClasses, NULL, Ids.Internal()(), Classes.Internal()() );
		}
		void RemoveClasses(
			const rNString &Ids,
			const rNString &Classes )
		{
			C_().Process( xdhcmn::fRemoveClasses, NULL, Ids.Internal()(), Classes.Internal()() );
		}
		void ToggleClasses(
			const rNString &Ids,
			const rNString &Classes )
		{
			C_().Process( xdhcmn::fToggleClasses, NULL, Ids.Internal()(), Classes.Internal()() );
		}
		void EnableElements( const rNString &Ids )
		{
			C_().Process( xdhcmn::fEnableElements, NULL, Ids.Internal()() );
		}
		void DisableElements( const rNString &Ids )
		{
			C_().Process( xdhcmn::fDisableElements, NULL, Ids.Internal()() );
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
		const char *GetValue(
			const nstring___ &Id,
			TOL_CBUFFER___ &Buffer )
		{
			C_().Process( xdhcmn::fGetValue, &Buffer, Id.Internal()( ) );

			return Buffer;
		}
		const str::string_ &GetValue(
			const nstring___ &Id,
			str::string_ &Value )
		{
		qRH
			TOL_CBUFFER___ Buffer;
		qRB
			Value.Append( GetValue( Id, Buffer ) );
		qRR
		qRT
		qRE
			return Value;
		}
		template <typename type> bso::sBool GetNumericalValue(
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

			GetValue( Id, RawValue );

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
		bso::sBool GetBooleanValue( const nstring___ &Id )
		{
			bso::sBool Value = false;
		qRH
			str::wString RawValue;
		qRB
			RawValue.Init();

			GetValue( Id, RawValue );

			if ( RawValue.Amount() != 0 ) {
				Value = RawValue == "true";
			}
		qRR
		qRT
		qRE
			return Value;
		}
		void SetValue(
			const nstring___ &Id,
			const nstring___ &Value )
		{
			C_().Process( xdhcmn::fSetValue, NULL, Id.Internal()( ), Value.Internal()( ) );
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
}

#endif
