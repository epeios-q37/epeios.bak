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
			<!-- Below both scripts are for PROD environment. -->
			<script src="faas/xdhtml_20200821.js"></script>
			<script src="faas/xdhwebq_20200608.js"></script>
			<script src="faas/qrcode.min.js"></script>
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
				function ignition(token) {
					connect(token);
				}
			</script>
		</head>
		<!--body id="Root" data-xdh-onevents="(keypress|About|SC+a)(keypress|Q37Refresh|SC+r)"-->
		<body onload="new QRCode(\'qrcode\', {width:125, height:125, correctLevel: QRCode.CorrectLevel.L}).makeCode(window.location.href);ignition(\'' . $token . '\');">
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
				<div style="display:table; margin: 5px auto auto auto;">
					<hr/>
					<span style="display: table; margin: 10px auto 5px auto;">Click or scan for another instance:</span>
					<div style="display: flex; justify-content: space-around;">
 						<a href="#" target="_blank">
 						   <div id="qrcode"></div>
						</a>
					</div>
					<div style="display: table; padding: 10px;">
						<hr/>
						<div style="display: table; padding: 10px;">
							<div style="display: table; margin: auto; border: 1px solid; padding: 10px; box-shadow: 5px 5px; text-align: center;">
								<span style="text-shadow: 1px 1px;">
									<span>Powered by the <a href="http://atlastk.org/" target="_blank"><span style="font-style: italic;">Atlas</span> toolkit</a></span>
								</span>
							</div>
						</div>
					</div>
				</div>
			 </div>
		</body>
	</html>')

?>
