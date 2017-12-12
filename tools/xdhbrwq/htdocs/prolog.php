<?php
	$out = '
		<!DOCTYPE html>
		<html>
			<head>
			<!-- If modified, report modification in "xdhcefq.html" from "xdhcefq" -->
			<!-- BEGIN -->
			<meta charset="UTF-8" />
			<!-- ' . $head . ' -->
' . file_get_contents( $head ) . '
			<script src="js/xdhtml.js"></script>
			<script src="xdhbrwq.js"></script>
			<script>handleQuery("xdh.php?_target=' . $target . '&_language=' . $language . '&_action=' . $action . '")</script>
		</head>
		<body id="Root" data-xdh-onevents="(keypress|About|SC+a)(keypress|Q37Refresh|SC+r)">
			<input type="hidden" id="LoginUserID" value="' . $UserID . '"/>
			<input type="hidden" id="LoginPassword" value="' . $Password . '"/>
		</body>
	</html>'
?>