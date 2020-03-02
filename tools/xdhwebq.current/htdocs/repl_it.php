<?php
/*
Copyright (C) 2019 Claude SIMON (http://q37.info/contact/).

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
Creates a page returned by the http server launched in Repl.it.
 */

$url = $_GET['url'];

$page1 = <<<EOT
<!DOCTYPE html>
<html>
	<head>
		<script src="https://atlastk.org/xdh/qrcode.min.js"></script>
		<script>
function genQRCode(url) {
	new QRCode("qrcode", {width:125, height:125, correctLevel: QRCode.CorrectLevel.L}).makeCode("%url%");
}
		</script>
	</head>
	<body onload="genQRCode('%url%')">
		<div style="display:table; margin: 10px auto 5px auto;">
			<span style="display: table; margin: 15px auto 10px auto;font-style: oblique;font-size: large;font-weight: bold;">Click or scan this QR code:</span>
			<div style="display: flex; justify-content: space-around;">
				<a target="_blank" href="%url%" alt="%url%">
 					<div id="qrcode"></div>
				</a>
			</div>
			<!--
			<span style="display: table; margin: 15px auto auto auto;font-style: oblique;font-size: large;font-weight: bold;">If it does not work, fork and/or restart!</center></span></span>
			<span style="display: table; margin: 15px auto auto auto;font-style: oblique;font-size: large;font-weight: bold;">See <a href="https://q37.info/s/zbgfjtp9" target="_blank">https://q37.info/s/zbgfjtp9</a> for more details.</span>
			-->
		</div>
	</body>
</html>
EOT;

// Blocked by popup blocker.
$page2 = <<<EOT
<!DOCTYPE html>
<html>
	<head>
	</head>
	<body onload="window.open('%url%', '_blank')"></body>
</html>
EOT;

echo str_replace("%url%", $url, $page1);
?>
