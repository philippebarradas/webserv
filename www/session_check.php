<!DOCTYPE html>

<?php
@session_start();
?>

<html>
	<head>
		<title>Webserv session check</title>
		<link rel="icon" type="image/x-icon" href="/favicon.ico">
	</head>
	<body bgcolor="white">

		<center><h1>Webserv session_check.php</h1></center>
		<hr><center>webcerveau/1.0 (Ubuntu)</center>

		<?php
		if ( isset($_SESSION['sessPseudo']) ): ?>
			<p>Hello <?php $_SESSION['sessPseudo'] ?> !</p>
		<?php else: ?>
			<p>Session was not define.</p>
			<?php endif; ?>
		<p>Return to <a href="/session.php">session.php</a>.</p>

	</body>
</html>

