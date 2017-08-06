const esketch = require('./eSketch.js');

console.log( esketch.wrapperInfo() );
console.log( esketch.componentInfo() ) ;

process.stdout.write( esketch.returnArgument( "Text from JS file" ) + '\n');
