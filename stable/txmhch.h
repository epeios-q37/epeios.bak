/* begin of 'entete.txt' template file V1.1 */
/* Best viewed with a tab size of 4 */
/*
	This file is part of the Epeios project. For all information
	concerning the Epeios project, this file and its conditions of use,
	consult the site: 

			http://www.epeios.org/
*/
/* end of 'entete.txt' template file */
/* begin of 'entete.h' template file V1.3 */
/*
Version:
	1.1.0 07/04/2000 07:18:29

Instigators:
	Claude SIMON

Authors:
	Claude SIMON

Contributors:
	/

Coordinator:
	Claude SIMON

Description:
	TeXt Manager HierarCHical
*/

/* Begin of automatic documentation generation part. */

//V 1.1.0
//C Claude SIMON
//D TeXt Manager HierarCHical
//R 07/04/2000 07:18:29

/* End of automatic documentation generation part. */

/* end of 'entete.h' template file */
/* begin of 'xxx.h' template file V2.5 */

#ifndef TXMHCH__INC
#define TXMHCH__INC

#define TXMHCH_NAME		"TXMHCH"

#define	TXMHCH_VERSION		"1.1.0"	
#define TXMHCH_OWNER		"The Epeios Project (www.epeios.org)"	

//  	Substitution is not applied to the rest when emancipated

#ifdef TXMHCH__LINE
#line 14
#endif

#include "ttr.h"

extern class ttr_tutor &TXMHCHTutor;

#if defined( E_DEBUG ) && !defined( TXMHCH_NODBG )
#define TXMHCH_DBG 
#endif

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

#include "err.h"
#include "flw.h"
#include "lst.h"
#include "dtr.h"
#include "str.h"
#include "ctn.h"

struct txmhch
{
	//e The type of a text node.
	enum type {
		//i Unknow type
		tUnknow,
		//i Title.
		tTitle,
		//i Normal type.
		tNormal,
		//i List description,
		tListDescription,
		//i Comment.
		tComment,
		//i Subtitle.
		tSubtitle,
		//i Reference.
		tReference,
	};
};

//t A paragraph.
typedef str_string_ txmhch_paragraph_;
typedef str_string	txmhch_paragraph;

//c A node.
class txmhch_node_
: public txmhch_paragraph_
{
public:
	struct s
	: txmhch_paragraph_::s
	{
		bso__ubyte Type;
	} &S_;
	txmhch_node_( s &S )
	: S_( S ),
	  txmhch_paragraph_( S )
	{}
	void reset( bso__bool P = true )
	{
		txmhch_paragraph_::reset( P );
		S_.Type = txmhch::tUnknow;
	}
	void plug( mmm_multimemory_ &M )
	{
		txmhch_paragraph_::plug( M );
	}
	void plug( MEMORY_DRIVER_ &MD )
	{
		txmhch_paragraph_::plug( MD );
	}
	txmhch_node_ &operator =( const txmhch_node_ &P )
	{
		txmhch_paragraph_::operator =( P );
		S_.Type = P.S_.Type;

		return *this;
	}
	//f Initialization.
	void Init( void )
	{
		txmhch_paragraph_::Init();
		S_.Type = txmhch::tNormal;
	}
	/*f Initialization with 'Paragraph' of type 'Type' ans level 'Level'. */
	void Init(
		const txmhch_paragraph_ &Paragraph,
		txmhch::type Type )
	{
		txmhch_paragraph_:: operator =( Paragraph );
		S_.Type = Type;
	}
	//f Return the type.
	txmhch::type Type( void ) const
	{
		return (txmhch::type)S_.Type;
	}
};

