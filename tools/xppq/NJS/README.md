## _xppq_ 

XML parser and preprocessor for Node.

## Install

```sh
npm install xppq
```

## Description

- Parses a stream containing XML formatted data and calls a callback for each token.
- Provides a read stream which takes another read stream an applies an [XML preprocessor](http://q37.info/s/t/xppq/) to it.

## Usage

```js
const xppq = require('xppq');
const fs = require( 'fs');

function callback( token, tag, attribute, value )
{
	switch ( token ) {
	case xppq.tokens.START_TAG :
		process.stdout.write( "Start tag: '" + tag + "'\n" );
		break;
	case xppq.tokens.ATTRIBUTE :
		process.stdout.write( "Attribute: '" + attribute + "' = '" + value + "'\n" );
		break;
	case xppq.tokens.VALUE :
		process.stdout.write( "Value:     '" + value.trim() + "'\n" );
		break;
	case xppq.tokens.END_TAG :
		process.stdout.write( "End tag:   '" + tag + "'\n" );
		break;
	}
}

const files="SomeFileContainingXMLFormattedData";

// XML parsing WITHOUT preprocessing.
xppq.parse( fs.createReadStream( file ), callback );

// XML parsing WITH preprocessing.
xppq.parse( new xppq.Stream( fs.createReadStream( file ) ), callback );
```
## Example

Given following XML file :

```XML
<?xml version="1.0" encoding="UTF-8"?>
<SomeTag xmlns:xpp="http://q37.info/ns/xpp/" AnAttribute="SomeAttributeValue">
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue</SomeOtherTag>
 <xpp:define name="SomeMacro">
  <xpp:bloc>Some macro content !</xpp:bloc>
 </xpp:define>
 <YetAnotherTag YetAnotherAttribute="YetAnotherAttributeValue">
  <xpp:expand select="SomeMacro"/>
 </YetAnotherTag>
</SomeTag>
```

With the callback from previous section, you will obtain :

 - *without* preprocessing :

```
Start tag: 'SomeTag'
Attribute: 'AnAttribute' = 'SomeAttributeValue'
Start tag: 'SomeOtherTag'
Attribute: 'AnotherAttribute' = 'AnotherAttributeValue'
Value:     'TagValue'
End tag:   'SomeOtherTag'
Start tag: 'xpp:define'
Attribute: 'name' = 'SomeMacro'
Start tag: 'xpp:bloc'
Value:     'Some macro content !'
End tag:   'xpp:bloc'
End tag:   'xpp:define'
Start tag: 'YetAnotherTag'
Attribute: 'YetAnotherAttribute' = 'YetAnotherAttributeValue'
Start tag: 'xpp:expand'
Attribute: 'select' = 'SomeMacro'
End tag:   'xpp:expand'
End tag:   'YetAnotherTag'
End tag:   'SomeTag'
```

- *with* preprocessing :

```
Start tag: 'SomeTag'
Attribute: 'AnAttribute' = 'SomeAttributeValue'
Start tag: 'SomeOtherTag'
Attribute: 'AnotherAttribute' = 'AnotherAttributeValue'
Value:     'TagValue'
End tag:   'SomeOtherTag'
Start tag: 'YetAnotherTag'
Attribute: 'YetAnotherAttribute' = 'YetAnotherAttributeValue'
Value:     'Some macro content !'
End tag:   'YetAnotherTag'
End tag:   'SomeTag'
```


## Test

```sh
node demo.js
```
## License

AGPL-3.0 © [Claude Simon](http://w37.info/contact/)

[npm]: https://www.npmjs.org/package/xppq
