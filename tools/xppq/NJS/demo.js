const fs = require( 'fs');
const stream = require( 'stream');
const xppq = require( './xppq.js');

function test( chunk ) {
    process.stdout.write( chunk.toString().toLowerCase() );
}

// new xppq.Stream( fs.createReadStream( 'test.xml' ) ).pipe( process.stdout );

 // new xppq.Stream( fs.createReadStream( 'result.xml' ) ).on( 'data', test );

xppq.parse( fs.createReadStream( 'test.xml' ), ( tag, attribute, value ) => { process.stdout.write( tag + " ; " + attribute + " ; " + value + "\n" ) } );

class Relay extends stream.Readable {
 constructor( stream, options ) {
  super( options );
  stream.on( 'data', (chunk) => { this.push( chunk ); console.log( "Yo !" );
  } );
  stream.on( 'end', () => { this.push( null ) } );
  this._read = () => {}; 
 }
}

// new xppq.Stream( new Relay( fs.createReadStream( 'test.xml' ) ) ).pipe( process.stdout );
/*
console.log( '1' );
file = fs.createReadStream( 'result.xml' );
console.log( '2' );

xpp = new xppq.Stream( file );
//xpp = file;

console.log( '3' );
relay = new Relay( xpp );
console.log( '4' );

xppq.parse( relay, ( tag, attribute, value ) => { process.stdout.write( tag + " ; " + attribute + " ; " + value + "\n" ) } );;
console.log( '5' );
*/

// xppq.parse( new xppq.Stream( fs.createReadStream( 'test.xml' ) ), ( tag, attribute, value ) => { process.stdout.write( tag + " ; " + attribute + " ; " + value + "\n" ) } );;
/*
console.log( '1' );
file.resume();
console.log( '2' );
xpp.resume();
console.log( '3' );
*/

// setTimeout( () => {}, 10000 );