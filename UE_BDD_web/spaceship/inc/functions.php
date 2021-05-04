<?php

function RandomStr($length){

    $alphabet ="0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    $token = substr(str_shuffle(str_repeat($alphabet,$length)),0,$length);

    return $token;
}

function CheckToken($token){
    include 'bdd.php';

    $sql = "SELECT * FROM rubika_users WHERE Token = :Token";
    $req = $db->prepare($sql);
    $req->execute(array(
        "Token" => $token
    ));
    $data = $req->fetch();
    if($req->rowCount() == 1){
        //TODO gestion de la durée de vie du token
        return true;
    }else{
        return false;
    }
}

function CreateNewToken_WithToken($oldToken){
    $newToken = RandomStr(60);
    include 'bdd.php';
    $sql = "UPDATE rubika_users SET Token = :NewToken WHERE Token = :OldToken";
    $req = $db->prepare($sql);
    $req->execute(array(
        "NewToken" => $newToken,
        "OldToken" => $oldToken,
    ));

    return $newToken;
}

function CreateNewToken_WithUsername($Username){
    $newToken = RandomStr(60);
    include 'bdd.php';
    $sql = "UPDATE rubika_users SET Token = :NewToken WHERE Username = :Username";
    $req = $db->prepare($sql);
    $req->execute(array(
        "NewToken" => $newToken,
        "Username" => $Username,
    ));

    return $newToken;
}