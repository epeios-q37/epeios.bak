/*
	Copyright (C) 2007-2017 Claude SIMON (http://q37.info/contact/).

	This file is part of xppq.

	xppq is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

	xppq is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with xppq. If not, see <http://www.gnu.org/licenses/>.
*/

#include <uv.h>
#include "v8q.h"

#include "common.h"

#include "scln.h"

using namespace common;

namespace {
	struct sWork_ {
		uv_work_t Request;
		cASync *Callbacks;
		bso::sBool Terminate;
		tht::rBlocker Blocker;
	};

	void WorkAsync_(uv_work_t *req)
	{
	qRFH
		sWork_ &Work = *static_cast<sWork_ *>( req->data );
	qRFB
//		Work.Blocker.Wait();
		Work.Terminate = Work.Callbacks->Process();
	qRFR
	qRFT
	qRFE(scln::ErrFinal() );
	}

	void WorkAsyncComplete_(
		uv_work_t *req,
		int Status )
	{
		sWork_ *Work = static_cast<sWork_ *>(req->data);
		v8::HandleScope scope(v8q::GetIsolate() );

		Work->Callbacks->Disclose();

		if ( !Work->Terminate ) {
//			Work->Blocker.Init();
			uv_queue_work( uv_default_loop(), req, WorkAsync_, WorkAsyncComplete_ );
//			Work->Blocker.Unblock();
		} else
			delete Work;
	}
}

void common::HandleASync( cASync &Callbacks )
{
	sWork_ *Work = new sWork_;

	if ( Work == NULL )
		qRAlc();

	Work->Request.data = Work;
	Work->Callbacks = &Callbacks;
	Work->Terminate = false;
//	Work->Blocker.Init();

	uv_queue_work( uv_default_loop(), &Work->Request, WorkAsync_, WorkAsyncComplete_ );
}
