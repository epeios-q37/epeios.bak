<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE xcmd2ly.xsl [
<!ENTITY nl "&#13;&#10;">
<!ENTITY tab "&#9;">
<!ENTITY pad "    ">
]>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<xsl:output method="text" encoding="UTF-8"/>
	<xsl:template name="Div">
		<xsl:param name="Num"/>
		<xsl:param name="Den"/>
		<xsl:param name="Result">0</xsl:param>
		<xsl:choose>
			<xsl:when test="number($Num)>=$Den">
				<xsl:call-template name="Div">
					<xsl:with-param name="Num" select="number($Num)-number($Den)"/>
					<xsl:with-param name="Den" select="$Den"/>
					<xsl:with-param name="Result" select="number($Result)+1"/>
				</xsl:call-template>
			</xsl:when>
			<xsl:otherwise>
				<xsl:value-of select="$Result"/>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>
	<xsl:template name="Div12">
		<xsl:param name="Num"/>
		<xsl:call-template name="Div">
			<xsl:with-param name="Num" select="$Num"/>
			<xsl:with-param name="Den">12</xsl:with-param>
		</xsl:call-template>
	</xsl:template>
	<xsl:template name="Div7">
		<xsl:param name="Num"/>
		<xsl:call-template name="Div">
			<xsl:with-param name="Num" select="$Num"/>
			<xsl:with-param name="Den">7</xsl:with-param>
		</xsl:call-template>
	</xsl:template>
	<xsl:template name="Mod">
		<xsl:param name="Num"/>
		<xsl:param name="Den"/>
		<xsl:choose>
			<xsl:when test="number($Num)>=$Den">
				<xsl:call-template name="Mod">
					<xsl:with-param name="Num" select="number($Num)-$Den"/>
					<xsl:with-param name="Den" select="$Den"/>
				</xsl:call-template>
			</xsl:when>
			<xsl:otherwise>
				<xsl:value-of select="$Num"/>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>
	<xsl:template name="Mod12">
		<xsl:param name="Num"/>
		<xsl:call-template name="Mod">
			<xsl:with-param name="Num" select="$Num"/>
			<xsl:with-param name="Den">12</xsl:with-param>
		</xsl:call-template>
	</xsl:template>
	<xsl:template name="Mod7">
		<xsl:param name="Num"/>
		<xsl:call-template name="Mod">
			<xsl:with-param name="Num" select="$Num"/>
			<xsl:with-param name="Den">7</xsl:with-param>
		</xsl:call-template>
	</xsl:template>
	<xsl:template name="Calc2power">
		<xsl:param name="Value">1</xsl:param>
		<xsl:param name="Indice"/>
		<xsl:choose>
			<xsl:when test="number($Indice)=0">
				<xsl:value-of select="$Value"/>
			</xsl:when>
			<xsl:when test="number($Indice)=-2">
				<xsl:text>\breve</xsl:text>
			</xsl:when>
			<xsl:when test="number($Indice)=-3">
				<xsl:text>\longa</xsl:text>
			</xsl:when>
			<xsl:when test="number($Indice)=-4">
				<xsl:text>\maxima</xsl:text>
			</xsl:when>
			<xsl:when test="number($Indice)=0">
				<xsl:value-of select="$Value"/>
			</xsl:when>
			<xsl:when test="number($Indice)&lt;0">
				<xsl:text>???</xsl:text>
			</xsl:when>
			<xsl:otherwise>
				<xsl:call-template name="Calc2power">
					<xsl:with-param name="Value" select="number($Value)*2"/>
					<xsl:with-param name="Indice" select="number($Indice)-1"/>
				</xsl:call-template>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>
	<xsl:template name="dot">
		<xsl:param name="Amount">1</xsl:param>
		<xsl:if test="number($Amount)!=0">
			<xsl:text>.</xsl:text>
			<xsl:call-template name="dot">
				<xsl:with-param name="Amount" select="number($Amount)-1"/>
			</xsl:call-template>
		</xsl:if>
	</xsl:template>
	<xsl:template name="OctaveUpOrDown">
		<xsl:param name="Symbol"/>
		<xsl:param name="Amount"/>
		<xsl:if test="not(number($Amount)=0)">
			<xsl:value-of select="$Symbol"/>
			<xsl:call-template name="OctaveUpOrDown">
				<xsl:with-param name="Symbol" select="$Symbol"/>
				<xsl:with-param name="Amount" select="number($Amount)-1"/>
			</xsl:call-template>
		</xsl:if>
	</xsl:template>
	<xsl:template name="OctaveUp">
		<xsl:param name="Amount"/>
		<xsl:call-template name="OctaveUpOrDown">
			<xsl:with-param name="Symbol">&apos;</xsl:with-param>
			<xsl:with-param name="Amount" select="$Amount"/>
		</xsl:call-template>
	</xsl:template>
	<xsl:template name="OctaveDown">
		<xsl:param name="Amount"/>
		<xsl:call-template name="OctaveUpOrDown">
			<xsl:with-param name="Symbol">,</xsl:with-param>
			<xsl:with-param name="Amount" select="$Amount"/>
		</xsl:call-template>
	</xsl:template>
	<xsl:template match="Melody">
		<xsl:text>\relative c</xsl:text>
		<xsl:variable name="BaseO" select="number(@BaseOctave)"/>
		<xsl:variable name="C" select="number(Note/Pitch/@Chromatic)"/>
		<xsl:variable name="AbsC" select="$C*($C>=0)-$C*($C&lt;0)"/>
		<xsl:variable name="D" select="number(Note/Pitch/@Diatonic)"/>
		<xsl:variable name="AbsD" select="$D*($D>=0)-$D*($D&lt;0)"/>
		<xsl:variable name="AbsO" select="number(Note/Pitch/@Octave)"/>
    <xsl:variable name="OffO" select="$AbsO - $BaseO"/>
    <!-- '$AbsO-' is handled as a variable identifier, hence the spaces. -->
    <xsl:choose>
      <xsl:when test="$OffO&gt;0">
        <xsl:call-template name="OctaveUp">
          <xsl:with-param name="Amount" select="$OffO"/>
        </xsl:call-template>
      </xsl:when>
      <xsl:when test="$DffO&lt;0">
        <xsl:call-template name="OctaveDown">
          <xsl:with-param name="Amount" select="$OffO"/>
        </xsl:call-template>
      </xsl:when>
    </xsl:choose>
		<xsl:text> {&nl;</xsl:text>
		<xsl:apply-templates select="*"/>
		<xsl:text>&nl;}&nl;</xsl:text>
	</xsl:template>
	<xsl:template match="Anacrousis">
		<xsl:text>&pad;\partial </xsl:text>
		<xsl:call-template name="Calc2power">
			<xsl:with-param name="Indice" select="number(@Base)-1"/>
		</xsl:call-template>
		<xsl:text> * </xsl:text>
		<xsl:value-of select="@Amount"/>
		<xsl:text>&nl;</xsl:text>
	</xsl:template>
	<xsl:template match="Bar">
		<xsl:text>&nl;&pad;|</xsl:text>
	</xsl:template>
	<xsl:template match="Signature">
		<xsl:apply-templates select="Time"/>
		<xsl:apply-templates select="Key"/>
	</xsl:template>
	<xsl:template match="Time">
		<xsl:if test="@Diff='yes'">
			<xsl:text>&pad;\time </xsl:text>
			<xsl:value-of select="@Numerator"/>
			<xsl:text>/</xsl:text>
			<xsl:value-of select="@Denominator"/>
			<xsl:text>&nl;</xsl:text>
		</xsl:if>
	</xsl:template>
	<xsl:template match="Key">
		<xsl:if test="@Diff='yes'">
			<xsl:text>&pad;\key </xsl:text>
			<xsl:value-of select="@Name"/>
			<xsl:choose>
				<xsl:when test="@Accidental='Sharp'">
					<xsl:text>is</xsl:text>
				</xsl:when>
				<xsl:when test="@Accidental='Flat'">
					<xsl:text>es</xsl:text>
				</xsl:when>
			</xsl:choose>
			<xsl:text> \major</xsl:text>
			<xsl:text>&nl;&pad;</xsl:text>
		</xsl:if>
	</xsl:template>
	<xsl:template match="Tuplet">
		<xsl:text>\times </xsl:text>
		<xsl:value-of select="@Numerator"/>
		<xsl:text>/</xsl:text>
		<xsl:value-of select="@Denominator"/>
		<xsl:text> {</xsl:text>
		<xsl:apply-templates select="*"/>
		<xsl:text>} </xsl:text>
	</xsl:template>
	<xsl:template match="Rest">
		<xsl:text> r</xsl:text>
		<xsl:apply-templates select="Duration"/>
	</xsl:template>
	<xsl:template match="Note">
		<xsl:apply-templates select="Pitch"/>
		<xsl:apply-templates select="Duration"/>
	</xsl:template>
	<xsl:template match="Pitch">
		<xsl:text> </xsl:text>
		<xsl:value-of select="@Name"/>
		<xsl:choose>
			<xsl:when test="@Accidental='Sharp'">
				<xsl:text>is</xsl:text>
			</xsl:when>
			<xsl:when test="@Accidental='Flat'">
				<xsl:text>es</xsl:text>
			</xsl:when>
		</xsl:choose>
		<xsl:variable name="C" select="number(Diff/@Chromatic)"/>
		<xsl:variable name="AbsC" select="$C*($C>=0)-$C*($C&lt;0)"/>
		<xsl:variable name="D" select="number(Diff/@Diatonic)"/>
		<xsl:variable name="AbsD" select="$D*($D>=0)-$D*($D&lt;0)"/>
		<xsl:if test="$AbsD&gt;3">
			<xsl:variable name="AbsO">
				<xsl:call-template name="Div12">
					<xsl:with-param name="Num" select="$AbsD - 1"/>
				</xsl:call-template>
			</xsl:variable>
			<!--xsl:text>&nl;!!!!!!!!!!&nl;</xsl:text>
			<xsl:value-of select="$C"/>
			<xsl:text>&nl;</xsl:text>
			<xsl:value-of select="$O"/>
			<xsl:text>&nl;!!!!!!!!!!&nl;</xsl:text-->
			<xsl:variable name="OffO">
				<xsl:call-template name="Mod12">
					<xsl:with-param name="Num" select="$AbsC"/>
				</xsl:call-template>
			</xsl:variable>
			<xsl:choose>
				<xsl:when test="$D&gt;0">
					<xsl:call-template name="OctaveUp">
						<xsl:with-param name="Amount" select="number($AbsO)+1"/>
					</xsl:call-template>
				</xsl:when>
				<xsl:when test="$D&lt;0">
					<xsl:call-template name="OctaveDown">
						<xsl:with-param name="Amount" select="number($AbsO)+1"/>
					</xsl:call-template>
				</xsl:when>
			</xsl:choose>
		</xsl:if>
	</xsl:template>
	<xsl:template match="Duration">
		<xsl:if test="@Diff='yes'">
			<xsl:call-template name="Calc2power">
				<xsl:with-param name="Indice" select="number(@Base)-1"/>
			</xsl:call-template>
			<xsl:if test="@Modifier">
				<xsl:call-template name="dot">
					<xsl:with-param name="Amount" select="@Modifier"/>
				</xsl:call-template>
			</xsl:if>
		</xsl:if>
		<xsl:if test="@TiedToNext='yes'">
			<xsl:if test="not(@TiedToPrevious) or (@TiedToPrevious='no')">
				<xsl:text>(</xsl:text>
			</xsl:if>
		</xsl:if>
		<xsl:if test="@TiedToPrevious='yes'">
			<xsl:if test="not(@TiedToNext) or (@TiedToNext='no')">
				<xsl:text>)</xsl:text>
			</xsl:if>
		</xsl:if>
	</xsl:template>
</xsl:stylesheet>