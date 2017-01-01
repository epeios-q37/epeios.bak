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

#ifndef CLNARG__INC
# define CLNARG__INC

# define CLNARG_NAME		"CLNARG"

# if defined( E_DEBUG ) && !defined( CLNARG_NODBG )
#  define CLNARG_DBG
# endif

// Command LiNe ARGuments

# include "err.h"
# include "flw.h"
# include "bso.h"
# include "bch.h"
# include "ctn.h"
# include "str.h"
# include "lcl.h"
# include "cio.h"

#include <stdarg.h>


#define CLNARG_DEFAULT_FLAG	'-'

//d The command seperator for the general usage printing ('GetCommandLabels' method).
#define CLNARG_GENERAL_SEPARATOR	"|"

//d The command seperator for the detail usage printing ('GetCommandLabels' method).
#define CLNARG_DETAIL_SEPARATOR	", "

#define CLNARG_STRING_PARAM___( name )\
	TOL_CBUFFER___ name

#	ifdef CLNARG_BUFFER_SIZE
#		define CLNARG__BUFFER_SIZE	CLNARG_BUFFER_SIZE
#else
#	define CLNARG__BUFFER_SIZE	50
#endif

#define CLNARG_BUFFER__	clnarg::buffer__

namespace clnarg {

	typedef char buffer__[CLNARG__BUFFER_SIZE+1];	// +1 pour '\0'.

	//t An option/argument id.
	typedef bso::u8__ id__;
	// if modified, modify below.

	//d Value the get an id when no value.
	#define CLNARG_NONE			BSO_S8_MAX
	#define CLNARG_ID_MAX		( CLNARG_NONE - 1 )

	//d Value of no short option/command.
	#define CLNARG_NO_SHORT	0

	//d Value to gice if no long option/command.
	#define CLNARG_NO_LONG	NULL

	enum message__ {
		mHelpHintMessage,
		mOptionWording,
		mOptionsWording,
		mArgumentWording,
		mArgumentsWording,
		mVersionCommandDescription,
		mLicenseCommandDescription,
		mHelpCommandDescription,
		mMissingCommandError,
		mUnknownOptionError,
		mMissingOptionArgumentError,
		mUnexpectedOptionError,
		mWrongNumberOfArgumentsError,
		b_amount,
		b_Undefined
	};

	const char *GetLabel( message__ Message );

	void GetMeaning(
		message__ Message,
		lcl::meaning_ *Meaning,
		... );

	inline void GetHelpHintMessageMeaning(
		const char *ProgramName,
		lcl::meaning_ &Meaning )
	{
		return GetMeaning( mHelpHintMessage, &Meaning, ProgramName );
	}

	inline void GetOptionWordingMeaning( lcl::meaning_ &Meaning )
	{
		return GetMeaning( mOptionWording, &Meaning );
	}

	inline void GetOptionsWordingMeaning( lcl::meaning_ &Meaning )
	{
		return GetMeaning( mOptionsWording, &Meaning );
	}

	inline void GetArgumentWordingMeaning( lcl::meaning_ &Meaning )
	{
		return GetMeaning( mArgumentWording, &Meaning );
	}

	inline void GetArgumentsWordingMeaning( lcl::meaning_ &Meaning )
	{
		return GetMeaning( mArgumentsWording, &Meaning );
	}

	inline void GetVersionCommandDescription( lcl::meaning_ &Meaning )
	{
		return GetMeaning( mVersionCommandDescription, &Meaning );
	}

	inline void GetLicenseCommandDescription( lcl::meaning_ &Meaning )
	{
		return GetMeaning( mLicenseCommandDescription, &Meaning );
	}

	inline void GetHelpCommandDescription( lcl::meaning_ &Meaning )
	{
		return GetMeaning( mHelpCommandDescription, &Meaning );
	}

