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

$out='
<head>
	<meta charset="UTF-16"></meta>
	<script src="https://cdnjs.cloudflare.com/ajax/libs/showdown/1.9.1/showdown.min.js" integrity="sha512-L03kznCrNOfVxOUovR6ESfCz9Gfny7gihUX/huVbQB9zjODtYpxaVtIaAkpetoiyV2eqWbvxMH9fiSv5enX7bw==" crossorigin="anonymous"></script>
	<script>
		function markdownToHTML(text) {
			converter = new showdown.Converter();
			return converter.makeHtml(text).replace(/<a /g, "<a target=\'_blank\' ");
		}
		function b64DecodeUnicode(str) {
			return decodeURIComponent(atob(str).split("").map(function(c) {
				return "%" + ("00" + c.charCodeAt(0).toString(16)).slice(-2);
			}).join(""));
		}
		function adjust() {
			let iframe = window.parent.document.body.firstElementChild.firstElementChild;
			iframe.height = "1";
			iframe.height = iframe.contentWindow.document.body.scrollHeight + \'px\';
		}
	</script>
	<style>
		details {
			font: 16px "Open Sans", Calibri, sans-serif;
			width: 620px;
		}

		details > summary {
			padding: 2px 6px;
			width: 15em;
			background-color: #ddd;
			border: none;
			box-shadow: 3px 3px 4px black;
			cursor: pointer;
		}

		details > div {
			border-radius: 0 0 10px 10px;
			background-color: #ddd;
			padding: 2px 6px;
			margin: 0;
			box-shadow: 3px 3px 4px black;
		}

		@keyframes fadeInDown {
			0% {
				opacity: 0;
				transform: scale(0);
			}

			100% {
				opacity: 1;
				transform: scale(1);
			}
		}

		details[open] {
			animation-name: fadeInDown;
			animation-duration: 1s;
		}
	</style>	
</head>
<body onload="document.getElementById(\'desc\').innerHTML=markdownToHTML(b64DecodeUnicode(\'' . $_REQUEST["text"]. '\'));adjust();">
	<details ontoggle="adjust();" style="width: 100%;">
		<summary>Description</summary>
		<div id="desc"/>
	</details>
</body>
';

echo $out;
?>
