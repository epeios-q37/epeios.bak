# The *XPPq* command-line tool

[![MacOS](https://github.com/epeios-q37/xppq-cli/actions/workflows/MacOS.yml/badge.svg)](https://github.com/epeios-q37/xppq-cli/actions/workflows/MacOS.yml) [![Ubuntu](https://github.com/epeios-q37/xppq-cli/actions/workflows/Ubuntu.yml/badge.svg)](https://github.com/epeios-q37/xppq-cli/actions/workflows/Ubuntu.yml) [![Windows](https://github.com/epeios-q37/xppq-cli/actions/workflows/Windows.yml/badge.svg)](https://github.com/epeios-q37/xppq-cli/actions/workflows/Windows.yml)

## Description

*XPPq* is a command-line tool which transforms an [*XML*](//q37.info/s/s3jtm3c9) file to another *XML* file, following directives inserted directly in the source *XML* file. This directives allow to handle macros, to affect value to variables and to test their values, to include files… In a glance, *XPPq* aims to be to *XML* what [*CPP*](//q37.info/s/xmpsbj77) is to *C*/*C++*.

More information about *XPPq* : <https://q37.info/s/kmpr3nzp>.

## Example

<!-- L'exemple est copié de la page dédiée sur le site epeios.q37.info -->

File `Common.xml`:
```xml
<?xml version="1.0" encoding="UTF-8"?>
<xpp:bloc xmlns:xpp="http://epeios.q37.info/ns/xpp">
  <xpp:define name="LinuxURL">
    <xpp:bloc>linux.org</xpp:bloc>
  </xpp:define>
  <xpp:define name="WindowsURL">
    <xpp:bloc>windows.com</xpp:bloc>
  </xpp:define>
  <xpp:define name="Linux">
    <xpp:bloc>
      <xpp:define name="Directory">
        <xpp:bloc>/home/dupond/</xpp:bloc>
      </xpp:define>
      <xpp:define name="RootURL">
        <xpp:expand select="LinuxURL"/>
      </xpp:define>
    </xpp:bloc>
  </xpp:define>
  <xpp:define name="Windows">
    <xpp:bloc>
      <xpp:define name="Directory">
        <xpp:bloc>c:\Documents\Dupond\</xpp:bloc>
      </xpp:define>
      <xpp:define name="RootURL">
        <xpp:expand select="WindowsURL"/>
      </xpp:define>
    </xpp:bloc>
  </xpp:define>
  <xpp:define name="File">
    <xpp:bloc>
      <xpp:expand select="Directory"/>
      <xpp:expand select="FileName"/>
    </xpp:bloc>
  </xpp:define>
  <xpp:ifeq select="OS" value="Linux">
    <xpp:expand select="Linux"/>
  </xpp:ifeq>
  <xpp:ifeq select="OS" value="Windows">
    <xpp:expand select="Windows"/>
  </xpp:ifeq>
  <SomeFile>
    <xpp:define name="FileName">
      <xpp:bloc>SomeFile</xpp:bloc>
    </xpp:define>
    <xpp:expand select="File"/>
  </SomeFile>
  <OtherFile>
    <xpp:define name="FileName">
      <xpp:bloc>OtherFile</xpp:bloc>
    </xpp:define>
    <xpp:expand select="File"/>
  </OtherFile>
  <SomeURL>
    <xpp:bloc>http://</xpp:bloc>
    <xpp:expand select="RootURL"/>
    <xpp:bloc>/something</xpp:bloc>
  </SomeURL>
</xpp:bloc>
```

File `Linux.xml`:
```xml
<?xml version="1.0" encoding="UTF-8"?>
<Configuration xmlns:xpp="http://epeios.q37.info/ns/xpp">
  <xpp:set name="OS" value="Linux"/>
  <xpp:expand href="common.xml"/>
</Configuration>
```

File `Windows.xml`
```xml
<?xml version="1.0" encoding="UTF-8"?>
<Configuration xmlns:xpp="http://epeios.q37.info/ns/xpp">
  <xpp:set name="OS" value="Windows"/>
  <xpp:expand href="common.xml"/>
</Configuration>
```

`xppq Linux.xml` outputs following:
```xml
<?xml version="1.0" encoding="UTF-8"?>
<Configuration>
	<SomeFile>/home/dupond/SomeFile</SomeFile>
	<OtherFile>/home/dupond/OtherFile</OtherFile>
	<SomeURL>http://linux.org/something</SomeURL>
</Configuration>
```

`xppq Windows.xml` outputs following:
```xml
<?xml version="1.0" encoding="UTF-8"?>
<Configuration>
	<SomeFile>c:\Documents\Dupond\SomeFile</SomeFile>
	<OtherFile>c:\Documents\Dupond\OtherFile</OtherFile>
	<SomeURL>http://windows.com/something</SomeURL>
</Configuration>
```
