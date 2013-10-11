/* DON'T MODIFY : computer-generated file ! */
/*
	API from : esketchbkd (Version) (Build : Aug 30 2013 12:12:36 MSVC_IA-32)

	This file was generated using getbkdapi 20130826 (Build Aug 26 2013 16:45:59 MSVC_IA-32)
*/

#ifndef ESKETCH__INC
# define ESKETCH__INC

# define ESKETCH_TARGET "esketch"
# define ESKETCH_API_VERSION "1"

# include "fblfrd.h"
# include "cpe.h"

namespace esketch {

	class statics___
	{
	private:
		fblfrd::object__ _ID;
		fblfrd::frontend___ *_Frontend;
		fblfrd::command__ _Commands[1];
	public:
		void reset( bso::bool__ = true )
		{
			_Frontend = NULL;
			_ID = FBLFRD_UNDEFINED_OBJECT;
		}
		E_CVDTOR( statics___ )
		void Init( fblfrd::frontend___ &Frontend )
		{
			fblfrd::commands_details CommandsDetails;
			fblfrd::command_detail CommandDetail;
			fblfrd::id16s Commands;

			fblfrd::id8_t__ Parameters[] = {
				0, 
			};

			_Frontend = &Frontend;

			_ID = FBLFRD_MASTER_OBJECT;
			CommandsDetails.Init();

			CommandDetail.Init();
			CommandDetail.Name = "SKTTest";;
			CommandDetail.Casts.Append( Parameters + 0, 1 );
			CommandsDetails.Append( CommandDetail );


			Commands.Init();
			_Frontend->GetCommands( FBLFRD_MASTER_TYPE, CommandsDetails, Commands );
			Commands.Recall( 0, 1, _Commands );
		}
		fblovl::reply__ SKTTest( void ) const
		{
			Frontend().PushHeader( _ID, Commands()[0] );

			Frontend().EndOfInParameters();


			return Frontend().Handle();
		}
		fblfrd::frontend___ &Frontend( void ) const
		{
			return *_Frontend;
		}
		const fblfrd::command__ *Commands( void ) const
		{
			return _Commands;
		}
	};

	class skt_myobject_common__
	{
	private:
		fblfrd::id16__ _ID;
		fblfrd::frontend___ *_Frontend;
		fblfrd::command__ _Commands[1];
	public:
		void reset( bso::bool__ = true )
		{
			_Frontend = NULL;
			_ID = FBLFRD_UNDEFINED_OBJECT;
		}
		E_CVDTOR( skt_myobject_common__ )
		void Init( fblfrd::frontend___ &Frontend )
		{
			fblfrd::commands_details CommandsDetails;
			fblfrd::command_detail CommandDetail;
			fblfrd::id16s Commands;

			fblfrd::id8_t__ Parameters[] = {
				0, 
			};

			_Frontend = &Frontend;

			_ID = _Frontend->GetType( str::string( "myobject" ) );

			CommandsDetails.Init();

			CommandDetail.Init();
			CommandDetail.Name = "Test";;
			CommandDetail.Casts.Append( Parameters + 0, 1 );
			CommandsDetails.Append( CommandDetail );


			Commands.Init();
			_Frontend->GetCommands( _ID, CommandsDetails, Commands );
			Commands.Recall( 0, 1, _Commands );
		}
		fblfrd::object__ GetNewObject( void )
		{
			return _Frontend->GetNewObject( _ID );
		}
		void RemoveObject( fblfrd::object__ Object )
		{
			_Frontend->RemoveObject( Object );
		}
		fblfrd::frontend___ &Frontend( void ) const
		{
			return *_Frontend;
		}
		const fblfrd::command__ *Commands( void ) const
		{
			return _Commands;
		}
	};

	class skt_myobject___
	{
	private:
		fblfrd::object__ _ID;
		skt_myobject_common__ *Common_;
	public:
		void reset( bso::bool__ P = true )
		{
			if ( P == true )
				if ( ( Common_ != NULL ) && ( _ID != FBLFRD_UNDEFINED_OBJECT ) )
					Common_->RemoveObject( _ID );

			Common_ = NULL;
			_ID = FBLFRD_UNDEFINED_OBJECT;
		}
		skt_myobject___( void )
		{
			reset( false );
		}
		~skt_myobject___( void )
		{
			reset();
		}
		fblfrd::object__ ID( void ) const
		{
			return _ID;
		}
		void Init( skt_myobject_common__ &Common )
		{
			Common_ = &Common;

			_ID = Common_->GetNewObject();
		}
		fblovl::reply__ Test( void ) const
		{
			Common_->Frontend().PushHeader( _ID, Common_->Commands()[0] );

			Common_->Frontend().EndOfInParameters();


			return Common_->Frontend().Handle();
		}
	};

}

#endif //#ifndef esketch__INC
