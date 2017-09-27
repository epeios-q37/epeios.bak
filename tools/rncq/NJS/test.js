/*
	opyright (C) 2007-2017 Claude SIMON (http://q37.info/contact/).
	This file is part of RNCq.

	RNCq is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

	RNCq is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with RNCq. If not, see <http://www.gnu.org/licenses/>.
*/

// Once installed ('npm install rncq'), launch 'npm explore rncq -- node test.js'.

const rncq = require('./RNCq.js');
const exprALG = "784512.8456120 * 8456120.48651320 - 84651320.896451320 / 84651230.89645120 * 86451320.798465123 / 84651320.789465120 * 12365478985245600000000";
const exprRPN = "784512.8456120 8456120.48651320 * 84651320.896451320 - 84651230.89645120 / 86451320.798465123 * 84651320.789465120 / 12365478985245600000000 *";
var expr = process.argv[2];

console.log( rncq.componentInfo() ) ;
console.log( rncq.wrapperInfo() );

const arg = process.argv[2];

process.stdout.write(rncq.returnArgument("Text from JS file") + '\n');

// 'true' : 'expr' must be algebraic ('1 + 2 * 3').
// 'false' : 'expr' must be RPN ('1 2 + 3 *').
if ( false ) {
    if (expr == undefined)
        expr = exprALG;
    rncq.evaluateALG(expr, (result) => process.stdout.write(result));
} else {
    if (expr == undefined)
        expr = exprRPN;
    rncq.evaluateRPN(expr, (result) => process.stdout.write(result));
}
