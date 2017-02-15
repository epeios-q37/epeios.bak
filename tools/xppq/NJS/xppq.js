const xppq = require('./build/Debug/xppqnjs.node');
const stream = require('stream');

class Stream extends stream.Readable {
 constructor( stream, options ) {
  super( options );
  xppq._wrapper( 0, stream, this, Buffer );
 }
}

module.exports.Stream = Stream;
