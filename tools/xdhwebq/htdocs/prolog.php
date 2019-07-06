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

$out = '
	<!DOCTYPE html>
		<html>
			<head>
			<!-- If modified, report modification in "xdhcefq.html" from "xdhcefq" -->
			<!-- BEGIN -->
			<meta http-equiv="Content-Type" content="text/html; charset=utf-8"/>
			<meta http-equiv="Cache-Control" content="no-cache"/>
			<script src="xdh/xdhtml.js"></script>
			<script src="xdh/xdhwebq.js"></script>
			<script src="xdh/qrcode.min.js"></script>
			<style id="XDHStyle"></style>'
. $head . '
			<script>handleQuery("'
. $cgi . '?_token=' . $token . '&_language=' . $language . '&_action=' . $action . '&_cont=" )</script>
		</head>
		<!--body id="Root" data-xdh-onevents="(keypress|About|SC+a)(keypress|Q37Refresh|SC+r)"-->
		<body onload="new QRCode(\'qrcode\', {width:125, height:125, correctLevel: QRCode.CorrectLevel.L}).makeCode(window.location.href)">
			<span>
				<noscript>
					<div style="display: table; margin: 50px auto auto auto;">
						<fieldset>
							<h3><i>JavaScript</i> is disabled! You have to enable <i>JavaScript</i> for this application to work!</h3>
						</fieldset>
					</div>
				</noscript>
				<input type="hidden" id="LoginUserID" value="' . $UserID . '"/>
				<input type="hidden" id="LoginPassword" value="' . $Password . '"/>
			</span>
			<span>
				<div style="display:table; margin: 5px auto auto auto;">
					<hr/>
					<span style="display: table; margin: 10px auto 5px auto;font-style: oblique;">For another instance, click or scan:</span>
					<div style="display: flex; justify-content: space-around;">
 						<a href="#" target="_blank">
 						   <div id="qrcode"></div>
						</a>
					</div>
						<span style="display: table; margin: auto;font-style: oblique;margin: 5px auto auto auto;">Generated with <a href="https://davidshimjs.github.io/qrcodejs/" target="_blank">qrcode.js</a></span>
						<div style="display: table; margin: 15px auto 10px auto; border: 1px solid; padding: 10px;box-shadow: 5px 5px; text-align: center;">
						<span style="text-shadow: 1px 1px;">
							<span>Powered by the <span style="font-style: italic;">Atlas</span> toolkit (<a href="http://atlastk.org/" target="_blank">http://atlastk.org/</a>)</span>
							<br/>
							<span>&gt;&gt;&gt;&nbsp;<a href="https://q37.info/s/sssznrb4" target="_blank">Live demonstrations</a>&nbsp;&lt;&lt;&lt;</span>
						</span>
					 </div>
				 </div>
			</span>
		</body>
	</html>'
?>