/*
	Copyright (C) 2015 Claude SIMON (http://q37.info/contact/).

	This file is part of fwtchrq.

    fwtchrq is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

    fwtchrq is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with fwtchrq.  If not, see <http://www.gnu.org/licenses/>
*/


#ifndef STORE__INC
# define STORE__INC

# include "dwtftr.h"
# include "dwtmov.h"
# include "dwtcpr.h"
//# include "fwtcmd.h"

# include "xml.h"

namespace store {
	// Version globale du contenu du flux XML.
	enum version__ {
		v0_1,
		v_amount,
		v_Current = v_amount - 1,
		v_Undefined
	};

	inline const char *GetLabel( version__ Version )
	{
		switch ( Version ) {
		case v0_1:
			return "0.1";
				break;
			qRGnr();
			break;
		}

		return NULL;	// Pour éviter un 'warning'.
	}

	version__ GetVersion( const str::string_ &Pattern );

	// Contenu du flux XML.
	// ATTENTION : au sens large.
	enum content__ {
		cTreeAndMovings,	// Contenu de l'arborescence + mouvements des répertoires.
		cModifications,	// Modifications entre deux arborescences, dont les mouvements de réperetoires.
		c_amount,
		c_Undefined,
		c_WhatEver = c_Undefined
	};

	const char *GetLabel( content__ Content );

	content__ GetContent( const str::string_ &Pattern );

	inline dwtftr::version__ GetFWTFTRVersion( version__ Version )
	{
		switch ( Version ) {
		case v0_1:
			return dwtftr::v0_1;
			break;
		default:
			qRGnr();
			break;
		}

		return dwtftr::v_Undefined;	// Pour éviter une 'warning'.
	}

	inline dwtmov::version__ GetFWTMOVVersion( version__ Version )
	{
		switch ( Version ) {
		case v0_1:
			return dwtmov::v0_1;
			break;
		default:
			qRGnr();
			break;
		}

		return dwtmov::v_Undefined;	// Pour éviter une 'warning'.
	}

	inline dwtcpr::version__ GetFWTCPRVersion( version__ Version )
	{
		switch ( Version ) {
		case v0_1:
			return dwtcpr::v0_1;
			break;
		default:
			qRGnr();
			break;
		}

		return dwtcpr::v_Undefined;	// Pour éviter une 'warning'.
	}

	class store__ 
	{
	private:
		content__ _Content;
	public:
		dwtftr::drow__ TreeRoot;
		const dwtftr::file_tree_ *Tree;
		dwtcpr::drow__ SceneRoot;
		const dwtcpr::scene_ *Scene;
		const dwtmov::movings_ *Movings;
		void reset( bso::bool__ = true )
		{
			TreeRoot = qNIL;
			Tree = NULL;

			SceneRoot = qNIL;
			Scene = NULL;

			Movings = NULL;

			_Content = c_Undefined;
		}
		E_CDTOR( store__ );
		void Init( void )
		{
			TreeRoot = qNIL;
			Tree = NULL;

			SceneRoot = qNIL;
			Scene = NULL;

			Movings = NULL;

			_Content = c_Undefined;
		}
		void InitAsTreeAndMovings(
			dwtftr::drow__ Root,
			const dwtftr::file_tree_ &Tree,
			const dwtmov::movings_ &Movings )
		{
			Init();

			_Content = cTreeAndMovings;
			TreeRoot = Root;
			this->Tree = &Tree;
			this->Movings = &Movings;
		}
		void InitAsModifications(
			dwtcpr::drow__ Root,
			const dwtcpr::scene_ &Scene,
			const dwtmov::movings_ &Movings )
		{
			Init();

			_Content = cModifications;
			SceneRoot = Root;
			this->Scene = &Scene;
			this->Movings = &Movings;
		}
		E_RODISCLOSE__( content__, Content );
	};

	class store___
	: public store__
	{
	public:
		dwtftr::file_tree Tree;
		dwtcpr::scene Scene;
		dwtmov::movings Movings;
		void reset( bso::bool__ P = true )
		{
			store__::reset( P );

			Tree.reset( P );
			Scene.reset( P );
			Movings.reset( P );
		}
		E_CDTOR( store___ );
		void Init( content__ Content = c_Undefined )
		{
			store__::Init();

			Tree.Init();
			Scene.Init();
			Movings.Init();

			switch ( Content ) {
			case cTreeAndMovings:
				InitAsTreeAndMovings( qNIL, Tree, Movings );
				break;
			case cModifications:
				InitAsModifications( qNIL, Scene, Movings );
				break;
			case c_Undefined:
				break;
			default:
				qRGnr();
				break;
			}
		}
	};

