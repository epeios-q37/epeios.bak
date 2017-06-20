console.log("\n***** TEST BEGIN *****");

const esketch = require('./esketch.js');

console.log( esketch.wrapperInfo() );
console.log( esketch.addonInfo() ) ;

process.stdout.write( esketch.returnArgument("Text from JS file") + '\n');

console.log("***** TEST END *****\n");
