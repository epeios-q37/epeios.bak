<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet xmlns="http://www.w3.org/1999/xhtml" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  <xsl:output method="html" indent="yes" />
  <xsl:template match="/">
    <xsl:apply-templates select="Items" />
  </xsl:template>
  <xsl:template match="Items">
    <ul class="item-ul">
      <xsl:apply-templates select="Item" />
    </ul>
  </xsl:template>
  <xsl:template match="Item">
    <li>
      <xsl:choose>
        <xsl:when test="Items">
          <xsl:attribute name="class">item-li item-li-with-child</xsl:attribute>
          <details>
            <summary class="item items-summary">
              <xsl:value-of select="@Title" />
            </summary>
            <xsl:apply-templates select="Items" />
          </details>
        </xsl:when>
        <xsl:otherwise>
          <xsl:attribute name="class">item-li item-li-without-child</xsl:attribute>
          <span class="item">
            <xsl:value-of select="@Title" />
          </span>          
        </xsl:otherwise>
      </xsl:choose>
    </li>
  </xsl:template>
</xsl:stylesheet>