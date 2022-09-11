<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet	version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:xpp="http://q37.info/ns/xpp/">
 <xsl:output method="html" encoding="UTF-8"/>
 <xsl:template match="/">
  <xsl:apply-templates select="*/Context"/>
 </xsl:template>
 <xsl:template match="Context">
  <xsl:choose>
   <xsl:when test="@MailDragging='InProgress' or @FolderDragging='InProgress'">
    <xdh-cast id="DroppingCast" kind="Droppable"/>
   </xsl:when>
   <xsl:otherwise>
    <xdh-cast id="DroppingCast" kind="Undroppable"/>
   </xsl:otherwise>
  </xsl:choose>
  <xdh-cast id="folderCasting">
   <xsl:attribute name="exec">
    <xsl:text>if ( getContent( element ) == '</xsl:text>
    <xsl:value-of select="@CurrentFolder"/>
    <xsl:text>') {element.style='cursor: pointer;background-color: lightgrey;'; mktreeExpandToNode( element );}</xsl:text>
    <xsl:text>else {element.style='cursor: pointer;';}</xsl:text>
   </xsl:attribute>
  </xdh-cast>
  <xdh-cast id="FolderRenamingCast">
   <xsl:attribute name="kind">
    <xsl:choose>
     <xsl:when test="(@CurrentFolder!=/*/Corpus/Folders/@Root) and (@CurrentFolder!=/*/Corpus/Folders/@Inbox) and (@CurrentFolderState='Viewing')">
      <xsl:text>Plain</xsl:text>
     </xsl:when>
     <xsl:otherwise>
      <xsl:text>Disabled</xsl:text>
     </xsl:otherwise>
    </xsl:choose>
   </xsl:attribute>
  </xdh-cast>
  <xdh-cast id="FolderCreationCast">
   <xsl:attribute name="kind">
    <xsl:choose>
     <xsl:when test="@CurrentFolderState='Viewing'">
      <xsl:text>Plain</xsl:text>
     </xsl:when>
     <xsl:otherwise>
      <xsl:text>Disabled</xsl:text>
     </xsl:otherwise>
    </xsl:choose>
   </xsl:attribute>
  </xdh-cast>
  <xdh-cast id="folderNameInputCast" exec="mktreeExpandToNode( element );"/>
 </xsl:template>
</xsl:stylesheet>