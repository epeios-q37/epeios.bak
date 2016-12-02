<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet	version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:xpp="http://q37.info/ns/xpp/">
 <xsl:output method="html" encoding="UTF-8"/>
 <!-- Fuctions -->
 <xsl:template name="Folder">
  <span data-xdh-onevent="SelectFolder" data-xdh-cast="folderCasting">
   <xsl:attribute name="data-xdh-content">
    <xsl:value-of select="@id"/>
   </xsl:attribute>
   <xsl:choose>
    <xsl:when test="@id=/*/Corpus/Folders/@Inbox">
     <span style="font-style: italic;">
      <xsl:text>#foldersInboxFolder#</xsl:text>
     </span>
    </xsl:when>
    <xsl:otherwise>
     <xsl:value-of select="@Name"/>
    </xsl:otherwise>
   </xsl:choose>
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
  <fieldset>
   <ul class="mktree">
   <li data-xdh-cast="DroppingCast" data-xdh-onevent="drop|DropToFolder">
    <xsl:attribute name="data-xdh-content">
     <xsl:value-of select="/*/Corpus/Folders/@Root"/>
    </xsl:attribute>
    <xsl:text>#foldersRootFolder#</xsl:text>
    <xsl:apply-templates select="Folders"/>
   </li>
  </ul>
  </fieldset>
  <button title="#foldersRenameFolderTitle#" data-xdh-onevent="RenameFolder">#foldersRenameFolder#</button>
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
   <xsl:if test="@id!=/*/Corpus/Folders/@Inbox">
    <img src="js/cursor_drag_hand.png" title="#foldersDragTitle#" style="width: 15px; height: 15px" data-xdh-onevents="(dragend|EndFolderDragging)|(dragstart|DragFolder)">
     <xsl:attribute name="data-xdh-content">
      <xsl:value-of select="@id"/>
     </xsl:attribute>
    </img>
   </xsl:if>
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