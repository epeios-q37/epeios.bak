/*
	Copyright (C) 2000-2015 Claude SIMON (http://q37.info/contact/).

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

#ifndef DWTDCT__INC
# define DWTDCT__INC

# define DWTDCT_NAME		"DWTDCT"

# if defined( E_DEBUG ) && !defined( DWTDCT_NODBG )
#  define DWTDCT_DBG
# endif

// Directory WatCher ConTent

# include "bso.h"
# include "err.h"
# include "str.h"
# include "stk.h"
# include "ltf.h"
# include "lstctn.h"

# include "dwtbsc.h"
# include "dwtxcl.h"
# include "dwtght.h"

namespace dwtdct {
	using namespace dwtbsc;

	qROW( irow__ );

	typedef bch::E_BUNCHt_( exclusion__, frow__ ) fexclusions_;
	E_AUTO( fexclusions );

	typedef bch::E_BUNCHt_( fil::size__, frow__ ) sizes_;
	E_AUTO( sizes );

	typedef bch::E_BUNCHt_( time_t, frow__ ) timestamps_;
	E_AUTO( timestamps );

	typedef ctn::E_MCONTAINERt_( str::string_, frow__ ) fstrings_;
	E_AUTO( fstrings );

	typedef ctn::E_MCONTAINERt_( str::string_, dwtbsc::grow__ ) gstrings_;
	E_AUTO( gstrings );

	class dir_data_
	{
	public:
		struct s
		{
			time_t TimeStamp;
			dwtght::grow__ GhostRow;	// Gestion du renommage de répertoire.
			name_::s Name;
			oddity_::s Oddity;
			exclusion__ Exclusion;
		} &S_;
		name_ Name;
		oddity_ Oddity;
		dir_data_( s &S )
		: S_( S ),
		  Name( S.Name ),
		  Oddity( S.Oddity )
		{}
		void reset( bso::bool__ P = true )
		{
			S_.TimeStamp = 0;
			S_.GhostRow = qNIL;
			S_.Exclusion = x_Undefined;

			Name.reset( P );
			Oddity.reset( P );
		}
		void plug( qAS_ &AS )
		{
			Name.plug( AS );
			Oddity.plug( AS );
		}
		dir_data_ &operator =( const dir_data_ &DD )
		{
			S_.TimeStamp = DD.S_.TimeStamp;
			S_.GhostRow = DD.S_.GhostRow;
			S_.Exclusion = DD.S_.Exclusion;

			Name = DD.Name;
			Oddity = DD.Oddity;

			return *this;
		}
		void Init( void )
		{
			S_.TimeStamp = 0;
			S_.GhostRow = qNIL;
			S_.Exclusion = x_Undefined;

			Name.Init();
			Oddity.Init();
		}
		E_RWDISCLOSE_( time_t, TimeStamp );
		E_RWDISCLOSE_( dwtght::grow__, GhostRow );
		E_RWDISCLOSE_( exclusion__, Exclusion );
	};

	class files_data_
	{
	public:
		struct s
		{
			fstrings_::s Names;
			fexclusions_::s Exclusions;
			sizes_::s Sizes;
			timestamps_::s TimeStamps;
		};
		fstrings_ Names;
		fexclusions_ Exclusions;
		sizes_ Sizes;
		timestamps_ TimeStamps;
		files_data_( s &S )
		: Names( S.Names ),
		  Exclusions( S.Exclusions ),
		  Sizes( S.Sizes ),
		  TimeStamps( S.TimeStamps )
		{}
		void reset( bso::bool__ P = true )
		{
			Names.reset( P );
			Exclusions.reset( P );
			Sizes.reset( P );
			TimeStamps.reset( P );
		}
		void plug( qAS_ &AS )
		{
			Names.plug( AS );
			Exclusions.plug( AS );
			Sizes.plug( AS );
			TimeStamps.plug( AS );
		}
		files_data_ &operator =( const files_data_ &FD )
		{
			Names = FD.Names;
			Exclusions = FD.Exclusions;
			Sizes = FD.Sizes;
			TimeStamps = FD.TimeStamps;

			return *this;
		}
		void Init( void )
		{
			Names.Init();
			Exclusions.Init();
			Sizes.Init();
			TimeStamps.Init();
		}
		E_NAVt( Names., frow__ );
	};

	class goofs_data_
	{
	public:
		struct s
		{
			gstrings_::s
				Names,
				Oddities;
		};
		gstrings_
			Names,
			Oddities;
		goofs_data_( s &S )
		: Names( S.Names ),
		  Oddities( S.Oddities )
		{}
		void reset( bso::bool__ P = true )
		{
			Names.reset( P );
			Oddities.reset( P );
		}
		void plug( qAS_ &AS )
		{
			Names.plug( AS );
			Oddities.plug( AS );
		}
		goofs_data_ &operator =( const goofs_data_ &GD )
		{
			Names = GD.Names;
			Oddities = GD.Oddities;

			return *this;
		}
		void Init( void )
		{
			Names.Init();
			Oddities.Init();
		}
	};

	class item_
	{
	public:
		struct s
		{
			irow__ Parent;
			str::string_::s Path;
			dir_data_::s Dir;
			files_data_::s Files;
			goofs_data_::s Goofs;
		} &S_;
		str::string_ Path;
		dir_data_ Dir;
		files_data_ Files;
		goofs_data_ Goofs;
		item_( s &S )
		: S_( S ),
		  Path( S.Path ),
		  Dir( S.Dir ),
		  Files( S.Files ),
		  Goofs( S.Goofs )
		{}
		void reset( bso::bool__ P = true )
		{
			S_.Parent = qNIL;

			Path.reset( P );
			Dir.reset( P );
			Files.reset( P );
			Goofs.reset( P );
		}
		void plug( qAS_ &AS )
		{
			Path.plug( AS );
			Dir.plug( AS );
			Files.plug( AS );
			Goofs.plug( AS );
		}
		item_ &operator =( const item_ &I )
		{
			S_.Parent = I.S_.Parent;

			Path = I.Path;
			Dir = I.Dir;
			Files = I.Files;
			Goofs = I.Goofs;

			return *this;
		}
		void Init( void )
		{
			S_.Parent = qNIL;

			Path.Init();
			Dir.Init();
			Files.Init();
			Goofs.Init();
		}
		E_RWDISCLOSE_( irow__, Parent );
	};

	E_AUTO( item )

	typedef bch::E_BUNCHt_( item_ *, irow__ ) _items_;

	void _Delete( _items_ &Items );

	class items_
	: public _items_
	{
	public:
		struct s
		: public _items_::s
		{};
		void reset( bso::bool__ P = true )
		{
			if ( P )
				_Delete( *this );

			_items_::reset( P );
		}
		items_( s &S )
		: _items_( S )
		{}
		// Other methds are inherited.
	};

	E_AUTO( items );

	typedef stk::E_BSTACK_( irow__ ) irows_;
	E_AUTO( irows );

	typedef bch::E_BUNCHt_( dwtbsc::files_ *, irow__ ) files_set_;
	E_AUTO( files_set );

	typedef bch::E_BUNCHt_( dwtbsc::commons_ *, irow__ ) commons_set_;
	E_AUTO( commons_set );

	typedef items_ content_;
	E_AUTO( content );

	using tol::timer__;

	class exploration_observer__
	: public timer__
	{
	protected:
		virtual void DWTDCTReport(
			bso::uint__ Handled,
			bso::uint__ ToHandle,
			tamount__ ThreadAmount ) = 0;
	public:
		void Report(
			bso::uint__ Handled,
			bso::uint__ ToHandle,
			tamount__ ThreadAmount )
		{
			if ( IsEnabled() ) {
				DWTDCTReport( Handled, ToHandle, ThreadAmount );
				Launch();
			}
		}
	};

	void Explore( 
		const str::string_ &Path,
		tamount__ ThreadAmountMax,
		const dwtxcl::excluder_ &Excluder,
		const dwtbsc::limitations__ &Limitations,
		const dwtbsc::ghosts_oddities_ &GO,
		exclusion_handling__ ExclusionHandling,
		content_ &Content,
		exploration_observer__ &Observer );

	class ghosts_setting_observer__
	: public timer__
	{
	protected:
		virtual void DWTDCTReport(
			bso::uint__ Handled,
			bso::uint__ Total,
			bso::uint__ Created,
			bso::uint__ Updated,
			bso::uint__ Skipped,
			bso::uint__ Failed,
			bso::uint__ Intruder,
			bso::uint__ Expected ) = 0;
	public:
		void Report(
			bso::uint__ Handled,
			bso::uint__ Total,
			bso::uint__ Created,
			bso::uint__ Updated,
			bso::uint__ Skipped,
			bso::uint__ Failed,
			bso::uint__ Intruder,
			bso::uint__ Expected )
		{
			if ( IsEnabled() ) {
				DWTDCTReport( Handled, Total, Created, Updated, Skipped, Failed, Intruder, Expected );
				Launch();
			}
		}
	};

	// ATTENTION : 'Content' doit avoir été récupèré avec 'rhKeepGhostLike'.
	void SetGhosts(
		const str::string_ &Root,
		const content_ &Content,
		const dwtbsc::ghosts_oddities_ &GO,
		ghosts_setting_observer__ &GhostsSettingObserver );	// Créer dans chaque répertoire le répertoire spécial servant à détecter le renommage des répertoires.

	// ATTENTION : 'Content' doit avoir été récupèré avec 'rhKeepGhostLike'.
	void DelGhosts(
		const str::string_ &Root,
		const content_ &Content,
		const dwtbsc::ghosts_oddities_ &GO,
		dwtdct::exploration_observer__ &Observer );	// Supprime toute trace des 'ghostt's

	class basic_ghosts_setting_observer___
	: public ghosts_setting_observer__
	{
	protected:
		virtual void DWTDCTReport(
			bso::uint__ Handled,
			bso::uint__ Total,
			bso::uint__ Created,
			bso::uint__ Updated,
			bso::uint__ Skipped,
			bso::uint__ Failed,
			bso::uint__ Intruder,
			bso::uint__ Expected );
	private:
		ltf::line_text_oflow___<> _Flow;
		str::string _Message;
	public:
		void reset( bso::bool__ P = true )
		{
			ghosts_setting_observer__::reset( P );
			_Flow.reset( P );
			_Message.reset( P );
		}
		E_CVDTOR( basic_ghosts_setting_observer___ );
		void Init(
			const str::string_ &Message,
			txf::text_oflow__ &TFLow,
			tol::delay__ Delay )
		{
			ghosts_setting_observer__::Init( Delay );

			_Flow.Init( TFLow );
			_Message.Init( Message );
		}
	};

	void TestGhosts(
		const str::string_ &Root,
		const content_ &Content,
		const dwtbsc::ghosts_oddities_ &GO,
		const str::string_ &NoGhostMessage,
		const str::string_ &GhostIgnoredMessage,
		txf::text_oflow__ &TFlow );

	class basic_exploration_observer___
	: public exploration_observer__
	{
	protected:
		virtual void DWTDCTReport(
			bso::uint__ Handled,
			bso::uint__ ToHandle,
			tamount__ ThreadAmount );
	private:
		str::string _Message;
		ltf::line_text_oflow___<> _Flow;
	public:
		void reset( bso::bool__ P = true )
		{
			exploration_observer__::reset( P );

			_Message.Init();
			_Flow.reset( P );
		}
		E_CVDTOR( basic_exploration_observer___ );
		void Init(
			tol::delay__ Delay,
			const str::string &Message,
			txf::text_oflow__ &TFlow )
		{
			exploration_observer__::Init( Delay );

			_Message.Init( Message );
			_Flow.Init( TFlow );
		}
	};
}

#endif
