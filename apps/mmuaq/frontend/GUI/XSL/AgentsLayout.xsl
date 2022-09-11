<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet	version="1.0"	xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:xpp="http://q37.info/ns/xpp/">
 <xsl:output method="html" encoding="UTF-8"/>
 <xsl:template match="/">
  <xsl:apply-templates select="*/Corpus"/>
  <xsl:apply-templates select="*/Content"/>
 </xsl:template>
 <xsl:template match="Corpus">
 </xsl:template>
 <xsl:template match="Content">
  <span class="vcenter-out">
   <span class="vcenter-in">
    <span>
     <fieldset>
      <span style="display: flex; flex-direction: row;">
       <fieldset style="height: 100%" title="#agentsTitle#">
        <legend>#agentsLegend#</legend>
        <span style="display: flex; flex-direction: column;">
        <span>
         <xsl:apply-templates select="Agents"/>
        </span>
         <span style="display: flex; flex-direction: column;">
         <button title="#agentsEditTitle#" data-xdh-onevent="EditAgent" data-xdh-cast="EditAgentCast">#agentsEdit#</button>
         <button title="#agentsNewTitle#" data-xdh-onevent="DefineAgent" data-xdh-cast="CreateAgentCast">#agentsNew#</button>
          <button title="#agentsRemoveTitle#" data-xdh-onevent="RemoveAgent" data-xdh-cast="EditAgentCast">#agentsRemove#</button>
         </span>
        </span>
       </fieldset>
       <span id="Agent"/>
      </span>
     </fieldset>
    </span>
   </span>
  </span>
 </xsl:template>
 <xsl:template match="Agents">
  <select size="10" data-xdh-cast="SelectAgentCast">
   <xsl:choose>
    <xsl:when test="@Amount=0">
    <option style="font-style: italic;">#agentsNoAgent#</option>
    </xsl:when>
    <xsl:otherwise>
     <xsl:apply-templates select="Agent"/>
    </xsl:otherwise>
   </xsl:choose>
  </select>
 </xsl:template>
 <xsl:template match="Agent">
  <option data-xdh-onevent="SelectAgent">
   <xsl:if test="@Disabled='true'">
    <xsl:attribute name="style">text-decoration: line-through;</xsl:attribute>
   </xsl:if>
   <xsl:attribute name="value">
    <xsl:value-of select="@id"/>
   </xsl:attribute>
   <xsl:if test="../@Current=@id">
    <xsl:attribute name="selected">selected</xsl:attribute>
   </xsl:if>
   <xsl:value-of select="."/>
  </option>
 </xsl:template>
</xsl:stylesheet>