/*
	Copyright (C) 1999-2017 Claude SIMON (http://q37.info/contact/).

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

//	$Id: tsrcpr.h,v 1.16 2012/11/14 16:06:38 csimon Exp $

#ifndef TSRCPR__INC
#define TSRCPR__INC

#define TSRCPR_NAME		"TSRCPR"

#define	TSRCPR_VERSION	"$Revision: 1.16 $"

#define TSRCPR_OWNER		"Claude SIMON (http://zeusw.org/intl/contact.html)"

#if defined( E_DEBUG ) && !defined( TSRCPR_NODBG )
#define TSRCPR_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.16 $
//C Claude SIMON (http://zeusw.org/intl/contact.html)
//R $Date: 2012/11/14 16:06:38 $

/* End of automatic documentation generation part. */

/* Addendum to the automatic documentation generation part. */
//D Tagged SouRCe PaRser 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/


#include "err.h"
#include "flw.h"
#include "que.h"
#include "ctn.h"
#include "str.h"
#include "xtf.h"

namespace tsrcpr {

	using que::managed_queue_;
	using ctn::multi_container_;


	template <class t> class table_
	: public E_MQUEUE_,
	  public E_CONTAINER_( t )
	{
	public:
		struct s
		: public E_MQUEUE_::s,
		  public E_CONTAINER_( t )::s
		{};
		table_( s &S )
		: E_MQUEUE_( S ),
		  E_CONTAINER_( t )( S ){}
		void reset( bool P = true )
		{
			E_MQUEUE_::reset( P );
			E_CONTAINER_( t )::reset( P );
		}
		void plug( mmm::multimemory_ &MM )
		{
			E_MQUEUE_::plug( MM );
			E_CONTAINER_( t )::plug( MM );
		}
		table_ &operator =( const table_ &T )
		{
			E_MQUEUE_::operator =( T );
			E_CONTAINER_( t )::operator =( T );

			return *this;
		}
		void Init( void )
		{
			E_MQUEUE_::Init();
			E_CONTAINER_( t )::Init();
		}
		tym::row__ New( void )
		{
			tym::row__ P = E_CONTAINER_( t )::New();

			E_MQUEUE_::Allocate( E_CONTAINER_( t )::Extent() );

			if ( E_MQUEUE_::Amount() )
				E_MQUEUE_::BecomeNext( P, E_MQUEUE_::Last() );
			else
				E_MQUEUE_::Create( P );

			E_CONTAINER_( t )::operator()( P ).Init();

			E_CONTAINER_( t )::Flush();

			return P;
		}
		void Add( const t &Objet )
		{
			E_CONTAINER_( t )::Store( Objet, New() );
		}
		E_NAV( E_MQUEUE_:: )
	};

	E_AUTO1( table )

	template <class t> inline txf::text_iflow___ &operator >>(
		txf::text_iflow___ &Flot,
		table_< t > &T )
	{
		return Flot >> *(E_MQUEUE_ *)&T;
	}

	//c An item.
	class item_
	{
	public:
		//o Name of item.
		str::string_ Name;
		//o Commentary.
		str::string_ Commentary;
		struct s {
			str::string_::s Name;
			str::string_::s Commentary;
		};
		item_( s &S )
		: Name( S.Name ),
		  Commentary( S.Commentary )
		{}
		void reset( bool P = true )
		{
			Name.reset( P );
			Commentary.reset( P );
		}
		void plug( mmm::multimemory_ &M )
		{
			Name.plug( M );
			Commentary.plug( M );
		}
		item_ &operator =( const item_ &I )
		{
			Name = I.Name;
			Commentary = I.Commentary;

			return *this;
		}
		//f Initialization.
		void Init( void )
		{
			Name.Init();
			Commentary.Init();
		}
		/*f Parse 'Flow' with long commentayr if 'Long' ar true */
		void Analyze(
			xtf::extended_text_iflow___ &IStream,
			bso::bool__ Long );
	};

