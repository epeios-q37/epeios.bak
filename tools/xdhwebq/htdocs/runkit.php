<?php
/*
	Copyright (C) 2018 Claude SIMON (http://q37.info/contact/).

	This file is part of xdhwebq.

	xdhwebq is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

	xdhwebq is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with xdhwebq. If not, see <http://www.gnu.org/licenses/>.
 */

 /*
	Gets a JS file (for Node.js) from the 'asset' directory on 'q37.info',
	and creates a page which embeds it on RunKit.
*/

$source = <<<EOT
/*
	JavaScript source code for Node.js of the '$NAME$' application.
	It's an example of use of the Atlas toolkit (http://atlastk.org).

	To run this program:
	- go to the bottom of the page,
	- click on the green 'run' button on the right,
	- scroll down as new things are displayed,
	- follow the instructions.

	The program will last only a few tens of seconds,
	due to a RunKit timeout.

	Feel free to modify the source code.
	
	Enjoy !
*/


EOT;
$app = $_GET['app'];

$token = array_key_exists( 'token', $_GET ) ? $_GET['token'] : uniqid();

$source = str_replace( "$NAME$", $app, $source );

// $source .= "\n\nprocess.env.ATK_TOKEN=\"" . $token . "\";\n\n";

$source .= file_get_contents( "http://q37.info/download/assets/" . $app . ".js" );

$source .=<<<EOT


console.log("Scroll down after a little while, as new things will be displayed.");
console.log("Below URL should have been automatically opened in your web browser.");
console.log("If not, pop up windows are probably not allowed by your web browser.");
"NOTA: the program will be stopped after a while due to RunKit timeout."

EOT;

$esource = str_replace("<", "&lt;", $source);
$esource = str_replace(">", "&gt;", $esource);
$esource = str_replace("\\", "\\\\", $esource);
$esource = str_replace(array("\r\n", "\n", "\r"), '\\n', $esource);
$esource = str_replace("'", "\\'", $esource);
$esource = str_replace("\"", '\\"', $esource);

$url = 'http://atlastk.org/xdh.php?_token=' . $token;

$page = '<!DOCTYPE html>
<html>
	<head>
		<script src="https://embed.runkit.com"></script>
	</head>
	<body>
		<div id="my-element"></div>
		<script>
function e()
{
window.scrollTo(0,document.body.scrollHeight);
setTimeout( () => {
	let url = "' . $url . '";
	alert("The \'' . $app . '\' application now runs on RunKit, and will be automatically opened in your web browser.\nIf not, follow the instructions at the bottom of the RunKit page.\n\nRemember: the application will be stopped after a few tens of seconds due to RunKit timeout!");
	window.open( url );
}, 2000 );
}
var source = "' . $esource . '";
var notebook = RunKit.createNotebook({
// the parent element for the new notebook
element: document.getElementById("my-element"),
// specify the source of the notebook
source: source.replace(/&gt;/g, ">").replace(/&lt;/g, "<"),
onLoad: (notebook) => {notebook.evaluate()},
onEvaluate: e,
env: [ "ATK_TOKEN=' . $token . '" ]
});
		</script>
	</body>
</html>';


// Microsoft Edge does have problems when the source is too long !
if ( preg_match('/Edge/i', $_SERVER['HTTP_USER_AGENT']) == 1 )
	$page = '<h3>Applications based on the <a href="http://atlastk.org"><em>Atlas</em> toolkit</a> will work on all modern web browser, including Microsoft Edge. But, due to limitations (bugs?) of Microsoft Edge, it can not be used to launch the online demos. You have to switch to another web navigator for this.</h3>';


$page2 = '
<!DOCTYPE html>
<html>
	<head>
		<script>
var url = "data:text/html;base64,' . base64_encode( $page ) . '";
alert( url );
window.open( url );
		</script>
	</head>
	<body>
	</body>
</html>
';

$page3 = '
<!DOCTYPE html>
<html>
  <head>
<script src="https://embed.runkit.com" data-element-id="script"></script>
<script>
  function expandFrame() {
    window.open(document.getElementById( "script").firstChild.src, "_top");
  }
</script>
</head>
<body>
<div id="script">
' . htmlspecialchars( $source ) . '
</div>
</body>
</html>
';

$page4 = '<!DOCTYPE html>
<html>
	<head>
		<script src="https://embed.runkit.com"></script>
	</head>
	<body>
		<div id="my-element"></div>
		<script>
		var source = "' . $esource . '";
		source = source.replace(/&gt;/g, ">").replace(/&lt;/g, "<");
		alert( source );
		var notebook = RunKit.createNotebook({
// the parent element for the new notebook
element: document.getElementById("my-element"),
// specify the source of the notebook
source: source,
onLoad: (notebook) => {notebook.evaluate()},
});
		</script>
	</body>
</html>';

echo $page;

?>
