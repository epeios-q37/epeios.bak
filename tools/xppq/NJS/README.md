## _xppq_ 

Fast *XML* parser and preprocessor addon for *Node.js*.

## Install

```sh
npm install xppq
```

For other platforms as *Windows*, you need *C++* development tools to be installed.

## Description

- Parses a stream containing *XML* formatted data and calls a callback for each token.
- Provides a read stream which takes another read stream an applies an [*XML* preprocessor](http://q37.info/s/t/xppq/) to it.

## Usage

```js
const xppq = require('xppq');
const fs = require('fs');

function callback( token, tag, attribute, value )
{
	switch ( token ) {
	case xppq.tokens.ERROR :
		write( ">>> ERROR:  '" + value + "'\n" );
		break;
	case xppq.tokens.START_TAG :
		write( "Start tag: '" + tag + "'\n" );
		break;
	case xppq.tokens.ATTRIBUTE :
		write( "Attribute: '" + attribute + "' = '" + value + "'\n" );
		break;
	case xppq.tokens.VALUE :
		write( "Value:     '" + value.trim() + "'\n" );
		break;
	case xppq.tokens.END_TAG :
		write( "End tag:   '" + tag + "'\n" );
		break;
	default:
		throw( "Unknown token !!!");
		break;
	}
}

const file="SomeFileContainingXMLFormattedData";

// XML parsing WITHOUT preprocessing.
xppq.parse( fs.createReadStream( file ), callback );

// XML parsing WITH preprocessing.
xppq.parse( new xppq.Stream( fs.createReadStream( file ) ).on( 'error', (err) => console.error( '>>> ERROR : ' + err ) ), callback );
```
## Error handling

For the preprocessing stream, when an error occurs, an `error` event is launched.

If an error occurs during parsing, the callback is called with the `token` parameter containing `xppq.tokens.ERROR`, and `value` containing a description of the error.

Error handling for both case is illustrated in the above section. 

## Example

Given following *XML* data :

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
node demo.js [0-4]
```

After installation, the files will be in `nodes_module/xppq` directory.

## Changelog

[https://github.com/epeios-q37/epeios/tree/master/tools/xppq/NJS/CHANGELOG.md](https://github.com/epeios-q37/epeios/tree/master/tools/xppq/NJS/CHANGELOG.md)

## Issue tracker

[https://github.com/epeios-q37/node-xppq/issues/](https://github.com/epeios-q37/node-xppq/issues/)

## License

AGPL-3.0 © [Claude Simon](http://w37.info/contact/)

[npm]: https://www.npmjs.org/package/xppq
