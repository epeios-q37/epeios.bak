<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet	version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:xpp="http://q37.info/ns/xpp/">
	<xsl:output method="html" encoding="UTF-8"/>
	<xsl:template match="/">
		<xsl:apply-templates select="*/Layout"/>
	</xsl:template>
	<xsl:template match="Layout">
		<xsl:variable name="BackendType">
			<xsl:choose>
				<xsl:when test="Backend/@Type">
					<xsl:value-of select="Backend/@Type"/>
				</xsl:when>
				<xsl:otherwise>
					<xsl:value-of select="DefaultBackendType"/>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<span class="vcenter-out">
			<span class="vcenter-in">
				<fieldset>
					<fieldset title="#lgnLoginTitle#">
						<legend>#lgnLogin#</legend>
						<xsl:element name="input">
							<xsl:attribute name="type">text</xsl:attribute>
							<xsl:attribute name="id">LoginUserID</xsl:attribute>
							<xsl:attribute name="title">#lgnLoginUserIDTitle#</xsl:attribute>
							<xsl:attribute name="placeholder">#lgnLoginUserIDPlaceholder#</xsl:attribute>
							<xsl:attribute name="value">
								<xsl:value-of select="Login/UserID"/>
							</xsl:attribute>
						</xsl:element>
						<xsl:element name="input">
							<xsl:attribute name="type">password</xsl:attribute>
							<xsl:attribute name="id">LoginPassword</xsl:attribute>
							<xsl:attribute name="title">#lgnLoginPasswordTitle#</xsl:attribute>
							<xsl:attribute name="placeholder">#lgnLoginPasswordPlaceholder#</xsl:attribute>
							<xsl:attribute name="value">
								<xsl:value-of select="Login/Password"/>
							</xsl:attribute>
						</xsl:element>
					</fieldset>
					<fieldset title="#lgnBackendToUse#" id="Backend">
						<legend>#lgnBackend#</legend>
						<div>
							<span>
								<select id="BackendType" title="#lgnBackendType#" data-xdh-onevent="SwitchBackendType">
									<option value="Straight">
										<xsl:if test="$BackendType='Straight'">
											<xsl:attribute name="selected">selected</xsl:attribute>
										</xsl:if>
										<xsl:text>#lgnStraightBackendOption#</xsl:text>
									</option>
									<option value="Embedded">
										<xsl:if test="$BackendType='Embedded'">
											<xsl:attribute name="selected">selected</xsl:attribute>
										</xsl:if>
										<xsl:text>#lgnEmbeddedBackendOption#</xsl:text>
									</option>
									<option value="Predefined">
										<xsl:if test="$BackendType='Predefined'">
											<xsl:attribute name="selected">selected</xsl:attribute>
										</xsl:if>
										<xsl:text>#lgnPredefinedBackendOption#</xsl:text>
									</option>
									<option value="None">
										<xsl:if test="$BackendType='None'">
											<xsl:attribute name="selected">selected</xsl:attribute>
										</xsl:if>
										<xsl:text>#lgnNoBackendOption#</xsl:text>
									</option>
								</select>
								<xsl:apply-templates select="Backends"/>
								<span style="display: inline-block;">
									<!-- if the 'style' attribute is set in the child element, the 'hidden' attribute doesn't work anymore on this child element...-->
									<fieldset title="#lgnEmbeddedBackendToUse#" id="EmbeddedBackend">
										<legend>#lgnEmbeddedBackend#</legend>
										<xsl:variable name="OS" select="../@OS"/>
										<xsl:variable name="DynamicLibraryExtension">
											<xsl:choose>
												<xsl:when test="contains($OS,'Win')">
													<xsl:text>.dll</xsl:text>
												</xsl:when>
												<xsl:when test="contains($OS,'Darwin')">
													<xsl:text>.dylib</xsl:text>
												</xsl:when>
												<xsl:when test="contains($OS,'POSIX')">
													<xsl:text>.so</xsl:text>
												</xsl:when>
											</xsl:choose>
										</xsl:variable>
										<button	title="#lgnBrowseEmbeddedBackends#">
											<xsl:attribute name="data-xdh-onevent">
												<xsl:text>(OpenFile|DisplayEmbeddedBackendFilename|(#lgnSelectEmbeddedBackend#|</xsl:text>
												<xsl:value-of select="$DynamicLibraryExtension"/>
												<xsl:text>|</xsl:text>
												<xsl:text>vtddspbkd</xsl:text>
												<xsl:value-of select="$DynamicLibraryExtension"/>
												<xsl:text>))</xsl:text>
											</xsl:attribute>#lgnBrowse#
										</button>
										<input type="text" size="50">
											<xsl:if test="Backend/@Type='Embedded'">
												<xsl:attribute name="value">
													<xsl:value-of select="Backend"/>
												</xsl:attribute>
											</xsl:if>
										</input>
									</fieldset>
								</span>
								<input id="RemoteBackend" title="#lgnStraightBackendToUse#" placeholder="#lgnAddressPort#" type="text">
									<xsl:if test="Backend/@Type='Straight'">
										<xsl:attribute name="value">
											<xsl:value-of select="Backend"/>
										</xsl:attribute>
									</xsl:if>
								</input>
								<input id="ProxyfiedBackend" title="#lgnProxyBackendToUse#" placeholder="#lgnAddressPort#" type="text">
									<xsl:if test="Backend/@Type='Proxy'">
										<xsl:attribute name="value">
											<xsl:value-of select="Backend"/>
										</xsl:attribute>
									</xsl:if>
								</input>
							</span>
						</div>
					</fieldset>
					<span class="hcenter">
						<button title="#lgnConnectToBackend#" data-xdh-onevent="Connect">#lgnOK#</button>
						<button title="#lgnCancelConnection#" data-xdh-onevent="Dismiss">#lgnCancel#</button>
					</span>
				</fieldset>
			</span>
		</span>
	</xsl:template>
	<xsl:template match="Backends">
		<select title="#lgnPredefinedBackends#" id="PredefinedBackend">
			<xsl:apply-templates select="Backend"/>
		</select>
	</xsl:template>
	<xsl:template match="Backend">
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