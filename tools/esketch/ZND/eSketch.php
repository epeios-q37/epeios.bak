<?php

$ZNDq_affix="esketch";	// We are not interested in a extension;

// Begin of generic part.
ZNDq_init();
ZNDq_register( "./lib" . $ZNDq_affix . "znd.so" );
echo "Yo !";

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