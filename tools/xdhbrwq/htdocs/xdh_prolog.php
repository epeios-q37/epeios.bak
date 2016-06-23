<?php
$out = '
<!DOCTYPE html>
<html>
	<head>
		<meta charset="UTF-8" />
		<script src="js/jquery.js"></script>
		<script src="js/jquery-ui/jquery-ui.js"></script>
		<script src="js/interact/interact-1.2.4.js"></script>
		<script src="js/Sortable/Sortable.js"></script>
		<script src="js/ckeditor/ckeditor.js"></script>
		<script src="js/ckeditor/adapters/jquery.js"></script>
		<script src="js/jqte/jquery-te-1.4.0.min.js"></script>
		<script src="js/datepicker/js/bootstrap-datepicker.js"></script>
		<script src="js/datepicker/js/locales/bootstrap-datepicker.fr.js" charset="UTF-8"></script>
		<script src="js/timepicker/jquery.timepicker.js"></script>
		<script src="js/jquery.alphanum/jquery.alphanum.js"></script>
		<script src="js/Zebra_Dialog/public/javascript/zebra_dialog.js"></script>
		<script src="js/xdhtml.js"></script>
		<script src="xdhbrwq.js"></script>
		<script>handleQuery("xdh.php?_target=' . $target . '&_language=' . $language . '&_action=' . $action . '")</script>
	</head>
	<body>
			<input type="hidden" id="AuthLogin" value="Hello"/>
			<input type="hidden" id="AuthPassword" value="toto"/>
	</body>
</html>'
?>