<?xml version="1.0"?>
<xsl:stylesheet version="1.0"
        xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
        xmlns:fo="http://www.w3.org/1999/XSL/Format"
>
    <xsl:template match="/">
        <xsl:apply-templates select="doc"/>
    </xsl:template>

    <xsl:template match="doc">
        <fo:root>
            <fo:layout-master-set>
                <fo:simple-page-master master-name="romp"
                        page-height="297mm"
                        page-width="21cm" margin="1in"
                >
                    <fo:region-body/>
                </fo:simple-page-master>
            </fo:layout-master-set>
            <xsl:for-each select="page">
                <fo:page-sequence master-reference="romp">
                    <fo:flow flow-name="xsl-region-body">
                        <xsl:apply-templates select="figure|h1"/>
                    </fo:flow>
                </fo:page-sequence>
            </xsl:for-each>
        </fo:root>
    </xsl:template>

    <xsl:template match="figure">
        <xsl:for-each select="img">
            <fo:block>
                <fo:external-graphic>
                    <xsl:attribute name="src">
                        <xsl:value-of select="@src"/>
                    </xsl:attribute>
                    <xsl:attribute name="content-width">
                        <xsl:value-of select="@content-width"/>
                    </xsl:attribute>
                </fo:external-graphic>
            </fo:block>
        </xsl:for-each>
    </xsl:template>

    <xsl:template match="h1">
        <fo:block font-size="24pt">
            <xsl:value-of select="."/>
        </fo:block>
    </xsl:template>

</xsl:stylesheet>


