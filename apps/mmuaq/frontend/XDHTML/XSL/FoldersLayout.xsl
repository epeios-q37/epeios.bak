<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet	version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:xpp="http://q37.info/ns/xpp/">
 <xsl:output method="html" encoding="UTF-8"/>
 <!-- Functions -->
 <xsl:template name="FolderName">
  <xsl:choose>
   <xsl:when test="@Name!=''">
    <xsl:value-of select="@Name"/>
   </xsl:when>
   <xsl:otherwise>
    <xsl:text>#foldersInbox#</xsl:text>
   </xsl:otherwise>
  </xsl:choose>
 </xsl:template>
 <!-- Templates -->
 <xsl:template match="/">
  <xsl:apply-templates select="*/Content"/>
  <xsl:apply-templates select="*/Corpus"/>
 </xsl:template>
 <xsl:template match="Content">
  <div class="css-treeview">
   <xsl:apply-templates select="Folders"/>
  </div>
 </xsl:template>
 <xsl:template match="Corpus">
 </xsl:template>
 <xsl:template match="Folders">
  <ul>
   <xsl:apply-templates select="Folder"/>
  </ul>
 </xsl:template>
 <xsl:template match="Folder">
  <xsl:choose>
   <xsl:when test="Folders/@Amount!=0">
    <li>
     <input type="checkbox">
      <xsl:attribute name="id">
       <xsl:text>Folder-</xsl:text>
       <xsl:value-of select="@id"/>
      </xsl:attribute>
     </input>
     <label data-xdh-onevent="SelectFolder">
      <xsl:if test="@id=../@Current">
       <xsl:attribute name="style">
       <xsl:text>font-style: italic;</xsl:text>
      </xsl:attribute>
      </xsl:if>
      <xsl:attribute name="data-xdh-content">
       <xsl:value-of select="@id"/>
      </xsl:attribute>
      <xsl:call-template name="FolderName"/>
     </label>
     <xsl:apply-templates select="Folders"/>
    </li>
   </xsl:when>
   <xsl:otherwise>
    <li>
     <span style="cursor: pointer;" data-xdh-onevent="SelectFolder">
      <xsl:attribute name="data-xdh-content">
       <xsl:value-of select="@id"/>
      </xsl:attribute>
      <xsl:call-template name="FolderName"/>
     </span>
    </li>
   </xsl:otherwise>
  </xsl:choose>
 </xsl:template>
</xsl:stylesheet>