/*
	Copyright (C) 1999 Claude SIMON (http://q37.info/contact/).

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

#ifndef XDHDWS_INC_
# define XDHDWS_INC_

# define XDHDWS_NAME		"XDHDWS"

# if defined( E_DEBUG ) && !defined( XDHDWS_NODBG )
#  define XDHDWS_DBG
# endif

// X(SL) DH(TML) DowNStream

# include "xdhcmn.h"

# include "err.h"
# include "tol.h"

# define XDHDWS_DEFAULT_SUFFIX  "xdh"

namespace xdhdws {
	using xdhcmn::nchar__;
	using xdhcmn::nstring___;
	using xdhcmn::rNString;

	class sProxy
	{
	private:
		Q37_MRMDF( xdhcmn::cUpstream, C_, Callback_ );
	public:
		void reset( bso::bool__ P = true )
		{
			Callback_ = NULL;
		}
		E_CVDTOR( sProxy );
		void Init( xdhcmn::cUpstream &Callback )
		{
			reset();

			Callback_ = &Callback;
		}
		void Process(
            const char *ScriptName,
            const str::dStrings &Values,
            str::dString &ReturnValue )
        {
            return C_().Process(ScriptName, Values, ReturnValue );
        }
        void Process(
            const str::dString &ScriptName,
            const str::dStrings &Values,
            str::dString &ReturnValue )
        {
        qRH
            qCBUFFERr Buffer;
        qRB
            Process(ScriptName.Convert(Buffer),Values,ReturnValue);
        qRR
        qRE
        qRT
        }
// Deprecated
# if 0
		const char *Execute(
			const nstring___ &Script,
			TOL_CBUFFER___ &Buffer )
		{
			C_().Process( xdhcmn::fExecute, &Buffer, Script.Internal()() );

			return Buffer;
		}
		const str::dString &Execute(
			const nstring___ &Script,
			str::dString &Result )
		{
		qRH;
			qCBUFFERr Buffer;
		qRB;
			Result.Append( Execute( Script, Buffer ) );
		qRR;
		qRT;
		qRE;
			return Result;
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
			C_().Process( xdhcmn::fConfirm, &Buffer, XML.Internal()(), XSL.Internal()(), Title.Internal()(), CloseText.Internal()() );

			Confirmed = !strcmp( "true", Buffer );
		qRR
		qRT
		qRE
			return Confirmed;
		}
		void PrependLayout(
			const nstring___ &Id,
			const nstring___ &XML,
			const nstring___ &XSL)
		{
			C_().Process(xdhcmn::fPrependLayout, NULL, Id.Internal()(), XML.Internal()(), XSL.Internal()());
		}
		void SetLayout(
			const nstring___ &Id,
			const nstring___ &XML,
			const nstring___ &XSL)
		{
			C_().Process(xdhcmn::fSetLayout, NULL, Id.Internal()(), XML.Internal()(), XSL.Internal()());
		}
		void AppendLayout(
			const nstring___ &Id,
			const nstring___ &XML,
			const nstring___ &XSL)
		{
			C_().Process(xdhcmn::fAppendLayout, NULL, Id.Internal()(), XML.Internal()(), XSL.Internal()());
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
		void SetTimeout(
			const rNString &Delay,
			const rNString &Action )
		{
			C_().Process( xdhcmn::fSetTimeout, NULL, Delay.Internal()(), Action.Internal()() );
		}
		const char *Parent(
			const nstring___ &Id,
			qCBUFFERr &Buffer )
		{
			C_().Process( xdhcmn::fParent, &Buffer, Id.Internal()() );

			return Buffer;
		}
		const char *FirstChild(
			const nstring___ &Id,
			qCBUFFERr &Buffer )
		{
			C_().Process( xdhcmn::fFirstChild, &Buffer, Id.Internal()() );

			return Buffer;
		}
		const char *LastChild(
			const nstring___ &Id,
			qCBUFFERr &Buffer )
		{
			C_().Process( xdhcmn::fLastChild, &Buffer, Id.Internal()() );

			return Buffer;
		}
		const char *PreviousSibling(
			const nstring___ &Id,
			qCBUFFERr &Buffer )
		{
			C_().Process( xdhcmn::fPreviousSibling, &Buffer, Id.Internal()() );

			return Buffer;
		}
		const char *NextSibling(
			const nstring___ &Id,
			qCBUFFERr &Buffer )
		{
			C_().Process( xdhcmn::fNextSibling, &Buffer, Id.Internal()() );

			return Buffer;
		}
		const char *CreateElement(
			const nstring___ &Name,
			const nstring___ &Id,
			qCBUFFERr &Buffer )
		{
			C_().Process( xdhcmn::fCreateElement, &Buffer, Name.Internal()(), Id.Internal()() );

			return Buffer;
		}
		void InsertChild(
			const nstring___ &Child,
			const nstring___ &Id )
		{
			C_().Process( xdhcmn::fInsertChild, NULL, Child.Internal()(), Id.Internal()() );
		}
		void AppendChild(
			const nstring___ &Child,
			const nstring___ &Id )
		{
			C_().Process( xdhcmn::fAppendChild, NULL, Child.Internal()(), Id.Internal()() );
		}
		void InsertBefore(
			const nstring___ &Sibling,
			const nstring___ &Id )
		{
			C_().Process( xdhcmn::fInsertBefore, NULL, Sibling.Internal()(), Id.Internal()() );
		}
		void InsertAfter(
			const nstring___ &Sibling,
			const nstring___ &Id )
		{
			C_().Process( xdhcmn::fInsertAfter, NULL, Sibling.Internal()(), Id.Internal()() );
		}
		void InsertCSSRule(
			const rNString &Id,
			const rNString &Rule,
			const rNString &Index)
		{
			C_().Process( xdhcmn::fInsertCSSRule, NULL, Id.Internal()(), Rule.Internal()(), Index.Internal()() );
		}
		const char *AppendCSSRule(
			const rNString &Id,
			const rNString &Rule,
			qCBUFFERr &Buffer )
		{
			C_().Process( xdhcmn::fAppendCSSRule, &Buffer, Id.Internal()(), Rule.Internal()() );

			return Buffer;
		}
		void RemoveCSSRule(
			const rNString &Id,
			const rNString &Index)
		{
			C_().Process( xdhcmn::fRemoveCSSRule, NULL, Id.Internal()(), Index.Internal()() );
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
		const char *Dummy(
			const nstring___ &Id,
			qCBUFFERr &Buffer )
		{
			C_().Process( xdhcmn::fDummy, &Buffer, Id.Internal()() );

			return Buffer;
		}
# endif
	};
}

#endif
