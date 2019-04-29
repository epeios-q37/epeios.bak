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
Gets a JS file (for Node.js) from the 'atlas-node' repository on GitHub,
and creates a page which embeds it on RunKit. Also runs it.
 */

$source = <<<EOT
/*
	JavaScript source code for Node.js of the '$NAME$' application.
	It's an example of use of the Atlas toolkit (https://atlastk.org).

	The source code of this application for other languages
	(Java, Node.js, RUBY...), and links to live demonstrations of
	other applications can also be found on https://atlastk.org.

	To run this program:
	- go to the bottom of the page,
	- click on the green 'run' button on the right,
	- scroll down as new things are displayed,
	- follow the instructions.

	The program will last only a few tens of seconds,
	due to a RunKit timeout.

	You can modify the source code for your own testings.

	Enjoy !
*/

EOT;

$app = $_GET['app'];

$token = array_key_exists('token', $_GET) ? $_GET['token'] : uniqid();

$source = str_replace("$NAME$", $app, $source);

$source .= file_get_contents("https://raw.githubusercontent.com/epeios-q37/atlas-node/master/" . $app . "/" . $app . ".js");

// $source .= file_get_contents("http://q37.info/download/assets/" . $app . ".js");

$source .= <<<EOT

console.log("Scroll down after a little while, as new things will be displayed.");
console.log("Below URL should have been automatically opened in your web browser.");
"NOTA: the program will be stopped after a while due to RunKit timeout."

EOT;

$esource = str_replace("<", "&lt;", $source);
$esource = str_replace(">", "&gt;", $esource);
$esource = str_replace("\\", "\\\\", $esource);
$esource = str_replace(array("\r\n", "\n", "\r"), '\\n', $esource);
$esource = str_replace("'", "\\'", $esource);
$esource = str_replace("\"", '\\"', $esource);

 $url = 'http://localhost:8080/xdh.php?_token=' . $token;

// $url = 'http://atlastk.org/xdh.php?_token=' . $token;

$page = '<!DOCTYPE html>
<html>
	<head>
		<script src="https://embed.runkit.com"></script>
	</head>
	<body>
		<div id="my-element"></div>
		<script>
function onLoad( notebook )
{
 notebook.setSource(source.replace(/&gt;/g, ">").replace(/&lt;/g, "<"))

	if ( confirm("The \'' . $app . '\' application will now be launched on RunKit and be automatically opened in your web browser.\n\nNOTA: the application will stop working after a few tens of seconds due to RunKit timeout.\nYou can then simply go back to the RunKit page, modify the source code if you like, and relaunch the application.\n\nClicking on Cancel gives you access to the source code.\n\nFor live demonstrations of other examples, see https://atlastk.org.\n\n") )
		notebook.evaluate();
	else
		window.scrollTo(0,0);

}
function onEvaluate()
{
	window.scrollTo(0,document.body.scrollHeight);
    let url = "' . $url . '";
	setTimeout( () => {
		window.open( url, "_self" );
	}, 5000 );
}
var source = "' . $esource . '";
var notebook = RunKit.createNotebook({
// the parent element for the new notebook
element: document.getElementById("my-element"),
// specify the source of the notebook
// source: source.replace(/&gt;/g, ">").replace(/&lt;/g, "<"),	// Does not work if content too long !
onLoad: onLoad,
onEvaluate: onEvaluate,
env: [ "ATK_TOKEN=' . $token . '" ]
});
		</script>
	</body>
</html>';

// Microsoft Edge does have problems when the source is too long !
// NO MORE ACCURATE : Works when the source code is loaded with 'setCode()'.
/*
if (preg_match('/Edge/i', $_SERVER['HTTP_USER_AGENT']) == 1) {
 $page = '<h3>Applications based on the <a href="http://atlastk.org"><em>Atlas</em> toolkit</a> will work on all modern web browser, including Microsoft Edge. But, due to limitations (bugs?) of Microsoft Edge, it can not be used to launch the online demos. You have to switch to another web navigator for this.</h3>';
}
*/
$page2 = '
<!DOCTYPE html>
<html>
	<head>
		<script>
var url = "data:text/html;base64,' . base64_encode($page) . '";
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
' . htmlspecialchars($source) . '
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
