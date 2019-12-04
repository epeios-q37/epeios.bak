/*
	Copyright (C) 2016-2017 Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of 'MMUAq' software.

    'MMUAq' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'MMUAq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'MMUAq'.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef BASE_INC_
# define BASE_INC_

# include "sclxdhtml.h"

# include "muafbc.h"
# include "muainf.h"

# include "xdhdws.h"

# define BASE_NAME				MUAINF_LC_AFFIX	SCLXDHTML_DEFAULT_SUFFIX
# define BASE_AUTHOR_NAME		MUAINF_AUTHOR_NAME
# define BASE_AUTHOR_CONTACT	MUAINF_AUTHOR_CONTACT
# define BASE_OWNER_NAME		MUAINF_OWNER_NAME
# define BASE_OWNER_CONTACT		MUAINF_OWNER_CONTACT
# define BASE_COPYRIGHT_YEARS	COPYRIGHT_YEARS
# define BASE_COPYRIGHT			BASE_COPYRIGHT_YEARS " " BASE_OWNER_NAME " (" BASE_OWNER_CONTACT ")"
# define BASE_SOFTWARE_NAME		MUAINF_SOFTWARE_NAME
# define BASE_SOFTWARE_VERSION	MUAINF_SOFTWARE_VERSION
# define BASE_SOFTWARE_DETAILS	MUAINF_SOFTWARE_NAME " V" MUAINF_SOFTWARE_VERSION
# define BASE_SOFTWARE_URL		MUAINF_SOFTWARE_URL


/***** Macros dealing with actions ****/
// Declaration.
# define BASE_ACD( name )\
	extern class s##name\
	: public base::cAction\
	{\
	protected:\
		virtual void SCLXLaunch(\
			core::rSession &Session,\
			const char *Id ) override;\
	public:\
		static const char *Name;\
	} name

// Registering.
# define BASE_ACR( name )\
	base::Register( s##name::Name, name );

// Definition.
# define BASE_AC( owner, name )\
	owner::s##name owner::name;\
	const char *owner::s##name::Name = #name;\
	void owner::s##name::SCLXLaunch(\
		core::rSession &Session,\
		const char *Id )
/**********/

// Predeclaration.

namespace core {
	class rSession;
}

namespace base {
	extern const char *Name;

	typedef sclxdhtml::cAction<core::rSession> cAction;

	void Register(
		const char *Name,
		cAction &Callback );

	class sActionHelper
	: public sclxdhtml::cActionHelper<core::rSession>
	{
	protected:
		virtual bso::bool__ SCLXOnBeforeAction(
			core::rSession &Session,
			const char *Id,
			const char *Action ) override;
		virtual void SCLXOnRefresh( core::rSession &Session ) override;
		virtual bso::bool__ SCLXOnClose( core::rSession &Session ) override;
	};

	typedef xdhdws::cCorpus cCorpus_;

	class sCorpusCallback
	: public cCorpus_
	{
	private:
		qRMV( core::rSession, S_,  Session_ );
	protected:
		/*
		Deported to '.cpp' due to 'g++', which needs 'core::rSession' definition
		on the declaration (where 'core::rSession' is only declared, but no defined),
		and not on instanciation.
		*/
		virtual void XDHDWSDump( xml::writer_ &Writer ) override;
	public:
		void reset( bso::bool__ P = true )
		{
			tol::reset( P, Session_ );
		}
		E_CVDTOR( sCorpusCallback );
		void Init( core::rSession &Session )
		{
			Session_ = &Session;
		}
	};

	XDHDWS_RACKS( Name );

	template <typename rack> class rRack_
	: public rack
	{
	private:
		sCorpusCallback Callback_;
	public:
		void reset( bso::bool__ P = true )
		{
			rack::reset( P );
			Callback_.reset( P );
		}
		E_CDTOR( rRack_ );
		void Init(
			const char *View,
			str::string_ &Target,
			core::rSession &Session )
		{
			Callback_.Init( Session );
			rack::Init( View, Target, Callback_ );
		}
	};

	typedef rRack_<rContentRack_> rContentRack;
	typedef rRack_<rContextRack_> rContextRack;
}

#endif