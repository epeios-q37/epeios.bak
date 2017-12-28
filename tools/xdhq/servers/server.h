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

#ifndef SERVER_INC_
# define SERVER_INC_

# include "prtcl.h"

# include "flw.h"
# include "str.h"

namespace server {
	void Handshake(
		flw::sRFlow &Flow,
		str::dString &Language );

	void GetAction(
		flw::sRWFlow &Flow,
		str::dString &Id,
		str::dString &Action );

	// '...S' Send the request.
	// '...R' get the response.

	namespace alert {
		void S(
			const str::dString &Message,
			flw::sWFlow &Flow );

		inline void R( flw::sRFlow &Flow )
		{}
	}

	inline void Alert(
		const str::dString &Script,
		flw::sRWFlow &Flow )
	{
		alert::S( Script, Flow );
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
		const str::dString &Script,
		flw::sRWFlow &Flow,
		str::dString &Response )
	{
		confirm::S( Script, Flow );
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

	namespace casts_by_ids {
		namespace set 
		{ 
			void S(
				const str::dStrings &Ids,
				const str::dStrings &Values,
				flw::sWFlow &Flow );

			inline void R( flw::sRFlow &Flow )
			{}
		}
		
		inline void Set(
			const str::dStrings &Ids,
			const str::dStrings &Values,
			flw::sRWFlow &Flow )
		{
			set::S( Ids, Values, Flow );
			set::R( Flow );
		}
	}

	namespace casts_by_tags {
		namespace set {
			void S(
				const str::dString &Id,
				const str::dStrings &Tags,
				const str::dStrings &Values,
				flw::sWFlow &Flow );

			inline void R( flw::sRFlow &Flow )
			{}
		}

		inline void Set(
			const str::dString &Id,
			const str::dStrings &Tags,
			const str::dStrings &Values,
			flw::sRWFlow &Flow )
		{
			set::S( Id, Tags, Values, Flow );
			set::R( Flow );
		}
	}

	namespace ap_ {
		namespace set {
			void S(
				prtcl::eAnswer Answer,
				const str::dString &Id,
				const str::dString &Name,
				const str::dString &Value,
				flw::sWFlow &Flow );

			inline void R( flw::sRFlow &Flow )
			{}
		}

		inline void Set(
			prtcl::eAnswer Answer,
			const str::dString &Id,
			const str::dString &Name,
			const str::dString &Value,
			flw::sRWFlow &Flow )
		{
			set::S( Answer, Id, Name, Value, Flow );
			set::R( Flow );
		}

		namespace get {
			void S(
				prtcl::eAnswer Answer,
				const str::dString &Id,
				const str::dString &Name,
				flw::sWFlow &Flow );

			void R(
				flw::sRFlow &Flow,
				str::dString &Value );
		}

		inline void Get(
			prtcl::eAnswer Answer,
			const str::dString &Id,
			const str::dString &Name,
			flw::sRWFlow &Flow,
			str::dString &Value )
		{
			get::S( Answer, Id, Name, Flow );
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
				ap_::set::S( prtcl::aSetAttribute_1, Id, Name, Value, Flow );
			}

			inline void R( flw::sRFlow &Flow )
			{
				ap_::set::R( Flow );
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
		}

		namespace get {
			inline void S(
				const str::dString &Id,
				const str::dString &Name,
				flw::sWFlow &Flow )
			{
				ap_::get::S( prtcl::aGetAttribute_1, Id, Name, Flow );
			}

			inline void R(
				flw::sRFlow &Flow,
				str::dString &Value )
			{
				ap_::get::R( Flow, Value );
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
				ap_::set::S( prtcl::aSetProperty_1, Id, Name, Value, Flow );
			}

			inline void R( flw::sRFlow &Flow )
			{
				ap_::set::R( Flow );
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
		}

		namespace get {
			inline void S(
				const str::dString &Id,
				const str::dString &Name,
				flw::sWFlow &Flow )
			{
				ap_::get::S( prtcl::aGetProperty_1, Id, Name, Flow );
			}

			inline void R(
				flw::sRFlow &Flow,
				str::dString &Value )
			{
				ap_::get::R( Flow, Value );
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
		}
	}
}

#endif