	void SaveTreeAndMovings(
		dwtftr::drow__ Row,
		const dwtftr::file_tree_ &Tree,
		const dwtmov::movings_ &Movings,
		xml::writer_ &Writer );

	dwtftr::drow__ LoadTreeAndMovings(
		xml::parser___ &Parser,
		dwtftr::load_observer__ &Observer,
		dwtftr::version__ TreeVersion,
		dwtftr::file_tree_ &Tree,
		dwtmov::version__ MovingsVersion,
		dwtmov::movings_ &Movings );

	void SaveModifications(
		dwtcpr::drow__ Root,
		const dwtcpr::scene_ &Scene,
		const dwtmov::movings_ &Movings,
		xml::writer_ &Writer );

	dwtcpr::drow__ LoadModifications(
		xml::parser___ &Parser,
		dwtcpr::load_observer__ &Observer,
		dwtcpr::version__ SceneVersion,
		dwtcpr::scene_ &Scene,
		dwtmov::version__ MovingsVersion,
		dwtmov::movings_ &Movings );

	void Save(
		const store__ &Store,
		const str::string_ &Generator,
		xml::writer_ &Writer );

	void Save(
		const store__ &Store,
		const str::string_ &Generator,
		txf::text_oflow__ &Flow );

	void Save(
		const store__ &Store,
		const str::string_ &Generator,
		const str::string_ &FileName );	// 'FileName' peut être vide, auquel cas c'est la console qui est utilisée.

	class observers__ {
	private:
		dwtftr::load_observer__ *_TreeObserver;
		dwtcpr::load_observer__ *_SceneObserver;
	public:
		void reset( bso::bool__ = true )
		{
			_TreeObserver = NULL;
			_SceneObserver = NULL;
		}
		E_CDTOR( observers__ );
		void Init(
			dwtftr::load_observer__ &TreeObserver,
			dwtcpr::load_observer__ &SceneObserver )
		{
			_TreeObserver = &TreeObserver;
			_SceneObserver = &SceneObserver;
		}
		dwtftr::load_observer__ &TreeObserver( void ) const
		{
			if ( _TreeObserver == NULL )
				qRGnr();

			return *_TreeObserver;
		}
		dwtcpr::load_observer__ &SceneObserver( void ) const
		{
			if ( _SceneObserver ==  NULL )
				qRGnr();

			return *_SceneObserver;
		}
	};

	bso::bool__ Load(
		xml::parser___ &Parser,
		content__ ExpectedContent,	// Si != 'c_Undefined', retourne 'false' si le contenu n'est pas celui attendu, 'true' sinon.
		const observers__ &Observers,
		store___ &Storey );

	bso::bool__ Load(
		flw::iflow__ &Flow,
		content__ ExpectedContent,	// Si != 'c_Undefined', retourne 'false' si le contenu n'est pas celui attendu, 'true' sinon.
		const observers__ &Observers,
		store___ &Store );

	bso::bool__ Load(
		const fnm::name___ &FileName,
		content__ ExpectedContent,	// Si != 'c_Undefined', retourne 'false' si le contenu n'est pas celui attendu, 'true' sinon.
		const observers__ &Observers,
		store___ &Store );

	class basic_observers___
	: public observers__
	{
	private:
		dwtftr::basic_load_observer___ TreeObserver;
		dwtcpr::basic_load_observer___ SceneObserver;
	public:
		void reset( bso::bool__ P = true )
		{
			observers__::reset( P );

			TreeObserver.reset( P );
			SceneObserver.reset( P );
		}
		E_CDTOR( basic_observers___ );
		void Init( 
			const str::string_ &TreeMessage,
			const str::string_ &SceneMessage,
			txf::text_oflow__ &Flow,
			tol::delay__ Delay )
		{
			TreeObserver.Init( TreeMessage, Flow, Delay );
			SceneObserver.Init( SceneMessage, Flow, Delay );

			observers__::Init( TreeObserver, SceneObserver );
		}
	};
}

#endif
