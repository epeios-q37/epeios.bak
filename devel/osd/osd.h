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

// Offset Storage Driver

#ifndef OSD_INC_
# define OSD_INC_

# define OSD_NAME		"OSD"

# if defined( E_DEBUG ) && !defined( OSD_NODBG )
#  define OSD_DBG
# endif

# include "err.h"
# include "sdr.h"
# include "tol.h"

namespace osd {
  template <typename storage, int offset> class sDriver
  : public qSDs
  {
  private:
    qRMV(storage, S_, Storage_);
  protected:
		virtual void SDRAllocate( sdr::sSize Size ) override
		{
			return S_().OSDAllocate(Size + offset);
    }
    virtual sdr::sSize SDRSize( void ) const override
		{
		  if ( Storage_ == NULL )
        return 0;

		  return S_().OSDSize();
		}
		//v Recall 'Amount' at position 'Position' and put them in 'Buffer'.
		virtual void SDRRecall(
			sdr::tRow Position,
			sdr::sSize Amount,
			sdr::sByte *Buffer ) override
    {
        return S_().OSDRecall(Position + offset, Amount, Buffer);
    }
		//v Write 'Amount' bytes from 'Buffer' to storage at position 'Position'.
		virtual void SDRStore(
			const sdr::sByte *Buffer,
			sdr::sSize Amount,
			sdr::tRow Position ) override
		{
		  return S_().OSDStore(Buffer, Amount, Position + offset);
		}
  public:
    void reset(bso::sBool P = true)
    {
      Storage_ = NULL;
      qSDs::reset(P);
    }
    qCVDTOR(sDriver);
    void Init(storage *Storage)
    {
      Storage_ = Storage;

      qSDs::Init();
    }
    void Init(storage &Storage)
    {
      Init(&Storage);
    }
    void Put(const sdr::sByte *Buffer)
    {
      if ( offset == 0 )  // Calling this function in this context does not make sense.
        qRFwk();

      return S_().OSDStore(Buffer, offset, 0);
    }
    void Get(sdr::sByte *Buffer)
    {
      if ( offset == 0 )  // Calling this function in this context does not make sense.
        qRFwk();

      return S_().OSDRecall(0, offset, Buffer);
    }
  };
}

#endif
