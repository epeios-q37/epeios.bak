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

// FRontenD INSTanCes

#ifndef FRDINSTC__INC
# define FRDINSTC__INC

# include "frdfrntnd.h"

namespace frdinstc {

	using namespace frdfrntnd;

	class rUser_
	{
	private:
		qRMV( frdfrntnd::rFrontend, F_,  Frontend_ );
		orgnzq::fStatics &S_( void )
		{
			return F_().Statics;
		}
		orgnzq::rOGZRecord Record_;
		orgnzq::rOGZMyObject Object_;
	public:
		void reset( bso::bool__ P = true )
		{	
			Frontend_ = NULL;

			Record_.reset( P );
			Object_.reset( P );
		}
		E_CVDTOR( rUser_ );
		void Init( frdfrntnd::rFrontend &Frontend )
		{
			Frontend_ = &Frontend;

			Record_.Init( Frontend.Record );
			Object_.Init( Frontend.MyObject );
		}
		bso::sBool Login(
			const str::vString &Username,
			const str::vString &Password )
		{
			bso::sBool Success = false;

			S_().OGZLogin( Username, Password, Success );

			return Success;
		}
		void EditRecord( fbltyp::sId Record )	// if == 'UndefinedId', we create empty record.
		{
			Record_.EditRecord( Record );
		}
		sFieldId CreateField(
			sTypeId Type,
			sNumberId Number,
			const str::dString &Label,
			const str::dString &Comment ) const
		{
			bFieldId Id = *UndefinedField;

			Record_.CreateField( *Type, *Number, Label, Comment, Id );

			return Id;
		}
		void GetFieldsColumns(
			fbltyp::dIds &Ids,
			fbltyp::dIds &Types,
			fbltyp::dId8s &Numbers,
			str::dStrings &Labels,
			str::dStrings &Comments ) const
		{
			Record_.GetFieldsColumns( Ids, Types, Numbers, Labels, Comments );
		}
	};

	qENUM( View )
	{
		vRecords,
		vRecord,
		v_amount,
		v_Undefined
	};

	const char *GetLabel( eView View );

	class rUser
	{
	private:
		rUser_ Core_;
		eView View_;
	public:
		void reset( bso::bool__ P = true )
		{	
			Core_.reset( P );
			View_ = v_Undefined;
		}
		E_CVDTOR( rUser );
		void Init( frdfrntnd::rFrontend &Frontend )
		{
			if ( Frontend.IsConnected() )
				Core_.Init( Frontend );

			View_ = vRecords;
		}
		bso::sBool Login(
			const str::vString &Username,
			const str::vString &Password )
		{
			return Core_.Login( Username, Password );
		}
		void CreateRecord( void )
		{
			View_ = vRecord;
			Core_.EditRecord( qNIL );
		}
		void CreateField(
			sTypeId Type,
			sNumberId Number,
			const str::dString &Label,
			const str::dString &Comment )
		{
			Core_.CreateField( Type, Number, Label, Comment );
		}
		void DumpFieldsColumns( xml::dWriter &Writer ) const;
		qRODISCLOSEr( eView, View );
	};
}

#endif
