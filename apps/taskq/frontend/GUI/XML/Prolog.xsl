<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet	version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:xpp="http://q37.info/ns/xpp/">
	<xsl:output method="html" encoding="UTF-8"/>
	<xsl:template match="/">
		<xsl:apply-templates select="*/Layout"/>
	</xsl:template>
	<xsl:template match="Layout">
		<fieldset title="#plgPresetToUse#">
			<legend>#plgPreset#</legend>
			<div>
				<span>
					<fieldset id="Border">
						<select id="Preset" data-xdh-onevent="SwitchPreset" title="#plgPreset#">
							<option value="None">
								<xsl:if test="DefaultPreset='None'">
									<xsl:attribute name="selected">selected</xsl:attribute>
								</xsl:if>
								<xsl:text>#plgNonePresetOption#</xsl:text>
							</option>
							<option value="Setup">
								<xsl:if test="DefaultPreset='Setup'">
									<xsl:attribute name="selected">selected</xsl:attribute>
								</xsl:if>
								<xsl:text>#plgSetupPresetOption#</xsl:text>
							</option>
							<option value="Project">
								<xsl:if test="DefaultPreset='Project'">
									<xsl:attribute name="selected">selected</xsl:attribute>
								</xsl:if>
								<xsl:text>#plgProjectPresetOption#</xsl:text>
							</option>
						</select>
						<xsl:apply-templates select="Setups"/>
						<span style="display: inline-block;">
							<!-- if the 'style' attribute is set in the child element, the 'hidden' attribute doesn't work anymore on this child element...-->
							<fieldset id="ProjectForm" title="#plgProjectFileToLoad#">
								<legend>#plgProjectFile#</legend>
								<button
									data-xdh-onevent="(OpenFile|DisplayProjectFilename|(#plgSelectProjectFile#|.xprj))"
									title="#plgBrowseProjectFiles#">#plgBrowse#</button>
								<input id="RemoteProject" type="text" size="50"/>
							</fieldset>
						</span>
					</fieldset>
				</span>
				<span class="hcenter">
					<button data-xdh-onevent="LoadPreset" title="#plgLoadProject#">#plgOK#</button>
					<button title="#lgnCancelConnection#" data-xdh-onevent="Dismiss">#lgnCancel#</button>
				</span>
			</div>
		</fieldset>
	</xsl:template>
	<xsl:template match="Setups">
		<span id="SetupForm">
			<select id="Setup" title="#plgSetups#">
				<xsl:apply-templates select="Setup"/>
			</select>
		</span>
	</xsl:template>
	<xsl:template match="Setup">
		<option>
			<xsl:if test="../@Default=@id">
				<xsl:attribute name="selected">selected</xsl:attribute>
			</xsl:if>
			<xsl:attribute name="value">
				<xsl:value-of select="@id"/>
			</xsl:attribute>
			<xsl:value-of select="@Alias"/>
		</option>
	</xsl:template>
</xsl:stylesheet>