/*
	'egeckocom.js' by Claude SIMON (http://zeusw.org/).

	 This file is part of 'eSketch' software.

    'eSketch' is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'eSketch' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with 'eSketch'.  If not, see <http://www.gnu.org/licenses/>.
*/

// $Id: egeckocom.js,v 1.1 2012/01/27 16:29:31 csimon Exp $


/*
try {
	netscape.security.PrivilegeManager.enablePrivilege("UniversalXPConnect");
	netscape.security.PrivilegeManager.enablePrivilege("UniversalBrowserWrite");
} catch (err) {
	alert(err);
}
*/
try {
	 egeckocom = Components.classes["@zeusw.org/egeckocom;5"].createInstance().QueryInterface(Components.interfaces.EIGeckoCOM);
} catch (err) {
	alert(err);
	window.close();
}

function create ( componentId, language )
 {
	var message = new Object;
	
	try {
		egeckocom.Create( componentId, language, message );
		
		if ( message.value ) {
			alert( message.value );
			window.close();
		}
			
	} catch ( err ) {
		alert( err );
	}
}


function retrieve ( componentId )
 {
	var message = new String();
	
	try {
		egeckocom.Retrieve( componentId, message );
		
		if ( message.value )
			alert( message.value );
			
	} catch ( err ) {
		alert( err );
	}
}

function register ()
 {
	var message = new String();
 
	try {
		egeckocom.Register( window, message );
		
		if ( message.value ) {
			alert( message.value );
			window.close();
		}
			
	} catch ( err ) {
		alert( err );
	}
}
 
function stop ()
 {
	var message = new String();

	try {
		commandLine = window.arguments[0].QueryInterface( Components.interfaces.nsICommandLine );
	} catch ( err ){
		commandLine = null;
	}

	try {
		egeckocom.Stop( commandLine, message );

		if ( message.value ) {
			alert( message.value );
			window.close();
		}
	} catch ( err ) {
		alert( err );
	}
}

function terminate()
 {
	var message = new String();
	
	try {
		egeckocom.Terminate( message );
		
		if ( message.value ) {
			alert( message.value );
			window.close();
		}
			
	} catch ( err ) {
		alert( err );
	}
}

function test()
 {
	var message = new String();
	
	try {
		egeckocom.Test( message );
		
		if ( message.value ) {
			alert( message.value );
			window.close();
		}
			
	} catch ( err ) {
		alert( err );
	}
}

function handlePseudoEvent( element, param )
 {
	var message = new String();
	
	try {
		egeckocom.HandlePseudoEvent( element, param, message );
		
		if ( message.value ) {
			alert( message.value );
			window.close();
		}
			
	} catch ( err ) {
		alert( err );
	}
}