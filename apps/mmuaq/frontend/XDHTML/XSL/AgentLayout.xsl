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
    <fieldset title="#agentTitle#" data-xdh-cast="EditCast">
     <legend>#agentLegend#</legend>
     <!-- No '<xsl:apply-templates select="Agent"/>', because there is no 'Agent' entry when creating a new one...-->
     <div style="display: flex; flex-direction: column;">
      <xsl:if test="@Disabled='true'">
       <span style="display: flex; justify-content: center;font-style: italic;">
        <label>#agentDisabled#</label>
       </span>
      </xsl:if>
      <span style="display: flex; justify-content: space-between;">
       <label>#agentNameLabel#</label>
       <input id="AgentName" title="#agentNameTitle#" placeholder="#agentNamePlaceholder#" type="text">
        <xsl:attribute name="value">
         <xsl:value-of select="Agent/@Name"/>
        </xsl:attribute>
       </input>
      </span>
      <span style="display: flex; justify-content: space-between;">
       <label>#agentProtocolLabel#</label>
       <select id="AgentProtocol">
        <option value="POP3">
         <xsl:if test="Agent/@Protocol='POP3'">
          <xsl:attribute name="selected">selected</xsl:attribute>
         </xsl:if>
         <xsl:text>POP3</xsl:text>
        </option>
        <option value="IMAP">
         <xsl:if test="Agent/@Protocol='IMAP'">
          <xsl:attribute name="selected">selected</xsl:attribute>
         </xsl:if>
         <xsl:text>IMAP</xsl:text>
        </option>
       </select>
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
       <input id="AgentPasswordToggle" data-xdh-onevent="ToggleAgentPassword" title="#agentPasswordModificationTitle#" type="checkbox"/>
       <label>#agentPasswordModificationLabel#</label>
      </span>
      <span style="display: flex; justify-content: space-between;" data-xdh-cast="PasswordEditCast">
       <label>#agentPasswordLabel#</label>
       <input id="AgentPassword" title="#agentPasswordTitle#" placeholder="#agentPasswordPlaceholder#" type="password"/>
      </span>
     </div>
     <br data-xdh-cast="ActionCast"/>
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
 <xsl:template match="Agent">
 </xsl:template>
 </xsl:stylesheet>