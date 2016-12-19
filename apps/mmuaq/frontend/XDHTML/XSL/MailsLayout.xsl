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
    <th style="width: 15px;">
     <img src="js/cursor_drag_hand.png" style="width: 15px; height: 15px"/>
    </th>
    <th>#mailsSubjectHeader#</th>
    <th>#mailsAccountHeader#</th>
   </tr>
   <xsl:apply-templates select="Mail"/>
  </table>
 </xsl:template>
 <xsl:template match="Mail">
  <tr data-xdh-onevent="SelectMail" data-xdh-cast="mailCasting">
   <xsl:attribute name="data-xdh-content">
    <xsl:value-of select="@id"/>
   </xsl:attribute>
   <td>
    <img src="js/cursor_drag_hand.png" title="#mailsDragTitle#" style="width: 15px; height: 15px" data-xdh-onevents="(dragstart|DragMail)|(dragend|EndMailDragging)">
     <xsl:attribute name="data-xdh-content">
      <xsl:value-of select="@id"/>
     </xsl:attribute>
    </img>
   </td>
   <td>
    <xsl:value-of select="@Subject"/>
   </td>
   <td>
    <xsl:variable name="Agent" select="@Agent"/>
    <xsl:value-of select="/*/Content/Agents/Agent[@id=$Agent]"/>
   </td>
  </tr>
 </xsl:template>
</xsl:stylesheet>