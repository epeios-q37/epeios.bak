<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" 
	xmlns="http://www.w3.org/1999/xhtml" 
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
	xmlns:xpp="http://q37.info/ns/xpp/">
	<xsl:output method="html" encoding="UTF-8"/>
	<xsl:template match="/">
		<span class="vcenter-out">
			<span class="vcenter-in">
				<span data-xdh-onevent="mouseleave|HideInput">
					<fieldset>
						<div data-xdh-onevent="ShowInput">
							<text>Clicking on this text shows an input field, leaving this frame hides it.</text>
						</div>
						<div id="Input">
							<input id="Pattern" type="text" size="50" data-xdh-onevent="Submit" placeholder="Text to convert to upper case"/>
							<button title="Converts to upper case" data-xdh-onevent="Submit">Convert</button>
						</div>
					</fieldset>
				</span>
			</span>
		</span>
	</xsl:template>
</xsl:stylesheet>