//c A hierarchical text.
class txmhch_hierarchical_text_
: public LIST_,
  public XMCONTAINER_( txmhch_node_ ),
  public dtr_dynamic_tree_
{
protected:
	virtual void LSTAllocate( SIZE__ Size )
	{
		XMCONTAINER_( txmhch_node_ )::Allocate( Size );
		dtr_dynamic_tree_::Allocate( Size );
	}
public:
	struct s
	: public LIST_::s,
	  public XMCONTAINER_( txmhch_node_ )::s,
	  public dtr_dynamic_tree_::s
	{};
	txmhch_hierarchical_text_( s &S )
	: LIST_( S ),
	  XMCONTAINER_( txmhch_node_ )( S ),
	  dtr_dynamic_tree_( S )
	{}
	void reset( bso__bool P = true )
	{
		LIST_::reset( P );
		XMCONTAINER_( txmhch_node_ )::reset( P );
		dtr_dynamic_tree_::reset( P );
	}
	void plug( mmm_multimemory_ &M )
	{
		LIST_::plug( M );
		XMCONTAINER_( txmhch_node_ )::plug( M );
		dtr_dynamic_tree_::plug( M );
	}
	txmhch_hierarchical_text_ &operator =( const txmhch_hierarchical_text_ &T )
	{
		LIST_::operator =( T );
		XMCONTAINER_( txmhch_node_ )::operator =( T );
		dtr_dynamic_tree_::operator =( T );
	}
	//f Initialization.
	void Init( void )
	{
		LIST_::Init();
		XMCONTAINER_( txmhch_node_ )::Init();
		dtr_dynamic_tree_::Init();
	}
	//f Return first section from 'Section'.
	POSITION__ First( POSITION__ Section ) const
	{
		return dtr_dynamic_tree_::First( Section );
	}
	//f Return last section from 'Section'.
	POSITION__ Last( POSITION__ Section ) const
	{
		return dtr_dynamic_tree_::Last( Section );
	}
	//f Return section next to 'Section'.
	POSITION__ Next( POSITION__ Section ) const
	{
		return dtr_dynamic_tree_::Next( Section );
	}
	//f Return section next to 'Node'.
	POSITION__ Previous( POSITION__ Section ) const
	{
		return dtr_dynamic_tree_::Previous( Section );
	}
	//f Create 'Paragraph' with type 'Type' and return its position. To create root node.
	POSITION__ Create(
		const txmhch_paragraph_ &Paragraph,
		txmhch::type Type )
	{
		POSITION__ P = LIST_::CreateEntry();

		XMCONTAINER_( txmhch_node_ )::operator ()( P ).Init( Paragraph, Type );

		return P;
	}
	//f 'Paragraph' of type 'Type' becomes next to node at 'Position'. Returns its position.
	POSITION__ BecomeNext(
		const txmhch_paragraph_ &Paragraph,
		txmhch::type Type,
		POSITION__ Position )
	{
		POSITION__ P = Create( Paragraph, Type );

		dtr_dynamic_tree_::BecomeNext( P, Position );

		return P;
	}
	//f 'Paragraph' of type 'Type' becomes previous to node at 'Position'. Returns its position.
	POSITION__ BecomePrevious(
		const txmhch_paragraph_ &Paragraph,
		txmhch::type Type,
		POSITION__ Position )
	{
		POSITION__ P = Create( Paragraph, Type );

		dtr_dynamic_tree_::BecomePrevious( P, Position );

		return P;
	}
	//f 'Paragraph' of type 'Type' becomes first to node at 'Position'. Returns its position.
	POSITION__ BecomeFirst(
		const txmhch_paragraph_ &Paragraph,
		txmhch::type Type,
		POSITION__ Position )
	{
		POSITION__ P = Create( Paragraph, Type );

		dtr_dynamic_tree_::BecomeFirst( P, Position );

		return P;
	}
	//f 'Paragraph' of type 'Type' becomes last to node at 'Position'. Returns its position.
	POSITION__ BecomeLast(
		const txmhch_paragraph_ &Paragraph,
		txmhch::type Type,
		POSITION__ Position )
	{
		POSITION__ P = Create( Paragraph, Type );

		dtr_dynamic_tree_::BecomeLast( P, Position );

		return P;
	}
};

AUTO( txmhch_hierarchical_text )

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
/* end of 'xxx.h' template file */