	inline void GetMissingCommandErrorMeaning( lcl::meaning_ &Meaning )
	{
		return GetMeaning( mMissingCommandError, &Meaning );
	}

	inline void GetUnknownOptionErrorMeaning(
		const char *Option,
		lcl::meaning_ &Meaning )
	{
		return GetMeaning( mUnknownOptionError, &Meaning, Option );
	}

	inline void GetMissingOptionArgumentErrorMeaning(
		const char *Option,
		lcl::meaning_ &Meaning )
	{
		return GetMeaning( mMissingOptionArgumentError, &Meaning, Option );
	}

	inline void GetUnexpectedOptionErrorMeaning(
		const char *Option,
		lcl::meaning_ &Meaning )
	{
		return GetMeaning( mUnexpectedOptionError, &Meaning, Option );
	}

	inline void GetWrongNumberOfArgumentsErrorMeaning( lcl::meaning_ &Meaning )
	{
		return GetMeaning( mWrongNumberOfArgumentsError, &Meaning );
	}

	//e View mode
	enum view {
		//i All print on one line.
		vOneLine = 10,	// Begins at 10 to detect old boolean usage.
		//i Split in 2 lines.
		vSplit,
		//i Amount of view.
		v_amount
	};

	class item__ {
	public:
		char Short;
		const char *Long;
		id__ Id;
		item__( void )
		{
			Short = CLNARG_NO_SHORT;
			Long = NULL;
			Id = CLNARG_NONE;
		}
	};

	typedef item__ command__;

	typedef bch::E_BUNCH_( command__ ) commands_;
	typedef bch::E_BUNCH( command__ ) commands;

	typedef item__ option__;

	typedef bch::E_BUNCH_( option__ ) options_;
	typedef bch::E_BUNCH( option__ ) options;

	//c Description of the available command line options and arguments.
	class description_
	{
	public:
		commands_ Commands;
		options_ Options;
		struct s {
			commands_::s Commands;
			options_::s Options;
		};
		description_( s &S )
		: Commands( S.Commands ),
		  Options( S.Options )
		{}
		void reset( bool P = true )
		{
			Commands.reset( P );
			Options.reset( P );
		}
		void plug( qAS_ &AS )
		{
			Commands.plug(AS );
			Options.plug( AS );
		}
		description_ &operator =( const description_ &D )
		{
			Commands = D.Commands;
			Options = D.Options;

			return *this;
		}
		//f Initialization
		void Init( void )
		{
			Commands.Init();
			Options.Init();
		}
		//f Add command 'Command'.
		void AddCommand( const command__ &Command )
		{
			Commands.Append( Command );
		}
		//f Add command with short name, 'Short', long name 'Long' and description 'Description' and id 'Id'.
		void AddCommand(
			char Short,
			const char *Long,
			int Id )
		{
			command__ Command;

			if ( Id > CLNARG_ID_MAX )
				qRLmt();
			
			Command.Short = Short;
			Command.Long = Long;
			Command.Id = (id__)Id;

			AddCommand( Command );
		}
		//f Add option 'Option'.
		void AddOption( const option__ &Option )
		{
			Options.Append( Option );
		}
		//f Add option with long name 'Long', short name 'Short', description 'Description', and amount 'Amount' and id 'Id'.
		void AddOption(
			char Short,
			const char *Long,
			int Id )
		{
			option__ Option;

			if ( Id > CLNARG_ID_MAX )
				qRLmt();

			Option.Long = Long;
			Option.Short = Short;
			Option.Id = (id__)Id;

			AddOption( Option );
		}
		//f Return the long and short command label corresponding to 'Id' using 'Separator' to seperate them.
		const char *GetCommandLabels(
			int Id,
			buffer__ &Buffer,
			const char *Separator = CLNARG_GENERAL_SEPARATOR ) const;
		//f Return the option label corresponding to 'Id'.
		const char *GetOptionLabels(
			int Id,
			buffer__ &Buffer,
			const char *Separator = CLNARG_GENERAL_SEPARATOR ) const;
	};

