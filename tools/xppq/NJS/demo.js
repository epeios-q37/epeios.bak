const fs = require( 'fs');
const stream = require( 'stream');
const xppq = require( './xppq.js');

function callback( tag, attribute, value )
{
	process.stdout.write( tag + " - " + attribute + " : '" + value.trim() + "'\n" )
}

var test = 4;

var arg = process.argv[2];

if ( arg != undefined )
	test = Number( arg );

switch ( test ) {
case 0:
	console.log( "No treatment ; to see the original file.\n" );
	fs.createReadStream( 'demo.xml' ).pipe( process.stdout );
	break;
case 1:
	console.log( "Piping the preprocessing stream.\n" );
	new xppq.Stream( fs.createReadStream( 'demo.xml' ) ).pipe( process.stdout );
	break;
case 2:
	console.log( "Using the preprocessing stream with a callback, wich transforms to lower case.\n" );
	new xppq.Stream( fs.createReadStream( 'demo.xml' ) ).on( 'data', ( chunk ) => { process.stdout.write( chunk.toString().toLowerCase() ) } );
	break;
case 3:
	console.log( "XML parsing without preprocessing.\n" );
	xppq.parse( fs.createReadStream( 'demo.xml' ), callback );
	break;
case 4:
	console.log( "XML parsing with preprocessing.\n" );
	xppq.parse( new xppq.Stream( fs.createReadStream( 'demo.xml' ) ), callback );
	break;
default :
	console.error( "'" + arg + "' is not a valid test number ; must be '0' to '4'." );
	break;
}