	//c An enum.
	class enum_
	{
	private:
		void AnalyzeItems_( xtf::extended_text_iflow___ &Flow );
	public:
		//o Enum name.
		str::string_ Name;
		//o Associated commentary.
		str::string_ Commentary;
		//o Items.
		table_<item_> Items;
		struct s {
			str::string_::s Name;
			str::string_::s Commentary;
			table_<item_>::s Items;
		};
		enum_( s &S )
		: Name( S.Name ),
		  Commentary( S.Commentary ),
		  Items( S.Items )
		{}
		void reset( bool P = true )
		{
			Name.reset( P );
			Commentary.reset( P );
			Items.reset( P );
		}
		void plug( mmm::multimemory_ &M )
		{
			Name.plug( M );
			Commentary.plug( M );
			Items.plug( M );
		}
		enum_ &operator =( const enum_ &E )
		{
			Name = E.Name;
			Commentary = E.Commentary;
			Items = E.Items;

			return *this;
		}
		//f Initialization.
		void Init( void )
		{
			Name.Init();
			Commentary.Init();
			Items.Init();
		}
		/*f Analyze 'Flow' with long commentary if 'Long' at true. */
		void Analyze(
			xtf::extended_text_iflow___ &Flow,
			bso::bool__ Long );
	};
			
	//c Une dfinition de type (typedef).
	class typedef_
	{
	public:
		//o Le nom du type.
		str::string_ Name;
		//o Le commentaire associ.
		str::string_ Commentaire;
		struct s
		{
			str::string_::s Name;
			str::string_::s Commentaire;
		};
		typedef_( s &S )
		: Name( S.Name ),
		  Commentaire( S.Commentaire ){}
		void reset( bool P = true )
		{
			Name.reset( P );
			Commentaire.reset( P );
		}
		void plug( mmm::multimemory_ &M )
		{
			Name.plug( M );
			Commentaire.plug( M );
		}
		typedef_ &operator =( const typedef_ &T )
		{
			Init();

			Name = T.Name;
			Commentaire = T.Commentaire;

			return *this;
		}
		//f Initialisation.
		void Init( void )
		{
			Name.Init();
			Commentaire.Init();
		}
		/*f Analyse 'Stream' sachant que la doc. se situe dans un commentaire long
		si 'Long' est  vrai */
		void Analyze(
			xtf::extended_text_iflow___ &IStream,
			bso::bool__ Long );
	};

	inline txf::text_oflow___ &operator<<(
		txf::text_oflow___ &Flot,
		const typedef_ &T )
	{
		return Flot << "T:" << T.Name << ": " << T.Commentaire;
	}
	/*
	inline flw_ascii_iflow_ &operator>>(
		flw_ascii_iflow_ &Flot,
		typedef_ &T )
	{
		Flot >> T.Name;
		Flot >> T.Commentaire;
		return Flot >> end;
	}
	*/


	//c Un '#define'.
	class define_
	{
	private:
		// Analyse des paramtres pour le stream 'Stream'.
		void AnalyserArguments_( xtf::extended_text_iflow___ &Flot );
	public:
		//o Le nom du define.
		str::string_ Name;
		//o Le commentaire associ.
		str::string_ Commentaire;
		//o Les parametres.
		table_<str::string_> Arguments;
		struct s
		{
			str::string_::s Name;
			str::string_::s Commentaire;
			table_<str::string_>::s Arguments;
		};
		define_( s &S )
		: Name( S.Name ),
		  Commentaire( S.Commentaire ),
		  Arguments( S.Arguments ){}
		void reset( bool P = true )
		{
			Name.reset( P );
			Commentaire.reset( P );
			Arguments.reset( P );
		}
		void plug( mmm::multimemory_ &M )
		{
			Name.plug( M );
			Commentaire.plug( M );
			Arguments.plug( M );
		}
		define_ &operator =( const define_ &M )
		{
			Init();

			Name = M.Name;
			Commentaire = M.Commentaire;
			Arguments = M.Arguments;

			return *this;
		}
		//f Initialisation.
		void Init( void )
		{
			Name.Init();
			Commentaire.Init();
			Arguments.Init();
		}
		/*f Analyse 'Stream' sachant que la doc. se situe dans un commentaire long
		si 'Long' est  vrai */
		void Analyze(
			xtf::extended_text_iflow___ &Flot,
			bso::bool__ Long );
	};

