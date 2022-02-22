<!DOCTYPE html>

<html>
	<head>
		<title>Webserv cookie test</title>
		<link rel="icon" type="image/x-icon" href="/favicon.ico">
	</head>
	<body bgcolor="white">

		<center><h1>Webserv cookie.php</h1></center>
		<hr><center>webcerveau/1.0 (Ubuntu)</center>

		<?php if ( !isset($_COOKIE['cookPseudo']) ): ?>
		<p>SET COOKIE:</p>
		<form method="POST" enctype="application/x-www-form-urlencoded">
			<input type="text" name="pseudo"></input>
			<input type="submit" name="sub_pseudo" value="phbdg valide"></input>
		</form>
		<?php else: ?>
			<?php echo "Hello " . $_COOKIE['cookPseudo'] . " !";?>
			<form method="POST" enctype="application/x-www-form-urlencoded">
				<input type="submit" name="delete_button" value="Delete cookie">
			</form>
		<?php endif; ?>

		<?php
			//@session_start();
			if(isset($_POST['sub_pseudo']) && !isset($_COOKIE['cookPseudo']))
			{
				$pseudo = $_POST['pseudo'];
				//setcookie('cookPseudo', $pseudo, time() + 3600 * 24 * 365, null, null, false, true);
				setcookie('cookPseudo', $pseudo, time() + 3600 * 24 * 365, null, null, false, true);
				//$_SESSION['division'] = $divId[0];
				//unset($_POST['submit']);
				header("Refresh: 0");
				//while (list($var,$value) = each($_SERVER))
				//echo "$var => $value <br />";
			}
			if(isset($_POST['delete_button']))
			{
				unset($_COOKIE['cookPseudo']);
				setcookie('cookPseudo', null, -1, '/');
				header("Refresh: 0");
				//while (list($var,$value) = each($_SERVER))
				//echo "$var => $value <br />";
			}
			//header("Location: index.php");
		?>

	</body>
</html>

