<?php
require "XPPq.php";

echo ZNDq::wrapperInfo() . "\n";
echo ZNDq::componentInfo() . "\n";

echo XPPq::returnArgument( "Argument from the PHP file !") . "\n";


class XPPStream {
    var $position;
    var $varname;

    function stream_open($path, $mode, $options, &$opened_path)
    {
        $url = parse_url($path);
        $this->varname = $url["host"];
        $this->position = 0;

        return true;
    }

    function stream_read($count)
    {
        $ret = fread($GLOBALS[$this->varname], $count);
        $this->position += strlen($ret);
        return $ret;
    }

    function stream_write($data)
    {
        $left = substr($GLOBALS[$this->varname], 0, $this->position);
        $right = substr($GLOBALS[$this->varname], $this->position + strlen($data));
        $GLOBALS[$this->varname] = $left . $data . $right;
        $this->position += strlen($data);
        return strlen($data);
    }

    function stream_tell()
    {
        return $this->position;
    }

    function stream_eof()
    {
        return feof($GLOBALS[$this->varname]);
    }

    function stream_seek($offset, $whence)
    {
        switch ($whence) {
            case SEEK_SET:
                if ($offset < strlen($GLOBALS[$this->varname]) && $offset >= 0) {
                     $this->position = $offset;
                     return true;
                } else {
                     return false;
                }
                break;

            case SEEK_CUR:
                if ($offset >= 0) {
                     $this->position += $offset;
                     return true;
                } else {
                     return false;
                }
                break;

            case SEEK_END:
                if (strlen($GLOBALS[$this->varname]) + $offset >= 0) {
                     $this->position = strlen($GLOBALS[$this->varname]) + $offset;
                     return true;
                } else {
                     return false;
                }
                break;

            default:
                return false;
        }
    }

    function stream_metadata($path, $option, $var) 
    {
  if($option == STREAM_META_TOUCH) {
   $url = parse_url($path);
         $varname = $url["host"];
   if(!isset($GLOBALS[$varname])) {
          $GLOBALS[$varname] = '';
   }
   return true;
  }
  return false;
    }
}

class StringToStream {
    var $position;
    var $varname;

    function stream_open($path, $mode, $options, &$opened_path)
    {
        $url = parse_url($path);
        $this->varname = $url["host"];
        $this->position = 0;

        return true;
    }

    function stream_read($count)
    {
        $ret = fread($GLOBALS[$this->varname], $count);
        $this->position += strlen($ret);
        return $ret;
    }

    function stream_write($data)
    {
        $left = substr($GLOBALS[$this->varname], 0, $this->position);
        $right = substr($GLOBALS[$this->varname], $this->position + strlen($data));
        $GLOBALS[$this->varname] = $left . $data . $right;
        $this->position += strlen($data);
        return strlen($data);
    }

    function stream_tell()
    {
        return $this->position;
    }

    function stream_eof()
    {
        return feof($GLOBALS[$this->varname]);
    }

    function stream_seek($offset, $whence)
    {
        switch ($whence) {
            case SEEK_SET:
                if ($offset < strlen($GLOBALS[$this->varname]) && $offset >= 0) {
                     $this->position = $offset;
                     return true;
                } else {
                     return false;
                }
                break;

            case SEEK_CUR:
                if ($offset >= 0) {
                     $this->position += $offset;
                     return true;
                } else {
                     return false;
                }
                break;

            case SEEK_END:
                if (strlen($GLOBALS[$this->varname]) + $offset >= 0) {
                     $this->position = strlen($GLOBALS[$this->varname]) + $offset;
                     return true;
                } else {
                     return false;
                }
                break;

            default:
                return false;
        }
    }

    function stream_metadata($path, $option, $var) 
    {
  if($option == STREAM_META_TOUCH) {
   $url = parse_url($path);
         $varname = $url["host"];
   if(!isset($GLOBALS[$varname])) {
          $GLOBALS[$varname] = '';
   }
   return true;
  }
  return false;
    }
}

/*
stream_wrapper_register("xpp", "XPPStream")
    or die("Failed to register protocol");
*/

stream_wrapper_register("xpp", "XMLPreprocessor")
    or die("Failed to register protocol");



$XML = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n" .
"<SomeTag xmlns:xpp=\"http://q37.info/ns/xpp/\" AnAttribute=\"SomeAttributeValue\">\n" .
" <SomeOtherTag AnotherAttribute=\"AnotherAttributeValue\">TagValue</SomeOtherTag>\n" .
" <xpp:define name=\"SomeMacro\">\n" .
"  <xpp:bloc>Some macro content from string !</xpp:bloc>\n" .
" </xpp:define>\n" .
" <YetAnotherTag YetAnotherAttribute=\"YetAnotherAttributeValue\">\n" .
"  <xpp:expand select=\"SomeMacro\"/>\n" .
" </YetAnotherTag>\n" .
"</SomeTag>\n"
 ;

 $fxml = fopen('data://text/plain,' . $XML, 'r' );
//	$fxml = fopen('Project.xml', 'r' );
	$stream = fopen("xpp://fxml", 'r' );

	while (!feof($stream)) {
    echo fgets($stream);
}

?>