const esketch = require('./eSketch.js');

console.log( esketch.componentInfo() ) ;
console.log( esketch.wrapperInfo() );

process.stdout.write( esketch.returnArgument( "Text from JS file" ) + '\n');
