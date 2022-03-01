<!DOCTYPE html>

<html>
	<head>
		<title>Webserv form.html</title>
	</head>
	<body bgcolor="green">
		<center><h1>Webserv form.html</h1></center>
		<hr><center>webcerveau/1.0 (Ubuntu)</center>

		<?php if ( !isset($_GET['name_get']) ): ?>
		<p>SET GET: </p>
		<form method="GET" action="">
			<input type="text" name="name_get"></input>
			<input type="submit" value="phbdg valide"> </input>
		</form>
		<?php else: ?>
			<p>GET is set to <?php echo $_GET['name_get']; ?> !</p>
		<?php endif; ?>

		<?php if ( !isset($_POST['name_post']) ): ?>
		<p>SET POST: </p>
		<form method="POST" action="" enctype="application/x-www-form-urlencoded">
			<input type="text" name="name_post"></input>
			<input type="submit" value="phbdg valide"> </input>
		</form>
		<?php else: ?>
			<p>POST is set to <?php echo $_POST['name_post']; ?> !</p>
		<?php endif; ?>
	</body>
</html>

