/*
	Header for the 'xulfsv' library by Claude SIMON (csimon at zeusw dot org)
	Copyright (C) 2004 Claude SIMON.

	This file is part of the Epeios (http://zeusw.org/epeios/) project.

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
	of the License, or (at your option) any later version.
 
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, go to http://www.fsf.org/
	or write to the:
  
         	         Free Software Foundation, Inc.,
           59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

//	$Id: xulfsv.h,v 1.1 2011/12/16 16:19:54 csimon Exp $

#ifndef XULFSV__INC
#define XULFSV__INC

#define XULFSV_NAME		"XULFSV"

#define	XULFSV_VERSION	"$Revision: 1.1 $"

#define XULFSV_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &XULFSVTutor;

#if defined( E_DEBUG ) && !defined( XULFSV_NODBG )
#define XULFSV_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.1 $
//C Claude SIMON (csimon at zeusw dot org)
//R $Date: 2011/12/16 16:19:54 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D XUL Frontend Session View 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

# include "xulfbs.h"
# include "frdkrn.h"

# define XULFSV_AFFIX	"SessionView"

namespace xulfsv {
	namespace {
		using namespace xulfbs;
	}

	// Standardisation.
	class widgets__
	{
	public:
		void reset( bso::bool__ = true )
		{}
		E_CVDTOR( widgets__ );
		void Init( void )
		{}
	};

	struct _session_view_core__
	: public _core__
	{
	protected:
		void _Refresh( xml::writer_ &Digest );
	public:
		widgets__ &Widgets;
		void reset( bso::bool__ = true )
		{
			// Standardisation.
		}
		E_VDTOR( _session_view_core__ );
		_session_view_core__( widgets__ &Widgets )
		: Widgets( Widgets )
		{}
		void Init( void )
		{
			// Standardisation.
		}
		void Register( nsIDOMWindow *Window );
	};

	namespace {
		template <typename trunk> E_TTCLONE__( xulfbs::_t_page__<trunk>, page__ );
	}

	template <typename trunk> struct session_view__
	: public _session_view_core__,
	  public page__<trunk>
	{
	private:
		xulftk::trunk___ &_Trunk( void )
		{
			return page__<trunk>::Trunk();
		}
		nsIDOMWindow *_Window( void )
		{
			return page__<trunk>::Window();
		}
		nsIDOMDocument *_Document( void )
		{
			return page__<trunk>::Document();
		}
	protected:
		void Refresh( xml::writer_ &Digest )
		{
			_session_view_core__::_Refresh( Digest );
		}
	public:
		void reset( bso::bool__ P = true )
		{
			_session_view_core__::reset( P );
			page__<trunk>::reset( P );
		}
		E_VDTOR( session_view__ );
		session_view__( widgets__ &Widgets )
		: _session_view_core__( Widgets )
		{}
		void Init( trunk &Trunk )
		{
			_session_view_core__::Init();
			page__<trunk>::Init( Trunk );
		}
		void Register( nsIDOMWindow *Window )
		{
			_session_view_core__::Register( Window );
			page__<trunk>::Register( Window );
		}
		trunk &Trunk( void )
		{
			return page__<trunk>::Trunk();
		}
		nsIDOMWindow *Window( void )
		{
			return page__<trunk>::Window();
		}
		nsIDOMDocument *Document( void )
		{
			return page__<trunk>::Document();
		}
		void Refresh( void )
		{
			page__<trunk>::Refresh();
		}
	};
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
