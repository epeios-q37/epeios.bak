const xdhwebq = require('xdhwebq');

console.log(xdhwebq.componentInfo());
console.log(xdhwebq.wrapperInfo());

console.log(xdhwebq.returnArgument("Text from JS file in RunKit !") + '\n');

// Only displayed in 'Runkit'.
"Click on the below left little triangle(s)...";