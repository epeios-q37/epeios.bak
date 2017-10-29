<?php
require "XPPq.php";

echo ZNDq::componentInfo() . "\n" . ZNDq::wrapperInfo() . "\n";

echo XPPq::returnArgument( "Argument from the PHP file !") . "\n";

$XML = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n" .
"<SomeTag xmlns:xpp=\"http://q37.info/ns/xpp/\" AnAttribute=\"SomeAttributeValue\">\n" .
" <SomeOtherTag AnotherAttribute=\"AnotherAttributeValue\">TagValue</SomeOtherTag>\n" .
" <xpp:define name=\"SomeMacro\">\n" .
"  <xpp:bloc>Some macro content from string !</xpp:bloc>\n" .
" </xpp:define>\n" .
" <YetAnotherTag YetAnotherAttribute=\"YetAnotherAttributeValue\">\n" .
"  <xpp:expand select=\"SomeMacro\"/>\n" .
" </YetAnotherTag>\n" .
"</SomeTag>\n"
 ;

	function getStream()
	{
	if ( false )
	 return fopen( 'data://text/plain,' . $GLOBALS["XML"], 'r' );
	else {
		$GLOBALS["fxml"] = fopen( 'data://text/plain,' . $GLOBALS["XML"], 'r' );
	//	$fxml = fopen('Project.xml', 'r' );
		 return fopen( "xpp://fxml", 'r' );
		}
	}

	$stream = getStream();

	while ( !feof( $stream ) ) {
    echo fgets( $stream );
}

echo "\n\n";

/*
function f( &$text )
{
	$text = $text . " the world";
}

 $test= "Hello";

	echo $test . " !" . "\n";

	f( $test );

	echo $test . " !" . "\n";
	*/

	$parser = XPPq::parserNew( getStream() );

	$token = XPPq::parserParse( $parser );

	while ( $token != XPPqParser::PROCESSED ) {
		switch ( $token ) {
		case XPPqParser::START_TAG :
			echo "Start tag: '" . XPPqParser::tagName( $parser ) . "'\n";
			break;
		case XPPqParser::ATTRIBUTE :
			echo  "Attribute: '" . XPPqParser::attributeName( $parser ) . "' = '" . data.value . "'\n";
			break;
		case XPPqParser::VALUE :
			echo  "Value:     '" . trim( XPPqParser::Value( $parser ) ) . "'\n";
			break;
		case XPPqParser::END_TAG :
			echo  "End tag:   '" . XPPqParser::tagName( $parser ) . "'\n";
			break;
		default:
			die( "Unknown token !!!" );
		}
		
	 $token = XPPq::parserParse( $parser );
	}

	XPPqParser::delete( $parser );
?>