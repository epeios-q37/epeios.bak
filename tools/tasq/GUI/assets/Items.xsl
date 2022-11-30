<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0"
  xmlns="http://www.w3.org/1999/xhtml"
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  xmlns:xdh="http://q37.info/ns/xdh">
  <xsl:output method="html" indent="yes" />
  <xsl:attribute-set name="ItemEvents">
    <xsl:attribute name="xdh:onevent">Select</xsl:attribute>
  </xsl:attribute-set>
  <xsl:attribute-set name="ItemWithChildren" use-attribute-sets="ItemEvents">
    <xsl:attribute name="id">
      <xsl:value-of select="@id"></xsl:value-of>
    </xsl:attribute>
    <xsl:attribute name="class">item</xsl:attribute>
  </xsl:attribute-set>
  <xsl:attribute-set name="ItemWithoutChildren" use-attribute-sets="ItemEvents">
    <xsl:attribute name="id">
      <xsl:value-of select="@id"></xsl:value-of>
    </xsl:attribute>
    <xsl:attribute name="class">tree_label item</xsl:attribute>
  </xsl:attribute-set>
  <xsl:template match="/Tasks">
    <ul class="tree">
      <xsl:choose>
        <xsl:when test="Items">
          <input type="checkbox" id="{generate-id()}"/>
          <label style="color: transparent; width: 0px;" class="tree_label" for="{generate-id()}">i</label>
          <span xsl:use-attribute-sets="ItemWithChildren" id="{@RootId}">Tasks with</span>
          <xsl:apply-templates select="Items"/>
          </xsl:when>
        <xsl:otherwise>
          <span xsl:use-attribute-sets="ItemEvents" id="{@RootId}" class="item">Tasks without</span>
        </xsl:otherwise>
      </xsl:choose>      
    </ul>
  </xsl:template>
  <xsl:template match="Items">
    <ul>
      <xsl:apply-templates select="Item" />
    </ul>
  </xsl:template>
  <xsl:template match="Item">
    <li>
      <xsl:choose>
        <xsl:when test="Items">
          <input type="checkbox" id="{generate-id()}"/>
          <label style="color: transparent; width: 0px;" class="tree_label" for="{generate-id()}">i</label>
          <span xsl:use-attribute-sets="ItemWithChildren">
            <xsl:value-of select="@Title"/>
          </span>
          <xsl:apply-templates select="Items"/>
          </xsl:when>
        <xsl:otherwise>
          <span xsl:use-attribute-sets="ItemWithoutChildren">
            <xsl:value-of select="@Title"/>
          </span>
        </xsl:otherwise>
      </xsl:choose>      
    </li>
  </xsl:template>
</xsl:stylesheet>