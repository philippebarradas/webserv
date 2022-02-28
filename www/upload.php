<!DOCTYPE html>
<html>
<body>

<form action="upload.php" method="post" enctype="multipart/form-data">
	Select image to upload:
	<input type="file" name="userfile" id="userfile">
    <input type="submit" value="Upload Image" name="submit">
</form>

<?php

if (isset($_FILES['userfile']))
{
	$_FILES['userfile'];
	$uploaddir = '/home/user42/Bureau/webserv/www/uploads/';
	$target_path = $uploaddir . basename($_FILES['userfile']['name']);
}

if (isset($_POST['submit']))
{
	if (move_uploaded_file($_FILES['userfile']['tmp_name'], $target_path))
	{
		echo "File is valid, and was successfully uploaded.\n";
	}
	else
	{
		echo "Possible move failed!\n";
	}
}

?>

</body>
</html>
