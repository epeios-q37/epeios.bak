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

      sdr::sSize Size = S_().OSDSize();

      if ( Size >= offset )
        Size -= offset;
      else if ( Size != 0 )
        qRFwk();

      return Size;
		}
		//v Recall 'Amount' at position 'Position' and put them in 'Buffer'.
		virtual sdr::sSize SDRFetch(
			sdr::tRow Position,
			sdr::sSize Amount,
			sdr::sByte *Buffer,
			qRPN) override
    {
        return S_().OSDFetch(Position + offset, Amount, Buffer, qRP);
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
    void Put(
      const sdr::sByte *Data,
      sdr::sSize Position,
      sdr::sSize Amount)
    {
      if ( offset == 0 )  // Calling this function in this context does not make sense.
        qRFwk();

      if ( ( Position + Amount ) > offset )
        qRFwk();

      return S_().OSDStore(Data, Amount, Position);
    }
    void Put(const sdr::sByte *Data)
    {
      return Put(Data, 0, offset);
    }
    void Get(
      sdr::sSize Position,
      sdr::sSize Amount,
      sdr::sByte *Data)
    {
      if ( offset == 0 )  // Calling this function in this context does not make sense.
        qRFwk();

      if ( ( Position + Amount ) > offset )
        qRFwk();

      S_().OSDFetch(Position, Amount, Data, qRPDefault);
    }
    void Get(sdr::sByte *Data)
    {
      return Get(0, offset, Data);
    }
  };
}

#endif
