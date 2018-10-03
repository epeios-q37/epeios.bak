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
			<meta charset="UTF-8" />
			<script src="xdh/xdhtml.js"></script>
			<script src="xdh/xdhwebq.js"></script>'
			. $head .
			'<script>handleQuery("xdh.php?_token=' . $token . '&_language=' . $language . '&_action=' . $action  . '&_cont=' . '" )</script>
		</head>
		<!--body id="Root" data-xdh-onevents="(keypress|About|SC+a)(keypress|Q37Refresh|SC+r)"-->
		<body>
			<input type="hidden" id="LoginUserID" value="' . $UserID . '"/>
			<input type="hidden" id="LoginPassword" value="' . $Password . '"/>
		</body>
	</html>'
?>