<?php

$ZNDq_affix="xppq";

// Begin of generic part.

if ( getenv("EPEIOS_SRC") === false )
 $path = realpath( dirname( __FILE__ ) ) . '/';
else {
 switch ( strtoupper( substr( php_uname ( 's' ), 0, 3 ) ) ) {
	case "WIN":
  $path = "h:\\bin\\";
		break;
	case "LIN":
  $path = "/home/csimon/bin/";
		break;
	case "DAR":
	 $path = "/Users/csimon/bin/";
		break;
	default:
	 echo "Unknown OS !!!\n";
		break;
	}
}
	
ZNDq_init();
ZNDq_register( str_replace( ' ', '\ ', str_replace( '\\', '/', $path ) ) . $ZNDq_affix . "znd" );

class ZNDq {
	static public function wrapperInfo()
	{
		return ZNDq_wrapperInfo(); 
	}
	static public function componentInfo()
	{
		return ZNDq_componentInfo(); 
	}
	static protected function register( $arguments )
	{
		return ZNDq_register( $arguments );
	}
}
// End of generic part.

class XPPq extends ZNDq {
	static public function returnArgument( $argument )
	{
	 return ZNDq_wrapper( 0, $argument );
	}
	static public function new( $stream )
	{
	 return ZNDq_wrapper( 1, $stream );
	}
	static public function read( $stream, $count )
	{
	 return ZNDq_wrapper( 2, $stream, $count );
	}
	static public function eof( $stream )
	{
	 return ZNDq_wrapper( 3, $stream );
	}
}

class XMLPreprocessor {
				var $stream;
    function stream_open($path, $mode, $options, &$opened_path)
    {
        $url = parse_url( $path );
        $this->stream = XPPq::new($GLOBALS[$url["host"]]);

								return true;
    }

    function stream_read($count)
    {
					    return XPPq::read( $this->stream, $count );
    }

    function stream_eof()
    {
					    return XPPq::eof( $this->stream );
    }
}
?>