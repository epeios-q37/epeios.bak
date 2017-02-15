const xppq = require('./build/Debug/esketchnjs.node');

module.exports.test0 = ( stream, callback ) => { xppq._wrapper( 0, stream, callback ) };
module.exports.test1 = ( stream ) => { xppq._wrapper( 1, stream ) };
module.exports.test2 = ( stream ) => { xppq._wrapper( 2, stream ) };

