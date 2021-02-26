<?php
/*
Copyright (C) 2020 Claude SIMON (http://q37.info/contact/).

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

// NOTA: '$_REQUEST["text"]' is already URL decoded…
$html = base64_decode($_REQUEST["text"]);

$out = <<<EOS
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8"/>
	<script>
		function adjust() {
			let iframe = window.parent.document.body.firstElementChild.firstElementChild.firstElementChild;
			iframe.height = "1"
			iframe.height = iframe.contentWindow.document.body.scrollHeight + 'px';
		}
	</script>
</head>
<body onload="adjust();">
$html
</body>
EOS;

echo $out;
?>
