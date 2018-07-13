const net = require('net');

const server = net.createServer((c) => {
  // 'connection' listener
		c.on('data', function(chunk) {console.log( ">" + chunk.toString() + "<" ) });
  console.log('client connected',c);
/*c.on('readable', function() {
  // there is some data to read now
  let data;

  while (data = this.read()) {
    console.log(">" + data + "<" );
  }
});
*/

  c.write('hello\r\n');
  c.pipe(c);
});
server.on('error', (err) => {
  throw err;
});
server.listen(53752, () => {
  console.log('server bound');
});
