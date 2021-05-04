<?php 

include "inc/functions.php";

$connexion =false;
if( !empty($_POST["Token"]) ){
    $connexion = false;

    if(CheckToken($_POST["Token"])){
        $connexion = true;
    }
}

if($connexion)
{
    include "inc/bdd.php";

    $sql = "SELECT * FROM rubika_users WHERE Token = :Token";
    $req = $db->prepare($sql);
    $req->execute(array(
        "Token" => $_POST["Token"]
    ));
    $data = $req->fetch();

    if($req->rowCount() == 1){
        $ue4_Answer = array(
            "IsConnected" => true,
            "PlayerData" => array(
                "Id" => $data["Id"],
                "Name" => $data["Name"],
                "Firstname" => $data["Firstname"],
                "Username" => $data["Username"],
                "Level" => $data["Level"]
                )
            );
    }
}else{
    $ue4_Answer = array(
        "IsConnected" => false,
        "Error" => "Problème de connexion"
        );
}


print json_encode($ue4_Answer,JSON_PRETTY_PRINT | JSON_UNESCAPED_UNICODE);