/*
	Copyright (C) 2017 Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of 'XDHq' software.

    'XDHq' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'XDHq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'XDHq'.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef PRXY_CMN_INC_
# define PRXY_CMN_INC_

# include "prtcl.h"

# include "str.h"

namespace prxy_cmn {

	qENUM( Request )
	{
		rExecute,
		rAlert,
		rConfirm,
		rSetLayout,
		rGetContents,
		rSetContents,
		rDressWidgets,
		rAddClasses,
		rRemoveClasses,
		rToggleClasses,
		rEnableElements,
		rDisableElements,
		rSetAttribute,
		rGetAttribute,
		rRemoveAttribute,
		rSetProperty,
		rGetProperty,
		rFocus_,
		rNew,
		r_amount,
		r_Undefined
	};

	// '...S' Send the request.
	// '...R' get the response.

	namespace execute {
		void S(
			const str::dString &Script,
			flw::sWFlow &Flow );

		void R(
			flw::sRFlow &Flow,
			str::dString &Return );
	}

	inline void Execute(
		const str::dString &Script,
		flw::sRWFlow &Flow,
		str::dString &Return )
	{
		execute::S( Script, Flow );
		execute::R( Flow, Return );
	}

	namespace alert {
		void S(
			const str::dString &Message,
			flw::sWFlow &Flow );

		inline void R( flw::sRFlow &Flow )
		{}
	}

	inline void Alert(
		const str::dString &Message,
		flw::sRWFlow &Flow )
	{
		alert::S( Message, Flow );
		alert::R( Flow );
	}

	namespace confirm {
		void S(
			const str::dString &Message,
			flw::sWFlow &Flow );

		void R(
			flw::sRFlow &Flow,
			str::dString &Response );
	}

	inline void Confirm(
		const str::dString &Message,
		flw::sRWFlow &Flow,
		str::dString &Response )
	{
		confirm::S( Message, Flow );
		confirm::R( Flow, Response );
	}

	namespace layout {
		namespace set {
			void S(
				const str::dString &Id,
				const str::dString &XML,
				const str::dString &XSL,
				const str::dString &Language,
				flw::sWFlow &Flow );

			inline void R( flw::sRFlow &Flow )
			{}
		}

		inline void Set(
			const str::dString &Id,
			const str::dString &XML,
			const str::dString &XSL,
			const str::dString &Language,
			flw::sRWFlow &Flow )
		{
			set::S( Id, XML, XSL, Language, Flow );
			set::R( Flow );
		}
	}

	namespace contents {
		namespace set {
			void S(
				const str::dStrings &Ids,
				const str::dStrings &Contents,
				flw::sWFlow &Flow );

			inline void R( flw::sRFlow &Flow )
			{}
		}

		inline void Set(
			const str::dStrings &Ids,
			const str::dStrings &Contents,
			flw::sRWFlow &Flow )
		{
			set::S( Ids, Contents, Flow );
			set::R( Flow );
		}

		namespace get {
			void S(
				const str::dStrings &Ids,
				flw::sWFlow &Flow );

			void R(
				flw::sRFlow &Flow,
				str::dStrings &Contents );
		}

		inline void Get(
			const str::dStrings &Id,
			flw::sRWFlow &Flow,
			str::dStrings &Contents )
		{
			get::S( Id, Flow );
			get::R( Flow, Contents );
		}
	}

	namespace widgets {
		namespace dress {
			void S(
				const str::dString &Id,
				flw::sWFlow &Flow );

			inline void R( flw::sRFlow &Flow )
			{}
		}

		inline void Dress(
			const str::dString &Id,
			flw::sRWFlow &Flow )
		{
			dress::S( Id, Flow );
			dress::R( Flow );
		}
	}

	namespace classes {
		namespace add {
			void S(
				const str::dStrings &Ids,
				const str::dStrings &Classes,
				flw::sWFlow &Flow );

			inline void R( flw::sRFlow &Flow )
			{}
		}

		inline void Add(
			const str::dStrings &Ids,
			const str::dStrings &Classes,
			flw::sRWFlow &Flow )
		{
			add::S( Ids, Classes, Flow );
			add::R( Flow );
		}

		namespace remove {
			void S(
				const str::dStrings &Ids,
				const str::dStrings &Classes,
				flw::sWFlow &Flow );

			inline void R( flw::sRFlow &Flow )
			{}
		}

		inline void Remove(
			const str::dStrings &Ids,
			const str::dStrings &Classes,
			flw::sRWFlow &Flow )
		{
			remove::S( Ids, Classes, Flow );
			remove::R( Flow );
		}

		namespace toggle {
			void S(
				const str::dStrings &Ids,
				const str::dStrings &Classes,
				flw::sWFlow &Flow );

			inline void R( flw::sRFlow &Flow )
			{}
		}

		inline void Toggle(
			const str::dStrings &Ids,
			const str::dStrings &Classes,
			flw::sRWFlow &Flow )
		{
			toggle::S( Ids, Classes, Flow );
			toggle::R( Flow );
		}
	}

	namespace elements {
		namespace enable {
			void S(
				const str::dStrings &Ids,
				flw::sWFlow &Flow );

			inline void R( flw::sRFlow &Flow )
			{}
		}

		inline void Enable(
			const str::dStrings &Ids,
			flw::sRWFlow &Flow )
		{
			enable::S( Ids, Flow );
			enable::R( Flow );
		}

		namespace disable {
			void S(
				const str::dStrings &Ids,
				flw::sWFlow &Flow );

			inline void R( flw::sRFlow &Flow )
			{}
		}

		inline void Disable(
			const str::dStrings &Id,
			flw::sRWFlow &Flow )
		{
			disable::S( Id, Flow );
			disable::R( Flow );
		}
	}

	namespace ap_ {
		namespace set {
			void S(
				prtcl::eCommand Command,
				const str::dString &Id,
				const str::dString &Name,
				const str::dString &Value,
				flw::sWFlow &Flow );

			inline void R( flw::sRFlow &Flow )
			{}
		}

		inline void Set(
			prtcl::eCommand Command,
			const str::dString &Id,
			const str::dString &Name,
			const str::dString &Value,
			flw::sRWFlow &Flow )
		{
			set::S( Command, Id, Name, Value, Flow );
			set::R( Flow );
		}

		namespace get {
			void S(
				prtcl::eCommand Command,
				const str::dString &Id,
				const str::dString &Name,
				flw::sWFlow &Flow );

			void R(
				flw::sRFlow &Flow,
				str::dString &Value );
		}

		inline void Get(
			prtcl::eCommand Command,
			const str::dString &Id,
			const str::dString &Name,
			flw::sRWFlow &Flow,
			str::dString &Value )
		{
			get::S( Command, Id, Name, Flow );
			get::R( Flow, Value );
		}

	}

	namespace attribute {
		namespace set {
			inline void S(
				const str::dString &Id,
				const str::dString &Name,
				const str::dString &Value,
				flw::sWFlow &Flow )
			{
				ap_::set::S( prtcl::cSetAttribute_1, Id, Name, Value, Flow );
			}

			inline void R( flw::sRFlow &Flow )
			{
				ap_::set::R( Flow );
			}
		}

		inline void Set(
			const str::dString &Id,
			const str::dString &Name,
			const str::dString &Value,
			flw::sRWFlow &Flow )
		{
			set::S( Id, Name, Value, Flow );
			set::R( Flow );
		}

		namespace get {
			inline void S(
				const str::dString &Id,
				const str::dString &Name,
				flw::sWFlow &Flow )
			{
				ap_::get::S( prtcl::cGetAttribute_1, Id, Name, Flow );
			}

			inline void R(
				flw::sRFlow &Flow,
				str::dString &Value )
			{
				ap_::get::R( Flow, Value );
			}
		}

		inline void Get(
			const str::dString &Id,
			const str::dString &Name,
			flw::sRWFlow &Flow,
			str::dString &Value )
		{
			get::S( Id, Name, Flow );
			get::R( Flow, Value );
		}

		namespace remove {
			void S(
				const str::dString &Id,
				const str::dString &Name,
				flw::sWFlow &Flow );

			inline void R( flw::sRFlow &Flow )
			{}
		}

		inline void Remove(
			const str::dString &Id,
			const str::dString &Name,
			flw::sRWFlow &Flow )
		{
			remove::S( Id, Name, Flow );
			remove::R( Flow );
		}
	}

	namespace property {
		namespace set {
			inline void S(
				const str::dString &Id,
				const str::dString &Name,
				const str::dString &Value,
				flw::sWFlow &Flow )
			{
				ap_::set::S( prtcl::cSetProperty_1, Id, Name, Value, Flow );
			}

			inline void R( flw::sRFlow &Flow )
			{
				ap_::set::R( Flow );
			}
		}

		inline void Set(
			const str::dString &Id,
			const str::dString &Name,
			const str::dString &Value,
			flw::sRWFlow &Flow )
		{
			set::S( Id, Name, Value, Flow );
			set::R( Flow );
		}

		namespace get {
			inline void S(
				const str::dString &Id,
				const str::dString &Name,
				flw::sWFlow &Flow )
			{
				ap_::get::S( prtcl::cGetProperty_1, Id, Name, Flow );
			}

			inline void R(
				flw::sRFlow &Flow,
				str::dString &Value )
			{
				ap_::get::R( Flow, Value );
			}
		}

		inline void Get(
			const str::dString &Id,
			const str::dString &Name,
			flw::sRWFlow &Flow,
			str::dString &Value )
		{
			get::S( Id, Name, Flow );
			get::R( Flow, Value );
		}
	}

	namespace focus {
		void S(
			const str::dString &Id,
			flw::sWFlow &Flow );

		inline void R( flw::sRFlow &Flow )
		{}
	}

	inline void Focus(
		const str::dString &Id,
		flw::sRWFlow &Flow )
	{
		focus::S( Id, Flow );
		focus::R( Flow );
	}

}

#endif