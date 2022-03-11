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

#define N4ALLW_COMPILATION_

#include "n4allw.h"

using namespace n4allw;

namespace {
	namespace {
		err::err___ qRRor_;
		scle::rError SCLError_;
		scll::rRack Locale_;
		sclm::sRack Rack_;
	}

	typedef n4all::cRegistrar cRegistrar_;

	class rRegistrar_
	: public cRegistrar_
	{
	private:
		qRMV( dFunctions_, F_, Functions_ );
	protected:
		virtual n4all::sIndex N4ALLRegister(
      void *Function,
      n4all::sIndex Index) override
		{
			sdr::sRow Row = F_().Append( Function );

			if ( *Row != Index )
        qRFwk();

      return Index;
		}
	public:
		void reset( bso::sBool P = true )
		{
			Functions_ = NULL;
		}
		qCVDTOR( rRegistrar_ );
		void Init( dFunctions_ & Functions )
		{
			Functions_ = &Functions;
		}
	};

	extern "C" typedef n4all::fRegister fRegister_;
}

void *n4allw::rLauncher::GetFunction_( sdr::sRow Row )
{
	if ( !Functions_.Exists( Row ) )
		qRFwk();  // Beyond last registered function.

  void *Function = Functions_(Row);

  if ( Function == NULL )
    qRFwk();  // No function registered for this row/index.

	return Function;
}

bso::bool__ n4allw::rLauncher::Register_(
	const fnm::rName &ComponentFilename,
	dlbrry::eNormalization Normalization,
	n4all::cRegistrar &Registrar,
	sclm::sRack &Rack,
	void *UP,
	bso::sBool SkipComponentUnloading )
{
	bso::bool__ Success = false;
qRH
	n4all::sData Data;
	fnm::name___ Location;
	TOL_CBUFFER___ Buffer;
qRB
	Location.Init();
	fnm::GetLocation( ComponentFilename, Location );
	Data.Init( Rack, Location, str::wString(), UP );

	//		cio::COut << __LOC__ << AddonFilename << txf::nl << txf::commit;

	Library_.Init( ComponentFilename, Normalization );

	if ( SkipComponentUnloading )
		Library_.SkipUnloading();

	//		cio::COut << __LOC__ << txf::nl << txf::commit;

	fRegister_ *Register = dlbrry::GetFunction<fRegister_ *>( E_STRING( N4ALL_REGISTER_FUNCTION_NAME ), Library_ );

	if ( Register != NULL ) {
		if ( Launcher_ != NULL )
			qRGnr();

		Launcher_ = Register( &Registrar, &Data );

		Success = true;
	}
qRR
qRT
qRE
	return Success;
}

bso::sBool n4allw::rLauncher::Init(
	const fnm::rName &ComponentFilename,
	dlbrry::eNormalization Normalization,
	sclm::sRack &Rack,
	void *UP,
	bso::sBool SkipComponentUnloading,
	qRPN )
{
	bso::sBool Return = false;
qRH;
	rRegistrar_ Registrar;
qRB;
	Functions_.Init();

	Registrar.Init( Functions_ );

	if ( !Register_( ComponentFilename, Normalization, Registrar, Rack, UP, SkipComponentUnloading ) ) {
		if ( qRP == err::hThrowException )
			qRFwk();
	} else
		Return = true;
qRR;
qRT;
qRE;
	return Return;
}
