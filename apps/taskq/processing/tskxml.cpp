/*
	Copyright (C) 2021 Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of 'TaskQ' software.

    'TaskQ' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'TaskQ' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'TaskQ'.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "tskxml.h"

#include "tskinf.h"

using namespace tskxml;

using namespace tsktasks;

namespace {
  namespace {
    qENUM( Markup ) {
      mVersionTag,
      mRootTaskTag,
      mSubTasksTag,
      mTaskTag,
      mDescriptionTag,
      mAmountAttr,
      mRowAttr,
      mLabelAttr,
      m_amount,
      m_Undefined

    };

  #define G_(label, suffix)\
  case m##label##suffix:\
    return #label;\
    break

  #define T(label)  G_( label, Tag )

  #define A(label)  G_( label, Attr )

    const char *GetLabel_(eMarkup Markup )
    {
      switch(Markup) {
      T( Version );
      T( RootTask );
      T( SubTasks );
      T( Task );
      T( Description );
      A( Amount );
      case mRowAttr:
        return "row";
        break;
      A( Label );
      default:
        qRFwk();
      }

      return NULL;  // To avoid a warning.
    }

    stsfsm::wAutomat MarkupAutomat_;

    void FillMarkupAutomat_( void )
    {
      MarkupAutomat_.Init();
      stsfsm::Fill<eMarkup>( MarkupAutomat_, m_amount, GetLabel_ );
    }

    eMarkup GetMarkup( const str::dString &Pattern )
    {
      return stsfsm::GetId( Pattern, MarkupAutomat_, m_Undefined, m_amount );
    }
  }

#define L(markup) GetLabel_(m##markup)

	class sBrowser_
	: public cBrowser
	{
  private:
    xml::rWriter Writer_;
    bso::sBool TaskPending_;
  protected:
	  virtual void TSKRoot(
      sLevel Level,
      sTRow Row,
      sdr::sSize Amount) override
    {
      Writer_.PushTag(L( RootTaskTag ));
      Writer_.PutAttribute(L( AmountAttr ), Amount);
    }
    virtual void TSKTask(
      eKinship Kinship,
      sLevel Level,
      sTRow Row,
      const str::dString &Label,
      const str::dString &Description) override
    {
      switch ( Kinship ) {
      case kFirst:
        break;
      case kChild:
        Writer_.PushTag(L( SubTasksTag ));
        break;
      case kSibling:
        Writer_.PopTag();
        break;
      default:
        qRFwk();
        break;
      }

      Writer_.PushTag(L( TaskTag ));
      Writer_.PutAttribute(L( RowAttr ), *Row);

      Writer_.PutAttribute(L( LabelAttr ), Label);

      if ( Description.Amount() )
        Writer_.PutValue(Description, L( DescriptionTag ));

      TaskPending_ = true;

    }
    virtual void TSKParent(sLevel Level) override
    {
      if ( TaskPending_ ) {
        Writer_.PopTag();
        TaskPending_ = false;
      }

      Writer_.PopTag();
    }
  public:
    void reset(bso::sBool P = true)
    {
      Writer_.reset(P);
      TaskPending_ = false;
    }
    qCVDTOR(sBrowser_);
    void Init(
      txf::sWFlow &Flow,
      const char *Generator)
    {
      tol::bDateAndTime Buffer;

      reset();

      Writer_.Init(Flow, xml::lIndent, xml::fEncoding());

      Writer_.PushTag(TSKINF_MC);
      Writer_.PutAttribute(L( VersionTag ), TSKINF_SOFTWARE_VERSION);
      Writer_.PutAttribute("Timestamp", tol::DateAndTime(Buffer));
      Writer_.PutAttribute("Generator", Generator);
    }
  };
}

void tskxml::Export(
  const rTasks &Tasks,
  sTRow Row,
  txf::sWFlow &Flow,
  const char *Generator)
{
qRH;
  sBrowser_ Browser;
qRB;
  Browser.Init(Flow, Generator);

  Tasks.Browse( Row, Browser);
qRR;
qRT;
qRE;
}

qGCTOR( tskxml )
{
	FillMarkupAutomat_();
}


