<?php
/*
Copyright (C) 2018 Claude SIMON (http://q37.info/contact/).

'github_html.php' is free software: you can redistribute it and/or
modify it under the terms of the GNU Affero General Public License as
published by the Free Software Foundation, either version 3 of the
License, or (at your option) any later version.

'github_html.php' is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with 'github_html.php'. If not, see <http://www.gnu.org/licenses/>.
 */

/* HTML files in a GitHub repository are not rendered, but only have their 
source be displayed.
With this script, an HTML file is retrieved from a GitHub repository an
rendered.
The file must be one of the a repository from user 'epeios-q37'
and given through the 'file' parameter. Example:
test.php?file=hangman-fr/master/LISEZMOI.html
/*

echo file_get_contents("http://raw.githubusercontent.com/epeios-q37/" . $_GET['file'] );

?>
