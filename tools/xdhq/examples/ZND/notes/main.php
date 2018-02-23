<?php
/*
	Copyright (C) 2018 Claude SIMON (http://q37.info/contact/).

	This file is part of UnJSq.

	UnJSq is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

	UnJSq is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with UnJSq If not, see <http://www.gnu.org/licenses/>.
*/

function getUnJSq() {
	if (getenv("EPEIOS_SRC") === false)
		$zndq_path = realpath(dirname(__FILE__)) . '/';
	else {
		switch (strtoupper(substr(php_uname('s') , 0, 3))) {
		case "WIN":
			$epeios_path = "h:\\hg\\epeios\\";
		break;
		case "LIN":
			$epeios_path = "/home/csimon/hg/epeios/";
		break;
		case "DAR":
			$epeios_path = "/Users/csimon/hg/epeios/";
		break;
		default:
			echo "Unknown OS !!!\n";
			break;
		}

		$zndq_path = $epeios_path . "tools/xdhq/UnJSq/ZND/";
	}

	require( $zndq_path . "UnJSq.php");
}

getUnJSq();

$viewModeElements = ["Pattern", "CreateButton", "DescriptionToggling", "ViewNotes"];

class MyData extends UnJSqDOM {
	public $pattern = "";
	public $hideDescriptions = false;
	public $id = 0;
	public $notes = [
		// First one must be empty; it is used as buffer for new entries.
		[
			'title' => '',
			'description' => '',
		],
		[
			'title' => 'Improve design',
			'description' => 'Tastes and colors... (aka &laquo;CSS aren&rsquo;t my cup of tea...&raquo;)',
		],
		[ 
			'title' => 'Fixing bugs',
			'description' => 'There are bugs ? Really ?',
		],
		[
			'title' => 'Implement new functionalities',
			'description' =>  "Although it&rsquo;s almost perfect..., isn&rsquo;t it ?",
		]
	];
}

function push($note, $id, $tree) {
	$tree->pushTag('Note');
	$tree->putAttribute('id', $id);

	foreach ($note as $key => $value) {
		$tree->pushTag( $key );
		$tree->putValue( $value );
		$tree->popTag();
	}

	$tree->popTag();
}

function handleDescriptions($dom) {
	if ($dom->hideDescriptions)
		$dom->disableElement("ViewDescriptions");
	else
		$dom->enableElement("ViewDescriptions");
}

function displayList( $dom ) {
	global $viewModeElements;
	$tree = new UnJSqTree();
	$i = 1;	// 0 skipped, as it serves as buffer for the new ones.
	$contents = [];

	$tree->pushTag( "Notes" );
	$tree->putAttribute( "HideDescriptions", $dom->hideDescriptions );

	$count = count($dom->notes );

	while ( $i < $count ) {
		if ( strtolower(substr($dom->notes[$i]['title'],0,strlen($dom->pattern))) === $dom->pattern ) {
			push( $dom->notes[$i], $i, $tree);
			$contents["Description." . $i] = $dom->notes[$i]['description'];
		}
		$i++;
	}

	$dom->setLayout( "Notes", $tree, "Notes.xsl");
	$dom->setContents( $contents );
	$dom->enableElements( $viewModeElements );
}

function view($dom) {
	global $viewModeElements;

	$dom->enableElements( $viewModeElements );
	$dom->setContent("Edit." . $dom->id, "");
	$dom->id = -1;
}

function edit($dom, $id) {
	global $viewModeElements;

	$dom->id = intval($id);
	$dom->setLayout("Edit." . $id, new UnJSqTree(), "Note.xsl" );
	$dom->setContents(
			[
				"Title" => $dom->notes[$dom->id]['title'],
				"Description" => $dom->notes[$dom->id]['description'],
			] );
	$dom->disableElements( $viewModeElements );
	$dom->dressWidgets("Notes");
}

function main() {
	$type = null;

	// Uncomment below for desktop interface. Default is web (port: 8080).
	// $type = UnJSq::DESKTOP;

	UnJSq::listen( "Connect", "notes", $type );

	$dom = new MyData();

	while ( true ) {
		switch( $dom->getAction( $id ) ) {
		case "Connect":
			$dom->setLayout( "", new UnJSqTree(), "Main.xsl" );
			displayList( $dom );
			break;
		case "ToggleDescriptions":
			$dom->hideDescriptions = $dom->getContent($id) == "true";
			handleDescriptions($dom);
			break;
		case "Search":
			$dom->pattern = strtolower( $dom->getContent( "Pattern" ) );
			displayList($dom);
			break;
		case "Edit":
			edit( $dom, $dom->getContent($id));
			break;
		case "Delete":
			if ( $dom->confirm("Are you sure you want to delete this entry ?" ) ) {
				array_splice($dom->notes,intval($dom->getContent($id)),1);
				displayList($dom);
			}
			break;
		case "Submit":
			$result = $dom->getContents(["Title", "Description"]);

			$title = trim($result['Title']);
			$description = $result['Description'];

			if ($title != '') {
				$dom->notes[$dom->id]['title'] = $title;
				$dom->notes[$dom->id]['description'] = $description;
				if ($dom->id == 0) {
					array_unshift( $dom->notes, [ 'title' => '', 'description' => '' ] );
					displayList($dom);
				} else {
					$contents["Title." . $dom->id] = $title;
					$contents["Description." . $dom->id] = $description;
					$dom->setContents($contents);
					view($dom);
				}
			} else
				$dom->alert("Title can not be empty !");
			break;
		case "Cancel":
			$result = $dom->getContents(["Title", "Description"]);
			if (($dom->notes[$dom->id]['title'] != $result['Title']) or ($dom->notes[$dom->id]['description'] != $result['Description'])) {
				if ( $dom->confirm("Are you sure you want to cancel your modifications ?" ) )
					view($dom);
			} else
				view($dom);
		break;
		default:
			die( "???" );
			break;
		}
	}
}

main();

?>
