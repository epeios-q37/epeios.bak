<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet	version="1.0"
                xmlns="http://www.w3.org/1999/xhtml"
				xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
				xmlns:xpp="http://q37.info/xpp"
				>
 <xsl:output method="html" encoding="UTF-8"/>
 <xsl:template match="/">
  <xsl:apply-templates select="*/Content"/>
 </xsl:template>
 <!-- Functions -->
 <xsl:template name="FolderName">
  <xsl:choose>
   <xsl:when test="@Name!=''">
    <xsl:value-of select="@Name"/>
   </xsl:when>
   <xsl:otherwise>
    <xsl:text>#mainInbox#</xsl:text>
   </xsl:otherwise>
  </xsl:choose>
 </xsl:template>
 <!-- Templates -->
 <xsl:template match="Content">
  <span class="vcenter-out">
   <span class="vcenter-in">
    <!--div>
     <ul class="tab">
      <li>
       <a>#mainMessages#</a>
      </li>
      <li>
       <a title="#mainConfigTitle#" data-xdh-onevent="Configuration">#mainConfig#</a>
      </li>
     </ul>
    </div-->
    <div class="css-treeview">
    <xsl:apply-templates select="Folders"/>
    </div>
    <xsl:apply-templates select="Mails"/>
     <button title="#mainConfigTitle#" data-xdh-onevent="Configuration">#mainConfig#</button>
   </span>
  </span>
 </xsl:template>
 <xsl:template match="Folders">
  <ul>
   <xsl:apply-templates select="Folder"/>
  </ul>
 </xsl:template>
 <xsl:template match="Mails">
  <table style="border: 1px solid black;">
   <tr>
    <th>Subject</th>
   </tr>
   <xsl:apply-templates select="Mail"/>
  </table>
 </xsl:template>
 <xsl:template match="Folder">
  <xsl:choose>
   <xsl:when test="Folders/@Amount!=0">
    <li>
     <input type="checkbox">
      <xsl:attribute name="id">
       <xsl:text>Folder-</xsl:text>
       <xsl:value-of select="@id"/>
      </xsl:attribute>
     </input>
     <label data-xdh-onevent="SelectFolder">
      <xsl:attribute name="for">
       <xsl:text>Folder-</xsl:text>
       <xsl:value-of select="@id"/>
      </xsl:attribute>
      <xsl:attribute name="data-xdh-content">
       <xsl:value-of select="@id"/>
      </xsl:attribute>
      <xsl:call-template name="FolderName"/>
     </label>
     <xsl:apply-templates select="Folders"/>
    </li>
   </xsl:when>
   <xsl:otherwise>
    <li>
     <span style="cursor: pointer;" data-xdh-onevent="SelectFolder">
      <xsl:attribute name="data-xdh-content">
       <xsl:value-of select="@id"/>
      </xsl:attribute>
      <xsl:call-template name="FolderName"/>
     </span>
    </li>
   </xsl:otherwise>
  </xsl:choose>
 </xsl:template>
 <xsl:template match="Mail">
  <tr>
   <td>
    <xsl:value-of select="@Subject"/>
   </td>
  </tr>
 </xsl:template>
</xsl:stylesheet>