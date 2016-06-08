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
# include "frdmisc.h"

# include "sclfrntnd.h"

# include "csducl.h"
# include "xml.h"


namespace frdfrntnd {
	typedef sclfrntnd::frontend___ rFrontend_;

	using frdmisc::sType;
	using frdmisc::dTypes;
	using frdmisc::UndefinedType;

	SCLF_I2S( Number, Id8 );
	SCLF_I( Column, Id );
	SCLF_I( Field, Id );
	SCLF_I( Record, Id );

	SCLF_I1S( Entry, Id );
	typedef dEntrys dEntries;
	qW( Entries );
	typedef dEntrysI1S dEntriesI1S;
	qW( EntriesI1S );

	class rFrontend
	: public rFrontend_
	{
	private:
		frdmisc::wXTypes Types_;
		wNumbersI2S Numbers_;
		void GetTypes_( void );
		void DumpTypes_( xml::writer_ &Writer );
		void GetNumbers_( void );
		void DumpNumbers_( xml::writer_ &Writer );
	protected:
		virtual void FBLFRDOnConnect( void ) override
		{
			Statics.Init( *this );
			Column.Init( *this );
			Field.Init( *this );
			MyObject.Init( *this );

			GetTypes_();
			GetNumbers_();
		}
		virtual void FBLFRDOnDisconnect( void ) override
		{
			Statics.reset();
			Column.reset();
			Field.reset();
			MyObject.reset();
		}
	public:
		orgnzq::fStatics Statics;
		orgnzq::fOGZColumnCommon Column;
		orgnzq::fOGZFieldCommon Field;
		orgnzq::fOGZMyObjectCommon MyObject;
		void reset( bso::bool__ P = true )
		{
			rFrontend_::reset( P );
			Types_.reset( P );
			Numbers_.reset( P );

			Statics.reset( P );
			Column.reset( P );
			Field.reset();
			MyObject.reset( P );
		}
		qCVDTOR( rFrontend );
		void Init(
			sclfrntnd::kernel___ &Kernel,
			const char *Language,
			fblfrd::reporting_callback__ &ReportingCallback,
			const rgstry::multi_level_registry_ &Registry )
		{
			rFrontend_::Init( Kernel, Language, ReportingCallback, Registry );
			Types_.Init();
			Numbers_.Init();
		}
		void DumpCorpus( xml::writer_ &Writer )
		{
			DumpTypes_( Writer );
			DumpNumbers_( Writer );
		}
		const frdmisc::wXTypes &Types( void ) const
		{
			if ( Types_.Amount() == 0 )
				qRGnr();

			return Types_;
		}
	};
}

#endif
