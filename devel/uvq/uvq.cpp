/*
	Copyright (C) 1999-2017 Claude SIMON (http://q37.info/contact/).

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

#define UVQ__COMPILATION

// Note to developer : you have to add '<path_to_node>/deps/uv/include' as 'Additional Include Directory' in the 'devel' related project.
#include <uv.h>

#include "uvq.h"

#include "cio.h"
#include "tht.h"
#ifdef BUILDING_NODE_EXTENSION
# include "v8q.h"
#endif

using namespace uvq;

namespace {
	bso::sUInt Amount_ = 0;
	namespace {
		struct sWork_ {
			uv_work_t Request;
			cAsync *Callbacks;
			tht::rBlocker Blocker;
			bso::sBool Error;
		};

		void ErrFinal_( void )
		{
			err::buffer__ Buffer;

			const char *Message = err::Message( Buffer );

			ERRRst();	// To avoid relaunching of current error by objects of the 'FLW' library.

#ifdef  BUILDING_NODE_EXTENSION
			v8::Isolate *Isolate = v8q::GetIsolate();
			if ( Isolate != NULL ) {
				v8::HandleScope scope( Isolate );
				Isolate->ThrowException( v8::Exception::Error( v8q::ToString( Buffer ) ) );
			} else 
#endif
			{
				qRH
				qRB
					if ( cio::IsInitialized() ) {
						if ( cio::Target() == cio::tConsole ) {
							cio::COut << txf::commit;
							cio::CErr << txf::nl << txf::tab;
						}

						cio::CErr << "{ " << Message << " }";

						if ( cio::Target() == cio::tConsole )
							cio::CErr << txf::nl;

						cio::CErr << txf::commit;
					} else
						qRFwk();
				qRR
				qRT
				qRE
			}
		}
	}

	void WorkAsync_(uv_work_t *req)
	{
	qRFH
		sWork_ &Work = *static_cast<sWork_ *>( req->data );
	qRFB
		Amount_++;
		Work.Callbacks->Work();
	qRFR
		Work.Error = true;
	qRFT
	qRFE( ErrFinal_() );
	}

	void WorkAsyncComplete_(
		uv_work_t *req,
		int Status )
	{
	qRFH
		sWork_ *Work = static_cast<sWork_ *>( req->data );
	qRFB
#ifdef BUILDING_NODE_EXTENSION
		v8::HandleScope scope(v8q::GetIsolate() );
#endif
		if ( !Work->Error ) {
			switch ( Work->Callbacks->After() ) {
			case bRelaunch:
				uv_queue_work( uv_default_loop(), req, WorkAsync_, WorkAsyncComplete_ );
				break;
			case bExitAndDelete:
				delete Work->Callbacks;
			case bExitOnly:
				delete Work;
				break;
			default:
				qRFwk();
				break;
			}
		}
		Amount_--;
	qRFR
	qRFT
	qRFE( ErrFinal_() );
	}
}

void uvq::Launch( cAsync &Callbacks )
{
	sWork_ *Work = new sWork_;

	if ( Work == NULL )
		qRAlc();

	Work->Request.data = Work;
	Work->Callbacks = &Callbacks;
	Work->Error = false;

	uv_queue_work( uv_default_loop(), &Work->Request, WorkAsync_, WorkAsyncComplete_ );
}

bso::sUInt uvq::AmountPending( void )
{
	return Amount_;
}



