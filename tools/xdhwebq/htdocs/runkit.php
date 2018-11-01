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

$source .= "\n\nprocess.env.ATK_TOKEN=\"" . $token . "\";\n\n";

$source .= file_get_contents( "http://q37.info/download/assets/" . $app . ".js" );

$source .=<<<EOT


console.log("Scroll down after a little while, as new things will be displayed.");
"NOTA: the program will be stopped after a while due to RunKit timeout."

EOT;

$source = str_replace("<", "&lt;", $source);
$source = str_replace(">", "&gt;", $source);
$source = str_replace("\\", "\\\\", $source);
$source = str_replace(array("\r\n", "\n", "\r"), '\\n', $source);
$source = str_replace("'", "\\'", $source);
$source = str_replace("\"", '\\"', $source);

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
	alert("The \'' . $app . '\' application now runs on RunKit. The URL to access this application is:\n\n" + url +"\n\nThis URL should now be automatically opened in your web browser. If not, follow the instructions at the bottom of the page.\n\nRemember: the application will be stopped after a few tens of seconds due to RunKit timeout!");
	window.open( url );
}, 2000 );
}
var source = "' . $source . '";
var notebook = RunKit.createNotebook({
// the parent element for the new notebook
element: document.getElementById("my-element"),
// specify the source of the notebook
source: source.replace(/&gt;/g, ">").replace(/&lt;/g, "<"),
onLoad: (notebook) => {notebook.evaluate()},
onEvaluate: e
});
		</script>
	</body>
</html>';

echo $page;

/*
echo '
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
'
*/
/*
echo '
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
<body onload_="expandFrame()">
<div id="script">
' . htmlspecialchars( $source ) . '
</div>
</body>
</html>
'
*/
?>
