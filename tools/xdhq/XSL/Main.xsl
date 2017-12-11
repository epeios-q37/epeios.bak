<?xml version="1.0" encoding="UTF-8"?>
<!-- NO BOM !! -->
<xsl:stylesheet	version="1.0" xmlns="http://www.w3.org/1999/xhtml" 	xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:xpp="http://q37.info/ns/xpp/">
	<xsl:output method="html" encoding="UTF-8"/>
	<xsl:template match="/XDHTML[@Background='Layout']">
		<span class="vcenter-out">
			<span class="vcenter-in">
				<span data-xdh-onevent="mouseleave|HideTestButton">
					<fieldset title="#mainUCConverter#">
						<legend>Un texte !!!</legend>
						<div data-xdh-onevent="ShowTestButton">
							<text>#mainHideShowTestButton#</text>
						</div>
						<div>
							<button data-xdh-onevent="Testing" data-xdh-cast="TestButtonCasting" title="#mainDisplayTestMessage#">#mainTest#</button>
						</div>
						<!-- 'data-xdh-content' empty because we don't rely on it (there is only one), but t must be present. -->
						<input id="Pattern" type="text" size="50" data-xdh-content=""/>
						<button title="#mainConvert#" data-xdh-onevent="Submission">#mainOK#</button>
					</fieldset>
				</span>
			</span>
		</span>
	</xsl:template>
</xsl:stylesheet>