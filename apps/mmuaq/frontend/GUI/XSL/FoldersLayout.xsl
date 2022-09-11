<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet	version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:xpp="http://q37.info/ns/xpp/">
 <xsl:output method="html" encoding="UTF-8"/>
 <!-- Functions -->
 <xsl:template name="Folder">
  <xsl:choose>
   <xsl:when test="(/*/Content/@CurrentFolderState='Edition') and (@id=/*/Content/@CurrentFolder)">
    <span data-xdh-cast="folderCasting">
     <xsl:attribute name="data-xdh-content">
      <xsl:value-of select="@id"/>
     </xsl:attribute>
     <input type="text" id="FolderName" data-xdh-onevents="(keydown|ApplyFolder|Enter)(keydown|DiscardFolder|Esc)" data-xdh-cast="folderNameInputCast">
      <xsl:attribute name="value">
       <xsl:value-of select="@Name"/>
      </xsl:attribute>
     </input>
    </span>
   </xsl:when>
   <xsl:otherwise>
    <span data-xdh-onevents="SelectFolder|(drop|DropToFolder)" data-xdh-casts="folderCasting|DroppingCast">
     <!-- We set an id, and do let the computer generate one, to keep the same one between 2 layout generation.
     When renaming a folder, 'SelectFolder' refreshes the layout. -->
     <xsl:attribute name="id">
      <xsl:text>Folder </xsl:text>
      <xsl:value-of select="@id"/>
     </xsl:attribute>
     <xsl:attribute name="data-xdh-content">
      <xsl:value-of select="@id"/>
     </xsl:attribute>
     <xsl:choose>
      <xsl:when test="@id=/*/Corpus/Folders/@Root">
       <span>
        <xsl:text>#foldersRootFolder#</xsl:text>
       </span>
      </xsl:when>
      <xsl:when test="@id=/*/Corpus/Folders/@Inbox">
       <span style="font-style: italic;">
        <xsl:text>#foldersInboxFolder#</xsl:text>
       </span>
      </xsl:when>
      <xsl:otherwise>
       <xsl:value-of select="@Name"/>
      </xsl:otherwise>
     </xsl:choose>
    </span>
   </xsl:otherwise>
  </xsl:choose>
 </xsl:template>
 <!-- Templates -->
 <xsl:template match="/">
  <xsl:apply-templates select="*/Content"/>
  <xsl:apply-templates select="*/Corpus"/>
 </xsl:template>
 <xsl:template match="Content">
  <fieldset>
   <xsl:apply-templates select="Folders">
    <xsl:with-param name="IsRoot">yes</xsl:with-param>
   </xsl:apply-templates>
  </fieldset>
  <button title="#foldersEditFolderTitle#" data-xdh-onevent="EditFolder" data-xdh-cast="FolderRenamingCast">#foldersEditFolder#</button>
  <button title="#foldersCreateFolderTitle#" data-xdh-onevent="CreateFolder" data-xdh-cast="FolderCreationCast">#foldersCreateFolder#</button>
 </xsl:template>
 <xsl:template match="Corpus">
 </xsl:template>
 <xsl:template match="Folders">
  <xsl:param name="IsRoot">no</xsl:param>
  <ul>
   <xsl:if test="$IsRoot='yes'">
    <xsl:attribute name="class">mktree</xsl:attribute>
   </xsl:if>
   <xsl:apply-templates select="Folder"/>
   <xsl:if test="(/*/Content/@CurrentFolderState='Creation') and ( ../@id=/*/Content/@CurrentFolder)">
    <li>
     <input type="text" id="FolderName" data-xdh-onevents="(keydown|ApplyFolder|Enter)(keydown|DiscardFolder|Esc)" data-xdh-cast="folderNameInputCast"/>
    </li>
   </xsl:if>
  </ul>
 </xsl:template>
 <xsl:template match="Folder">
  <li>
   <xsl:attribute name="data-xdh-content">
    <xsl:value-of select="@id"/>
   </xsl:attribute>
   <xsl:if test="(@id!=/*/Corpus/Folders/@Inbox) and (@id!=/*/Corpus/Folders/@Root)">
    <img src="js/cursor_drag_hand.png" title="#foldersDragTitle#" style="width: 15px; height: 15px" data-xdh-onevents="(dragend|EndFolderDragging)|(dragstart|DragFolder)">
     <xsl:attribute name="data-xdh-content">
      <xsl:value-of select="@id"/>
     </xsl:attribute>
    </img>
   </xsl:if>
   <xsl:call-template name="Folder"/>
   <xsl:if test="(Folders/@Amount!=0) or (/*/Content/@CurrentFolderState='Creation' ) and ( @id=/*/Content/@CurrentFolder)">
    <xsl:apply-templates select="Folders"/>
   </xsl:if>
  </li>
 </xsl:template>
</xsl:stylesheet>