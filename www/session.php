<!DOCTYPE html>

<?php
@session_start();
?>

<html>
	<head>
		<title>Webserv session test</title>
		<link rel="icon" type="image/x-icon" href="/favicon.ico">
	</head>
	<body bgcolor="white">

		<center><h1>Webserv session.php</h1></center>
		<hr><center>webcerveau/1.0 (Ubuntu)</center>

		<?php
		if ( !isset($_SESSION['sessPseudo']) ): ?>
		<p>SET SESSION:</p>
		<form method="POST" enctype="application/x-www-form-urlencoded">
			<input type="text" name="pseudo"></input>
			<input type="submit" name="sub_pseudo" value="phbdg valide"></input>
		</form>
		<?php else: ?>
			<?php echo "Hello " . $_SESSION['sessPseudo'] . " !";?>
			<form method="POST" enctype="application/x-www-form-urlencoded">
				<input type="submit" name="delete_button" value="Delete session">
			</form>
			<p>check session on another <a href="/">page </a></p>
		<?php endif; ?>

		<?php
		if(isset($_POST['sub_pseudo']) && !isset($_SESSION['sessPseudo']))
		{
			$pseudo = $_POST['pseudo'];

			echo "deff pseudo = " . $_POST['pseudo'];

			$_SESSION['sessPseudo'] = $pseudo;
			header("Refresh: ");
			//while (list($var,$value) = each($_SERVER))
			//echo "$var => $value <br />";
		}
		if(isset($_POST['delete_button']))
		{
			unset($_SESSION['sessPseudo']);
			header("Refresh: 0");
			//while (list($var,$value) = each($_SERVER))
			//echo "$var => $value <br />";
		}
		?>

	</body>
</html>

