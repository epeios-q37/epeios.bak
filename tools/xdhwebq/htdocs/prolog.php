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
. $head .
  '<script>handleQuery("' . $cgi . '?_token=' . $token . '&_language=' . $language . '&_action=' . $action . '&_cont=' . '" )</script>
		</head>
		<!--body id="Root" data-xdh-onevents="(keypress|About|SC+a)(keypress|Q37Refresh|SC+r)"-->
		<body>
			<noscript>
				<div style="display: table; margin: 50px auto auto auto;">
					<fieldset>
						<h3><i>JavaScript</i> is disabled! You have to enable <i>JavaScript</i> for this application to work!</h3>
					</fieldset>
				</div>
			</noscript>
			<input type="hidden" id="LoginUserID" value="' . $UserID . '"/>
			<input type="hidden" id="LoginPassword" value="' . $Password . '"/>
		</body>
	</html>'
?>