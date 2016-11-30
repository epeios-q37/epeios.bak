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
 <!-- Fuctions -->
 <xsl:template name="Folder">
  <span data-xdh-onevent="SelectFolder" data-xdh-cast="folderCasting">
   <xsl:attribute name="data-xdh-content">
    <xsl:value-of select="@id"/>
   </xsl:attribute>
   <xsl:call-template name="FolderName"/>
   <!--input type="text">
    <xsl:attribute name="value">ll
     <xsl:call-template name="FolderName"/>
    </xsl:attribute>
   </input-->
  </span>
 </xsl:template>
 <!-- Templates -->
 <xsl:template match="/">
  <xsl:apply-templates select="*/Content"/>
  <xsl:apply-templates select="*/Corpus"/>
 </xsl:template>
 <xsl:template match="Content">
  <ul class="mktree" id="totot">
   <li>truc
   <xsl:apply-templates select="Folders"/>
   </li>
  </ul>
 </xsl:template>
 <xsl:template match="Corpus">
 </xsl:template>
 <xsl:template match="Folders">
  <ul>
   <xsl:apply-templates select="Folder"/>
  </ul>
 </xsl:template>
 <xsl:template match="Folder">
  <li data-xdh-cast="DroppingCast" data-xdh-onevent="drop|DropToFolder">
   <xsl:attribute name="data-xdh-content">
    <xsl:value-of select="@id"/>
   </xsl:attribute>
   <img src="js/cursor_drag_hand.png" title="#foldersDragTitle#" style="width: 15px; height: 15px" data-xdh-onevents="(dragend|EndFolderDragging)|(dragstart|DragFolder)">
    <xsl:attribute name="data-xdh-content">
     <xsl:value-of select="@id"/>
    </xsl:attribute>
   </img>
   <xsl:choose>
    <xsl:when test="Folders/@Amount!=0">
      <xsl:call-template name="Folder"/>
     <xsl:apply-templates select="Folders"/>
    </xsl:when>
    <xsl:otherwise>
     <xsl:call-template name="Folder"/>
    </xsl:otherwise>
   </xsl:choose>
  </li>
 </xsl:template>
</xsl:stylesheet>