	E_AUTO( description )

	//t An argument.
	typedef str::string_ argument_;
	typedef str::string	argument;

	// Arguments.
	typedef ctn::E_MCONTAINER_( argument_ ) arguments_;
	typedef ctn::E_MCONTAINER( argument_ ) arguments;

	// Option list.
	typedef bch::E_BUNCH_( id__ ) option_list_;
	typedef bch::E_BUNCH( id__ ) option_list;


	//c Class to analyze the command line argument.
	class analyzer___
	{
	private:
		const char **ArgV_;
		int ArgC_;
		const description_ *Description_;
		char Flag_;
		bso::u8__ ArgCount_;
		bso::bool__ GetArgument_( 
			int &i,
			id__ Option,
			argument_ &Argument );
	public:
		analyzer___( void )
		{
			ArgV_ = NULL;
			ArgC_ = 0;
			Description_ = NULL;
			Flag_ = 0;
			ArgCount_ = 0;
		}
		//f Initialization.
		void Init(
			const char *ArgV[],
			int ArgC,
			const description_ &Description,
			char Flag = CLNARG_DEFAULT_FLAG )
		{
			ArgV_ = ArgV;
			ArgC_ = ArgC;
			Description_ = &Description;
			Flag_ = Flag;
			ArgCount_ = 0;
		}
		//f Initialization.
		void Init(
			int ArgC,
			const char *ArgV[],
			const description_ &Description,
			char Flag = CLNARG_DEFAULT_FLAG )
		{
			Init( ArgV, ArgC, Description, Flag );
		}
		//f Return the eventual command.
		id__ GetCommand( void ) const;
		//f Put in 'Options' options. If returned value != 'NULL', then it points of the invalid option/command.
		const char *GetOptions( option_list_ &Options ) const;
		//f Put in 'Argument' the argument of option 'Option'.
		void GetArgument( 
			int Option,
			argument_ &Argument );
		//f Put in 'Arguments' the arguments of option 'Option'.
		void GetArguments( 
			int Option,
			arguments_ &Arguments );
		//f Put in 'Arguments' arguments no associated to an option. A 'GetArguments( Id, ... )' for each option MUST be called before calling this function.
		void GetArguments( arguments_ &Arguments );
		//f Return descriptions.
		const description_ &Description( void )
		{
			return *Description_;
		}
	};
	
	/*f Print the usage text for command in 'Description' identified by 'CommandId'
	using 'Text' and 'View'. If 'Default' at true, the command is the default one. */
	void PrintCommandUsage(
		const description_ &Description,
		int CommandId,
		const char *Text,
		clnarg::view View,
		txf::text_oflow__ &Flow = cio::COut,
		bso::bool__ Default = false );

	inline void PrintCommandUsage(
		const description_ &Description,
		int CommandId,
		const char *Text,
		clnarg::view View,
		bso::bool__ Default,
		txf::text_oflow__ &Flow = cio::COut )
	{
		PrintCommandUsage( Description, CommandId, Text, View, Flow, Default );
	}
		
	/*f Print the usage text for option, with parameter, in 'Description' identified
	by 'OptionId' using 'Text' and 'View'. 'Parameter' is the parameter of the option.	*/
	void PrintOptionUsage(
		const description_ &Description,
		int OptionId,
		const char *Parameter,
		const char *Text,
		clnarg::view View,
		txf::text_oflow__ &Flow = cio::COut );

	/*f Print the usage text for option, without parameter, in 'Description'
	identified by 'OptionId' using 'Text' and 'View'. */
	inline void PrintOptionUsage(
		const description_ &Description,
		int OptionId,
		const char *Text,
		clnarg::view View,
		txf::text_oflow__ &Flow = cio::COut )
	{
		PrintOptionUsage( Description, OptionId, NULL, Text, View, Flow );
	}
}

#endif