	/*
	inline flw_ascii_iflow_ &operator>>(
		flw_ascii_iflow_ &Flot,
		macro_ &M )
	{
		Flot >> begin;
		Flot >> tag >> data >> M.Name;
		Flot >> tag >> data >> M.Commentaire;
		Flot >> tag >> data >> M.Arguments;
		return Flot >> end;
	}
	*/


	//c A 'shortcut'..
	class shortcut_
	: public define_
	{
	public:
		//o L'alias.
		str::string_ Alias;
		struct s
		: public define_::s
		{
			str::string_::s Alias;
		};
		shortcut_( s &S )
		: define_( S ),
		  Alias( S.Alias ){}
		void reset( bool P = true )
		{
			define_::reset( P );
			Alias.reset( P );
		}
		void plug( mmm::multimemory_ &M )
		{
			define_::plug( M );
			Alias.plug( M );
		}
		shortcut_ &operator =( const shortcut_ &S )
		{
			Init();

			define_::operator =( S );
			Alias = S.Alias;

			return *this;
		}
		//f Initialisation.
		void Init( void )
		{
			define_::Init();
			Alias.Init();
		}
		/*f Analyse 'Stream' sachant que la doc. se situe dans un commentaire long
		si 'Long' est  vrai */
		void Analyze(
			xtf::extended_text_iflow___ &Flot,
			bso::bool__ Long );
	};

	inline txf::text_oflow___ &operator<<(
		txf::text_oflow___ &Flot,
		const shortcut_ &S )
	{
		return Flot << "D " << S.Name << ", " << S.Alias << ": " << S.Commentaire;
	}




	//c Un paramtre.
	class parametre_
	{
	public:
		struct s
		{
			str::string_::s
				Type,
				Name,
				Valeur;
		};
		//o Le type.
		str::string_ Type;
		//o Le nom.
		str::string_ Name;
		//o La valeur par dfaut.
		str::string_ Valeur;
		parametre_( s &S )
		: Type( S.Type ),
		  Name( S.Name ),
		  Valeur( S.Valeur ){}
		void reset( bool P = true )
		{
			Type.reset( P );
			Name.reset( P );
			Valeur.reset( P );
		}
		//f Initialisation.
		void Init( void )
		{
			Type.Init();
			Name.Init();
			Valeur.Init();
		}
		// Affectation.
		parametre_ &operator =( const parametre_ &P )
		{
			Type = P.Type;
			Name = P.Name;
			Valeur = P.Valeur;

			return *this;
		}
		//f Analyse du ficher 'Stream'.
		void Analyze( xtf::extended_text_iflow___ &Flot );
		void plug( mmm::multimemory_ &Multimemoire )
		{
			Type.plug( Multimemoire );
			Name.plug( Multimemoire );
			Valeur.plug( Multimemoire );
		}
	};

	E_AUTO( parametre );

