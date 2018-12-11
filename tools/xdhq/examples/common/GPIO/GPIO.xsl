<?xml version="1.0" encoding="UTF-8"?>
<!-- NO BOM !! -->
<xsl:stylesheet version="1.0" xmlns="http://www.w3.org/1999/xhtml" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<xsl:output method="html" encoding="UTF-8"/>
	<!-- Function -->
	<xsl:template name="GetModeLabel">
		<xsl:param name="Mode" select="@Mode"/>
		<xsl:value-of select="/XDHTML/Corpus/Modes/Mode[@id=$Mode]/@Label"/>
	</xsl:template>
	<!-- End of functions -->
	<xsl:template match="/XDHTML">
		<xsl:apply-templates select="GPIOs"/>
	</xsl:template>
	<xsl:template match="GPIOs">
		<table>
			<thead>
				<tr>
					<th/>
					<th>wPi</th>
					<th>Mode</th>
					<th>Value</th>
				</tr>
			</thead>
			<tbody>
				<xsl:apply-templates select="GPIO"/>
			</tbody>
		</table>
		<div>
			<fieldset style="text-align: center;">
				<button data-xdh-onevent="All">All</button>
				<button data-xdh-onevent="None">None</button>
				<button data-xdh-onevent="Invert">Invert</button>
			</fieldset>
			<fieldset style="text-align: center;">
				<xsl:apply-templates select="/XDHTML/Corpus/Modes/Mode" mode="Button"/>
			</fieldset>
		</div>
	</xsl:template>
	<xsl:template match="GPIO">
		<xsl:variable name="ModeLabel">
			<xsl:call-template name="GetModeLabel"/>
		</xsl:variable>
		<tr>
			<td>
				<input id="Selector.{@id}" type="checkbox" data-xdh-onevent="Toggle">
					<xsl:if test="@Selected='True'">
						<xsl:attribute name="checked">checked</xsl:attribute>
					</xsl:if>
				</input>
			</td>
			<td title="WringPi id.">
				<xsl:value-of select="@id"/>
			</td>
			<td>
				<select data-xdh-onevent="SwitchMode" id="Mode.{@id}" title="Select pin mode.">
					<xsl:apply-templates select="/XDHTML/Corpus/Modes/Mode" mode="Select">
						<xsl:with-param name="Mode" select="@Mode"/>
					</xsl:apply-templates>
				</select>
			</td>
			<td>
				<input data-xdh-onevent="ChangeValue" id="Value.{@id}" type="range" min="0" max="100" value="{@Value}" title="Set pin value, in OUT and PWM mode.">
					<xsl:choose>
						<xsl:when test="$ModeLabel='IN'">
							<xsl:attribute name="disabled">disabled</xsl:attribute>
						</xsl:when>
						<xsl:when test="$ModeLabel='OUT'">
							<xsl:attribute name="step">100</xsl:attribute>
						</xsl:when>
					</xsl:choose>
				</input>
			</td>
		</tr>
	</xsl:template>
	<xsl:template match="/XDHTML/Corpus/Modes/Mode" mode="Select">
		<xsl:param name="Mode"/>
		<option value="{@id}">
			<xsl:if test="$Mode=@id">
				<xsl:attribute name="selected">selected</xsl:attribute>
			</xsl:if>
			<xsl:value-of select="@Label"/>
		</option>
	</xsl:template>
	<xsl:template match="/XDHTML/Corpus/Modes/Mode" mode="Button">
		<button id="{@Label}" data-xdh-onevent="{@Label}">
			<xsl:value-of select="@Label"/>
		</button>
	</xsl:template>
</xsl:stylesheet>