/*
	Copyright (C) 2010-2015 Claude SIMON (http://q37.info/contact/).

	This file is part of dpkq.

    dpkq is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

    dpkq is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with dpkq.  If not, see <http://www.gnu.org/licenses/>
*/

#include "dpkq.h"

#include "data_d.h"

#include "data_c.h"

#include "registry.h"

#include "sclmisc.h"

#include "cio.h"

using namespace data_d;

using namespace data_c;

using namespace dpkrcd;

namespace {
	namespace {
		class dRandomValue_
		{
		public:
			struct s {
				str::string_::s Name;
				bso::uint__ Limit;
			} &S_;
			str::string_ Name;
			dRandomValue_( s &S )
			: S_( S ),
			  Name( S.Name )
			{}
			void reset( bso::bool__ P = true )
			{
				Name.reset( P );
				S_.Limit = 0;
			}
			void plug( str::sHook &Hook )
			{
				Name.plug( Hook );
			}
			void plug( qASd *AS )
			{
				Name.plug( AS );
			}
			dRandomValue_ &operator =( const dRandomValue_ &RV )
			{
				Name = RV.Name;
				S_.Limit = RV.S_.Limit;

				return *this;
			}
			void Init( void )
			{
				Name.Init();
				S_.Limit = 0;
			}
			void Init(
				const str::string_ &Name,
				bso::uint__ Limit )
			{
				this->Name.Init( Name );
				S_.Limit = Limit;
			}
			E_RODISCLOSE_( bso::uint__, Limit );
		};

		qW( RandomValue_ );

		typedef ctn::E_MCONTAINER_( dRandomValue_ ) dRandomValues_;
		qW( RandomValues_ );

		bso::uint__ GetRandomLimit_( const str::string_ &Name )
		{
			bso::uint__ Limit = 0;
		qRH
			rgstry::tags Tags;
		qRB
			Tags.Init();
			Tags.Append( Name );

			Limit = sclmisc::MGetUInt( rgstry::tentry__( registry::TaggedRandomLimit, Tags ), RAND_MAX );
		qRR
		qRT
		qRE
			return Limit;
		}

		void GetRandoms_(
			const str::strings_ &Names,
			dRandomValues_ &Randoms )
		{
		qRH
			wRandomValue_ Random;
			ctn::qCMITEMsl( str::string_ ) Name;
			sdr::row__ Row = qNIL;
		qRB
			Name.Init( Names );

			Row = Names.First();

			while ( Row != qNIL ) {
				Random.Init( Name( Row ), GetRandomLimit_( Name( Row ) ) );
				Randoms.Append( Random );

				Row = Names.Next( Row );
			}
		qRR
		qRT
		qRE
		}

		void GetRandoms_( dRandomValues_ &Randoms )
		{
		qRH
			str::strings Names;
		qRB
			Names.Init();
			sclmisc::GetValues( registry::RandomName, Names );

			GetRandoms_( Names, Randoms );
		qRR
		qRT
		qRE
		}

		void HandleRandom_(
			const dRandomValue_ &Random,
			xml::writer_ &Writer )
		{
			Writer.PushTag( "Random" );
			Writer.PutAttribute( "Name", Random.Name );
			xml::PutAttribute( "Limit", Random.Limit(), Writer );
			xml::PutAttribute( "Value", rand() % Random.GetLimit(), Writer );
			Writer.PopTag();
		}

		void HandleRandoms_(
			const dRandomValues_ &Randoms,
			xml::writer_ &Writer )
		{
			ctn::qCMITEMsl( dRandomValue_ ) Random;
			sdr::row__ Row = qNIL;

			if ( Randoms.Amount() == 0 )
				return;

			tol::InitializeRandomGenerator();

			Random.Init( Randoms );

			Row = Randoms.First();

			Writer.PushTag( "Parameters" );

			while ( Row != qNIL ) {
				HandleRandom_( Random( Row ), Writer );

				Row = Randoms.Next( Row );
			}

			Writer.PopTag();
		}
		static void Display_(
			const str::string_ &XML,
			xml::writer_ &Writer )
		{
			qRH
				flx::E_STRING_IFLOW__ IFlow;
			xtf::extended_text_iflow__ XFlow;
			qRB
				IFlow.Init( XML );
			XFlow.Init( IFlow, utf::f_Default );

			if ( xpp::Process( XFlow, xpp::criterions___( "" ), Writer ) != xpp::sOK )
				qRGnr();
			qRR
				qRT
				qRE
		}

		void Display_(
			const record_ &Record,
			xml::writer_ &Writer )
		{
			Display_( Record.Content, Writer );
		}

