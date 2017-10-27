<?php

$ZNDq_affix="esketch";

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

class eSketch extends ZNDq {
	static public function returnArgument( $argument )
	{
	 return ZNDq_wrapper( 0, $argument );
	}
}
?>