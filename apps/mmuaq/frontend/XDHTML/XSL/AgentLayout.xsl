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
    <fieldset title="#agentTitle#" style="display: flex;" data-xdh-cast="EditCast">
     <legend>#agentLegend#</legend>
     <div style="display: flex; flex-direction: column;">
      <span style="display: flex; justify-content: space-between;">
       <label>#agentNameLabel#</label>
       <input id="AgentName" title="#agentNameTitle#" placeholder="#agentNamePlaceholder#" type="text">
        <xsl:attribute name="value">
         <xsl:value-of select="Agent/@Name"/>
        </xsl:attribute>
       </input>
      </span>
      <span style="display: flex; justify-content: space-between;">
       <label>#agentHostPortLabel#</label>
       <input id="AgentHostPort" title="#agentHostPortTitle#" placeholder="#agentHostPortPlaceholder#" type="text">
        <xsl:attribute name="value">
         <xsl:value-of select="Agent/@HostPort"/>
        </xsl:attribute>
       </input>
      </span>
      <span style="display: flex; justify-content: space-between;">
       <label>#agentUsernameLabel#</label>
       <input id="AgentUsername" title="#agentUsernameTitle#" placeholder="#agentUsernamePlaceholder#" type="text">
        <xsl:attribute name="value">
         <xsl:value-of select="Agent/@Username"/>
        </xsl:attribute>
       </input>
      </span>
      <span data-xdh-cast="PasswordEditionChoiceCast">
       <input title="#agentPasswordModificationTitle#" type="checkbox"/>
       <label>#agentPasswordModificationLabel#</label>
      </span>
      <span style="display: flex; justify-content: space-between;" data-xdh-cast="PasswordEditCast">
       <label>#agentPasswordLabel#</label>
       <input id="AgentPassword" title="#agentPasswordTitle#" placeholder="#agentPasswordPlaceholder#" type="password"/>
      </span>
     </div>
     <br  data-xdh-cast="ActionCast"/>
     <div style="display: flex; width: 100%; justify-content: space-around;">
      <button title="#agentSubmitTitle#" data-xdh-onevent="SubmitAgent" data-xdh-cast="ActionCast">#agentSubmit#</button>
      <button title="#agentCancelTitle#" data-xdh-onevent="DiscardAgent" data-xdh-cast="ActionCast">#agentCancel#</button>
     </div>
    </fieldset>
   </span>
  </span>
 </xsl:template>
 <xsl:template match="Corpus">
 </xsl:template>
</xsl:stylesheet>