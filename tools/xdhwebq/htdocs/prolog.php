<?php
	$out = '
		<!DOCTYPE html>
		<html>
			<head id="_xdh_head">
			<!-- If modified, report modification in "xdhcefq.html" from "xdhcefq" -->
			<!-- BEGIN -->
			<meta charset="UTF-8" />
			<script src="xdh/xdhtml.js"></script>
			<script src="xdh/xdhwebq.js"></script>
			<script>handleQuery("atlas.php?_token=' . $token . '&_language=' . $language . '&_action=' . $action  . '&_cont=' . '" )</script>
		</head>
		<!--body id="Root" data-xdh-onevents="(keypress|About|SC+a)(keypress|Q37Refresh|SC+r)"-->
		<body>
			<input type="hidden" id="LoginUserID" value="' . $UserID . '"/>
			<input type="hidden" id="LoginPassword" value="' . $Password . '"/>
		</body>
	</html>'
?>