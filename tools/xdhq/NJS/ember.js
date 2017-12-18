/*
	Copyright (C) 2007-2017 Claude SIMON (http://q37.info/contact/).

	This file is part of XDHq.

	XDHq is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

	XDHq is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with XDHq. If not, see <http://www.gnu.org/licenses/>.
*/

// Once installed ('npm install xdhq'), launch 'npm explore xdhq -- node test.js'.

const xdhq = require('./XDHq.js');

console.log( xdhq.componentInfo() ) ;
console.log( xdhq.wrapperInfo() );

// process.stdout.write(xdhq.returnArgument("Text from JS file") + '\n');

var rentals = [{
      id: 'grand-old-mansion',
      title: 'Grand Old Mansion',
      owner: 'Veruca Salt',
      city: 'San Francisco',
      category: 'Estate',
      bedrooms: 15,
      image: 'https://upload.wikimedia.org/wikipedia/commons/c/cb/Crane_estate_(5).jpg',
      description: "This grand old mansion sits on over 100 acres of rolling hills and dense redwood forests.",
	  latitude: 37.7175,
	  longitude: -121.979
    }, {
      id: 'urban-living',
      title: 'Urban Living',
      owner: 'Mike TV',
      city: 'Seattle',
      category: 'Condo',
      bedrooms: 1,
      image: 'https://upload.wikimedia.org/wikipedia/commons/0/0e/Alfonso_13_Highrise_Tegucigalpa.jpg',
      description: "A commuters dream. This rental is within walking distance of 2 bus stops and the Metro.",
	  latitude: 47.5459,
	  longitude: -121.881
    }, {
      id: 'la-plage-digitale',
      title: 'La Plage Digitale',
      owner: 'Alsace Digitale',
      city: 'Strasbourg',
      category: 'Coworking',
      bedrooms: 0,
      image: 'https://upload.wikimedia.org/wikipedia/commons/thumb/2/27/La_Plage_digitale_3.jpg/800px-La_Plage_digitale_3.jpg',
      description: "Let's work together! Drop me a line.",
	  latitude: 47.5459,
	  longitude: -121.881
    }, {
      id: 'downtown-charm',
      title: 'Downtown Charm',
      owner: 'Violet Beauregarde',
      city: 'Portland',
      category: 'Apartment',
      bedrooms: 3,
      image: 'https://upload.wikimedia.org/wikipedia/commons/f/f7/Wheeldon_Apartment_Building_-_Portland_Oregon.jpg',
      description: "Convenience is at your doorstep with this charming downtown rental. Great restaurants and active night life are within a few feet.",
	  latitude: 45.4611,
	  longitude:  -122.6692
    }];

class MyData extends xdhq.XDH {
	constructor() {
		super();
		this.timestamp = new Date();
		this.pattern = "";
		this.hideMaps = "false";
	}
}

function nop( xdh ) {
	console.log( arguments.callee );
}

function onConnection() {
	console.log( arguments.callee );
	console.log("Connection detected !");

	return new MyData();
}

function push(rental,id,tree) {
	console.log( arguments.callee );
	tree.pushTag('Rental');
	tree.putAttribute('id',id);
	for (var prop in rental) {
		tree.pushTag(prop);
		tree.putValue(rental[prop]);
		tree.popTag();
	}
	
	tree.popTag();
}

function handleMapsCast( xdh ) {
	console.log( arguments.callee );
	var value = "";
	
	if ( xdh.hideMaps == "false" )
		value = "Plain";
	else
		value = "Hidden";
	
	xdh.setCasts( "Rentals", "MapCast", value, nop );
}

function displayList( xdh ) {
	console.log( arguments.callee );
	var tree = new xdhq.LayoutTree();
	var i = rentals.length;
	
	tree.pushTag("Rentals");
	tree.putAttribute("Mode", "List" );
	
	while ( i-- ) {
		if ( rentals[i]['city'].toLowerCase().startsWith( xdh.pattern ) )
			push(rentals[i],i,tree);
	}
	
	tree.popTag();
	
	xdh.setLayout( "Rentals", tree, "ember/Rentals.xsl", (xdh) => { xdh.dressWidgets( "Rentals", handleMapsCast ); } );
}

function displayRecord( xdh, id ) {
	console.log( arguments.callee );
	var tree = new xdhq.LayoutTree();
	
	tree.pushTag("Rentals");
	tree.putAttribute("Mode", "Record" );
	push(rentals[id],id,tree);
	tree.popTag();
	
	xdh.setLayout( "Rentals", tree, "ember/Rentals.xsl", nop );
}

function acConnect( xdh, id ) {
	console.log( arguments.callee );
	var tree = new xdhq.LayoutTree();
	
	xdh.setLayout( "Root", tree, "ember/Main.xsl", (xdh) => { displayList( xdh ); });
}

function handleImage( xdh, result ) {
	console.log( arguments.callee );
	if ( result == 'image' )
		xdh.setAttribute( xdh.imageToHandle, 'class', 'image wide', nop );
	else
		xdh.setAttribute( xdh.imageToHandle, 'class', 'image', nop );
}

function acHandlePicture( xdh, id ) {
	console.log( arguments.callee );
	xdh.imageToHandle = id;
	xdh.getAttribute( id, 'class', handleImage );
}

function handleRecord( xdh, result ) {
	console.log( arguments.callee );
	displayRecord( xdh, parseInt( result ) );
}

function acDisplayRecord( xdh, id ) {
	xdh.getContent( id, handleRecord );
}

function acToList( xdh, id ) {
	console.log( arguments.callee );
	displayList( xdh );
}

function acSubmit( xdh, id ) {
	console.log( arguments.callee );
	xdh.getContent( "Pattern", (xdh,result) => { xdh.pattern = result.toLowerCase(); displayList( xdh );});
}

function acToggleMaps( xdh, id ) {
	console.log( arguments.callee );
	xdh.getContent( id, ( xdh, result ) => { xdh.hideMaps = result; handleMapsCast( xdh ); } );
}
	
function main()
{
	console.log( arguments.callee );
	xdhq.register("Connect", acConnect);	// Testing.
	xdhq.register([
		["HandlePicture", acHandlePicture],
		["DisplayRecord", acDisplayRecord],
		["ToList", acToList],
		["Submit", acSubmit],
		["ToggleMaps", acToggleMaps],
	]);

	xdhq.listen(onConnection, "12345");
}

main();
