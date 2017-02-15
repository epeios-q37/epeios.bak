const fs = require( 'fs');
const stream = require( 'stream');
const xppq = require( './xppq.js');

function test( chunk ) {
    process.stdout.write( chunk.toString().toLowerCase() );
}
readStream = new xppq.Stream( fs.createReadStream( 'test.xml' ) );
readStream.on( 'data', test );




