<?php
// Is used by 'php -S localhost:8080'.
// Is only used by the self-hosting (SLFH) release of the Atlas toolkit
// NOT used during development (the 'Node.js' web server is used instead).

require( "h:\\hg\\epeios\\tools\\xdhwebq\\ZND\\XDHWebQ.php");

echo XDHWebQ::serve();
?>