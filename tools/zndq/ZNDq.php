<?php

zndq_Init();

class ZNDq {
	static public function wrapperInfo()
	{
		return zndq_wrapper_info(); 
	}
	static public function componentInfo()
	{
		return zndq_component_info(); 
	}
}
?>