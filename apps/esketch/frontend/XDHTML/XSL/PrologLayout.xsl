<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet	version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:xpp="http://q37.info/ns/xpp/">
	<xsl:output method="html" encoding="UTF-8"/>
	<xsl:template match="/">
		<xsl:apply-templates select="*/Layout"/>
	</xsl:template>
	<xsl:template match="Layout">
		<span class="vcenter-out">
			<span class="vcenter-in">
				<fieldset title="#plgProjectToLoad#">
					<legend>#plgProject#</legend>
					<div>
						<span>
							<fieldset data-xdh-cast="NewProjectCast">
								<select id="ProjectType" data-xdh-onevent="SwitchProjectType" title="#plgProjectTypes#">
									<option value="New">
										<xsl:if test="DefaultProjectType='New'">
											<xsl:attribute name="selected">selected</xsl:attribute>
										</xsl:if>
										<xsl:text>#plgNewProjectOption#</xsl:text>
									</option>
									<option value="Predefined">
										<xsl:if test="DefaultProjectType='Predefined'">
											<xsl:attribute name="selected">selected</xsl:attribute>
										</xsl:if>
										<xsl:text>#plgPredefinedProjectOption#</xsl:text>
									</option>
									<option value="Remote">
										<xsl:if test="DefaultProjectType='Remote'">
											<xsl:attribute name="selected">selected</xsl:attribute>
										</xsl:if>
										<xsl:text>#plgUserProjectOption#</xsl:text>
									</option>
								</select>
								<xsl:apply-templates select="Projects"/>
								<span style="display: inline-block;">
									<!-- if the 'style' attribute is set in the child element, the 'hidden' attribute doesn't work anymore on this child element...-->
									<fieldset data-xdh-cast="UserProjectCast" title="#plgProjectFileToLoad#">
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
							<button data-xdh-onevent="LoadProject" title="#plgLoadProject#">#plgOK#</button>
							<button title="#lgnCancelConnection#" data-xdh-onevent="Dismiss">#lgnCancel#</button>
						</span>
					</div>
				</fieldset>
			</span>
		</span>
	</xsl:template>
	<xsl:template match="Projects">
		<select id="PredefinedProject" data-xdh-cast="PredefinedProjectsCast" title="#plgPredefinedProjects#">
			<xsl:apply-templates select="Project"/>
		</select>
	</xsl:template>
	<xsl:template match="Project">
		<option>
			<xsl:if test="@Selected='true'">
				<xsl:attribute name="selected">selected</xsl:attribute>
			</xsl:if>
			<xsl:attribute name="value">
				<xsl:value-of select="@id"/>
			</xsl:attribute>
			<xsl:value-of select="@Alias"/>
		</option>
	</xsl:template>
</xsl:stylesheet>