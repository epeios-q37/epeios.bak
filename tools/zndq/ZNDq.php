<?php

class ZNDqDecl {
	protected $affix="";	// We are not interested in a extension;
}

// Begin of generic part.
ZNDq_Init();

class ZNDq extends ZNDqDecl {
	static public function wrapperInfo()
	{
		return ZNDq_wrapperInfo(); 
	}
	static public function componentInfo()
	{
		return ZNDq_componentInfo(); 
	}
}
// End of generic part.
?>