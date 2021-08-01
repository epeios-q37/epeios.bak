<?xml version="1.0" encoding="UTF-8"?>
<!-- NO BOM !! -->
<xsl:stylesheet version="1.0"
	xmlns="http://www.w3.org/1999/xhtml"
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
	xmlns:xdh="http://q37.info/ns/xdh">
 <xsl:output method="html" encoding="UTF-8"/>
 <xsl:template match="/XDHTML[@Background='Layout']">
  <div class="menu">
   <a class="ember-view">
    <h1>
     <em>MegaRentals</em>
    </h1>
   </a>
   <div class="links">
    <a href="http://q37.info/xdhtml/" class="ember-view">
     <xsl:text>        About    </xsl:text>
    </a>
    <a href="http://q37.info/contact/" class="ember-view">
    <xsl:text>        Contact    </xsl:text>
    </a>
   </div>
  </div>
  <div class="jumbo">
   <div class="right tomster"/>
   <h2>Welcome!</h2>
   <p>We hope you find exactly what you're looking for in a place to stay.</p>
   <a href="/about" class="button ember-view">
   <xsl:text>    About Us   </xsl:text></a>
  </div>
  <div class="list-filter ember-view">
   <div style="display: flex; width: 80%; margin: auto;">
    <input id="Pattern" placeholder="Filter By City" class="light ember-text-field ember-view" type="text" xdh:onevent="Submit"/>
    <input type="checkbox" xdh:onevent="ToggleMaps"/>
   </div>
   <div id="Rentals"/>
  </div>
 </xsl:template>
</xsl:stylesheet>