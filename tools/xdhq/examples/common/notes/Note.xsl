<?xml version="1.0" encoding="UTF-8"?>
<!-- NO BOM !! -->
<xsl:stylesheet version="1.0" 
	xmlns="http://www.w3.org/1999/xhtml" 
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<xsl:output method="html" encoding="UTF-8"/>
	<xsl:template match="/XDHTML">
		<div>
			<article class="listing">
				<div>
					<span class="list-filter">
						<input type="text" id="Title" placeholder="Title"/>
					</span>
					<textarea id="Description">
						<xsl:attribute name="data-xdh-widget">
							<xsl:text>ckeditor|{enterMode : CKEDITOR.ENTER_BR, linkShowTargetTab: false}|val\(\)|</xsl:text>
						</xsl:attribute>
						<xsl:text>This text should not be necessary, but without it, Microsoft Edge eats above buttons (https://developer.microsoft.com/en-us/microsoft-edge/platform/issues/11041419/)</xsl:text>
					</textarea>
				</div>
				<span style="display:inline-block; width: 10px;"/>
				<div style="display: flex; justify-content: space-around;">
					<button class="button" data-xdh-onevent="Submit">Submit</button>
					<span></span>
					<button class="button" data-xdh-onevent="Cancel">Cancel</button>
				</div>
			</article>
		</div>
	</xsl:template>
</xsl:stylesheet>