<?xml version="1.0" encoding="UTF-8"?>
<!-- NO BOM !! -->
<xsl:stylesheet	version="1.0" xmlns="http://www.w3.org/1999/xhtml" 	xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:xpp="http://q37.info/ns/xpp/">
	<xsl:output method="html" encoding="UTF-8"/>
	<xsl:template match="/XDHTML[@Background='Layout']">
				<div class="ember-view">
					<article class="listing" style="display: flex; width:100%; justify-content: space-between;align-items: center;">
						<div>
							<input type="text" id="Title" placeholder="Title"/>
							<textarea id="Description">
								<xsl:attribute name="data-xdh-widget">
									<xsl:text>ckeditor|{enterMode : CKEDITOR.ENTER_BR, linkShowTargetTab: false}|val\(\)|</xsl:text>
								</xsl:attribute>
								<xsl:text>This text should not be necessary, but without it, Microsoft Edge eats following buttons (https://developer.microsoft.com/en-us/microsoft-edge/platform/issues/11041419/)</xsl:text>
							</textarea>
						</div>
						<span style="display: flex; flex-direction: column;">
							<button data-xdh-onevent="Submit">Submit</button>
							<button data-xdh-onevent="Cancel">Cancel</button>
						</span>
					</article>
				</div>
	</xsl:template>
</xsl:stylesheet>