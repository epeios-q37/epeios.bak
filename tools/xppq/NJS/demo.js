const fs = require( 'fs');
const stream = require( 'stream');
const xppq = require( './xppq.js');

function test( chunk ) {
    process.stdout.write( chunk.toString().toLowerCase() );
}

// new xppq.Stream( fs.createReadStream( 'test.xml' ) ).on( 'data', test );

// xppq.parse( fs.createReadStream( 'result.xml' ), ( tag, attribute, value ) => { process.stdout.write( tag + " ; " + attribute + " ; " + value + "\n" ) } );;

xppq.parse( new xppq.Stream( fs.createReadStream( 'test.xml' ) ), ( tag, attribute, value ) => { process.stdout.write( tag + " ; " + attribute + " ; " + value + "\n" ) } );;