	//f Operateur de sortie pour 'P'.
	inline txf::text_oflow___ &operator <<(
		txf::text_oflow___ &Flot,
		const parametre_ &P )
	{
		return Flot << P.Type << ' ' << P.Name << ' ' << P.Valeur;
	}
	/*
	//f Operateur de sortie pour 'P'.
	inline flw_ascii_iflow_ &operator >>(
		flw_ascii_iflow_ &Flot,
		parametre_ &P )
	{
		Flot >> begin;
		Flot >> tag >> data >> P.Type;
		Flot >> tag >> data >> P.Name;
		Flot >> tag >> data >> P.Valeur;
		return Flot >> end;
	}
	*/
	//c Un argument de dclaration de template.
	class argument_
	{
	public:
		//o Le type ('class', 'int', 'long', ... ).
		str::string_ Type;
		//o Le nom.
		str::string_ Name;
		struct s
		{
			str::string_::s Type;
			str::string_::s Name;
		};
		argument_( s &S )
		: Type( S.Type ),
		  Name( S.Name ){}
		void reset( bool P = true )
		{
			Type.reset( P );
			Name.reset( P );
		}
		void plug( mmm::multimemory_ &M )
		{
			Type.plug( M );
			Name.plug( M );
		}
		//f Initialisation.
		void Init( void )
		{
			Type.Init();
			Name.Init();
		}
		//f Analyse 'Stream'
		void Analyze( xtf::extended_text_iflow___ &Flot );
		argument_ &operator =( const argument_ &T )
		{
			Type = T.Type;
			Name = T.Name;

			return *this;
		}
	};

	E_AUTO( argument )

	inline txf::text_oflow___ &operator <<(
		txf::text_oflow___ &Flot,
		const argument_ &P )
	{
		return Flot << P.Type << ' ' << P.Name;
	}
	/*
	inline flw_ascii_iflow_ &operator >>(
		flw_ascii_iflow_ &Flot,
		argument_ &P )
	{
		Flot >> begin;
		Flot >> tag >> data >> P.Type;
		Flot >> tag >> data >> P.Name;
		return Flot >> end;
	}
	*/

	//c Une liste de dclaration de templates
	class template_
	{
	public:
		//o La liste des dclarations de templates.
		table_<argument_> Arguments;
		struct s
		{
			table_<argument_>::s Arguments;
		};
		template_( s &S )
		: Arguments( S.Arguments ){}
		void reset( bool P = true )
		{
			Arguments.reset( P );
		}
		void plug( mmm::multimemory_ &M )
		{
			Arguments.plug( M );
		}
		//f Initialisation
		void Init( void )
		{
			Arguments.Init();
		}
		//f Analyse de 'Stream'.
		void Analyze( xtf::extended_text_iflow___ &Flot );
		template_ &operator =( const template_ &T )
		{
			Arguments = T.Arguments;

			return *this;
		}
	};

	/*
	//f Entree standard de 'P';
	inline flw_ascii_iflow_ &operator >>(
		flw_ascii_iflow_ &Flot,
		template_ &P )
	{
		return Flot >> begin >> tag >> data >> P.Arguments >> end;
	}
	*/


	//c Une mthode.
	class methode_
	{
	private:
		void AnalyserParametres_( xtf::extended_text_iflow___ &Flot );
	public:
		struct s {
			str::string_::s
				Type,
				Name,
				Commentaire;
			table_<parametre_>::s Parametres;
		};
		//o Les paramtres.
		table_<parametre_> Parametres;
		//o Le type.
		str::string_ Type;
		//o Le nom.
		str::string_ Name;
		//o Le commentaire.
		str::string_ Commentaire;
		methode_( s &S )
		: Parametres( S.Parametres ),
		  Type( S.Type ),
		  Name( S.Name ),
		  Commentaire( S.Commentaire ){}
		void reset( bool P = true )
		{
			Type.reset( P );
			Name.reset( P );
			Parametres.reset( P );
			Commentaire.reset( P );
		}
		//f Initialisation.
		void Init( void )
		{
			Type.Init();
			Name.Init();
			Parametres.Init();
			Commentaire.Init();
		}
		/*f Analyse 'Stream' sachant qu'il est prcd par un commentaire long
		si 'Long' == true. */
		void Analyze(
			xtf::extended_text_iflow___ &Flot,
			bso::bool__ Long );
		void plug( mmm::multimemory_ &M )
		{
			Type.plug( M );
			Name.plug( M );
			Parametres.plug( M );
			Commentaire.plug( M );
		}
		methode_ &operator =( const methode_ &O )
		{
			Type = O.Type;
			Name = O.Name;
			Parametres = O.Parametres;
			Commentaire = O.Commentaire;

			return *this;
		}
	};

