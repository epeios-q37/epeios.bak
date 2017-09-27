const rncq = require('rncq');

const exprALG = "784512.8456120 * 8456120.48651320 - 84651320.896451320 / 84651230.89645120 * 86451320.798465123 / 84651320.789465120 * 12365478985245600000000";
const exprRPN = "784512.8456120 8456120.48651320 * 84651320.896451320 - 84651230.89645120 / 86451320.798465123 * 84651320.789465120 / 12365478985245600000000 *";

// 'true' : 'exprALG' is used, which  must be algebraic ('1 + 2 * 3').
// 'false' : 'exprRPN' is used, which must be RPN ('1 2 + 3 *').
if ( true )
    rncq.evaluateALG(exprALG, (result) => console.log(result));
else
    rncq.evaluateRPN(exprRPN, (result) => console.log(result));

// Only displayed in 'Runkit'.
"Click on the below left little triangle(s)...";
