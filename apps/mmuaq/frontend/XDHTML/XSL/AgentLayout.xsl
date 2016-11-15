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
    <fieldset title="#agentTitle#" style="display: flex; justify-content: space-around;" data-xdh-cast="InputCast">
     <legend>#agentLegend#</legend>
      <fieldset style="display: flex; flex-direction: column;">
       <span style="display: flex; justify-content: space-between;">
        <label>#agentNameLabel#</label>
        <input title="#agentNameTitle#" placeholder="#agentNamePlaceholder#" type="text"/>
       </span>
       <span style="display: flex; justify-content: space-between;">
        <label>#agentHostPortLabel#</label>
        <input title="#agentHostPortTitle#" placeholder="#agentHostPortPlaceholder#" type="text"/>
       </span>
       <span style="display: flex; justify-content: space-between;">
        <label>#agentUsernameLabel#</label>
        <input title="#agentUsernameTitle#" placeholder="#agentUsernamePlaceholder#" type="text"/>
       </span>
       <span>
        <input title="#agentPasswordModificationTitle#" type="checkbox"/>
        <label>#agentPasswordModificationLabel#</label>
       </span>
       <span style="display: flex; justify-content: space-between;">
        <label>#agentPasswordLabel#</label>
        <input title="#agentPasswordTitle#" placeholder="#agentPasswordPlaceholder#" type="password"/>
       </span>
      </fieldset>
      <span style="display: flex; width: 100%; justify-content: space-around;">
       <button title="#agentSubmitTitle#">#agentSubmit#</button>
       <button title="#agentCancelTitle#" data-xdh-onevent="DiscardAgent">#agentCancel#</button>
      </span>
    </fieldset>
   </span>
  </span>
 </xsl:template>
 <xsl:template match="Corpus">
 </xsl:template>
</xsl:stylesheet>