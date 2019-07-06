<?php
/*
Copyright (C) 2018 Claude SIMON (http://q37.info/contact/).

This file is part of XDHq.

WDHq is free software: you can redistribute it and/or
modify it under the terms of the GNU Affero General Public License as
published by the Free Software Foundation, either version 3 of the
License, or (at your option) any later version.

XDHq is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with XDHq. If not, see <http://www.gnu.org/licenses/>.
 */

function getXDHq() {
    if (getenv("EPEIOS_SRC") !== false) {
        switch (strtoupper(substr(php_uname('s'), 0, 3))) {
        case "WIN":
            $epeiosPath = "h:\\hg\\epeios\\";
            break;
        case "LIN":
            $epeiosPath = "/home/csimon/hg/epeios/";
            break;
        case "DAR":
            $epeiosPath = "/Users/csimon/hg/epeios/";
            break;
        default:
            die("Unknown OS !!!");
            break;
        }

        $xdhqPath = $epeiosPath . "tools/xdhq/wrappers/ZND/";
    } else {
        $xdhqPath = "phar://Atlas.phar/";
    }

    require $xdhqPath . "XDHq.php";
}

getXDHq();

set_error_handler(function ($errno, $errstr) {
    return strpos($errstr, 'Declaration of') === 0;
}, E_WARNING);

class Atlas extends Thread {
    const GUI_NONE = 0;
    const GUI_DESKTOP = 1;
    const GUI_WEB = 2;
    const GUI_DESKTOP_AND_WEB = 3;
    const GUI_DEFAULT = self::GUI_DESKTOP;

    static private function execute_(string $command) {
        if (XDHq::isWin()) {
            fclose(popen("start " . $command, 'r'));
        } else {
            fclose(popen($command . " &", 'r'));
        }
    }

    private static function launchWeb_($dir) {
        if (XDHq::isDev()) {
            self::execute_("node h:/hg/epeios/tools/xdhq/examples/common/httpd.js h:/hg/epeios/tools/xdhq/examples/common/" . $dir);
        } else {
            self::execute_("node -e \"require(require('xdhwebqnjs').fileName).launch('" . $dir . "');\"");
        }

    }

    private static function launchDesktop_($dir) {
        if (self::isDev()) {
            self::execute_("h:/hg/epeios/tools/xdhelcq/node_modules/electron/dist/electron h:/hg/epeios/tools/xdhelcq/index.js -m=h:/bin/xdhqxdh h:/hg/epeios/tools/xdhq/examples/common/" . $dir);
        } else {
            self::execute_("node -e \"require('child_process').spawnSync(require('xdhelcq').electron,[require('path').join(require('xdhelcq').path, 'index.js'),'-m=' + require('xdhqxdh').fileName, require('upath').normalize(require('path').join(require('path').resolve(__dirname),'" . $dir . "'))])\"");
        }

    }

    private static function getDefaultGUI_() {
        if (XDHq::isDev()) {
            return self::GUI_DESKTOP;
        } else {
            return self::GUI_WEB;
        }

    }

    private static function getDefaultMODE_() {
        if (XDHq::isDev()) {
            return XDHq::MODE_PROD;
        } else {
            return XDHq::MODE_DEMO;
        }

    }

    static function readAsset($path, string $dir = ".") {
        return XDHq::readAsset($path, $dir);
    }

    public function __construct($userObject) {
        $this->dom = new XDHqDOM($this);
        $this->userObject = $userObject;
    }

	public static function new(callable $callback) {
		return new Atlas(call_user_func($callback));
	}


    public static function launch(callable $callback, string $headContent = "", $gui = null, string $dir = ".") {
        global $argv;

        $mode = self::getDefaultMode_();

        if ($gui === null) {
            $gui = self::getDefaultGUI_();

            if (count($argv) > 1) {
                switch ($argv[1]) {
                case "d":
                case "desktop":
                    $gui = self::GUI_DESKTOP;
                    break;
                case "w":
                case "web":
                    $gui = self::GUI_WEB;
                    break;
                case "dw":
                case "wd":
                    $gui = self::GUI_DESKTOP_AND_WEB;
                    break;
                case "n":
                case "none":
                    $gui = self::GUI_NONE;
                    break;
                case 'W':
                    $gui = self::GUI_NONE; // The httpd server is launched externally.
                    $mode = XDHq::MODE_DEMO;
                    break;
                default:
                    die("Unknown GUI !");
                    break;
                }
            }
        }

        XDHq::launch(array("Atlas", "new"), $callback, $headContent, $mode, $dir);

        switch ($gui) {
        case self::GUI_NONE:
            break;
        case self::GUI_DESKTOP:
            self::launchDesktop_($dir);
            break;
        case self::GUI_WEB:
            self::launchWeb_($dir);
            break;
        case self::GUI_DESKTOP_AND_WEB:
            self::launchDesktop_($dir);
            self::launchWeb_($dir);
            break;
        default:
            die("Unknown gui !!!");
            break;
        }
    }

    public function run() {
        while (true) {
            $action = $this->dom->getAction($id);
            call_user_func(array($this->userObject, 'handle'), $this->dom, $action, $id);
        }
    }

    public static function createXML(string $rootTag) {
        return new XDHqXML($rootTag);
    }

    public static function createHTML(string $rootTag = "") {
        return new XDHqXML($rootTag);
    }
};
?>
