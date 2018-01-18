<?php

/*
	Copyright (C) 2017 by Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of eSketch.

	eSketch is free software: you can redistribute it and/or modify it
	under the terms of the GNU Affero General Public License as published
	by the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	eSketch is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with eSketch.  If not, see <http://www.gnu.org/licenses/>.
*/

require "eSketch.php";

echo eSketch::componentInfo() . "\n" . eSketch::wrapperInfo() . "\n";

echo eSketch::returnArgument( "Argument from the PHP file !!!") . "\n";

print_r( eSketch::TestStrings( array( "Hello ", "the ", "World", "!!!"  )) );
?>