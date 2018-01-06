<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" 
  xmlns="http://www.w3.org/1999/xhtml" 
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
  <xsl:output method="html" encoding="UTF-8"/>
  <xsl:template match="/">
    <div>
      <input type="textbox" id="Message" value="Hello UnJSq!"/>
      <button data-xdh-onevent="Reverse">Reverse Message</button>
    </div>
  </xsl:template>
</xsl:stylesheet>