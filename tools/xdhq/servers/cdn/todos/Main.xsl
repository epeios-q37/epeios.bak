<?xml version="1.0" encoding="UTF-8"?>
<!-- NO BOM !! -->
<xsl:stylesheet	version="1.0" xmlns="http://www.w3.org/1999/xhtml" 	xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:xpp="http://q37.info/ns/xpp/">
 <xsl:output method="html" encoding="UTF-8"/>
 <xsl:template match="/XDHTML[@Background='Layout']">
  <div class="menu">
   <a class="ember-view">
    <h1>
     <em>Todos</em>
    </h1>
   </a>
  </div>
  <div class="jumbo">
   <div class="right tomster"/>
   <h2>Welcome!</h2>
   <p>This is an example of use of the XDHTML technology !</p>
   <a href="/about" class="button ember-view">
   <xsl:text>    About Us   </xsl:text></a>
  </div>
  <div class="list-filter ember-view">
   <div style="display: flex; width: 80%; margin: auto;">
    <input id="Pattern"  data-xdh-cast="EditionCast" placeholder="Filter By Title" class="light ember-text-field ember-view" type="text" data-xdh-onevent="Search"/>
    <input type="checkbox" data-xdh-cast="EditionCast" data-xdh-onevent="ToggleDescriptions"/>
				<button data-xdh-value="0" data-xdh-onevent="Edit" data-xdh-cast="EditionCast">Create</button>
   </div>
   <div id="Todos"/>
  </div>
 </xsl:template>
</xsl:stylesheet>