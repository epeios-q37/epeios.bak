<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet	version="1.0" xmlns="http://www.w3.org/1999/xhtml" 	xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:xpp="http://q37.info/ns/xpp/">
	<xsl:output method="html" encoding="UTF-8"/>
	<xsl:template match="/">
		<xsl:apply-templates select="*/Layout"/>
	</xsl:template>
	<xsl:template match="Layout">
		<div id="mapdiv" style="height: 100%"></div>
		<script>
			map = new OpenLayers.Map("mapdiv");
			map.addLayer(new OpenLayers.Layer.OSM());

			var lonLat = new OpenLayers.LonLat( -0.1279688 ,51.5077286 )
			.transform(
			new OpenLayers.Projection("EPSG:4326"), // transform from WGS 1984
			map.getProjectionObject() // to Spherical Mercator Projection
			);

			var zoom=16;

			var markers = new OpenLayers.Layer.Markers( "Markers" );
			map.addLayer(markers);

			markers.addMarker(new OpenLayers.Marker(lonLat));

			map.setCenter (lonLat, zoom);
		</script>

		<span class="vcenter-out">
			<span class="vcenter-in">
				<span data-xdh-onevent="mouseleave|HideTestButton">
					<fieldset title="#mainUCConverter#">
						<legend>#mainConverter#</legend>
						<div data-xdh-onevent="ShowTestButton">
							<text>#mainHideShowTestButton#</text>
						</div>
						<div>
							<button data-xdh-onevent="Testing" data-xdh-cast="TestButtonCasting" title="#mainDisplayTestMessage#">#mainTest#</button>
						</div>
						<!-- 'data-xdh-content' empty because we don't rely on it (there is only one), but t must be present. -->
						<input id="Pattern" type="text" size="50"/>
						<button title="#mainConvert#" data-xdh-onevent="Submission">#mainOK#</button>
					</fieldset>
				</span>
			</span>
		</span>
	</xsl:template>
</xsl:stylesheet>