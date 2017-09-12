/*
	Copyright (C) 2007-2017 Claude SIMON (http://q37.info/contact/).

	This file is part of eSketch.

	eSketch is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

	eSketch is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with eSketch. If not, see <http://www.gnu.org/licenses/>.
*/

// Once installed ('npm install esketch'), launch 'npm explore esketch -- node test.js'.

const esketch = require('./eSketch.js');

console.log( esketch.componentInfo() ) ;
console.log( esketch.wrapperInfo() );

process.stdout.write( esketch.returnArgument( "Text from JS file" ) + '\n');
