<!DOCTYPE html>
<html>
<body>
    <?php
    // In PHP versions earlier than 4.1.0, $HTTP_POST_FILES should be used instead
    // of $_FILES.

    $uploaddir = '/home/user42/Bureau/webserv/www/uploads';
    $uploadfile = $uploaddir . basename($_FILES['userfile']['name']);


    echo '<pre>';
    if (move_uploaded_file($_FILES['userfile']['tmp_name'], $uploadfile)) {
        echo "File is valid, and was successfully uploaded.\n";
    } else {
        echo "Possible file upload attack!\n";
    }

    echo 'Here is some more debugging info:';
    print_r($_FILES);

    print "</pre>";
    ?>
<form action="upload.php" method="post" enctype="multipart/form-data">
	Select image to upload:
	<input type="file" name="userfile" id="userfile">
	<input type="submit" value="Upload Image" name="submit">
</form>

<img id="userfile"> </img>

</body>
</html>