	E_AUTO( methode )


	/*
	//f Lecture standard.
	inline flw_ascii_iflow_ &operator >>(
		flw_ascii_iflow_ &Flot,
		methode_ &F )
	{
		Flot >> begin;
		Flot >> tag >> data >> F.Type;
		Flot >> tag >> data >> F.Name;
		Flot >> tag >> data >> F.Commentaire;
		Flot >> tag >> data >> F.Parametres;
		return Flot >> end;
	}
	*/

	//c Une function.
	class function_
	{
	private:
		void AnalyserParametres_( xtf::extended_text_iflow___ &Flot );
	public:
		struct s {
			str::string_::s
				Type,
				Name,
				Commentaire;
			table_<parametre_>::s Parametres;
			template_::s Template;
		};
		//o La dclaration de template
		template_ Template;
		//o Les paramtres.
		table_<parametre_> Parametres;
		//o Le type.
		str::string_ Type;
		//o Le nom.
		str::string_ Name;
		//o Le commentaire.
		str::string_ Commentaire;
		function_( s &S )
		: Template( S.Template ),
		  Parametres( S.Parametres ),
		  Type( S.Type ),
		  Name( S.Name ),
		  Commentaire( S.Commentaire ){}
		void reset( bool P = true )
		{
			Template.reset( P );
			Type.reset( P );
			Name.reset( P );
			Parametres.reset( P );
			Commentaire.reset( P );
		}
		//f Initialisation.
		void Init( void )
		{
			Template.Init();
			Type.Init();
			Name.Init();
			Parametres.Init();
			Commentaire.Init();
		}
		/*f Analyse 'Stream' sachant qu'il est prcd par un commentaire long
		si 'Long' == true. */
		void Analyze(
			xtf::extended_text_iflow___ &Flot,
			bso::bool__ Long );
		void plug( mmm::multimemory_ &M )
		{
			Template.plug( M );
			Type.plug( M );
			Name.plug( M );
			Parametres.plug( M );
			Commentaire.plug( M );
		}
		function_ &operator =( const function_ &O )
		{
			Template = O.Template;
			Type = O.Type;
			Name = O.Name;
			Parametres = O.Parametres;
			Commentaire = O.Commentaire;

			return *this;
		}
	};

	E_AUTO( function )


	/*
	//f Lecture standard.
	inline flw_ascii_iflow_ &operator >>(
		flw_ascii_iflow_ &Flot,
		function_ &F )
	{
		Flot >> begin;
		Flot >> tag >> data >> F.Template;
		Flot >> tag >> data >> F.Type;
		Flot >> tag >> data >> F.Name;
		Flot >> tag >> data >> F.Commentaire;
		Flot >> tag >> data >> F.Parametres;
		return Flot >> end;
	}
	*/

	//c Un objet contenu dans une classe.
	class objet_
	{
	public:
		struct s
		{
			str::string_::s
				Type,
				Name,
				Commentaire;
			friend class objet_;
		};
		//o Le type.
		str::string_ Type;
		//o Le nom.
		str::string_ Name;
		//o Le commentaire.
		str::string_ Commentaire;
		objet_( s &S )
		: Type( S.Type ),
		  Name( S.Name ),
		  Commentaire( S.Commentaire ){}
		void reset( bool P = true )
		{
			Type.reset( P );
			Name.reset( P );
			Commentaire.reset( P );
		}
		//f Initialisation.
		void Init( void )
		{
			Type.Init();
			Name.Init();
			Commentaire.Init();
		}
		/*f Analyse le contenu de 'Stream' sachant qu'il est prcde
		par un commentaire long si 'Long', court sinon. */
		void Analyze(
			xtf::extended_text_iflow___ &Flot,
			bso::bool__ Long );
		void plug( mmm::multimemory_ &M )
		{
			Type.plug( M );
			Name.plug( M );
			Commentaire.plug( M );
		}
		objet_ &operator =( const objet_ &O )
		{
			Type = O.Type;
			Name = O.Name;
			Commentaire = O.Commentaire;

			return *this;
		}
	};

