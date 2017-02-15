const fs = require( 'fs');
const stream = require( 'stream');
const xppq = require( './xppq.js');

function test( chunk ) {
    process.stdout.write( chunk.toString().toLowerCase() );
}
/*
Stream = new xppq.Stream( fs.createReadStream( 'test.xml' ) );
Stream.on( 'data', test );

xppq.parse( fs.createReadStream( 'test.xml' ), ( tag, attribute, value ) => { process.stdout.write( tag + " ; " + attribute + " ; " + value + "\n" ) } );;
*/
xppq.parse( new xppq.Stream( fs.createReadStream( 'test.xml' ) ), ( tag, attribute, value ) => { process.stdout.write( tag + " ; " + attribute + " ; " + value + "\n" ) } );;


