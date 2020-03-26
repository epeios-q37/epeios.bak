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
							<button data-xdh-onevent="ConnectedTesting" id="TestButton" title="#mainDisplayTestMessage#">#mainTest#</button>
						</div>
						<!-- 'data-xdh-content' empty because we don't rely on it (there is only one), but must be present. -->
						<input id="Pattern" type="text" size="50" data-xdh-onevent="Submission"/>
						<button title="#mainConvert#" data-xdh-onevent="Submission">#mainOK#</button>
						<div>
							<button data-xdh-onevent="UnconnectedTesting" title="#mainDisplayUnconnectedMessage#">#mainUnconnectedTest#</button>
						</div>
					</fieldset>
				</span>
			</span>
		</span>
	</xsl:template>
</xsl:stylesheet>
