/*
	Copyright (C) 2007-2017 Claude SIMON (http://q37.info/contact/).

	This file is part of NJSq.

	NJSq is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

	NJSq is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with NJSq. If not, see <http://www.gnu.org/licenses/>.
*/

// Once installed ('npm install njsq'), launch ''npm explore njsq -- node test.js'.

const njsq = require( './NJSq.js');

console.log( njsq._wrapperInfo() );