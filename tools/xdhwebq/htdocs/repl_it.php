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
$turl = substr($url,0,1+strpos($url,"?")) . "…";	// truncated URL.

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
		<div style="display:table; margin: 5px auto 5px auto;">
			<div style="display: flex; justify-content: space-around;">
				<fieldset
					style="display: block; margin-left: auto; margin-right: auto; border-radius: 15px; border-style: none; padding: 10px; box-shadow: 1px 1px 12px #555;">
					<a target="_blank" href="%url%" alt="%url%">
						<div id="qrcode"></div>
					</a>
				</fieldset>
			</div>
			<span style="display: table; margin: 15px auto 10px auto;font-style: oblique;font-size: large;font-weight: bold; text-align: center; width: 90%;">
				Your application is now running at the 
				URL displayed in the terminal (<a target="_blank" href="%url%" alt="%url%">%turl%</a>). By clicking the above QR code, 
				this URL will be opened	in a new tab, or window, depending 
				on your settings. <br />
				To open the application on your smartphone
				or tablet, scan the QR code with your device. Enjoy!
			</span>
			<!--span
				style="display: table; margin: 15px auto 10px auto;font-style: oblique;font-size: large;font-weight: bold; text-align: center;">
				If you stop then run again, you may<br />
				have to click the top left refresh button.<br />
				See <a href="https://q37.info/s/zbgfjtp9" target="_blank">https://q37.info/s/zbgfjtp9</a> for more details.
			</span-->
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

echo str_replace("%turl%",$turl,str_replace("%url%", $url, $page1));
?>
