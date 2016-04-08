﻿<!-- This file is used to transform data generated by 'getbkdapi' (a tool from the Epeios project) in a C++ header file using XSLT.
This header file contains then the API to access to the backend to which 'getbkdapi' was connected. -->

<!DOCTYPE cpp_headers [
<!ENTITY nl "&#13;&#10;">
<!ENTITY tab "&#9;">
]>

<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<xsl:output method="text"/>
	<xsl:template match="/">
		<xsl:text>/* DON'T MODIFY : computer-generated file ! */&nl;</xsl:text>
		<xsl:text>/*&nl;&tab;API from : </xsl:text>
		<xsl:value-of select="/API/Misc/Backend/Details"/>
		<xsl:text>&nl;&nl;&tab;This file was generated using </xsl:text>
		<xsl:value-of select="/API/Misc/Generator/Name"/>
		<xsl:text> </xsl:text>
		<xsl:value-of select="/API/Misc/Generator/Version"/>
		<xsl:text> (Build </xsl:text>
		<xsl:value-of select="/API/Misc/Generator/Build"/>
		<xsl:text>)&nl;*/&nl;&nl;</xsl:text>
		<xsl:text>#ifndef </xsl:text>
		<xsl:value-of select="/API/@TargetUppercased"/>
		<xsl:text>__INC&nl;</xsl:text>
		<xsl:text># define </xsl:text>
		<xsl:value-of select="/API/@TargetUppercased"/>
		<xsl:text>__INC&nl;</xsl:text>
		<xsl:text>&nl;</xsl:text>
		<xsl:text># define </xsl:text>
		<xsl:value-of select="/API/@TargetUppercased"/>
		<xsl:text>_TARGET "</xsl:text>
		<xsl:value-of select="/API/@target"/>
		<xsl:text>"&nl;</xsl:text>
		<xsl:text># define </xsl:text>
		<xsl:value-of select="/API/@TargetUppercased"/>
		<xsl:text>_API_VERSION "</xsl:text>
		<xsl:value-of select="/API/@APIVersion"/>
		<xsl:text>"&nl;</xsl:text>
		<xsl:apply-templates select="API"/>
		<xsl:text>#endif //#ifndef </xsl:text>
		<xsl:value-of select="/API/@target"/>
	  <xsl:text>__INC&nl;</xsl:text>
	</xsl:template>
	<xsl:template match="API">
	  <xsl:text>&nl;# include "fblfrd.h"&nl;</xsl:text>
	  <xsl:text># include "cpe.h"&nl;</xsl:text>
	  <xsl:text>&nl;</xsl:text>
	  <xsl:text>namespace </xsl:text>
		<xsl:value-of select="/API/@target"/>
		<xsl:text> {&nl;</xsl:text>
		<xsl:text>&nl;</xsl:text>
		<xsl:text>&tab;namespace {&nl;</xsl:text>
		<xsl:text>&tab;&tab;typedef fblfrd::frontend_depot__ _frontend_depot__;&nl;</xsl:text>
		<xsl:text>&tab;}&nl;</xsl:text>
		<xsl:text>&nl;</xsl:text>
		<!--
		<xsl:apply-templates select="Messages" mode="enum"/>
		<xsl:apply-templates select="Messages" mode="code"/>
		<xsl:apply-templates select="Messages" mode="test"/>
		-->
		<xsl:apply-templates select="Types"/>
		<xsl:text>}&nl;</xsl:text>
		<xsl:text>&nl;</xsl:text>
	</xsl:template>
	<!--
	<xsl:template match="Messages" mode="enum">
		<xsl:text>&tab;enum message__ {&nl;</xsl:text>
		<xsl:apply-templates select="Message" mode="enum"/>
		<xsl:text>&tab;&tab;m_amount,&nl;</xsl:text>
		<xsl:text>&tab;&tab;m_Undefined,&nl;</xsl:text>
		<xsl:text>&tab;};&nl;</xsl:text>
		<xsl:text>&nl;</xsl:text>
	</xsl:template>
	<xsl:template match="Message" mode="enum">
		<xsl:text>&tab;&tab;</xsl:text>
		<xsl:value-of select="@Content"/>
		<xsl:text>,&nl;</xsl:text>
	</xsl:template>
	<xsl:template match="Messages" mode="code">
		<xsl:text>&tab;inline message__ GetMessageCode( fblfrd::frontend___ &amp;Frontend )&nl;</xsl:text>
		<xsl:text>&tab;{&nl;</xsl:text>
		<xsl:text>&tab;&tab;const char *Message = Frontend.GetRawMessage();&nl;</xsl:text>
		<xsl:text>&nl;</xsl:text>
		<xsl:apply-templates select="Message" mode="code"/>
		<xsl:text>&tab;&tab;ERRb();&nl;</xsl:text>
		<xsl:text>&nl;</xsl:text>
		<xsl:text>&tab;&tab;return m_Undefined;&tab;// To avoid a warning.&nl;</xsl:text>
		<xsl:text>&tab;}&nl;</xsl:text>
		<xsl:text>&nl;</xsl:text>
	</xsl:template>
	<xsl:template match="Message" mode="code">
		<xsl:text>&tab;&tab;if ( !strcmp( Message, "</xsl:text>
		<xsl:value-of select="@Content"/>
		<xsl:text>" ) )&nl;</xsl:text>
		<xsl:text>&tab;&tab;&tab;return </xsl:text>
		<xsl:value-of select="@Content"/>
		<xsl:text>;</xsl:text>
		<xsl:text>&nl;</xsl:text>
		<xsl:text>&nl;</xsl:text>
	</xsl:template>
	<xsl:template match="Messages" mode="test">
		<xsl:text>&tab;inline void TestMessages( const fblfrd::strings_ &amp;Messages )&nl;</xsl:text>
		<xsl:text>&tab;{&nl;</xsl:text>
		<xsl:text>&tab;&tab;ctn::E_CMITEM( fblfrd::string_ ) Message;&nl;</xsl:text>
		<xsl:text>&tab;&tab;epeios::row__ Row = NONE;&nl;</xsl:text>
		<xsl:text>&nl;</xsl:text>
		<xsl:text>&tab;&tab;if ( Messages.Amount() != </xsl:text>
		<xsl:value-of select="@Amount"/>
		<xsl:text> )&nl;</xsl:text>
		<xsl:text>&tab;&tab;&tab;ERRb();&nl;</xsl:text>
		<xsl:text>&nl;</xsl:text>
		<xsl:text>&tab;&tab;Message.Init( Messages );&nl;</xsl:text>
		<xsl:text>&nl;</xsl:text>
		<xsl:text># ifdef E_USE_VC_WORKAROUND&nl;</xsl:text>
		<xsl:text>#  pragma inline_depth( 0 )	// Sinon VC fait n'importe quoi en configuration 'Release'.&nl;</xsl:text>
		<xsl:text># endif&nl;</xsl:text>
		<xsl:text>&nl;</xsl:text>
		<xsl:text>&tab;&tab;Row = Messages.First();&nl;</xsl:text>
		<xsl:text>&nl;</xsl:text>
		<xsl:text># ifdef E_USE_VC_WORKAROUND&nl;</xsl:text>
		<xsl:text>#  pragma inline_depth()&nl;</xsl:text>
		<xsl:text>#  endif&nl;</xsl:text>
		<xsl:text>&nl;</xsl:text>
		<xsl:apply-templates select="Message" mode="test"/>
		<xsl:text>&tab;&tab;if ( Row != NONE )&nl;</xsl:text>
		<xsl:text>&tab;&tab;&tab;ERRb();&nl;</xsl:text>
		<xsl:text>&tab;}&nl;</xsl:text>
		<xsl:text>&nl;</xsl:text>
	</xsl:template>
	<xsl:template match="Message" mode="test">
		<xsl:text>&tab;&tab;if ( Message( Row ) != fblfrd::string( "</xsl:text>
		<xsl:value-of select="@Content"/>
		<xsl:text>" ) )&nl;</xsl:text>
		<xsl:text>&tab;&tab;&tab;ERRb();&nl;</xsl:text>
		<xsl:text>&nl;</xsl:text>
		<xsl:text>&tab;&tab;Row = Messages.Next( Row );&nl;</xsl:text>
		<xsl:text>&nl;</xsl:text>
		<xsl:text>&nl;</xsl:text>
	</xsl:template>
	-->
	<xsl:template match="Types">
		<xsl:apply-templates select="Type" mode="common"/>
		<xsl:apply-templates select="Type" mode="object"/>
		</xsl:template>
		<xsl:template match="Type" mode="common">
		<xsl:choose>
			<xsl:when test="@Object='Master'">
				<xsl:text>&tab;class fStatics</xsl:text>
			</xsl:when>
			<xsl:otherwise>
				<xsl:text>&tab;class </xsl:text>
				<xsl:text>f</xsl:text>
				<xsl:value-of select="Prefix/@Upper"/>
				<xsl:value-of select="Name/@Raw"/>
				<xsl:text>Common</xsl:text>
			</xsl:otherwise>
		</xsl:choose>
		<xsl:text>&nl;</xsl:text>
		<xsl:text>&tab;: public _frontend_depot__&nl;</xsl:text>
		<xsl:text>&tab;{&nl;</xsl:text>
		<xsl:text>&tab;private:&nl;</xsl:text>
		<xsl:choose>
			<xsl:when test="@Object='Master'">
				<xsl:text>&tab;&tab;fblfrd::object__</xsl:text>
			</xsl:when>
			<xsl:otherwise>
				<xsl:text>&tab;&tab;fblfrd::id16__</xsl:text>
			</xsl:otherwise>
		</xsl:choose>
		<xsl:text> _ID;&nl;</xsl:text>
		<xsl:text>&tab;&tab;fblfrd::command__ _Commands[</xsl:text>
		<xsl:value-of select="Commands/@Amount"/>
		<xsl:text>];&nl;</xsl:text>
		<xsl:text>&tab;public:&nl;</xsl:text>
		<xsl:text>&tab;&tab;void reset( bso::bool__ P = true )&nl;</xsl:text>
		<xsl:text>&tab;&tab;{&nl;</xsl:text>
		<xsl:text>&tab;&tab;&tab;_frontend_depot__::reset( P );&nl;</xsl:text>
		<xsl:text>&tab;&tab;&tab;_ID = FBLFRD_UNDEFINED_OBJECT;&nl;</xsl:text>
		<xsl:text>&tab;&tab;}&nl;</xsl:text>
		<xsl:text>&tab;&tab;E_CVDTOR( </xsl:text>
		<xsl:choose>
			<xsl:when test="@Object='Master'">
				<xsl:text>fStatics</xsl:text>
			</xsl:when>
			<xsl:otherwise>
				<xsl:text>f</xsl:text>
				<xsl:value-of select="Prefix/@Upper"/>
				<xsl:value-of select="Name/@Raw"/>
				<xsl:text>Common</xsl:text>
			</xsl:otherwise>
		</xsl:choose>
		<xsl:text> )&nl;</xsl:text>
		<xsl:text>&tab;&tab;void Init( fblfrd::frontend___ &amp;Frontend )&nl;</xsl:text>
		<xsl:text>&tab;&tab;{&nl;</xsl:text>
		<xsl:text>&tab;&tab;&tab;fblfrd::commands_details CommandsDetails;&nl;</xsl:text>
		<xsl:text>&tab;&tab;&tab;fblfrd::command_detail CommandDetail;&nl;</xsl:text>
		<xsl:text>&tab;&tab;&tab;fblfrd::id16s Commands;&nl;</xsl:text>
		<xsl:text>&nl;</xsl:text>
		<xsl:text>&tab;&tab;&tab;fblfrd::id8__ Parameters[] = {&nl;</xsl:text>
		<xsl:apply-templates select="Commands" mode="data"/>
		<xsl:text>&tab;&tab;&tab;};&nl;</xsl:text>
		<xsl:text>&nl;</xsl:text>
		<xsl:text>&tab;&tab;&tab;_frontend_depot__::Init( Frontend );&nl;</xsl:text>
		<xsl:text>&nl;</xsl:text>
		<xsl:text>&tab;&tab;&tab;_ID = </xsl:text>
		<xsl:choose>
			<xsl:when test="@Object='Master'">
				<xsl:text>FBLFRD_MASTER_OBJECT;</xsl:text>
			</xsl:when>
			<xsl:otherwise>
				<xsl:text>this->Frontend().GetType( str::string( "</xsl:text>
				<xsl:value-of select="Name/@Raw"/>
				<xsl:text>" ) );&nl;</xsl:text>
			</xsl:otherwise>
		</xsl:choose>
		<xsl:text>&nl;</xsl:text>
		<xsl:text>&tab;&tab;&tab;CommandsDetails.Init();&nl;</xsl:text>
		<xsl:text>&nl;</xsl:text>
		<xsl:apply-templates select="Commands" mode="affectation"/>
		<xsl:text>&tab;&tab;&tab;Commands.Init();&nl;</xsl:text>
		<xsl:text>&tab;&tab;&tab;this->Frontend().GetCommands( </xsl:text>
		<xsl:choose>
			<xsl:when test="@Object='Master'">
				<xsl:text>FBLFRD_MASTER_TYPE</xsl:text>
			</xsl:when>
			<xsl:otherwise>
				<xsl:text>_ID</xsl:text>
			</xsl:otherwise>
		</xsl:choose>
		<xsl:text>, CommandsDetails, Commands );&nl;</xsl:text>
		<xsl:text>&tab;&tab;&tab;Commands.Recall( 0, </xsl:text>
		<xsl:value-of select="Commands/@Amount"/>
		<xsl:text>, _Commands );&nl;</xsl:text>
		<xsl:text>&tab;&tab;}&nl;</xsl:text>
		<xsl:choose>
			<xsl:when test="@Object='Master'">
				<xsl:apply-templates select="Commands" mode="function"/>
			</xsl:when>
			<xsl:otherwise>
				<xsl:text>&tab;&tab;fblfrd::object__ GetNewObject( void )&nl;</xsl:text>
				<xsl:text>&tab;&tab;{&nl;</xsl:text>
				<xsl:text>&tab;&tab;&tab;return Frontend().GetNewObject( _ID );&nl;</xsl:text>
				<xsl:text>&tab;&tab;}&nl;</xsl:text>
				<xsl:text>&tab;&tab;void RemoveObject( fblfrd::object__ Object )&nl;</xsl:text>
				<xsl:text>&tab;&tab;{&nl;</xsl:text>
				<xsl:text>&tab;&tab;&tab;Frontend().RemoveObject( Object );&nl;</xsl:text>
				<xsl:text>&tab;&tab;}&nl;</xsl:text>
			</xsl:otherwise>
		</xsl:choose>
		<xsl:text>&tab;&tab;const fblfrd::command__ *Commands( void ) const&nl;</xsl:text>
		<xsl:text>&tab;&tab;{&nl;</xsl:text>
		<xsl:text>&tab;&tab;&tab;return _Commands;&nl;</xsl:text>
		<xsl:text>&tab;&tab;}&nl;</xsl:text>
		<xsl:text>&tab;};&nl;</xsl:text>
		<xsl:text>&nl;</xsl:text>
	</xsl:template>
	<xsl:template match="Type[@Object='Master']" mode="object"/>
	<xsl:template match="Type" mode="object">
		<xsl:text>&tab;class </xsl:text>
		<xsl:text>r</xsl:text>
		<xsl:value-of select="Prefix/@Upper"/>
		<xsl:value-of select="Name/@Raw"/>
		<xsl:text>&nl;</xsl:text>
		<xsl:text>&tab;{&nl;</xsl:text>
		<xsl:text>&tab;private:&nl;</xsl:text>
		<xsl:text>&tab;&tab;fblfrd::object__ _ID;&nl;</xsl:text>
		<xsl:text>&tab;&tab;</xsl:text>
		<xsl:text>f</xsl:text>
		<xsl:value-of select="Prefix/@Upper"/>
		<xsl:value-of select="Name/@Raw"/>
		<xsl:text>Common *Common_;&nl;</xsl:text>
		<xsl:text>&tab;public:&nl;</xsl:text>
		<xsl:text>&tab;&tab;void reset( bso::bool__ P = true )&nl;</xsl:text>
		<xsl:text>&tab;&tab;{&nl;</xsl:text>
		<xsl:text>&tab;&tab;&tab;if ( P == true )&nl;</xsl:text>
		<xsl:text>&tab;&tab;&tab;&tab;if ( ( Common_ != NULL ) &amp;&amp; ( _ID != FBLFRD_UNDEFINED_OBJECT ) )&nl;</xsl:text>
		<xsl:text>&tab;&tab;&tab;&tab;&tab;Common_->RemoveObject( _ID );&nl;</xsl:text>
		<xsl:text>&nl;</xsl:text>
		<xsl:text>&tab;&tab;&tab;Common_ = NULL;&nl;</xsl:text>
		<xsl:text>&tab;&tab;&tab;_ID = FBLFRD_UNDEFINED_OBJECT;&nl;</xsl:text>
		<xsl:text>&tab;&tab;}&nl;</xsl:text>
		<xsl:text>&tab;&tab;</xsl:text>
		<xsl:text>r</xsl:text>
		<xsl:value-of select="Prefix/@Upper"/>
		<xsl:value-of select="Name/@Raw"/>
		<xsl:text>( void )&nl;</xsl:text>
		<xsl:text>&tab;&tab;{&nl;</xsl:text>
		<xsl:text>&tab;&tab;&tab;reset( false );&nl;</xsl:text>
		<xsl:text>&tab;&tab;}&nl;</xsl:text>
		<xsl:text>&tab;&tab;~</xsl:text>
		<xsl:text>r</xsl:text>
		<xsl:value-of select="Prefix/@Upper"/>
		<xsl:value-of select="Name/@Raw"/>
		<xsl:text>( void )&nl;</xsl:text>
		<xsl:text>&tab;&tab;{&nl;</xsl:text>
		<xsl:text>&tab;&tab;&tab;reset();&nl;</xsl:text>
		<xsl:text>&tab;&tab;}&nl;</xsl:text>
		<xsl:text>&tab;&tab;fblfrd::object__ ID( void ) const&nl;</xsl:text>
		<xsl:text>&tab;&tab;{&nl;</xsl:text>
		<xsl:text>&tab;&tab;&tab;return _ID;&nl;</xsl:text>
		<xsl:text>&tab;&tab;}&nl;</xsl:text>
		<xsl:text>&tab;&tab;void Init( </xsl:text>
		<xsl:text>f</xsl:text>
		<xsl:value-of select="Prefix/@Upper"/>
		<xsl:value-of select="Name/@Raw"/>
		<xsl:text>Common &amp;Common )&nl;</xsl:text>
		<xsl:text>&tab;&tab;{&nl;</xsl:text>
		<xsl:text>&tab;&tab;&tab;Common_ = &amp;Common;&nl;</xsl:text>
		<xsl:text>&nl;</xsl:text>
		<xsl:text>&tab;&tab;&tab;_ID = Common_->GetNewObject();&nl;</xsl:text>
		<xsl:text>&tab;&tab;}&nl;</xsl:text>
		<xsl:apply-templates select="Commands" mode="function">
			<xsl:with-param name="Base">Common_-></xsl:with-param>
		</xsl:apply-templates>
		<xsl:text>&tab;};&nl;</xsl:text>
		<xsl:text>&nl;</xsl:text>
	</xsl:template>
	<xsl:template match="Commands" mode="data">
		<xsl:apply-templates select="Command" mode="data"/>
	</xsl:template>
	<xsl:template match="Command" mode="data">
		<xsl:apply-templates select="Parameters" mode="data"/>
	</xsl:template>
	<xsl:template match="Parameters" mode="data">
		<xsl:text>&tab;&tab;&tab;&tab;</xsl:text>
		<xsl:apply-templates select="In/Parameter" mode="data"/>
		<xsl:text>0, </xsl:text>
		<xsl:apply-templates select="Out/Parameter" mode="data"/>
		<xsl:text>&nl;</xsl:text>
	</xsl:template>
	<xsl:template match="Parameter" mode="data">
		<xsl:value-of select="ID"/>
		<xsl:text>, </xsl:text>
	</xsl:template>
	<xsl:template match="Commands" mode="function">
		<xsl:param name="Base"/>
		<xsl:apply-templates select="Command" mode="function">
			<xsl:with-param name="Base">
				<xsl:value-of select="$Base"/>
			</xsl:with-param>
		</xsl:apply-templates>
   </xsl:template>
   <xsl:template match="Command" mode="function">
        <xsl:param name="Base"/>
		<xsl:text>&tab;&tab;fblovl::reply__ </xsl:text>
		<xsl:value-of select="Name"/>
		<xsl:text>( </xsl:text>
		<xsl:apply-templates select="Parameters" mode="function"/>
		<xsl:text>&tab;&tab;{&nl;</xsl:text>
		<xsl:text>&tab;&tab;&tab;</xsl:text>
		<xsl:value-of select="$Base"/>
		<xsl:text>Frontend().PushHeader( _ID, </xsl:text>
		<xsl:value-of select="$Base"/>
		<xsl:text>Commands()[</xsl:text>
		<xsl:value-of select="position()-1"/>
		<xsl:text>] );&nl;</xsl:text>
		<xsl:apply-templates select="Parameters" mode="instruction">
			<xsl:with-param name="Base" select="$Base"/>
	   </xsl:apply-templates>
	</xsl:template>
	<xsl:template match="Parameters" mode="function">
		<xsl:choose>
			<xsl:when test="@Amount=1">
				<xsl:text>void ) const&nl;</xsl:text>
			</xsl:when>
			<xsl:otherwise>
				<xsl:text>&nl;</xsl:text>
				<xsl:apply-templates select="In/Parameter" mode="function"/>
				<xsl:apply-templates select="Out/Parameter" mode="function"/>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>
	<xsl:template match="In/Parameter" mode="function">
		<xsl:text>&tab;&tab;&tab;</xsl:text>
		<xsl:choose>
			<xsl:when test="Name='flow'"/>
			<xsl:otherwise>
				<xsl:text>const </xsl:text>
			</xsl:otherwise>
		</xsl:choose>
		<xsl:text>fblfrd::</xsl:text>
		<xsl:value-of select="Type"/>
		<xsl:value-of select="Name"/>
		<xsl:choose>
			<xsl:when test="@Type='Static'">
				<xsl:text>__</xsl:text>
			</xsl:when>
			<xsl:when test="@Type='Bunch'">
				<xsl:text>_</xsl:text>
			</xsl:when>
			<xsl:when test="@Type='Container'">
				<xsl:text>_</xsl:text>
			</xsl:when>
			<xsl:when test="@Type='Other'">
				<xsl:choose>
					<xsl:when test="Name='flow'">
						<xsl:text>__</xsl:text>
					</xsl:when>
				</xsl:choose>
			</xsl:when>
		</xsl:choose>
		<xsl:text> &amp;In</xsl:text>
		<xsl:value-of select="@Discriminator"/>
		<xsl:choose>
			<xsl:when test="@Position='Last'">
				<xsl:text> ) const&nl;</xsl:text>
			</xsl:when>
			<xsl:otherwise>
				<xsl:text>,&nl;</xsl:text>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>
	<xsl:template match="Out/Parameter" mode="function">
		<xsl:text>&tab;&tab;&tab;fblfrd::</xsl:text>
		<xsl:value-of select="Name"/>
		<xsl:choose>
			<xsl:when test="@Type='Static'">
				<xsl:text>__</xsl:text>
			</xsl:when>
			<xsl:when test="@Type='Bunch'">
				<xsl:text>_</xsl:text>
			</xsl:when>
			<xsl:when test="@Type='Container'">
				<xsl:text>_</xsl:text>
			</xsl:when>
			<xsl:when test="@Type='Other'">
				<xsl:choose>
					<xsl:when test="Name='flow'">
						<xsl:text>__</xsl:text>
					</xsl:when>
				</xsl:choose>
			</xsl:when>
		</xsl:choose>
		<xsl:text> </xsl:text>
		<xsl:choose>
			<xsl:when test="Name='flow'">
				<xsl:text>*</xsl:text>
			</xsl:when>
		</xsl:choose>
		<xsl:text>&amp;Out</xsl:text>
		<xsl:value-of select="@Discriminator"/>
		<xsl:choose>
			<xsl:when test="@Position='Last'">
				<xsl:text> ) const&nl;</xsl:text>
			</xsl:when>
			<xsl:otherwise>
				<xsl:text>,&nl;</xsl:text>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>
	<xsl:template match="parameter" mode="function">
		<xsl:choose>
			<xsl:when test="@Position='Last'">
				<xsl:text>) const&nl;</xsl:text>
			</xsl:when>
			<xsl:otherwise>
				<xsl:text>,&nl;</xsl:text>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>
	<xsl:template match="Commands" mode="affectation">
		<xsl:apply-templates select="Command" mode="affectation"/>
		<xsl:text>&nl;</xsl:text>
	</xsl:template>
	<xsl:template match="Command" mode="affectation">
		<xsl:text>&tab;&tab;&tab;CommandDetail.Init();&nl;</xsl:text>
		<xsl:text>&tab;&tab;&tab;CommandDetail.Name = "</xsl:text>
		<xsl:value-of select="Name"/>
		<xsl:text>";;&nl;</xsl:text>
		<xsl:text>&tab;&tab;&tab;CommandDetail.Casts.Append( Parameters + </xsl:text>
		<xsl:value-of select="Parameters/@Position"/>
		<xsl:text>, </xsl:text>
		<xsl:value-of select="Parameters/@Amount"/>
		<xsl:text> );&nl;</xsl:text>
		<xsl:text>&tab;&tab;&tab;CommandsDetails.Append( CommandDetail );&nl;</xsl:text>
		<xsl:text>&nl;</xsl:text>
	</xsl:template>
	<xsl:template match="Parameters" mode="instruction">
		<xsl:param name="Base"/>
		<xsl:apply-templates select="In/Parameter" mode="instruction">
			<xsl:with-param name="Base" select="$Base"/>
		</xsl:apply-templates>
		<xsl:text>&nl;</xsl:text>
		<xsl:text>&tab;&tab;&tab;</xsl:text>
		<xsl:value-of select="$Base"/>
		<xsl:text>Frontend().EndOfInParameters();&nl;</xsl:text>
		<xsl:text>&nl;</xsl:text>
		<xsl:apply-templates select="Out/Parameter" mode="instruction">
			<xsl:with-param name="Base" select="$Base"/>
		</xsl:apply-templates>
		<xsl:text>&nl;</xsl:text>
		<xsl:text>&tab;&tab;&tab;return </xsl:text>
		<xsl:value-of select="$Base"/>
		<xsl:text>Frontend().Handle();&nl;</xsl:text>
		<xsl:text>&tab;&tab;}&nl;</xsl:text>
	</xsl:template>
	<xsl:template match="In/Parameter" mode="instruction">
		<xsl:param name="Base"/>
		<xsl:text>&tab;&tab;&tab;</xsl:text>
		<xsl:value-of select="$Base"/>
		  <xsl:text>Frontend().</xsl:text>
		<xsl:call-template name="HandleTypeCase">
			<xsl:with-param name="Name" select="Name"/>
		</xsl:call-template>
		  <xsl:text>In( In</xsl:text>
		  <xsl:value-of select="@Discriminator"/>
		  <xsl:text> );&nl;</xsl:text>
	</xsl:template>
	<xsl:template match="Out/Parameter" mode="instruction">
		<xsl:param name="Base"/>
		<xsl:text>&tab;&tab;&tab;</xsl:text>
		<xsl:value-of select="$Base"/>
		<xsl:text>Frontend().</xsl:text>
		<xsl:call-template name="HandleTypeCase">
			<xsl:with-param name="Name" select="Name"/>
		</xsl:call-template>
		<xsl:text>Out( Out</xsl:text>
		<xsl:value-of select="@Discriminator"/>
		<xsl:text> );&nl;</xsl:text>
	</xsl:template>
	<xsl:template name="HandleTypeCase">
		<xsl:param name="Name"/>
		  <xsl:choose>
			<xsl:when test="substring($Name,1,2)='xi'">XI<xsl:value-of select="substring($Name,3)"/></xsl:when>
			<xsl:when test="substring($Name,1,8)='xstrings'">XStrings<xsl:value-of select="substring($Name,9)"/></xsl:when>
			<xsl:when test="$Name='uint'">UInt</xsl:when>
			<xsl:when test="$Name='uints'">UInts</xsl:when>
			<xsl:when test="$Name='sint'">SInt</xsl:when>
			<xsl:when test="$Name='sints'">SInts</xsl:when>
			<xsl:when test="$Name='ubig'">UBig</xsl:when>
			<xsl:when test="$Name='ubigs'">UBigs</xsl:when>
			<xsl:when test="$Name='sbig'">SBig</xsl:when>
			<xsl:when test="$Name='sbigs'">SBigs</xsl:when>
			<xsl:otherwise>
				<xsl:value-of select="translate(substring($Name,1,1),'fobicsbx','FOBICSBX')"/>
				<xsl:value-of select="substring($Name,2)"/>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>
</xsl:stylesheet>
