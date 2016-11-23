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
    <fieldset id="Folders"/>
    <xsl:apply-templates select="Mails"/>
     <button title="#mainConfigTitle#" data-xdh-onevent="Configuration">#mainConfig#</button>
   </span>
  </span>
 </xsl:template>
 <xsl:template match="Mails">
  <table style="border: 1px solid black;">
   <tr>
    <th>Subject</th>
   </tr>
   <xsl:apply-templates select="Mail"/>
  </table>
 </xsl:template>
 <xsl:template match="Mail">
  <tr>
   <td>
    <xsl:value-of select="@Subject"/>
   </td>
  </tr>
 </xsl:template>
</xsl:stylesheet>