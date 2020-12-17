<?php
/*
Copyright (C) 2015 Claude SIMON (http://q37.info/contact/).

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

$out = str_replace(array("\r", "\n", "\t"), '','
	<!DOCTYPE html>
		<html>
			<head>
			<!-- If modified, report modification in "xdhcefq.html" from "xdhcefq" -->
			<!-- BEGIN -->
			<meta http-equiv="Content-Type" content="text/html; charset=utf-8"/>
			<meta http-equiv="Cache-Control" content="no-cache"/>
			<!--Below both scripts are for DEV environment. -->
			<script src="faas/xdhtml.js"></script>
			<script src="faas/xdhwebq.js"></script>
			<script src="faas/qrcode.min.js"></script><!-- For the \'AttachQRCode\' primitive. -->
			<!-- Below both scripts are for PROD environment. -->
			<script src="faas/xdhtml_20201001.js"></script>
			<script src="faas/xdhwebq_20200608.js"></script>
			<style id="XDHStyle">
				.xdh_style {
					display: table;
					margin: 20px auto auto auto;
				}
			</style>'
. $head . '
			<!--script>handleQuery("'
. $cgi . '?_token=' . $token . '&_language=' . $language . '&_action=' . $action . '&_cont=" )</script-->
			<script>
				function adjustQRCodeIFrame()
				{
					let iframe = document.body.lastChild.firstChild;
					iframe.style.height = iframe.contentWindow.document.body.scrollHeight + \'px\';
				}
				function ignition(token,id,qrcodeOnly) {
						document.body.lastChild.firstChild.src = "FaaSFooter.php?url=" + window.location.href;
						if (!qrcodeOnly) connect(token,id);
				}
			</script>
		</head>
		<!--body id="Root" data-xdh-onevents="(keypress|About|SC+a)(keypress|Q37Refresh|SC+r)"-->
		<body onload="ignition(\'' . $token . '\',\'' . $id . '\',' . $qrcodeOnly . ');">
			<div class="xdh_style">
				<noscript>
					<div style="display: table; margin: 50px auto auto auto;">
						<fieldset>
							<h3><i>JavaScript</i> is disabled! You have to enable <i>JavaScript</i> for this application to work!</h3>
						</fieldset>
					</div>
				</noscript>
				<input type="hidden" id="LoginUserID" value="' . $UserID . '"/>
				<input type="hidden" id="LoginPassword" value="' . $Password . '"/>
			</div>
			<div' . $additional . '>
				<iframe style="border: none; width: 100%;" onload="adjustQRCodeIFrame();"/>
			</div>
		</body>
	</html>')
?>
