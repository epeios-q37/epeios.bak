/*
MIT License

Copyright (c) 2018 Claude SIMON (https://q37.info/s/rmnmqd49)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

/* Some logging facilities */

Object.defineProperty(global, '__stack', {
	get: function() {
		var orig = Error.prepareStackTrace;
		Error.prepareStackTrace = function(_, stack) {
			return stack;
		};
		var err = new Error;
		Error.captureStackTrace(err, arguments.callee);
		var stack = err.stack;
		Error.prepareStackTrace = orig;
		return stack;
	}
});

Object.defineProperty(global, '__line', {
	get: function() {
		return __stack[2].getLineNumber();
	}
});

Object.defineProperty(global, '__function', {
	get: function() {
		return __stack[2].getFunctionName();
	}
});

function LOG() {
	console.log( "--> " + __function + ": " +  __line + " (" + __filename + ")" );
}
/**/

const xdhq = require('./XDHq.js');

console.log(xdhq.componentInfo());
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

function onConnection() {
	LOG();
	console.log("Connection detected !");

	return new MyData();
}

function push(rental,id,tree) {
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
	LOG();
	var value = "";
	
	if ( xdh.hideMaps == "false" )
		value = "Plain";
	else
		value = "Hidden";
	
	xdh.setCastByTag( "Rentals", "MapCast", value );
}

function displayList( xdh ) {
	LOG();
	var tree = new xdhq.LayoutTree();
	var i = rentals.length;
	
	tree.pushTag("Rentals");
	tree.putAttribute("Mode", "List");
	tree.putAttribute("HideMaps", xdh.hideMaps);
	
	while ( i-- ) {
		if ( rentals[i]['city'].toLowerCase().startsWith( xdh.pattern ) )
			push(rentals[i],i,tree);
	}
	
	tree.popTag();
	
	xdh.setLayout( "Rentals", tree, "Rentals.xsl", () => { handleMapsCast( xdh ); } );
}

function displayRecord( xdh, id ) {
	LOG();
	var tree = new xdhq.LayoutTree();
	
	tree.pushTag("Rentals");
	tree.putAttribute("Mode", "Record" );
	push(rentals[id],id,tree);
	tree.popTag();
	
	xdh.setLayout( "Rentals", tree, "Rentals.xsl" );
}

function acConnect( xdh, id ) {
	LOG();
	var tree = new xdhq.LayoutTree();
	
	xdh.setLayout( "", tree, "Main.xsl", () => { displayList( xdh ); });
}

function handleImage( xdh, result ) {
	LOG();
	if (result == 'image')
		xdh.putAttribute( xdh.imageToHandle, 'class', 'image wide' );
	else
		xdh.putAttribute( xdh.imageToHandle, 'class', 'image' );
}

function acHandlePicture( xdh, id ) {
	LOG();
	xdh.imageToHandle = id;
	xdh.getAttribute( id, 'class', ( result ) => handleImage( xdh, result ) );
}

function handleRecord( xdh, result ) {
	LOG();
	displayRecord(xdh, parseInt(result));
}

function acDisplayRecord( xdh, id ) {
	xdh.getContent( id, (result) => handleRecord( xdh, result ) );
}

function acToList( xdh, id ) {
	LOG();
	displayList(xdh);
}

function acSubmit( xdh, id ) {
	LOG();
	xdh.getContent("Pattern", (result) => { xdh.pattern = result.toLowerCase(); displayList(xdh); });
}

function acToggleMaps( xdh, id ) {
	LOG();
	xdh.getContent(id, (result) => { xdh.hideMaps = result; handleMapsCast(xdh); });
}
	
function main()
{
	LOG();
	xdhq.register([
		["Connect", acConnect],
		["HandlePicture", acHandlePicture],
		["DisplayRecord", acDisplayRecord],
		["ToList", acToList],
		["Submit", acSubmit],
		["ToggleMaps", acToggleMaps],
	]);

	xdhq.launch( onConnection, "rentals", "Connect" );
}

main();
