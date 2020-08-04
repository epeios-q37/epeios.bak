<?xml version="1.0" encoding="UTF-8"?>
<!-- NO BOM !! -->
<xsl:stylesheet	version="1.0" xmlns="http://www.w3.org/1999/xhtml" 	xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:xpp="http://q37.info/ns/xpp/">
 <xsl:output method="html" encoding="UTF-8"/>
 <xsl:template match="/XDHTML[@Background='Layout']">
  <xsl:apply-templates select="Rentals"/>
 </xsl:template>
 <xsl:template match="Rentals">
  <xsl:choose>
   <xsl:when test="@Mode = 'List'">
    <ul>
     <xsl:apply-templates select="Rental" mode="List"/>
    </ul>
   </xsl:when>
   <xsl:when test="@Mode = 'Record'">
    <a data-xdh-onevent="ToList"  style="cursor: pointer;">Back to list</a>
    <xsl:apply-templates select="Rental" mode="Record"/>
   </xsl:when>
   <xsl:otherwise>
    <xsl:message>Unknown rentals mode !</xsl:message>
   </xsl:otherwise>
  </xsl:choose>
 </xsl:template>
 <xsl:template name="Details">
  <xsl:param name="style"/>
  <div class="detail owner">
   <span>
    <xsl:attribute name="style">
     <xsl:value-of select="$style"/>
    </xsl:attribute>
    <xsl:text>Owner: </xsl:text>
   </span>
   <xsl:value-of select="owner"/>
  </div>
  <div class="detail type">
   <span>
    <xsl:attribute name="style">
     <xsl:value-of select="$style"/>
    </xsl:attribute>
    <xsl:text>Type: </xsl:text>
   </span>
   <xsl:value-of select="category"/>
  </div>
  <div class="detail location">
   <span>
    <xsl:attribute name="style">
     <xsl:value-of select="$style"/>
    </xsl:attribute>
    <xsl:text>Location: </xsl:text>
   </span>
   <xsl:value-of select="city"/>
  </div>
  <div class="detail bedrooms">
   <span>
    <xsl:attribute name="style">
     <xsl:value-of select="$style"/>
    </xsl:attribute>
    <xsl:text>Number of bedrooms: </xsl:text>
   </span>
   <xsl:value-of select="bedrooms"/>
  </div>
 </xsl:template>
 <xsl:template match="Rental" mode="List">
  <li>
   <div class="ember-view">
    <article class="listing">
     <a class="image" data-xdh-onevent="HandlePicture">
      <img alt="">
       <xsl:attribute name="src">
        <xsl:value-of select="image"/>
       </xsl:attribute>
      </img>
      <small>View Larger</small>
     </a>
     <h3>
      <a class="ember-view" data-xdh-onevent="DisplayRecord" style="cursor: pointer;">
       <xsl:attribute name="data-xdh-content">
        <xsl:value-of select="@id"/>
       </xsl:attribute>
       <xsl:value-of select="title"/>
      </a>
     </h3>
     <xsl:call-template name="Details"/>
     <div class="ember-view">
      <div>
       <div class="map" style="overflow: hidden;" data-xdh-cast="MapCast" data-xdh-display-reminder="">
       <xsl:if test="/XDHTML/Rentals/@HideMaps='true'">
        <xsl:attribute name="style">display: none;</xsl:attribute>
       </xsl:if>
        <div style="height: 100%; width: 100%; position: absolute; top: 0px; left: 0px; background-color: rgb(229, 227, 223);">
         <xsl:attribute name="id">
          <xsl:value-of select="id"/>
         </xsl:attribute>
        </div>
        <script>
         <xsl:text>openMap('</xsl:text>
         <xsl:value-of select="id"/>
         <xsl:text>', </xsl:text>
         <xsl:value-of select="longitude"/>
         <xsl:text>,	</xsl:text>
         <xsl:value-of select="latitude"/>
         <xsl:text>);	</xsl:text>
        </script>
       </div>
       <div class="map" style="overflow: hidden;">
        <div style="height: 100px; width: 1000px;">
         <xsl:attribute name="data-xdh-widget_">
          <xsl:text>geomap|{center: [1, 43],zoom: 6}</xsl:text>
         </xsl:attribute>
        </div>
       </div>
      </div>
     </div>
    </article>
   </div>
  </li>
 </xsl:template>
 <xsl:template match="Rental" mode="Record">
  <div class="jumbo show-listing">
   <h2 class="title">
    <xsl:value-of select="title"/>
   </h2>
   <div class="right detail-section">
    <xsl:call-template name="Details">
     <xsl:with-param name="style">
      <xsl:text>font-weight: bold;</xsl:text>
     </xsl:with-param>
    </xsl:call-template>
    <p/>
    <p class="description">
     <xsl:value-of select="description"/>
    </p>
   </div>
   <img class="rental-pic">
    <xsl:attribute name="src">
     <xsl:value-of select="image"/>
    </xsl:attribute>
   </img>
  </div>
 </xsl:template>
</xsl:stylesheet>