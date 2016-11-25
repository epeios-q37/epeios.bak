<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet	version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:xpp="http://q37.info/ns/xpp/">
 <xsl:output method="html" encoding="UTF-8"/>
 <xsl:template match="/">
  <xsl:apply-templates select="*/Content"/>
  <xsl:apply-templates select="*/Corpus"/>
 </xsl:template>
 <xsl:template match="Content">
  <div>
   <xsl:apply-templates select="Mails"/>
  </div>
  <fieldset id="Mail"/>
 </xsl:template>
 <xsl:template match="Mails">
  <table style="border: 1px solid black; width: 100%;">
   <tr>
    <th>Subject</th>
   </tr>
   <xsl:apply-templates select="Mail"/>
  </table>
 </xsl:template>
 <xsl:template match="Mail">
  <tr data-xdh-onevent="SelectMail" style="cursor: pointer;">
   <xsl:attribute name="data-xdh-content">
    <xsl:value-of select="@id"/>
   </xsl:attribute>
   <td>
    <xsl:value-of select="@Subject"/>
   </td>
  </tr>
 </xsl:template>
</xsl:stylesheet>