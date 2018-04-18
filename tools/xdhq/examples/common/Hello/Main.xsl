<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns="http://www.w3.org/1999/xhtml" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<xsl:output method="html" encoding="UTF-8"/>
	<xsl:template match="/">
		<span class="vcenter-out">
			<span class="vcenter-in">
					<label>Name:</label>
					<input maxlength="20" placeholder="Enter a name here" type="text" data-xdh-onevent="input|Typing"/>
					<hr/>
					<h1>
					 <span>Hello </span>
						<span style="font-style: italic;" id="name"/>
						<span>!</span></h1>
			</span>
		</span>
	</xsl:template>
</xsl:stylesheet>