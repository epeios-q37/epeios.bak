/*
	Copyright (C) 2017 by Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of XPPq.

    XPPq is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    XPPq is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with XPPq.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "processor.h"

#include "xpp.h"

using namespace processor;

using sclznd::sCaller;

namespace {
	class rRack_ {
	private:
		sclznd::rStreamDriver Stream_;
		flw::sDressedIFlow<> SFlow_;
		xtf::extended_text_iflow__ SXFlow_;
	public:
		xpp::preprocessing_iflow___ Flow;
		void reset( bso::sBool P = true )
		{
			tol::reset( P, Stream_, SFlow_, SXFlow_, Flow );
		}
		qCDTOR( rRack_ );
		void Init( sclznd::sCaller &Caller )
		{
			Stream_.Init( Caller );
			SFlow_.Init( Stream_ );
			SXFlow_.Init( SFlow_, utf::f_Default );
			Flow.Init( SXFlow_, xpp::criterions___( str::wString() ) );
		}
	};
}

SCLZND_F( processor::New )
{
qRH;
	rRack_ *Rack = NULL;
qRB;
	if ( ( Rack = new rRack_ ) == NULL )
		qRAlc();

	Rack->Init( Caller );

	Caller.SetReturnValue( (bso::sS64)Rack );
qRR;
	if ( Rack != NULL )
		delete Rack;
qRT;
qRE;
}

SCLZND_F( processor::EndOfFlow )
{
qRH;
	bso::sS64 Long = 0;
qRB;
	Caller.Get( Long );

	if ( Long == 0 )
		qRGnr();

	rRack_ &Rack = *(rRack_ *)Long;

	Caller.SetReturnValue( Rack.Flow.EndOfFlow() );
qRR;
qRT;
qRE;
}

SCLZND_F( processor::Read )
{
qRH;
	bso::sS64 Long = 0, Count = 0;
	str::wString Buffer;
qRB;
	Caller.Get( Long );

	if ( Long == 0 )
		qRGnr();

	rRack_ &Rack = *(rRack_ *)Long;

	Caller.Get( Count );

	Buffer.Init();

	while ( Count-- && !Rack.Flow.EndOfFlow() )
		Buffer.Append( Rack.Flow.Get() );

	Caller.SetReturnValue( Buffer );
qRR;
qRT;
qRE;
}

SCLZND_F( processor::Delete )
{
qRH;
	bso::sS64 Long = 0;
qRB;
	Caller.Get( Long );

	if ( Long == 0 )
		qRGnr();

	delete (rRack_ * )Long;
qRR;
qRT;
qRE;
}
