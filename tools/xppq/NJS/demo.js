const fs = require( 'fs');
const stream = require( 'stream');
const xppq = require( './xppq.js');

function test( chunk ) {
    process.stdout.write( chunk.toString().toLowerCase() );
}

// new xppq.Stream( fs.createReadStream( 'test.xml' ) ).pipe( process.stdout );

// new xppq.Stream( fs.createReadStream( 'result.xml' ) ).on( 'data', test );

class Relay extends stream.Readable {
 constructor( stream, options ) {
  super( options );
  stream.on( 'data', (chunk) => { this.push( chunk ); process.stdout.write( "\n>>>>> " + chunk + " <<<<<\n" ) } );
  stream.on( 'end', () => { this.push( null ) } );
  this._read = () => {}; 
 }
}

xppq.parse( new Relay( fs.createReadStream( 'result.xml' ) ), ( tag, attribute, value ) => { process.stdout.write( tag + " ; " + attribute + " ; " + value + "\n" ) } );

// xppq.parse( new Relay( new xppq.Stream( fs.createReadStream( 'test.xml' ) ) ), ( tag, attribute, value ) => { "!" + process.stdout.write( tag + " ; " + attribute + " ; '" + value.trim() + "'\n" ) } );
