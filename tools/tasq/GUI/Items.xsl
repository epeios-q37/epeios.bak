<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0"
  xmlns="http://www.w3.org/1999/xhtml"
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  xmlns:xdh="http://q37.info/ns/xdh">
  <xsl:output method="html" indent="yes" />
  <xsl:attribute-set name="ItemEvents">
    <xsl:attribute name="xdh:onevent">Select</xsl:attribute>
  </xsl:attribute-set>
  <xsl:attribute-set name="Item" use-attribute-sets="ItemEvents">
    <xsl:attribute name="id">
      <xsl:value-of select="@id"></xsl:value-of>
    </xsl:attribute>
  </xsl:attribute-set>
  <xsl:template match="/">
    <xsl:apply-templates select="Items" />
  </xsl:template>
  <xsl:template match="Items">
    <ul class="item-ul" style="width: 100%;">
      <xsl:apply-templates select="Item" />
    </ul>
  </xsl:template>
  <xsl:template match="Item">
    <li style="display: flex; width: 100%; justify-content: space-between;">
      <xsl:choose>
        <xsl:when test="Items">
          <xsl:attribute name="class">item-li item-li-with-child</xsl:attribute>
          <details xdh:onevent="Select">
            <summary class="item items-summary" xsl:use-attribute-sets="Item">
              <xsl:value-of select="@Title" />
            </summary>
            <xsl:apply-templates select="Items" />
          </details>
        </xsl:when>
        <xsl:otherwise>
          <xsl:attribute name="class">item-li item-li-without-child</xsl:attribute>
          <span class="item" xsl:use-attribute-sets="Item">
            <xsl:value-of select="@Title" />
          </span>
        </xsl:otherwise>
      </xsl:choose>
      <span>🗏</span>
    </li>
  </xsl:template>
</xsl:stylesheet>