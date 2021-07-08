/*
	Copyright (C) 2021 Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of 'TaskQ' software.

    'TaskQ' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'TaskQ' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'TaskQ'.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef FRDFRNTND_INC_
# define FRDFRNTND_INC_

# include "frdapi.h"

# include "frdrgstry.h"

# include "sclf.h"

# include "csducl.h"
# include "xml.h"


namespace frdfrntnd {
	using sclf::rKernel;
	typedef sclf::rFrontend rFrontend_;
	typedef fblfrd::cFrontend cFrontend_;

	using fbltyp::dString;
	using fbltyp::dStrings;

	class rFrontend
	: public rFrontend_,
	  public cFrontend_
	{
	protected:
		virtual void FBLFRDOnConnection( void ) override
		{
			Statics.Init( *this );
			MyObject.Init( *this );
		}
		virtual void FBLFRDOnDisconnection( void ) override
		{
			Statics.reset();
			MyObject.reset();
		}
	public:
		taskq::fStatics Statics;
		taskq::fTSKMyObjectCommon MyObject;
		void reset( bso::bool__ P = true )
		{
			rFrontend_::reset( P );
			Statics.reset( P );
			MyObject.reset( P );
		}
		qCVDTOR( rFrontend );
		void Init(
			rKernel &Kernel,
			const char *Language,
			fblfrd::cReporting &ReportingCallback )
		{
			rFrontend_::Init( Kernel, Language, *this, ReportingCallback );
		}
		void LoadSetupOfId( const dString &Id )
		{
			Statics.LoadSetupOfId_1( Id );
		}
		void LoadSetupContent( const dString &Content )
		{
			Statics.LoadSetupContent_1( Content );
		}
	};

	class rMyObject
	{
	private:
		taskq::rTSKMyObject Object_;
	public:
		void reset( bso::bool__ P = true )
		{
			Object_.reset( P );
		}
		E_CVDTOR( rMyObject );
		void Init( frdfrntnd::rFrontend &Frontend )
		{
			Object_.Init( Frontend.MyObject );
		}
		dString &ToUpper( dString &String );
		void TestMessage( void )
		{
			Object_.Test_1();
		}
	};


}

#endif
