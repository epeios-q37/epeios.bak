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
         <button title="#agentsEditTitle#" data-xdh-cast="EditAgentCast">#agentsEdit#</button>
         <button title="#agentsNewTitle#" data-xdh-onevent="NewAgent" data-xdh-cast="CreateAgentCast">#agentsNew#</button>
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
   </xsl:choose>
  </select>
 </xsl:template>
</xsl:stylesheet>