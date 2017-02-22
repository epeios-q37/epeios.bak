// You can submit an additional parameter of value from '0' to '4' as id of the test to launch.

const fs = require( 'fs');
const stream = require( 'stream');
const xppq = require( './xppq.js');

function write( text )
{
	process.stdout.write( text );
}

function callback( token, tag, attribute, value )
{
	switch ( token ) {
	case xppq.tokens.START_TAG :
		write( "Start tag: '" + tag + "'\n" );
		break;
	case xppq.tokens.ATTRIBUTE :
		write( "Attribute: '" + attribute + "' = '" + value + "'\n" );
		break;
	case xppq.tokens.VALUE :
		write( "Value:     '" + value.trim() + "'\n" );
		break;
	case xppq.tokens.END_TAG :
		write( "End tag:   '" + tag + "'\n" );
		break;
	}
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
	new xppq.Stream( fs.createReadStream( 'demo.xml' ) ).on( 'data', ( chunk ) => { write( chunk.toString().toLowerCase() ) } );
	break;
case 3:
	console.log( "XML parsing WITHOUT preprocessing.\n" );
	xppq.parse( fs.createReadStream( 'demo.xml' ), callback );
	break;
case 4:
	console.log( "XML parsing WITH preprocessing.\n" );
	xppq.parse( new xppq.Stream( fs.createReadStream( 'demo.xml' ) ), callback );
	break;
default :
	console.error( "'" + arg + "' is not a valid test number ; must be '0' to '4'." );
	break;
}