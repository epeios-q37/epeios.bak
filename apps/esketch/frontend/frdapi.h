/* DON'T MODIFY : computer-generated file ! */
/*
	API from : esketchbkd V`echo toto` Build May 25 2016 08:48:53 - Win32;MSC 1800;IA-32

	This file was generated using barq 20160313 (Build May 25 2016 10:23:05 Win32;MSC 1800;IA-32)
*/

#ifndef ESKETCH__INC
# define ESKETCH__INC

# define ESKETCH_TARGET "esketch"
# define ESKETCH_API_VERSION "1"

# include "fblfrd.h"
# include "cpe.h"

namespace esketch {

	namespace {
		typedef fblfrd::frontend_depot__ _frontend_depot__;
	}

	class fStatics
	: public _frontend_depot__
	{
	private:
		fblfrd::object__ _ID;
		fblfrd::command__ _Commands[3];
	public:
		void reset( bso::bool__ P = true )
		{
			_frontend_depot__::reset( P );
			_ID = FBLFRD_UNDEFINED_OBJECT;
		}
		E_CVDTOR( fStatics )
		void Init( fblfrd::frontend___ &Frontend )
		{
			fblfrd::commands_details CommandsDetails;
			fblfrd::command_detail CommandDetail;
			fblfrd::id16s Commands;

			fblfrd::id8__ Parameters[] = {
				25, 0, 
				25, 0, 
				0, 
			};

			_frontend_depot__::Init( Frontend );

			_ID = FBLFRD_MASTER_OBJECT;
			CommandsDetails.Init();

			CommandDetail.Init();
			CommandDetail.Name = "LoadSetupOfId";;
			CommandDetail.Casts.Append( Parameters + 0, 2 );
			CommandsDetails.Append( CommandDetail );

			CommandDetail.Init();
			CommandDetail.Name = "LoadSetupContent";;
			CommandDetail.Casts.Append( Parameters + 2, 2 );
			CommandsDetails.Append( CommandDetail );

			CommandDetail.Init();
			CommandDetail.Name = "SKTTest";;
			CommandDetail.Casts.Append( Parameters + 4, 1 );
			CommandsDetails.Append( CommandDetail );


			Commands.Init();
			this->Frontend().GetCommands( FBLFRD_MASTER_TYPE, CommandsDetails, Commands );
			Commands.Recall( 0, 3, _Commands );
		}
		fblovl::reply__ LoadSetupOfId( 
			const fblfrd::string_ &In1 ) const
		{
			Frontend().PushHeader( _ID, Commands()[0] );
			Frontend().StringIn( In1 );

			Frontend().EndOfInParameters();


			return Frontend().Handle();
		}
		fblovl::reply__ LoadSetupContent( 
			const fblfrd::string_ &In1 ) const
		{
			Frontend().PushHeader( _ID, Commands()[1] );
			Frontend().StringIn( In1 );

			Frontend().EndOfInParameters();


			return Frontend().Handle();
		}
		fblovl::reply__ SKTTest( void ) const
		{
			Frontend().PushHeader( _ID, Commands()[2] );

			Frontend().EndOfInParameters();


			return Frontend().Handle();
		}
		const fblfrd::command__ *Commands( void ) const
		{
			return _Commands;
		}
	};

	class fSKTMyObjectCommon
	: public _frontend_depot__
	{
	private:
		fblfrd::id16__ _ID;
		fblfrd::command__ _Commands[2];
	public:
		void reset( bso::bool__ P = true )
		{
			_frontend_depot__::reset( P );
			_ID = FBLFRD_UNDEFINED_OBJECT;
		}
		E_CVDTOR( fSKTMyObjectCommon )
		void Init( fblfrd::frontend___ &Frontend )
		{
			fblfrd::commands_details CommandsDetails;
			fblfrd::command_detail CommandDetail;
			fblfrd::id16s Commands;

			fblfrd::id8__ Parameters[] = {
				25, 0, 25, 
				0, 
			};

			_frontend_depot__::Init( Frontend );

			_ID = this->Frontend().GetType( str::string( "MyObject" ) );

			CommandsDetails.Init();

			CommandDetail.Init();
			CommandDetail.Name = "ToUC";;
			CommandDetail.Casts.Append( Parameters + 0, 3 );
			CommandsDetails.Append( CommandDetail );

			CommandDetail.Init();
			CommandDetail.Name = "Test";;
			CommandDetail.Casts.Append( Parameters + 3, 1 );
			CommandsDetails.Append( CommandDetail );


			Commands.Init();
			this->Frontend().GetCommands( _ID, CommandsDetails, Commands );
			Commands.Recall( 0, 2, _Commands );
		}
		fblfrd::object__ GetNewObject( void )
		{
			return Frontend().GetNewObject( _ID );
		}
		void RemoveObject( fblfrd::object__ Object )
		{
			Frontend().RemoveObject( Object );
		}
		const fblfrd::command__ *Commands( void ) const
		{
			return _Commands;
		}
	};

	class rSKTMyObject
	{
	private:
		fblfrd::object__ _ID;
		fSKTMyObjectCommon *Common_;
	public:
		void reset( bso::bool__ P = true )
		{
			if ( P == true )
				if ( ( Common_ != NULL ) && ( _ID != FBLFRD_UNDEFINED_OBJECT ) && Common_->Frontend().IsConnected() )
					Common_->RemoveObject( _ID );

			Common_ = NULL;
			_ID = FBLFRD_UNDEFINED_OBJECT;
		}
		rSKTMyObject( void )
		{
			reset( false );
		}
		~rSKTMyObject( void )
		{
			reset();
		}
		fblfrd::object__ ID( void ) const
		{
			return _ID;
		}
		void Init( fSKTMyObjectCommon &Common )
		{
			Common_ = &Common;

			_ID = Common_->GetNewObject();
		}
		fblovl::reply__ ToUC( 
			const fblfrd::string_ &In1,
			fblfrd::string_ &Out1 ) const
		{
			Common_->Frontend().PushHeader( _ID, Common_->Commands()[0] );
			Common_->Frontend().StringIn( In1 );

			Common_->Frontend().EndOfInParameters();

			Common_->Frontend().StringOut( Out1 );

			return Common_->Frontend().Handle();
		}
		fblovl::reply__ Test( void ) const
		{
			Common_->Frontend().PushHeader( _ID, Common_->Commands()[1] );

			Common_->Frontend().EndOfInParameters();


			return Common_->Frontend().Handle();
		}
	};

}

#endif //#ifndef esketch__INC
