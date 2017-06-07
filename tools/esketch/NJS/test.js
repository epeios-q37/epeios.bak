const esketch = require( './esketch.js');

console.log( esketch.wrapperInfo() );
console.log( esketch.addonInfo() ) ;

process.stdout.write( esketch.returnArgument( "Text from JS file") + '\n' );