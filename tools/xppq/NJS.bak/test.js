const fs = require( 'fs');
const stream = require( 'stream');
const xppq = require( './xppq.js');

function write( text )
{
	process.stdout.write( text );
}
function callback( token, tag, attribute, value )
{
	switch ( token ) {
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
	}
}

function test( chunk ) {
    write( chunk.toString().toLowerCase() );
}

// new xppq.Stream( fs.createReadStream( 'test.xml' ) ).pipe( process.stdout );

// new xppq.Stream( fs.createReadStream( 'result.xml' ) ).on( 'data', test );

class Relay extends stream.Readable {
 constructor( stream, options ) {
  super( options );
  stream.on( 'data', (chunk) => { this.push( chunk ); process.stdout.write( "\n>>>>> " + chunk + " <<<<<\n" ) } );
  stream.on( 'end', () => { this.push( null ) } );
  this._read = () => {process.nextTick(() => this.emit('error', new Error( 'Gni !')));}; 
 }
}

xppq.parse( new Relay( new fs.createReadStream( 'result.xml' ) ), callback );

// xppq.parse( new xppq.Stream( fs.createReadStream( 'test.xml' ) ), callback );

// TO FIX : doesn't work because file is too big.
// xppq.parse( new xppq.Stream( fs.createReadStream( "H:/Misc/Training/Excerpts.xml" ) ), callback );

