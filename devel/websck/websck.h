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

// WebSocket

#ifndef WEBSCK_INC_
# define WEBSCK_INC_

# define WEBSCK_NAME		"WEBSCK"

# if defined( E_DEBUG ) && !defined( WEBSCK_NODBG )
#  define WEBSCK_DBG
# endif

# include "err.h"
# include "fdr.h"
# include "str.h"

namespace websck {

	class dField {
	public:
        struct s {
            str::dString::s
                Label,
                Value;
        };
        str::dString
            Label,
            Value;
        dField( s &S )
        : Label( S.Label),
          Value( S.Value )
        {}
        void reset( bso::sBool P = true ) {
            tol::reset( P, Label, Value );
        }
        dField &operator =( const dField &Field )
        {
            Label = Field.Label;
            Value = Field.Value;

            return *this;
        }
        void plug( qASd *AS )
		{
            Label.plug( AS );
			Value.plug( AS );
		}
        qDTOR( dField );
		void Init( void ) {
            tol::Init( Label, Value );
        }
	};

	qW( Field );

	typedef crt::qCRATEdl( dField ) dFields;
	qW( Fields );

	sdr::sRow Search(
        const str::dString &Label,
        const dFields &Fields);

    inline bso::sBool GetValue(
        const str::dString &Label,
        const dFields &Fields,
        str::dString &Value)
    {
        sdr::sRow Row = Search(Label, Fields);

        if ( Row != qNIL ) {
            Value.Append(Fields(Row).Value);
            return true;
        } else
            return false;
    }

	class dHeader
	{
	public:
        struct s {
            str::dString::s FirstLine;
            dFields::s Fields;
        };
        str::dString FirstLine;
        dFields Fields;
        dHeader(s &S)
        : FirstLine(S.FirstLine),
          Fields(S.Fields)
        {}
        void reset(bso::sBool P = true)
        {
            tol::reset(P, FirstLine, Fields);
        }
        dHeader &operator =(const dHeader &H)
        {
            FirstLine = H.FirstLine;
            Fields = H.Fields;

            return *this;
        }
        qDTOR(dHeader);
        void Init(void)
        {
            tol::Init(FirstLine, Fields);
        }
	};

	qW(Header);

	inline sdr::sRow Search(
        const str::dString &Label,
        const dHeader &Header)
    {
        return Search(Label, Header.Fields);
    }

    inline bso::sBool GetValue(
        const str::dString &Label,
        const dHeader &Header,
        str::dString &Value)
    {
        return GetValue(Label, Header.Fields, Value);
    }


	// If false, then the client doesn't send a standard WebSocket header.
	// 'Fields' does nevertheless contain the fields of the header.
    bso::sBool Handshake(
        fdr::rRWDriver &RWDriver,
        dHeader &Header);

	// If false, then the client doesn't send a standard WebSocket header.
    bso::sBool Handshake(fdr::rRWDriver &RWDriver);

    typedef fdr::rRDressedDriver rRDriver_;

    class rRDriver
    : public rRDriver_
    {
    private:
        flw::rDressedRFlow<> Flow_;
        bso::sSize Length_;
        bso::sU32 Mask_;
        bso::sU8 MaskCounter_;
        bso::sSize GetLength( flw::rRFlow &Flow );
     protected:
    	virtual fdr::size__ FDRRead(
			fdr::size__ Maximum,
			fdr::byte__ *Buffer ) override;
		virtual bso::sBool FDRDismiss(
			bso::sBool Unlock,
			qRPN ) override
		{
			return Flow_.Dismiss( Unlock, ErrHandling );
		}
		virtual fdr::sTID FDRRTake( fdr::sTID Owner ) override
		{
			return Flow_.RDriver().RTake( Owner );
		}
    public:
        void reset( bso::sBool P = true )
        {
            rRDriver_::reset(P);
            Flow_.reset(P);
            Length_ = 0;
            Mask_ = 0;
            MaskCounter_ = 0;
        }
        qCVDTOR(rRDriver);
        void Init(
            fdr::rRDriver &Driver,  // Before calling this funciton, 'Driver' must have be called with 'Handshke(…)'
            fdr::thread_safety__ ThreadSafety )
        {
            rRDriver_::Init(ThreadSafety);
            Flow_.Init(Driver);
            Length_ = 0;
            Mask_ = 0;
            MaskCounter_ = 0;
        }
    };

	typedef flw::rDressedRFlow<> rRFlow_;

	class rRFlow
	: public rRFlow_
	{
	private:
		rRDriver Driver_;
	public:
		void reset( bso::bool__ P = true )
		{
			rRFlow_::reset( P );
			Driver_.reset( P );
		}
		qCDTOR( rRFlow );
		void Init( fdr::rRDriver &Driver )  // Before calling this function, 'Driver' must have be called with 'Handshke(…)'
		{
			Driver_.Init( Driver, fdr::ts_Default );
			rRFlow_::Init( Driver_  );
		}
	};

	typedef fdr::rWDressedDriver rWDriver_;

	class rWDriver
	: public rWDriver_
	{
	private:

        flw::rDressedWFlow<> Flow_;
        bso::sBool InProgress_;
        void SendSize_(
            bso::sU64 Size,
            bso::sU8 Pos );
        void SendSize_(bso::sU64 Size);
	protected:
		virtual fdr::size__ FDRWrite(
			const fdr::byte__ *Buffer,
			fdr::size__ Maximum ) override
		{
            if ( InProgress_ )
                Flow_.Put(0);   // 'FIN' bit clear and opcode 'continuation'.
            else {
                Flow_.Put(1);   // 'FIN' bit clear and opcode 'text'.
                InProgress_ = true;
            }

            SendSize_(Maximum);

            Flow_.Write(Buffer, Maximum);

            return Maximum;
		}
		virtual bso::sBool FDRCommit(
			bso::sBool Unlock,
			qRPN ) override
		{
            if ( InProgress_ ) {
                Flow_.Put(0x80);   // 'FIN' bit set and OpCode 'continuation'.
                SendSize_(0);   // Data of size 0.
                InProgress_ = false;
            }

			return Flow_.Commit( Unlock, ErrHandling );
		}
		virtual fdr::sTID FDRWTake( fdr::sTID Owner ) override
		{
			 return Flow_.WDriver().WTake( Owner );
		}
    public:
        void reset( bso::sBool P = true )
        {
            rWDriver_::reset(P);
            Flow_.reset(P);
            InProgress_ = false;
        }
        qCVDTOR(rWDriver);
        void Init(
            fdr::rWDriver &Driver,  // Before calling this funciton, 'Driver' must have be called with 'Handshke(…)'
            fdr::thread_safety__ ThreadSafety )
        {
            rWDriver_::Init(ThreadSafety);
            Flow_.Init(Driver);
            InProgress_ = false;
        }
    };

	typedef flw::rDressedWFlow<> rWFlow_;

	class rWFlow
	: public rWFlow_
	{
	private:
		rWDriver Driver_;
	public:
		void reset( bso::bool__ P = true )
		{
			rWFlow_::reset( P );
			Driver_.reset( P );
		}
		qCDTOR( rWFlow );
		void Init( fdr::rWDriver &Driver )  // Before calling this function, 'Driver' must have be called with 'Handshke(…)'
		{
			Driver_.Init( Driver, fdr::ts_Default );
			rWFlow_::Init( Driver_  );
		}
	};
}

#endif