	E_AUTO( objet )

	inline txf::text_oflow___ &operator <<(
		txf::text_oflow___ &Flot,
		const objet_ &O )
	{
		return Flot << "O " << O.Type << ' ' << O.Name << ": " << O.Commentaire;
	}
	/*
	inline flw_ascii_iflow_ &operator >>(
		flw_ascii_iflow_ &Flot,
		objet_ &O )
	{
		Flot >> O.Type;
		Flot >> O.Name;
		return Flot >> O.Commentaire;
	}
	*/

	//c Une classe.
	class classe_
	{
	private:
		void AnalyzeBases_( xtf::extended_text_iflow___ &Flot );
	public:
		struct s
		{
			table_<methode_>::s Methodes;
			table_<methode_>::s Virtuels;
			table_<objet_>::s Objets;
			table_<objet_>::s Restreints;
			table_<str::string_>::s Bases;
			str::string_::s Commentaire;
			str::string_::s Name;
			str::string_::s Type;
			template_::s Template;
			friend class classe_;
		};
		//o Les mthodes.
		table_<methode_> Methodes;
		//o Les mthodes virtuelles.
		table_<methode_> Virtuels;
		//o Les objets.
		table_<objet_> Objets;
		//o Les objets  accs restreints.
		table_<objet_> Restreints;
		//o Bases classes.
		table_<str::string_> Bases;
		//o Le commentaire.
		str::string_ Commentaire;
		//o Le nom.
		str::string_ Name;
		//o Le type ('struct' ou  'class')
		str::string_ Type;
		//o Les dclarations de templates.
		template_ Template;
		classe_( s &S )
		: Methodes( S.Methodes ),
		  Commentaire( S.Commentaire ),
		  Name( S.Name ),
		  Objets( S.Objets ),
		  Type( S.Type ),
		  Template( S.Template ),
		  Virtuels( S.Virtuels ),
		  Restreints( S.Restreints ),
		  Bases( S.Bases )
		{}
		  void reset( bool P = true )
		{
			Methodes.reset( P );
			Commentaire.reset( P );
			Name.reset( P );
			Objets.reset( P );
			Type.reset( P );
			Template.reset( P );
			Virtuels.reset( P );
			Restreints.reset( P );
			Bases.reset( P );
		}
		//f Initialisation.
		void Init( void )
		{
			Methodes.Init();
			Commentaire.Init();
			Name.Init();
			Objets.Init();
			Type.Init();
			Template.Init();
			Virtuels.Init();
			Restreints.Init();
			Bases.Init();
		}
		/*f Analyse 'Stream' sachant que l'on se situe dans un commentaire long
		si 'Long', court sinon. */
		void Analyze(
			xtf::extended_text_iflow___ &Flot,
			bso::bool__ Long );
		void plug( mmm::multimemory_ &M )
		{
			Methodes.plug( M );
			Objets.plug( M );
			Commentaire.plug( M );
			Name.plug( M );
			Type.plug( M );
			Template.plug( M );
			Virtuels.plug( M );
			Restreints.plug( M );
			Bases.plug( M );
		}
		//f Affectation.
		classe_ &operator =( const classe_ &C )
		{
			Methodes = C.Methodes;
			Objets = C.Objets;
			Commentaire = C.Commentaire;
			Name = C.Name;
			Type = C.Type;
			Template = C.Template;
			Virtuels = C.Virtuels;
			Restreints = C.Restreints;
			Bases = C.Bases;

			return *this;
		}
	};

	E_AUTO( classe )

	/*
	//f Operateur d"entre pour 'C'.
	inline flw_ascii_iflow_ &operator >>(
		flw_ascii_iflow_ &Flot,
		classe_ &C )
	{
		Flot >> begin;
		Flot >> tag >> data >> C.Name;
		Flot >> tag >> data >> C.Commentaire;
		Flot >> tag >> data >> C.Methodes;
		Flot >> tag >> data >> C.Virtuels;
		Flot >> tag >> data >> C.Objets;
		Flot >> tag >> data >> C.Restreints;
		Flot >> tag >> data >> C.Type;
		Flot >> tag >> data >> C.Template;
		return Flot >> end;
	}
	*/

