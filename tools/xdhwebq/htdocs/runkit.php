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
	This is a JavaScript source code for Node.js.
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


if ( array_key_exists( 'token', $_GET ) ) {
	$source .= "\n\nprocess.env.ATK_TOKEN=\"" . $_GET['token'] . "\";\n\n";
}

$source .= file_get_contents( "http://q37.info/download/assets/" . $_GET['app'] . ".js" );

$source .=<<<EOT


"NOTA: the program will be stopped after a while due to RunKit timeout."
EOT;

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
<body onload="expandFrame()">
<div id="script">
' . htmlspecialchars( $source ) . '
</div>
</body>
</html>
'
?>
