<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet	version="1.0"
                xmlns="http://www.w3.org/1999/xhtml"
				xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
				xmlns:xpp="http://q37.info/xpp"
				>
	<xsl:output method="html" encoding="UTF-8"/>
	<xsl:template match="/">
		<head>
			<xpp:expand href="styles" />
			<title>Main</title>
		</head>
		<body data-xdh-onevent="keypress|About|CS+a">
			<xsl:apply-templates select="*/Content"/>
		</body>
	</xsl:template>
	<xsl:template match="Content">
		<span class="vcenter-out">
			<span class="vcenter-in">
				<span>
					<fieldset>
						<legend>#mainFieldsLegend#</legend>
						<span id="Fields"/> 
					</fieldset>
					<fieldset data-xdh-onevents="(mouseenter|HideFacetiousButton)|(mouseleave|ShowFacetiousButton)">
						<legend>#mainFacetiousButtonLegend#</legend>
						<div>
							<text>#mainFacetiousButtonMessage#</text>
						</div>
						<div style="display: inline-bloc;widht=100%">
							<span >
								<input id="Freeze" type="checkbox"/>#mainFreeze#
							</span>
							<span data-xdh-onevent="mouseenter|HideFacetiousButton">
								<button data-xdh-onevent="Surrender" data-xdh-cast="FacetiousButtonCasting" title="#mainFacetiousButtonMessage#">#mainFacetiousButtonLabel#</button>
							</span>
						</div>
					</fieldset>
				</span>
			</span>
		</span>
	</xsl:template>
</xsl:stylesheet>