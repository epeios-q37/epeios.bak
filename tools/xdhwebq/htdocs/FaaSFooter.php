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

const PATTERN = "_supplier=qrcode";

$rawUrl = $_REQUEST['url'];
$url = str_replace("&" . PATTERN, "", $rawUrl);
$detailsOpenAttribute = strpos($rawUrl, PATTERN) ? 'open="open"' : "";

echo <<<EOS
<head>
  <script src="qrcode.min.js"></script>
  <script>
    function adjustHeight() {
      let iframe = window.frameElement;
      iframe.style.height = '50px';
      let body = document.body;
      iframe.style.height = body.scrollHeight + 'px';
    }
  </script>
  <style>
    details {
      font-size: 1rem;
      margin: auto;
      box-shadow: 0 20px 25px -5px rgba(0, 0, 0, 0.1), 0 10px 10px -5px rgba(0, 0, 0, 0.04);
      width: 100%;
      background: #ffffff;
      border-radius: 8px;
      position: relative;
      width: 500px;
    }

    details .summary-title {
      -webkit-user-select: none;
      -moz-user-select: none;
      -ms-user-select: none;
      user-select: none;
    }

    details:hover {
      cursor: pointer;
    }

    details .summary-content {
      border-top: 1px solid #e2e8f0;
      cursor: default;
      padding: 1em;
      font-weight: 300;
      line-height: 1.5;
    }

    details summary {
      padding: 1em;
    }

    details summary:focus {
      outline: none;
    }

    details summary:hover .summary-chevron-up svg {
      opacity: 1;
    }

    details .summary-chevron-up svg {
      opacity: 0.5;
    }

    details .summary-chevron-up,
    details .summary-chevron-down {
      pointer-events: none;
      position: absolute;
      top: 0.75em;
      right: 1em;
      background: #ffffff;
    }

    details .summary-chevron-up svg,
    details .summary-chevron-down svg {
      display: block;
    }

    details summary::-webkit-details-marker {
      display: none;
    }

    @keyframes fadeInDown {
      0% {
        opacity: 0;
        transform: translateY(-5em);
      }

      100% {
        opacity: 1;
        transform: translateY(0);
      }
    }

    details[open] {
      animation-name: fadeInDown;
      animation-duration: 1s;
    }

    summary {
      list-style: none;
    }
  </style>
</head>

<body
  onload="new QRCode('qrcode', {width:125, height:125, correctLevel: QRCode.CorrectLevel.L}).makeCode('$url');adjustHeight();">
  <details ontoggle="adjustHeight();" $detailsOpenAttribute>
    <summary> <span class="summary-title">More…</span>
      <div class="summary-chevron-up">
        <svg xmlns="http://www.w3.org/2000/svg" width="24" height="24" viewBox="0 0 24 24" fill="none"
          stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"
          class="feather feather-chevron-down">
          <polyline points="6 9 12 15 18 9"></polyline>
        </svg>
      </div>
    </summary>
    <div class="summary-content">
      <hr />
      <span style="display: table; margin: 10px auto 5px auto;">QR code to scan with a mobile device,</span>
      <div style="display: flex; justify-content: space-around;">
        <a style="cursor: pointer;" target="_blank" href="$url">
          <div id="qrcode"></div>
        </a>
      </div>
      <span style="display: table; margin: 10px auto 5px auto;">or to click on for a new instance.</span>
      <hr />
      <div style="display: table; padding: 10px; margin: auto;">
        <div style="display: table; padding: 10px;">
          <div
            style="display: table; margin: auto; border: 1px solid; padding: 10px; box-shadow: 5px 5px; text-align: center;">
            <span style="text-shadow: 1px 1px;">
              <span>Powered by the <a href="http://atlastk.org/" target="_blank"><span
                    style="font-style: italic;">Atlas</span> toolkit</a></span>
            </span>
          </div>
        </div>
      </div>
    </div>
    <div class="summary-chevron-down">
      <svg xmlns="http://www.w3.org/2000/svg" width="24" height="24" viewBox="0 0 24 24" fill="none"
        stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"
        class="feather feather-chevron-up">
        <polyline points="18 15 12 9 6 15"></polyline>
      </svg>
    </div>
  </details>
</body>
EOS;
?>
