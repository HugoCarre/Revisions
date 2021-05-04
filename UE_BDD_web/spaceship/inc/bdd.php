<?php
//Connexion à la bdd

$db_server="localhost";
$db_username="root";
$db_paswword="";
$db_dbname="rubika";

try{
    $db = new PDO('mysql:host='.$db_server.';dbname='.$db_dbname.';charset=utf8',$db_username, $db_paswword);
}
catch(PDOException $e)
{
    echo 'Connexion échouée:'.$e->getMessage();
}

?>