	//c Un source.
	class library_
	{
	public:
		//o Les functions.
		table_<function_> Functions;
		//o Les objets.
		table_<objet_> Objets;
		//o Les classes.
		table_<classe_> Classes;
		//o Les typedefs.
		table_<typedef_> Typedefs;
		//o Les 'define's.
		table_<define_> Defines;
		//o Les shortcuts.
		table_<shortcut_> Shortcuts;
		//o Enums.
		table_<enum_> Enums;
		//o Coordinators.
		str::string_ Coordinators;
		//o Version.
		str::string_ Version;
		//o Short description.
		str::string_ Description;
		//o Date & time of release.
		str::string_ Release;
		struct s
		{
			table_<function_>::s Functions;
			table_<objet_>::s Objets;
			table_<classe_>::s Classes;
			table_<typedef_>::s Typedefs;
			table_<define_>::s Defines;
			table_<shortcut_>::s Shortcuts;
			table_<enum_>::s Enums;
			str::string_::s Coordinators;
			str::string_::s Version;
			str::string_::s Description;
			str::string_::s Release;
		};
		library_( s &S )
		: Functions( S.Functions ),
		  Objets( S.Objets ),
		  Classes( S.Classes ),
		  Typedefs( S.Typedefs ),
		  Defines( S.Defines ),
		  Shortcuts( S.Shortcuts ),
		  Enums( S.Enums ),
		  Coordinators( S.Coordinators ),
		  Version( S.Version ),
		  Description( S.Description ),
		  Release( S.Release ){}
		void reset( bool P = true )
		{
			Functions.reset( P );
			Objets.reset( P );
			Classes.reset( P );
			Typedefs.reset( P );
			Defines.reset( P );
			Shortcuts.reset( P );
			Enums.reset( P );
			Coordinators.reset( P );
			Version.reset( P );
			Description.reset( P );
			Release.reset( P );
		}
		void Init( void )
		{
			Functions.Init();
			Objets.Init();
			Classes.Init();
			Typedefs.Init();
			Defines.Init();
			Shortcuts.Init();
			Enums.Init();
			Coordinators.Init();
			Version.Init();
			Description.Init();
			Release.Init();

		}
		//f Analyze 'Flot'.
		void Analyze( xtf::extended_text_iflow___ &Flot );
		void plug( mmm::multimemory_ &M )
		{
			Functions.plug( M );
			Objets.plug( M );
			Classes.plug( M );
			Typedefs.plug( M );
			Defines.plug( M );
			Shortcuts.plug( M );
			Enums.plug( M );
			Coordinators.plug( M );
			Version.plug( M );
			Description.plug( M );
			Release.plug( M );
		}
		library_ &operator =( const library_ &S )
		{
			Functions = S.Functions;
			Objets = S.Objets;
			Classes = S.Classes;
			Typedefs = S.Typedefs;
			Shortcuts = S.Shortcuts;
			Enums = S.Enums;
			Defines = S.Defines;
			Coordinators = S.Coordinators;
			Version = S.Version;
			Description = S.Description;
			Release = S.Release;

			return *this;
		}
	};

	/*
	//f Operateur d'entre pour 'S'.
	inline flw_ascii_iflow_ &operator >>(
		flw_ascii_iflow_ &Flot,
		source_ &S )
	{
		Flot >> begin;
		Flot >> tag >> data >> S.Functions;
		Flot >> tag >> data >> S.Objets;
		Flot >> tag >> data >> S.Classes;
		Flot >> tag >> data >> S.Typedefs;
		Flot >> tag >> data >> S.Defines;
		Flot >> tag >> data >> S.Macros;
		return Flot >> end;
	}
	*/

	E_AUTO( library )
}

/*$END$*/
#endif
