const fs = require( 'fs');
const xppq = require( './esketch.js');

function test( chunk ) {
    process.stdout.write( chunk.toString().toLowerCase() );
}

var testFilename = 'test.txt';

readStream = fs.createReadStream( testFilename );
xppq.test0( readStream, test );

readStream = fs.createReadStream( testFilename );
xppq.test1( readStream );

readStream = fs.createReadStream( testFilename );
xppq.test2( readStream );