		static void Display_(
			rrow__ Row,
			const records_ &Records,
			str::string_ &Label,
			xml::writer_ &Writer )
		{
			ctn::qCITEMs( record_, rrow__ ) Record;

			Record.Init( Records );

			Display_( Record( Row ), Writer );

			Label = Record().Label;
		}

		static void DisplayAll_(
			const records_ &Records,
			xml::writer_ &Writer )
		{
			ctn::qCITEMs( record_, rrow__ ) Record;
			rrow__ Row = Records.First();

			Record.Init( Records );

			while ( Row != qNIL ) {
				Display_( Record( Row ), Writer );

				Row = Records.Next( Row );
			}
		}

		dpkbsc::counter__ GetSkippedAmount_(
			dpkctx::amount__ SessionAmount,
			const rrows_ &RecordRows,
			const records_ &Records )
		{
			dpkbsc::counter__ Skipped = 0;
			sdr::row__ Row = RecordRows.Last();
			rrow__ RecordRow = qNIL;
			ctn::qCITEMs( record_, rrow__ ) Record;

			Record.Init( Records );

			while ( ( Row != qNIL ) && ( SessionAmount-- ) ) {
				RecordRow = RecordRows( Row );

				if ( Records.Exists( RecordRow ) )
					if ( Record( RecordRow ).Skip() ) {
						if ( Skipped == DPKBSC_COUNTER_MAX )
							qRLmt();
						Skipped++;
					}

				Row = RecordRows.Previous( Row );
			}

			return Skipped;

		}


		sId  Display_(
			sId Id,
			dpkbsc::counter__ Skipped,
			bso::uint__ SessionMaxDuration,
			const records_ &Records,
			str::string_ &Label,
			dpkctx::context_ &Context,
			xml::writer_ &Writer )
		{
		qRH
			rrow__ Row = qNIL;
			bso::integer_buffer__ Buffer;
			ctn::qCITEMs( record_, rrow__ ) Record;
			dpkbsc::counter__ Counter = 0;
			lcl::meaning Meaning;
		qRB
			Counter = Records.Amount();	// NOTA : there is at least one record.
			Record.Init( Records );

			Writer.PutAttribute( "TotalAmount", bso::Convert( Records.Amount(), Buffer ) );
			Writer.PutAttribute( "SkippedTotalAmount", bso::Convert( Skipped, Buffer ) );

			if ( Id == DATA_D_ALL ) {
				Writer.PutAttribute( "Amount", bso::Convert( Records.Amount(), Buffer ) );
				DisplayAll_( Records, Writer );
			} else {
				if ( Id == 0 ) {
					do
						Row = Context.Pick( Records.Amount(), SessionMaxDuration );
					while ( Record( Row ).GetSkip() && ( --Counter ) );

					if ( !Counter ) {
						Writer.PutAttribute( "Amount", "0" );
						Row = qNIL;
					} else if ( !Record( Row ).GetSkip() ) {
						Id = *Row + 1;
						Writer.PutAttribute( "Amount", "1" );
					}

					xml::PutAttribute( "SessionAmount", Context.S_.Session, Writer );
					xml::PutAttribute( "SessionSkippedAmount", GetSkippedAmount_( Context.S_.Session, Context.Pool, Records ), Writer );

					xml::PutAttribute( "CycleAmount", Context.S_.Cycle, Writer );
					xml::PutAttribute( "CycleSkippedAmount", GetSkippedAmount_( Context.S_.Cycle, Context.Pool, Records ), Writer );

				} else {
					if ( Id > Records.Amount() ) {
						Meaning.Init();
						Meaning.SetValue( "NoRecordOfGivenIdError" );
						Meaning.AddTag( bso::Convert( Id, Buffer ) );
						sclmisc::ReportAndAbort( Meaning );
					} else {
						Writer.PutAttribute( "Amount", "1" );
						Row = Id - 1;
					}
				}

				if ( Row != qNIL )
					Display_( Row, Records, Label, Writer );
			}
		qRR
		qRT
		qRE
			return Id;
		}

		sId Display_(
			sId Id,
			const dpktbl::table_ &Table,
			bso::uint__ SessionMaxDuration,
			str::string_ &Label,
			str::string_ &TableLabel,
			dpkctx::context_ &Context,
			xml::writer_ &Writer )
		{
			Writer.PushTag( TableLabel = Table.Label );

			Id = Display_( Id, Table.Skipped(), SessionMaxDuration, Table.Records, Label, Context, Writer );

			Writer.PopTag();

			return Id;
		}

