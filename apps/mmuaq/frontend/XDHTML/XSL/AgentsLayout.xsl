<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet	version="1.0"	xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:xpp="http://q37.info/ns/xpp/">
 <xsl:output method="html" encoding="UTF-8"/>
 <xsl:template match="/">
  <xsl:apply-templates select="*/Content"/>
  <xsl:apply-templates select="*/Corpus"/>
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
         <select size="10">
          <option>toto</option>
         </select>
        </span>
         <span style="display: flex; flex-direction: column;">
         <button title="#agentsEditTitle#">#agentsEdit#</button>
          <button title="#agentsNewTitle#">#agentsNew#</button>
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
 <xsl:template match="Corpus">
 </xsl:template>
</xsl:stylesheet>