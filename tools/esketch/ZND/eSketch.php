<?php

$zndq_affix="esketch";

/***** Begin of generic part. ****/
if ( getenv("EPEIOS_SRC") === false )
 $zndq_path = realpath( dirname( __FILE__ ) ) . '/';
else {
 switch ( strtoupper( substr( php_uname ( 's' ), 0, 3 ) ) ) {
	case "WIN":
  $zndq_path = "h:\\bin\\";
		break;
	case "LIN":
  $zndq_path = "/home/csimon/bin/";
		break;
	case "DAR":
	 $zndq_path = "/Users/csimon/bin/";
		break;
	default:
	 echo "Unknown OS !!!\n";
		break;
	}
}
	
zndq_init();
zndq_register( str_replace( ' ', '\ ', str_replace( '\\', '/', $zndq_path ) ) . $zndq_affix . "znd" );

class ZNDq {
	static public function wrapperInfo()
	{
		return zndq_wrapper_info(); 
	}
	static public function componentInfo()
	{
		return zndq_component_info(); 
	}
	static protected function register( $arguments )
	{
		return zndq_register( $arguments );
	}
}
/**** End of generic part. ****/

class eSketch extends ZNDq {
	static public function returnArgument( $argument )
	{
	 return ZNDq_wrapper( 0, $argument );
	}
}
?>