		sId Display_(
			sId Id,
			const dData &Data,
			const str::string_ &XSLFileName,
			bso::uint__ SessionMaxDuration,
			str::string_ &Label,
			str::string_ &TableLabel,
			dpkctx::context_ &Context,
			txf::text_oflow__ &Output )
		{
		qRH
			xml::writer Writer;
			ctn::qCITEMs( dpktbl::table_, dpktbl::trow__ ) Table;
			wRandomValues_ Randoms;
			str::string NS, URI;
			tol::buffer__ Buffer;
		qRB
			Randoms.Init();
			GetRandoms_( Randoms );

			xml::WriteXMLHeader( Output, xml::eUTF_8 );
			Output << txf::nl;

			if ( XSLFileName.Amount() != 0 ) {
				xml::WriteXSLDeclaration( XSLFileName, Output );
				Output << txf::nl;
			}

			Table.Init( Data );

			Writer.Init( Output, xml::oIndent, xml::e_None );

			Writer.PushTag( "Picking" );

			NS.Init( "xmlns:" );
			NS.Append( NamespaceLabel );
			NS.Truncate();

			URI.Init();
			sclmisc::MGetValue( registry::NamespaceURI, URI );

			Writer.PutAttribute( NS, URI );

			Writer.PushTag( "Misc" );

			Writer.PutValue(tol::DateAndTime(Buffer), "TimeStamp" );

			Writer.PushTag( "Generator" );

			Writer.PushTag( "Name" );
			Writer.PutValue( NAME_MC );
			Writer.PopTag();

			Writer.PushTag( "Version" );
			Writer.PutValue( VERSION );
			Writer.PopTag();

			Writer.PushTag( "URL" );
			Writer.PutValue( WEBSITE_URL );
			Writer.PopTag();

			Writer.PopTag();

			Writer.PushTag( "Session" );
			Writer.PushTag( "MaxDuration" );
			xml::PutValue( SessionMaxDuration, Writer );
			Writer.PopTag();
			Writer.PopTag();
			Writer.PopTag();

			HandleRandoms_( Randoms, Writer );

			Writer.PushTag( "Data" );

			Id = Display_( Id, Table( Data.Last() ), SessionMaxDuration, Label, TableLabel, Context, Writer );

			Writer.PopTag();

			Writer.PopTag();
		qRR
		qRT
		qRE
			return  Id;
		}

		sId DisplayWithoutBackup_(
			sId Id,
			const dData &Data,
			const str::string_ &XSLFileName,
			bso::uint__ SessionMaxDuration,
			str::string_ &Label,
			str::string_ &TableLabel,
			dpkctx::context_ &Context,
			const char *FileName )
		{
		qRH
			flf::file_oflow___ FFlow;
			txf::text_oflow__ TFlow;
		qRB
			if ( FFlow.Init( FileName ) != tol::rSuccess )
				sclmisc::ReportFileOpeningErrorAndAbort( FileName );

			TFlow.Init( FFlow );

			Id = Display_( Id, Data, XSLFileName, SessionMaxDuration, Label, TableLabel, Context, TFlow );
		qRR
		qRT
		qRE
			return Id;
		}
	}

	sId Display_(
		sId Id,
		const dData &Data,
		const str::string_ &XSLFileName,
		bso::uint__ SessionMaxDuration,
		str::string_ &Label,
		str::string_ &TableLabel,
		dpkctx::context_ &Context,
		const char *FileName )
	{
	qRH
		bso::bool__ Backuped = false;
	qRB
		sclmisc::CreateBackupFile( FileName );

		Backuped = true;

		Id = DisplayWithoutBackup_( Id, Data, XSLFileName, SessionMaxDuration, Label, TableLabel, Context, FileName );
	qRR
		if ( Backuped )
			sclmisc::RecoverBackupFile( FileName );
	qRT
	qRE
		return Id;
	}
}

void data_d::Initialize( void )
{
	// Nothing to do.
}

sId data_d::Display(
	sId Id,
	const dData &Data,
	const str::string_ &XSLFileName,
	bso::uint__ SessionMaxDuration,
	str::string_ &Label,
	str::string_ &TableLabel,
	dpkctx::context_ &Context,
	const str::string_ &OutputFileName )
{
qRH
	TOL_CBUFFER___ Buffer;
qRB
	if ( OutputFileName.Amount() == 0 )
		Id = Display_( Id, Data, XSLFileName, SessionMaxDuration, Label, TableLabel, Context, cio::COut );
	else
		Id = Display_( Id, Data, XSLFileName, SessionMaxDuration, Label, TableLabel, Context, OutputFileName.Convert( Buffer ) );
qRR
qRT
qRE
	return Id;
}


