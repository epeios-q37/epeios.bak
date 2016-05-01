/*
	Copyright (C) 2015-2016 Claude SIMON (http://q37.info/contact/).

	This file is part of 'orgnzq' software.

    'orgnzq' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'orgnzq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'orgnzq'.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef FRDFRNTND__INC
# define FRDFRNTND__INC

# include "ogzinf.h"
# include "frdapi.h"

# include "frdrgstry.h"

# include "sclfrntnd.h"

# include "csducl.h"
# include "xml.h"


namespace frdfrntnd {
	typedef sclfrntnd::frontend___ _frontend___;

	SCLF_IL( Type, Id );
	SCLF_ILW( Number, Id8 );
	SCLF_I( Field, Id );
	

	class rFrontend
	: public _frontend___
	{
	private:
		wTypes Types_;
		wNumbers Numbers_;
		void GetTypes_( void );
		void DumpTypes_( xml::writer_ &Writer );
		void GetNumbers_( void );
		void DumpNumbers_( xml::writer_ &Writer );
	protected:
		virtual void FBLFRDOnConnect( void ) override
		{
			Statics.Init( *this );
			Field.Init( *this );
			Record.Init( *this );
			MyObject.Init( *this );

			GetTypes_();
			GetNumbers_();
		}
		virtual void FBLFRDOnDisconnect( void ) override
		{
			Statics.reset();
			Field.reset();
			Record.reset();
			MyObject.reset();
		}
	public:
		orgnzq::fStatics Statics;
		orgnzq::fOGZFieldCommon Field;
		orgnzq::fOGZRecordCommon Record;
		orgnzq::fOGZMyObjectCommon MyObject;
		void reset( bso::bool__ P = true )
		{
			_frontend___::reset( P );
			Types_.reset( P );
			Numbers_.reset( P );

			Statics.reset( P );
			Field.reset();
			Record.reset();
			MyObject.reset( P );
		}
		qCVDTOR( rFrontend );
		void Init(
			sclfrntnd::kernel___ &Kernel,
			const char *Language,
			fblfrd::reporting_callback__ &ReportingCallback,
			const rgstry::multi_level_registry_ &Registry )
		{
			_frontend___::Init( Kernel, Language, ReportingCallback, Registry );
			Types_.Init();
			Numbers_.Init();
		}
		void DumpCorpus( xml::writer_ &Writer )
		{
			DumpTypes_( Writer );
			DumpNumbers_( Writer );
		}
	};

}

#endif
