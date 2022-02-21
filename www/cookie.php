<!DOCTYPE html>

<html>
	<head>
		<title>Webserv cookie/session</title>
		<link rel="icon" type="image/x-icon" href="/favicon.ico">
	</head>
	<body bgcolor="white">

		<center><h1>Webserv cookie.html</h1></center>
		<hr><center>webcerveau/1.0 (Ubuntu)</center>
		<?php
			if(isset($_COOKIE['cookPseudo']))
			{
				echo "Hello " . $_COOKIE['cookPseudo'] . " !";
			}
			//header("Location: index.php");
		?>

		<?php if ( !isset($_COOKIE['cookPseudo']) ): ?>
		<p>SET COOKIE COOKIE</p>
		<form method="POST" enctype="application/x-www-form-urlencoded">
			<input type="text" name="pseudo"></input>
			<input type="submit" name="submit" value="phbdg valide"></input>
		</form>
		<?php endif; ?>

		<?php
			//@session_start();
			if(isset($_POST['submit']) && !isset($_COOKIE['cookPseudo']))
			{
				$pseudo = $_POST['pseudo'];
				//setcookie('cookPseudo', $pseudo, time() + 3600 * 24 * 365, null, null, false, true);
				setcookie('cookPseudo', $pseudo, time() + 10, null, null, false, true);
				//$_SESSION['division'] = $divId[0];
				unset($_POST['submit']);
				//header("Refresh: 5");
				while (list($var,$value) = each($_SERVER))
				echo "$var => $value <br />";
			}
			//header("Location: index.php");
		?>

	</body>
</html>

