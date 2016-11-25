<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet	version="1.0"
                xmlns="http://www.w3.org/1999/xhtml"
				xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
				xmlns:xpp="http://q37.info/xpp"
				>
 <xsl:output method="html" encoding="UTF-8"/>
 <xsl:template match="/">
  <xsl:apply-templates select="*/Content"/>
 </xsl:template>
 <xsl:template match="Content">
  <span class="vcenter-out">
   <span class="vcenter-in">
    <!--div>
     <ul class="tab">
      <li>
       <a>#mainMessages#</a>
      </li>
      <li>
       <a title="#mainConfigTitle#" data-xdh-onevent="Configuration">#mainConfig#</a>
      </li>
     </ul>
    </div-->
    <fieldset>
     <fieldset>
      <div style="display: flex; flex-direction: row;">
       <fieldset id="Folders"/>
       <fieldset id="Mails"/>
     </div>
     </fieldset>
     <button title="#mainConfigTitle#" data-xdh-onevent="Configuration">#mainConfig#</button>
    </fieldset>
   </span>
  </span>
 </xsl:template>
</xsl:stylesheet>