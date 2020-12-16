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

$url = $_REQUEST['url'];

$out='
<head>
  <script src="faas/qrcode.min.js"></script>
</head>
<body onload="new QRCode(\'qrcode\', {width:125, height:125, correctLevel: QRCode.CorrectLevel.L}).makeCode(\'' . $url . '\');">
  <div style="display:table; margin: 5px auto auto auto;">
    <hr/>
    <span style="display: table; margin: 10px auto 5px auto;">Scan this QR code with your smartphone,</span>
    <div style="display: flex; justify-content: space-around;">
      <a style="cursor: pointer;" target="_blank" href="' . $url . '">
        <div id="qrcode"></div>
      </a>
  </div>
  <span style="display: table; margin: 10px auto 5px auto;">or simply click on it for a new instance.</span>
  <hr/>
  <div style="display: table; padding: 10px; margin: auto;">
    <div style="display: table; padding: 10px;">
      <div style="display: table; margin: auto; border: 1px solid; padding: 10px; box-shadow: 5px 5px; text-align: center;">
        <span style="text-shadow: 1px 1px;">
          <span>Powered by the <a href="http://atlastk.org/" target="_blank"><span style="font-style: italic;">Atlas</span> toolkit</a></span>
        </span>
      </div>
    </div>
  </div>
</body>
';

echo $out;
?>