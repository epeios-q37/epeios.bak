<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet	version="1.0" xmlns="http://www.w3.org/1999/xhtml" 	xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:xpp="http://q37.info/ns/xpp/">
	<xsl:output method="html" encoding="UTF-8"/>
	<xsl:template match="/">
		<xsl:apply-templates select="*/Layout"/>
	</xsl:template>
	<xsl:template match="Layout">
		<span class="vcenter-out">
			<span class="vcenter-in">
				<span data-xdh-onevent="mouseleave|HideTestButton">
					<fieldset title="#mainUCConverter#">
						<legend>#mainConverter#</legend>
						<div data-xdh-onevent="ShowTestButton">
							<text>#mainHideShowTestButton#</text>
						</div>
						<div>
							<button data-xdh-onevent="Testing" data-xdh-cast="TestButtonCasting" title="#mainDisplayTestMessage#">#mainTest#</button>
						</div>
						<input id="Pattern" type="text" size="50"/>
						<button title="#mainConvert#" data-xdh-onevent="Submission">#mainOK#</button>
					</fieldset>
				</span>
			</span>
		</span>
	</xsl:template>
</xsl:stylesheet>