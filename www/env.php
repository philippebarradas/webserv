<?php

var_dump($_POST);

<<<<<<< HEAD
=======
if(isset($_FILES['avatar']))
{ 
     $dossier = '/home/user42/Bureau/webserv/www/upload/';
     $fichier = basename($_FILES['avatar']['name']);

    echo $fichier;

     if(move_uploaded_file($_FILES['avatar']['tmp_name'], $dossier . $fichier)) //Si la fonction renvoie TRUE, c'est que ça a fonctionné...
     {
          echo 'Upload effectué avec succès !';
     }
     else //Sinon (la fonction renvoie FALSE).
     {
          echo 'Echec de l\'upload !';
     }
}
else
    echo 'NO AVATAR';
>>>>>>> 8d51e34053bf07a8af856831e591d22b5021200b

?>