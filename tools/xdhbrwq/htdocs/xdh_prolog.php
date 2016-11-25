<?php
$out = '
<!DOCTYPE html>
<html>
	<head>
		<!-- If modified, report modification in "xdhcefq.html" from "xdhcefq" -->
		<!-- BEGIN -->
		<meta charset="UTF-8" />

		<script src="js/jquery.js"></script>

		<script src="js/jquery-ui/jquery-ui.js"></script>
		<link rel="stylesheet" href="js/jquery-ui/jquery-ui.css"/>

		<script src="js/interact/interact-1.2.4.js"></script>

		<script src="js/Sortable/Sortable.js"></script>

		<script src="js/ckeditor/ckeditor.js"></script>
		<script src="js/ckeditor/adapters/jquery.js"></script>

		<script src="js/jqte/jquery-te-1.4.0.min.js"></script>
		<link rel="stylesheet" href="js/jqte/jquery-te-1.4.0.css"/>

		<script src="js/datepicker/js/bootstrap-datepicker.js"></script>
		<script src="js/datepicker/js/locales/bootstrap-datepicker.fr.js" charset="UTF-8"></script>
		<link rel="stylesheet" href="js/datepicker/css/datepicker.css" />

		<script src="js/timepicker/jquery.timepicker.js"></script>
		<link rel="stylesheet" href="js/timepicker/jquery.timepicker.css" />

		<script src="js/jquery.alphanum/jquery.alphanum.js"></script>

		<script src="js/xdhtml.js"></script>

		<style type="text/css">

  * {
min-height: 0;
min-width: 0;
}
html, body {
	height: 100%;
	padding: 0;
	margin: 0;
}

.vcenter-out, .hcenter {
	display: table;
	height: 100%;
	margin: auto;
}

.vcenter-in {
	display: table-cell;
	vertical-align: middle;
}
			
   /**************************/
   /* Tabs handling section. */
   /**************************/

   /* From http://www.w3schools.com/howto/howto_js_tabs.asp */

   ul.tab {
   list-style-type: none;
   margin: 0;
   padding: 0;
   overflow: hidden;
   border: 1px solid #ccc;
   background-color: #f1f1f1;
   }

   /* Float the list items side by side */
   ul.tab li {float: left;}

   /* Style the links inside the list items */
   ul.tab li a {
   display: inline-block;
   color: black;
   text-align: center;
   padding: 14px 16px;
   text-decoration: none;
   transition: 0.3s;
   font-size: 17px;
   }

   /* Change background color of links on hover */
   ul.tab li a:hover {
   background-color: #ddd;
   }

   ul.tab li a:focus, .active {
   background-color: #ccc;
   }

   /*********************************/
   /* End of Tabs handling section. */
   /*********************************/

   /******************************/
   /* Tree view handling section */
   /******************************/

   /* From http://www.bypeople.com/folders-with-no-js-codepen/ */

   .css-treeview ul,
   .css-treeview li
   {
   padding: 0;
   margin: 0;
   list-style: none;
   }

   .css-treeview input
   {
   position: absolute;
   opacity: 0;
   }

   .css-treeview
   {
   user-select: none;
   }

   .css-treeview a
   {
   color: #35d;
   text-decoration: none;
   }

   .css-treeview a:hover
   {
   text-decoration: underline;
   }

   .css-treeview input + label + ul
   {
   margin: 0 0 0 22px;
   }

   .css-treeview input ~ ul
   {
   display: none;
   }

   .css-treeview label,
   .css-treeview label::before
   {
   cursor: pointer;
   }

   .css-treeview input:disabled + label
   {
   cursor: default;
   opacity: .6;
   }

   .css-treeview input:checked:not(:disabled) ~ ul
   {
   display: block;
   }

   .css-treeview label,
   .css-treeview label::before
   {
   background: url("js/treeview-icons.png") no-repeat;
   }

   .css-treeview label,
   .css-treeview a,
   .css-treeview label::before
   {
   display: inline-block;
   height: 16px;
   line-height: 16px;
   vertical-align: middle;
   }

   .css-treeview label
   {
   background-position: 18px 0;
   }

   .css-treeview label::before
   {
   content: "";
   width: 16px;
   margin: 0 22px 0 0;
   vertical-align: middle;
   background-position: 0 -32px;
   }

   .css-treeview input:checked + label::before
   {
   background-position: 0 -16px;
   }

   @media screen and (-webkit-min-device-pixel-ratio:0)
   {
   .css-treeview
   {
   -webkit-animation: webkit-adjacent-element-selector-bugfix infinite 1s;
   }

   @-webkit-keyframes webkit-adjacent-element-selector-bugfix
   {
   from
   {
   padding: 0;
   }
   to
   {
   padding: 0;
   }
   }
   }
   /*************************************/
   /* End of tree view handling section */
   /*************************************/
		</style>
		<!-- END -->
		<script src="xdhbrwq.js"></script>
		<script>handleQuery("xdh.php?_target=' . $target . '&_language=' . $language . '&_action=' . $action . '")</script>
	</head>
	<body id="Root" data-xdh-onevents="(keypress|About|SC+a)(keypress|Q37Refresh|SC+r)">
			<input type="hidden" id="LoginUserID" value="' . $UserID . '"/>
			<input type="hidden" id="LoginPassword" value="' . $Password . '"/>
	</body>
</html>'
?>