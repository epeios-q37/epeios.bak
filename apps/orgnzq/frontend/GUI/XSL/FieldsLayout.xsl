<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet	version="1.0"
				xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
				xmlns:xpp="http://q37.info/ns/xpp/draft"
				>
 <xsl:output method="html" encoding="UTF-8" doctype-system="about:legacy-compat"/>
 <xpp:expand href="functions.xsl"/>
 <xsl:template match="/">
  <xsl:apply-templates select="*/Layout"/>
 </xsl:template>
 <!-- Functions -->
 <xsl:template name="ColumnLabel">
  <xsl:param name="Id"/>
  <xsl:value-of select="/*/Layout/Columns/Column[@id=$Id]/Label"/>
 </xsl:template>
 <!-- End of functions -->
 <xsl:template match="Layout">
  <span class="vcenter-out">
   <span class="vcenter-in">
    <xsl:apply-templates select="Fields"/>
   </span>
  </span>
 </xsl:template>
 <xsl:template match="Fields">
  <fieldset>
   <legend>#fieldsTitle#</legend>
   <xsl:choose>
    <xsl:when test="@Amount='0' and ../@Focus!='Field'">
     <span style="font-style: italic;">#fieldsEmptyRecord#</span>
    </xsl:when>
    <xsl:otherwise>
     <xsl:apply-templates select="Field"/>
     <xsl:if test="../@Focus='Field' and not(@Selected)">
      <fieldset>
       <legend>
        <xsl:value-of select="//ColumnBuffer/Label"/>
       </legend>
       <span id="Field"/>
      </fieldset>
     </xsl:if>
    </xsl:otherwise>
   </xsl:choose>
  </fieldset>
 </xsl:template>
 <xsl:template match="Field">
  <div style="display: flex;">
   <img title="#fieldsDragAndDropTitle#" style="width: 15px; height: 15px" src="js/cursor_drag_hand.png" data-xdh-onevents="(dragstart|DragField)|(drop|DropField)|(dragend|EndFieldDragging)" data-xdh-casts="FieldDraggingCast|FieldDroppingCast">
    <xsl:attribute name="data-xdh-value">
     <xsl:value-of select="@id"/>
    </xsl:attribute>
   </img>
   <fieldset style="width: 100%;">
    <xsl:attribute name="data-xdh-value" >
     <xsl:value-of select="@id"/>
    </xsl:attribute>
    <legend data-xdh-onevent="DefineField">
     <xsl:attribute name="data-xdh-value">
      <xsl:value-of select="@id"/>
     </xsl:attribute>
     <xsl:call-template name="ColumnLabel">
      <xsl:with-param name="Id" select="@Column"/>
     </xsl:call-template>
    </legend>
    <xsl:choose>
     <xsl:when test="@id=../@Selected">
      <span id="Field"/>
     </xsl:when>
     <xsl:otherwise>
      <xsl:apply-templates select="Entries"/>
     </xsl:otherwise>
    </xsl:choose>
   </fieldset>
  </div>
 </xsl:template>
 <xsl:template match="Entries">
  <xsl:choose>
   <xsl:when test="@Amount&gt;1">
    <xsl:apply-templates select="Entry" mode="Boxed"/>
   </xsl:when>
   <xsl:otherwise>
    <xsl:apply-templates select="Entry" mode="Unboxed"/>
   </xsl:otherwise>
  </xsl:choose>
 </xsl:template>
 <xsl:template match="Entry" mode="Boxed">
  <fieldset>
   <xsl:apply-templates select="." mode="Unboxed"/>
  </fieldset>
 </xsl:template>
 <xsl:template match="Entry" mode="Unboxed">
  <span>
   <xsl:attribute name="data-xdh-content">
    <xsl:value-of select="../../@id"/>
    <xsl:text>;</xsl:text>
    <xsl:value-of select="@id"/>
   </xsl:attribute>
   <!--<xsl:call-template name="DOE">
    <xsl:with-param name="Content">
     <xsl:value-of select="."/>
    </xsl:with-param>
   </xsl:call-template>-->
  </span>
 </xsl:template>
</xsl:stylesheet>