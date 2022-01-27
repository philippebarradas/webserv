<!DOCTYPE html>

<html>
    <head>
		<title>Webserv index.php</title>
		<link rel="icon" type="image/x-icon" href="/favicon.ico">
	</head>
    <body bgcolor="white">
        <center><h1>Webserv index.php</h1></center>
        <hr><center>webcerveau/1.0 (Ubuntu)</center>
		<p>PRINT _GET</p>
		<?php
			print_r($_GET);
		?>


		<p>TEST GET</p>
		<form method="GET" action="">
			<input type="text" name="nom"></input>
			<input type="submit" value="phbdg valide"> </input>
		</form>
		<?php
			if (isset($_GET['nom']))
				echo "nom = '" . $_GET['nom'] . "'";
			else
				echo "not define";
		?>

		<p>TEST POST</p>
		<form method="POST" action="" enctype="text/plain">
			<input type="text" name="nom"></input>
			<input type="submit" value="phbdg valide"> </input>
		</form>
		<?php
			if (isset($_POST['nom']))
				echo "nom = '" . $_POST['nom'] . "'";
			else
				echo "not define";
		?>

    </body>
</html>
