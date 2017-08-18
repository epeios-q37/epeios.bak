// You can submit an additional parameter of value from '0' to '4' as id of the test to launch.

"use strict"

const fs = require( 'fs');
const stream = require( 'stream');
const xppq = require( __dirname + '/xppq.js');

function write( text )
{
	process.stdout.write( text );
}

function callback( token, tag, attribute, value )
{
	switch ( token ) {
	case xppq.tokens.ERROR :
		write( ">>> ERROR:  '" + value + "'\n" );
		break;
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
	default:
		throw( "Unknown token !!!");
		break;
	}
}

var test = 4;

var arg = process.argv[2];

if ( arg != undefined )
	test = Number( arg );

// console.log( xppq.info() );
console.log( xppq.extendedInfo() );

const file = __dirname + '/demo.xml';

switch ( test ) {
case 0:
	console.log( "No treatment ; to see the original file.\n" );
	fs.createReadStream( file ).pipe( process.stdout );
	break;
case 1:
	console.log( "Piping the preprocessing stream.\n" );
	new xppq.Stream( fs.createReadStream( file ) ).on( 'error', (err) => console.error( '\n>>> ERROR : ' + err + '\n') ).pipe( process.stdout );
	break;
case 2:
	console.log( "Using the preprocessing stream with a callback, wich transforms to lower case.\n" );
	new xppq.Stream( fs.createReadStream( file ) ).on( 'data', ( chunk ) => write( chunk.toString().toLowerCase() ) ).on( 'error', (err) => console.error( '\n>>> ERROR : ' + err + '\n') );
	break;
case 3:
	console.log( "XML parsing WITHOUT preprocessing.\n" );
	xppq.parse( fs.createReadStream( file ), callback );
	break;
case 4:
	console.log( "XML parsing WITH preprocessing.\n" );
	xppq.parse( new xppq.Stream( fs.createReadStream( file ) ).on( 'error', (err) => console.error( '>>> ERROR : ' + err ) ), callback );
	break;
default :
	console.error( "'" + arg + "' is not a valid test id ; must be '0' to '4'." );
	break;
}