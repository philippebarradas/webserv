<?php
	while (list($var,$value) = each($_SERVER))
		echo "$var => $value <br />";
	 echo "POST VAR: <br />";
	if (isset($_POST['nom']))
		echo "nom = '" . $_POST['nom'] . "'<br /> <br />";
	else
		echo "not define <br />";
?>



<?php var_dump($_POST);?>

<?php print_r($_SERVER);?>
